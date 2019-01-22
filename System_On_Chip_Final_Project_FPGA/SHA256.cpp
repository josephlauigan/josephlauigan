//
//  SHA256.cpp
//  ESE532 Project
//
//  Created by JD Lauigan on 12/6/17.
//  Copyright © 2017 Joseph Lauigan. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
//#include <memory.h>
#include <math.h>
#include "Project1.h"




#define ROTL(a,b) (((a) << (b)) | ((a) >> (32-(b))))
#define ROTR(a,b) (((a) >> (b)) | ((a) << (32-(b))))
#define CH(x,y,z) (((x) & (y)) ^ (~(x) & (z)))
#define MAJ(x,y,z)  (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))
#define SUM0(x) (ROTR(x,2) ^ ROTR(x,13) ^ ROTR(x,22))
#define SUM1(x) (ROTR(x,6) ^ ROTR(x,11) ^ ROTR(x,25))
#define SIG0(x) (ROTR(x,7) ^ ROTR(x,18) ^ (x >> 3))
#define SIG1(x) (ROTR(x,17) ^ ROTR(x,19) ^ (x >> 10))


static const unsigned long z[64] = {
    0x428a2f98,0x71374491,0xb5c0fbcf,0xe9b5dba5,0x3956c25b,0x59f111f1,0x923f82a4,0xab1c5ed5,
    0xd807aa98,0x12835b01,0x243185be,0x550c7dc3,0x72be5d74,0x80deb1fe,0x9bdc06a7,0xc19bf174,
    0xe49b69c1,0xefbe4786,0x0fc19dc6,0x240ca1cc,0x2de92c6f,0x4a7484aa,0x5cb0a9dc,0x76f988da,
    0x983e5152,0xa831c66d,0xb00327c8,0xbf597fc7,0xc6e00bf3,0xd5a79147,0x06ca6351,0x14292967,
    0x27b70a85,0x2e1b2138,0x4d2c6dfc,0x53380d13,0x650a7354,0x766a0abb,0x81c2c92e,0x92722c85,
    0xa2bfe8a1,0xa81a664b,0xc24b8b70,0xc76c51a3,0xd192e819,0xd6990624,0xf40e3585,0x106aa070,
    0x19a4c116,0x1e376c08,0x2748774c,0x34b0bcb5,0x391c0cb3,0x4ed8aa4a,0x5b9cca4f,0x682e6ff3,
    0x748f82ee,0x78a5636f,0x84c87814,0x8cc70208,0x90befffa,0xa4506ceb,0xbef9a3f7,0xc67178f2
};


//#pragma SDS data access_pattern(Input:SEQUENTIAL, length_in:SEQUENTIAL)
//#pragma SDS data copy(Input[0:MAX_DATA_SIZE], length_in[0:chunks_created], sha_data[0:(chunks_created*SHA_DATA_LENGTH)])
void sha256_hw(unsigned char Input[MAX_DATA_SIZE],unsigned int length_in[MAX_FIFO_LENGTH],unsigned int num_chunks, unsigned int sha_data[SHA_VALUES])
{
    unsigned int N = 0, length_prev = 0, length = 0, length_current;
    unsigned int j = 0, k = 0, y = 0;
    unsigned long a, b, c, d, e, f, g, h, t1, t2, m[64];
    unsigned char Chunk[MAX_CHUNK_SIZE_PAD];
    unsigned int data[SHA_DATA_LENGTH];
    
//#pragma HLS array_partition variable=Chunk cyclic factor=6
//#pragma HLS array_partition variable=data cyclic factor=8
    //int length_total = 0;
    int output_offset = 0;
    for(unsigned int i = 0; i < num_chunks+1; i++)
    {
        if(i == 0)
        {
            length_prev = 0;
            length_current = length_in[i];
        }
        else if(i == num_chunks)
        {
            length_prev = length_in[i-1];
            length_current = MAX_DATA_SIZE;
        }
        else
        {
            length_prev = length_in[i-1];
            length_current = length_in[i];
        }
        
        length = length_current - length_prev;
        for(y = 0,j = length_prev; y <length, j < length_current; y++, j++)
        {
            Chunk[y] = Input[j];
            //printf("Chunk Loaded %d: %u\n", i,Chunk[i]);
        }
        
        
        
        
        //initial values for hash value
        data[0] = 0x6a09e667;
        data[1] = 0xbb67ae85;
        data[2] = 0x3c6ef372;
        data[3] = 0xa54ff53a;
        data[4] = 0x510e527f;
        data[5] = 0x9b05688c;
        data[6] = 0x1f83d9ab;
        data[7] = 0x5be0cd19;
        
        //pad input if needed
        if(length%64 == 0)
        {
            N = length/64;
        }
        else if (length < 64)
        {
            unsigned int pad = (64 - length);
            N = (length+pad)/64;
            for(unsigned int z = 0; z < pad; z++)
            {
                Chunk[length+z] = 0;
            }
        }
        else
        {
            unsigned int pad = length%64;
            N = (length+(64-pad))/64;
            for(unsigned int z = 0; z < (64-pad); z++)
            {
                Chunk[length+z] = 0;
            }
        }
        
        //calculate SHA-256 hash for each 512-bit block of input chunk
        for(int l = 0; l < N; l++)
        {
            
            for(j=0,k=0; j<16; j++, k+=4)
            {
//#pragma HLS PIPELINE
                m[j] = (Chunk[(N*l)+k] << 24) | (Chunk[(N*l)+k+1]<<16) | (Chunk[(N*l)+k+2]<<8) | Chunk[(N*l)+k+3];
            }
            for( ;j<64; j++)
            {
//#pragma HLS PIPELINE
                m[j] = SIG1(m[j-2]) + m[j-7] + SIG0(m[j-15]) + m[j-16];
            }
            
            a = data[0];
            b = data[1];
            c = data[2];
            d = data[3];
            e = data[4];
            f = data[5];
            g = data[6];
            h = data[7];
            
            
            for(j = 0; j <64; ++j)
            {
//#pragma HLS PIPELINE
                t1 = h + SUM1(a) + CH(e,f,g) + z[j] + m[j];
                t2 = SUM0(a) + MAJ(a,b,c);
                h = g;
                g = f;
                f = e;
                e = d + t1;
                d = c;
                c = b;
                b = a;
                a = t1 + t2;
            }
            
            
            data[0] +=a;
            data[1] +=b;
            data[2] +=c;
            data[3] +=d;
            data[4] +=e;
            data[5] +=f;
            data[6] +=g;
            data[7] +=h;
        }
        
        for(int l = 0; l < SHA_DATA_LENGTH; l++)
        {
            sha_data[output_offset+l] = data[l];
        }
        output_offset += SHA_DATA_LENGTH;
        
    }
    
    
}

