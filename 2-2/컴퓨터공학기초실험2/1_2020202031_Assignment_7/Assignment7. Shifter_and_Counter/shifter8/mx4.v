module mx4(d0, d1, d2, d3, s, y);
	input d0, d1, d2, d3;
	input [1:0] s;
	output reg y;
	
	always@(s, d0, d1, d2, d3) begin
		case(s) // y value is decided by s
			2'b00: y = d0;
			2'b01: y = d1;
			2'b10: y = d2;
			2'b11: y = d3;
			default : y = 1'bx;
		endcase
	end
	
endmodule