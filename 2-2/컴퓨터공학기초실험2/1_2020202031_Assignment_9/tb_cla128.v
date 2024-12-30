`timescale 1ns/100ps

module tb_cla128;
	reg [127:0] tb_a, tb_b;
	reg tb_ci;
	wire [127:0] tb_s;
	wire tb_co;
	
	cla128 U0(.a(tb_a), .b(tb_b), .ci(tb_ci), .s(tb_s), .co(tb_co));

	initial begin
		tb_a = 128'h0000000000ffffffffffffffffffffff;
		tb_b = 128'h00000000000000000000000000000001;
		tb_ci = 1'b0; #5;
		tb_ci = 1'b1; #5;
		
		tb_a = 128'hffff0000000000000000000000000000;
		tb_b = 128'h00010000000000000000000000000000;
		tb_ci = 1'b0; #5;
		tb_ci = 1'b1; #5;
		
		$stop;
	end
	
endmodule