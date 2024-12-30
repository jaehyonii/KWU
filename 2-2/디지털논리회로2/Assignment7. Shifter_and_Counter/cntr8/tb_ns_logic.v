`timescale 1ns/100ps

module tb_ns_logic;
	reg tb_load, tb_inc;
	reg [2:0] tb_state;
	wire [2:0] tb_next_state;
	
	ns_logic U0(.load(tb_load), .inc(tb_inc), .state(tb_state), .next_state(tb_next_state));
	
	parameter IDLE_STATE = 3'b000;
	parameter LOAD_STATE = 3'b001;
	parameter INC_STATE = 3'b010;
	parameter INC2_STATE = 3'b011;
	parameter DEC_STATE = 3'b100;
	parameter DEC2_STATE = 3'b101;
	
	initial begin	
		// load have priority
		// when load is 0 inc affects
		// when load is 1 inc can't affect
	
		tb_state = IDLE_STATE;
		tb_load = 0; tb_inc = 0; #10;
		tb_load = 0; tb_inc = 1; #10;
		tb_load = 1; tb_inc = 0; #10;
		tb_load = 1; tb_inc = 1; #10;
		
		tb_state = LOAD_STATE;
		tb_load = 0; tb_inc = 0; #10;
		tb_load = 0; tb_inc = 1; #10;
		tb_load = 1; tb_inc = 0; #10;
		tb_load = 1; tb_inc = 1; #10;
		
		tb_state = INC_STATE;
		tb_load = 0; tb_inc = 0; #10;
		tb_load = 0; tb_inc = 1; #10;
		tb_load = 1; tb_inc = 0; #10;
		tb_load = 1; tb_inc = 1; #10;
		
		tb_state = INC2_STATE;
		tb_load = 0; tb_inc = 0; #10;
		tb_load = 0; tb_inc = 1; #10;
		tb_load = 1; tb_inc = 0; #10;
		tb_load = 1; tb_inc = 1; #10;
		
		tb_state = DEC_STATE;
		tb_load = 0; tb_inc = 0; #10;
		tb_load = 0; tb_inc = 1; #10;
		tb_load = 1; tb_inc = 0; #10;
		tb_load = 1; tb_inc = 1; #10;
		
		tb_state = DEC2_STATE;
		tb_load = 0; tb_inc = 0; #10;
		tb_load = 0; tb_inc = 1; #10;
		tb_load = 1; tb_inc = 0; #10;
		tb_load = 1; tb_inc = 1; #10;
		
		$stop;
	end
endmodule