`timescale 1ns/1ns
/**
HiLo module separates the high and low parts of a 64-bit input.

inputs :
  clk : Clock signal
  reset : Reset signal
  MULAns : 64-bit input
outputs :
  HiOut : High part of the input (bits 63-32)
  LoOut : Low part of the input (bits 31-0)
*/
module HiLo( clk, Signal, MULAns, HiOut, LoOut, reset );
input clk ;
input reset ;
input [5:0] Signal ;
input [63:0] MULAns ;
output [31:0] HiOut ;
output [31:0] LoOut ;

reg [63:0] HiLo ;

parameter MULTU_out = 6'b111111;
parameter MADDU_out = 6'b111110;


always@( posedge clk or reset )
begin
  if ( Signal == MULTU_out ) begin
    HiLo = ( reset ) ? 64'b0 : MULAns ;
  end

  if ( Signal == MADDU_out ) begin
    HiLo = HiLo + MULAns ;
  end 
  
end

assign HiOut = HiLo[63:32] ;
assign LoOut = HiLo[31:0] ;

endmodule