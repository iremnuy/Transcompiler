; ModuleID = 'advcalc2ir'
source_filename = "postfix_module"

store i32 %0, i32* x
store i32 %0, i32* y
%1 = load i32, i32* @x
%2 = load i32, i32* @y
%3 = add i32 %d, %d
%4 = mul i32 %d, %d
%5 = add i32 %d, %d
store i32 %5, i32* zvalue
%6 = load i32, i32* @zvalue
%7 = load i32, i32* @x
%8 = load i32, i32* @y
%9 = sub i32 %d, %d
%10 = load i32, i32* @zvalue
%11 = sub i32 %d, %d
store i32 %11, i32* k
%12 = load i32, i32* @x
%13 = load i32, i32* @y
%14 = mul i32 %d, %d
%15 = add i32 %d, %d
%16 = mul i32 %d, %d
%17 = mul i32 %d, %d
%18 = add i32 %d, %d
store i32 %18, i32* k
%19 = load i32, i32* @k
%20 = add i32 %d, %d
