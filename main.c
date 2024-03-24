//
//  main.c
//  RC4 Implementation
//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <inttypes.h>
#include "RC4.h"
#include"time.h"

//Function to read file by getting pointer to the file name to return the file contents and file size
unsigned char * readFile(FILE *f,long *fsize)
{
    unsigned char * data;
    fseek (f,0,2);
    long x =ftell(f);
    *fsize = x;
    fseek (f,0,0);
    data = (unsigned char *) calloc(*fsize+10,sizeof(unsigned char));
    memset (data,0,*fsize+10);
    fread(data,1,*fsize,f);
    fclose (f);
    return data;
}


// function to encrypt data in the sourceFile using keyFile then store the Cipher in resultFile
void Encryptor(const char *sourceFile, const char  *keyFile, const char *resultFile)
{   
    unsigned char * data;
    unsigned char * Cypher;
    long fsize,ksize ;
    // Clock
    clock_t start, finish;
    double time_taken;
	
	
    FILE * f = fopen (sourceFile,"rb");
    data = readFile(f,&fsize);
    Cypher = (unsigned char *) calloc(fsize+10,sizeof(unsigned char));
    memset (Cypher,0,fsize+10);
    unsigned char* key;
    f = fopen (keyFile,"rb");
    key = readFile(f,&ksize);
    
    if(ksize > 256) // check length key to keep sure that it's less than or equal 256 byte
    {
        printf("Key length can not be more than 256 byte according to the RC4 algorithm");
        exit(0);
    }
    
    //printing plain text on console
    printf("\nPlain Text from %s:\n%s\n",sourceFile, data);
    
    //printing Key text on console
    printf("\nKey from %s:\n%s\n",keyFile, key);
    
    //Calling the RC4 function to do the encryption and return result in Cypher
    start = clock();
    RC4(data, fsize-1, key,ksize,Cypher);
    finish = clock();
    //printing Cipher result on console to the user in HEX format
    printf("\n\nCipher written to %s: \n",resultFile);
    for (int i = 0 ; i < fsize-1;i++)
    {
        printf("%02hhX  ",Cypher[i]);
    }
    printf("\n\n");
    
    /* Write Cipher to the resultFile */
    f = fopen(resultFile, "w");
    fwrite(Cypher, 1, fsize, f);
    fclose(f);
    
    //release the pointers in memory
    free (data);
    free (Cypher);
    free (key);
    time_taken = (double)(finish - start)/(double)CLOCKS_PER_SEC;
    printf("Finished processing. Time taken: %lf seconds.\n", time_taken);
}

int main(int argc, const char * argv[]) {
    const char *choice = argv[1];
    const char *sourceFile = argv[2];
    const char *keyFile = argv[3];
    const char *resultFile = argv[4];
    
    
    if(strcmp(choice,"Encrypt") == 0)
    {	
        Encryptor(sourceFile, keyFile, resultFile);	
}
return 0;
}
