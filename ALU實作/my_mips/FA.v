`timescale 1ns/1ns
// 1bit全加器
module FA(input a, input b, input cin, output sum, output cout) ;
	
	wire   e1, e2, e3 ;
    // If either one or all of a, b, and cin are 1, then the output sum will be 1
	// a,b如果有一個是1 e1就會是1 如果同時為1或同時為0那e1會是0
	xor( e1, a, b ) ;

	// e1,cin如果有一個是1 sum就會是1 如果同時為1或同時為0那sum會是0
    xor( sum, e1, cin ) ; 

	// 判斷要不要進位
    // If two or all of a, b, and cin are 1, then the output cout will be 1
	// a,b如果都是1 e2就會是1 其他情況e2會是0
	and( e2, a, b ) ;

	// e1,cin如果都是1 e3就會是1 其他情況e3會是0
	and( e3, e1, cin ) ;

	// e2,e3如果有一個是1 cout就會是1 
	or( cout, e2, e3 ) ; 
	
endmodule
