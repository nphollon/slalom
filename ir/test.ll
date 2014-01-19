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
  call void @testEnqueue()
  ret i1 0
}

@.qBigger1     = private unnamed_addr constant %TestName c"Q Bigger 1  \00"
@.headIsN1     = private unnamed_addr constant %TestName c"Head Is N1  \00"
@.n1HasF1      = private unnamed_addr constant %TestName c"N1 Has F1   \00"
@.n1PtToLast   = private unnamed_addr constant %TestName c"N1PtToLast  \00"
@.qBigger2     = private unnamed_addr constant %TestName c"Q Bigger 2  \00"
@.n2HasF2      = private unnamed_addr constant %TestName c"N2 Has F2   \00"
@.headNoChange = private unnamed_addr constant %TestName c"HeadNoChange\00"
@.n1PtToN2     = private unnamed_addr constant %TestName c"N1 Pt To N2 \00"
define void @testEnqueue() {
  %q = call %Queue* @createEmptyQueue()
  
  ; Enqueue a function
  %f1 = call %Function* @createICombinator()
  call void @enqueue(%Queue* %q, %Function* %f1)
  %node1 = call %QueueNode* @getTail(%Queue* %q)

  ; Assert that q length is 1
  %length1 = call %Index @getLength(%Queue* %q)
  call void @assertEqIndex(%Index %length1, %Index 1, %TestName* @.qBigger1)

  ; Assert that node1 contains f1
  %node1Data = call %Function* @getData(%QueueNode* %node1)
  call void @assertEqFunction(%Function* %node1Data, %Function* %f1, %TestName* @.n1HasF1)

  ; Assert that node1 points to LAST
  %node1Next = call %QueueNode* @getNext(%QueueNode* %node1)
  call void @assertEqQueueNode(%QueueNode* %node1Next, %QueueNode* @.LAST, %TestName* @.n1PtToLast)

  ; Assert that q head is node1
  %head = call %QueueNode* @getHead(%Queue* %q)
  call void @assertEqQueueNode(%QueueNode* %node1, %QueueNode* %head, %TestName* @.headIsN1)

  ; Enqueue another function
  %f2 = call %Function* @createICombinator()
  call void @enqueue(%Queue* %q, %Function* %f2)
  %node2 = call %QueueNode* @getTail(%Queue* %q)

  ; Assert that q length is 2
  %length2 = call %Index @getLength(%Queue* %q)
  call void @assertEqIndex(%Index %length2, %Index 2, %TestName* @.qBigger2)

  ; Assert that node2 contains f2
  %node2Data = call %Function* @getData(%QueueNode* %node2)
  call void @assertEqFunction(%Function* %node2Data, %Function* %f2, %TestName* @.n2HasF2)

  ; Assert that q head is still node1
  %head2 = call %QueueNode* @getHead(%Queue* %q)
  call void @assertEqQueueNode(%QueueNode* %head2, %QueueNode* %node1, %TestName* @.headNoChange)

  ; Assert that node1 points to node2
  %node1Next2 = call %QueueNode* @getNext(%QueueNode* %node1)
  call void @assertEqQueueNode(%QueueNode* %node1Next2, %QueueNode* %node2, %TestName* @.n1PtToN2)

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