/**
Register File (32 32-bit registers)

input :
    clk : Clock signal
    RegWrite : Control signal to enable register write
    RN1 : Register number for rs (read operation)
    RN2 : Register number for rt (read operation)
    WN : Register number for write operation
    WD : Data to be written to the register
output :
    RD1 : Data read from the rs register
    RD2 : Data read from the rt register
*/

module reg_file( clk, RegWrite, RN1, RN2, WN, WD, RD1, RD2 );
	input clk;
	input RegWrite;
	input[4:0] RN1, RN2, WN;
	input[31:0] WD;
	output[31:0] RD1, RD2;

	reg[31:0] RD1, RD2;
	reg[31:0] file_array[31:1];

	always @( RN1 or file_array[RN1] ) begin   
		if ( RN1 == 0 )
			RD1 = 32'd0;
		else
			RD1 = file_array[RN1];
		$display( "%d, reg_file[%d] => %d (Port 1)", $time/10, RN1, RD1);
	end

	always @( RN2 or file_array[RN2] ) begin
		if ( RN2 == 0 )
			RD2 = 32'd0;
		else
			RD2 = file_array[RN2];
		$display( "%d, reg_file[%d] => %d (Port 2)", $time/10, RN2, RD2);
	end

	always @( posedge clk ) begin
		if ( RegWrite && (WN != 0) ) begin
			file_array[WN] <= WD;
			$display( "%d, reg_file[%d] <= %d (Write)", $time/10, WN, WD);
		end
	end
endmodule

