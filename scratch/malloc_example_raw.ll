; ModuleID = 'toconvert.c'
target datalayout = "e-p:32:32:32-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:32:64-f32:32:32-f64:32:64-v64:64:64-v128:128:128-a0:0:64-f80:32:32-n8:16:32-S128"
target triple = "i386-pc-linux-gnu"

%struct.Data = type { i32 }

define i32* @makeInteger(i32 %i) nounwind {
  %1 = alloca i32, align 4
  store i32 %i, i32* %1, align 4
  %2 = call noalias i8* @malloc(i32 4) nounwind
  %3 = bitcast i8* %2 to i32*
  ret i32* %3
}

declare noalias i8* @malloc(i32) nounwind

define void @makeData1(%struct.Data* noalias sret %agg.result, i32 %i) nounwind {
  %1 = alloca i32, align 4
  %toReturn = alloca %struct.Data, align 4
  store i32 %i, i32* %1, align 4
  %2 = load i32* %1, align 4
  %3 = getelementptr inbounds %struct.Data* %toReturn, i32 0, i32 0
  store i32 %2, i32* %3, align 4
  %4 = bitcast %struct.Data* %agg.result to i8*
  %5 = bitcast %struct.Data* %toReturn to i8*
  call void @llvm.memcpy.p0i8.p0i8.i32(i8* %4, i8* %5, i32 4, i32 4, i1 false)
  ret void
}

declare void @llvm.memcpy.p0i8.p0i8.i32(i8* nocapture, i8* nocapture, i32, i32, i1) nounwind

define %struct.Data* @makeData2(i32 %i) nounwind {
  %1 = alloca i32, align 4
  %toReturn = alloca %struct.Data*, align 4
  store i32 %i, i32* %1, align 4
  %2 = call noalias i8* @malloc(i32 4) nounwind
  %3 = bitcast i8* %2 to %struct.Data*
  store %struct.Data* %3, %struct.Data** %toReturn, align 4
  %4 = load i32* %1, align 4
  %5 = load %struct.Data** %toReturn, align 4
  %6 = getelementptr inbounds %struct.Data* %5, i32 0, i32 0
  store i32 %4, i32* %6, align 4
  %7 = load %struct.Data** %toReturn, align 4
  ret %struct.Data* %7
}

define i32 @main() nounwind {
  %1 = alloca i32, align 4
  %integer = alloca i32*, align 4
  %data1 = alloca %struct.Data, align 4
  %data2 = alloca %struct.Data*, align 4
  store i32 0, i32* %1
  %2 = call i32* @makeInteger(i32 5)
  store i32* %2, i32** %integer, align 4
  call void @makeData1(%struct.Data* sret %data1, i32 5)
  %3 = call %struct.Data* @makeData2(i32 5)
  store %struct.Data* %3, %struct.Data** %data2, align 4
  %4 = load i32** %integer, align 4
  %5 = bitcast i32* %4 to i8*
  call void @free(i8* %5) nounwind
  %6 = load %struct.Data** %data2, align 4
  %7 = bitcast %struct.Data* %6 to i8*
  call void @free(i8* %7) nounwind
  ret i32 0
}

declare void @free(i8*) nounwind
