define void @setBody(%Function* %f, %Body %newBody) {
  %body_p = call %Body* @getBodyPointer(%Function* %f)
  store %Body %newBody, %Body* %body_p
  ret void
}

define %Index @getArity(%Function* %f) {
  %arity_p = call %Index* @getArityPointer(%Function* %f)
  %arity = load %Index* %arity_p
  ret %Index %arity
}

define void @setArity(%Function* %f, %Index %newArity) {
  %arity_p = call %Index* @getArityPointer(%Function* %f)
  store %Index %newArity, %Index* %arity_p
  ret void
}

define %Queue* @getArguments(%Function* %f) {
  %args_p = call %Queue** @getArgumentsPointer(%Function* %f)
  %args = load %Queue** %args_p
  ret %Queue* %args
}

define void @setArguments(%Function* %f, %Queue* %args) {
  %args_p = call %Queue** @getArgumentsPointer(%Function* %f)
  store %Queue* %args, %Queue** %args_p
  ret void
}

define void @addArgument(%Function* %app, %Function* %arg) {
  %args = call %Queue* @getArguments(%Function* %app)
  call void @enqueue(%Queue* %args, %Function* %arg)
  ret void
}

define %Function* @substitute(%Queue*) {
  ret %Function* null
}

define %Function* @createFunction(%Body %body, %Index %arity, %Queue* %arguments) {
  %f_size = load i32* @.FUNCTION_SIZE
  %f_i8 = tail call noalias i8* @malloc(i32 %f_size) nounwind
  %f = bitcast i8* %f_i8 to %Function*

  call void @setBody(%Function* %f, %Body %body)
  call void @setArity(%Function* %f, %Index %arity)
  call void @setArguments(%Function* %f, %Queue* %arguments)

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
  %argsCopy = call %Queue* @qCopy(%Queue* %args)

  %copy = call %Function* @createFunction(%Body %body, %Index %arity, %Queue* %argsCopy)
  ret %Function* %copy
}

define void @fDestroy(%Function* %f) {
  ; Free arguments queue
  %args = call %Queue* @getArguments(%Function* %f)
  call void @qDestroy(%Queue* %args)
  ; Free function
  %fCast = bitcast %Function* %f to i8*
  call void @free(i8* %fCast) nounwind
  ret void
}