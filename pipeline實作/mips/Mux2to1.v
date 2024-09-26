`timescale 1ns/1ns
/**
Mux2to1 module implements a 2-to-1 multiplexer.

inputs :
    dataA : 1-bit input A
    shift : 1-bit input B
    signal : Control signal (1 bit)
outputs :
    dataOut : Output selected based on the control signal
*/
module Mux2to1( dataA, shift, signal, dataOut );
input dataA ;
input shift ;
input signal ;
output dataOut ;

wire temp ;

assign temp = signal ? shift : dataA ;

assign dataOut = temp ;
endmodule