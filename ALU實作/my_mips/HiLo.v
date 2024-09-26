`timescale 1ns/1ns
module HiLo( clk, MULAns, HiOut, LoOut, reset );
input clk ;
input reset ;
input [63:0] MULAns ;
output [31:0] HiOut ;
output [31:0] LoOut ;

reg [63:0] HiLo ;


always@( posedge clk or reset )
begin
  HiLo = ( reset ) ? 64'b0 : MULAns ;
end

assign HiOut = HiLo[63:32] ;
assign LoOut = HiLo[31:0] ;

endmodule