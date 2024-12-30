`timescale 1ns/100ps

module tb_fifo_ns;
	reg tb_wr_en, tb_rd_en;
	reg [2:0] tb_state;
	reg [3:0] tb_data_count;
	wire [2:0] tb_next_state;
	
	parameter INIT = 3'b000;
	parameter NO_OP = 3'b001;
	parameter WRITE = 3'b010;
	parameter WR_ERROR = 3'b011;
	parameter READ = 3'b100;
	parameter RD_ERROR = 3'b101;
	
	fifo_ns U0(.wr_en(tb_wr_en), .rd_en(tb_rd_en), .state(tb_state), .data_count(tb_data_count), .next_state(tb_next_state));
	
	initial begin
		tb_state = INIT; tb_data_count = 4'h0; tb_wr_en = 0; tb_rd_en = 0; #10; // next = NO_OP
		tb_state = INIT; tb_data_count = 4'h0; tb_wr_en = 1; tb_rd_en = 1; #10; // next = NO_OP
		tb_state = INIT; tb_data_count = 4'h0; tb_wr_en = 0; tb_rd_en = 1; #10; // next = RD_ERROR
		tb_state = INIT; tb_data_count = 4'h0; tb_wr_en = 1; tb_rd_en = 0; #10; // next = WRITE
		
		tb_state = WRITE; tb_data_count = 4'h4; tb_wr_en = 0; tb_rd_en = 0; #10; // next = NO_OP
		tb_state = WRITE; tb_data_count = 4'h4; tb_wr_en = 1; tb_rd_en = 1; #10; // next = NO_OP
		tb_state = WRITE; tb_data_count = 4'h4; tb_wr_en = 0; tb_rd_en = 1; #10; // next = READ
		tb_state = WRITE; tb_data_count = 4'h4; tb_wr_en = 1; tb_rd_en = 0; #10; // next = WRITE
		tb_state = WRITE; tb_data_count = 4'h8; tb_wr_en = 1; tb_rd_en = 0; #10; // next = WR_ERROR
		
		tb_state = WR_ERROR; tb_data_count = 4'h4; tb_wr_en = 0; tb_rd_en = 0; #10; // next = NO_OP
		tb_state = WR_ERROR; tb_data_count = 4'h4; tb_wr_en = 1; tb_rd_en = 1; #10; // next = NO_OP		
		tb_state = WR_ERROR; tb_data_count = 4'h4; tb_wr_en = 0; tb_rd_en = 1; #10; // next = READ
		tb_state = WR_ERROR; tb_data_count = 4'h8; tb_wr_en = 1; tb_rd_en = 0; #10; // next = WR_ERROR
		
		tb_state = READ; tb_data_count = 4'h4; tb_wr_en = 0; tb_rd_en = 0; #10; // next = NO_OP
		tb_state = READ; tb_data_count = 4'h4; tb_wr_en = 1; tb_rd_en = 1; #10; // next = NO_OP		
		tb_state = READ; tb_data_count = 4'h4; tb_wr_en = 0; tb_rd_en = 1; #10; // next = READ
		tb_state = READ; tb_data_count = 4'h0; tb_wr_en = 0; tb_rd_en = 1; #10; // next = RD_ERROR
		tb_state = READ; tb_data_count = 4'h4; tb_wr_en = 1; tb_rd_en = 0; #10; // next = WRITE
		
		tb_state = RD_ERROR; tb_data_count = 4'h4; tb_wr_en = 0; tb_rd_en = 0; #10; // next = NO_OP
		tb_state = RD_ERROR; tb_data_count = 4'h4; tb_wr_en = 1; tb_rd_en = 1; #10; // next = NO_OP		
		tb_state = RD_ERROR; tb_data_count = 4'h0; tb_wr_en = 0; tb_rd_en = 1; #10; // next = RD_ERROR
		tb_state = RD_ERROR; tb_data_count = 4'h8; tb_wr_en = 1; tb_rd_en = 0; #10; // next = WRITE
		
		tb_state = NO_OP; tb_data_count = 4'h4; tb_wr_en = 0; tb_rd_en = 0; #10; // next = NO_OP
		tb_state = NO_OP; tb_data_count = 4'h4; tb_wr_en = 1; tb_rd_en = 1; #10; // next = NO_OP		
		tb_state = NO_OP; tb_data_count = 4'h4; tb_wr_en = 0; tb_rd_en = 1; #10; // next = READ
		tb_state = NO_OP; tb_data_count = 4'h0; tb_wr_en = 0; tb_rd_en = 1; #10; // next = RD_ERROR
		tb_state = NO_OP; tb_data_count = 4'h4; tb_wr_en = 1; tb_rd_en = 0; #10; // next = WRITE
		tb_state = NO_OP; tb_data_count = 4'h8; tb_wr_en = 1; tb_rd_en = 0; #10; // next = WR_ERROR
		
		$stop;
	end
	
endmodule