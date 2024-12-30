`timescale 1ns/100ps

module tb_register3_r;
	reg tb_clk, tb_reset_n;
	reg [2:0] tb_D;
	wire [2:0] tb_Q;

	_register3_r U0(.clk(tb_clk), .reset_n(tb_reset_n), .D(tb_D), .Q(tb_Q));
	
	always begin
		#5; tb_clk = ~tb_clk;
	end
	
	initial begin
		tb_clk = 0; tb_reset_n = 0; #2;
		tb_reset_n = 1;
		tb_D = 3'b000; #10;
		tb_D = 3'b001; #10;
		tb_D = 3'b010; #10;
		tb_D = 3'b011; #10;
		tb_D = 3'b100; #10;
		tb_D = 3'b101; #10;
		tb_D = 3'b110; #10;
		tb_D = 3'b111; #10;
		$stop;
	end
endmodule