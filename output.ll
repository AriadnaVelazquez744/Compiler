; ModuleID = 'hulk_module'
source_filename = "hulk_module"

@0 = private unnamed_addr constant [18 x i8] c"hello \\\22 world \\\22\00", align 1
@1 = private unnamed_addr constant [4 x i8] c"%g\0A\00", align 1
@2 = private unnamed_addr constant [7 x i8] c"false\0A\00", align 1
@3 = private unnamed_addr constant [6 x i8] c"true\0A\00", align 1

declare i32 @printf(ptr, ...)

declare i32 @puts(ptr)

define i32 @main() {
entry:
  %0 = call i32 (ptr, ...) @printf(ptr @1, double 3.000000e+00)
  %1 = call i32 @puts(ptr @0)
  %2 = call i32 @puts(ptr @3)
  ret i32 0
}
