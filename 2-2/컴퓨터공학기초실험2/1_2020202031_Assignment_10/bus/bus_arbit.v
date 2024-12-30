module bus_arbit(clk, reset_n, m0_req, m1_req, m0_grant, m1_grant);
	input  	  clk, reset_n, m0_req, m1_req;
	output reg m0_grant, m1_grant;
	
	// Encoded states
	parameter M0_grant = 1'b0;
	parameter M1_grant = 1'b1;
	
	// Sequential circuit part
	reg state, next_state;
	
	// state change
	always@ (posedge clk, negedge reset_n) begin
		if(reset_n == 1'b0) state = M0_grant;
		else state = next_state;
	end
	
	// combinational circuit part
	
	// state transition condition
	always @(state, m0_req, m1_req) begin
		casex ({state, m0_req, m1_req})
			{M0_grant, 1'b1, 1'bx},
			{M0_grant, 1'b0, 1'b0}: next_state <= M0_grant;
			{M0_grant, 1'b0, 1'b1}: next_state <= M1_grant;
			
			{M1_grant, 1'bx, 1'b0}: next_state <= M0_grant;
			{M1_grant, 1'bx, 1'b1}: next_state <= M1_grant;
			
			default: next_state <= 1'bx;
		endcase
	end
	
	
	// state output generation
	always @(state) begin
		if(state == M0_grant) begin m0_grant <= 1'b1; m1_grant <= 1'b0; end
		else begin m0_grant <= 1'b0; m1_grant <= 1'b1; end
	end
	
endmodule
