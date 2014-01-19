%Function = type i1
%QueueNode = type { %Function*, %QueueNode* }
@.LAST = private unnamed_addr constant %QueueNode { %Function* null, %QueueNode* null }

define %Function* @getArgument(%QueueNode* %qn) {
  %argument_p = getelementptr %QueueNode* %qn, i64 0, i32 0
  %argument = load %Function** %argument_p
  ret %Function* %argument
}

define %QueueNode* @getNext(%QueueNode* %qn) {
  %next_p = getelementptr %QueueNode* %qn, i64 0, i32 1
  %next = load %QueueNode** %next_p
  ret %QueueNode* %next
}


; For test output in main
%TestName = type [13 x i8]
@.pass = private unnamed_addr constant [2 x i8] c".\00"
declare i32 @puts(i8* nocapture) nounwind

define i1 @assertCond(i1 %cond, %TestName* %name) {
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
  ret i1 0
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