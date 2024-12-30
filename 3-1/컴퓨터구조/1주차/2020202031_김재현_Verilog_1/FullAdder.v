module FullAdder(a, b, c_in, sum, c_out);
	input a;
	input b;
	input c_in;
	output c_out;
	output sum;

	assign {c_out, sum} = a + b + c_in;
endmodule