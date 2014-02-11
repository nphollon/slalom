declare noalias i8* @malloc(i32) nounwind
declare void @free(i8*) nounwind

; assume sizeof(pointer) = sizeof(i64) = 8

%Body = type %Function*(%Queue*)*

; sizeof(index) = sizeof(i8) = 1
%Index = type i8


; sizeof(function) = sizeof(index) + 2 * sizeof(pointer) = 17
%Function = type <{ %Body, %Index, %Queue* }>
@.FUNCTION_SIZE = private unnamed_addr constant i32 17

define %Body* @getBodyPointer(%Function* %f) {
  %p = getelementptr %Function* %f, i64 0, i32 0
  ret %Body* %p
}

define %Index* @getArityPointer(%Function* %f) {
  %p = getelementptr %Function* %f, i64 0, i32 1
  ret %Index* %p
}

define %Queue** @getArgumentsPointer(%Function* %f) {
  %p = getelementptr %Function* %f, i64 0, i32 2
  ret %Queue** %p
}


; sizeof(queue node) = 2 * sizeof(pointer) = 16
%QueueNode = type <{ %Function*, %QueueNode* }>
@.QUEUENODE_SIZE = private unnamed_addr constant i32 16

define %Function** @getDataPointer(%QueueNode* %qn) {
  %p = getelementptr %QueueNode* %qn, i64 0, i32 0
  ret %Function** %p
}

define %QueueNode** @getNextPointer(%QueueNode* %qn) {
  %p = getelementptr %QueueNode* %qn, i64 0, i32 1
  ret %QueueNode** %p
}


; sizeof(queue) = sizeof(index) + 2 * sizeof(pointer) = 17
%Queue = type <{ %Index, %QueueNode*, %QueueNode* }>
@.QUEUE_SIZE = private unnamed_addr constant i32 17

define %Index* @getLengthPointer(%Queue* %q) {
  %p = getelementptr %Queue* %q, i64 0, i32 0
  ret %Index* %p
}

define %QueueNode** @getHeadPointer(%Queue* %q) {
  %p = getelementptr %Queue* %q, i64 0, i32 1
  ret %QueueNode** %p
}

define %QueueNode** @getTailPointer(%Queue* %q) {
  %p = getelementptr %Queue* %q, i64 0, i32 2
  ret %QueueNode** %p
}