`timescale 1ns/100ps

module tb_fifo_cal;
	reg [2:0] tb_next_state, tb_head, tb_tail;
	reg [3:0] tb_data_count;
	wire tb_we, tb_re;
	wire [2:0] tb_next_head, tb_next_tail;
	wire [3:0] tb_next_data_count;
	
	parameter INIT = 3'b000;
	parameter NO_OP = 3'b001;
	parameter WRITE = 3'b010;
	parameter WR_ERROR = 3'b011;
	parameter READ = 3'b100;
	parameter RD_ERROR = 3'b101;
	
	fifo_cal U0(.next_state(tb_next_state), .head(tb_head), .tail(tb_tail), .data_count(tb_data_count),
				.we(tb_we), .re(tb_re), .next_head(tb_next_head), .next_tail(tb_next_tail), .next_data_count(tb_next_data_count));
	
	initial begin // expected tb_we, tb_re, tb_next_head, tb_next_tail, tb_next_data_count
		tb_next_state = INIT; tb_head = 3'h7; tb_tail = 3'h7; tb_data_count = 4'h8; #10; // expected 0 1 0 0 0
		tb_next_state = INIT; tb_head = 3'h0; tb_tail = 3'h5; tb_data_count = 4'h5; #10; // expected 1 1 0 0 0
		tb_next_state = INIT; tb_head = 3'h7; tb_tail = 3'h2; tb_data_count = 4'h3; #10; // expected 1 1 0 0 0
		tb_next_state = INIT; tb_head = 3'h2; tb_tail = 3'h2; tb_data_count = 4'h0; #10; // expected 1 0 0 0 0
		
		tb_next_state = NO_OP; tb_head = 3'h2; tb_tail = 3'h2; tb_data_count = 4'h8; #10; // expected 0 1 2 2 8
		tb_next_state = NO_OP; tb_head = 3'h0; tb_tail = 3'h5; tb_data_count = 4'h5; #10; // expected 1 1 0 5 5
		tb_next_state = NO_OP; tb_head = 3'h7; tb_tail = 3'h2; tb_data_count = 4'h3; #10; // expected 1 1 7 2 3
		tb_next_state = NO_OP; tb_head = 3'h2; tb_tail = 3'h2; tb_data_count = 4'h0; #10; // expected 1 0 2 2 0
		
		tb_next_state = WRITE; tb_head = 3'h0; tb_tail = 3'h5; tb_data_count = 4'h5; #10; // expected 1 1 0 6 6
		tb_next_state = WRITE; tb_head = 3'h7; tb_tail = 3'h2; tb_data_count = 4'h3; #10; // expected 1 1 7 3 4
		tb_next_state = WRITE; tb_head = 3'h2; tb_tail = 3'h2; tb_data_count = 4'h0; #10; // expected 1 0 2 3 1
		
		tb_next_state = WR_ERROR; tb_head = 3'h2; tb_tail = 3'h2; tb_data_count = 4'h8; #10; // expected 0 1 2 2 8
		
		tb_next_state = READ; tb_head = 3'h2; tb_tail = 3'h2; tb_data_count = 4'h8; #10; // expected 0 1 3 2 7
		tb_next_state = READ; tb_head = 3'h0; tb_tail = 3'h5; tb_data_count = 4'h5; #10; // expected 1 1 1 5 4
		tb_next_state = READ; tb_head = 3'h7; tb_tail = 3'h2; tb_data_count = 4'h3; #10; // expected 1 1 0 2 2
		
		tb_next_state = RD_ERROR; tb_head = 3'h2; tb_tail = 3'h2; tb_data_count = 4'h0; #10; // expected 1 0 2 2 0
		
		$stop;
	end
	
endmodule