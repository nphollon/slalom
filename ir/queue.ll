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

define void @decrementLength(%Queue* %q) {
  %length = call %Index @getLength(%Queue* %q)
  %newLength = sub nuw %Index %length, 1
  call void @setLength(%Queue* %q, %Index %newLength)
  ret void
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

define %Function* @dequeue(%Queue* %q) {
entry:
  %isEmpty = call i1 @isEmpty(%Queue* %q)
  br i1 %isEmpty, label %returnNull, label %removeHead

removeHead:
  %head = call %QueueNode* @getHead(%Queue* %q)
  %data = call %Function* @getData(%QueueNode* %head)
  %next = call %QueueNode* @getNext(%QueueNode* %head)

  call void @setHead(%Queue* %q, %QueueNode* %next)
  call void @decrementLength(%Queue* %q)

  %headCast = bitcast %QueueNode* %head to i8*
  call void @free(i8* %headCast) nounwind

  ret %Function* %data

returnNull:
  ret %Function* null
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

define void @qnDestroy(%QueueNode* %qn) {
entry:
  %isLast = icmp eq %QueueNode* %qn, @.LAST
  br i1 %isLast, label %exit, label %recursiveFree

recursiveFree:
  %next = call %QueueNode* @getNext(%QueueNode* %qn)

  %data = call %Function* @getData(%QueueNode* %qn)
  %qnCast = bitcast %QueueNode* %qn to i8*
  call void @fDestroy(%Function* %data)
  call void @free(i8* %qnCast) nounwind

  tail call void @qnDestroy(%QueueNode* %next)
  ret void

exit:
  ret void
}

define void @qDestroy(%Queue* %q) {
  ; Free nodes recursively from head
  %head = call %QueueNode* @getHead(%Queue* %q)
  call void @qnDestroy(%QueueNode* %head)

  ; Free queue
  %qCast = bitcast %Queue* %q to i8*
  call void @free(i8* %qCast) nounwind
  ret void
}