module ram(clk, cen, wen, s_addr, s_din,
				s_dout);
	input clk;
	input cen, wen;
	input [7:0] s_addr;
	input [63:0] s_din;
	output reg [63:0] s_dout;

	reg [63:0] mem [0:255];

	integer i;

	initial begin
		//memory initialization
		//Fill the initial syntax
		for(i = 0; i < 256; i = i + 1) begin
			mem[i] = 64'b0;
		end
		
		s_dout = 64'h0;
	end

	// read/write performance
	always @(posedge clk) begin
		if(cen == 1'b0) s_dout <= 64'h0;
		else if(wen == 1'b0) s_dout <= mem[s_addr];
		else begin mem[s_addr] <= s_din; s_dout <= 64'h0; end
	end
	
endmodule
