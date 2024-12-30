`timescale 1ns/100ps

module tb_register32_r_en;
	reg tb_clk, tb_reset_n, tb_en;
	reg [31:0] tb_d;
	wire [31:0] tb_q;
	
	register32_r_en U0(.clk(tb_clk), .reset_n(tb_reset_n), .en(tb_en), .d(tb_d), .q(tb_q));
	
	always begin
		#5; tb_clk = ~tb_clk;
	end
	
	initial begin
		tb_clk = 0; tb_reset_n = 0; tb_en = 1; #3;
		tb_reset_n = 1;
		tb_d = 32'haabbccdd; #10;
		tb_d = 32'h11223344; #10;
		tb_d = 32'h99887766; #10;
		
		tb_en = 0; #10;
		tb_d = 32'habcdef12; #10;
		tb_d = 32'h11883366; #10;
		
		$stop;
	end
	
endmodule