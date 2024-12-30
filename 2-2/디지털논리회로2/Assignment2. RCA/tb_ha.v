`timescale 1ns/100ps

module tb_ha;
	reg a;
	reg b;
	wire s;
	wire co;
	
	ha ha1(a, b, s, co);
	
initial
begin
	// a b _ s co
	#0		a = 0; b = 0; // 00_00
	#10	b = 1; //01_10
	#10	a = 1; b = 0; //10_10
	#10	b = 1; // 11_01
	#10	$finish;
end
endmodule
