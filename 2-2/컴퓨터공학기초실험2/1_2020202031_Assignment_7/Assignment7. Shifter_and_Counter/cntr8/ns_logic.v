module ns_logic(load, inc, state, next_state);
	
	parameter IDLE_STATE = 3'b000;
	parameter LOAD_STATE = 3'b001;
	parameter INC_STATE = 3'b010;
	parameter INC2_STATE = 3'b011;
	parameter DEC_STATE = 3'b100;
	parameter DEC2_STATE = 3'b101;
	
	input load, inc;
	input [2:0] state;
	output reg [2:0] next_state;
	
	always @ (load, inc, state)
	begin
		casex({state, load, inc})
			{IDLE_STATE, 1'b1, 1'bx} : next_state = LOAD_STATE;
			{IDLE_STATE, 1'b0, 1'b0} : next_state = DEC_STATE;
			{IDLE_STATE, 1'b0, 1'b1} : next_state = INC_STATE;
			
			{LOAD_STATE, 1'b1, 1'bx} : next_state = LOAD_STATE;
			{LOAD_STATE, 1'b0, 1'b0} : next_state = DEC_STATE;
			{LOAD_STATE, 1'b0, 1'b1} : next_state = INC_STATE;
			
			{INC_STATE, 1'b1, 1'bx} : next_state = LOAD_STATE;
			{INC_STATE, 1'b0, 1'b0} : next_state = DEC_STATE;
			{INC_STATE, 1'b0, 1'b1} : next_state = INC2_STATE;
			
			{INC2_STATE, 1'b1, 1'bx} : next_state = LOAD_STATE;
			{INC2_STATE, 1'b0, 1'b0} : next_state = DEC_STATE;
			{INC2_STATE, 1'b0, 1'b1} : next_state = INC_STATE;
			
			{DEC_STATE, 1'b1, 1'bx} : next_state = LOAD_STATE;
			{DEC_STATE, 1'b0, 1'b0} : next_state = DEC2_STATE;
			{DEC_STATE, 1'b0, 1'b1} : next_state = INC_STATE;
			
			{DEC2_STATE, 1'b1, 1'bx} : next_state = LOAD_STATE;
			{DEC2_STATE, 1'b0, 1'b0} : next_state = DEC_STATE;
			{DEC2_STATE, 1'b0, 1'b1} : next_state = INC_STATE;
			default : next_state = 3'bx;
		endcase
	end
	
endmodule