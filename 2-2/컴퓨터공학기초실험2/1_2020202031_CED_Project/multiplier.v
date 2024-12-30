module multiplier(clk, reset_n, multiplier, multiplicand, op_start, op_clear, op_done, result);
	input		clk, reset_n;
	input		[63:0] multiplier, multiplicand;
	input		op_start, op_clear;
	output reg	op_done;
	output reg	[127:0] result;

	// Encoded states
	parameter INIT = 2'b00;
	parameter SET  = 2'b01;
	parameter MUL  = 2'b10;
	parameter DONE = 2'b11;
	
	// Sequential circuit part
	reg  [63:0]  cp_multiplier, cp_multiplicand;
	reg  	 		 LSB;
	reg  [1:0]	 state, next_state;
	reg  [7:0]	 count;
	wire [7:0]	 next_count;
	wire [127:0] next_result;
	
	// state change
	always @(posedge clk, negedge reset_n) begin
		if(reset_n == 1'b0) state <= INIT;
		else state <= next_state;
	end
	
	// combinational circuit part
	cal_radix4 U0(.multiplier(cp_multiplier), .last_3bits({cp_multiplicand[1:0], LSB}), .result_in(result), .result_out(next_result));
	cla8 cla8_count(.a(count), .b(8'b1), .ci(1'b0), .s(next_count), .co());
	
	// state transition condition
	always @(state or op_start or op_clear or count) begin
		case({state, op_start, op_clear})
			{INIT, 1'b0, 1'b0},
			{INIT, 1'b0, 1'b1}: next_state <= INIT;
			{INIT, 1'b1, 1'b0}: next_state <= SET;
			
			{SET , 1'b0, 1'b0},
			{SET , 1'b1, 1'b0}: next_state <= MUL;
			{SET , 1'b0, 1'b1}: next_state <= INIT;

			{MUL , 1'b0, 1'b0},
			{MUL , 1'b1, 1'b0}: begin
										if(count < 8'd31) next_state <= MUL;	
										else next_state <= DONE;
									  end
			{MUL , 1'b0, 1'b1}: next_state <= INIT;

			{DONE, 1'b0, 1'b0},
			{DONE, 1'b1, 1'b0}: next_state <= DONE;
			{DONE, 1'b0, 1'b1}: next_state <= INIT;
			
			default: next_state <= 2'bx;
		endcase
	end
	
	// state output generation
	always @(state, count) begin
		case(state)
			INIT: begin op_done <= 1'b0; result <= 128'h0; end
			SET : begin op_done <= 1'b0; result <= 128'h0; end
			MUL : begin op_done <= 1'b0; result <= next_result; end
			DONE: begin op_done <= 1'b1; result <= next_result;end
			default: begin op_done <= 1'bx; result <= 128'hx; end
		endcase
	end
	
	// count control
	always @(posedge clk) begin
		if(state == INIT) count <= 8'h0;
		else if(state != DONE) count <= next_count;
	end
	
	// operands control
	always @(state, count) begin
		case(state)
			INIT: begin	end
			SET : begin 
							LSB <= 1'b0;
							cp_multiplier <= multiplier;
							cp_multiplicand <= multiplicand;
					end
			MUL : begin
							LSB <= cp_multiplicand[1];
							cp_multiplicand <= cp_multiplicand >> 2;
					end
			DONE: begin end
			
			default: begin cp_multiplier <= 64'hx; cp_multiplicand <= 64'hx; LSB <= 1'bx; end
		endcase
	end
	
	
endmodule
