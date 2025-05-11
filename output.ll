; ModuleID = 'hulk_module'
source_filename = "hulk_module"

@0 = private unnamed_addr constant [13 x i8] c"Hello, world\00", align 1

declare i32 @printf(ptr, ...)

declare i32 @puts(ptr)

define i32 @main() {
entry:
  %0 = call i32 @puts(ptr @0)
  ret i32 0
}
