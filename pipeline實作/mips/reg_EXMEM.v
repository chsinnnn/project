/**
32-Bit Register with Synchronous Reset

input :
    clk : Clock signal
    rst : Reset signal
    en_reg : Control signal to enable register write
    
output :
    
*/

module reg_EXMEM(clk, reset, en_reg, 
				 BranchEX, MemReadEX, MemWriteEX, RegWriteEX, MemtoRegEX, JumpEX, 
				 b_tgt, alu_out, rfile_rd2EX, jump_addrEX, rfile_rd1EX, 
				 rfile_wn, 
				 Zero, 
				 BranchMEM, MemReadMEM, MemWriteMEM, RegWriteMEM, MemtoRegMEM ,JumpMEM, 
				 b_tgtMEM, alu_outMEM, rfile_rd2MEM, jump_addrMEM, rfile_rd1MEM, 
				 rfile_wnMEM, 
				 ZeroMEM);

	input clk, reset, en_reg;
	input BranchEX, MemReadEX, MemWriteEX, RegWriteEX, MemtoRegEX, JumpEX;
	input[31:0] b_tgt, alu_out, rfile_rd2EX, jump_addrEX, rfile_rd1EX;
	input[4:0] rfile_wn;
	input Zero;

	output BranchMEM, MemReadMEM, MemWriteMEM, RegWriteMEM, MemtoRegMEM, JumpMEM;
	output[31:0] b_tgtMEM, alu_outMEM, rfile_rd2MEM, jump_addrMEM, rfile_rd1MEM;
	output[4:0] rfile_wnMEM;
	output ZeroMEM;

	reg BranchMEM, MemReadMEM, MemWriteMEM, RegWriteMEM, MemtoRegMEM, JumpMEM;
	reg[31:0] b_tgtMEM, rfile_rd2MEM, alu_outMEM, jump_addrMEM, rfile_rd1MEM;
	reg[4:0] rfile_wnMEM;
	reg ZeroMEM;

	always@(posedge clk )begin
		if(reset)begin
			b_tgtMEM <= 32'd0;
			ZeroMEM <= 1'd0;
			alu_outMEM <= 32'd0;
			rfile_rd2MEM <= 32'd0;
			rfile_wnMEM <= 5'd0;
			jump_addrMEM <= 32'd0;
			rfile_rd1MEM <= 32'd0;
			BranchMEM <= 1'd0;
			MemReadMEM <= 1'd0;
			MemWriteMEM <= 1'd0;
			RegWriteMEM <= 1'd0;
			MemtoRegMEM <= 1'd0;
			JumpMEM <= 1'd0;
		end
		else if(en_reg) begin
			b_tgtMEM <= b_tgt;
			ZeroMEM <= Zero;
			alu_outMEM <= alu_out;
			rfile_rd2MEM <= rfile_rd2EX;
			rfile_wnMEM <= rfile_wn;
			jump_addrMEM <= jump_addrEX;
			rfile_rd1MEM <= rfile_rd1EX;
			BranchMEM <= BranchEX;
			MemReadMEM <= MemReadEX;
			MemWriteMEM <= MemWriteEX;
			RegWriteMEM <= RegWriteEX;
			MemtoRegMEM <= MemtoRegEX;
			JumpMEM <= JumpEX;
		end
		
	end

endmodule
