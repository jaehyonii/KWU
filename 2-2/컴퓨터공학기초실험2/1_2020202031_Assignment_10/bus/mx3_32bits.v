module mx3_32bits(d0, d1, s, y);
	input [31:0] d0, d1;
	input [1:0] s;
	output reg [31:0] y;

	always@(s, d0, d1) begin
		case(s)
			2'b10: y <= d0;
			2'b01: y <= d1;
			2'b00: y <= 32'h0;
			default: y <= 32'hx;
		endcase
	end
endmodule
