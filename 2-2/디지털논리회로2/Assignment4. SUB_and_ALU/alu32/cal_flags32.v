module cal_flags32(op, result, co_add, co_prev_add, c, n, z, v);
	input [2:0] op; // opcode
	input [31:0] result;
	input co_add, co_prev_add; // carry outs of ind 3, 2 calculation
	output c, n, z, v; // flags

	assign c = (op[2:1] != 2'b11) ? 1'b0 : co_add; //carry
	assign n = result[31]; //negative
	assign z = (result == 32'b0) ? 1'b1 : 1'b0; // zero
	assign v = (op[2:1] != 2'b11) ? 1'b0 : co_add ^ co_prev_add; // overflow
endmodule