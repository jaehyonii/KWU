module fifo(clk, reset_n, rd_en, wr_en, d_in, d_out, full, empty, wr_ack, wr_err, rd_ack, rd_err, data_count);
	input clk, reset_n, rd_en, wr_en;
	input [31:0] d_in;
	output [31:0] d_out;
	output full, empty, wr_ack, wr_err, rd_ack, rd_err;
	output [3:0] data_count;
	
	wire [31:0] rData;
	wire [2:0] state, next_state;
	wire [3:0] d_count, next_d_count;
	wire [2:0] head, next_head;
	wire [2:0] tail, next_tail;
	wire we, re;
	
	fifo_ns U0(.wr_en(wr_en), .rd_en(rd_en), .state(state), .data_count(d_count), .next_state(next_state));
	fifo_cal U1(.next_state(next_state), .head(head), .tail(tail), .data_count(d_count), .we(we), .re(re), 
					.next_head(next_head), .next_tail(next_tail), .next_data_count(next_d_count));
	fifo_out U2(.state(state), .data_count(d_count), .full(full), .empty(empty),
					.wr_ack(wr_ack), .wr_err(wr_err), .rd_ack(rd_ack), .rd_err(rd_err));
	Register_file U3(.clk(clk), .reset_n(reset_n), .wAddr(tail), .wData(d_in), .we(we), .rAddr(head), .rData(rData), .re(re));
	
	register3_r_en reg_state(.clk(clk), .reset_n(reset_n), .en(1'b1), .d(next_state), .q(state));
	register4_r_en reg_data_count(.clk(clk), .reset_n(reset_n), .en(1'b1), .d(next_d_count), .q(d_count));
	register3_r_en reg_head(.clk(clk), .reset_n(reset_n), .en(1'b1), .d(next_head), .q(head));
	register3_r_en reg_tail(.clk(clk), .reset_n(reset_n), .en(1'b1), .d(next_tail), .q(tail));
	register32_r_en reg_dout(.clk(clk), .reset_n(reset_n), .en(1'b1), .d(rData), .q(d_out));
	
	assign data_count = d_count;
endmodule