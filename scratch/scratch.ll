; For test output in main
@.pass = private unnamed_addr constant [12 x i8] c"test passed\00"
@.fail = private unnamed_addr constant [12 x i8] c"test failed\00"
declare i32 @puts(i8* nocapture) nounwind

; TODO
; * @apply needs to handle list overflow
; * @evaluate needs to handle improper input (#args > arity)
; * return value of @S needs to persist
; * test @evaluate, @apply, @emptyInputs, and @S
; * create factory methods @getIComb, @getKComb, and @getSComb

; CONTRACTS
; functions that accept %Inputs* assume the argument is safe
; functions that accept %Function* assume the argument is unsafe
; functions that accept unsafe arguments will never fail
; functions that receive null %Function* will return null %Function*

%Arity = type i32
%Inputs = type [3 x %Function*]
%Body = type %Function*(%Inputs*)*
%Function = type {%Body, %Arity, %Inputs}

; @Capacity value must be <= size of %Inputs
@Capacity = private unnamed_addr constant %Arity 3

define %Arity @length(%Inputs* %list) {
entry:
  %capacity = load %Arity* @Capacity
  br label %loop
loop:
  %index = phi %Arity [0, %entry], [%nextIndex, %iterate]
  %isAtCapacity = icmp eq %Arity %index, %capacity
  br i1 %isAtCapacity, label %exit, label %inspectIndex
inspectIndex:
  %itemPtr = getelementptr %Inputs* %list, i64 0, %Arity %index
  %item = load %Function** %itemPtr
  %isNullItem = icmp eq %Function* %item, null
  br i1 %isNullItem, label %exit, label %iterate
iterate:
  %nextIndex = add %Arity %index, 1
  br label %loop
exit:
  ret %Arity %index
}

define %Function** @push(%Inputs* %list, %Function* %item) {
entry:
  %capacity = load %Arity* @Capacity
  %length = call %Arity @length(%Inputs* %list)
  %listIsFull = icmp eq %Arity %length, %capacity
  br i1 %listIsFull, label %error, label %success
success:
  %itemPtr = getelementptr %Inputs* %list, i64 0, %Arity %length
  store %Function* %item, %Function** %itemPtr
  ret %Function** %itemPtr
error:
  ret %Function** null
}

define void @emptyInputs(%Inputs* %list) {
entry:
  %capacity = load %Arity* @Capacity
  br label %loop
loop:
  %index = phi %Arity [0, %entry], [%nextIndex, %emptyIndex]
  %isAtCapacity = icmp eq %Arity %index, %capacity
  br i1 %isAtCapacity, label %exit, label %emptyIndex
emptyIndex:
  %itemPtr = getelementptr %Inputs* %list, i64 0, %Arity %index
  store %Function* null, %Function** %itemPtr
  %nextIndex = add %Arity %index, 1
  br label %loop
exit:
  ret void
}

define %Function* @evaluate(%Function* %function) {
entry:
  %isNull = icmp eq %Function* %function, null
  br i1 %isNull, label %error, label %checkArity

checkArity:
  %arityP = getelementptr %Function* %function, i64 0, i32 1
  %arity = load %Arity* %arityP

  %args = getelementptr %Function* %function, i64 0, i32 2
  %argNum = call %Arity @length(%Inputs* %args)

  %tooManyArgs = icmp ugt %Arity %argNum, %arity
  br i1 %tooManyArgs, label %error, label %properFunction

properFunction:
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
  %applicatorIsNull = icmp eq %Function* %applicator, null
  %inputIsNull = icmp eq %Function* %input, null
  %invalidArguments = or i1 %applicatorIsNull, %inputIsNull
  br i1 %invalidArguments, label %error, label %validArguments

validArguments:
  %args = getelementptr %Function* %applicator, i64 0, i32 2
  call %Function** @push(%Inputs* %args, %Function* %input)
  %toReturn = call %Function* @evaluate(%Function* %applicator)
  ret %Function* %toReturn

error:
  ret %Function* null
}

define %Function* @getItem(%Inputs* %list, %Arity %index) {
  %itemP = getelementptr %Inputs* %list, i64 0, %Arity %index
  %item = load %Function** %itemP
  ret %Function* %item
}

define %Function* @I(%Inputs* %args) {
entry:
  %firstItem = call %Function* @getItem(%Inputs* %args, %Arity 0)
  ret %Function* %firstItem
}

define %Function* @K(%Inputs* %args) {
entry:
  %firstItem = call %Function* @getItem(%Inputs* %args, %Arity 0)
  ret %Function* %firstItem
}

define %Function* @S(%Inputs* %args) {
entry:
  %firstItem = call %Function* @getItem(%Inputs* %args, %Arity 0)
  %secondItem = call %Function* @getItem(%Inputs* %args, %Arity 1)
  %thirdItem = call %Function* @getItem(%Inputs* %args, %Arity 2)
  %tmp13 = call %Function* @apply(%Function* %firstItem, %Function* %thirdItem)
  %tmp23 = call %Function* @apply(%Function* %secondItem, %Function* %thirdItem)
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