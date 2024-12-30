`timescale 1ns/1ps
`include "RippleCarryAdder.v"
`include "FullAdder.v"
module tb_RippleCarryAdder;
   reg [3:0] a;
   reg [3:0] b; 
   reg c_in;
   wire [3:0] sum;
   wire c_out;

   
   RippleCarryAdder uut(.a(a), .b(b), .c_in(c_in), .sum(sum), .c_out(c_out));
   
   initial
   begin
      $dumpfile("tb_RippleCarryAdder.vcd");
      $dumpvars(0,tb_RippleCarryAdder); 

       a = 0;
       b = 0;
       c_in = 1;
       #10;

       a = 1;
       b = 1;
       c_in = 0;
       #10;

       a = 1;
       b = 0;
       c_in = 0;
       #10;

       a = 8;
       b = 0;
       c_in = 1;
       #10;

       a = 15;
       b = 0;
       c_in = 0;
       #10;

       a = 15;
       b = 1;
       c_in = 0;
       #10;

       a = 15;
       b = 2;
       c_in = 1;
       #10;

       $display("Test complete");
   end

endmodule