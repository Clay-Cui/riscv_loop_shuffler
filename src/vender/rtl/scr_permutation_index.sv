////////////////////////////////////////////////////////////////////////////////
// Engineer:                                                                  //
//                                                                            //
// Design Name:    Permuation registers                                       //
// Project Name:                                                              //
// Language:                                                                  //
//                                                                            //
// Description:                                                               //
//                                                                            //
//                                                                            //
//                                                                            //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

module scr_permutation_index #(
    parameter BITS_PER_ELEMENT = 7
)
(
    input   logic                                   clk,
    input   logic                                   rst,
    input   logic           [31:0]                  current_pc_i,
    input   logic           [63:0]                  hwlp_end_addr_i,
    input   logic           [63:0]                  se_counter_i,
    input   logic           [BITS_PER_ELEMENT:0]    num_elements_in,
    output  logic           [BITS_PER_ELEMENT-1:0]    index_o 
);

    logic reached_last_pc;
    assign reached_last_pc = hwlp_end_addr_i[31:0]==current_pc_i;

    logic [7:0] index1, index2;

    
    
    assign index1 = se_counter_i[7:0]-1;
    always_ff @(posedge clk)
    begin
        if (rst == 0)
            index2 = 0;
        else if(reached_last_pc && current_pc_i!=0 && index1 !=0)
            index2 = index1-1;
        else if((se_counter_i[7:0] == num_elements_in ) && index2==0)
            index2 = index1;
    end
    // always_comb
    // begin
    //     if(hwlp_end_addr_i[31:0]==current_pc_i) 
    //         index[0] = index[0]-1;
    //     else
    //         index[0] = index;
    // end
    // always_ff @(posedge clk)
    // begin
    //     permutation_index_q[1] = permutation_index_d[1];
    // end
    // always_comb begin
    //     if(clear_instr_valid&hwlp_reached_last_pc[1]) begin
    //         permutation_index_d[1] = index[1]-1;
    //     end else if (permutation_index_d[1] >= index[1] || (permutation_index_d[1] == 0 && index[1]>1)) begin
    //         permutation_index_d[1] = index[1];
    //     end else begin
    //         permutation_index_d[1] = permutation_index_q[1];
    //     end
    // end
    
    // assign per_o = {28'b0,permutation[permutation_index_q]};
    assign index_o = index2;
endmodule