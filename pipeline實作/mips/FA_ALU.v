`timescale 1ns/1ns
/**
FA_ALU module performs arithmetic and logical operations on two input data based on the control signal.

inputs :
	a : Input A
	b : Input B
	cin : Carry-in
	control : Control signal
	less : Less-than comparison signal
outputs :
	cout : Carry-out
	sum : Result of addition or subtraction
	andNum : Result of bitwise AND operation
	orNum : Result of bitwise OR operation
	sltNum : Result of set less than operation
*/
module FA_ALU( input a, input b, input cin, input control, output cout, output sum, output andNum, output orNum, input less, output sltNum ) ;

	assign sltNum = less ?  less : 0 ; // set sltNum ( if less == 1, then sltNum = 1, else sltNum = 0 )
	and ( andNum, a, b ) ; // set the result of a & b
	or ( orNum, a, b ) ; // set the result of a | b
	FA U_FA( .a(a), .b(b^control), .cin(cin), .cout(cout), .sum(sum) ) ; // do addition or subtraction with control
	
endmodule