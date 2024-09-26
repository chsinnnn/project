/**
This module implements a 32-bit register with synchronous reset functionality.
It allows writing data into the register when the enable signal (en_reg) is active, and outputs the stored data on the d_out signal.

input :
	rst : Reset signal
	en_reg : Control signal for write enable
	d_in : Data to be written into the register
output :
    d_out : Data read from the register
*/
module reg32 ( clk, rst, en_reg, d_in, d_out );
    input clk, rst, en_reg;
    input[31:0]	d_in;
    output[31:0] d_out;
    reg [31:0] d_out;
   
    always @( posedge clk ) begin
        if ( rst )
			d_out <= 32'b0;
        else if ( en_reg )
			d_out <= d_in;
    end

endmodule