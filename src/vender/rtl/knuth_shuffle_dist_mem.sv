/*
 * This file is Fisher-Yates shuffle module.
*/

module knuth_shuffle_dist_mem #(
    parameter MAX_ELEMENTS = 128,
    parameter BITS_PER_ELEMENT = $clog2(MAX_ELEMENTS)
  )
  (
    input wire clk_in, rst_in,
    input wire [BITS_PER_ELEMENT-1:0] num_elements_in,       // contains NUM_ELEMENTS - 1 --> for 16, should be 15!
    input wire rst_num_elements,                             // when active, will take a new number of elements
    input wire perm_req_in,
    input wire [BITS_PER_ELEMENT-1:0] addr_in,
    input wire rst_index,
    input wire trng_ready,
    input wire [79:0] trng_seed,
    output wire init_done,
    output wire perm_done,
    output wire [BITS_PER_ELEMENT-1:0] num_elements_index_out,
//    output reg [MAX_ELEMENTS-1:0] [BITS_PER_ELEMENT-1:0] data_out
    output reg [BITS_PER_ELEMENT-1:0] data_out
  );
  
  reg req_prng;
  reg count;
  wire [BITS_PER_ELEMENT-1:0] seed_prng;
  
  // Interface for trivium PRNG
  trivium_top #( .WORDSIZE(BITS_PER_ELEMENT), .OUTPUT_BITS(BITS_PER_ELEMENT) ) 
  trivium_top_INST
    (.clk_i(clk_in),
     .rst_i(rst_in),
     .req_i(req_prng),
     .refr_i(0),
     .start(trng_ready),
     .key_i(trng_seed),
     .prng_o(seed_prng),
     .busy_o(),
     .ready_o(ready_prng)
     );

  // Interface for distributed dual-port memory (2 read ports, 1 write port)
  
  reg [BITS_PER_ELEMENT-1:0] addr_A;
  reg [BITS_PER_ELEMENT-1:0] addr_B;
  reg [BITS_PER_ELEMENT-1:0] din_A;
  reg wren;
  
  // 1 using A to permute, B to output
  // 0 using B to permute, A to output
  reg ce, ce_n;           
  
  reg [BITS_PER_ELEMENT-1:0] din_A_A;
  reg wren_A;
  reg [BITS_PER_ELEMENT-1:0] din_B_A;
  reg wren_B;
  
  reg [BITS_PER_ELEMENT-1:0] addr_A_A;
  reg [BITS_PER_ELEMENT-1:0] addr_A_B;
  reg [BITS_PER_ELEMENT-1:0] addr_B_A;
  reg [BITS_PER_ELEMENT-1:0] addr_B_B;

  wire [BITS_PER_ELEMENT-1:0] dout_A;
  wire [BITS_PER_ELEMENT-1:0] dout_B;
  
  wire [BITS_PER_ELEMENT-1:0] dout_A_A;
  wire [BITS_PER_ELEMENT-1:0] dout_A_B;
  
  wire [BITS_PER_ELEMENT-1:0] dout_B_A;
  wire [BITS_PER_ELEMENT-1:0] dout_B_B;
    
//  wire [BITS_PER_ELEMENT-1:0] dout_C_A;
//  wire [BITS_PER_ELEMENT-1:0] dout_C_B;
  
    dist_mem_gen_0 dist_mem_INST_A (
      .a(addr_A_A),       
      .d(din_A_A),      
      .dpra(addr_A_B),  
      .clk(clk_in),    
      .we(wren_A),      
      .spo(dout_A_A),  
      .dpo(dout_A_B)  
    );
  
    dist_mem_gen_0 dist_mem_INST_B (
      .a(addr_B_A),       
      .d(din_B_A),      
      .dpra(addr_B_B),  
      .clk(clk_in),    
      .we(wren_B),      
      .spo(dout_B_A),  
      .dpo(dout_B_B)  
    );
  
  
//  dist_mem_gen_1 dist_mem_INST_C (
//    .a(addr_B_A),       
//    .d(din_B_A),      
//    .dpra(addr_B_B),  
//    .clk(clk_in),    
//    .we(wren_B),      
//    .spo(dout_C_A),  
//    .dpo(dout_C_B)  
//  );  
  
  // FSM state definition
  localparam sRst         = 4'b0000;
  localparam sPrepInit    = 4'b0001;
  localparam sInit        = 4'b0010;
  localparam sInitDone    = 4'b0011;
  localparam sWaitPerm    = 4'b0100;
  localparam sReadVals    = 4'b0101;
  localparam sSwap1       = 4'b0110;
  localparam sSwap2       = 4'b0111;
  localparam sPermDone    = 4'b1011;
  localparam sUpdateIndex = 4'b1100;
  localparam sUpdateIndexi = 4'b1101;
  
  // FSM state register
  reg[3:0] rFSM_current, wFSM_next;
  
  // FSM counter
  reg[BITS_PER_ELEMENT-1:0] rCnt_current, wCnt_next;
  
  // Num Elements
  reg[BITS_PER_ELEMENT-1:0] rElem_current,rElem_current_A, rElem_current_B ;

  reg rRst_index,wRst_index;    
  // FSM State register
  //  - with synchronous reset
  always @(posedge clk_in)
  begin
    if (rst_in | rst_num_elements ) begin
      rFSM_current <= sRst;
      rCnt_current <= 0;
      rElem_current <= MAX_ELEMENTS - 1;
//      rElem_current_A <= MAX_ELEMENTS - 1;
//      rElem_current_B <= MAX_ELEMENTS - 1;
      ce = 1;
      rRst_index = 0;
//    end else if (perm_req_in) begin
//      ce = ~ce;
    end
    else if(rst_index) begin
//      if(ce && rElem_current_B == num_elements_in)
//        rFSM_current <= sInitDone;
//      else if(!ce && rElem_current_A == num_elements_in)
//        rFSM_current <= sInitDone;
//      else begin
        rFSM_current <= sUpdateIndexi;
//        if(ce)
//            rElem_current_A <= num_elements_in;
//        else 
//            rElem_current_B <= num_elements_in;
//        end
      rCnt_current <= 0;
      rElem_current <= num_elements_in;

      ce = ce_n;
      rRst_index = 1;
      end
    else begin
      rFSM_current <= wFSM_next;
      rCnt_current <= wCnt_next;
      rElem_current <= num_elements_in;
//      rElem_current_A <= num_elements_in;
//      rElem_current_B <= num_elements_in;
      ce = ce_n;
      rRst_index = wRst_index;
    end
  end
 
  // Next state logic
  //  - defines values of wFSM_next and wCnt_next
  //  - in function of inputs and rFSM_current
  always @(*)
  begin
    wRst_index = rRst_index;
    case (rFSM_current)
    
      sRst:    
        begin
          wFSM_next = sPrepInit;
          wCnt_next = 0;
          ce_n = ce;
        end
        
      sPrepInit:    
        begin
          wFSM_next = sInit;
          wCnt_next = rCnt_current + 1;
          ce_n = ce;
        end
                  
      sInit:    
        if (rCnt_current != rElem_current) begin    // fill in the identity permutation in memory
          wFSM_next = sInit;
          wCnt_next = rCnt_current + 1;
          ce_n = ce;
        end else if (ready_prng)  begin             // wait for PRNG to be ready
          wFSM_next = sInitDone;
          wCnt_next = rCnt_current;
          ce_n = ce;
        end else begin
          wFSM_next = sInit;
          wCnt_next = rCnt_current;
          ce_n = ce;
        end
      
      sInitDone:                 
        begin
          wFSM_next = sReadVals;
          wCnt_next = rElem_current;
          ce_n = ce;
          wRst_index = 0;
        end
      
      sWaitPerm:                                 // skip after reset
        if (perm_req_in) begin
          wFSM_next = sReadVals;
          wCnt_next = rElem_current;
          ce_n = ~ce;
        end else begin
          wFSM_next = sWaitPerm;
          wCnt_next = 0;
          ce_n = ce;
        end 
      
      sReadVals:
        begin
          wFSM_next = sSwap1;
          wCnt_next = rCnt_current;
          ce_n = ce;
        end
        
      sSwap1:
        begin
          wFSM_next = sSwap2;
          wCnt_next = rCnt_current -1;
          ce_n = ce;
        end
        
      sSwap2:
        begin
          if (rCnt_current == 0) begin
            wFSM_next = sPermDone;
            wCnt_next = 0;
          ce_n = ce;
          end else begin
            wFSM_next = sReadVals;
            wCnt_next = rCnt_current;
          ce_n = ce;
          end
        end
      
//      sPrepUpdate:
//        begin
//          wFSM_next = sUpdateOut;
//          wCnt_next = rCnt_current + 1;
//        end
      
//      sUpdateOut:
//        begin
//          if (rCnt_current == rElem_current) begin
//            wFSM_next = sLastUpdate;
//            wCnt_next = 0;
//          end else begin
//            wFSM_next = sUpdateOut;
//            wCnt_next = rCnt_current + 1;
//          end
//        end
      
//      sLastUpdate:
//        begin
//          wFSM_next = sPermDone;
//          wCnt_next = 0;
//        end

      sUpdateIndexi:
      begin
        wFSM_next <= sUpdateIndex;
        ce_n = ce;
        wCnt_next = 0;
      end
      sUpdateIndex:
        begin
            if(ce && rElem_current_B == num_elements_in)
                wFSM_next <= sInitDone;
            else if(!ce && rElem_current_A == num_elements_in)
                wFSM_next <= sInitDone;
            else begin           
                wFSM_next <= sPrepInit;
            end
        ce_n = ~ce;
        wCnt_next = 0;
        end
        
      sPermDone:
        begin
          wFSM_next = sWaitPerm;
          wCnt_next = 0;
          ce_n = ce;
        end
           
      default:
        begin
          wFSM_next = sRst;
          wCnt_next = 0;
          ce_n = ce;
        end
    endcase
  end   
  
  // Temporary register for swap
  reg [BITS_PER_ELEMENT-1:0] rtmp1;
  reg [BITS_PER_ELEMENT-1:0] rtmp2;
  always @(posedge clk_in)
  begin
    if (rst_in==1)begin
      rtmp1 <= 0;
      rtmp2 <= 0;
      end
    else if (rFSM_current == sReadVals)begin
      rtmp1 <= dout_A;
      rtmp2 <= dout_B;
      end
  end
  
  // Temporary register for seed & mod
  reg [BITS_PER_ELEMENT-1:0] seed_prng_reg;
  reg [BITS_PER_ELEMENT:0] mod_reg;
  
  always @(posedge clk_in)
  begin
    if (rst_in==1) begin
      seed_prng_reg <= 0;
      mod_reg <= 0;
    end else if (rFSM_current == sWaitPerm) begin
      mod_reg <= rElem_current + 1;
    end else if ( rFSM_current == sInitDone ) begin
      seed_prng_reg <= seed_prng;
      mod_reg <= rElem_current + 1;
    end else if ( rFSM_current == sReadVals ) begin
      seed_prng_reg <= seed_prng;
    end else if ( rFSM_current == sSwap2) begin
      seed_prng_reg <= seed_prng;
      mod_reg <= (mod_reg==1) ? rElem_current + 1 : mod_reg - 1;
    end
  end
  
  wire [BITS_PER_ELEMENT-1:0] seed_prng_mod;
  assign seed_prng_mod = (mod_reg!=0) ? seed_prng_reg % mod_reg : 0;
  
  // Output logic
  // - drive memory inputs & trivium inputs
assign dout_A = ce?dout_A_A:dout_B_A;
assign dout_B = ce?dout_A_B:dout_B_B;


  always @(*)
  begin
    addr_A = 0;
    addr_B = 0;
    din_A = 0;
    wren = 0;
    req_prng = 0;
    
      
    if (rFSM_current == sInit) begin
      addr_A = rCnt_current;
      din_A = rCnt_current;
      wren = 1;
    end else if (rFSM_current == sPrepInit) begin
      wren = 1;
    end else if (rFSM_current == sReadVals) begin
      addr_A = rCnt_current;
      addr_B = seed_prng_mod;
      req_prng = 1;
    end else if (rFSM_current == sSwap1) begin
      addr_A = rCnt_current;
      din_A = rtmp2;
      wren = 1;
    end else if (rFSM_current == sSwap2) begin
      addr_A = seed_prng_mod;
      din_A = rtmp1;
      wren = 1;
    end 
  end
  
 always @(*)
 begin
    addr_A_A = addr_A ;
    addr_A_B = addr_B ;
    din_A_A = din_A ;
    wren_A = wren && ce;
    
    addr_B_A = addr_A ;
    addr_B_B = addr_B ;
    din_B_A = din_A ;
    wren_B = wren && ~ce;
    
    //        if(ce)
//            rElem_current_A <= num_elements_in;
//        else 
//            rElem_current_B <= num_elements_in;
    if (rFSM_current != sInit && rFSM_current != sPrepInit)
        if(ce )
            addr_B_A = addr_in;
        else
            addr_A_A = addr_in;
    else if(!rRst_index)
        begin
        wren_A = wren;
        wren_B = wren;
        end else
        begin
            if(ce )
                addr_B_A = addr_in;
            else
                addr_A_A = addr_in;            
        
        end
 end
  

 always @(posedge clk_in)
begin
    if(rFSM_current == sPermDone)
        if(ce)
            rElem_current_A <= num_elements_in;
        else 
            rElem_current_B <= num_elements_in;
end

  always @(*)
  begin
    if(rst_in==1)
      data_out <= 0;
    else 
      data_out <= ce? dout_B_A:dout_A_A;

//      data_out <= addr_in;
  end
  

  
  // output done signals
  assign init_done = (rFSM_current == sInitDone) ? 1 : 0;
  assign perm_done = (rFSM_current == sPermDone) ? 1 : 0;
  assign num_elements_index_out = ce? rElem_current_B:rElem_current_A;
endmodule


