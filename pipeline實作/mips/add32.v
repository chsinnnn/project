module add32(a, b, result, sel);
  input [31:0] a, b;
  input sel ;
  output [31:0] result;

	assign result = (sel) ? (a+b) : (a);
  
endmodule
