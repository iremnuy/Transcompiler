; ModuleID = 'advcalc2ir'
declare i32 @printf(i8*, ...)
@print.str = constant [4 x i8] c"%d\0A\00"

define i32 @main() {
	%x = alloca i32
	%y = alloca i32
	%zvalue = alloca i32
	%k = alloca i32
	store i32 3, i32* %x
	store i32 5, i32* %y
	%1 = load i32, i32* %x
	%2 = load i32, i32* %y
	%3 = add i32 %1, %2
	%4 = mul i32 %1, %3
	%5 = add i32 %23, %4
	store i32 %5, i32* %zvalue
	%6 = load i32, i32* %zvalue
	call i32 (i8*, ...) @printf(i8* getelementptr ([4 x i8], [4 x i8]* @print.str, i32 0, i32 0), i32 %6)
	%7 = load i32, i32* %x
	%8 = load i32, i32* %y
	%9 = load i32, i32* %zvalue
	%10 = load i32, i32* %y
	%11 = sub i32 %7, %8
	%12 = sub i32 %11, %9
	store i32 %12, i32* %k
	%13 = load i32, i32* %x
	%14 = load i32, i32* %y
	%15 = mul i32 %3, %14
	%16 = add i32 %2, %5
	%17 = mul i32 %1, %16
	%18 = mul i32 %15, %17
	%19 = add i32 %13, %18
	store i32 %19, i32* %k
	%20 = add i32 %999999, %1
	call i32 (i8*, ...) @printf(i8* getelementptr ([4 x i8], [4 x i8]* @print.str, i32 0, i32 0), i32 %21 )
	ret i32 0
}
