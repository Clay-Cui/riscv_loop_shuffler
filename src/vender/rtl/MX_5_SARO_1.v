`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 02/14/2024 01:31:37 PM
// Design Name: 
// Module Name: MX_4_SARO_1
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


module MX_5_SARO_1
    #(parameter BUFFER_SIZE = 256)
(
    input clk,
    input ena,
    input reset,
    input req,
    output o,
    output rng_ready_o,
//    output [BUFFER_SIZE-1:0] buffer_out0,
//    output [BUFFER_SIZE-1:0] buffer_out1,
//    output [BUFFER_SIZE-1:0] buffer_out2,
//    output [BUFFER_SIZE-1:0] buffer_out3,
//    output [BUFFER_SIZE-1:0] buffer_out4,
    output [BUFFER_SIZE-1:0] buffer_out
    
    );
    
    wire [4:0] ro_out;
    reg [4:0] ro_out_r;
    reg [20:0] count;
    reg sample_clk;
    
    reg rng_ready;
    
//    reg [BUFFER_SIZE-1:0] buffer0;
//    reg [BUFFER_SIZE-1:0] buffer1;
//    reg [BUFFER_SIZE-1:0] buffer2;
//    reg [BUFFER_SIZE-1:0] buffer3;
//    reg [BUFFER_SIZE-1:0] buffer4;
    reg [BUFFER_SIZE-1:0] buffer;
    reg [10:0] counter;
    SARO SARO_0(
        .ena(ena),
        .o(ro_out[0])
    );
    SARO SARO_1(
        .ena(ena),
        .o(ro_out[1])
    );
    SARO SARO_2(
        .ena(ena),
        .o(ro_out[2])
    );
    SARO SARO_3(
        .ena(ena),
        .o(ro_out[3])
    );
    SARO SARO_4(
        .ena(ena),
        .o(ro_out[4])
    );    
    
//    reg xor_out;
//    always @(posedge clk)
//    begin
//        xor_out <= ro_out[0] ^ ro_out[1] ^ ro_out[2] ^ ro_out[3];
//    end

    always @(posedge clk)
    begin
        if(reset) begin
            
            count = 0;
            sample_clk<=0;
        end else if (count < 1000) begin
            count <= count+1;
            sample_clk<=0;
        end else begin
            count <= 0;
            sample_clk <= 1;
            end
    end
    
    always @(posedge clk)
    begin
        if(reset) begin
            ro_out_r <= 0;
        end else if (sample_clk) begin
            ro_out_r[0] <= ro_out[0];
            ro_out_r[1] <= ro_out[1];
            ro_out_r[2] <= ro_out[2];
            ro_out_r[3] <= ro_out[3];
        end        
    end
    
    assign xor_out = ro_out_r[0] ^ ro_out_r[1] ^ ro_out_r[2] ^ ro_out_r[3]^ ro_out_r[4];
    
    always @(posedge clk)
    begin
        if(reset | req) begin
            buffer <= 0;
//            buffer0 <= 0;
//            buffer1 <= 0;
//            buffer2 <= 0;
//            buffer3 <= 0;
            counter <= 0;
            rng_ready <= 0;
            end
        else if( sample_clk) begin
        if(counter <BUFFER_SIZE) begin
            buffer = {buffer[BUFFER_SIZE-2:0], xor_out};
//            buffer0 = {buffer0[BUFFER_SIZE-2:0], ro_out_r[0]};
//            buffer1 = {buffer1[BUFFER_SIZE-2:0], ro_out_r[1]};
//            buffer2 = {buffer2[BUFFER_SIZE-2:0], ro_out_r[2]};
//            buffer3 = {buffer3[BUFFER_SIZE-2:0], ro_out_r[3]};
//            buffer4 = {buffer4[BUFFER_SIZE-2:0], ro_out_r[0] ^ ro_out_r[1] ^ ro_out_r[2] ^ ro_out_r[3]};
            counter <= counter + 1;
            end else begin
                rng_ready <= 1;
            end
            end
    end
    
    
    assign o =  xor_out ;
    assign buffer_out = buffer;
//    assign buffer_out0 = buffer0;
//    assign buffer_out1 = buffer1;
//    assign buffer_out2 = buffer2;
//    assign buffer_out3 = buffer3;
//    assign buffer_out4 = buffer4;
    assign rng_ready_o = rng_ready;
    
endmodule
