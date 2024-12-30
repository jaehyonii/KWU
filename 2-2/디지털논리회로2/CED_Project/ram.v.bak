module ram(clk, cen, wen, addr, din, dout);
	input clk;
	input cen, wen;
	input [4:0] addr;
	input [31:0] din;
	output reg [31:0] dout;

	reg [31:0] mem [0:31];

	integer i;

	initial begin
		//memory initialization
		//Fill the initial syntax
		for(i = 0; i < 32; i = i + 1) begin
			mem[i] = 32'b0;
		end
	end

	always @(posedge clk) begin
		// read/write performance
		if(cen == 1'b0) dout <= 32'b0;
		else if(wen == 1'b0) dout <= mem[addr];
		else begin mem[addr] <= din; dout <= 32'b0; end
	end
	
endmodule
