; ModuleID = 'advcalc2ir'
declare i32 @printf(i8*, ...)
@print.str = constant [4 x i8] c"%d\0A\00"

define i32 @main() {
	%siu = alloca i32
	%siuuu = alloca i32
	store i32 11, i32* %siu
	store i32 7, i32* %siuuu
	%3 = load i32, i32* %siu
	%4 = load i32, i32* %siuuu
	%5 = sdiv i32 %0, %0
	call i32 (i8*, ...) @printf(i8* getelementptr ([4 x i8], [4 x i8]* @print.str, i32 0, i32 0), i32  )
	%8 = load i32, i32* %siu
	%9 = load i32, i32* %siuuu
	%10 = mul i32 %0, %0
	store i32 77, i32* %siu
	%16 = load i32, i32* %siu
	%17 = load i32, i32* %siu
	%18 = sub i32 %0, %0
	%19 = load i32, i32* %siu
	%20 = load i32, i32* %siu
	%21 = mul i32 %0, %0
	%22 = load i32, i32* %siu
	%23 = sdiv i32 %0, %0
	%24 = add i32 %0, %0
	call i32 (i8*, ...) @printf(i8* getelementptr ([4 x i8], [4 x i8]* @print.str, i32 0, i32 0), i32  )
	ret i32 0
}
