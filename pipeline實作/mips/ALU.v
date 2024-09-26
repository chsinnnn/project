`timescale 1ns/1ns
/**
ALU (Arithmetic Logic Unit) module performs various arithmetic and logical operations on two input data, based on a control signal.

inputs :
	dataA : 32-bit input data A
	dataB : 32-bit input data B
	Signal : 6-bit control signal
	reset : Reset signal
outputs :
	dataOut: 32-bit output data
*/
module ALU( input [31:0] dataA, input [31:0] dataB, input [2:0] Signal, output [31:0] dataOut, input reset, output zero );

wire control;
wire [31:0] sum, andNum, orNum, sltNum ;

//   Signal ( 6-bits)
parameter AND = 6'b100100; //   AND  : 36
parameter OR  = 6'b100101; //   OR   : 37
parameter ADD = 6'b100000; //   ADD  : 32
parameter SUB = 6'b100010; //   SUB  : 34
parameter SLT = 6'b101010; //   SLT  : 42


// if the signal == SUB or signal == SLT, then the control will be 1, else the control will be 0
assign control = ( Signal == AND ) ? 0 :
	     	     ( Signal == OR  ) ? 0 :
	     	     ( Signal == ADD ) ? 0 :
	     	     ( Signal == SUB ) ? 1 :
	     	     ( Signal == SLT ) ? 1 : 0 ;

FA_ALU32 U_FA_ALU32( .a(dataA), .b(dataB), .cin(control), .sum(sum), .andNum(andNum), .orNum(orNum), .sltNum(sltNum) ) ;

// set dataOut
assign dataOut = ( Signal == AND ) ? andNum :
	     	     ( Signal == OR  ) ? orNum  :
	     	     ( Signal == ADD ) ? sum :
	     	     ( Signal == SUB ) ? sum :
	     	     ( Signal == SLT ) ? sltNum : 32'b0 ;

assign zero = ( dataOut == 32'b0 ) ? ( Signal == SUB ) ? 1 : 0 : 0 ;

endmodule
