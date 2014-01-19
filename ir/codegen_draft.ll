; For test output in main
@.pass = private unnamed_addr constant [12 x i8] c"test passed\00"
@.fail = private unnamed_addr constant [12 x i8] c"test failed\00"
declare i32 @puts(i8* nocapture) nounwind

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