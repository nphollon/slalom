@.pass = private unnamed_addr constant [12 x i8] c"test passed\00"
@.fail = private unnamed_addr constant [12 x i8] c"test failed\00"

declare i32 @puts(i8* nocapture) nounwind

%argListT = type [3 x %fcf*]
%fcf = type {%fcf(%argListT)*, i2, %argListT}

; TO APPLY an applicator to an input
; * ADD the input to the applicator's argument list
; * EVALUATE the applicator

; TO ADD an item to a list
; * GET THE LENGTH of the list
; * GET THE CAPACITY of the list
; * if length < capacity, place the item at index (length + 1)
; * if length >= capacity, ERROR

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

define i1 @assert(i64, i64) {
entry:
  %cond = icmp eq i64 %0, %1
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

define i64 @length(%argListT* %list) {
entry:
  %list0 = load %argListT* %list
  %valueAt0 = extractvalue %argListT %list0, 0
  %listIsEmpty = icmp ne %fcf* %valueAt0, null
  %length = zext i1 %listIsEmpty to i64
  ret i64 %length
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

  ; Assert that list length is 0
  %len0 = call i64 @length(%argListT* %list)
  call i1 @assert(i64 %len0, i64 0)

  ; Add item to list, assert length is 1
  %function = alloca %fcf
  store %fcf* %function, %fcf** %item1
  %len1 = call i64 @length(%argListT* %list)
  call i1 @assert(i64 %len1, i64 1)

  ; Add item to list, assert length is 2
  store %fcf* %function, %fcf** %item2
  %len2 = call i64 @length(%argListT* %list)
  call i1 @assert(i64 %len1, i64 2)

  ret i1 0
}