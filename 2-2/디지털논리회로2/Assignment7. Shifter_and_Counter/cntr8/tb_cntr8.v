`timescale 1ns/100ps

module tb_cntr8;
	reg tb_clk, tb_reset_n, tb_inc, tb_load;
	reg [7:0] tb_d_in;
	wire [7:0] tb_d_out;
	wire [2:0] tb_o_state;

	cntr8 U0_cntr8(.clk(tb_clk), .reset_n(tb_reset_n), .inc(tb_inc), .load(tb_load), .d_in(tb_d_in), .d_out(tb_d_out), .o_state(tb_o_state));
	
	always begin
		#5; tb_clk = ~tb_clk; // clock period is 10ns
	end
	
	initial begin
		tb_clk = 0; tb_reset_n = 0; #3;
		tb_reset_n = 1; tb_load = 0;
		
		// d_out is 0 after reset
		tb_d_in = 8'h00;
		tb_inc = 1; #10; // d_out = 8'h01
		tb_inc = 1; #10; // d_out = 8'h02
		tb_inc = 1; #10; // d_out = 8'h03
		tb_inc = 1; #10; // d_out = 8'h04
		tb_inc = 1; #10; // d_out = 8'h05
		tb_inc = 0; #10; // d_out = 8'h04
		tb_inc = 0; #10; // d_out = 8'h03
		tb_inc = 0; #10; // d_out = 8'h02
		tb_inc = 0; #10; // d_out = 8'h01
		tb_inc = 0; #10; // d_out = 8'h00
		
		tb_d_in = 8'h44;
		tb_load = 1; #10; // d_out = 8'h44
		tb_load = 0;
		tb_inc = 1; #10; // d_out = 8'h45
		tb_inc = 1; #10; // d_out = 8'h46
		tb_inc = 1; #10; // d_out = 8'h47
		tb_inc = 1; #10; // d_out = 8'h48
		
		tb_reset_n = 0; #10; // d_out = 8'h00
		tb_reset_n = 1;
		tb_inc = 0; #10; // d_out = 8'hff
		tb_inc = 0; #10; // d_out = 8'hfe
		tb_inc = 0; #10; // d_out = 8'hfd
		tb_inc = 0; #10; // d_out = 8'hfc
		
		$stop;
	end
	
endmodule