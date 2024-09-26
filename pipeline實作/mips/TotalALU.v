`timescale 1ns/1ns
module TotalALU( clk, dataA, dataB, shamt, ALUOp, Signal, Output, zero, reset, nop );
input reset ;
input clk ;
input [31:0] dataA ;
input [31:0] dataB ;
input [4:0] shamt ;
input [1:0] ALUOp ;
input [5:0] Signal ;
input nop ;
output zero ;
output [31:0] Output ;

//   Signal ( 6-bits)?
//   AND  : 36
//   OR   : 37
//   ADD  : 32
//   SUB  : 34
//   SRL  : 2
//   SLT  : 42
//   MULTU : 25

reg [5:0] temp ;

parameter AND = 6'b100100;
parameter OR  = 6'b100101;
parameter ADD = 6'b100000;
parameter SUB = 6'b100010;
parameter SLT = 6'b101010;

parameter SRL = 6'b000010;

parameter MADDU = 6'b011100; // MADDU  : 28
parameter MULTU= 6'b011001;
parameter MFHI= 6'b010000;
parameter MFLO= 6'b010010;
/*
�w�q�U�ذT��
*/
//============================
wire [2:0]  SignaltoALU ;
wire [5:0]  SignaltoSHT ;
wire [5:0]  SignaltoMUL ;
wire [5:0]  SignaltoMUX ;
wire [31:0] ALUOut, HiOut, LoOut, ShifterOut ;
wire [31:0] dataOut ;
wire [63:0] MULAns ;
/*
�w�q�U�ر��u
*/
//============================
/*
reg [31:0] temp_dataA, temp_dataB ;

always@( posedge clk )
begin
  if ( nop == 1'b0 ) begin
    temp_dataA = dataA ;
    temp_dataB = dataB ;
  end
end
*/

ALUControl ALUControl( .clk(clk), .ALUOp(ALUOp), .Signal(Signal), .nop(nop), .SignaltoALU(SignaltoALU), .SignaltoSHT(SignaltoSHT), .SignaltoMUL(SignaltoMUL), .SignaltoMUX(SignaltoMUX) );
ALU ALU( .dataA(dataA), .dataB(dataB), .Signal(SignaltoALU), .dataOut(ALUOut), .reset(reset), .zero(zero) );
Multu Multu( .clk(clk), .dataA(dataA), .dataB(dataB), .Signal(SignaltoMUL), .dataOut(MULAns), .reset(reset) );
Shifter Shifter( .dataA(dataA), .shamt(shamt), .Signal(SignaltoSHT), .dataOut(ShifterOut), .reset(reset) );
HiLo HiLo( .clk(clk), .Signal(SignaltoMUL), .MULAns(MULAns), .HiOut(HiOut), .LoOut(LoOut), .reset(reset) );
MUX MUX( .ALUOut(ALUOut), .HiOut(HiOut), .LoOut(LoOut), .Shifter(ShifterOut), .Signal(SignaltoMUX), .dataOut(dataOut) );
/*
�إߦU��module
*/
assign Output = dataOut ;


endmodule