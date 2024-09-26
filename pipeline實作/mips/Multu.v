`timescale 1ns/1ns
/**
Multu module performs unsigned multiplication of two 32-bit inputs.

inputs :
	clk : Clock signal
	dataA : 32-bit input A
	dataB : 32-bit input B
	Signal : Control signal
	reset : Reset signal
outputs :
	dataOut : 64-bit output representing the result of multiplication
*/
module Multu( input clk, input [31:0] dataA, input [31:0] dataB, input [5:0] Signal, output reg [63:0] dataOut, input reset);

reg [6:0] start;
reg [63:0] MCND, PROD;
reg [31:0] MPY;
// Control Signal Definitions
parameter MULTU = 6'b011001; // MULTU  : 25
parameter MADDU = 6'b011100; // MADDU  : 28
parameter MULTU_out = 6'b111111;
parameter MADDU_out = 6'b111110;

// When there is a positive edge on the clk signal or the reset signal is asserted, the code block inside the always block will be executed immediately
always@( posedge clk or reset ) begin

	if (reset) begin
		start = 2 ;
		PROD = 64'b0;
		dataOut = 64'b0;
		MCND = 64'b0 ;
		MPY = 32'b0 ;
	end
	else begin
		case (Signal)          
		MULTU:
		begin
			if( start == 2 ) begin
				MCND[31:0] = dataA; // set Multiplicand
                MPY[31:0] = dataB; // set Multiplier
 				
			end
			start = start + 1 ;
			if (MPY[0] == 1'b0) begin
				MCND = MCND << 1; // Shift MAND left 1 bit
				MPY = MPY >> 1; // Shift MPY right 1 bit
			end
			else begin
				PROD = PROD + MCND;
                MCND = MCND << 1; // Shift MAND left 1 bit
				MPY = MPY >> 1; // Shift MPY right 1 bit
			end
			
		end

		MADDU:
		begin
			if( start == 2 ) begin
				MCND[31:0] = dataA; // set Multiplicand
                MPY[31:0] = dataB; // set Multiplier
 				
			end
			start = start + 1 ;
			if (MPY[0] == 1'b0) begin
				MCND = MCND << 1; // Shift MAND left 1 bit
				MPY = MPY >> 1; // Shift MPY right 1 bit
			end
			else begin
				PROD = PROD + MCND;
                MCND = MCND << 1; // Shift MAND left 1 bit
				MPY = MPY >> 1; // Shift MPY right 1 bit
			end
			
		end
			
		MULTU_out:
		begin
            dataOut = PROD ;
			start = 2;
			PROD = 64'b0;
			MCND = 64'b0 ;
			MPY = 32'd0 ;
		end

		MADDU_out:
		begin
            dataOut = PROD ;
			start = 2;
			PROD = 64'b0;
			MCND = 64'b0 ;
			MPY = 32'd0 ;
		end

		endcase
	end
end

endmodule