`timescale 1ns/1ns
module ALUControl( clk, Signal, SignaltoALU, SignaltoSHT, SignaltoMUL, SignaltoMUX );
input clk ;
input [5:0] Signal ;
output [5:0] SignaltoALU ;
output [5:0] SignaltoSHT ;
output [5:0] SignaltoMUL ;
output [5:0] SignaltoMUX ;

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


parameter AND = 6'b100100;  //   AND  : 36
parameter OR  = 6'b100101;  //   OR   : 37
parameter ADD = 6'b100000;  //   ADD  : 32
parameter SUB = 6'b100010;  //   SUB  : 34
parameter SLT = 6'b101010;  //   SLT  : 42
parameter SRL = 6'b000010;  //   SRL  : 2
parameter MULTU= 6'b011001;  //   MULTU : 25
parameter MFHI= 6'b010000;  // 16
parameter MFLO= 6'b010010;  // 18


always@( Signal )
begin
  if ( Signal == MULTU )
  begin
    counter = 0 ;
  end

end

always@( posedge clk )
begin
  temp = Signal ;
  if ( Signal == MULTU )
  begin
    counter = counter + 1 ;
    if ( counter == 32 )  // MULTU done
    begin
      temp = 6'b111111 ; // Open HiLo reg for MULTU  
      counter = 0 ;
    end
  end

end


assign SignaltoALU = temp ;
assign SignaltoSHT = temp ;
assign SignaltoMUL = temp ;
assign SignaltoMUX = temp ;


endmodule