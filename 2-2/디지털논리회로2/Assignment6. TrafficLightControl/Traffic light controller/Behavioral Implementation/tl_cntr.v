module tl_cntr (clk, reset_n, Ta, Tb, La, Lb);
	input clk, reset_n, Ta, Tb;
	output [1:0] La, Lb;

	// Encoded states
	parameter S0 = 2'b00; 
	parameter S1 = 2'b01;
	parameter S2 = 2'b10;
	parameter S3 = 2'b11;

	// Encoded colors for outputs la and lb
	parameter GREEN = 2'b00;
	parameter YELLOW = 2'b01;
	parameter RED = 2'b10;
	
	// Sequential circuit part
	reg [1:0] state;
	reg [1:0] next_state;
	always @ (posedge clk or negedge reset_n) begin
		if (reset_n == 1'b0) state <= S0;
		else state <= next_state;
	end

	// Combinational circuit part
	// State transition condition
	always @ (state or Ta or Tb) begin
		casex ({state, Ta, Tb})
			{S0, 1'b0, 1'bx}: next_state <= S1;
			{S0, 1'b1, 1'bx}: next_state <= S0;
			{S1, 1'bx, 1'bx}: next_state <= S2;
			{S2, 1'bx, 1'b0}: next_state <= S3;
			{S2, 1'bx, 1'b1}: next_state <= S2;
			{S3, 1'bx, 1'bx}: next_state <= S0;
			default: next_state <= 2'bx;
		endcase
	end 

	reg [1:0] La, Lb;
	// State output generation
	always @ (state) begin
		casex (state)
			S0: begin La = GREEN; Lb = RED; end
			S1: begin La = YELLOW; Lb = RED; end
			S2: begin La = RED; Lb = GREEN; end
			S3: begin La = RED; Lb = YELLOW; end
			default: next_state <= 3'bx;
		endcase
	end
	
endmodule
