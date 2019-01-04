//
//  Project1.h
//  ESE532 Project
//
//  Created by JD Lauigan on 12/6/17.
//  Copyright © 2017 Joseph Lauigan. All rights reserved.
//

#ifndef Project1_h
#define Project1_h

//#include "hls_stream.h"
//#include "lzw.h"



#define MAX_CHUNK_SIZE (64000)
#define MAX_CHUNK_SIZE_PAD (64063)
#define MAX_DATA_SIZE (64)
#define CHUNK_SIZE_PAD (4063)
#define MAX_FIFO_LENGTH (1)
#define SHA_VALUES (MAX_FIFO_LENGTH*SHA_DATA_LENGTH)
#define CHUNK_SIZE (4000)
#define WINDOW_SIZE (16)
#define PRIME (379)
//#define MODULUS (0xbfe6b8a5bf378d83LL) //LBFS poly
#define MODULUS (4294967296)
#define SHA_DATA_LENGTH (8)

extern unsigned int chunks_created;




void chunk_match(unsigned char Input[MAX_DATA_SIZE], unsigned int length_in[MAX_FIFO_LENGTH], unsigned int sha_in[MAX_FIFO_LENGTH], unsigned int Output[MAX_DATA_SIZE], unsigned int num_chunks);
void sha256_test(unsigned char Input[MAX_DATA_SIZE], unsigned int length_input[MAX_FIFO_LENGTH], unsigned long Output[SHA_VALUES]);
unsigned int create_chunk_test(unsigned char Input[MAX_DATA_SIZE], unsigned int Output[MAX_FIFO_LENGTH]);
//unsigned int create_chunk(hls::stream<unsigned char> &Input, hls::stream<unsigned char> &Data, hls::stream<unsigned int> &Output);
//void create_chunk2(unsigned char Input[MAX_DATA_SIZE], hls::stream<unsigned char> &Data, hls::stream<unsigned int> &Output, unsigned int num_chunks);
//void sha256(hls::stream<unsigned char> &Data_in, hls::stream<unsigned int> &length_in, hls::stream<unsigned char> &chunk_out, hls::stream<unsigned int> &length_out, hls::stream<unsigned long> &sha_out);
void sha256_hw(unsigned char Input[MAX_DATA_SIZE],unsigned int length_in[MAX_FIFO_LENGTH],unsigned int num_chunks, unsigned int sha_data[SHA_VALUES]);
//void sha256_hw2(hls::stream<unsigned char> &Data_in, hls::stream<unsigned int> &length_in,unsigned int length_out[MAX_FIFO_LENGTH], unsigned int sha_data[SHA_VALUES]);
void Chunk_Create_Sha(unsigned char Input[MAX_DATA_SIZE], unsigned int length_out[MAX_FIFO_LENGTH], unsigned int sha_data[SHA_VALUES], unsigned int num_chunks);

#endif /* Project1_h */
