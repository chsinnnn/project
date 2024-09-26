`timescale 1ns/1ns
module Multu( input clk, input [31:0] dataA, input [31:0] dataB, input [5:0] Signal, output reg [63:0] dataOut, input reset);

reg [6:0] start;
reg [63:0] MCND, PROD;
reg [31:0] MPY;
parameter MULTU = 6'b011001; // MULTU  : 25
parameter OUT = 6'b111111;


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
			
		OUT:
		begin
            dataOut = PROD ;
			start = 0;
			PROD = 64'b0;
			MCND = 64'b0 ;
			MPY = 32'd0 ;
		end
		endcase
	end
end

endmodule