`timescale 1ns/100ps

module tb_os_logic;
	reg [2:0] tb_state;
	reg [7:0] tb_d_in;
	wire [7:0] tb_d_out;
	
	os_logic U0(.state(tb_state), .d_in(tb_d_in), .d_out(tb_d_out));
	
	parameter IDLE_STATE = 3'b000;
	parameter LOAD_STATE = 3'b001;
	parameter INC_STATE = 3'b010;
	parameter INC2_STATE = 3'b011;
	parameter DEC_STATE = 3'b100;
	parameter DEC2_STATE = 3'b101;
	
	initial begin
		tb_d_in = 8'hfa;
		tb_state = IDLE_STATE; #10; // check action of IDLE_STATE
		tb_state = LOAD_STATE; #10; // check action of LOAD_STATE
		tb_state = INC_STATE; #10; // check action of INC_STATE
		tb_state = INC2_STATE; #10; // check action of INC2_STATE
		tb_state = DEC_STATE; #10; // check action of DEC_STATE
		tb_state = DEC2_STATE; #10; // check action of DEC2_STATE
		
		$stop;
	end
	
endmodule