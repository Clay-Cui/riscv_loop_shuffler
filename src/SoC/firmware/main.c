// This file is Copyright (c) 2020 Florent Kermarrec <florent@enjoy-digital.fr>
// License: BSD

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <irq.h>
#include <libbase/uart.h>
#include <libbase/console.h>
#include <generated/csr.h>

#include "ntt/testvectors.h"
#include "ntt/ntt.h"


// void aes_Shuffle_dummy_32(void);
void aes_shuffle
void permu128(void);



// unsigned char cycles[8];     
unsigned char pt_aes[16] = { 0x00, 0x01, 0x02, 0x03,
                             0x04, 0x05, 0x06, 0x07,
                             0x08, 0x09, 0x0a, 0x0b,
                             0x0c, 0x0d, 0x0e, 0x0f};
unsigned char key_aes[16] = {0x03, 0xcf, 0x4f, 0x3c,
							0x2b, 0x7e, 0x15, 0x16, 
							 0x28, 0xae, 0xd2, 0xa6, 
							 0xab, 0xf7, 0x15, 0x88
							 };
					 
unsigned char key_backup[16] = {0x09, 0xcf, 0x4f, 0x3c,
								0x2b, 0x7e, 0x15, 0x16, 
							 0x28, 0xae, 0xd2, 0xa6, 
							 0xab, 0xf7, 0x15, 0x88 
							 };
						

uint32_t  shiftRowTable[32] = {
                                  0x00*4,0x0d*4,0x0a*4,0x07*4,
                                  0x04*4,0x01*4,0x0e*4,0x0b*4,
                                  0x08*4,0x05*4,0x02*4,0x0f*4,
                                  0x0c*4,0x09*4,0x06*4,0x03*4,
								  0x10*4,0x1d*4,0x1a*4,0x17*4,
                                  0x14*4,0x11*4,0x1e*4,0x1b*4,
                                  0x18*4,0x15*4,0x12*4,0x1f*4,
                                  0x1c*4,0x19*4,0x16*4,0x13*4};


uint32_t map1[32] =
        { 1, 2, 3, 0, 5, 6, 7, 4, 9, 10, 11, 8, 13, 14, 15, 12};
uint32_t map2[32] =
        { 2, 3, 0, 1, 6, 7, 4, 5, 10, 11, 8, 9, 14, 15, 12, 13};
uint32_t map3[32] =
        { 3, 0, 1, 2, 7, 4, 5, 6, 11, 8, 9, 10, 15, 12, 13, 14};	
					  							

// unsigned char inter[32];


	
unsigned char order[128*3];


unsigned char permu[128];	
	
unsigned char r[16];				 						
	// int num=0;
void init_tab(void){
	// int i;
	// for(i = 0; i<16; i++){
	// 	map1[i] = map1[i]*4;
	// 	map2[i] = map2[i]*4;
	// 	map3[i] = map3[i]*4;

	// 	map1[i+16] = map1[i]+64;
	// 	map2[i+16] = map2[i]+64;
	// 	map3[i+16] = map3[i]+64;
	// }
}



void print_vec(char * id, uint16_t * ptr)
{
    int i;
    printf("\n"); printf(id); printf("\n");
    for(i=0;i<256;i++)
    {
        printf("0x%04x, ", ptr[i]);
        if ( (i+1)%16 == 0) {
        printf("\n");
        }
    }
}

unsigned char pt;
unsigned int t1, t2;


int i;
unsigned int len, j,k,start;

int16_t t, zeta;
uint8_t temp;
uint8_t stage;
int loop = 0;
int main(void)
{
#ifdef CONFIG_CPU_HAS_INTERRUPT
	irq_setmask(0);
	irq_setie(1);
#endif
	
	uart_init();


	while(1){
		

	// AES:
	asm volatile(
		"setindex 16\n"
	);

	for(i=0;i<16;i++){
			pt_aes[i]=uart_read();
		}
	
	// Another 16/32 random bytes.
	// Put before the plaintext.
	// for(i=0;i<16;i++){
	// 		r[i]=uart_read();
	// 	}


	aes_shuffle();
	for (i=0;i<16;i++){
				uart_write(pt_aes[i]);
				
	}

	memcpy(key_aes,key_backup,16);
	/******************************************/
	// NTT
    uint16_t r[256];
	uint8_t t;
	for (i = 0;i<256;i++){
		t = uart_read();
		r[i] = t<<8;
		t = uart_read();
		r[i] = r[i] ^ t;
	}
	
	asm volatile(
		"csrw 0x7e0, 3\n"
    	"csrw 0x7e1, 1\n"
	);

    // for(i=0; i<256; i++)
    // r[i] = NTT_INP_TV1[i];
	
    // perform NTT
	// Clock cycle counting
	// asm volatile(
	// 	"csrr %[t1], 0x780\n"
	// 	: [t1]"=r" (t1)
	// );
    ntt( &r[0] );
	
	// asm volatile( 
	// 	"csrr %[t2], 0x780\n"
	// 	: [t2]"=r" (t2)
	// );	

	// Check NTT result
    // for(i=0; i<256; i++)
    // {
    //     if (r[i] != NTT_OUT_TV1[i])
    //     {
			
    //         printf("\n *** RESULT NOT OK ***\n");
	// 		printf("%d\n",i);
    //         print_vec("Received Output", &r[0]);
    //         print_vec("Expected Output", &NTT_OUT_TV1[0]);
    //         break;
    //     }
	// 	// printf("%04x\n",NTT_OUT_TV1[i]);
    // }
    	// printf("\n *** RESULT OK ***\n");
	// t1 = 0;
	// t2 = 0;

	// print_vec("Expected Output", &NTT_OUT_TV1[0]);
	// printf("nttend\n");
	// if (loop == 50){

	// 	for(i=0;i<256;i++){
	// 		uart_write(r[i]>>8);
	// 		uart_write(r[i]);
	// 	}
	// 	loop = 0;
	// }


		// for(i=0;i<4;i++)
		// uart_write(((t1>>(i*8))));
	
	// for(i=0;i<4;i++)
		// uart_write(((t2>>(i*8))));

	}


	return 0;
}
