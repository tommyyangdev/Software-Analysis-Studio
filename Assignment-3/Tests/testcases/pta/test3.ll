; ModuleID = './test5.ll'
source_filename = "./test5.c"
target datalayout = "e-m:o-i64:64-i128:128-n32:64-S128"
target triple = "arm64-apple-macosx14.0.0"

; Function Attrs: noinline nounwind ssp uwtable(sync)
define i32 @main() #0 !dbg !9 {
entry:
  %y2 = alloca ptr, align 8
  %y2_ = alloca ptr, align 8
  %y3 = alloca i32, align 4
  %z3 = alloca i32, align 4
  %y3_ = alloca i32, align 4
  call void @llvm.dbg.declare(metadata ptr %y2, metadata !15, metadata !DIExpression()), !dbg !17
  call void @llvm.dbg.declare(metadata ptr %y2_, metadata !18, metadata !DIExpression()), !dbg !19
  call void @llvm.dbg.declare(metadata ptr undef, metadata !20, metadata !DIExpression()), !dbg !21
  call void @llvm.dbg.declare(metadata ptr %y3, metadata !22, metadata !DIExpression()), !dbg !23
  call void @llvm.dbg.declare(metadata ptr %z3, metadata !24, metadata !DIExpression()), !dbg !25
  call void @llvm.dbg.declare(metadata ptr %y3_, metadata !26, metadata !DIExpression()), !dbg !27
  call void @llvm.dbg.value(metadata ptr undef, metadata !28, metadata !DIExpression()), !dbg !29
  store ptr %y3, ptr %y2, align 8, !dbg !30
  call void @llvm.dbg.value(metadata ptr %z3, metadata !31, metadata !DIExpression()), !dbg !29
  call void @llvm.dbg.value(metadata ptr undef, metadata !32, metadata !DIExpression()), !dbg !29
  call void @llvm.dbg.value(metadata ptr %y2, metadata !34, metadata !DIExpression()), !dbg !29
  call void @llvm.dbg.value(metadata ptr undef, metadata !35, metadata !DIExpression()), !dbg !29
  %0 = load i32, ptr %y3_, align 4, !dbg !36
  %tobool = icmp ne i32 %0, 0, !dbg !36
  br i1 %tobool, label %if.then, label %if.end, !dbg !38

if.then:                                          ; preds = %entry
  call void @llvm.dbg.value(metadata ptr %y2_, metadata !34, metadata !DIExpression()), !dbg !29
  store ptr %y3_, ptr %y2_, align 8, !dbg !39
  br label %if.end, !dbg !41

if.end:                                           ; preds = %if.then, %entry
  %y1.0 = phi ptr [ %y2_, %if.then ], [ %y2, %entry ], !dbg !29
  call void @llvm.dbg.value(metadata ptr %y1.0, metadata !34, metadata !DIExpression()), !dbg !29
  %1 = load ptr, ptr %y1.0, align 8, !dbg !42
  call void @llvm.dbg.value(metadata ptr %1, metadata !28, metadata !DIExpression()), !dbg !29
  store ptr %z3, ptr %y1.0, align 8, !dbg !43
  call void @llvm.dbg.value(metadata ptr %1, metadata !31, metadata !DIExpression()), !dbg !29
  %2 = load ptr, ptr %y2, align 8, !dbg !44
  call void @MAYALIAS(ptr noundef %1, ptr noundef %2), !dbg !45
  call void @MAYALIAS(ptr noundef %1, ptr noundef %1), !dbg !46
  ret i32 0, !dbg !47
}

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare void @llvm.dbg.declare(metadata, metadata, metadata) #1

declare void @MAYALIAS(ptr noundef, ptr noundef) #2

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare void @llvm.dbg.value(metadata, metadata, metadata) #1

attributes #0 = { noinline nounwind ssp uwtable(sync) "frame-pointer"="non-leaf" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="apple-m1" "target-features"="+aes,+crc,+crypto,+dotprod,+fp-armv8,+fp16fml,+fullfp16,+lse,+neon,+ras,+rcpc,+rdm,+sha2,+sha3,+sm4,+v8.1a,+v8.2a,+v8.3a,+v8.4a,+v8.5a,+v8a,+zcm,+zcz" }
attributes #1 = { nocallback nofree nosync nounwind speculatable willreturn memory(none) }
attributes #2 = { "frame-pointer"="non-leaf" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="apple-m1" "target-features"="+aes,+crc,+crypto,+dotprod,+fp-armv8,+fp16fml,+fullfp16,+lse,+neon,+ras,+rcpc,+rdm,+sha2,+sha3,+sm4,+v8.1a,+v8.2a,+v8.3a,+v8.4a,+v8.5a,+v8a,+zcm,+zcz" }

!llvm.dbg.cu = !{!0}
!llvm.module.flags = !{!2, !3, !4, !5, !6, !7}
!llvm.ident = !{!8}

!0 = distinct !DICompileUnit(language: DW_LANG_C11, file: !1, producer: "Homebrew clang version 16.0.6", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false, nameTableKind: None, sysroot: "/Library/Developer/CommandLineTools/SDKs/MacOSX14.sdk", sdk: "MacOSX14.sdk")
!1 = !DIFile(filename: "test5.c", directory: "/Users/z5489735/2023/0522/Software-Security-Analysis/Assignment-1/Tests/testcases/pta")
!2 = !{i32 7, !"Dwarf Version", i32 4}
!3 = !{i32 2, !"Debug Info Version", i32 3}
!4 = !{i32 1, !"wchar_size", i32 4}
!5 = !{i32 8, !"PIC Level", i32 2}
!6 = !{i32 7, !"uwtable", i32 1}
!7 = !{i32 7, !"frame-pointer", i32 1}
!8 = !{!"Homebrew clang version 16.0.6"}
!9 = distinct !DISubprogram(name: "main", scope: !10, file: !10, line: 8, type: !11, scopeLine: 8, spFlags: DISPFlagDefinition, unit: !0, retainedNodes: !14)
!10 = !DIFile(filename: "./test5.c", directory: "/Users/z5489735/2023/0522/Software-Security-Analysis/Assignment-1/Tests/testcases/pta")
!11 = !DISubroutineType(types: !12)
!12 = !{!13}
!13 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!14 = !{}
!15 = !DILocalVariable(name: "y2", scope: !9, file: !10, line: 10, type: !16)
!16 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !13, size: 64)
!17 = !DILocation(line: 10, column: 12, scope: !9)
!18 = !DILocalVariable(name: "y2_", scope: !9, file: !10, line: 10, type: !16)
!19 = !DILocation(line: 10, column: 22, scope: !9)
!20 = !DILocalVariable(name: "x3", scope: !9, file: !10, line: 11, type: !13)
!21 = !DILocation(line: 11, column: 6, scope: !9)
!22 = !DILocalVariable(name: "y3", scope: !9, file: !10, line: 11, type: !13)
!23 = !DILocation(line: 11, column: 10, scope: !9)
!24 = !DILocalVariable(name: "z3", scope: !9, file: !10, line: 11, type: !13)
!25 = !DILocation(line: 11, column: 14, scope: !9)
!26 = !DILocalVariable(name: "y3_", scope: !9, file: !10, line: 11, type: !13)
!27 = !DILocation(line: 11, column: 18, scope: !9)
!28 = !DILocalVariable(name: "x2", scope: !9, file: !10, line: 10, type: !16)
!29 = !DILocation(line: 0, scope: !9)
!30 = !DILocation(line: 12, column: 15, scope: !9)
!31 = !DILocalVariable(name: "z2", scope: !9, file: !10, line: 10, type: !16)
!32 = !DILocalVariable(name: "x1", scope: !9, file: !10, line: 9, type: !33)
!33 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !16, size: 64)
!34 = !DILocalVariable(name: "y1", scope: !9, file: !10, line: 9, type: !33)
!35 = !DILocalVariable(name: "z1", scope: !9, file: !10, line: 9, type: !33)
!36 = !DILocation(line: 17, column: 6, scope: !37)
!37 = distinct !DILexicalBlock(scope: !9, file: !10, line: 17, column: 6)
!38 = !DILocation(line: 17, column: 6, scope: !9)
!39 = !DILocation(line: 19, column: 7, scope: !40)
!40 = distinct !DILexicalBlock(scope: !37, file: !10, line: 17, column: 11)
!41 = !DILocation(line: 20, column: 2, scope: !40)
!42 = !DILocation(line: 21, column: 8, scope: !9)
!43 = !DILocation(line: 22, column: 6, scope: !9)
!44 = !DILocation(line: 26, column: 15, scope: !9)
!45 = !DILocation(line: 26, column: 2, scope: !9)
!46 = !DILocation(line: 27, column: 2, scope: !9)
!47 = !DILocation(line: 28, column: 2, scope: !9)