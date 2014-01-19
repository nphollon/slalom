@.LAST = private unnamed_addr constant %QueueNode <{ %Function* null, %QueueNode* null }>

declare noalias i8* @malloc(i32) nounwind
declare void @free(i8*) nounwind

define %Function* @getData(%QueueNode* %qn) {
  %data_p = call %Function** @getDataPointer(%QueueNode* %qn)
  %data = load %Function** %data_p
  ret %Function* %data
}

define void @setData(%QueueNode* %qn, %Function* %newData) {
  %data_p = call %Function** @getDataPointer(%QueueNode* %qn)
  store %Function* %newData, %Function** %data_p
  ret void
}

define %QueueNode* @getNext(%QueueNode* %qn) {
  %next_p = call %QueueNode** @getNextPointer(%QueueNode* %qn)
  %next = load %QueueNode** %next_p
  ret %QueueNode* %next
}

define void @setNext(%QueueNode* %qn, %QueueNode* %newNext) {
  %next_p = call %QueueNode** @getNextPointer(%QueueNode* %qn)
  store %QueueNode* %newNext, %QueueNode** %next_p
  ret void
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

define i1 @isEmpty(%Queue* %q) {
  %length = call %Index @getLength(%Queue* %q)
  %isEmpty = icmp eq %Index %length, 0
  ret i1 %isEmpty
};

define void @incrementLength(%Queue* %q) {
  %length = call %Index @getLength(%Queue* %q)
  %newLength = add nuw %Index %length, 1
  call void @setLength(%Queue* %q, %Index %newLength)
  ret void
}

define %Function* @dequeue(%Queue*) {
  ret %Function* null
}

define void @enqueue(%Queue* %q, %Function* %f) {
entry:
  %qn = call %QueueNode* @createNode(%Function* %f)

  %isEmpty = call i1 @isEmpty(%Queue* %q)
  br i1 %isEmpty, label %setHead, label %setTailNext

setHead:
  call void @setHead(%Queue* %q, %QueueNode* %qn)
  br label %updateTail

setTailNext:
  %tail = call %QueueNode* @getTail(%Queue* %q)
  call void @setNext(%QueueNode* %tail, %QueueNode* %qn)
  br label %updateTail

updateTail:
  call void @setTail(%Queue* %q, %QueueNode* %qn)
  call void @incrementLength(%Queue* %q)
  ret void
}

define %QueueNode* @createNode(%Function* %f) {
  %qn_size = load i32* @.QUEUENODE_SIZE
  %qn_i8 = tail call noalias i8* @malloc(i32 %qn_size) nounwind
  %qn = bitcast i8* %qn_i8 to %QueueNode*
  call void @setData(%QueueNode* %qn, %Function* %f)
  call void @setNext(%QueueNode* %qn, %QueueNode* @.LAST)
  ret %QueueNode* %qn
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