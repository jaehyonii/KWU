module bus_addr(m_grant, address, s0_sel, s1_sel);
	input			 m_grant;
	input [15:0] address;
	output reg	 s0_sel, s1_sel;
	
	parameter p_grant = 1'b1;
	parameter n_grant = 1'b0;
	
	always @(address, m_grant) begin
		case(m_grant)
			p_grant : if		(16'h0000<=address && address<=16'h07ff)	{s0_sel, s1_sel} = 2'b10; // slave0 memory map
						 else if (16'h7000<=address && address<=16'h71ff)	{s0_sel, s1_sel} = 2'b01; // slave1 memory map
						 else																{s0_sel, s1_sel} = 2'b00; // not included in any memory maps
			n_grant : {s0_sel, s1_sel} = 2'b00; // master doesn't have grant
		endcase
	end
	
endmodule
