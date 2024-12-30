`timescale 1ns/100ps

module tb_Top;
   reg tb_clk, tb_reset_n, tb_m_req, tb_m_wr;
   reg [15:0] tb_m_addr;
   reg [63:0] tb_m_dout;
   wire tb_m_grant, tb_interrupt;
   wire [63:0] tb_m_din;
   
   Top U0_Top(.clk(tb_clk), .reset_n(tb_reset_n), .m_req(tb_m_req), .m_wr(tb_m_wr), .m_addr(tb_m_addr), .m_dout(tb_m_dout),
					.m_grant(tb_m_grant), .m_din(tb_m_din), .interrupt(tb_interrupt));
      
   always begin
      #5; tb_clk = ~tb_clk;
   end
	
   initial
   begin
		// write mode
      tb_clk = 1'b0; tb_reset_n = 1'b0;
		tb_m_req = 1'b0; tb_m_wr = 1'b1; #3;
		
		tb_reset_n = 1'b1;
		
		// master granted
      tb_m_req = 1; #10;
      tb_m_addr = 16'h0000; tb_m_dout = 64'h1111111111111111; #10;
      tb_m_addr = 16'h1111; tb_m_dout = 64'h2222222222222222; #10;
      tb_m_addr = 16'h0234; tb_m_dout = 64'h1234567812345678; #10;
      tb_m_addr = 16'h07FF; tb_m_dout = 64'hFFFFFFFFFFFFFFFF; #10;
      tb_m_addr = 16'h0700; tb_m_dout = 64'hAAAAAAAAAAAAAAAA; #10;
		
		// read mode
		tb_m_wr = 1'b0;
      tb_m_addr = 16'h0000; #100;
      tb_m_addr = 16'h1111; #100;
      tb_m_addr = 16'h0234; #100;
      tb_m_addr = 16'h07FF; #100;
      tb_m_addr = 16'h0700; #100;
		
		// write mode
      tb_m_wr = 1'b1; #10;      
      tb_m_addr = 16'h7020; tb_m_dout = 64'd10; #10; // operand 10
      tb_m_addr = 16'h7018; tb_m_dout = 64'd1; #10; // intrEn 1
      tb_m_addr = 16'h7000; tb_m_dout = 64'd1; #3500; // opstart 1
		
		// read mode
		tb_m_wr = 0;
      tb_m_addr = 16'h7030; #100; // result_l
      tb_m_addr = 16'h7028; #100; // result_h
      
		// write mode
		tb_m_wr = 1;
		tb_m_addr = 16'h7008; tb_m_dout = 64'd1; #10; // opclear 1
      tb_m_addr = 16'h7008; tb_m_dout = 64'd0; #10; // opclear 0
		
      tb_m_addr = 16'h7020; tb_m_dout = 64'd25; #10; // operand 25
      tb_m_addr = 16'h7018; tb_m_dout = 64'd1; #10; // intrEn 1
      tb_m_addr = 16'h7000; tb_m_dout = 64'd1; #8500;// opstart 1
		
		// read mode
		tb_m_wr = 0;
      tb_m_addr = 16'h7030; #100;// result_l
      tb_m_addr = 16'h7028; #100;// result_h
      
		// write mode
		tb_m_wr = 1;
      tb_m_addr = 16'h7008; tb_m_dout = 64'd1; #10; //opclear 1
      tb_m_addr = 16'h7008; tb_m_dout = 64'd0; #10; //opclear 0
		
      tb_m_addr = 16'h7020; tb_m_dout = 64'd0; #10; //operand 0
      tb_m_addr = 16'h7018; tb_m_dout = 64'd1; #10; //intrEn 1
      tb_m_addr = 16'h7000; tb_m_dout = 64'd1; #100;// opstart 1
		
		// read mode
		tb_m_wr = 0;
      tb_m_addr = 16'h7030; #100; // result_l
      tb_m_addr = 16'h7028; #100; // result_h
      
		// write mode
		tb_m_wr = 1;
      tb_m_addr = 16'h7008; tb_m_dout = 64'd1; #10; // opclear 1
      tb_m_addr = 16'h7008; tb_m_dout = 64'd0; #10; // opclear 0
      
      tb_m_addr = 16'h7020; tb_m_dout = 64'd1; #10; // operand 1
      tb_m_addr = 16'h7018; tb_m_dout = 64'd1; #10; // intrEn 1
      tb_m_addr = 16'h7000; tb_m_dout = 64'd1; #100; // opstart 1
		
		// read mode
		tb_m_wr = 0;
      tb_m_addr = 16'h7030; #100; // result_l
      tb_m_addr = 16'h7028; #100; // result_h
      
		// write mode
		tb_m_wr = 1;
      tb_m_addr = 16'h7008; tb_m_dout = 64'd1; #10; //opclear 1
      tb_m_addr = 16'h7008; tb_m_dout = 64'd0; #10; //opclear 0
      tb_m_req = 1'b0;
      $stop;
   end
endmodule