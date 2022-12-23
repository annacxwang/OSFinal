#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <unistd.h>
#include <math.h>

int isNumber(char in[]){

    for(int i=0;in[i]!=0;i++){
    	if(in[i]>'9' || in[i]< '0'){
    	    return 0;
    	}
    }
    return 1;
}

void xorbuf(unsigned int *buffer, int size, unsigned int* result) {

    for (int i = 0; i < size; i++) {
        *result ^= buffer[i];
        //printf("xor %08x = %08x \n",buffer[i],result);
    }
    
    //return result;
}

double now(){
    struct timeval tv;
    gettimeofday(&tv,0);
    return tv.tv_usec/1000.0 + tv.tv_sec*1000.0;

}
int main(int argc,char * argv[])
{
   /* printf function displays the content that is
    * passed between the double quotes.
    */
    
    if(argc != 2){
    	printf("Check usage : ./fast <filename> \n");
        return 0;
    }
    //1- filename 

    long block_size = 400000;

    
    double start,end;
    int fd = open(argv[1],O_RDONLY);
    if(fd == -1){
    	   printf("Fail to open file: %s\n",argv[1]);
    	   return 0;
    	}
    unsigned int* buf= (unsigned int*) malloc(block_size);
    
    long byte_read,total_read;
    unsigned int result =0;
    double time = 0.0;
    start = now();
    byte_read = read(fd,buf,block_size);
    end = now();
    time = end - start;
    total_read = 0;
    while(byte_read>0){
    	total_read+=byte_read;
    	xorbuf(buf,block_size/sizeof(int),&result);
    	start = now();
    	byte_read = read(fd,buf,block_size);
    	end = now();
    	time +=end - start;
    	
    }
    	
    
    	printf("%ld bytes read in %f milliseconds\n",total_read,time);

    	printf("xor result is %08x\n",result);
    	double MiB = total_read / pow(2.0,20.0);
    	double seconds = time /1000.0;
    	
    	printf("reading speed is %f MiB/s\n",MiB/seconds);
        free(buf);
    	close(fd);
 
   return 0;
}
