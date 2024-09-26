/**
32-Bit Register with Synchronous Reset

input :
    clk : Clock signal
    rst : Reset signal
    en_reg : Control signal to enable register write
    
output :
    
*/
module reg_IDEX(clk, reset, en_reg,
                RegDst, ALUSrc, Branch, MemRead, MemWrite, RegWrite, MemtoReg, Jump, 
                pc_incrID, rfile_rd1, rfile_rd2, extend_immed, jump_addr, 
                ALUOp, 
                rt, rd, shamt, 
                funct,nopID,
                RegDstEX, ALUSrcEX, BranchEX, MemReadEX, MemWriteEX, RegWriteEX, MemtoRegEX, JumpEX, 
                ALUOpEX,
                pc_incrEX, rfile_rd1EX, rfile_rd2EX, extend_immedEX, jump_addrEX, 
                rtEX, rdEX, shamtEX,
                functEX, nopEX);
                
	input clk, reset, en_reg;
	input RegDst, ALUSrc,Branch, MemRead, MemWrite, RegWrite, MemtoReg, Jump;
	input [1:0] ALUOp;
	input[31:0] pc_incrID, rfile_rd1, rfile_rd2, extend_immed, jump_addr;
	input[4:0] rt, rd, shamt;
	input[5:0] funct;
	input nopID ;

	output RegDstEX, ALUSrcEX, BranchEX, MemReadEX, MemWriteEX, RegWriteEX, MemtoRegEX, JumpEX;
	output [1:0] ALUOpEX;
	output[31:0] pc_incrEX, rfile_rd1EX, rfile_rd2EX, extend_immedEX, jump_addrEX;
	output[4:0] rtEX, rdEX, shamtEX;
	output[5:0] functEX;
	output nopEX ;

	reg RegDstEX, ALUSrcEX, BranchEX, MemReadEX, MemWriteEX, RegWriteEX, MemtoRegEX, JumpEX;
	reg [1:0] ALUOpEX;
	reg[31:0] pc_incrEX, rfile_rd1EX, rfile_rd2EX, extend_immedEX, jump_addrEX;
	reg[4:0] rtEX, rdEX, shamtEX;
	reg[5:0] functEX;
	reg nopEX ;

	always@(posedge clk)begin
		if(reset)begin
			pc_incrEX <= 32'd0;
			rfile_rd1EX <= 32'd0;
			rfile_rd2EX <= 32'd0;
			extend_immedEX <= 32'd0;
			rtEX <= 5'd0;
			rdEX <= 5'd0;
			functEX <= 6'd0;
			shamtEX <= 5'd0;
			jump_addrEX <= 32'd0;
			RegDstEX <= 1'd0;
			ALUOpEX <= 2'd0;
			ALUSrcEX <= 1'd0;
			BranchEX <= 1'd0;
			MemReadEX <= 1'd0;
			MemWriteEX <= 1'd0;
			RegWriteEX <= 1'd0;
			MemtoRegEX <= 1'd0;
			JumpEX <= 1'd0;
			nopEX <= 1'b0;
		end
		else if(en_reg) begin
			pc_incrEX <= pc_incrID;
			rfile_rd1EX <= rfile_rd1;
			rfile_rd2EX <= rfile_rd2;
			extend_immedEX <= extend_immed;
			rtEX <= rt;
			rdEX <= rd;
			functEX <= funct;
			shamtEX <= shamt;
			jump_addrEX <= jump_addr;
			RegDstEX <= RegDst;
			ALUOpEX <= ALUOp;
			ALUSrcEX <= ALUSrc;
			BranchEX <= Branch;
			MemReadEX <= MemRead;
			MemWriteEX <= MemWrite;
			RegWriteEX <= RegWrite;
			MemtoRegEX <= MemtoReg;
			JumpEX <= Jump;
			nopEX <= nopID;
		end
		
	end

endmodule


