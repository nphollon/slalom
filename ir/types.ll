%Index = type i8
%QueueNode = type <{ %Function*, %QueueNode* }>
%Queue = type <{ %Index, %QueueNode*, %QueueNode* }>
%Body = type %Function*(%Queue*)*
%Function = type <{ %Body, %Index, %Queue* }>

; sizeof(pointer) = sizeof(i64) = 8

; sizeof(index) = sizeof(i8) = 1
@.INDEX_SIZE = private unnamed_addr constant i32 1

; sizeof(function) = 1 + sizeof(index) + sizeof(pointer) = 10
@.FUNCTION_SIZE = private unnamed_addr constant i32 10

; sizeof(queue node) = 2 * sizeof(pointer) = 16
@.QUEUENODE_SIZE = private unnamed_addr constant i32 16

; sizeof(queue) = sizeof(index) + 2 * sizeof(pointer) = 17
@.QUEUE_SIZE = private unnamed_addr constant i32 17
