@.pass = private unnamed_addr constant [12 x i8] c"test passed\00"
@.fail = private unnamed_addr constant [12 x i8] c"test failed\00"
@.breakpoint = private unnamed_addr constant [12 x i8] c"breakpoint!\00"
;  %bkPtr = getelementptr [12 x i8]* @.breakpoint, i64 0, i64 0

declare i32 @puts(i8* nocapture) nounwind

%argListT = type [3 x %fcf*]
%fcf = type {%fcf(%argListT)*, i2, %argListT}

define i64 @length(%argListT* %list) {
entry:
  br label %loop
loop:
  %index = phi i64 [0, %entry], [%nextIndex, %iterate]
  %isAtCapacity = icmp eq i64 %index, 3
  br i1 %isAtCapacity, label %ret, label %inspectIndex
inspectIndex:
  %itemPtr = getelementptr %argListT* %list, i64 0, i64 %index
  %item = load %fcf** %itemPtr
  %isNullItem = icmp eq %fcf* %item, null
  br i1 %isNullItem, label %ret, label %iterate
iterate:
  %nextIndex = add i64 %index, 1
  br label %loop
ret:
  ret i64 %index
}

define %fcf** @push(%argListT* %list, %fcf* %item) {
entry:
  %length = call i64 @length(%argListT* %list)
  %listIsFull = icmp eq i64 %length, 3
  br i1 %listIsFull, label %error, label %success
success:
  %itemPtr = getelementptr %argListT* %list, i64 0, i64 %length
  store %fcf* %item, %fcf** %itemPtr
  ret %fcf** %itemPtr
error:
  ret %fcf** null
}

; TO APPLY an applicator to an input
; * ADD the input to the applicator's argument list
; * EVALUATE the applicator

; TO EVALUATE a function
; * GET THE ARITY of the function
; * GET THE NUMBER OF ARGUMENTS of the function
; * if number < arity, return the function
; * if number = arity, CALL the function
; * if number > arity, ERROR

define %fcf* @apply(%fcf* %applicator, %fcf* %input) {
entry:
  %resultP = alloca %fcf*
  store %fcf* %applicator, %fcf** %resultP

  %result = load %fcf** %resultP
  ret %fcf* %result
}

define %fcf* @I(%argListT %args) {
entry:
  %first = extractvalue %argListT %args, 0
  ret %fcf* %first
}

define %fcf* @K(%argListT %args) {
entry:
  %first = extractvalue %argListT %args, 0
  ret %fcf* %first
}

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
  ; Create list of 3 null pointers
  %list = alloca %argListT
  %item1 = getelementptr %argListT* %list, i64 0, i32 0
  %item2 = getelementptr %argListT* %list, i64 0, i32 1
  %item3 = getelementptr %argListT* %list, i64 0, i32 2
  store %fcf* null, %fcf** %item1
  store %fcf* null, %fcf** %item2
  store %fcf* null, %fcf** %item3

  ; Add item to list, assert length is 1
  %func1 = alloca %fcf
  store %fcf* %func1, %fcf** %item1
  %len1 = call i64 @length(%argListT* %list)
  %lengthIs1 = icmp eq i64 %len1, 1
  call i1 @assertCond(i1 %lengthIs1)

  ; Add item to list via @push function
  ; Assert item has been added to list, and that length is 2
  %func2 = alloca %fcf
  %pushRet = call %fcf** @push(%argListT* %list, %fcf* %func2)
  %pushRetDeref = load %fcf** %pushRet
  %itemWasPushed = icmp eq %fcf* %func2, %pushRetDeref
  call i1 @assertCond(i1 %itemWasPushed)
  %len2 = call i64 @length(%argListT* %list)
  %lengthIs2 = icmp eq i64 %len2, 2
  call i1 @assertCond(i1 %lengthIs2)

  ; Assert that @push return null pointer if list is full
  call %fcf** @push(%argListT* %list, %fcf* %func1)
  %pushRetFull = call %fcf** @push(%argListT* %list, %fcf* %func2)
  %ptrIsNull = icmp eq %fcf** %pushRetFull, null
  call i1 @assertCond(i1 %ptrIsNull)

  ret i1 0
}