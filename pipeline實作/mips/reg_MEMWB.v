/**
32-Bit Register with Synchronous Reset

input :
    clk : Clock signal
    rst : Reset signal
    en_reg : Control signal to enable register write

output :
    
*/

module reg_MEMWB(clk, reset, en_reg, 
                 RegWriteMEM, MemtoRegMEM, 
                 dmem_rdata, alu_outMEM, 
                 rfile_wnMEM, 
                 RegWriteWB, MemtoRegWB, 
                 dmem_rdataWB, alu_outWB, 
                 rfile_wnWB);

	input clk, reset, en_reg;
	input RegWriteMEM, MemtoRegMEM;
	input[31:0] dmem_rdata ,alu_outMEM;
	input[4:0] rfile_wnMEM;

	output RegWriteWB, MemtoRegWB;
	output[31:0] dmem_rdataWB, alu_outWB;
	output[4:0] rfile_wnWB;

	reg RegWriteWB, MemtoRegWB;
	reg[31:0] dmem_rdataWB, alu_outWB;
	reg[4:0] rfile_wnWB;

	always@(posedge clk)begin
		if(reset)begin
			dmem_rdataWB <= 32'd0;
			alu_outWB <= 32'd0;
			rfile_wnWB <= 5'd0;
			RegWriteWB <= 1'd0;
			MemtoRegWB <= 1'd0;
		end
		else if(en_reg) begin
			dmem_rdataWB <= dmem_rdata;
			alu_outWB <= alu_outMEM;
			rfile_wnWB <= rfile_wnMEM;
			RegWriteWB <= RegWriteMEM;
			MemtoRegWB <= MemtoRegMEM;
		end
		
	end

endmodule
