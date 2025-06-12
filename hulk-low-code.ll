; ModuleID = 'hulk_module'
source_filename = "hulk_module"

@0 = private unnamed_addr constant [7 x i8] c"false\0A\00", align 1
@1 = private unnamed_addr constant [6 x i8] c"true\0A\00", align 1

declare i32 @printf(ptr, ...)

declare i32 @puts(ptr)

define i32 @main() {
entry:
  %0 = call i32 @puts(ptr @0)
  ret i32 0
}
