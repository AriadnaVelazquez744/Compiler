; ModuleID = 'hulk_module'
source_filename = "hulk_module"

@0 = private unnamed_addr constant [6 x i8] c"hola\00\00", align 1
@1 = private unnamed_addr constant [6 x i8] c"hola\00\00", align 1
@2 = private unnamed_addr constant [6 x i8] c"hola\00\00", align 1
@3 = private unnamed_addr constant [6 x i8] c"hola\00\00", align 1
@4 = private unnamed_addr constant [6 x i8] c"hola\00\00", align 1
@5 = private unnamed_addr constant [7 x i8] c"mundo\00\00", align 1
@6 = private unnamed_addr constant [6 x i8] c"hola\00\00", align 1
@7 = private unnamed_addr constant [7 x i8] c"mundo\00\00", align 1
@8 = private unnamed_addr constant [7 x i8] c"false\0A\00", align 1
@9 = private unnamed_addr constant [6 x i8] c"true\0A\00", align 1
@10 = private unnamed_addr constant [7 x i8] c"false\0A\00", align 1
@11 = private unnamed_addr constant [6 x i8] c"true\0A\00", align 1
@12 = private unnamed_addr constant [7 x i8] c"false\0A\00", align 1
@13 = private unnamed_addr constant [6 x i8] c"true\0A\00", align 1
@14 = private unnamed_addr constant [7 x i8] c"false\0A\00", align 1
@15 = private unnamed_addr constant [6 x i8] c"true\0A\00", align 1
@16 = private unnamed_addr constant [7 x i8] c"false\0A\00", align 1
@17 = private unnamed_addr constant [6 x i8] c"true\0A\00", align 1
@18 = private unnamed_addr constant [7 x i8] c"false\0A\00", align 1
@19 = private unnamed_addr constant [6 x i8] c"true\0A\00", align 1
@20 = private unnamed_addr constant [7 x i8] c"false\0A\00", align 1
@21 = private unnamed_addr constant [6 x i8] c"true\0A\00", align 1
@22 = private unnamed_addr constant [7 x i8] c"false\0A\00", align 1
@23 = private unnamed_addr constant [6 x i8] c"true\0A\00", align 1
@24 = private unnamed_addr constant [7 x i8] c"false\0A\00", align 1
@25 = private unnamed_addr constant [6 x i8] c"true\0A\00", align 1
@26 = private unnamed_addr constant [7 x i8] c"false\0A\00", align 1
@27 = private unnamed_addr constant [6 x i8] c"true\0A\00", align 1
@28 = private unnamed_addr constant [7 x i8] c"false\0A\00", align 1
@29 = private unnamed_addr constant [6 x i8] c"true\0A\00", align 1
@30 = private unnamed_addr constant [7 x i8] c"false\0A\00", align 1
@31 = private unnamed_addr constant [6 x i8] c"true\0A\00", align 1
@32 = private unnamed_addr constant [4 x i8] c"%g\0A\00", align 1
@33 = private unnamed_addr constant [4 x i8] c"%g\0A\00", align 1
@34 = private unnamed_addr constant [4 x i8] c"%g\0A\00", align 1
@35 = private unnamed_addr constant [4 x i8] c"%g\0A\00", align 1
@36 = private unnamed_addr constant [4 x i8] c"%g\0A\00", align 1
@37 = private unnamed_addr constant [4 x i8] c"%g\0A\00", align 1

declare i32 @printf(ptr, ...)

declare i32 @puts(ptr)

define i32 @main() {
entry:
  %strequal = call i1 @hulk_str_equals(ptr @0, ptr @1)
  %strequal1 = call i1 @hulk_str_equals(ptr @2, ptr @3)
  %strnotequal = xor i1 %strequal1, true
  %concat = call ptr @hulk_str_concat(ptr @4, ptr @5)
  %concat2 = call ptr @hulk_str_concat_space(ptr @6, ptr @7)
  %powtmp = call double @llvm.pow.f64(double 2.000000e+00, double 3.000000e+00)
  %powtmp3 = call double @llvm.pow.f64(double 2.000000e+00, double 4.000000e+00)
  %0 = call i32 @puts(ptr @9)
  %1 = call i32 @puts(ptr @10)
  %2 = call i32 @puts(ptr @13)
  %3 = call i32 @puts(ptr @15)
  %4 = call i32 @puts(ptr @17)
  %5 = call i32 @puts(ptr @18)
  %6 = select i1 %strequal, ptr @21, ptr @20
  %7 = call i32 @puts(ptr %6)
  %8 = select i1 %strnotequal, ptr @23, ptr @22
  %9 = call i32 @puts(ptr %8)
  %10 = call i32 @puts(ptr @24)
  %11 = call i32 @puts(ptr @27)
  %12 = call i32 @puts(ptr @28)
  %13 = call i32 @puts(ptr @31)
  %14 = call i32 @puts(ptr %concat)
  %15 = call i32 @puts(ptr %concat2)
  %16 = call i32 (ptr, ...) @printf(ptr @32, double 7.000000e+00)
  %17 = call i32 (ptr, ...) @printf(ptr @33, double -1.000000e+00)
  %18 = call i32 (ptr, ...) @printf(ptr @34, double 3.000000e+00)
  %19 = call i32 (ptr, ...) @printf(ptr @35, double 1.200000e+01)
  %20 = call i32 (ptr, ...) @printf(ptr @36, double %powtmp)
  %21 = call i32 (ptr, ...) @printf(ptr @37, double %powtmp3)
  ret i32 0
}

declare i1 @hulk_str_equals(ptr, ptr)

declare ptr @hulk_str_concat(ptr, ptr)

declare ptr @hulk_str_concat_space(ptr, ptr)

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare double @llvm.pow.f64(double, double) #0

attributes #0 = { nocallback nofree nosync nounwind speculatable willreturn memory(none) }
