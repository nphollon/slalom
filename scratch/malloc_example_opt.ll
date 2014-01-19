; ModuleID = 'malloc_example.c'
target datalayout = "e-p:32:32:32-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:32:64-f32:32:32-f64:32:64-v64:64:64-v128:128:128-a0:0:64-f80:32:32-n8:16:32-S128"
target triple = "i386-pc-linux-gnu"

%struct.Data = type { i32 }

define noalias i32* @makeInteger(i32 %i) nounwind {
  %1 = tail call noalias i8* @malloc(i32 4) nounwind
  %2 = bitcast i8* %1 to i32*
  ret i32* %2
}

declare noalias i8* @malloc(i32) nounwind

define void @makeData1(%struct.Data* noalias nocapture sret %agg.result, i32 %i) nounwind {
  %agg.result.0 = getelementptr inbounds %struct.Data* %agg.result, i32 0, i32 0
  store i32 %i, i32* %agg.result.0, align 4
  ret void
}

define noalias %struct.Data* @makeData2(i32 %i) nounwind {
  %1 = tail call noalias i8* @malloc(i32 4) nounwind
  %2 = bitcast i8* %1 to %struct.Data*
  %3 = bitcast i8* %1 to i32*
  store i32 %i, i32* %3, align 4, !tbaa !0
  ret %struct.Data* %2
}

define i32 @main() nounwind {
  %data1 = alloca %struct.Data, align 4
  %1 = call i32* @makeInteger(i32 undef)
  call void @makeData1(%struct.Data* sret %data1, i32 5)
  %2 = call %struct.Data* @makeData2(i32 5)
  %3 = bitcast i32* %1 to i8*
  call void @free(i8* %3) nounwind
  %4 = bitcast %struct.Data* %2 to i8*
  call void @free(i8* %4) nounwind
  ret i32 0
}

declare void @free(i8* nocapture) nounwind

!0 = metadata !{metadata !"int", metadata !1}
!1 = metadata !{metadata !"omnipotent char", metadata !2}
!2 = metadata !{metadata !"Simple C/C++ TBAA", null}
