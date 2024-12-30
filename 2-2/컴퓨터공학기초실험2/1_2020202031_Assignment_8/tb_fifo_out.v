`timescale 1ns/100ps

module tb_fifo_out;
	reg [2:0] tb_state;
	reg [3:0] tb_data_count;
	wire tb_full, tb_empty, tb_wr_ack, tb_wr_err, tb_rd_ack, tb_rd_err;
	
	parameter INIT = 3'b000;
	parameter NO_OP = 3'b001;
	parameter WRITE = 3'b010;
	parameter WR_ERROR = 3'b011;
	parameter READ = 3'b100;
	parameter RD_ERROR = 3'b101;
	
	fifo_out U0(.state(tb_state), .data_count(tb_data_count), .full(tb_full), .empty(tb_empty),
				.wr_ack(tb_wr_ack), .wr_err(tb_wr_err), .rd_ack(tb_rd_ack), .rd_err(tb_rd_err));
	
	initial begin
		// expected {tb_full, tb_empty, tb_wr_ack, tb_wr_err, tb_rd_ack, tb_rd_err}
		tb_state = INIT; tb_data_count = 4'h0; #10; // expected 0 1 0 0 0 0
		tb_state = INIT; tb_data_count = 4'h4; #10; // expected 0 0 0 0 0 0
		tb_state = INIT; tb_data_count = 4'h8; #10; // expected 1 0 0 0 0 0
		
		tb_state = NO_OP; tb_data_count = 4'h0; #10; // expected 0 1 0 0 0 0
		tb_state = NO_OP; tb_data_count = 4'h4; #10; // expected 0 0 0 0 0 0
		tb_state = NO_OP; tb_data_count = 4'h8; #10; // expected 1 0 0 0 0 0
		
		tb_state = WRITE; tb_data_count = 4'h0; #10; // expected 0 1 1 0 0 0
		tb_state = WRITE; tb_data_count = 4'h4; #10; // expected 0 0 1 0 0 0
		tb_state = WRITE; tb_data_count = 4'h8; #10; // expected 1 0 1 0 0 0
		
		tb_state = WR_ERROR; tb_data_count = 4'h0; #10; // expected 0 1 0 1 0 0
		tb_state = WR_ERROR; tb_data_count = 4'h4; #10; // expected 0 0 0 1 0 0
		tb_state = WR_ERROR; tb_data_count = 4'h8; #10; // expected 1 0 0 1 0 0
		
		tb_state = READ; tb_data_count = 4'h0; #10; // expected 0 1 0 0 1 0
		tb_state = READ; tb_data_count = 4'h4; #10; // expected 0 0 0 0 1 0
		tb_state = READ; tb_data_count = 4'h8; #10; // expected 1 0 0 0 1 0
		
		tb_state = RD_ERROR; tb_data_count = 4'h0; #10; // expected 0 1 0 0 0 1
		tb_state = RD_ERROR; tb_data_count = 4'h4; #10; // expected 0 0 0 0 0 1
		tb_state = RD_ERROR; tb_data_count = 4'h8; #10; // expected 1 0 0 0 0 1
		
		$stop;
	end
endmodule