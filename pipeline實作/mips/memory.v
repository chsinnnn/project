
/**
This module implements a memory unit that performs memory read and write operations based on control signals. 
It has the ability to read data from a specified memory address and write data to a specified memory address.

input :
    MemRead : Control memory read operation
    MemWrite : Control memory write operation
    addr : Memory address to access
    wd : Data to write into memory
output :
    rd : Data read from memory
*/
module memory( clk, MemRead, MemWrite, wd, addr, rd );
	input clk, MemRead, MemWrite;
	input[31:0] addr, wd;
	output[31:0] rd;
	reg[31:0] rd;
	
	
	reg [7:0] mem_array [0:1023];  // Memory size: 1024 Bytes.
	
	// Read data from the specified memory location
	always @( MemRead or mem_array[addr] or mem_array[addr+1] or 
			  mem_array[addr+2] or mem_array[addr+3] ) begin
		if ( MemRead == 1'b1 ) begin
			rd[7:0] = mem_array[addr];
			rd[15:8] = mem_array[addr+1];
			rd[23:16] = mem_array[addr+2];
			rd[31:24] = mem_array[addr+3];
			$display( "%d, reading data: Mem[%d] => %d", $time/10, addr, rd );
		end
		else rd = 32'hxxxxxxxx;
	end
	
	// Write data to the specified memory location
	always @( posedge clk ) begin
		if ( MemWrite == 1'b1 ) begin
			$display( "%d, writing data: Mem[%d] <= %d", $time/10, addr, wd );
			mem_array[addr] <= wd[7:0];
			mem_array[addr+1] <= wd[15:8];
			mem_array[addr+2] <= wd[23:16];
			mem_array[addr+3] <= wd[31:24];
		end
	end
	
endmodule


