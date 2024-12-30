`timescale 1ns/100ps

module tb_tl_cntr;
	reg tb_clk, tb_reset_n;
	reg tb_Ta, tb_Tb;
	wire [1:0] tb_La, tb_Lb;
	
	// instance of tl_cntr
	tl_cntr U0_tl_cntr(.clk(tb_clk), .reset_n(tb_reset_n), .Ta(tb_Ta), .Tb(tb_Tb), .La(tb_La), .Lb(tb_Lb));
	
	always begin
		#5; tb_clk = ~tb_clk; // clock period is 10ns
	end
		
	initial begin
		tb_clk = 0; tb_reset_n = 0; tb_Ta = 1; tb_Tb = 1; #8; // set initial value
		tb_reset_n = 1; tb_Tb = 0; #10;
		tb_Ta = 0; #10;
		tb_Tb = 1; #10;
		tb_Ta = 1; #10;
		tb_Tb = 0; #10;
		tb_Ta = 0; #10;
		tb_Ta = 1; #10;
		$stop;
	end
endmodule