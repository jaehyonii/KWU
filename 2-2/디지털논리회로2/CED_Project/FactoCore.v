module FactoCore(clk, reset_n, s_sel, s_wr, s_addr, s_din,
						  s_dout, interrupt);
	input				  clk, reset_n, s_sel, s_wr;
	input [15:0]	  s_addr;
	input [63:0]	  s_din;
	
	output reg[63:0] s_dout;
	output			  interrupt;
	
	// W type registers
	reg [63:0] opstart,  next_opstart;
	reg [63:0] opclear,  next_opclear;
	reg [63:0] intrEn,   next_intrEn;
	reg [63:0] operand,  next_operand;
	// R type registers
	reg [63:0] opdone;
	reg [63:0] result_h;
	reg [63:0] result_l;
	
	// Encoded states
	parameter INIT  = 3'b000;
	parameter INPUT = 3'b001;
	parameter FCTRL = 3'b010;
	parameter MUL	 = 3'b011;
	parameter END   = 3'b100;
	parameter CLEAR = 3'b101;
	
	reg [63:0] in_operand, next_in_operand; // copy of operand used to progress factoCore
	reg [63:0] multiplicand;
	reg mulstart, mulclear;
	wire [127:0] mul_result;	
	wire muldone; // to check if multiplier ends
	// combinational circuit part
	_and2 U0_interrupt(.a(intrEn[0]), .b(opdone[0]), .y(interrupt));
	multiplier U1_Mul(.clk(clk), .reset_n(reset_n), .multiplier(in_operand), .multiplicand(multiplicand),	.op_start(mulstart), .op_clear(mulclear),
							.op_done(muldone), .result(mul_result));
	
	
	// state change
	reg [2:0] state, next_state;
	always @(posedge clk, negedge reset_n) begin
		if (reset_n == 1'b0) state <= INIT;
		else			 	   state <= next_state;
	end
	
	// state transition condition
	always @(state, next_opstart, in_operand, next_opclear, muldone, s_sel) begin
		case(state)
			INIT : next_state <= INPUT;
			INPUT: if(next_opstart[0] == 1'b1) next_state <= FCTRL;
					 else								  next_state <= INPUT;
			FCTRL: if(in_operand > 64'd1) 	  next_state <= MUL;
					 else								  next_state <= END;
			MUL  : if(muldone == 1'b1) 		  next_state <= FCTRL;
					 else					 		  	  next_state <= MUL;
			END  : if(next_opclear[0] == 1'b1) next_state <= CLEAR;
					 else								  next_state <= END;
			CLEAR: if(next_opclear[0] == 1'b0) next_state <= INPUT;
					 else								  next_state <= CLEAR;
			default: next_state <= 3'bx;
		endcase
	end
	
	// writable registers transition condition
	always @(s_sel, s_wr, s_addr, s_din, state) begin
		case(state)
			CLEAR: begin
						next_opstart <= 64'd0;
						if(s_sel == 1'b1 && s_wr == 1'b1) begin
							case(s_addr[8:3])
								6'b000_001: next_opclear <= s_din;
								6'b000_011: next_intrEn  <= s_din;
								6'b000_100: next_operand <= s_din;
							endcase
						end
					 end
			INIT,
			INPUT,
			FCTRL,
			MUL,
			END:	 begin
						if(s_sel == 1'b1 && s_wr == 1'b1) begin
							case(s_addr[8:3])
								6'b000_000: next_opstart <= s_din;
								6'b000_001: next_opclear <= s_din;
								6'b000_011: next_intrEn  <= s_din;
								6'b000_100: next_operand <= s_din;
							endcase
						end
					 end
		endcase
	end
	
	
	// writable registers control
	always @(posedge clk, negedge reset_n) begin
		if (reset_n == 0) begin
			opstart <= 64'd0;
			opclear <= 64'd0;
			intrEn  <= 64'd0;
			operand <= 64'd0;
		end
		else begin
			case(next_state)
				INIT : begin
							opstart <= 64'd0;
							opclear <= 64'd0;
							intrEn  <= 64'd0;
							operand <= 64'd0;
						 end
				CLEAR: begin
							opstart  <= 64'd0;
							if(s_sel && s_wr) begin
								if(s_addr[8:3] == 6'b000_001) opclear <= next_opclear;
							end
						 end
				INPUT,
				FCTRL,
				MUL,
				END  : begin
							if(s_sel && s_wr) begin
								case(s_addr[8:3])
									6'b000_000: opstart <= next_opstart;
									6'b000_001: opclear <= next_opclear;
									6'b000_011: intrEn  <= next_intrEn;
									6'b000_100: operand <= next_operand;
								endcase
							end
						 end
				default: begin
								opstart  <= 64'bx;
								opclear  <= 64'bx;
								intrEn   <= 64'bx;
								operand  <= 64'bx;
							end
			endcase
		end
	end
	
	// readable registers control
	always @(state) begin
		case(state)
			INIT,
			CLEAR: begin
						opdone   <= 64'd0;
						result_h <= 64'd0;
						result_l <= 64'd1;
					 end
			INPUT: begin end
			FCTRL: begin
						opdone[1] <= 1'b1;
						if(muldone == 1'b1) begin
							result_h <= mul_result[127:64];
							result_l <= mul_result[63:0];
						end
					 end
			MUL  : begin
					 end
			END  : begin
						opdone[0] <= 1'b1;
					 end
			default: begin
							opdone   <= 64'bx;
							result_h <= 64'bx;
							result_l <= 64'bx;
						end
		endcase
	end
	
	// next_in_operand control
	always @(state, next_operand, in_operand) begin
		if(state == INPUT) 		next_in_operand <= next_operand;
		else if(state == FCTRL) next_in_operand <= in_operand - 64'd1;
	end
	
	// in_operand, multiplicand, mulstart, mulclear control
	always @(state) begin
		case (state)
			INIT,
			CLEAR,
			END,
			INPUT: begin
						in_operand <= 64'd0;
						multiplicand <= 64'd0;
						mulstart <= 1'b0;
						mulclear <= 1'b1;
					 end
			FCTRL: begin
						in_operand <= next_in_operand;
						mulstart <= 1'b0;
						mulclear <= 1'b1;
					 end
			MUL  : begin
						if(result_l == 64'd0) multiplicand <= result_h;
						else 						 multiplicand <= result_l;
						mulstart <= 1'b1;
						mulclear <= 1'b0;
					 end
		endcase
	end
	
	// output control
	always @(s_sel, s_wr, s_addr, opdone, result_h, result_l) begin
		if(s_sel == 1'b0) s_dout <= 64'd0;
		else if(s_wr == 1'b1) s_dout <= 64'd0;
		else begin
			case(s_addr[8:3])
				3'b000_010: s_dout <= opdone;
				3'b000_101: s_dout <= result_h;
				3'b000_110: s_dout <= result_l;
				default: s_dout <= 64'd0;
			endcase
		end
	end
	
	
endmodule
