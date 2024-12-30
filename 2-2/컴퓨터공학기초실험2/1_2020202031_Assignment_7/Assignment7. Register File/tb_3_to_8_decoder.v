`timescale 1ns/100ps

module tb_3_to_8_decoder;
	reg [2:0] tb_d;
	wire [7:0] tb_q;
	
	_3_to_8_decoder U0(.d(tb_d), .q(tb_q)); // 3 to 8 decoder module
	
	initial begin
		tb_d = 3'b000; #10;
		tb_d = 3'b001; #10;
		tb_d = 3'b010; #10;
		tb_d = 3'b011; #10;
		tb_d = 3'b100; #10;
		tb_d = 3'b101; #10;
		tb_d = 3'b110; #10;
		tb_d = 3'b111; #10;
		$stop;
	end
endmodule