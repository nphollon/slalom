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

  %isEmptyAfterRemoval = call i1 @isEmpty(%Queue* %q)
  br i1 %isEmptyAfterRemoval, label %setTailToLast, label %freeOldHead

setTailToLast:
  call void @setTail(%Queue* %q, %QueueNode* @.LAST)
  br label %freeOldHead

freeOldHead:
  call void @qnSalvage(%QueueNode* %head)
  ret %Function* %data

returnNull:
  ret %Function* null
}

define void @transfer(%Queue* %receiver, %Queue* %sender, %Index %n) {
entry:
  switch %Index %n, label %splitQueue [ %Index 0, label %exit ]

splitQueue:
  %receiverIsEmpty = call i1 @isEmpty(%Queue* %receiver)

  %senderHead = call %QueueNode* @getHead(%Queue* %sender)
  %receiverNewTail = call %QueueNode* @getLink(%QueueNode* %senderHead, %Index %n)
  %senderNewHead = call %QueueNode* @getNext(%QueueNode* %receiverNewTail)

  call void @setHead(%Queue* %sender, %QueueNode* %senderNewHead)
  call void @decreaseLength(%Queue* %sender, %Index %n)
  call void @increaseLength(%Queue* %receiver, %Index %n)

  br i1 %receiverIsEmpty, label %joinAtHead, label %joinAtTail

joinAtHead:
  call void @setHead(%Queue* %receiver, %QueueNode* %senderHead)
  br label %setNewTail

joinAtTail:
  %receiverTail = call %QueueNode* @getTail(%Queue* %receiver)
  call void @setNext(%QueueNode* %receiverTail, %QueueNode* %senderHead)
  br label %setNewTail

setNewTail:
  call void @setTail(%Queue* %receiver, %QueueNode* %receiverNewTail)
  call void @setNext(%QueueNode* %receiverNewTail, %QueueNode* @.LAST)
  br label %exit
exit:
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

define void @qDestroy(%Queue* %q) {
  %head = call %QueueNode* @getHead(%Queue* %q)
  call void @qnDestroy(%QueueNode* %head)
  %qCast = bitcast %Queue* %q to i8*
  call void @free(i8* %qCast) nounwind
  ret void
}