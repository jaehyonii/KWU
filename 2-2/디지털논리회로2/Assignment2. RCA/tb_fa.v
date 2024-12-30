`timescale 1ns/100ps

module tb_fa;
	reg a, b, ci;
	wire s, co;
	
	fa fa1(a, b, ci, s, co);
	
initial
begin
	// a b ci _ s co
	#0		a = 0; b = 0; ci = 0; // 000_00
	#10	ci = 1; // 001_10
	#10	b = 1; ci = 0; // 010_10
	#10	ci = 1; // 011_01
	#10	a = 1; b = 0; ci = 0; // 100_10
	#10	ci = 1; // 101_01
	#10	b = 1; ci = 0; // 110_01
	#10	ci = 1; // 111_11
	#10	$finish;
end
endmodule
