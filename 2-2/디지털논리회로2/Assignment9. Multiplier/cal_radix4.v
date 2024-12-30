module cal_radix4(multiplier, last_3bits, result_in, result_out);
	input				  [2:0]  last_3bits;
	input				  [63:0] multiplier;
	input	signed	  [127:0] result_in;
	output reg		  [127:0] result_out;
	
	wire signed [127:0] add_out, sub_out;
	wire signed [127:0] shift_add_out, shift_sub_out;
	
	cla128 cla128_add			(.a({multiplier,  64'h0}),					  .b(result_in), .ci(1'b0), .s(add_out), .co());
	cla128 cla128_sub			(.a({~multiplier, 64'hffffffffffffffff}), .b(result_in), .ci(1'b1), .s(sub_out), .co());
	cla128 cla128_shift_add (.a({multiplier,  64'h0}),					  .b(result_in >>> 1), .ci(1'b0), .s(shift_add_out), .co());
	cla128 cla128_shift_sub (.a({~multiplier, 64'hffffffffffffffff}), .b(result_in >>> 1), .ci(1'b1), .s(shift_sub_out), .co());
	
	always @(last_3bits, result_in) begin
		case(last_3bits)
			3'b000, 3'b111: result_out = result_in >>> 2; // shift shift
			3'b001, 3'b010: result_out = add_out >>> 2; // add shift shift
			3'b011:			 result_out = shift_add_out >>> 1; // shift add shift
			3'b100:			 result_out = shift_sub_out >>> 1; // shift sub shift
			3'b101, 3'b110: result_out = sub_out >>> 2; // sub shift shift
			default:			 result_out = 128'bx;
		endcase
	end
	
endmodule
