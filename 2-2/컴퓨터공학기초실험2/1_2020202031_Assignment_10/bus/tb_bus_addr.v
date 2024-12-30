`timescale 1ns/100ps

module tb_bus_addr;
	reg [7:0]  tb_address;
	wire		  tb_s0_sel, tb_s1_sel;
	
	bus_addr U0(.address(tb_address), .s0_sel(tb_s0_sel), .s1_sel(tb_s1_sel));
	
	initial begin
		// slave0 memory map
		tb_address = 8'h00; #5;
		tb_address = 8'h08; #5;
		tb_address = 8'h10; #5;
		tb_address = 8'h18; #5;
		tb_address = 8'h1F; #5;
		
		// slave1 memory map
		tb_address = 8'h20; #5;
		tb_address = 8'h28; #5;
		tb_address = 8'h30; #5;
		tb_address = 8'h38; #5;
		tb_address = 8'h3F; #5;
		
		// not included in any memory maps
		tb_address = 8'h40; #5;
		tb_address = 8'h9F; #5;
		tb_address = 8'hFF; #5;
		
		$stop;
	end
	
endmodule
