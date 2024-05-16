`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 02/14/2024 01:22:19 PM
// Design Name: 
// Module Name: SARO
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module SARO(
    input ena,
    output o
    );
    
    wire[1:0] del;
    
//    assign del[0] = del[1] & ena;
    
//    assign del[1] = ~ del[0];

    LUT2 #(.INIT(4'b1000))
    lut2_i
    (   
        .O(del[0]),
        .I0(del[1]),
        .I1(ena)
    );
    
    LUT1 #(.INIT(2'b01))
    lut1_0
    (
        .O(del[1]),
        .I0(del[0])
    );
//    LUT1 #(.INIT(2'b01))
//    lut1_1
//    (
//        .O(del[2]),
//        .I0(del[1])
//    );
//    LUT1 #(.INIT(2'b01))
//    lut1_2
//    (
//        .O(del[3]),
//        .I0(del[2])
//    );
    
    assign o = del[1];  
    
endmodule
