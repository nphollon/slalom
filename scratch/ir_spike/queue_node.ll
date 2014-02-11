@.LAST = private unnamed_addr constant %QueueNode <{ %Function* null, %QueueNode* null }>

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

define void @setNext(%QueueNode* %qn, %QueueNode* %newNext) {
  %next_p = call %QueueNode** @getNextPointer(%QueueNode* %qn)
  store %QueueNode* %newNext, %QueueNode** %next_p
  ret void
}

define %QueueNode* @getLink(%QueueNode* %qn, %Index %i) {
entry:
  %isThisNode = icmp eq %Index %i, 1
  br i1 %isThisNode, label %returnThis, label %iterate
returnThis:
  ret %QueueNode* %qn
iterate:
  %iNext = sub nuw %Index %i, 1
  %next = call %QueueNode* @getNext(%QueueNode* %qn)
  %toReturn = tail call %QueueNode* @getLink(%QueueNode* %next, %Index %iNext)
  ret %QueueNode* %toReturn
}

define %QueueNode* @createNode(%Function* %f) {
  %qn_size = load i32* @.QUEUENODE_SIZE
  %qn_i8 = tail call noalias i8* @malloc(i32 %qn_size) nounwind
  %qn = bitcast i8* %qn_i8 to %QueueNode*

  %data_p = call %Function** @getDataPointer(%QueueNode* %qn)
  store %Function* %f, %Function** %data_p

  call void @setNext(%QueueNode* %qn, %QueueNode* @.LAST)
  ret %QueueNode* %qn
}

define void @qnDestroy(%QueueNode* %qn) {
entry:
  %isLast = icmp eq %QueueNode* %qn, @.LAST
  br i1 %isLast, label %exit, label %recursiveFree

recursiveFree:
  %next = call %QueueNode* @getNext(%QueueNode* %qn)

  %data = call %Function* @getData(%QueueNode* %qn)
  call void @fDestroy(%Function* %data)
  call void @qnSalvage(%QueueNode* %qn)

  tail call void @qnDestroy(%QueueNode* %next)
  ret void

exit:
  ret void
}

define void @qnSalvage(%QueueNode* %qn) {
  %qnCast = bitcast %QueueNode* %qn to i8*
  call void @free(i8* %qnCast) nounwind
  ret void 
}