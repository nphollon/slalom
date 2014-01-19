; For test output in main
%TestName = type [13 x i8]
@.pass = private unnamed_addr constant [2 x i8] c".\00"
declare i32 @puts(i8* nocapture) nounwind

define void @assertEqIndex(%Index, %Index, %TestName* %name) {
  %equal = icmp eq %Index %0, %1
  call void @assertCond(i1 %equal, %TestName* %name)
  ret void
}

define void @assertEqFunction(%Function*, %Function*, %TestName* %name) {
  %equal = icmp eq %Function* %0, %1
  call void @assertCond(i1 %equal, %TestName* %name)
  ret void
}

define void @assertEqQueueNode(%QueueNode*, %QueueNode*, %TestName* %name) {
  %equal = icmp eq %QueueNode* %0, %1
  call void @assertCond(i1 %equal, %TestName* %name)
  ret void
}

define void @assertEqBody(%Body*, %Body, %TestName* %name) {
  %b = load %Body* %0
  %equal = icmp eq %Body %b, %1
  call void @assertCond(i1 %equal, %TestName* %name)
  ret void
}

define void @assertCond(i1 %cond, %TestName* %name) {
entry:
  br i1 %cond, label %testPassed, label %testFailed
testPassed:
  %passPtr = getelementptr [2 x i8]* @.pass, i64 0, i64 0
  call i32 @puts(i8* %passPtr)
  ret void
testFailed:
  %namePtr = getelementptr %TestName* %name, i64 0, i64 0
  call i32 @puts(i8* %namePtr)
  ret void
}

define i1 @main() {
entry:
  call void @testLast()
  call void @testCreateEmptyQueue()
  call void @testCreateICombinator()
  call void @testEnqueueAndDequeue()
  ret i1 0
}

@.qBigger1 = private unnamed_addr constant %TestName c"Q Bigger 1  \00"
@.headIsFirst = private unnamed_addr constant %TestName c"HeadIsFirst \00"
@.lastIsSecond = private unnamed_addr constant %TestName c"LastIsSecond\00"
@.tailIsHead = private unnamed_addr constant %TestName c"TailIsHead  \00"
@.qBigger2 = private unnamed_addr constant %TestName c"Q Bigger 2  \00"
@.headNoChange = private unnamed_addr constant %TestName c"HeadNoChange\00"
define void @testEnqueueAndDequeue() {
  %q = call %Queue* @createEmptyQueue()
  
  ; Enqueue a function
  %f1 = call %Function* @createICombinator()
  %args1 = call %Queue* @getArguments(%Function* %f1)
  call void @enqueue(%Queue* %q, %Function* %f1)

  ; Assert that q length is 1
  %length1 = call %Index @getLength(%Queue* %q)
  call void @assertEqIndex(%Index %length1, %Index 1, %TestName* @.qBigger1)

  ; Assert that q head contains f1
  %head = call %QueueNode* @getHead(%Queue* %q)
  %headData = call %Function* @getData(%QueueNode* %head)
  call void @assertEqFunction(%Function* %headData, %Function* %f1, %TestName* @.headIsFirst)

  ; Assert that q head points to LAST
  %headNext = call %QueueNode* @getNext(%QueueNode* %head)
  call void @assertEqQueueNode(%QueueNode* %headNext, %QueueNode* @.LAST, %TestName* @.lastIsSecond)

  ; Assert that q head and q tail are the same
  %tail = call %QueueNode* @getTail(%Queue* %q)
  call void @assertEqQueueNode(%QueueNode* %tail, %QueueNode* %head, %TestName* @.tailIsHead)

  ; Enqueue another function
  %f2 = call %Function* @createICombinator()
  %args2 = call %Queue* @getArguments(%Function* %f2)
  call void @enqueue(%Queue* %q, %Function* %f2)

  ; Assert that q length is 2
  %length2 = call %Index @getLength(%Queue* %q)
  call void @assertEqIndex(%Index %length2, %Index 2, %TestName* @.qBigger2)

  ; Assert that q head is unchanged
  %head2 = call %QueueNode* @getHead(%Queue* %q)
  call void @assertEqQueueNode(%QueueNode* %head2, %QueueNode* %head, %TestName* @.headNoChange)

  ret void
}

@.iArity1 = private unnamed_addr constant %TestName c"I Arity 1   \00"
@.iArgsEmpty = private unnamed_addr constant %TestName c"I Args Empty\00"
@.iBodyDequeue = private unnamed_addr constant %TestName c"IBodyDequeue\00"
define void @testCreateICombinator() {
  %i = call %Function* @createICombinator()

  ; Assert that body is @dequeue
  %body_p = call %Body* @getBodyPointer(%Function* %i)
  call void @assertEqBody(%Body* %body_p, %Body @dequeue, %TestName* @.iBodyDequeue)

  ; Assert that arity is 1
  %arity = call %Index @getArity(%Function* %i)
  call void @assertEqIndex(%Index %arity, %Index 1, %TestName* @.iArity1)

  ; Assert that arguments is an empty queue
  %arguments = call %Queue* @getArguments(%Function* %i)
  %len = call %Index @getLength(%Queue* %arguments)
  call void @assertEqIndex(%Index %len, %Index 0, %TestName* @.iArgsEmpty)

  ; Free function & arguments
  %arguments_i8 = bitcast %Queue* %arguments to i8*
  call void @free(i8* %arguments_i8) nounwind
  %i_i8 = bitcast %Function* %i to i8*
  call void @free(i8* %i_i8) nounwind
  ret void
}

@.emptyLength0 = private unnamed_addr constant %TestName c"EmptyLength0\00"
@.emptyHead = private unnamed_addr constant %TestName c"EmptyHead   \00"
@.emptyTail = private unnamed_addr constant %TestName c"EmptyTail   \00"
define void @testCreateEmptyQueue() {
  %q = call %Queue* @createEmptyQueue()

  ; Assert that length of q is 0
  %q_len = call %Index @getLength(%Queue* %q)
  call void @assertEqIndex(%Index %q_len, %Index 0, %TestName* @.emptyLength0)

  ; Assert that head node of q is LAST
  %q_head = call %QueueNode* @getHead(%Queue* %q)
  call void @assertEqQueueNode(%QueueNode* %q_head, %QueueNode* @.LAST, %TestName* @.emptyHead)

  ; Assert that tail node of q is LAST
  %q_tail = call %QueueNode* @getTail(%Queue* %q)
  call void @assertEqQueueNode(%QueueNode* %q_tail, %QueueNode* @.LAST, %TestName* @.emptyTail)

  ; Free queue
  %q_i8 = bitcast %Queue* %q to i8*
  call void @free(i8* %q_i8) nounwind
  ret void
}

@.lastDataNull = private unnamed_addr constant %TestName c"LastDataNull\00"
@.lastNextNull = private unnamed_addr constant %TestName c"LastNextNull\00"
define void @testLast() {
  ; Assert that @.LAST.argument is null
  %last_data = call %Function* @getData(%QueueNode* @.LAST)
  call void @assertEqFunction(%Function* %last_data, %Function* null, %TestName* @.lastDataNull)

  ; Assert that @.LAST.next is null
  %last_next = call %QueueNode* @getNext(%QueueNode* @.LAST)
  call void @assertEqQueueNode(%QueueNode* %last_next, %QueueNode* null, %TestName* @.lastNextNull)

  ret void
}