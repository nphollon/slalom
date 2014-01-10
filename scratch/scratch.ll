@.str = private unnamed_addr constant [12 x i8] c"hello world\00"

declare i32 @puts(i8* nocapture) nounwind

%argListT = type [3 x %fcf]
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

; TO GET THE LENGTH of a list

; TO GET THE CAPACITY of a list

; TO GET THE ARITY of a function

; TO GET THE NUMBER OF ARGUMENTS of a function

; TO CALL a function

; TO ERROR

define %fcf @apply(%fcf %applicator, %fcf %input) {
entry:
  %resultP = alloca %fcf
  store %fcf %applicator, %fcf* %resultP

  %result = load %fcf* %resultP
  ret %fcf %result
}

define %fcf @I(%argListT %args) {
entry:
  %first = extractvalue %argListT %args, 0
  ret %fcf %first
}

define %fcf @K(%argListT %args) {
entry:
  %first = extractvalue %argListT %args, 0
  ret %fcf %first
}

define %fcf @S(%argListT %args) {
entry:
  %first = extractvalue %argListT %args, 0
  %second = extractvalue %argListT %args, 1
  %third = extractvalue %argListT %args, 2
  %tmp13 = call %fcf @apply(%fcf %first, %fcf %third)
  %tmp23 = call %fcf @apply(%fcf %second, %fcf %third)
  %result = call %fcf @apply(%fcf %tmp13, %fcf %tmp23)
  ret %fcf %result
}

define i32 @main() {
  %cast210 = getelementptr [12 x i8]* @.str, i64 0, i64 0
  call i32 @puts(i8* %cast210)
  ret i32 0
}