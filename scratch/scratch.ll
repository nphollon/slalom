@.str = private unnamed_addr constant [12 x i8] c"hello world\00"

declare i32 @puts(i8* nocapture) nounwind

%slalfunc = type { %slalfunc* (%slalfunc*) *, i8 }

define %slalfunc* @I(%slalfunc* %x) {
  ret %slalfunc* %x
}

define i32 @main() {
  %cast210 = getelementptr [12 x i8]* @.str, i64 0, i64 0
  call i32 @puts(i8* %cast210)
  ret i32 0
}