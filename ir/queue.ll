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

define void @increaseLength(%Queue* %q, %Index %change) {
  %length = call %Index @getLength(%Queue* %q)
  %newLength = add nuw %Index %length, %change
  call void @setLength(%Queue* %q, %Index %newLength)
  ret void
}

define void @decreaseLength(%Queue* %q, %Index %change) {
  %length = call %Index @getLength(%Queue* %q)
  %newLength = sub nuw %Index %length, %change
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
  call void @increaseLength(%Queue* %q, %Index 1)
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
  call void @decreaseLength(%Queue* %q, %Index 1)

  %headCast = bitcast %QueueNode* %head to i8*
  call void @free(i8* %headCast) nounwind

  ret %Function* %data

returnNull:
  ret %Function* null
}

define %Queue* @cut(%Queue* %q, %Index %cutLength) {
entry:
  %cutQ = call %Queue* @createEmptyQueue()
  %isEmptyCut = icmp eq %Index %cutLength, 0
  br i1 %isEmptyCut, label %exit, label %splitQueue

splitQueue:
  %cutHead = call %QueueNode* @getHead(%Queue* %q)
  %cutTail = call %QueueNode* @getLink(%QueueNode* %cutHead, %Index %cutLength)
  %qNewHead = call %QueueNode* @getNext(%QueueNode* %cutTail)

  call void @setHead(%Queue* %q, %QueueNode* %qNewHead)
  call void @decreaseLength(%Queue* %q, %Index %cutLength)

  call void @setHead(%Queue* %cutQ, %QueueNode* %cutHead)
  call void @setTail(%Queue* %cutQ, %QueueNode* %cutTail)
  call void @setNext(%QueueNode* %cutTail, %QueueNode* @.LAST)
  call void @setLength(%Queue* %cutQ, %Index %cutLength)

  br label %exit
exit:
  ret %Queue* %cutQ
}

define void @paste(%Queue* %front, %Queue* %back) {
  %lengthBack = call %Index @getLength(%Queue* %back)
  call void @increaseLength(%Queue* %front, %Index %lengthBack)

  %frontTail = call %QueueNode* @getTail(%Queue* %front)
  %backHead = call %QueueNode* @getHead(%Queue* %back)
  call void @setNext(%QueueNode* %frontTail, %QueueNode* %backHead)

  %backTail = call %QueueNode* @getTail(%Queue* %back)
  call void @setTail(%Queue* %front, %QueueNode* %backTail)
  
  call void @qSalvage(%Queue* %back)
  ret void
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

define void @copyDataToQueue(%Queue* %q, %QueueNode* %qn) {
entry:
  %isLast = icmp eq %QueueNode* %qn, @.LAST
  br i1 %isLast, label %exit, label %copyData
copyData:
  %data = call %Function* @getData(%QueueNode* %qn)
  %dataCopy = call %Function* @fCopy(%Function* %data)
  call void @enqueue(%Queue* %q, %Function* %dataCopy)
  %next = call %QueueNode* @getNext(%QueueNode* %qn)
  call void @copyDataToQueue(%Queue* %q, %QueueNode* %next)
  br label %exit
exit:
  ret void  
}

define %Queue* @qCopy(%Queue* %original) {
  %copy = call %Queue* @createEmptyQueue()
  %originalHead = call %QueueNode* @getHead(%Queue* %original)
  call void @copyDataToQueue(%Queue* %copy, %QueueNode* %originalHead)
  ret %Queue* %copy
}

define void @qSalvage(%Queue* %q) {
  %qCast = bitcast %Queue* %q to i8*
  call void @free(i8* %qCast) nounwind
  ret void
}

define void @qDestroy(%Queue* %q) {
  %head = call %QueueNode* @getHead(%Queue* %q)
  call void @qnDestroy(%QueueNode* %head)
  call void @qSalvage(%Queue* %q)
  ret void
}