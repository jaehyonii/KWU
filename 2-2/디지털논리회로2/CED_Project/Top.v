module Top(clk, reset_n, m_req, m_wr, m_addr, m_dout,
				m_grant, m_din, interrupt);
	input			  clk, reset_n;
	input			  m_req, m_wr;
	input [15:0]  m_addr;
	input [63:0]  m_dout;
	
	output		  m_grant;
	output [63:0] m_din;
	output		  interrupt;
	
	wire [63:0] s0_dout, s1_dout;
	wire			s0_sel, s1_sel;
	wire [15:0] s_addr;
	wire			s_wr;
	wire [63:0] s_din;
	
	Bus U0_Bus(.clk(clk), .reset_n(reset_n), .m_req(m_req), .m_wr(m_wr), .m_address(m_addr), .m_dout(m_dout), .s0_dout(s0_dout), .s1_dout(s1_dout),
			.m_grant(m_grant), .m_din(m_din), .s0_sel(s0_sel), .s1_sel(s1_sel), .s_address(s_addr), .s_wr(s_wr), .s_din(s_din));
	ram U1_ram(.clk(clk), .cen(s0_sel), .wen(s_wr), .s_addr(s_addr[10:3]), .s_din(s_din),
			.s_dout(s0_dout));
	FactoCore U2_FactoCore(.clk(clk), .reset_n(reset_n), .s_sel(s1_sel), .s_wr(s_wr), .s_addr(s_addr), .s_din(s_din),
					.s_dout(s1_dout), .interrupt(interrupt));
					
endmodule
