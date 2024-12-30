

module tb_8_to_1_MUX;
	reg [31:0] tb_a, tb_b, tb_c, tb_d, tb_e, tb_f, tb_g, tb_h;
	reg [2:0] tb_sel;
	wire [31:0] tb_d_out;
	
	_8_to_1_MUX U0(.a(tb_a), .b(tb_b), .c(tb_c), .d(tb_d), .e(tb_e), .f(tb_f), .g(tb_g), .h(tb_h), .sel(tb_sel), .d_out(tb_d_out));
	
	initial begin
		tb_a = 32'h12345678;
		tb_b = 32'h13579bdf;
		tb_c = 32'h147ad147;
		tb_d = 32'h2468ace1;
		tb_e = 32'h258be258;
		tb_f = 32'h3579bdf1;
		tb_g = 32'haabbccdd;
		tb_h = 32'hffeeddcc;
		
		tb_sel = 3'b000; #10;
		tb_sel = 3'b001; #10;
		tb_sel = 3'b010; #10;
		tb_sel = 3'b011; #10;
		tb_sel = 3'b100; #10;
		tb_sel = 3'b101; #10;
		tb_sel = 3'b110; #10;
		tb_sel = 3'b111; #10;
		
		$stop;
	end
endmodule