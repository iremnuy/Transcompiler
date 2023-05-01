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
	%3 = add i32 1, %2
	%4 = mul i32 %1, %3
 	%5 = add i32 23, %4
	store i32 %5, i32* %zvalue
	%6 = load i32, i32* %zvalue
	call i32 (i8*, ...) @printf(i8* getelementptr ([4 x i8], [4 x i8]* @print.str, i32 0, i32 0), i32 %6)
	%7 = load i32, i32* %x
	%8 = load i32, i32* %y
	%9 = load i32, i32* %zvalue
	%10 = sub i32 %7, %8
	%11 = sub i32 %10, %9
	store i32 %11, i32* %k
	%12 = load i32, i32* %x
	%13 = load i32, i32* %y
	%14 = mul i32 3, %13
 	%15 = add i32 2, 5
	%16 = mul i32 1, %15
 	%17 = mul i32 %14, %16
 	%18 = add i32 %12, %17
	store i32 %18, i32* %k
	%19 = load i32, i32* %k
	%20 = add i32 %19, 1
	call i32 (i8*, ...) @printf(i8* getelementptr ([4 x i8], [4 x i8]* @print.str, i32 0, i32 0), i32 %21 )
	ret i32 0
}
