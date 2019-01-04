//
//  main.cpp
//  ESE532 Project
//
//  Created by JD Lauigan on 12/6/17.
//  Copyright © 2017 Joseph Lauigan. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include "Project1.h"




void Exit_with_error(void)
{
    perror(NULL);
    exit(EXIT_FAILURE);
}

void Check_error(int Error, const char * Message)
{
    if (Error)
    {
        puts(Message);
        exit(EXIT_FAILURE);
    }
}

void Free(unsigned char * Frame)
{

    free(Frame);

}

void Load_data(unsigned char * Data)
{
    unsigned int Size = MAX_DATA_SIZE;
    
    FILE * File = fopen("/Users/jdlauigan/Documents/UPenn/Franklin1.txt", "rb");
    if (File == NULL)
        Exit_with_error();
    
    if (fread(Data, 1, Size, File) != Size)
        Exit_with_error();
    
    if (fclose(File) != 0)
        Exit_with_error();

}

void Store_data(const char * Filename, unsigned char * Data, unsigned int Size)
{

    FILE * File = fopen(Filename, "wb");
    if (File == NULL)
        Exit_with_error();
    
    if (fwrite(Data, 1, Size, File) != Size)
        Exit_with_error();
    
    if (fclose(File) != 0)
        Exit_with_error();
}


unsigned char * Allocate_Char(int Size)
{
    unsigned char * Frame = (unsigned char *)

    malloc(Size);

    Check_error(Frame == NULL, "Could not allocate memory.\n");
    
    return Frame;
}

unsigned int * Allocate_int(int Size)
{
    unsigned int * Frame = (unsigned int *)

    malloc(Size);

    Check_error(Frame == NULL, "Could not allocate memory.\n");
    
    return Frame;
}

unsigned long * Allocate_Long(int Size)
{
    unsigned long * Frame = (unsigned long *)

    malloc(Size);

    Check_error(Frame == NULL, "Could not allocate memory.\n");
    
    return Frame;
}




int main()
{
    unsigned char * Input_data = Allocate_Char(MAX_DATA_SIZE);
    //unsigned char * Chunk_data = Allocate_Char(MAX_DATA_SIZE);
    //unsigned char * Length_str = Allocate_Char(MAX_FIFO_LENGTH);
    //unsigned char * Sha_str = Allocate_Char(MAX_FIFO_LENGTH);
    unsigned int * Length_Arr = Allocate_int(MAX_FIFO_LENGTH);
    unsigned int * hash = Allocate_int(SHA_VALUES);
    
#ifdef __SDSCC__
    FATFS FS;
    Check_error(f_mount(&FS, "0:/", 0) != FR_OK, "Could not mount SD-card");
#endif
    //Load_data(Input_data);
    //unsigned char Temp[MAX_DATA_SIZE];
    //static unsigned int hash_table[MAX_FIFO_LENGTH][SHA_DATA_LENGTH];
    unsigned int chunks_created;
    unsigned int Output[MAX_DATA_SIZE];
    
    
    unsigned char Input_test[64] = {'t','t','t','t','t','t','t','t','t','t','t','t','t','t','t','t','t','t','t','t','t','t','t','t','t','t','t','t','t','t','t','t','t','t','t','t','t','t','t','t','t','t','t','t','t','t','t','t','t','t','t','t','t','t','t','t','t','t','t','t','t','t','t','t'};
    unsigned int Length_test[1] = {32};
     
    
    unsigned int temp;
    chunks_created = 1;
    printf("%u\n",32);
    //chunks_created = create_chunk_test(Input_data, Length_Arr);
    printf("Chunks Created: %d\n", chunks_created);
    //Chunk_Create_Sha(Input_data, Length_Arr, hash, chunks_created);
    //sha256_hw(Input_data, Length_Arr,chunks_created, hash);
    sha256_hw(Input_test, Length_test,chunks_created, hash);
    chunk_match(Input_test, Length_test, hash, Output, chunks_created);
    
    
    /*
     for(unsigned int i = 0; i < chunks_created;i++)
     {
     printf("(After) Length: %u\n",Length_Arr[i]);
     }*/
    
    /*
    
     //unsigned long temp;
     for(unsigned int i = 0; i < chunks_created+1; i++)
     {
         for(int j = 0; j < SHA_DATA_LENGTH;j++)
         {
            temp = hash[(i*SHA_DATA_LENGTH)+j];
            printf("(After)SHA VALUE %d: %u\n", i, temp);
         }
     }
      */
    
    
}

