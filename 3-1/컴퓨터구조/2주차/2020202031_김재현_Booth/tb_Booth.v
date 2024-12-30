`timescale 1ns/1ps
`include "booth.v"
module tb_booth;
	reg [3:0] input1, input2;
	reg clk, start, reset;
	wire [7:0] result;
	wire [1:0] count;

	booth Booth(.input1(input1), .input2(input2), .clk(clk), .start(start), .reset(reset), .result(result), .count(count));

	always #5 clk = ~clk;

	initial begin
		$dumpfile("tb_booth.vcd");
		$dumpvars(0, tb_booth);

		start = 0;
		reset = 0;
		clk = 0;
		input1 = 4;
		input2 = 5;
		#10;
		reset = 1;
		#10;
		start = 1;
		#100;
		
		start = 0;
		reset = 0;
		input1 = -4;
		input2 = 5;
		#10;
		reset = 1;
		#10;
		start = 1;
		#100;
		
		start = 0;
		reset = 0;
		input1 = 4;
		input2 = -5;
		#10;
		reset = 1;
		#10;
		start = 1;
		#100;
		
		start = 0;
		reset = 0;
		input1 = -4;
		input2 = -5;
		#10;
		reset = 1;
		#10;
		start = 1;
		#100;
		
		start = 0;
		reset = 0;
		input1 = 5;
		input2 = 0;
		#10;
		reset = 1;
		#10;
		start = 1;
		#100;
		
		start = 0;
		reset = 0;
		input1 = 0;
		input2 = 5;
		#10;
		reset = 1;
		#10;
		start = 1;
		#100;
		
		start = 0;
		reset = 0;
		input1 = 5;
		input2 = -8;
		#10;
		reset = 1;
		#10;
		start = 1;
		#100;
		
		start = 0;
		reset = 0;
		input1 = -8;
		input2 = 5;
		#10;
		reset = 1;
		#10;
		start = 1;
		#640; $finish;
	end
endmodule