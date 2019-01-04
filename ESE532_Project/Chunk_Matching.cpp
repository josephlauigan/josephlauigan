//
//  Chunk_Matching.cpp
//  ESE532 Project
//
//  Created by JD Lauigan on 12/6/17.
//  Copyright © 2017 Joseph Lauigan. All rights reserved.
//

#include <stdio.h>
#include <string>
//#include "lzw.h"
#include "Project1.h"



//static int hash_index = 0;
//static int length_global = 0;
int curr_output_index = 0;


void chunk_match(unsigned char Input[MAX_DATA_SIZE], unsigned int length_in[MAX_FIFO_LENGTH], unsigned int sha_in[MAX_FIFO_LENGTH], unsigned int Output[MAX_DATA_SIZE], unsigned int num_chunks, const char * Filename)
{
    
    static unsigned int hash_table[MAX_FIFO_LENGTH][SHA_DATA_LENGTH];
    unsigned int length = 0, length_current= 0;
    char chunk_local[MAX_CHUNK_SIZE];
    static unsigned int sha_local[SHA_DATA_LENGTH];
    //unsigned int hash_local[SHA_DATA_LENGTH];
    unsigned int flag = 1, sha_offset = 0;
    unsigned int found_index = 0, found_count = 0;
    int i = 0;
    unsigned int hash_index = 0;
    
    /*
     * Assumption is that the chunk_in, length_in, sha_in have correct data. Sha_in stream has 32b int's, so have to pull in 8x int's from stream to get 256b SHA value
     * then parse static table hash_table (this will need to be optimized later). If found, send 32b header with index of found hash, if not send to lzw then output.
     */
    
    printf("Num_chuks: %d\n",num_chunks);
    for(int j = 0; j < num_chunks+1; j++)
    {
        printf("Chunk Match %d\n",j);
        flag = 1;
        if(j==0)
        {
            length = 0;
            length_current = length_in[j];
            printf("index %d, length = %u\n",j, length);
        }
        else if(j == num_chunks)
        {
            length = length_in[j-1];
            length_current = MAX_DATA_SIZE;
            printf("index %d, length = %u\n",j, length);
        }
        else
        {
            length = length_in[j-1];
            length_current = length_in[j];
            printf("index %d, length = %u\n",j, length);
        }
        
        
        for(i = 0; i < SHA_DATA_LENGTH;i++)
        {
            sha_local[i] = sha_in[sha_offset+i];
            printf("Index %d, SHA %d: %u\n",j,i,sha_local[i]);
        }
        sha_offset += SHA_DATA_LENGTH;
        
        if(hash_index == 0)
        {
            printf("hash index : %d\n",hash_index);
            for(i = 0;i<SHA_DATA_LENGTH;i++)
            {
                hash_table[hash_index][i] = sha_local[i];
                printf("initital hash table set : %u, hash local: %u\n",hash_table[hash_index][i],sha_local[i]);
            }
            hash_index++;
            for(unsigned int h = length; h < length_current; h++)
            {
                chunk_local[h] = Input[h];
            }
            //call lzw function, passing in chunk_local[i] and length.
            //write to output file the output of lzw
            //software_compress(chunk_local, length_in[j]-length, Output);
        }
        else
        {
            printf("hash index : %d\n",hash_index);
            for(i = 0; i < hash_index; i++)
            {
                for(int p = 0; p < SHA_DATA_LENGTH; p++)
                {
                    printf("hash table[%d][%d] : %u, hash local[%d]: %u\n",i,p,hash_table[i][p],p,sha_local[p]);
                    if(sha_local[p] != hash_table[i][p])
                    {
                        printf("Flag set to zero\n");
                        flag = 0;
                    }
                }
                if(flag!=0)
                {
                    printf("Flag NOT set to zero\n");
                    found_index = i;
                    found_count++;
                    break;
                }
            }
            
            if(flag==0)
            {
                for(i=0;i<SHA_DATA_LENGTH;i++)
                {
                    hash_table[hash_index][i] = sha_local[i];
                }
                hash_index++;
                printf("updated hash_index: %d\n call LZW on chunk(%d)\n",hash_index,j);
                //Call LZW function, passing chunk_local[i] and length
                for(unsigned int h = length; h < length_current; h++)
                {
                    chunk_local[h] = Input[h];
                }
                //write output of lzw function to output_file
                //software_compress(chunk_local, length_in[j]-length, Output);
            }
            else
            {
                //format the 32b header to have have bit 0 always be 1, then the found index.
                unsigned int output = (found_index >> 1) | 0x1;
                Output[curr_output_index++] = output;
                //write this binary to output file
            }
        }
        
        
        
    }
    printf("Chunks Matched: %d\n", found_count);
    
}
