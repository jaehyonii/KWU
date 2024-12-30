module bus_arbit(clk, reset_n, m_req, m_grant);
	input  	  clk, reset_n, m_req;
	output reg m_grant;
	
	parameter p_grant = 1'b1;
	parameter n_grant = 1'b0;
	// Sequential circuit part
	reg state, next_state;
	
	// state change
	always@ (posedge clk, negedge reset_n) begin
		if(reset_n == 1'b0) state = n_grant;
		else state = next_state;
	end
	
	// state transition condition
	always @(m_req) begin
		case (m_req)
			1'b0: next_state <= n_grant;
			1'b1: next_state <= p_grant;
			
			default: next_state <= 1'bx;
		endcase
	end
	
	// state output generation
	always @(state) begin
		if(state == p_grant) m_grant <= 1'b1;
		else						m_grant <= 1'b0;
	end
	
endmodule
