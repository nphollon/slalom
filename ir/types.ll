%Function = type i8
%Index = type i8
%QueueNode = type <{ %Function*, %QueueNode* }>
%Queue = type <{ %Index, %QueueNode*, %QueueNode* }>

@.FUNCTION_SIZE = private unnamed_addr constant i32 1
@.INDEX_SIZE = private unnamed_addr constant i32 1
@.QUEUENODE_SIZE = private unnamed_addr constant i32 16
@.QUEUE_SIZE = private unnamed_addr constant i32 17
