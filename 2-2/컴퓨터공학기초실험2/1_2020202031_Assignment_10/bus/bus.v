module bus(clk, reset_n, m0_req, m1_req, m0_wr, m1_wr, m0_address, m1_address, m0_dout, m1_dout, s0_dout, s1_dout,
				m0_grant, m1_grant, m_din, s0_sel, s1_sel, s_wr, s_address, s_din);
	input 		 clk, reset_n;
	input 		 m0_req, m1_req;
	input 		 m0_wr, m1_wr;
	input [7:0]  m0_address, m1_address;
	input [31:0] m0_dout, m1_dout;
	input [31:0] s0_dout, s1_dout;
	
	output 		  m0_grant, m1_grant;
	output [31:0] m_din;
	output 		  s0_sel, s1_sel;
	output 		  s_wr;
	output [7:0]  s_address;
	output [31:0] s_din;
	
	reg [1:0] m_din_sel;
	
	bus_arbit U0(.clk(clk), .reset_n(reset_n), .m0_req(m0_req), .m1_req(m1_req), .m0_grant(m0_grant), .m1_grant(m1_grant));
	bus_addr U1(.address(s_address), .s0_sel(s0_sel), .s1_sel(s1_sel));
	
	mx2 U2_wr(.d0(m0_wr), .d1(m1_wr), .s(m1_grant), .y(s_wr)); // decide s_wr
	mx2_8bits u3_address(.d0(m0_address), .d1(m1_address), .s(m1_grant), .y(s_address)); // decide address
	mx2_32bits U4_data_to_slave(.d0(m0_dout), .d1(m1_dout), .s(m1_grant), .y(s_din)); // decide data
	
	always@(posedge clk, negedge reset_n)begin
		if(reset_n == 1'b0) m_din_sel <= 2'b00;
		else m_din_sel <= {s0_sel, s1_sel};
	end
	
	mx3_32bits U5_data_to_master(.d0(s0_dout), .d1(s1_dout), .s(m_din_sel), .y(m_din));
	
endmodule
