define %Index @getArity(%Function* %f) {
  %arity_p = call %Index* @getArityPointer(%Function* %f)
  %arity = load %Index* %arity_p
  ret %Index %arity
}

define %Queue* @getArguments(%Function* %f) {
  %args_p = call %Queue** @getArgumentsPointer(%Function* %f)
  %args = load %Queue** %args_p
  ret %Queue* %args
}

define %Function* @substitute(%Queue* %q) {
  %applicator1 = call %Function* @dequeue(%Queue* %q)
  %applicator2 = call %Function* @dequeue(%Queue* %q)

  %input1 = call %Function* @dequeue(%Queue* %q)
  %input2 = call %Function* @fCopy(%Function* %input1)

  %eval1 = call %Function* @apply(%Function* %applicator1, %Function* %input1)
  %eval2 = call %Function* @apply(%Function* %applicator2, %Function* %input2)

  %result = call %Function* @apply(%Function* %eval1, %Function* %eval2)
  ret %Function* %result
}

define %Function* @evaluate(%Function* %f) {
entry:
  %args = call %Queue* @getArguments(%Function* %f)
  %totalArgCount = call %Index @getLength(%Queue* %args)
  %arity = call %Index @getArity(%Function* %f)
  %readyForEval = icmp uge %Index %totalArgCount, %arity
  br i1 %readyForEval, label %evalF, label %doNothing

evalF:
  %bodyPointer = call %Body* @getBodyPointer(%Function* %f)
  %body = load %Body* %bodyPointer

  %argsToConsume = call %Queue* @createEmptyQueue()
  call void @transfer(%Queue* %argsToConsume, %Queue* %args, %Index %arity)
  %result = call %Function* %body(%Queue* %argsToConsume)
  call void @qDestroy(%Queue* %argsToConsume)

  %remainingArgCount = sub nuw %Index %totalArgCount, %arity
  %resultArgs = call %Queue* @getArguments(%Function* %result)
  call void @transfer(%Queue* %resultArgs, %Queue* %args, %Index %remainingArgCount)

  call void @fDestroy(%Function* %f)
  %finalResult = call %Function* @evaluate(%Function* %result)
  ret %Function* %finalResult

doNothing:
  ret %Function* %f
}

define %Function* @apply(%Function* %applicator, %Function* %input) {
  %args = call %Queue* @getArguments(%Function* %applicator)
  call void @enqueue(%Queue* %args, %Function* %input)
  %result = call %Function* @evaluate(%Function* %applicator)
  ret %Function* %result
}

define %Function* @createFunction(%Body %body, %Index %arity, %Queue* %arguments) {
  %f_size = load i32* @.FUNCTION_SIZE
  %f_i8 = tail call noalias i8* @malloc(i32 %f_size) nounwind
  %f = bitcast i8* %f_i8 to %Function*

  %body_p = call %Body* @getBodyPointer(%Function* %f)
  store %Body %body, %Body* %body_p

  %arity_p = call %Index* @getArityPointer(%Function* %f)
  store %Index %arity, %Index* %arity_p

  %args_p = call %Queue** @getArgumentsPointer(%Function* %f)
  store %Queue* %arguments, %Queue** %args_p

  ret %Function* %f
}

define %Function* @createBareFunction(%Body %body, %Index %arity) {
  %emptyArgs = call %Queue* @createEmptyQueue()
  %f = call %Function* @createFunction(%Body %body, %Index %arity, %Queue* %emptyArgs)
  ret %Function* %f
}

define %Function* @createICombinator() {
  %f = call %Function* @createBareFunction(%Body @dequeue, %Index 1)
  ret %Function* %f
}

define %Function* @createKCombinator() {
  %f = call %Function* @createBareFunction(%Body @dequeue, %Index 2)
  ret %Function* %f
}

define %Function* @createSCombinator() {
  %f = call %Function* @createBareFunction(%Body @substitute, %Index 3)
  ret %Function* %f
}

define %Function* @fCopy(%Function* %original) {
  %arity = call %Index @getArity(%Function* %original)

  %bodyP = call %Body* @getBodyPointer(%Function* %original)
  %body = load %Body* %bodyP

  %args = call %Queue* @getArguments(%Function* %original)
  %argsHead = call %QueueNode* @getHead(%Queue* %args)
  %argsCopy = call %Queue* @createEmptyQueue()
  call void @copyDataToQueue(%Queue* %argsCopy, %QueueNode* %argsHead)

  %copy = call %Function* @createFunction(%Body %body, %Index %arity, %Queue* %argsCopy)
  ret %Function* %copy
}

define void @fDestroy(%Function* %f) {
  %args = call %Queue* @getArguments(%Function* %f)
  call void @qDestroy(%Queue* %args)
  %fCast = bitcast %Function* %f to i8*
  call void @free(i8* %fCast) nounwind
  ret void
}