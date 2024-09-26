/**
This module implements unsigned extension functionality for a given input data.
It takes a 16-bit immediate value (immed_in) and extends it to a 32-bit value (ext_immed_out) while treating the input as an unsigned value.

input :
	immed_in : Data to be read for unsign extension
output :
	ext_immed_out : Output of the unsign-extended data
*/
module unsign_extend( immed_in, ext_immed_out );
	input[15:0] immed_in;
	output[31:0] ext_immed_out;
	assign ext_immed_out = { 16'd0, immed_in };
endmodule