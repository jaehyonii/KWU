module register3_r_en(clk, reset_n, en, d, q);
	input clk, reset_n, en;
	input [2:0] d;
	output reg [2:0] q;
	
	always@(posedge clk or negedge reset_n)
	begin
		if(reset_n == 0) q <= 2'b0; // reset_n == 0; output is 0
		else if(en) q <= d; // en == 1; register acts like DFF
		else q <= q; // en == 0; output maintains previous value
	end
	
endmodule