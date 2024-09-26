`timescale 1ns/1ns

// 呼叫FA_ALU32來用
module ALU( input [31:0] dataA, input [31:0] dataB, input [5:0] Signal, output [31:0] dataOut, input reset );

wire control;
wire [31:0] sum, andNum, orNum, sltNum ;

//   Signal ( 6-bits)
// opcode
parameter AND = 6'b100100; //   AND  : 36
parameter OR  = 6'b100101; //   OR   : 37
parameter ADD = 6'b100000; //   ADD  : 32
parameter SUB = 6'b100010; //   SUB  : 34
parameter SLT = 6'b101010; //   SLT  : 42

// 判斷control會是多少
// if the signal == SUB or signal == SLT, then the control will be 1, else the control will be 0
assign control = ( Signal == AND ) ? 0 :
	     	     ( Signal == OR  ) ? 0 :
	     	     ( Signal == ADD ) ? 0 :
	     	     ( Signal == SUB ) ? 1 :
	     	     ( Signal == SLT ) ? 1 : 0 ;

// FA_ALU32( input [31:0] a, input [31:0] b, input cin, output [31:0] sum, output [31:0] andNum, output [31:0] orNum, output [31:0] sltNum )
FA_ALU32 U_FA_ALU32( .a(dataA), .b(dataB), .cin(control), .sum(sum), .andNum(andNum), .orNum(orNum), .sltNum(sltNum) ) ;

// set dataOut  做完的結果
assign dataOut = ( Signal == AND ) ? andNum :
	     	     ( Signal == OR  ) ? orNum  :
	     	     ( Signal == ADD ) ? sum :
	     	     ( Signal == SUB ) ? sum :
	     	     ( Signal == SLT ) ? sltNum : 32'd0 ;

endmodule
