`timescale 1ns/1ns 
module Shifter (input [31:0] dataA, input [4:0] shamt, input [5:0] Signal, output [31:0] dataOut, input reset); 

parameter SRL = 6'b000010;
wire   [31:0]  temp4, temp3, temp2, temp1, temp0 ;

  Mux2to1 Mux0_0( .dataA(dataA[0]), .shift(dataA[1]), .signal(shamt[0]), .dataOut(temp0[0]) ) ;
  Mux2to1 Mux0_1( .dataA(dataA[1]), .shift(dataA[2]), .signal(shamt[0]), .dataOut(temp0[1]) ) ;
  Mux2to1 Mux0_2( .dataA(dataA[2]), .shift(dataA[3]), .signal(shamt[0]), .dataOut(temp0[2]) ) ;
  Mux2to1 Mux0_3( .dataA(dataA[3]), .shift(dataA[4]), .signal(shamt[0]), .dataOut(temp0[3]) ) ;
  Mux2to1 Mux0_4( .dataA(dataA[4]), .shift(dataA[5]), .signal(shamt[0]), .dataOut(temp0[4]) ) ;
  Mux2to1 Mux0_5( .dataA(dataA[5]), .shift(dataA[6]), .signal(shamt[0]), .dataOut(temp0[5]) ) ;
  Mux2to1 Mux0_6( .dataA(dataA[6]), .shift(dataA[7]), .signal(shamt[0]), .dataOut(temp0[6]) ) ;
  Mux2to1 Mux0_7( .dataA(dataA[7]), .shift(dataA[8]), .signal(shamt[0]), .dataOut(temp0[7]) ) ;
  Mux2to1 Mux0_8( .dataA(dataA[8]), .shift(dataA[9]), .signal(shamt[0]), .dataOut(temp0[8]) ) ;
  Mux2to1 Mux0_9( .dataA(dataA[9]), .shift(dataA[10]), .signal(shamt[0]), .dataOut(temp0[9]) ) ;
  Mux2to1 Mux0_10( .dataA(dataA[10]), .shift(dataA[11]), .signal(shamt[0]), .dataOut(temp0[10]) ) ;
  Mux2to1 Mux0_11( .dataA(dataA[11]), .shift(dataA[12]), .signal(shamt[0]), .dataOut(temp0[11]) ) ;
  Mux2to1 Mux0_12( .dataA(dataA[12]), .shift(dataA[13]), .signal(shamt[0]), .dataOut(temp0[12]) ) ;
  Mux2to1 Mux0_13( .dataA(dataA[13]), .shift(dataA[14]), .signal(shamt[0]), .dataOut(temp0[13]) ) ;
  Mux2to1 Mux0_14( .dataA(dataA[14]), .shift(dataA[15]), .signal(shamt[0]), .dataOut(temp0[14]) ) ;
  Mux2to1 Mux0_15( .dataA(dataA[15]), .shift(dataA[16]), .signal(shamt[0]), .dataOut(temp0[15]) ) ;
  Mux2to1 Mux0_16( .dataA(dataA[16]), .shift(dataA[17]), .signal(shamt[0]), .dataOut(temp0[16]) ) ;
  Mux2to1 Mux0_17( .dataA(dataA[17]), .shift(dataA[18]), .signal(shamt[0]), .dataOut(temp0[17]) ) ;
  Mux2to1 Mux0_18( .dataA(dataA[18]), .shift(dataA[19]), .signal(shamt[0]), .dataOut(temp0[18]) ) ;
  Mux2to1 Mux0_19( .dataA(dataA[19]), .shift(dataA[20]), .signal(shamt[0]), .dataOut(temp0[19]) ) ;
  Mux2to1 Mux0_20( .dataA(dataA[20]), .shift(dataA[21]), .signal(shamt[0]), .dataOut(temp0[20]) ) ;
  Mux2to1 Mux0_21( .dataA(dataA[21]), .shift(dataA[22]), .signal(shamt[0]), .dataOut(temp0[21]) ) ;
  Mux2to1 Mux0_22( .dataA(dataA[22]), .shift(dataA[23]), .signal(shamt[0]), .dataOut(temp0[22]) ) ;
  Mux2to1 Mux0_23( .dataA(dataA[23]), .shift(dataA[24]), .signal(shamt[0]), .dataOut(temp0[23]) ) ;
  Mux2to1 Mux0_24( .dataA(dataA[24]), .shift(dataA[25]), .signal(shamt[0]), .dataOut(temp0[24]) ) ;
  Mux2to1 Mux0_25( .dataA(dataA[25]), .shift(dataA[26]), .signal(shamt[0]), .dataOut(temp0[25]) ) ;
  Mux2to1 Mux0_26( .dataA(dataA[26]), .shift(dataA[27]), .signal(shamt[0]), .dataOut(temp0[26]) ) ;
  Mux2to1 Mux0_27( .dataA(dataA[27]), .shift(dataA[28]), .signal(shamt[0]), .dataOut(temp0[27]) ) ;
  Mux2to1 Mux0_28( .dataA(dataA[28]), .shift(dataA[29]), .signal(shamt[0]), .dataOut(temp0[28]) ) ;
  Mux2to1 Mux0_29( .dataA(dataA[29]), .shift(dataA[30]), .signal(shamt[0]), .dataOut(temp0[29]) ) ;
  Mux2to1 Mux0_30( .dataA(dataA[30]), .shift(dataA[31]), .signal(shamt[0]), .dataOut(temp0[30]) ) ;
  Mux2to1 Mux0_31( .dataA(dataA[31]), .shift(1'b0), .signal(shamt[0]), .dataOut(temp0[31]) ) ;

  Mux2to1 Mux1_0( .dataA(temp0[0]), .shift(temp0[2]), .signal(shamt[1]), .dataOut(temp1[0]) ) ;
  Mux2to1 Mux1_1( .dataA(temp0[1]), .shift(temp0[3]), .signal(shamt[1]), .dataOut(temp1[1]) ) ;
  Mux2to1 Mux1_2( .dataA(temp0[2]), .shift(temp0[4]), .signal(shamt[1]), .dataOut(temp1[2]) ) ;
  Mux2to1 Mux1_3( .dataA(temp0[3]), .shift(temp0[5]), .signal(shamt[1]), .dataOut(temp1[3]) ) ;
  Mux2to1 Mux1_4( .dataA(temp0[4]), .shift(temp0[6]), .signal(shamt[1]), .dataOut(temp1[4]) ) ;
  Mux2to1 Mux1_5( .dataA(temp0[5]), .shift(temp0[7]), .signal(shamt[1]), .dataOut(temp1[5]) ) ;
  Mux2to1 Mux1_6( .dataA(temp0[6]), .shift(temp0[8]), .signal(shamt[1]), .dataOut(temp1[6]) ) ;
  Mux2to1 Mux1_7( .dataA(temp0[7]), .shift(temp0[9]), .signal(shamt[1]), .dataOut(temp1[7]) ) ;
  Mux2to1 Mux1_8( .dataA(temp0[8]), .shift(temp0[10]), .signal(shamt[1]), .dataOut(temp1[8]) ) ;
  Mux2to1 Mux1_9( .dataA(temp0[9]), .shift(temp0[11]), .signal(shamt[1]), .dataOut(temp1[9]) ) ;
  Mux2to1 Mux1_10( .dataA(temp0[10]), .shift(temp0[12]), .signal(shamt[1]), .dataOut(temp1[10]) ) ;
  Mux2to1 Mux1_11( .dataA(temp0[11]), .shift(temp0[13]), .signal(shamt[1]), .dataOut(temp1[11]) ) ;
  Mux2to1 Mux1_12( .dataA(temp0[12]), .shift(temp0[14]), .signal(shamt[1]), .dataOut(temp1[12]) ) ;
  Mux2to1 Mux1_13( .dataA(temp0[13]), .shift(temp0[15]), .signal(shamt[1]), .dataOut(temp1[13]) ) ;
  Mux2to1 Mux1_14( .dataA(temp0[14]), .shift(temp0[16]), .signal(shamt[1]), .dataOut(temp1[14]) ) ;
  Mux2to1 Mux1_15( .dataA(temp0[15]), .shift(temp0[17]), .signal(shamt[1]), .dataOut(temp1[15]) ) ;
  Mux2to1 Mux1_16( .dataA(temp0[16]), .shift(temp0[18]), .signal(shamt[1]), .dataOut(temp1[16]) ) ;
  Mux2to1 Mux1_17( .dataA(temp0[17]), .shift(temp0[19]), .signal(shamt[1]), .dataOut(temp1[17]) ) ;
  Mux2to1 Mux1_18( .dataA(temp0[18]), .shift(temp0[20]), .signal(shamt[1]), .dataOut(temp1[18]) ) ;
  Mux2to1 Mux1_19( .dataA(temp0[19]), .shift(temp0[21]), .signal(shamt[1]), .dataOut(temp1[19]) ) ;
  Mux2to1 Mux1_20( .dataA(temp0[20]), .shift(temp0[22]), .signal(shamt[1]), .dataOut(temp1[20]) ) ;
  Mux2to1 Mux1_21( .dataA(temp0[21]), .shift(temp0[23]), .signal(shamt[1]), .dataOut(temp1[21]) ) ;
  Mux2to1 Mux1_22( .dataA(temp0[22]), .shift(temp0[24]), .signal(shamt[1]), .dataOut(temp1[22]) ) ;
  Mux2to1 Mux1_23( .dataA(temp0[23]), .shift(temp0[25]), .signal(shamt[1]), .dataOut(temp1[23]) ) ;
  Mux2to1 Mux1_24( .dataA(temp0[24]), .shift(temp0[26]), .signal(shamt[1]), .dataOut(temp1[24]) ) ;
  Mux2to1 Mux1_25( .dataA(temp0[25]), .shift(temp0[27]), .signal(shamt[1]), .dataOut(temp1[25]) ) ;
  Mux2to1 Mux1_26( .dataA(temp0[26]), .shift(temp0[28]), .signal(shamt[1]), .dataOut(temp1[26]) ) ;
  Mux2to1 Mux1_27( .dataA(temp0[27]), .shift(temp0[29]), .signal(shamt[1]), .dataOut(temp1[27]) ) ;
  Mux2to1 Mux1_28( .dataA(temp0[28]), .shift(temp0[30]), .signal(shamt[1]), .dataOut(temp1[28]) ) ;
  Mux2to1 Mux1_29( .dataA(temp0[29]), .shift(temp0[31]), .signal(shamt[1]), .dataOut(temp1[29]) ) ;
  Mux2to1 Mux1_30( .dataA(temp0[30]), .shift(1'b0), .signal(shamt[1]), .dataOut(temp1[30]) ) ;
  Mux2to1 Mux1_31( .dataA(temp0[31]), .shift(1'b0), .signal(shamt[1]), .dataOut(temp1[31]) ) ;

  Mux2to1 Mux2_0( .dataA(temp1[0]), .shift(temp1[4]), .signal(shamt[2]), .dataOut(temp2[0]) ) ;
  Mux2to1 Mux2_1( .dataA(temp1[1]), .shift(temp1[5]), .signal(shamt[2]), .dataOut(temp2[1]) ) ;
  Mux2to1 Mux2_2( .dataA(temp1[2]), .shift(temp1[6]), .signal(shamt[2]), .dataOut(temp2[2]) ) ;
  Mux2to1 Mux2_3( .dataA(temp1[3]), .shift(temp1[7]), .signal(shamt[2]), .dataOut(temp2[3]) ) ;
  Mux2to1 Mux2_4( .dataA(temp1[4]), .shift(temp1[8]), .signal(shamt[2]), .dataOut(temp2[4]) ) ;
  Mux2to1 Mux2_5( .dataA(temp1[5]), .shift(temp1[9]), .signal(shamt[2]), .dataOut(temp2[5]) ) ;
  Mux2to1 Mux2_6( .dataA(temp1[6]), .shift(temp1[10]), .signal(shamt[2]), .dataOut(temp2[6]) ) ;
  Mux2to1 Mux2_7( .dataA(temp1[7]), .shift(temp1[11]), .signal(shamt[2]), .dataOut(temp2[7]) ) ;
  Mux2to1 Mux2_8( .dataA(temp1[8]), .shift(temp1[12]), .signal(shamt[2]), .dataOut(temp2[8]) ) ;
  Mux2to1 Mux2_9( .dataA(temp1[9]), .shift(temp1[13]), .signal(shamt[2]), .dataOut(temp2[9]) ) ;
  Mux2to1 Mux2_10( .dataA(temp1[10]), .shift(temp1[14]), .signal(shamt[2]), .dataOut(temp2[10]) ) ;
  Mux2to1 Mux2_11( .dataA(temp1[11]), .shift(temp1[15]), .signal(shamt[2]), .dataOut(temp2[11]) ) ;
  Mux2to1 Mux2_12( .dataA(temp1[12]), .shift(temp1[16]), .signal(shamt[2]), .dataOut(temp2[12]) ) ;
  Mux2to1 Mux2_13( .dataA(temp1[13]), .shift(temp1[17]), .signal(shamt[2]), .dataOut(temp2[13]) ) ;
  Mux2to1 Mux2_14( .dataA(temp1[14]), .shift(temp1[18]), .signal(shamt[2]), .dataOut(temp2[14]) ) ;
  Mux2to1 Mux2_15( .dataA(temp1[15]), .shift(temp1[19]), .signal(shamt[2]), .dataOut(temp2[15]) ) ;
  Mux2to1 Mux2_16( .dataA(temp1[16]), .shift(temp1[20]), .signal(shamt[2]), .dataOut(temp2[16]) ) ;
  Mux2to1 Mux2_17( .dataA(temp1[17]), .shift(temp1[21]), .signal(shamt[2]), .dataOut(temp2[17]) ) ;
  Mux2to1 Mux2_18( .dataA(temp1[18]), .shift(temp1[22]), .signal(shamt[2]), .dataOut(temp2[18]) ) ;
  Mux2to1 Mux2_19( .dataA(temp1[19]), .shift(temp1[23]), .signal(shamt[2]), .dataOut(temp2[19]) ) ;
  Mux2to1 Mux2_20( .dataA(temp1[20]), .shift(temp1[24]), .signal(shamt[2]), .dataOut(temp2[20]) ) ;
  Mux2to1 Mux2_21( .dataA(temp1[21]), .shift(temp1[25]), .signal(shamt[2]), .dataOut(temp2[21]) ) ;
  Mux2to1 Mux2_22( .dataA(temp1[22]), .shift(temp1[26]), .signal(shamt[2]), .dataOut(temp2[22]) ) ;
  Mux2to1 Mux2_23( .dataA(temp1[23]), .shift(temp1[27]), .signal(shamt[2]), .dataOut(temp2[23]) ) ;
  Mux2to1 Mux2_24( .dataA(temp1[24]), .shift(temp1[28]), .signal(shamt[2]), .dataOut(temp2[24]) ) ;
  Mux2to1 Mux2_25( .dataA(temp1[25]), .shift(temp1[29]), .signal(shamt[2]), .dataOut(temp2[25]) ) ;
  Mux2to1 Mux2_26( .dataA(temp1[26]), .shift(temp1[30]), .signal(shamt[2]), .dataOut(temp2[26]) ) ;
  Mux2to1 Mux2_27( .dataA(temp1[27]), .shift(temp1[31]), .signal(shamt[2]), .dataOut(temp2[27]) ) ;
  Mux2to1 Mux2_28( .dataA(temp1[28]), .shift(1'b0), .signal(shamt[2]), .dataOut(temp2[28]) ) ;
  Mux2to1 Mux2_29( .dataA(temp1[29]), .shift(1'b0), .signal(shamt[2]), .dataOut(temp2[29]) ) ;
  Mux2to1 Mux2_30( .dataA(temp1[30]), .shift(1'b0), .signal(shamt[2]), .dataOut(temp2[30]) ) ;
  Mux2to1 Mux2_31( .dataA(temp1[31]), .shift(1'b0), .signal(shamt[2]), .dataOut(temp2[31]) ) ;

  Mux2to1 Mux3_0( .dataA(temp2[0]), .shift(temp2[8]), .signal(shamt[3]), .dataOut(temp3[0]) ) ;
  Mux2to1 Mux3_1( .dataA(temp2[1]), .shift(temp2[9]), .signal(shamt[3]), .dataOut(temp3[1]) ) ;
  Mux2to1 Mux3_2( .dataA(temp2[2]), .shift(temp2[10]), .signal(shamt[3]), .dataOut(temp3[2]) ) ;
  Mux2to1 Mux3_3( .dataA(temp2[3]), .shift(temp2[11]), .signal(shamt[3]), .dataOut(temp3[3]) ) ;
  Mux2to1 Mux3_4( .dataA(temp2[4]), .shift(temp2[12]), .signal(shamt[3]), .dataOut(temp3[4]) ) ;
  Mux2to1 Mux3_5( .dataA(temp2[5]), .shift(temp2[13]), .signal(shamt[3]), .dataOut(temp3[5]) ) ;
  Mux2to1 Mux3_6( .dataA(temp2[6]), .shift(temp2[14]), .signal(shamt[3]), .dataOut(temp3[6]) ) ;
  Mux2to1 Mux3_7( .dataA(temp2[7]), .shift(temp2[15]), .signal(shamt[3]), .dataOut(temp3[7]) ) ;
  Mux2to1 Mux3_8( .dataA(temp2[8]), .shift(temp2[16]), .signal(shamt[3]), .dataOut(temp3[8]) ) ;
  Mux2to1 Mux3_9( .dataA(temp2[9]), .shift(temp2[17]), .signal(shamt[3]), .dataOut(temp3[9]) ) ;
  Mux2to1 Mux3_10( .dataA(temp2[10]), .shift(temp2[18]), .signal(shamt[3]), .dataOut(temp3[10]) ) ;
  Mux2to1 Mux3_11( .dataA(temp2[11]), .shift(temp2[19]), .signal(shamt[3]), .dataOut(temp3[11]) ) ;
  Mux2to1 Mux3_12( .dataA(temp2[12]), .shift(temp2[20]), .signal(shamt[3]), .dataOut(temp3[12]) ) ;
  Mux2to1 Mux3_13( .dataA(temp2[13]), .shift(temp2[21]), .signal(shamt[3]), .dataOut(temp3[13]) ) ;
  Mux2to1 Mux3_14( .dataA(temp2[14]), .shift(temp2[22]), .signal(shamt[3]), .dataOut(temp3[14]) ) ;
  Mux2to1 Mux3_15( .dataA(temp2[15]), .shift(temp2[23]), .signal(shamt[3]), .dataOut(temp3[15]) ) ;
  Mux2to1 Mux3_16( .dataA(temp2[16]), .shift(temp2[24]), .signal(shamt[3]), .dataOut(temp3[16]) ) ;
  Mux2to1 Mux3_17( .dataA(temp2[17]), .shift(temp2[25]), .signal(shamt[3]), .dataOut(temp3[17]) ) ;
  Mux2to1 Mux3_18( .dataA(temp2[18]), .shift(temp2[26]), .signal(shamt[3]), .dataOut(temp3[18]) ) ;
  Mux2to1 Mux3_19( .dataA(temp2[19]), .shift(temp2[27]), .signal(shamt[3]), .dataOut(temp3[19]) ) ;
  Mux2to1 Mux3_20( .dataA(temp2[20]), .shift(temp2[28]), .signal(shamt[3]), .dataOut(temp3[20]) ) ;
  Mux2to1 Mux3_21( .dataA(temp2[21]), .shift(temp2[29]), .signal(shamt[3]), .dataOut(temp3[21]) ) ;
  Mux2to1 Mux3_22( .dataA(temp2[22]), .shift(temp2[30]), .signal(shamt[3]), .dataOut(temp3[22]) ) ;
  Mux2to1 Mux3_23( .dataA(temp2[23]), .shift(temp2[31]), .signal(shamt[3]), .dataOut(temp3[23]) ) ;
  Mux2to1 Mux3_24( .dataA(temp2[24]), .shift(1'b0), .signal(shamt[3]), .dataOut(temp3[24]) ) ;
  Mux2to1 Mux3_25( .dataA(temp2[25]), .shift(1'b0), .signal(shamt[3]), .dataOut(temp3[25]) ) ;
  Mux2to1 Mux3_26( .dataA(temp2[26]), .shift(1'b0), .signal(shamt[3]), .dataOut(temp3[26]) ) ;
  Mux2to1 Mux3_27( .dataA(temp2[27]), .shift(1'b0), .signal(shamt[3]), .dataOut(temp3[27]) ) ;
  Mux2to1 Mux3_28( .dataA(temp2[28]), .shift(1'b0), .signal(shamt[3]), .dataOut(temp3[28]) ) ;
  Mux2to1 Mux3_29( .dataA(temp2[29]), .shift(1'b0), .signal(shamt[3]), .dataOut(temp3[29]) ) ;
  Mux2to1 Mux3_30( .dataA(temp2[30]), .shift(1'b0), .signal(shamt[3]), .dataOut(temp3[30]) ) ;
  Mux2to1 Mux3_31( .dataA(temp2[31]), .shift(1'b0), .signal(shamt[3]), .dataOut(temp3[31]) ) ;

  Mux2to1 Mux4_0( .dataA(temp3[0]), .shift(temp3[16]), .signal(shamt[4]), .dataOut(temp4[0]) ) ;
  Mux2to1 Mux4_1( .dataA(temp3[1]), .shift(temp3[17]), .signal(shamt[4]), .dataOut(temp4[1]) ) ;
  Mux2to1 Mux4_2( .dataA(temp3[2]), .shift(temp3[18]), .signal(shamt[4]), .dataOut(temp4[2]) ) ;
  Mux2to1 Mux4_3( .dataA(temp3[3]), .shift(temp3[19]), .signal(shamt[4]), .dataOut(temp4[3]) ) ;
  Mux2to1 Mux4_4( .dataA(temp3[4]), .shift(temp3[20]), .signal(shamt[4]), .dataOut(temp4[4]) ) ;
  Mux2to1 Mux4_5( .dataA(temp3[5]), .shift(temp3[21]), .signal(shamt[4]), .dataOut(temp4[5]) ) ;
  Mux2to1 Mux4_6( .dataA(temp3[6]), .shift(temp3[22]), .signal(shamt[4]), .dataOut(temp4[6]) ) ;
  Mux2to1 Mux4_7( .dataA(temp3[7]), .shift(temp3[23]), .signal(shamt[4]), .dataOut(temp4[7]) ) ;
  Mux2to1 Mux4_8( .dataA(temp3[8]), .shift(temp3[24]), .signal(shamt[4]), .dataOut(temp4[8]) ) ;
  Mux2to1 Mux4_9( .dataA(temp3[9]), .shift(temp3[25]), .signal(shamt[4]), .dataOut(temp4[9]) ) ;
  Mux2to1 Mux4_10( .dataA(temp3[10]), .shift(temp3[26]), .signal(shamt[4]), .dataOut(temp4[10]) ) ;
  Mux2to1 Mux4_11( .dataA(temp3[11]), .shift(temp3[27]), .signal(shamt[4]), .dataOut(temp4[11]) ) ;
  Mux2to1 Mux4_12( .dataA(temp3[12]), .shift(temp3[28]), .signal(shamt[4]), .dataOut(temp4[12]) ) ;
  Mux2to1 Mux4_13( .dataA(temp3[13]), .shift(temp3[29]), .signal(shamt[4]), .dataOut(temp4[13]) ) ;
  Mux2to1 Mux4_14( .dataA(temp3[14]), .shift(temp3[30]), .signal(shamt[4]), .dataOut(temp4[14]) ) ;
  Mux2to1 Mux4_15( .dataA(temp3[15]), .shift(temp3[31]), .signal(shamt[4]), .dataOut(temp4[15]) ) ;
  Mux2to1 Mux4_16( .dataA(temp3[16]), .shift(1'b0), .signal(shamt[4]), .dataOut(temp4[16]) ) ;
  Mux2to1 Mux4_17( .dataA(temp3[17]), .shift(1'b0), .signal(shamt[4]), .dataOut(temp4[17]) ) ;
  Mux2to1 Mux4_18( .dataA(temp3[18]), .shift(1'b0), .signal(shamt[4]), .dataOut(temp4[18]) ) ;
  Mux2to1 Mux4_19( .dataA(temp3[19]), .shift(1'b0), .signal(shamt[4]), .dataOut(temp4[19]) ) ;
  Mux2to1 Mux4_20( .dataA(temp3[20]), .shift(1'b0), .signal(shamt[4]), .dataOut(temp4[20]) ) ;
  Mux2to1 Mux4_21( .dataA(temp3[21]), .shift(1'b0), .signal(shamt[4]), .dataOut(temp4[21]) ) ;
  Mux2to1 Mux4_22( .dataA(temp3[22]), .shift(1'b0), .signal(shamt[4]), .dataOut(temp4[22]) ) ;
  Mux2to1 Mux4_23( .dataA(temp3[23]), .shift(1'b0), .signal(shamt[4]), .dataOut(temp4[23]) ) ;
  Mux2to1 Mux4_24( .dataA(temp3[24]), .shift(1'b0), .signal(shamt[4]), .dataOut(temp4[24]) ) ;
  Mux2to1 Mux4_25( .dataA(temp3[25]), .shift(1'b0), .signal(shamt[4]), .dataOut(temp4[25]) ) ;
  Mux2to1 Mux4_26( .dataA(temp3[26]), .shift(1'b0), .signal(shamt[4]), .dataOut(temp4[26]) ) ;
  Mux2to1 Mux4_27( .dataA(temp3[27]), .shift(1'b0), .signal(shamt[4]), .dataOut(temp4[27]) ) ;
  Mux2to1 Mux4_28( .dataA(temp3[28]), .shift(1'b0), .signal(shamt[4]), .dataOut(temp4[28]) ) ;
  Mux2to1 Mux4_29( .dataA(temp3[29]), .shift(1'b0), .signal(shamt[4]), .dataOut(temp4[29]) ) ;
  Mux2to1 Mux4_30( .dataA(temp3[30]), .shift(1'b0), .signal(shamt[4]), .dataOut(temp4[30]) ) ;
  Mux2to1 Mux4_31( .dataA(temp3[31]), .shift(1'b0), .signal(shamt[4]), .dataOut(temp4[31]) ) ;

/*
  assign temp0 = shamt[0] ? {1'b0, in[31:1]} : in; // if ctrl[0] == 1, then shift right by 1 bit 
  assign temp1 = shamt[1] ? {2'b0, temp0[31:2]} : temp0; // if ctrl[1] == 1, then shift right by 2 bit 
  assign temp2 = shamt[2] ? {4'b0, temp1[31:4]} : temp1; // if ctrl[2] == 1, then shift right by 4 bit 
  assign temp3 = shamt[3] ? {8'b0, temp2[31:8]} : temp2; // if ctrl[3] == 1, then shift right by 8 bit 
  assign temp4 = shamt[4] ? {16'b0, temp3[31:16]} : temp3; // if ctrl[4] == 1, then shift right by 16 bit 
*/

  assign dataOut = reset ? 32'b0 : temp4;

endmodule