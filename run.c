#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int isNumber(char in[]){

    for(int i=0;in[i]!=0;i++){
    	if(in[i]>'9' || in[i]< '0'){
    	    return 0;
    	}
    }
    return 1;
}

unsigned int xorbuf(unsigned int *buffer, int size) {
    unsigned int result = 0;
    for (int i = 0; i < size; i++) {
        result ^= buffer[i];
    }
    return result;
}


int main(int argc,char * argv[])
{
   /* printf function displays the content that is
    * passed between the double quotes.
    */
    
    if(argc != 5){
    	printf("Check usage : ./run <filename> [-r|-w] <block_size> <block_count>\n");
    }
    //1- filename 2-rw 3- block size 4- block count
    if(!isNumber(argv[3]) || !isNumber(argv[4])){
    	printf("Check usage : ./run <filename> [-r|-w] <block_size> <block_count>\n");
    	return 0;
    }
    int block_size = atoi(argv[3]);
    int block_ct = atoi(argv[4]);
    int buf_size = block_size * block_ct; // file size in bytes
    if(buf_size % 4 != 0){
    	buf_size = buf_size - (buf_size %4); 
    }
    
    if(strcmp(argv[2],"-r")==0){
    	//printf("1\n");
    	int fd = open(argv[1],O_RDONLY);
    	if(fd == -1){
    	    printf("Fail to open file: %s\n",argv[1]);
    	    return 0;
    	}
    	unsigned int* buf= malloc(buf_size);
    	long byte_read = read(fd,buf,buf_size);
    	if (byte_read == -1){
    	    printf("Fail to read from: %s\n",argv[1]);
    	    return 0;
    	}
    	printf("%ld\n",byte_read);
    	/**
    	for(int i  =0;i<buf_size/sizeof(int);i++){
    	    printf("%d ",buf[i]);
    	}
    	**/
    	printf("xor result is %d\n",xorbuf(buf,buf_size/sizeof(int)));
    	close(fd);
    
    }
    else if(strcmp(argv[2],"-w")==0){
    printf("2\n");
    
    }
    else{
    printf("Check usage : ./run <filename> [-r|-w] <block_size> <block_count>\n");
    }
   return 0;
}
