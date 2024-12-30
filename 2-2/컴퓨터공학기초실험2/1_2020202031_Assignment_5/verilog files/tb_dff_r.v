`timescale 1ns/100ps

module tb_dff_r;
	reg tb_clk, tb_reset_n, tb_d;
	wire tb_q;
	
	_dff_r U0_dff_r(.clk(tb_clk), .reset_n(tb_reset_n), . d(tb_d), .q(tb_q));
	
	always begin
		#5; tb_clk = ~tb_clk;
	end
	
	initial begin
		tb_clk = 0; tb_reset_n = 0; tb_d = 0;
		#3; tb_d = 1;
		#10; tb_reset_n = 1; tb_d = 0;
		#10; tb_d = 1;	#10; tb_d = 0;
		#10; tb_d = 1;	#10; tb_d = 0;
		#10; tb_d = 1;	#10; tb_d = 0;
		#10; tb_d = 1;	#10; tb_d = 0;
		#10; tb_d = 1;	#10; tb_d = 0;
		#20; tb_reset_n = 0; tb_d = 1;
		#10; tb_d = 0; #10; tb_d = 1;
		#10; $stop;		
	end
	
endmodule