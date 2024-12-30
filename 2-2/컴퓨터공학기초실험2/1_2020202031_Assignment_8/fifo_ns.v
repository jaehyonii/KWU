module fifo_ns(wr_en, rd_en, state, data_count, next_state);
	input wr_en, rd_en;
	input [2:0] state;
	input [3:0] data_count;
	output reg [2:0] next_state;

	parameter INIT = 3'b000;
	parameter NO_OP = 3'b001;
	parameter WRITE = 3'b010;
	parameter WR_ERROR = 3'b011;
	parameter READ = 3'b100;
	parameter RD_ERROR = 3'b101;
	
	always @(state, wr_en, rd_en, data_count) begin
		casex({state, wr_en, rd_en})
			{3'bx, 1'b1, 1'b1} : next_state <= NO_OP;
			{3'bx, 1'b0, 1'b0} : next_state <= NO_OP;
			
			{INIT, 1'b0, 1'b1} : next_state <= RD_ERROR;
			{INIT, 1'b1, 1'b0} : next_state <= WRITE;
			
			{WRITE, 1'b0, 1'b1} : next_state <= READ;
			{WRITE, 1'b1, 1'b0} : begin
											if(data_count < 4'h8) next_state <= WRITE;
											else next_state <= WR_ERROR;
										 end
			
			{WR_ERROR, 1'b0, 1'b1} : next_state <= READ;
			{WR_ERROR, 1'b1, 1'b0} : next_state <= WR_ERROR;
			
			{READ, 1'b1, 1'b0} : next_state <= WRITE;
			{READ, 1'b0, 1'b1} :	begin
											if(data_count > 4'h0) next_state <= READ;
											else next_state <= RD_ERROR;
										end
			
			{RD_ERROR, 1'b0, 1'b1} : next_state <= RD_ERROR;
			{RD_ERROR, 1'b1, 1'b0} : next_state <= WRITE;
			
			{NO_OP, 1'b0, 1'b1} : begin
											if(data_count > 4'b0) next_state <= READ;
											else next_state <= RD_ERROR;
										 end
			{NO_OP, 1'b1, 1'b0} : begin
											if(data_count < 4'h8) next_state <= WRITE;
											else next_state <= WR_ERROR;
										 end
		endcase
	end
	
endmodule