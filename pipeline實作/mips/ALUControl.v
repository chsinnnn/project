`timescale 1ns/1ns
/**
ALUControl module generates control signals for different components in the ALU based on the input Signal.

inputs :
  clk : Clock signal
  Signal : 6-bit input signal
outputs :
  SignaltoALU : 6-bit control signal for ALU
  SignaltoSHT : 6-bit control signal for shift operation
  SignaltoMUL : 6-bit control signal for multiplication operation
  SignaltoMUX : 6-bit control signal for multiplexer
*/
module ALUControl( clk, ALUOp, Signal, SignaltoALU, SignaltoSHT, SignaltoMUL, SignaltoMUX, nop );
input clk ;
input [1:0] ALUOp ;
input [5:0] Signal ;
input nop ;
output [2:0] SignaltoALU ;
output [5:0] SignaltoSHT ;
output [5:0] SignaltoMUL ;
output [5:0] SignaltoMUX ;
reg [2:0] SignaltoALU ; 

//   Signal ( 6-bits)?
//   AND  : 36
//   OR   : 37
//   ADD  : 32
//   SUB  : 34
//   SRL  : 2
//   SLT  : 42
//   MULTU : 25


reg [5:0] temp ;
reg [6:0] counter ;

// Control Signal Definitions
parameter AND = 6'b100100;
parameter OR  = 6'b100101;
parameter ADD = 6'b100000;
parameter SUB = 6'b100010;
parameter SLT = 6'b101010;
parameter SRL = 6'b000010;
parameter NOP = 6'b000000;
parameter MADDU = 6'b011100; // MADDU  : 28
parameter MULTU= 6'b011001;
parameter MFHI= 6'b010000;
parameter MFLO= 6'b010010;
parameter MULTU_out = 6'b111111;
parameter MADDU_out = 6'b111110;

parameter alu_add = 3'b010;
parameter alu_sub = 3'b110;
parameter alu_and = 3'b000;
parameter alu_or  = 3'b001;
parameter alu_slt = 3'b111;


always @(ALUOp or Signal)
begin
  case (ALUOp) 
    2'b00 : SignaltoALU = alu_add;
    2'b01 : SignaltoALU = alu_sub;
    2'b10 : case (Signal) 
              ADD : SignaltoALU = alu_add;
              SUB : SignaltoALU = alu_sub;
              AND : SignaltoALU = alu_and;
              OR  : SignaltoALU = alu_or;
              SLT : SignaltoALU = alu_slt;
              default SignaltoALU = 3'bxxx;
            endcase
    default SignaltoALU = 3'bxxx;
  endcase
end



always@( Signal )
begin
  if ( Signal == MULTU || Signal == MADDU )
  begin
    counter = 0 ;
  end
/*
If Signal changes to MULTU, reset the counter to 0
*/
end



always@( posedge clk )
begin
  if ( nop == 1'b0 ) begin
    temp = Signal ;
  end

  if ( Signal == MULTU )
  begin
    counter = counter + 1 ;
    if ( counter == 32 )
    begin
      temp = MULTU_out ; // Open HiLo reg for MULTU
      counter = 0 ;
    end

  end

  if ( Signal == MADDU )
  begin
    counter = counter + 1 ;
    if ( counter == 32 )
    begin
      temp = MADDU_out ; // Open HiLo reg for MULTU
      counter = 0 ;
    end
  end

end



assign SignaltoSHT = temp ;
assign SignaltoMUL = temp ;
assign SignaltoMUX = temp ;


endmodule