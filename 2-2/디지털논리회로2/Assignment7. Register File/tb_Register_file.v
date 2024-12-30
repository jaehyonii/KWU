`timescale 1ns/100ps

module tb_Register_file;
	reg tb_clk, tb_reset_n, tb_we;
	reg [2:0] tb_wAddr, tb_rAddr;
	reg [31:0] tb_wData;
	wire [31:0] tb_rData;
	
	// instance of Register_file
	Register_file U0_RF(.clk(tb_clk), .reset_n(tb_reset_n), .wAddr(tb_wAddr), .wData(tb_wData), .we(tb_we), 
							.rAddr(tb_rAddr), .rData(tb_rData));
	
	always begin
		#5; tb_clk = ~tb_clk; // clock period is 10ns
	end
	
	initial begin
		tb_clk = 0; tb_reset_n = 0; tb_we = 0; #2;
		tb_reset_n = 1;
		
		tb_we = 1; tb_wAddr = 3'b000; tb_rAddr = 3'b000; tb_wData = 32'habcd1234; #10; // update register0 value
		tb_we = 0;
		// check registers' value
		tb_rAddr = 3'b000; tb_wAddr = 3'b000; #10;
		tb_rAddr = 3'b001; tb_wAddr = 3'b001; #10;
		tb_rAddr = 3'b010; tb_wAddr = 3'b010; #10;
		tb_rAddr = 3'b100; tb_wAddr = 3'b100; #10;
		tb_rAddr = 3'b110; tb_wAddr = 3'b110; #10;
		
		tb_we = 1; tb_wAddr = 3'b001; tb_rAddr = 3'b001; tb_wData = 32'h1234cdef; #10; // update register1 value
		tb_we = 0;
		// check registers' value
		tb_rAddr = 3'b000; tb_wAddr = 3'b000; #10;
		tb_rAddr = 3'b001; tb_wAddr = 3'b001; #10;
		tb_rAddr = 3'b011; tb_wAddr = 3'b011; #10;
		tb_rAddr = 3'b101; tb_wAddr = 3'b101; #10;
		tb_rAddr = 3'b111; tb_wAddr = 3'b111; #10;
		
		tb_we = 1; tb_wAddr = 3'b011; tb_rAddr = 3'b011; tb_wData = 32'hf9876543; #10; // update register3 value
		tb_we = 0;
		// check registers' value
		tb_rAddr = 3'b000; tb_wAddr = 3'b000; #10;
		tb_rAddr = 3'b001; tb_wAddr = 3'b001; #10;
		tb_rAddr = 3'b010; tb_wAddr = 3'b010; #10;
		tb_rAddr = 3'b011; tb_wAddr = 3'b011; #10;
		tb_rAddr = 3'b101; tb_wAddr = 3'b101; #10;
		tb_rAddr = 3'b111; tb_wAddr = 3'b111; #10;
		
		tb_we = 1; tb_wAddr = 3'b111; tb_rAddr = 3'b111; tb_wData = 32'hffffaaaa; #10; // update register7 value
		tb_we = 0;
		// check registers' value
		tb_rAddr = 3'b000; tb_wAddr = 3'b000; #10;
		tb_rAddr = 3'b001; tb_wAddr = 3'b001; #10;
		tb_rAddr = 3'b010; tb_wAddr = 3'b010; #10;
		tb_rAddr = 3'b011; tb_wAddr = 3'b011; #10;
		tb_rAddr = 3'b100; tb_wAddr = 3'b100; #10;
		tb_rAddr = 3'b101; tb_wAddr = 3'b101; #10;
		tb_rAddr = 3'b110; tb_wAddr = 3'b110; #10;
		tb_rAddr = 3'b111; tb_wAddr = 3'b111; #10;
		
		$stop;
	end
endmodule