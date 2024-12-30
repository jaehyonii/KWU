`timescale 1ns/100ps

module tb_ram;
	reg tb_clk;
	reg tb_cen, tb_wen;
	reg [4:0] tb_addr;
	reg [31:0] tb_din;
	wire [31:0] tb_dout;
	
	ram U0_ram(.clk(tb_clk), .cen(tb_cen), .wen(tb_wen), .addr(tb_addr), .din(tb_din), .dout(tb_dout));
	
	always begin
		#5; tb_clk = ~tb_clk;
	end
	
	integer i;
	
	initial begin
		tb_clk = 0; #3;
		tb_cen = 0; #10;
		
		// check memory initialization
		tb_cen = 1; tb_wen = 0;
		for(i = 0; i<32; i=i+1)begin
			tb_addr = i; #10;
		end
		
		// check memory write
		tb_cen = 1; tb_wen = 1;
		for(i = 0; i<32; i=i+1)begin
			tb_addr = i; tb_din = i + 3; #10;
		end
		
		// check memory read
		tb_cen = 1; tb_wen = 0;
		for(i = 0; i<32; i=i+1)begin
			tb_addr = i; #10;
		end
		
		// check situation when cen = 0
		tb_cen = 0; tb_wen = 0;
		for(i = 0; i<16; i=i+1)begin
			tb_addr = i; #10;
		end
		tb_cen = 0; tb_wen = 1;
		for(i = 16; i<32; i=i+1)begin
			tb_addr = i; #10;
		end
		
		$stop;
	end
	
endmodule
