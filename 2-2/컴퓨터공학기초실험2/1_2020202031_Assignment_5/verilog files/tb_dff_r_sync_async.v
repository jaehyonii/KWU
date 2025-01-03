`timescale 1ns/100ps

module tb_dff_r_sync_async;
	reg tb_clk, tb_reset_n, tb_d;
	wire tb_q_sync, tb_q_async;
	
	_dff_r_sync_async U0_dff_r_sync_async(.clk(tb_clk), .reset_n(tb_reset_n), .d(tb_d), .q_sync(tb_q_sync), .q_async(tb_q_async));
	
	always begin
		#5; tb_clk = ~tb_clk;
	end
	
	initial begin
		tb_clk = 0; tb_reset_n = 0; tb_d = 0; #2;
		tb_d = 1; #10;
		tb_d = 0; #10;
		tb_d = 1; tb_reset_n = 1; #10;
		tb_d = 0; #10;
		tb_d = 1; #10;
		tb_d = 0; #10;
		tb_d = 1; #10;
		tb_d = 0; tb_reset_n = 0; #10;
		tb_d = 1; #10;
		tb_d = 0; #10;
		$stop;
		
	end
	
endmodule