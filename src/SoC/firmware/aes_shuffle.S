.extern pt_aes
.extern key_aes
.extern inter
.extern cycles
.extern index_random
.extern order



.section .text

.global aes_shuffle
.type   aes_shuffle,@function
aes_shuffle:
    addi sp, sp, -112
    sw x1, 0(sp)
    sw s0, 4(sp)        //By convention, s0 is stored by callee.
    sw t0, 8(sp)
    sw t1, 12(sp)
    sw t2, 16(sp)
    sw t3, 20(sp)
    sw t4, 24(sp)
    sw t5, 28(sp)
    sw t6, 32(sp)
    sw a0, 36(sp)
    sw a1, 40(sp)
    sw a2, 44(sp)
    sw a3, 48(sp)
    sw a4, 52(sp)
    sw a5, 56(sp)
    sw a6, 60(sp)
    sw a7, 64(sp)
    sw s1, 68(sp)
    sw s2, 72(sp)
    sw s3, 76(sp)
    sw s4, 80(sp)
    sw s5, 84(sp)
    sw s6, 88(sp)
    sw s7, 92(sp)
    sw s8, 96(sp)
    sw s9, 100(sp)
    sw s10, 104(sp)
    sw s11, 108(sp)
    //la t3, time
    //csrrs t4, 0xb05, x0
    //sw t4, 0(t3)

    add x1, x0, x0
    add s0, x0, x0        
    addi t2, x0, 0
    addi t3, x0, 0
    addi t4, x0, 0
    addi t5, x0, 0
    addi t6, x0, 0
    addi a0, x0, 0
    addi a1, x0, 0
    addi a2, x0, 0
    addi a3, x0, 0
    addi a4, x0, 0
    addi a5, x0, 0
    addi a6, x0, 0
    addi a7, x0, 0
    addi s1, x0, 0
    addi s2, x0, 0
    addi s3, x0, 0
    addi s4, x0, 0
    addi s5, x0, 0
    addi s6, x0, 0
    addi s7, x0, 0
    addi s8, x0, 0
    addi s9, x0, 0
    addi s10,x0, 0
    addi s11,x0, 0


    li t3, 0x82001000
    sw x0, 0(t3)

    # la s10, index_random
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    


    addi t5, x0, 1
    # li t3, 0x82000800
    # sw t5, 0(t3)
    li t3, 0x82001000
    sw t5, 0(t3)

    # First round 
    # csrw 0x7e0, 3
    # csrw 0x7e1, 1
    # la s11, cycles
    # csrr t4, 0x780
    # sw t4, 0(s11)

    call Wrapper_KeyAdditionSboxShiftRows_first

    # li t3, 0x82000800
    # sw x0, 0(t3)
    li t3, 0x82001000
    sw x0, 0(t3)


    call Mixcolumns_Shuffle_table

    addi t1,zero, 0x01
    call Wrapper_NextKey




    # # Second round
    call Wrapper_KeyAdditionSboxShiftRows
    call Mixcolumns_Shuffle_table
    addi t1, zero,0x02
    call Wrapper_NextKey
    
    # Third round
    call Wrapper_KeyAdditionSboxShiftRows
    call Mixcolumns_Shuffle_table
    addi t1, zero,0x04
    call Wrapper_NextKey
    
    
    # Fourth round
    call Wrapper_KeyAdditionSboxShiftRows
    call Mixcolumns_Shuffle_table
    addi t1, zero,0x08
    call Wrapper_NextKey
    
    # Fifth round
    call Wrapper_KeyAdditionSboxShiftRows
    call Mixcolumns_Shuffle_table
    addi t1, zero,0x10
    call Wrapper_NextKey
    
    # Sixth round
    call Wrapper_KeyAdditionSboxShiftRows
    call Mixcolumns_Shuffle_table
    addi t1, zero,0x20
    call Wrapper_NextKey
    
    # Seventh round
    call Wrapper_KeyAdditionSboxShiftRows
    call Mixcolumns_Shuffle_table
    addi t1, zero,0x40
    call Wrapper_NextKey
    
    # Eighth round
    call Wrapper_KeyAdditionSboxShiftRows
    call Mixcolumns_Shuffle_table
    addi t1, zero,0x80
    call Wrapper_NextKey
    
    # Ninth round
    call Wrapper_KeyAdditionSboxShiftRows
    call Mixcolumns_Shuffle_table
    addi t1, zero,0x1b
    call Wrapper_NextKey
    
    # Last round
    call Wrapper_KeyAdditionSboxShiftRows
    addi t1, zero,0x36
    call Wrapper_NextKey
    call Wrapper_KeyAddition_last

    # csrr t4, 0x780
    # sw t4, 4(s11)
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    


    
    lw x1, 0(sp)
    lw s0, 4(sp)
    lw t0, 8(sp)
    lw t1, 12(sp)
    lw t2, 16(sp)
    lw t3, 20(sp)
    lw t4, 24(sp)
    lw t5, 28(sp)
    lw t6, 32(sp)
    lw a0, 36(sp)
    lw a1, 40(sp)
    lw a2, 44(sp)
    lw a3, 48(sp)
    lw a4, 52(sp)
    lw a5, 56(sp)
    lw a6, 60(sp)
    lw a7, 64(sp)
    lw s1, 68(sp)
    lw s2, 72(sp)
    lw s3, 76(sp)
    lw s4, 80(sp)
    lw s5, 84(sp)
    lw s6, 88(sp)
    lw s7, 92(sp)
    lw s8, 96(sp)
    lw s9, 100(sp)
    lw s10, 104(sp)
    lw s11, 108(sp)
    addi sp, sp, 112
    ret

    //AES high-level subroutines

Wrapper_KeyAdditionSboxShiftRows_first:



    la a0, pt_aes
    la a1, key_aes
    la a2, sbox
    la a3, shiftRowTable
    la a4, inter
    setindex 16

    lp.counti x0, 16
    lp.starti x0, startLoopKeyAdditionSboxShiftRows_first
    lp.endi   x0, endKeyAdditionSboxShiftRows_first

    startLoopKeyAdditionSboxShiftRows_first: 
            lwp t3, 0(a0)
            lwp t4, 0(a1)
            xor t3, t3, t4
            add t5, t3, a2
            lbu t4, 0(t5)
            lwp t5, 0(a3)
            add a5, a4, t5
    endKeyAdditionSboxShiftRows_first:   sb t4, 0(a5)
ret


Wrapper_KeyAdditionSboxShiftRows:

    # Set index
    # lw t1, 4(s10)
    # lw t0, 0(s10)
    # li t1, 0x44

    # addi s10, s10, 8
    setindex 16

    la a0, pt_aes
    la a1, key_aes
    la a2, sbox
    la a3, shiftRowTable
    la a4, inter
    # la a6, order

    lp.counti x0, 16
    lp.starti x0, startLoopKeyAdditionSboxShiftRows
    lp.endi   x0, endKeyAdditionSboxShiftRows

    startLoopKeyAdditionSboxShiftRows: 
            # addtoba t0, x0, x0
            # sb t0, 0(a6)
            # addi a6, a6, 1
            lwp t3, 0(a0)
            lwp t4, 0(a1)
            xor t3, t3, t4
            add t5, t3, a2
            lbu t4, 0(t5)
            lwp t5, 0(a3)
            add a5, a4, t5
    endKeyAdditionSboxShiftRows:   sb t4, 0(a5)
            # sb t3, 0(a4)
    # endKeyAdditionSboxShiftRows:   addi a4, a4, 1
ret



Wrapper_KeyAddition_last:
    # lw t1, 4(s10)
    # lw t0, 0(s10)
    
    # addi s10, s10, 8
    setindex 16

    la  a0, pt_aes
    la  a2, inter
    la  a1, key_aes

    lp.counti x0, 16
    lp.starti x0, startLoopKeyAddition_last
    lp.endi   x0, endKeyAddition_last

    startLoopKeyAddition_last: 
            lwp    t3, 0(a1)
            lwp    t4, 0(a2)
            xor t3, t3, t4
    endKeyAddition_last:  
            swp t3, 0(a0) 
ret







Mixcolumns_Shuffle_table:
    # lw t1, 4(s10)
    # lw t0, 0(s10)
    
    # addi s10, s10, 8
    setindex 16

    la a0, inter
    la a1, xtime
    la a2, pt_aes
    
    la a3, order

    la a4, map1
    la a5, map2
    la a6, map3
    # sw x0, 0(a3)                   // Set it to zero
        
    lp.counti x0, 16
    lp.starti x0, startMixColumnsTable
    lp.endi   x0, endMixColumnsTable

    startMixColumnsTable:
            # addtoba t0, x0, x0
            # sb t0, 16(a3)
            # addi a3, a3, 1    
        lwp t1, 0(a0)
        lwp t3, 0(a4)
        add t4, a0, t3          # address of inter[i+1]
        lbu t5, 0(t4)           # value of inter[map1[i]]
        xor t1, t1, t5

        add t6, t1, a1
        lbu t1, 0(t6)

        xor t1, t1, t5

        lwp t5, 0(a5)
        add t6, a0, t5
        lbu t5, 0(t6)

        xor t1, t1, t5

        lwp t5, 0(a6)
        add t6, a0, t5
        lbu t5, 0(t6)
        
        xor t1, t1, t5

    endMixColumnsTable: swp t1, 0(a2)
        # sb t1, 0(t0)

ret









Wrapper_NextKey:
    la a1, key_aes
    la a2, sbox
    // First column
    lw s2, 0x0c*4(a1)

    addi sp, sp, -4
    sw x1, 0(sp)
    add s2, s2, a2
    lbu s3, 0(s2)

    lw s8, 0x03*4(a1)
    xor s3, s3, s8
    sb s3, 0x03*4(a1)

    lw s2, 0x0d*4(a1)
    add s2, s2, a2
    lbu s3, 0(s2)
    lw s8, 0*4(a1)
    xor s3, s3, s8
    xor s3, s3, t1
    sb s3, 0*4(a1)

    lw s2, 0x0e*4(a1)
    add s2, s2, a2
    lbu s3, 0(s2)
    lw s8, 0x01*4(a1)
    xor s3, s3, s8
    sb s3, 0x01*4(a1)

    lw s2, 0x0f*4(a1)
    add s2, s2, a2
    lbu s3, 0(s2)
    lw s8, 0x02*4(a1)
    xor s3, s3, s8
    sb s3, 0x02*4(a1)

    call NextKey
    addi a1, a1, 0x04*4
    call NextKey
    addi a1, a1, 0x04*4
    call NextKey


    lw x1, 0(sp)
    addi sp, sp, 4
ret

NextKey:
    lw s2, 0x04*4(a1)
    lw s8, 0*4(a1)
    xor s2, s2, s8
    sb s2, 0x04*4(a1)

    lw s2, 0x05*4(a1)
    lw s8, 0x01*4(a1)
    xor s2, s2, s8
    sb s2, 0x05*4(a1)

    lw s2, 0x06*4(a1)
    lw s8, 0x02*4(a1)
    xor s2, s2, s8
    sb s2, 0x06*4(a1)

    lw s2, 0x07*4(a1)
    lw s8, 0x03*4(a1)
    xor s2, s2, s8
    sb s2, 0x07*4(a1)
ret




    //Memory tables
.balign(0x100)



sbox:
        //0     1    2      3     4    5     6     7      8    9     A      B    C     D     E     F
.byte  0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76
.byte  0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0
.byte  0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15
.byte  0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75
.byte  0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84
.byte  0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf
.byte  0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8
.byte  0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2
.byte  0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73
.byte  0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb
.byte  0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79
.byte  0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08
.byte  0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a
.byte  0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e
.byte  0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf
.byte  0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16

xtime:
.byte 0x00,0x02,0x04,0x06,0x08,0x0a,0x0c,0x0e,0x10,0x12,0x14,0x16,0x18,0x1a,0x1c,0x1e
.byte 0x20,0x22,0x24,0x26,0x28,0x2a,0x2c,0x2e,0x30,0x32,0x34,0x36,0x38,0x3a,0x3c,0x3e
.byte 0x40,0x42,0x44,0x46,0x48,0x4a,0x4c,0x4e,0x50,0x52,0x54,0x56,0x58,0x5a,0x5c,0x5e
.byte 0x60,0x62,0x64,0x66,0x68,0x6a,0x6c,0x6e,0x70,0x72,0x74,0x76,0x78,0x7a,0x7c,0x7e
.byte 0x80,0x82,0x84,0x86,0x88,0x8a,0x8c,0x8e,0x90,0x92,0x94,0x96,0x98,0x9a,0x9c,0x9e
.byte 0xa0,0xa2,0xa4,0xa6,0xa8,0xaa,0xac,0xae,0xb0,0xb2,0xb4,0xb6,0xb8,0xba,0xbc,0xbe
.byte 0xc0,0xc2,0xc4,0xc6,0xc8,0xca,0xcc,0xce,0xd0,0xd2,0xd4,0xd6,0xd8,0xda,0xdc,0xde
.byte 0xe0,0xe2,0xe4,0xe6,0xe8,0xea,0xec,0xee,0xf0,0xf2,0xf4,0xf6,0xf8,0xfa,0xfc,0xfe
.byte 0x1b,0x19,0x1f,0x1d,0x13,0x11,0x17,0x15,0x0b,0x09,0x0f,0x0d,0x03,0x01,0x07,0x05
.byte 0x3b,0x39,0x3f,0x3d,0x33,0x31,0x37,0x35,0x2b,0x29,0x2f,0x2d,0x23,0x21,0x27,0x25
.byte 0x5b,0x59,0x5f,0x5d,0x53,0x51,0x57,0x55,0x4b,0x49,0x4f,0x4d,0x43,0x41,0x47,0x45
.byte 0x7b,0x79,0x7f,0x7d,0x73,0x71,0x77,0x75,0x6b,0x69,0x6f,0x6d,0x63,0x61,0x67,0x65
.byte 0x9b,0x99,0x9f,0x9d,0x93,0x91,0x97,0x95,0x8b,0x89,0x8f,0x8d,0x83,0x81,0x87,0x85
.byte 0xbb,0xb9,0xbf,0xbd,0xb3,0xb1,0xb7,0xb5,0xab,0xa9,0xaf,0xad,0xa3,0xa1,0xa7,0xa5
.byte 0xdb,0xd9,0xdf,0xdd,0xd3,0xd1,0xd7,0xd5,0xcb,0xc9,0xcf,0xcd,0xc3,0xc1,0xc7,0xc5
.byte 0xfb,0xf9,0xff,0xfd,0xf3,0xf1,0xf7,0xf5,0xeb,0xe9,0xef,0xed,0xe3,0xe1,0xe7,0xe5


