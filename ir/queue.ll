@.LAST = private unnamed_addr constant %QueueNode <{ %Function* null, %QueueNode* null }>

declare noalias i8* @malloc(i32) nounwind
declare void @free(i8*) nounwind

define %Function* @getData(%QueueNode* %qn) {
  %data_p = call %Function** @getDataPointer(%QueueNode* %qn)
  %data = load %Function** %data_p
  ret %Function* %data
}

define %QueueNode* @getNext(%QueueNode* %qn) {
  %next_p = call %QueueNode** @getNextPointer(%QueueNode* %qn)
  %next = load %QueueNode** %next_p
  ret %QueueNode* %next
}

define %Index @getLength(%Queue* %q) {
  %len_p = call %Index* @getLengthPointer(%Queue* %q)
  %len = load %Index* %len_p
  ret %Index %len  
}

define void @setLength(%Queue* %q, %Index %newLength) {
  %len_p = call %Index* @getLengthPointer(%Queue* %q)
  store %Index %newLength, %Index* %len_p
  ret void
}

define %QueueNode* @getHead(%Queue* %q) {
  %head_p = call %QueueNode** @getHeadPointer(%Queue* %q)
  %head = load %QueueNode** %head_p
  ret %QueueNode* %head
}

define void @setHead(%Queue* %q, %QueueNode* %newHead) {
  %head_p = call %QueueNode** @getHeadPointer(%Queue* %q)
  store %QueueNode* %newHead, %QueueNode** %head_p
  ret void
}

define %QueueNode* @getTail(%Queue* %q) {
  %tail_p = call %QueueNode** @getTailPointer(%Queue* %q)
  %tail = load %QueueNode** %tail_p
  ret %QueueNode* %tail
}

define void @setTail(%Queue* %q, %QueueNode* %newTail) {
  %tail_p = call %QueueNode** @getTailPointer(%Queue* %q)
  store %QueueNode* %newTail, %QueueNode** %tail_p
  ret void
}

define %Function* @dequeue(%Queue*) {
  ret %Function* null
}

define %Queue* @createEmptyQueue() {
  %q_size = load i32* @.QUEUE_SIZE
  %q_i8 = tail call noalias i8* @malloc(i32 %q_size) nounwind
  %q = bitcast i8* %q_i8 to %Queue*
  call void @setLength(%Queue* %q, %Index 0)
  call void @setHead(%Queue* %q, %QueueNode* @.LAST)
  call void @setTail(%Queue* %q, %QueueNode* @.LAST)
  ret %Queue* %q
}