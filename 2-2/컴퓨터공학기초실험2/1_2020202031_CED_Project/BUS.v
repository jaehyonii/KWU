module Bus(clk, reset_n, m_req, m_wr, m_address, m_dout, s0_dout, s1_dout,
				m_grant, m_din, s0_sel, s1_sel, s_address, s_wr, s_din);
	input 		 clk, reset_n;
	input 		 m_req, m_wr;
	input [15:0] m_address;
	input [63:0] m_dout;
	input [63:0] s0_dout, s1_dout;
	
	output 		  m_grant;
	output [63:0] m_din;
	output 		  s0_sel, s1_sel;
	output [15:0] s_address;
	output 		  s_wr;
	output [63:0] s_din;
	
	reg [1:0] m_din_sel; // used to select which s_dout to be delivered to m_din
	always@(posedge clk, negedge reset_n)begin
		if(reset_n == 1'b0) m_din_sel <= 2'b00;
		else					  m_din_sel <= {s0_sel, s1_sel};
	end
	mx3_64bits U5_data_to_master(.d0(s0_dout), .d1(s1_dout), .s(m_din_sel), .y(m_din));
	
	bus_arbit U0(.clk(clk), .reset_n(reset_n), .m_req(m_req), .m_grant(m_grant));
	bus_addr U1(.m_grant(m_grant), .address(m_address), .s0_sel(s0_sel), .s1_sel(s1_sel));
	
	assign s_wr = m_wr; // decide s_wr
	assign s_address = m_address; // decide address
	assign s_din = m_dout; // decide data
	
endmodule
