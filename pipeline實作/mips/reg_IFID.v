/**
32-Bit Register with Synchronous Reset

input :
    clk : Clock signal
    rst : Reset signal
    en_reg : Control signal to enable register write
    pc_incr : Data to be written to the register (program counter increment)
    instrc : Data to be written to the register (instruction)
output :
    pc_incrID : Data read from the register (program counter increment)
    instrID : Data read from the register (instruction)
*/

module reg_IFID(clk, reset, en_reg, pc_incr, instr, pc_incrID, instrID, nopID );
	input clk, reset, en_reg;
	input[31:0] pc_incr, instr;
	output[31:0] pc_incrID, instrID;
	output nopID ;
	reg[31:0] pc_incrID, instrID;
	reg nopID ;

	always@(posedge clk)begin
		if(reset)begin
			pc_incrID <= 32'd0;
			instrID <= 32'd0;
			nopID <= 1'b0 ;
		end
		else if ( instr == 32'b0 ) begin
		  if ( instrID[5:0] == 6'b011001 || instrID[5:0] == 6'b011100 ) begin
		    pc_incrID <= pc_incrID;
			  instrID <= instrID;
			  nopID <= 1'b1 ;
		  end
		  else begin
			  pc_incrID <= pc_incr;
			  instrID <= instr;
			  nopID <= 1'b1 ;
			end 
		end
		else if ( en_reg ) begin
			pc_incrID <= pc_incr;
			instrID <= instr;
			nopID <= 1'b0 ;
		end
		
	end

endmodule

