define i32 @gcd(i32 %x, i32 %y) {
entry:
  %xEqualsY = icmp eq i32 %x, %y
  br i1 %xEqualsY, label %ret, label %cond_false_1
ret:
  ret i32 %x
cond_false_1:
  %xLessThanY = icmp ult i32 %x, %y
  br i1 %xLessThanY, label %cond_true, label %cond_false_2
cond_true:
  %yMinusX = sub i32 %y, %x
  %recur_1 = call i32 @gcd(i32 %x, i32 %yMinusX)
  ret i32 %recur_1
cond_false_2:
  %xMinusY = sub i32 %x, %y
  %recur_2 = call i32 @gcd(i32 %xMinusY, i32 %y)
  ret i32 %recur_2
}

define void @main() {
entry:
  ret void
}