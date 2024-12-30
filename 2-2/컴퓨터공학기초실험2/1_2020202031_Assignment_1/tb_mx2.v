`timescale 1ns/100ps

module test_mx2;

	reg d0, d1, s;
	wire y;
	
mx2 mx_top (.y(y), .d0(d0), .d1(d1), .s(s));

initial
begin
	#0		d0 = 0; d1 = 0; s = 0; // 000_0
	#10 	s = 1; // 001_0
	#10	d0 = 0; d1 = 1; s= 0; // 010_0
	#10	s = 1; // 011_1
	#10	d0 = 1; d1 = 0; s= 0; // 100_1
	#10	s = 1; // 101_0
	#10	d0 = 1; d1 = 1; s= 0; // 110_1
	#10	s = 1; // 111_1
	#10	$finish;
end

endmodule
