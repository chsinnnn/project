`timescale 1ns/1ns

// signal 移多少
module Mux2to1( dataA, shift, signal, dataOut );
input dataA ;
input shift ;
input signal ;
output dataOut ;

wire temp ;


assign temp = signal ? shift : dataA ;

assign dataOut = temp ;
endmodule