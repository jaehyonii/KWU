`timescale 1ns/100ps

module tb_bus;
	reg 		  tb_clk, tb_reset_n;
	reg 		  tb_m_req;
	reg 		  tb_m_wr;
	reg [15:0] tb_m_address;
	reg [63:0] tb_m_dout;
	reg [63:0] tb_s0_dout, tb_s1_dout;
	
	wire 		   tb_m_grant;
	wire [63:0] tb_m_din;
	wire 		   tb_s0_sel, tb_s1_sel;
	wire [15:0] tb_s_address;
	wire 		   tb_s_wr;
	wire [63:0] tb_s_din;

	Bus U0(.clk(tb_clk), .reset_n(tb_reset_n), .m_req(tb_m_req), .m_wr(tb_m_wr), .m_address(tb_m_address),
				.m_dout(tb_m_dout), .s0_dout(tb_s0_dout), .s1_dout(tb_s1_dout),
				.m_grant(tb_m_grant), .m_din(tb_m_din), .s0_sel(tb_s0_sel), .s1_sel(tb_s1_sel),
				.s_address(tb_s_address), .s_wr(tb_s_wr), .s_din(tb_s_din));
	
	always begin
		#5; tb_clk = ~tb_clk;
	end
	
	initial begin
		tb_clk = 1; tb_reset_n = 0;
		tb_m_req = 0; tb_m_wr = 0; tb_m_address=16'h0; tb_m_dout=64'h0;
		tb_s0_dout = 64'h0; tb_s1_dout = 64'h0; #17;
		
		tb_reset_n = 1; #10;
		
		tb_m_req = 1;
		tb_s0_dout = 64'h1; tb_s1_dout = 64'h2; #10;
		
		tb_m_wr = 1;
		// write to slave0
		tb_m_address = 16'h0000; tb_m_dout = 64'h02; #10;
		tb_m_address = 16'h0400; tb_m_dout = 64'h04; #10;
		tb_m_address = 16'h0700; tb_m_dout = 64'h06; #10;
		tb_m_address = 16'h07ff; tb_m_dout = 64'h20; #10;
		
		// write to slave1
		tb_m_address = 16'h7000; tb_m_dout = 64'h22; #10;
		tb_m_address = 16'h70ff; tb_m_dout = 64'h24; #10;
		tb_m_address = 16'h7100; tb_m_dout = 64'h26; #10;
		tb_m_address = 16'h71ff; tb_m_dout = 64'hff; #10;
		
		// write to nothing
		tb_m_address = 16'h5000; tb_m_dout = 64'h26; #10;
		tb_m_address = 16'h7200; tb_m_dout = 64'hff; #10;
		
		tb_m_wr = 0; 
		// read from slave0
		tb_m_address = 16'h0000; #10;
		tb_m_address = 16'h0400; #10;
		tb_m_address = 16'h0700; #10;
		tb_m_address = 16'h07ff; #10;
		
		// read from slave1
		tb_m_address = 16'h7000; #10;
		tb_m_address = 16'h70ff; #10;
		tb_m_address = 16'h7100; #10;
		tb_m_address = 16'h71ff; #10;
		
		// read from nothing
		tb_m_address = 16'h5000; #10;
		tb_m_address = 16'h7200; #10;
		
		$stop;
	end
	
endmodule
