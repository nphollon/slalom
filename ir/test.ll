; For test output in main
%TestName = type [13 x i8]
@.pass = private unnamed_addr constant [2 x i8] c".\00"
declare i32 @puts(i8* nocapture) nounwind

define i1 @assertCond(i1 %cond, %TestName* %name) {
entry:
  br i1 %cond, label %testPassed, label %testFailed
testPassed:
  %passPtr = getelementptr [2 x i8]* @.pass, i64 0, i64 0
  call i32 @puts(i8* %passPtr)
  ret i1 0
testFailed:
  %namePtr = getelementptr %TestName* %name, i64 0, i64 0
  call i32 @puts(i8* %namePtr)
  ret i1 1
}

define i1 @main() {
entry:
  call void @testLast()
  call void @testCreateEmptyQueue()
  ret i1 0
}

@.emptyLength0 = private unnamed_addr constant %TestName c"EmptyLength0\00"
@.emptyHead = private unnamed_addr constant %TestName c"EmptyHead   \00"
@.emptyTail = private unnamed_addr constant %TestName c"EmptyTail   \00"
define void @testCreateEmptyQueue() {
entry:
  %q = call %Queue* @createEmptyQueue()

  ; Assert that length of q is 0
  %q_len = call %Index @getLength(%Queue* %q)
  %emptyLength0 = icmp eq %Index %q_len, 0
  call i1 @assertCond(i1 %emptyLength0, %TestName* @.emptyLength0)

  ; Assert that head node of q is LAST
  %q_head = call %QueueNode* @getHead(%Queue* %q)
  %emptyHead = icmp eq %QueueNode* %q_head, @.LAST
  call i1 @assertCond(i1 %emptyHead, %TestName* @.emptyHead)

  ; Assert that tail node of q is LAST
  %q_tail = call %QueueNode* @getTail(%Queue* %q)
  %emptyTail = icmp eq %QueueNode* %q_tail, @.LAST
  call i1 @assertCond(i1 %emptyTail, %TestName* @.emptyTail)

  ; Free queue
  %q_i8 = bitcast %Queue* %q to i8*
  call void @free(i8* %q_i8) nounwind
  ret void
}

@.lastArgNull = private unnamed_addr constant %TestName c"LastArgNull \00"
@.lastNextNull = private unnamed_addr constant %TestName c"LastNextNull\00"
define void @testLast() {
entry:
  ; Assert that @.LAST.argument is null
  %last_arg = call %Function* @getArgument(%QueueNode* @.LAST)
  %lastArgNull = icmp eq %Function* %last_arg, null
  call i1 @assertCond(i1 %lastArgNull, %TestName* @.lastArgNull)

  ; Assert that @.LAST.next is null
  %last_next = call %QueueNode* @getNext(%QueueNode* @.LAST)
  %lastNextNull = icmp eq %QueueNode* %last_next, null
  call i1 @assertCond(i1 %lastNextNull, %TestName* @.lastNextNull)
  ret void
}