`timescale 1ns/100ps

module tb_cal_radix4;
	reg		  [2:0]   tb_last_3bits;
	reg		  [63:0]  tb_multiplier;
	reg		  [127:0] tb_result_in;
	wire		  [127:0] tb_result_out;

	cal_radix4 U0(.multiplier(tb_multiplier), .last_3bits(tb_last_3bits), .result_in(tb_result_in), .result_out(tb_result_out));

	initial begin
		tb_multiplier = 64'h00000000ffffffff; tb_result_in = 128'h0;
		
		// shift shift
		tb_last_3bits=3'b000; #5;
		tb_last_3bits=3'b111; #5;
		
		// add shift shift
		tb_last_3bits=3'b001; #5;
		tb_last_3bits=3'b010; #5;
		
		// shift add shift
		tb_last_3bits=3'b011; #5;
		
		// shift sub shift
		tb_last_3bits=3'b100; #5;
		
		// sub shift shift
		tb_last_3bits=3'b101; #5;
		tb_last_3bits=3'b110; #5;
		
		
		
		tb_multiplier = 64'hffffffffff000000; tb_result_in = 128'hffffffffffffffffffff000000000000;
		
		// shift shift
		tb_last_3bits=3'b000; #5;
		tb_last_3bits=3'b111; #5;
		
		// add shift shift
		tb_last_3bits=3'b001; #5;
		tb_last_3bits=3'b010; #5;
		
		// shift add shift
		tb_last_3bits=3'b011; #5;
		
		// shift sub shift
		tb_last_3bits=3'b100; #5;
		
		// sub shift shift
		tb_last_3bits=3'b101; #5;
		tb_last_3bits=3'b110; #5;
		
		$stop;
	end
endmodule
