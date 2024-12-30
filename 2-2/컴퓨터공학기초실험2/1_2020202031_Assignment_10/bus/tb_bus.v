`timescale 1ns/100ps

module tb_bus;
	reg 		  tb_clk, tb_reset_n;
	reg 		  tb_m0_req, tb_m1_req;
	reg 		  tb_m0_wr, tb_m1_wr;
	reg [7:0]  tb_m0_address, tb_m1_address;
	reg [31:0] tb_m0_dout, tb_m1_dout;
	reg [31:0] tb_s0_dout, tb_s1_dout;
	
	wire 		   tb_m0_grant, tb_m1_grant;
	wire [31:0] tb_m_din;
	wire 		   tb_s0_sel, tb_s1_sel;
	wire 		   tb_s_wr;
	wire [7:0]  tb_s_address;
	wire [31:0] tb_s_din;

	bus U0(.clk(tb_clk), .reset_n(tb_reset_n), .m0_req(tb_m0_req), .m1_req(tb_m1_req), .m0_wr(tb_m0_wr), .m1_wr(tb_m1_wr),
			.m0_address(tb_m0_address), .m1_address(tb_m1_address), .m0_dout(tb_m0_dout), .m1_dout(tb_m1_dout),
			.s0_dout(tb_s0_dout), .s1_dout(tb_s1_dout),
			.m0_grant(tb_m0_grant), .m1_grant(tb_m1_grant), .m_din(tb_m_din), .s0_sel(tb_s0_sel), .s1_sel(tb_s1_sel),
			.s_wr(tb_s_wr), .s_address(tb_s_address), .s_din(tb_s_din));
	
	always begin
		#5; tb_clk = ~tb_clk;
	end
	
	initial begin
		tb_clk = 1; tb_reset_n = 0;
		tb_m0_req = 0; tb_m0_wr = 0; tb_m0_address=8'h00; tb_m0_dout=32'h0;
		tb_m1_req = 0; tb_m1_wr = 0; tb_m1_address=8'h00; tb_m1_dout=32'h0;
		tb_s0_dout = 32'h0; tb_s1_dout = 32'h0; #17;
		
		tb_reset_n = 1; #10;
		
		tb_m0_req = 1;
		tb_s0_dout = 32'h1; tb_s1_dout = 32'h2; #10;
		
		tb_m0_wr = 1; #10;
		
		tb_m0_address = 8'h1; tb_m0_dout = 32'h2; #10;
		tb_m0_address = 8'h2; tb_m0_dout = 32'h4; #10;
		tb_m0_address = 8'h3; tb_m0_dout = 32'h6; #10;
		tb_m0_address = 8'h20; tb_m0_dout = 32'h20; #10;
		tb_m0_address = 8'h21; tb_m0_dout = 32'h22; #10;
		tb_m0_address = 8'h22; tb_m0_dout = 32'h24; #10;
		tb_m0_address = 8'h23; tb_m0_dout = 32'h26; #10;
		tb_m0_address = 8'ha0; tb_m0_dout = 32'hff; #20;
		
		$stop;
	end
	
endmodule
