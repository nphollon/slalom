@.pass = private unnamed_addr constant [12 x i8] c"test passed\00"
@.fail = private unnamed_addr constant [12 x i8] c"test failed\00"
@.breakpoint = private unnamed_addr constant [12 x i8] c"breakpoint!\00"
;  %bkPtr = getelementptr [12 x i8]* @.breakpoint, i64 0, i64 0

declare i32 @puts(i8* nocapture) nounwind

; TODO
; * @length, @push, @fillArgList & @emptyArgList have magic number 3
; * where should errors (null pointers) be checked? should they propagate through @apply?
; * test @evaluate and @apply
; * fix signature of @S
; * create factory methods @getIComb, @getKComb, and @getSComb

%Inputs = type [3 x %Function*]
%Arity = type i32
%Body = type %Function*(%Inputs*)*
%Function = type {%Body, %Arity, %Inputs}

define %Arity @length(%Inputs* %list) {
entry:
  br label %loop
loop:
  %index = phi %Arity [0, %entry], [%nextIndex, %iterate]
  %isAtCapacity = icmp eq %Arity %index, 3
  br i1 %isAtCapacity, label %ret, label %inspectIndex
inspectIndex:
  %itemPtr = getelementptr %Inputs* %list, i64 0, %Arity %index
  %item = load %Function** %itemPtr
  %isNullItem = icmp eq %Function* %item, null
  br i1 %isNullItem, label %ret, label %iterate
iterate:
  %nextIndex = add %Arity %index, 1
  br label %loop
ret:
  ret %Arity %index
}

define %Function** @push(%Inputs* %list, %Function* %item) {
entry:
  %length = call %Arity @length(%Inputs* %list)
  %listIsFull = icmp eq %Arity %length, 3
  br i1 %listIsFull, label %error, label %success
success:
  %itemPtr = getelementptr %Inputs* %list, i64 0, %Arity %length
  store %Function* %item, %Function** %itemPtr
  ret %Function** %itemPtr
error:
  ret %Function** null
}

define void @fillArgList(%Inputs* %list, %Function*, %Function*, %Function*) {
entry:
  %item1 = getelementptr %Inputs* %list, i64 0, i32 0
  %item2 = getelementptr %Inputs* %list, i64 0, i32 1
  %item3 = getelementptr %Inputs* %list, i64 0, i32 2
  store %Function* %0, %Function** %item1
  store %Function* %1, %Function** %item2
  store %Function* %2, %Function** %item3
  ret void
}

define void @emptyArgList(%Inputs* %list) {
entry:
  call void @fillArgList(%Inputs* %list, %Function* null, %Function* null, %Function* null)
  ret void
}

define %Function* @evaluate(%Function* %function) {
  %arityP = getelementptr %Function* %function, i64 0, i32 1
  %arity = load %Arity* %arityP

  %args = getelementptr %Function* %function, i64 0, i32 2
  %argNum = call %Arity @length(%Inputs* %args)

  %tooManyArgs = icmp ugt %Arity %argNum, %arity
  br i1 %tooManyArgs, label %error, label %validFunction

validFunction:
  %tooFewArgs = icmp ult %Arity %argNum, %arity
  br i1 %tooFewArgs, label %notReadyForCall, label %readyForCall

readyForCall:
  %bodyP = getelementptr %Function* %function, i64 0, i32 0
  %body = load %Body* %bodyP
  %toReturn = call %Function* %body(%Inputs* %args)
  ret %Function* %toReturn

notReadyForCall:
  ret %Function* %function

error:
  ret %Function* null
}

define %Function* @apply(%Function* %applicator, %Function* %input) {
entry:
  %args = getelementptr %Function* %applicator, i64 0, i32 2
  call %Function** @push(%Inputs* %args, %Function* %input)
  %toReturn = call %Function* @evaluate(%Function* %applicator)
  ret %Function* %toReturn
}

define %Function* @I(%Inputs* %args) {
entry:
  %firstItemP = getelementptr %Inputs* %args, i64 0, i64 0
  %firstItem = load %Function** %firstItemP
  ret %Function* %firstItem
}

define %Function* @K(%Inputs* %args) {
entry:
  %firstItemP = getelementptr %Inputs* %args, i64 0, i64 0
  %firstItem = load %Function** %firstItemP
  ret %Function* %firstItem
}

define %Function* @S(%Inputs %args) {
entry:
  %first = extractvalue %Inputs %args, 0
  %second = extractvalue %Inputs %args, 1
  %third = extractvalue %Inputs %args, 2
  %tmp13 = call %Function* @apply(%Function* %first, %Function* %third)
  %tmp23 = call %Function* @apply(%Function* %second, %Function* %third)
  %result = call %Function* @apply(%Function* %tmp13, %Function* %tmp23)
  ret %Function* %result
}

define i1 @assertCond(i1 %cond) {
  br i1 %cond, label %testPassed, label %testFailed
testPassed:
  %passPtr = getelementptr [12 x i8]* @.pass, i64 0, i64 0
  call i32 @puts(i8* %passPtr)
  ret i1 0
testFailed:
  %failPtr = getelementptr [12 x i8]* @.fail, i64 0, i64 0
  call i32 @puts(i8* %failPtr)
  ret i1 1
}

define i1 @main() {
entry:
  ret i1 0
}