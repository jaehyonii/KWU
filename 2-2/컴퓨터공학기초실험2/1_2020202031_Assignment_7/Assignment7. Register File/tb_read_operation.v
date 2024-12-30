`timescale 1ns/100ps

module tb_read_operation;
	reg [31:0] tb_from_reg0, tb_from_reg1, tb_from_reg2, tb_from_reg3, tb_from_reg4, tb_from_reg5, tb_from_reg6, tb_from_reg7;
	reg [2:0] tb_Addr;
	wire [31:0] tb_Data;
	
	read_operation U0(.Addr(tb_Addr), .Data(tb_Data), .from_reg0(tb_from_reg0), .from_reg1(tb_from_reg1), .from_reg2(tb_from_reg2), 
							.from_reg3(tb_from_reg3), .from_reg4(tb_from_reg4), .from_reg5(tb_from_reg5),
							.from_reg6(tb_from_reg6), .from_reg7(tb_from_reg7));
	
	initial begin
		tb_from_reg0 = 32'h12345678;
		tb_from_reg1 = 32'h13579bdf;
		tb_from_reg2 = 32'habcdef82;
		tb_from_reg3 = 32'h2385065d;
		tb_from_reg4 = 32'haaabbccd;
		tb_from_reg5 = 32'hffff222d;
		tb_from_reg6 = 32'h113239dc;
		tb_from_reg7 = 32'hccccffff;
		
		tb_Addr = 3'b000; #10;
		tb_Addr = 3'b001; #10;
		tb_Addr = 3'b010; #10;
		tb_Addr = 3'b011; #10;
		tb_Addr = 3'b100; #10;
		tb_Addr = 3'b101; #10;
		tb_Addr = 3'b110; #10;
		tb_Addr = 3'b111; #10;
		$stop;
	end
	
endmodule