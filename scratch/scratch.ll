@.pass = private unnamed_addr constant [12 x i8] c"test passed\00"
@.fail = private unnamed_addr constant [12 x i8] c"test failed\00"
@.breakpoint = private unnamed_addr constant [12 x i8] c"breakpoint!\00"
;  %bkPtr = getelementptr [12 x i8]* @.breakpoint, i64 0, i64 0

declare i32 @puts(i8* nocapture) nounwind

; TODO: find better names for these types and the underlying concepts
;   (Slalom functions vs. LLVM functions)
%argListT = type [3 x %fcf*]
%arityT = type i32
%signature = type %fcf*(%argListT*)*
%fcf = type {%signature, %arityT, %argListT}

; assumes argListT has size 3
define %arityT @length(%argListT* %list) {
entry:
  br label %loop
loop:
  %index = phi %arityT [0, %entry], [%nextIndex, %iterate]
  %isAtCapacity = icmp eq %arityT %index, 3
  br i1 %isAtCapacity, label %ret, label %inspectIndex
inspectIndex:
  %itemPtr = getelementptr %argListT* %list, i64 0, %arityT %index
  %item = load %fcf** %itemPtr
  %isNullItem = icmp eq %fcf* %item, null
  br i1 %isNullItem, label %ret, label %iterate
iterate:
  %nextIndex = add %arityT %index, 1
  br label %loop
ret:
  ret %arityT %index
}

; assumes argListT has size 3
define %fcf** @push(%argListT* %list, %fcf* %item) {
entry:
  %length = call %arityT @length(%argListT* %list)
  %listIsFull = icmp eq %arityT %length, 3
  br i1 %listIsFull, label %error, label %success
success:
  %itemPtr = getelementptr %argListT* %list, i64 0, %arityT %length
  store %fcf* %item, %fcf** %itemPtr
  ret %fcf** %itemPtr
error:
  ret %fcf** null
}

define %signature* @setFunctionPointer(%fcf* %slalfunc, %signature %funcValue) {
entry:
  %funcPtr = getelementptr %fcf* %slalfunc, i64 0, i32 0
  store %signature %funcValue, %signature* %funcPtr
  ret %signature* %funcPtr
}

; assumes argListT has size 3
define void @fillArgList(%argListT* %list, %fcf*, %fcf*, %fcf*) {
entry:
  %item1 = getelementptr %argListT* %list, i64 0, i32 0
  %item2 = getelementptr %argListT* %list, i64 0, i32 1
  %item3 = getelementptr %argListT* %list, i64 0, i32 2
  store %fcf* %0, %fcf** %item1
  store %fcf* %1, %fcf** %item2
  store %fcf* %2, %fcf** %item3
  ret void
}

; assumes argListT has size 3
define void @emptyArgList(%argListT* %list) {
entry:
  call void @fillArgList(%argListT* %list, %fcf* null, %fcf* null, %fcf* null)
  ret void
}

; TO EVALUATE a function
; * GET THE ARITY of the function
; * GET THE NUMBER OF ARGUMENTS of the function
; * if number < arity, return the function
; * if number = arity, CALL the function
; * if number > arity, ERROR
; NOT YET TESTED
define %fcf* @evaluate(%fcf* %slalfunc) {
  %arityP = getelementptr %fcf* %slalfunc, i64 0, i32 1
  %arity = load %arityT* %arityP

  %args = getelementptr %fcf* %slalfunc, i64 0, i32 2
  %argNum = call %arityT @length(%argListT* %args)

  %tooManyArgs = icmp ugt %arityT %argNum, %arity
  br i1 %tooManyArgs, label %error, label %validFunction
validFunction:
  %tooFewArgs = icmp ult %arityT %argNum, %arity
  br i1 %tooFewArgs, label %notReadyForCall, label %readyForCall
readyForCall:
  %funcP = getelementptr %fcf* %slalfunc, i64 0, i32 0
  %func = load %signature* %funcP
  %toReturn = call %fcf* %func(%argListT* %args)
  ret %fcf* %toReturn
notReadyForCall:
  ret %fcf* %slalfunc
error:
  ret %fcf* null
}


; TO APPLY an applicator to an input
; * ADD the input to the applicator's argument list
; * EVALUATE the applicator
define %fcf* @apply(%fcf* %applicator, %fcf* %input) {
entry:
  %resultP = alloca %fcf*
  store %fcf* %applicator, %fcf** %resultP

  %result = load %fcf** %resultP
  ret %fcf* %result
}

define %fcf* @I(%argListT* %args) {
entry:
  %firstItemP = getelementptr %argListT* %args, i64 0, i64 0
  %firstItem = load %fcf** %firstItemP
  ret %fcf* %firstItem
}

define %fcf* @K(%argListT* %args) {
entry:
  %firstItemP = getelementptr %argListT* %args, i64 0, i64 0
  %firstItem = load %fcf** %firstItemP
  ret %fcf* %firstItem
}

; TODO: change argument to %argListT*
define %fcf* @S(%argListT %args) {
entry:
  %first = extractvalue %argListT %args, 0
  %second = extractvalue %argListT %args, 1
  %third = extractvalue %argListT %args, 2
  %tmp13 = call %fcf* @apply(%fcf* %first, %fcf* %third)
  %tmp23 = call %fcf* @apply(%fcf* %second, %fcf* %third)
  %result = call %fcf* @apply(%fcf* %tmp13, %fcf* %tmp23)
  ret %fcf* %result
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