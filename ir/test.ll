; For test output in main
%TestName = type [13 x i8]
@.NULLC = private unnamed_addr constant i8 0
declare i32 @putchar(i32) nounwind
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
  call i32 @putchar(i32 46) ; Prints a period
  ret void
testFailed:
  %namePtr = getelementptr %TestName* %name, i64 0, i64 0
  call i32 @puts(i8* @.NULLC)
  call i32 @puts(i8* %namePtr)
  ret void
}

define void @assertIsI(%Function* %f, %TestName* %name) {
  %bodyP = call %Body* @getBodyPointer(%Function* %f)
  call void @assertEqBody(%Body* %bodyP, %Body @dequeue, %TestName* %name)
  %arity = call %Index @getArity(%Function* %f)
  call void @assertEqIndex(%Index %arity, %Index 1, %TestName* %name)
  ret void
} 

define void @assertIsK(%Function* %f, %TestName* %name) {
  %bodyP = call %Body* @getBodyPointer(%Function* %f)
  call void @assertEqBody(%Body* %bodyP, %Body @dequeue, %TestName* %name)
  %arity = call %Index @getArity(%Function* %f)
  call void @assertEqIndex(%Index %arity, %Index 2, %TestName* %name)
  ret void
}

define void @assertIsS(%Function* %f, %TestName* %name) {
  %bodyP = call %Body* @getBodyPointer(%Function* %f)
  call void @assertEqBody(%Body* %bodyP, %Body @substitute, %TestName* %name)
  %arity = call %Index @getArity(%Function* %f)
  call void @assertEqIndex(%Index %arity, %Index 3, %TestName* %name)
  ret void
}

define void @setArity(%Function* %f, %Index %newArity) {
  %arity_p = call %Index* @getArityPointer(%Function* %f)
  store %Index %newArity, %Index* %arity_p
  ret void
}

define void @addArgument(%Function* %app, %Function* %arg) {
  %args = call %Queue* @getArguments(%Function* %app)
  call void @enqueue(%Queue* %args, %Function* %arg)
  ret void
}

define i1 @main() {
entry:
  call void @testLast()
  call void @testCreateEmptyQueue()
  call void @testEnqueue()
  call void @testDequeue()
  call void @testNullTransfer()
  call void @testPartialTransfer()

  call void @testCreateICombinator()
  call void @testCreateKCombinator()
  call void @testCreateSCombinator()
  call void @testFCopy()
  call void @testEvaluate()
  call void @testEvaluateOverflow()
  call void @testEvaluateRecursive()
  call void @testApply()
  call void @testSubstitute()
  call void @testSubstituteEval1()
  call void @testSubstituteEval2()

  call i32 @puts(i8* @.NULLC)
  ret i1 0
}

@.subsNestEval = private unnamed_addr constant %TestName c"subsNestEval\00"
define void @testSubstituteEval2() {
  ; Call substitute(S, I, S)
  %args = call %Queue* @createEmptyQueue()
  %arg1 = call %Function* @createSCombinator()
  %arg2 = call %Function* @createICombinator()
  %arg3 = call %Function* @createSCombinator()
  call void @enqueue(%Queue* %args, %Function* %arg1)
  call void @enqueue(%Queue* %args, %Function* %arg2)
  call void @enqueue(%Queue* %args, %Function* %arg3)
  %result = call %Function* @substitute(%Queue* %args)

  ; Assert result is (S S S)
  %resultArgs = call %Queue* @getArguments(%Function* %result)
  %resultArg2 = call %QueueNode* @getTail(%Queue* %resultArgs)
  %resultF2 = call %Function* @getData(%QueueNode* %resultArg2)
  call void @assertIsS(%Function* %resultF2, %TestName* @.subsNestEval)

  call void @qDestroy(%Queue* %args)
  call void @fDestroy(%Function* %result)
  ret void
}

@.skksEvalToS  = private unnamed_addr constant %TestName c"SkksEvalToS \00"
define void @testSubstituteEval1() {
  ; Call substitute(K, K, S)
  %args = call %Queue* @createEmptyQueue()
  %arg1 = call %Function* @createKCombinator()
  %arg2 = call %Function* @createKCombinator()
  %arg3 = call %Function* @createSCombinator()
  call void @enqueue(%Queue* %args, %Function* %arg1)
  call void @enqueue(%Queue* %args, %Function* %arg2)
  call void @enqueue(%Queue* %args, %Function* %arg3)
  %result = call %Function* @substitute(%Queue* %args)

  ; Assert result is (S)
  call void @assertIsS(%Function* %result, %TestName* @.skksEvalToS)

  call void @qDestroy(%Queue* %args)
  call void @fDestroy(%Function* %result)
  ret void
}

@.sikiAppIsS   = private unnamed_addr constant %TestName c"SikiAppIsS  \00"
@.sikiArg1IsI  = private unnamed_addr constant %TestName c"SikiArg1IsI \00"
@.sikiHas2Args = private unnamed_addr constant %TestName c"SikiHas2Args\00"
@.sikiArg2IsK  = private unnamed_addr constant %TestName c"SikiArg2IsK \00"
@.kInSikiHasAr = private unnamed_addr constant %TestName c"KInSikiHasAr\00"
@.kInSikiArgI  = private unnamed_addr constant %TestName c"KInSikiArgI \00"
@.sikiArgsUniq = private unnamed_addr constant %TestName c"SikiArgsUniq\00"
define void @testSubstitute() {
  ; Call substitute(S, K, I)
  %args = call %Queue* @createEmptyQueue()
  %arg1 = call %Function* @createSCombinator()
  %arg2 = call %Function* @createKCombinator()
  %arg3 = call %Function* @createICombinator()
  call void @enqueue(%Queue* %args, %Function* %arg1)
  call void @enqueue(%Queue* %args, %Function* %arg2)
  call void @enqueue(%Queue* %args, %Function* %arg3)
  %result = call %Function* @substitute(%Queue* %args)

  ; Assert result is (S I (K I))
  call void @assertEqFunction(%Function* %result, %Function* %arg1, %TestName* @.sikiAppIsS)
  
  %resultArgs = call %Queue* @getArguments(%Function* %result)
  %resultArg1 = call %QueueNode* @getHead(%Queue* %resultArgs)
  %resultF1 = call %Function* @getData(%QueueNode* %resultArg1)
  call void @assertIsI(%Function* %resultF1, %TestName* @.sikiArg1IsI)

  %resultArgsLength = call %Index @getLength(%Queue* %resultArgs)
  call void @assertEqIndex(%Index %resultArgsLength, %Index 2, %TestName* @.sikiHas2Args)

  %resultArg2 = call %QueueNode* @getTail(%Queue* %resultArgs)
  %resultF2 = call %Function* @getData(%QueueNode* %resultArg2)
  call void @assertEqFunction(%Function* %resultF2, %Function* %arg2, %TestName* @.sikiArg2IsK)

  %resultF2Args = call %Queue* @getArguments(%Function* %resultF2)
  %resultF2ArgsLength = call %Index @getLength(%Queue* %resultF2Args)
  call void @assertEqIndex(%Index %resultF2ArgsLength, %Index 1, %TestName* @.kInSikiHasAr)

  %resultF2Arg1 = call %QueueNode* @getHead(%Queue* %resultF2Args)
  %resultF2F1 = call %Function* @getData(%QueueNode* %resultF2Arg1)
  call void @assertIsI(%Function* %resultF2F1, %TestName* @.kInSikiArgI)

  %argsAreUnique = icmp ne %Function* %resultF2F1, %resultF1
  call void @assertCond(i1 %argsAreUnique, %TestName* @.sikiArgsUniq)

  call void @qDestroy(%Queue* %args)
  call void @fDestroy(%Function* %result)
  ret void
}

@.kiAppIsK     = private unnamed_addr constant %TestName c"Ki App Is K \00"
@.kiHas1Arg    = private unnamed_addr constant %TestName c"Ki Has 1 Arg\00"
@.kiArgIsI     = private unnamed_addr constant %TestName c"Ki Arg Is I \00"
@.kisEvalToI   = private unnamed_addr constant %TestName c"KisEvalToI  \00"
define void @testApply() {
  ; Apply K to I
  %applicator1 = call %Function* @createKCombinator()
  %input1 = call %Function* @createICombinator()
  %applicator2 = call %Function* @apply(%Function* %applicator1, %Function* %input1)

  ; Assert result is K I
  call void @assertEqFunction(%Function* %applicator1, %Function* %applicator2, %TestName* @.kiAppIsK)
  %app2Args = call %Queue* @getArguments(%Function* %applicator2)
  %app2ArgsLength = call %Index @getLength(%Queue* %app2Args)
  call void @assertEqIndex(%Index %app2ArgsLength, %Index 1, %TestName* @.kiHas1Arg)
  %app2Arg1 = call %QueueNode* @getHead(%Queue* %app2Args)
  %app2Arg1Data = call %Function* @getData(%QueueNode* %app2Arg1)
  call void @assertEqFunction(%Function* %app2Arg1Data, %Function* %input1, %TestName* @.kiArgIsI)

  ; Apply S to (K I)
  %input2 = call %Function* @createSCombinator()
  %finalResult = call %Function* @apply(%Function* %applicator2, %Function* %input2)  

  ; Assert result is I
  call void @assertEqFunction(%Function* %finalResult, %Function* %input1, %TestName* @.kisEvalToI)

  call void @fDestroy(%Function* %finalResult)
  ret void
}

@.kiisEvalToS  = private unnamed_addr constant %TestName c"KiisEvalToS \00"
define void @testEvaluateRecursive() {
  ; Evaluate (K I I S)
  %applicator = call %Function* @createKCombinator()
  %arg1 = call %Function* @createICombinator()
  %arg2 = call %Function* @createICombinator()
  %arg3 = call %Function* @createSCombinator()
  call void @addArgument(%Function* %applicator, %Function* %arg1)
  call void @addArgument(%Function* %applicator, %Function* %arg2)
  call void @addArgument(%Function* %applicator, %Function* %arg3)
  %result = call %Function* @evaluate(%Function* %applicator)

  ; Assert result is (S)
  call void @assertEqFunction(%Function* %result, %Function* %arg3, %TestName* @.kiisEvalToS)

  call void @fDestroy(%Function* %result)
  ret void
}

@.kkisAppIsK   = private unnamed_addr constant %TestName c"KkisAppIsK  \00"
@.kkisHas1Arg  = private unnamed_addr constant %TestName c"KkisHas1Arg \00"
@.kkisArgIsS   = private unnamed_addr constant %TestName c"KkisArgIsS  \00"
define void @testEvaluateOverflow() {
  ; Evaluate (K K I S)
  %applicator = call %Function* @createKCombinator()
  %arg1 = call %Function* @createKCombinator()
  %arg2 = call %Function* @createICombinator()
  %arg3 = call %Function* @createSCombinator()
  call void @addArgument(%Function* %applicator, %Function* %arg1)
  call void @addArgument(%Function* %applicator, %Function* %arg2)
  call void @addArgument(%Function* %applicator, %Function* %arg3)
  %result = call %Function* @evaluate(%Function* %applicator)
  
  ; Assert result is (K S)
  call void @assertIsK(%Function* %result, %TestName* @.kkisAppIsK)
  %resultArgs = call %Queue* @getArguments(%Function* %result)
  %resultArgNum = call %Index @getLength(%Queue* %resultArgs)
  call void @assertEqIndex(%Index %resultArgNum, %Index 1, %TestName* @.kkisHas1Arg)
  %resultNode1 = call %QueueNode* @getHead(%Queue* %resultArgs)
  %resultArg1 = call %Function* @getData(%QueueNode* %resultNode1)
  call void @assertEqFunction(%Function* %resultArg1, %Function* %arg3, %TestName* @.kkisArgIsS)
  
  call void @fDestroy(%Function* %result)
  ret void
}

@.evalUnderflo = private unnamed_addr constant %TestName c"EvalUnderflo\00"
@.evalExactK   = private unnamed_addr constant %TestName c"Eval Exact K\00"
define void @testEvaluate() {
  ; Evaluate (K I)
  %applicator = call %Function* @createKCombinator()
  %arg1 = call %Function* @createICombinator()
  call void @addArgument(%Function* %applicator, %Function* %arg1)
  %result1 = call %Function* @evaluate(%Function* %applicator)

  ; Assert result is applicator
  call void @assertEqFunction(%Function* %result1, %Function* %applicator, %TestName* @.evalUnderflo)

  ; Evaluate (K I S)
  %arg2 = call %Function* @createSCombinator()
  call void @addArgument(%Function* %applicator, %Function* %arg2)
  %result2 = call %Function* @evaluate(%Function* %applicator)
  
  ; Assert result is arg1
  call void @assertEqFunction(%Function* %result2, %Function* %arg1, %TestName* @.evalExactK)

  call void @fDestroy(%Function* %result2)
  ret void
}

@.fCopyArity   = private unnamed_addr constant %TestName c"F Copy Arity\00"
@.fCopyBody    = private unnamed_addr constant %TestName c"F Copy Body \00"
@.fCopyArg1    = private unnamed_addr constant %TestName c"F Copy Arg1 \00"
@.fCopyArg2    = private unnamed_addr constant %TestName c"F Copy Arg2 \00"
define void @testFCopy() {
  %original = call %Function* @createSCombinator()
  %arg1 = call %Function* @createICombinator()
  %arg2 = call %Function* @createKCombinator()
  call void @addArgument(%Function* %original, %Function* %arg1)
  call void @addArgument(%Function* %original, %Function* %arg2)

  ; Copy the function
  %copy = call %Function* @fCopy(%Function* %original)

  ; Assert arity is the same
  %cArity = call %Index @getArity(%Function* %copy)
  %oArity = call %Index @getArity(%Function* %original)
  call void @assertEqIndex(%Index %cArity, %Index %oArity, %TestName* @.fCopyArity)

  ; Assert body is the same
  %cBodyP = call %Body* @getBodyPointer(%Function* %copy)
  %oBodyP = call %Body* @getBodyPointer(%Function* %original)
  %oBody = load %Body* %oBodyP
  call void @assertEqBody(%Body* %cBodyP, %Body %oBody, %TestName* @.fCopyBody)

  ; Assert arguments are deep copied
  %cArgs = call %Queue* @getArguments(%Function* %copy)
  %cArg1 = call %Function* @dequeue(%Queue* %cArgs)
  call void @assertIsI(%Function* %cArg1, %TestName* @.fCopyArg1)
  %cArg2 = call %Function* @dequeue(%Queue* %cArgs)
  call void @assertIsK(%Function* %cArg2, %TestName* @.fCopyArg2)

  call void @fDestroy(%Function* %original)
  call void @fDestroy(%Function* %copy)
  call void @fDestroy(%Function* %cArg1)
  call void @fDestroy(%Function* %cArg2)
  ret void
}

@.sBodyDequeue = private unnamed_addr constant %TestName c"SBodyDequeue\00"
@.sArity2      = private unnamed_addr constant %TestName c"S Arity 2   \00"
@.sArgsEmpty   = private unnamed_addr constant %TestName c"S Args Empty\00"
define void @testCreateSCombinator() {
  %s = call %Function* @createSCombinator()

  ; Assert body is @substitute
  %body = call %Body* @getBodyPointer(%Function* %s)
  call void @assertEqBody(%Body* %body, %Body @substitute, %TestName* @.sBodyDequeue)

  ; Assert arity 3
  %arity = call %Index @getArity(%Function* %s)
  call void @assertEqIndex(%Index %arity, %Index 3, %TestName* @.sArity2)

  ; Assert arguments queue is empty
  %arguments = call %Queue* @getArguments(%Function* %s)
  %isEmpty = call i1 @isEmpty(%Queue* %arguments)
  call void @assertCond(i1 %isEmpty, %TestName* @.sArgsEmpty)

  call void @fDestroy(%Function* %s)
  ret void
}

@.kBodyDequeue = private unnamed_addr constant %TestName c"KBodyDequeue\00"
@.kArity2      = private unnamed_addr constant %TestName c"K Arity 2   \00"
@.kArgsEmpty   = private unnamed_addr constant %TestName c"K Args Empty\00"
define void @testCreateKCombinator() {
  %k = call %Function* @createKCombinator()

  ; Assert body is dequeue
  %body = call %Body* @getBodyPointer(%Function* %k)
  call void @assertEqBody(%Body* %body, %Body @dequeue, %TestName* @.kBodyDequeue)

  ; Assert arity 2
  %arity = call %Index @getArity(%Function* %k)
  call void @assertEqIndex(%Index %arity, %Index 2, %TestName* @.kArity2)

  ; Assert arguments queue is empty
  %arguments = call %Queue* @getArguments(%Function* %k)
  %isEmpty = call i1 @isEmpty(%Queue* %arguments)
  call void @assertCond(i1 %isEmpty, %TestName* @.kArgsEmpty)

  call void @fDestroy(%Function* %k)
  ret void
}

@.transferLen2 = private unnamed_addr constant %TestName c"TransferLen2\00"
@.transferF1   = private unnamed_addr constant %TestName c"Transfer F1 \00"
@.transferF2   = private unnamed_addr constant %TestName c"Transfer F2 \00"
@.n2NextIsLast = private unnamed_addr constant %TestName c"N2NextIsLast\00"
@.f3Remains    = private unnamed_addr constant %TestName c"F3 Remains  \00"
@.only1Left    = private unnamed_addr constant %TestName c"Only 1 Left \00"
define void @testPartialTransfer() {
  %receiver = call %Queue* @createEmptyQueue()
  %sender = call %Queue* @createEmptyQueue()
  %f1 = call %Function* @createICombinator()
  %f2 = call %Function* @createICombinator()
  %f3 = call %Function* @createICombinator()
  call void @enqueue(%Queue* %sender, %Function* %f1)
  call void @enqueue(%Queue* %sender, %Function* %f2)
  call void @enqueue(%Queue* %sender, %Function* %f3)

  ; Transfer 2
  call void @transfer(%Queue* %receiver, %Queue* %sender, %Index 2)

  ; Assert that receiver has length 2
  %receiverLength = call %Index @getLength(%Queue* %receiver)
  call void @assertEqIndex(%Index %receiverLength, %Index 2, %TestName* @.transferLen2)

  ; Assert that head of receiver contains f1
  %receiverN1 = call %QueueNode* @getHead(%Queue* %receiver)
  %n1Data = call %Function* @getData(%QueueNode* %receiverN1)
  call void @assertEqFunction(%Function* %n1Data, %Function* %f1, %TestName* @.transferF1)

  ; Assert that tail of receiver contains f2
  %receiverN2 = call %QueueNode* @getTail(%Queue* %receiver)
  %n2Data = call %Function* @getData(%QueueNode* %receiverN2)
  call void @assertEqFunction(%Function* %n2Data, %Function* %f2, %TestName* @.transferF2)

  ; Assert that tail.next is LAST
  %n2Next = call %QueueNode* @getNext(%QueueNode* %receiverN2)
  call void @assertEqQueueNode(%QueueNode* %n2Next, %QueueNode* @.LAST, %TestName* @.n2NextIsLast)

  ; Assert that head of sender contains f3
  %senderHead = call %QueueNode* @getHead(%Queue* %sender)
  %senderHeadData = call %Function* @getData(%QueueNode* %senderHead)
  call void @assertEqFunction(%Function* %senderHeadData, %Function* %f3, %TestName* @.f3Remains)

  ; Assert that q has length 1
  %senderLength = call %Index @getLength(%Queue* %sender)
  call void @assertEqIndex(%Index %senderLength, %Index 1, %TestName* @.only1Left)

  call void @qDestroy(%Queue* %receiver)
  call void @qDestroy(%Queue* %sender)
  ret void
}

@.nullTransfer = private unnamed_addr constant %TestName c"NullTransfer\00"
define void @testNullTransfer() {
  %receiver = call %Queue* @createEmptyQueue()
  %sender = call %Queue* @createEmptyQueue()
  %f1 = call %Function* @createICombinator()
  call void @enqueue(%Queue* %sender, %Function* %f1)

  ; Transfer 0 from sender to receiver
  call void @transfer(%Queue* %receiver, %Queue* %sender, %Index 0)

  ; Assert that receiver is still empty
  %receiverIsEmpty = call i1 @isEmpty(%Queue* %receiver)
  call void @assertCond(i1 %receiverIsEmpty, %TestName* @.nullTransfer)

  call void @qDestroy(%Queue* %sender)
  call void @qDestroy(%Queue* %receiver)
  ret void
}

@.dq1IsF1      = private unnamed_addr constant %TestName c"DQ1 Is F1   \00"
@.qSmaller1    = private unnamed_addr constant %TestName c"Q Smaller 1 \00"
@.headIsN2     = private unnamed_addr constant %TestName c"Head Is N2  \00"
@.qSmaller0    = private unnamed_addr constant %TestName c"Q Smaller 0 \00"
@.dq2IsF2      = private unnamed_addr constant %TestName c"DQ2 Is F2   \00"
@.headIsLast   = private unnamed_addr constant %TestName c"Head Is Last\00"
@.tailIsLast   = private unnamed_addr constant %TestName c"Tail Is Last\00"
@.qNoSmaller   = private unnamed_addr constant %TestName c"Q No Smaller\00"
@.dq3IsNull    = private unnamed_addr constant %TestName c"DQ3 Is Null \00"
@.headNotNull  = private unnamed_addr constant %TestName c"HeadNotNull \00"
define void @testDequeue() {
  ; create a queue with some functions
  %q = call %Queue* @createEmptyQueue()
  %f1 = call %Function* @createICombinator()
  %f2 = call %Function* @createICombinator()
  call void @enqueue(%Queue* %q, %Function* %f1)
  %node1 = call %QueueNode* @getTail(%Queue* %q)
  call void @enqueue(%Queue* %q, %Function* %f2)
  %node2 = call %QueueNode* @getTail(%Queue* %q)

  ; Dequeue a function
  %dq1 = call %Function* @dequeue(%Queue* %q)

  ; Assert that q length is 1
  %length = call %Index @getLength(%Queue* %q)
  call void @assertEqIndex(%Index %length, %Index 1, %TestName* @.qSmaller1)

  ; Assert that dq1 is f1
  call void @assertEqFunction(%Function* %dq1, %Function* %f1, %TestName* @.dq1IsF1)

  ; Assert that q head is node2
  %head = call %QueueNode* @getHead(%Queue* %q)
  call void @assertEqQueueNode(%QueueNode* %head, %QueueNode* %node2, %TestName* @.headIsN2)

  ; Dequeue another function
  %dq2 = call %Function* @dequeue(%Queue* %q)

  ; Assert that q is empty
  %isEmpty = call i1 @isEmpty(%Queue* %q)
  call void @assertCond(i1 %isEmpty, %TestName* @.qSmaller0)

  ; Assert that dq2 is f2
  call void @assertEqFunction(%Function* %dq2, %Function* %f2, %TestName* @.dq2IsF2)

  ; Assert that q head is LAST
  %head2 = call %QueueNode* @getHead(%Queue* %q)
  call void @assertEqQueueNode(%QueueNode* %head2, %QueueNode* @.LAST, %TestName* @.headIsLast)
  
  ; Assert that q tail is LAST
  %tail2 = call %QueueNode* @getTail(%Queue* %q)
  call void @assertEqQueueNode(%QueueNode* %tail2, %QueueNode* @.LAST, %TestName* @.tailIsLast)

  ; Dequeue another function
  %dq3 = call %Function* @dequeue(%Queue* %q)

  ; Assert that q length is still 0
  %isStillEmpty = call i1 @isEmpty(%Queue* %q)
  call void @assertCond(i1 %isStillEmpty, %TestName* @.qNoSmaller)

  ; Assert that dq3 is null
  call void @assertEqFunction(%Function* %dq3, %Function* null, %TestName* @.dq3IsNull)
  
  ; Assert that q head is LAST
  %head3 = call %QueueNode* @getHead(%Queue* %q)
  call void @assertEqQueueNode(%QueueNode* %head3, %QueueNode* @.LAST, %TestName* @.headNotNull)

  call void @fDestroy(%Function* %dq1)
  call void @fDestroy(%Function* %dq2)
  call void @qDestroy(%Queue* %q)
  ret void
}

@.qBigger1     = private unnamed_addr constant %TestName c"Q Bigger 1  \00"
@.headIsN1     = private unnamed_addr constant %TestName c"Head Is N1  \00"
@.n1HasF1      = private unnamed_addr constant %TestName c"N1 Has F1   \00"
@.n1PtToLast   = private unnamed_addr constant %TestName c"N1PtToLast  \00"
@.qBigger2     = private unnamed_addr constant %TestName c"Q Bigger 2  \00"
@.n2HasF2      = private unnamed_addr constant %TestName c"N2 Has F2   \00"
@.headNoChange = private unnamed_addr constant %TestName c"HeadNoChange\00"
@.n1PtToN2     = private unnamed_addr constant %TestName c"N1 Pt To N2 \00"
@.n1NotPtToN3  = private unnamed_addr constant %TestName c"N1NotPtToN3 \00"
@.n2PtToN3     = private unnamed_addr constant %TestName c"N2 Pt To N3 \00"
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

  ; Enqueue another function
  %f3 = call %Function* @createICombinator()
  call void @enqueue(%Queue* %q, %Function* %f3)
  %node3 = call %QueueNode* @getTail(%Queue* %q)

  ; Assert that node1 still points to node2
  %node1Next3 = call %QueueNode* @getNext(%QueueNode* %node1)
  call void @assertEqQueueNode(%QueueNode* %node1Next3, %QueueNode* %node2, %TestName* @.n1NotPtToN3)

  ; Assert that node2 points to node3
  %node2Next = call %QueueNode* @getNext(%QueueNode* %node2)
  call void @assertEqQueueNode(%QueueNode* %node2Next, %QueueNode* %node3, %TestName* @.n2PtToN3)

  call void @qDestroy(%Queue* %q)
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
  %isEmpty = call i1 @isEmpty(%Queue* %arguments)
  call void @assertCond(i1 %isEmpty, %TestName* @.iArgsEmpty)

  call void @fDestroy(%Function* %i)
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

  call void @qDestroy(%Queue* %q)
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