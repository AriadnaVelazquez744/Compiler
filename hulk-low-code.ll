; ModuleID = 'hulk_module'
source_filename = "hulk_module"

@0 = private unnamed_addr constant [8 x i8] c"hello\0A\00\00", align 1
@1 = private unnamed_addr constant [4 x i8] c"%g\0A\00", align 1
@2 = private unnamed_addr constant [4 x i8] c"%g\0A\00", align 1
@3 = private unnamed_addr constant [4 x i8] c"%g\0A\00", align 1
@4 = private unnamed_addr constant [4 x i8] c"%g\0A\00", align 1
@5 = private unnamed_addr constant [4 x i8] c"%g\0A\00", align 1
@6 = private unnamed_addr constant [4 x i8] c"%g\0A\00", align 1
@7 = private unnamed_addr constant [4 x i8] c"%g\0A\00", align 1

declare i32 @printf(ptr, ...)

declare i32 @puts(ptr)

define i32 @main() {
entry:
  %mincall = call double @fmin(double 4.000000e+00, double 7.000000e+00)
  %maxcall = call double @fmax(double 6.000000e+00, double 9.000000e+00)
  %0 = call double @llvm.sin.f64(double 0.000000e+00)
  %1 = call double @llvm.cos.f64(double 0.000000e+00)
  %2 = call double @llvm.sqrt.f64(double 1.600000e+01)
  %logcall = call double @hulk_log_base(double 0.000000e+00, double 0.000000e+00)
  %3 = call i32 (ptr, ...) @printf(ptr @1, double %mincall)
  %4 = call i32 (ptr, ...) @printf(ptr @2, double %maxcall)
  %5 = call i32 (ptr, ...) @printf(ptr @3, double %0)
  %6 = call i32 (ptr, ...) @printf(ptr @4, double %1)
  %7 = call i32 (ptr, ...) @printf(ptr @5, double %2)
  %8 = call i32 @puts(ptr @0)
  %9 = call i32 (ptr, ...) @printf(ptr @6, double 1.000000e+00)
  %10 = call i32 (ptr, ...) @printf(ptr @7, double %logcall)
  ret i32 0
}

declare double @fmin(double, double)

declare double @fmax(double, double)

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare double @llvm.sin.f64(double) #0

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare double @llvm.cos.f64(double) #0

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare double @llvm.sqrt.f64(double) #0

declare double @hulk_log_base(double, double)

attributes #0 = { nocallback nofree nosync nounwind speculatable willreturn memory(none) }
