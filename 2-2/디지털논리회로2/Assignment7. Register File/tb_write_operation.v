`timescale 1ns/100ps

module tb_write_operation;
	reg [2:0] tb_Addr;
	reg tb_we;
	wire [7:0] tb_wEn;
	
	write_operation U0(.Addr(tb_Addr), .we(tb_we), .wEn(tb_wEn));
	
	initial begin
		// when we == 0
		tb_we = 0;
		tb_Addr = 3'b000; #2; // to_reg == 8'b00000000
		tb_Addr = 3'b001; #2; // to_reg == 8'b00000000
		tb_Addr = 3'b010; #2; // to_reg == 8'b00000000
		tb_Addr = 3'b011; #2; // to_reg == 8'b00000000
		tb_Addr = 3'b100; #2; // to_reg == 8'b00000000
		tb_Addr = 3'b101; #2; // to_reg == 8'b00000000
		tb_Addr = 3'b110; #2; // to_reg == 8'b00000000
		tb_Addr = 3'b111; #2; // to_reg == 8'b00000000
		
		// when we == 1
		tb_we = 1;
		tb_Addr = 3'b000; #2; // to_reg == 8'b00000001
		tb_Addr = 3'b001; #2; // to_reg == 8'b00000010
		tb_Addr = 3'b010; #2; // to_reg == 8'b00000100
		tb_Addr = 3'b011; #2; // to_reg == 8'b00001000
		tb_Addr = 3'b100; #2; // to_reg == 8'b00010000
		tb_Addr = 3'b101; #2; // to_reg == 8'b00100000
		tb_Addr = 3'b110; #2; // to_reg == 8'b01000000
		tb_Addr = 3'b111; #2; // to_reg == 8'b10000000
		$stop;
	end
	
endmodule