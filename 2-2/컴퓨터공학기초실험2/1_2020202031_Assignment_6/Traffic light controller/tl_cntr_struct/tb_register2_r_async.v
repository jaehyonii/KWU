`timescale 1ns/100ps

module tb_register2_r_async;
	reg tb_clk, tb_reset_n;
	reg [1:0] tb_D;
	wire [1:0] tb_Q;
	
	// instance of _register2_r_async
	_register2_r_async U0_register2_r_async(.clk(tb_clk), .reset_n(tb_reset_n), .D(tb_D), .Q(tb_Q));
	
	always begin
		#5; tb_clk = ~tb_clk;
	end
	
	initial begin
		tb_clk = 0; tb_reset_n = 0; tb_D = 2'b00; #2;
		// when tb_clk rising, tb_Q follows tb_D
		tb_reset_n = 1; #10;
		tb_D = 2'b01; #10;
		tb_D = 2'b10; #10;
		tb_D = 2'b11; #10;
		$stop;
	end
	
endmodule