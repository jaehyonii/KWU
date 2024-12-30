`timescale 1ns/100ps

module tb_register32_8;
	reg tb_clk, tb_reset_n;
	reg [7:0] tb_en;
	reg [31:0] tb_d_in;
	wire [31:0] tb_d_out0, tb_d_out1, tb_d_out2, tb_d_out3, tb_d_out4, tb_d_out5, tb_d_out6, tb_d_out7;
	
	register32_8 U0(.clk(tb_clk), .reset_n(tb_reset_n), .en(tb_en), .d_in(tb_d_in),
						.d_out0(tb_d_out0), .d_out1(tb_d_out1), .d_out2(tb_d_out2), .d_out3(tb_d_out3),
						.d_out4(tb_d_out4), .d_out5(tb_d_out5), .d_out6(tb_d_out6), .d_out7(tb_d_out7));

	always begin
		#5; tb_clk = ~tb_clk; // clock period is 10ns
	end
	
	initial begin
		tb_clk = 0; tb_reset_n = 0; #3;
		tb_reset_n = 1;
		tb_en = 8'b00000000;
		tb_d_in = 32'h12345678; #10;
		tb_d_in = 32'hfedcba98; #10;
		
		tb_en = 8'b00000001;
		tb_d_in = 32'h12345678; #10;
		tb_d_in = 32'hfedcba98; #10;
		
		tb_en = 8'b00000010;
		tb_d_in = 32'hababcdcd; #10;
		tb_d_in = 32'hdfdfbaba; #10;
		
		tb_en = 8'b00000100;
		tb_d_in = 32'h12984756; #10;
		tb_d_in = 32'h88776633; #10;
		
		tb_en = 8'b00001000;
		tb_d_in = 32'hfafababa; #10;
		tb_d_in = 32'hffffffff; #10;
		
		tb_en = 8'b00010000;
		tb_d_in = 32'h11111111; #10;
		tb_d_in = 32'hfedddbbb; #10;
		
		tb_en = 8'b00100000;
		tb_d_in = 32'haaaaaaaa; #10;
		tb_d_in = 32'h99999999; #10;
		
		tb_en = 8'b01000000;
		tb_d_in = 32'h12312312; #10;
		tb_d_in = 32'h98798798; #10;
		
		tb_en = 8'b10000000;
		tb_d_in = 32'h826ddddd; #10;
		tb_d_in = 32'h91735940; #10;
		
		$stop;
	end
endmodule