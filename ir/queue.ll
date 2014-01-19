@.LAST = private unnamed_addr constant %QueueNode <{ %Function* null, %QueueNode* null }>

declare noalias i8* @malloc(i32) nounwind
declare void @free(i8*) nounwind

define %Function* @getArgument(%QueueNode* %qn) {
entry:
  %argument_p = getelementptr %QueueNode* %qn, i64 0, i32 0
  %argument = load %Function** %argument_p
  ret %Function* %argument
}

define %QueueNode* @getNext(%QueueNode* %qn) {
entry:
  %next_p = getelementptr %QueueNode* %qn, i64 0, i32 1
  %next = load %QueueNode** %next_p
  ret %QueueNode* %next
}

define %Index @getLength(%Queue* %q) {
entry:
  %len_p = getelementptr %Queue* %q, i64 0, i32 0
  %len = load %Index* %len_p
  ret %Index %len  
}

define void @setLength(%Queue* %q, %Index %newLength) {
entry:
  %len_p = getelementptr %Queue* %q, i64 0, i32 0
  store %Index %newLength, %Index* %len_p
  ret void
}

define %QueueNode* @getHead(%Queue* %q) {
entry:
  %head_p = getelementptr %Queue* %q, i64 0, i32 1
  %head = load %QueueNode** %head_p
  ret %QueueNode* %head
}

define void @setHead(%Queue* %q, %QueueNode* %newHead) {
entry:
  %head_p = getelementptr %Queue* %q, i64 0, i32 1
  store %QueueNode* %newHead, %QueueNode** %head_p
  ret void
}

define %QueueNode* @getTail(%Queue* %q) {
entry:
  %tail_p = getelementptr %Queue* %q, i64 0, i32 2
  %tail = load %QueueNode** %tail_p
  ret %QueueNode* %tail
}

define void @setTail(%Queue* %q, %QueueNode* %newTail) {
entry:
  %tail_p = getelementptr %Queue* %q, i64 0, i32 2
  store %QueueNode* %newTail, %QueueNode** %tail_p
  ret void
}

define %Function* @dequeue(%Queue*) {
entry:
  ret %Function* null
}

define %Queue* @createEmptyQueue() {
entry:
  %q_size = load i32* @.QUEUE_SIZE
  %q_i8 = tail call noalias i8* @malloc(i32 %q_size) nounwind
  %q = bitcast i8* %q_i8 to %Queue*
  call void @setLength(%Queue* %q, %Index 0)
  call void @setHead(%Queue* %q, %QueueNode* @.LAST)
  call void @setTail(%Queue* %q, %QueueNode* @.LAST)
  ret %Queue* %q
}
