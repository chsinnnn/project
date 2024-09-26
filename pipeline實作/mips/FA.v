`timescale 1ns/1ns
/**
FA (Full Adder) module performs addition of two inputs with a carry-in.

inputs :
	a : Input A
	b : Input B
	cin : Carry-in
outputs :
	sum : Result of addition
	cout : Carry-out
*/
module FA(input a, input b, input cin, output sum, output cout) ;
	
	wire   e1, e2, e3 ;
    // If either one or all of a, b, and cin are 1, then the output sum will be 1
	xor( e1, a, b ) ;
    xor( sum, e1, cin ) ; 
    // If two or all of a, b, and cin are 1, then the output cout will be 1
	and( e2, a, b ) ;
	and( e3, e1, cin ) ;
	or( cout, e2, e3 ) ;  
	
endmodule
