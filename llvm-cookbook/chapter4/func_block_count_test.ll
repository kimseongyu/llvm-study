; ModuleID = 'func_block_count_test.c'
source_filename = "func_block_count_test.c"
target datalayout = "e-m:o-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-n32:64-S128-Fn32"
target triple = "arm64-apple-macosx15.0.0"

; Function Attrs: noinline nounwind  ssp uwtable(sync)
define i32 @main(i32 noundef %0, ptr noundef %1) #0 {
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  %5 = alloca ptr, align 8
  %6 = alloca i32, align 4
  %7 = alloca i32, align 4
  %8 = alloca i32, align 4
  %9 = alloca i32, align 4
  store i32 0, ptr %3, align 4
  store i32 %0, ptr %4, align 4
  store ptr %1, ptr %5, align 8
  store i32 0, ptr %9, align 4
  store i32 0, ptr %6, align 4
  br label %10

10:                                               ; preds = %42, %2
  %11 = load i32, ptr %6, align 4
  %12 = icmp slt i32 %11, 10
  br i1 %12, label %13, label %45

13:                                               ; preds = %10
  store i32 0, ptr %7, align 4
  br label %14

14:                                               ; preds = %28, %13
  %15 = load i32, ptr %7, align 4
  %16 = icmp slt i32 %15, 10
  br i1 %16, label %17, label %31

17:                                               ; preds = %14
  store i32 0, ptr %8, align 4
  br label %18

18:                                               ; preds = %24, %17
  %19 = load i32, ptr %8, align 4
  %20 = icmp slt i32 %19, 10
  br i1 %20, label %21, label %27

21:                                               ; preds = %18
  %22 = load i32, ptr %9, align 4
  %23 = add nsw i32 %22, 1
  store i32 %23, ptr %9, align 4
  br label %24

24:                                               ; preds = %21
  %25 = load i32, ptr %8, align 4
  %26 = add nsw i32 %25, 1
  store i32 %26, ptr %8, align 4
  br label %18, !llvm.loop !6

27:                                               ; preds = %18
  br label %28

28:                                               ; preds = %27
  %29 = load i32, ptr %7, align 4
  %30 = add nsw i32 %29, 1
  store i32 %30, ptr %7, align 4
  br label %14, !llvm.loop !8

31:                                               ; preds = %14
  store i32 0, ptr %7, align 4
  br label %32

32:                                               ; preds = %38, %31
  %33 = load i32, ptr %7, align 4
  %34 = icmp slt i32 %33, 10
  br i1 %34, label %35, label %41

35:                                               ; preds = %32
  %36 = load i32, ptr %9, align 4
  %37 = add nsw i32 %36, 1
  store i32 %37, ptr %9, align 4
  br label %38

38:                                               ; preds = %35
  %39 = load i32, ptr %7, align 4
  %40 = add nsw i32 %39, 1
  store i32 %40, ptr %7, align 4
  br label %32, !llvm.loop !9

41:                                               ; preds = %32
  br label %42

42:                                               ; preds = %41
  %43 = load i32, ptr %6, align 4
  %44 = add nsw i32 %43, 1
  store i32 %44, ptr %6, align 4
  br label %10, !llvm.loop !10

45:                                               ; preds = %10
  store i32 0, ptr %6, align 4
  br label %46

46:                                               ; preds = %70, %45
  %47 = load i32, ptr %6, align 4
  %48 = icmp slt i32 %47, 20
  br i1 %48, label %49, label %73

49:                                               ; preds = %46
  store i32 0, ptr %7, align 4
  br label %50

50:                                               ; preds = %56, %49
  %51 = load i32, ptr %7, align 4
  %52 = icmp slt i32 %51, 20
  br i1 %52, label %53, label %59

53:                                               ; preds = %50
  %54 = load i32, ptr %9, align 4
  %55 = add nsw i32 %54, 1
  store i32 %55, ptr %9, align 4
  br label %56

56:                                               ; preds = %53
  %57 = load i32, ptr %7, align 4
  %58 = add nsw i32 %57, 1
  store i32 %58, ptr %7, align 4
  br label %50, !llvm.loop !11

59:                                               ; preds = %50
  store i32 0, ptr %7, align 4
  br label %60

60:                                               ; preds = %66, %59
  %61 = load i32, ptr %7, align 4
  %62 = icmp slt i32 %61, 20
  br i1 %62, label %63, label %69

63:                                               ; preds = %60
  %64 = load i32, ptr %9, align 4
  %65 = add nsw i32 %64, 1
  store i32 %65, ptr %9, align 4
  br label %66

66:                                               ; preds = %63
  %67 = load i32, ptr %7, align 4
  %68 = add nsw i32 %67, 1
  store i32 %68, ptr %7, align 4
  br label %60, !llvm.loop !12

69:                                               ; preds = %60
  br label %70

70:                                               ; preds = %69
  %71 = load i32, ptr %6, align 4
  %72 = add nsw i32 %71, 1
  store i32 %72, ptr %6, align 4
  br label %46, !llvm.loop !13

73:                                               ; preds = %46
  %74 = load i32, ptr %9, align 4
  ret i32 %74
}

attributes #0 = { noinline nounwind  ssp uwtable(sync) "frame-pointer"="non-leaf" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="apple-m1" "target-features"="+aes,+altnzcv,+ccdp,+ccidx,+ccpp,+complxnum,+crc,+dit,+dotprod,+flagm,+fp-armv8,+fp16fml,+fptoint,+fullfp16,+jsconv,+lse,+neon,+pauth,+perfmon,+predres,+ras,+rcpc,+rdm,+sb,+sha2,+sha3,+specrestrict,+ssbs,+v8.1a,+v8.2a,+v8.3a,+v8.4a,+v8a,+zcm,+zcz" }

!llvm.module.flags = !{!0, !1, !2, !3, !4}
!llvm.ident = !{!5}

!0 = !{i32 2, !"SDK Version", [2 x i32] [i32 15, i32 4]}
!1 = !{i32 1, !"wchar_size", i32 4}
!2 = !{i32 8, !"PIC Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 1}
!4 = !{i32 7, !"frame-pointer", i32 1}
!5 = !{!"Homebrew clang version 20.1.3"}
!6 = distinct !{!6, !7}
!7 = !{!"llvm.loop.mustprogress"}
!8 = distinct !{!8, !7}
!9 = distinct !{!9, !7}
!10 = distinct !{!10, !7}
!11 = distinct !{!11, !7}
!12 = distinct !{!12, !7}
!13 = distinct !{!13, !7}
