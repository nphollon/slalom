; ModuleID = 'sizeof_example.c'
target datalayout = "e-p:32:32:32-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:32:64-f32:32:32-f64:32:64-v64:64:64-v128:128:128-a0:0:64-f80:32:32-n8:16:32-S128"
target triple = "i386-pc-linux-gnu"

define i32 @main() nounwind {
  %1 = alloca i32, align 4
  %x = alloca i32, align 4
  %y = alloca i32, align 4
  %z = alloca i32, align 4
  store i32 0, i32* %1
  store i32 4, i32* %x, align 4
  store i32 8, i32* %y, align 4
  store i32 12, i32* %z, align 4
  %2 = load i32* %x, align 4
  %3 = load i32* %y, align 4
  %4 = add nsw i32 %2, %3
  %5 = load i32* %z, align 4
  %6 = add nsw i32 %4, %5
  ret i32 %6
}
