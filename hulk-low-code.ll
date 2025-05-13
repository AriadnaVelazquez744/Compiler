; ModuleID = 'hulk_module'
source_filename = "hulk_module"

@0 = private unnamed_addr constant [4 x i8] c"%g\0A\00", align 1
@1 = private unnamed_addr constant [6 x i8] c"hola\00\00", align 1

declare i32 @printf(ptr, ...)

declare i32 @puts(ptr)

define i32 @main() {
entry:
  %0 = call i32 (ptr, ...) @printf(ptr @0, double 1.000000e+00)
  %1 = call i32 @puts(ptr @1)
  ret i32 0
}
