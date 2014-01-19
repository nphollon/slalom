define %Body* @getBody(%Function* %f) {
entry:
  %body = getelementptr %Function* %f, i64 0, i32 0
  ret %Body* %body
}

define void @setBody(%Function* %f, %Body %newBody) {
entry:
  %body_p = getelementptr %Function* %f, i64 0, i32 0
  store %Body %newBody, %Body* %body_p
  ret void
}

define %Index @getArity(%Function* %f) {
entry:
  %arity_p = getelementptr %Function* %f, i64 0, i32 1
  %arity = load %Index* %arity_p
  ret %Index %arity
}

define void @setArity(%Function* %f, %Index %newArity) {
entry:
  %arity_p = getelementptr %Function* %f, i64 0, i32 1
  store %Index 1, %Index* %arity_p
  ret void
}

define %Queue* @getArguments(%Function* %f) {
entry:
  %args_p = getelementptr %Function* %f, i64 0, i32 2
  %args = load %Queue** %args_p
  ret %Queue* %args
}

define void @setArgs(%Function* %f, %Queue* %newArgs) {
entry:
  %args_p = getelementptr %Function* %f, i64 0, i32 2
  store %Queue* %newArgs, %Queue** %args_p
  ret void
}

define %Function* @createICombinator() {
entry:
  %f_size = load i32* @.FUNCTION_SIZE
  %f_i8 = tail call noalias i8* @malloc(i32 %f_size) nounwind
  %f = bitcast i8* %f_i8 to %Function*

  call void @setBody(%Function* %f, %Body @dequeue)

  call void @setArity(%Function* %f, %Index 1)

  %args = call %Queue* @createEmptyQueue()
  call void @setArgs(%Function* %f, %Queue* %args)

  ret %Function* %f
}