; ModuleID = 'toconvert.cpp'
target datalayout = "e-p:32:32:32-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:32:64-f32:32:32-f64:32:64-v64:64:64-v128:128:128-a0:0:64-f80:32:32-n8:16:32-S128"
target triple = "i386-pc-linux-gnu"

@.str = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

define i32 @_Z1Ii(i32 %x) nounwind {
  %1 = alloca i32, align 4
  store i32 %x, i32* %1, align 4
  %2 = load i32* %1, align 4
  ret i32 %2
}

define i32 (i32)* @_Z13getPointerToIv() nounwind {
  %functionPointer = alloca i32 (i32)*, align 4
  store i32 (i32)* @_Z1Ii, i32 (i32)** %functionPointer, align 4
  %1 = load i32 (i32)** %functionPointer, align 4
  ret i32 (i32)* %1
}

define i32 @main() {
  %1 = alloca i32, align 4
  store i32 0, i32* %1
  %2 = call i32 (i32)* ()* @_Z13getPointerToIv()
  %3 = call i32 %2(i32 1)
  %4 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([4 x i8]* @.str, i32 0, i32 0), i32 %3)
  ret i32 0
}

declare i32 @printf(i8*, ...)
