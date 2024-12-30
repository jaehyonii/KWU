`timescale 1ns/100ps

module tb_shifter8;
	reg tb_clk, tb_reset_n;
	reg [2:0] tb_op;
	reg [1:0] tb_shamt;
	reg [7:0] tb_d_in;
	wire [7:0] tb_d_out;
	
	shifter8 U0_shifter8(.clk(tb_clk), .reset_n(tb_reset_n), .op(tb_op),	.shamt(tb_shamt), .d_in(tb_d_in), .d_out(tb_d_out));
	
	always begin
		#5; tb_clk = ~tb_clk; // clock period is 10ns
	end
	
	initial begin
		tb_clk = 0; tb_reset_n = 0; #3; // initialization
		tb_reset_n = 1;
		tb_d_in = 8'b01100111;
		
		tb_op = 3'b000; tb_shamt = 2'b00; #10;
		tb_op = 3'b000; tb_shamt = 2'b01; #10;
		tb_op = 3'b000; tb_shamt = 2'b10; #10;
		tb_op = 3'b000; tb_shamt = 2'b11; #10;
		
		tb_op = 3'b001; tb_shamt = 2'b00; #10;
		tb_op = 3'b001; tb_shamt = 2'b01; #10;
		tb_op = 3'b001; tb_shamt = 2'b10; #10;
		tb_op = 3'b001; tb_shamt = 2'b11; #10;
		
		tb_op = 3'b010; tb_shamt = 2'b00; #10;
		tb_op = 3'b010; tb_shamt = 2'b01; #10;
		tb_op = 3'b010; tb_shamt = 2'b10; #10;
		tb_op = 3'b010; tb_shamt = 2'b11; #10;
		
		tb_op = 3'b011; tb_shamt = 2'b00; #10;
		tb_op = 3'b011; tb_shamt = 2'b01; #10;
		tb_op = 3'b011; tb_shamt = 2'b10; #10;
		tb_op = 3'b011; tb_shamt = 2'b11; #10;
		
		tb_op = 3'b100; tb_shamt = 2'b00; #10;
		tb_op = 3'b100; tb_shamt = 2'b01; #10;
		tb_op = 3'b100; tb_shamt = 2'b10; #10;
		tb_op = 3'b100; tb_shamt = 2'b11; #10;
		
		tb_d_in = 8'b10110011; // check MSB of ASL result
		tb_op = 3'b100; tb_shamt = 2'b00; #10;
		tb_op = 3'b100; tb_shamt = 2'b01; #10;
		tb_op = 3'b100; tb_shamt = 2'b10; #10;
		tb_op = 3'b100; tb_shamt = 2'b11; #10;
		
		// check if op is not valid
		tb_op = 3'b101; tb_shamt = 2'b00; #10;
		tb_op = 3'b101; tb_shamt = 2'b01; #10;
		tb_op = 3'b101; tb_shamt = 2'b10; #10;
		tb_op = 3'b101; tb_shamt = 2'b11; #10;
		
		$stop;
	end
	
endmodule