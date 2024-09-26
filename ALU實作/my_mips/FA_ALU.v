`timescale 1ns/1ns
// 取用FA來組成1-BIT的ALU
module FA_ALU( input a, input b, input cin, input control, output cout, output sum, output andNum, output orNum, input less, output sltNum ) ;
    
	// slt的功能 比大小
	// if (less)  sltNum = less;  (dataA<dataB)
	// else  sltNum = 0;          (dataA>=dataB)
	assign sltNum = less ?  less : 0 ; // set sltNum ( if less == 1, then sltNum = 1, else sltNum = 0 )
	
	// &的功能
	and ( andNum, a, b ) ; // set the result of a & b
	
	// or的功能
	or ( orNum, a, b ) ; // set the result of a | b
	
	// +/-的功能
	// ALU.v有設定control, control=0做加法,control=1做減法
	FA U_FA( .a(a), .b(b^control), .cin(cin), .cout(cout), .sum(sum) ) ; // do addition or subtraction with control
	
endmodule