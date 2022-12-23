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

unsigned int xorbuf(unsigned int *buffer, int size) {
    unsigned int result = 0;
    for (int i = 0; i < size; i++) {
        result ^= buffer[i];
        //printf("xor %08x = %08x \n",buffer[i],result);
    }
    
    return result;
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
    
    if(argc != 5){
    	printf("Check usage : ./run <filename> [-r|-w] <block_size> <block_count>\n");
        return 0;
    }
    //1- filename 2-rw 3- block size 4- block count
    if(!isNumber(argv[3]) || !isNumber(argv[4])){
    	printf("Check usage : ./run <filename> [-r|-w] <block_size> <block_count>\n");
    	return 0;
    }
    long block_size = atoi(argv[3]);
    
    if(block_size % 4 != 0){
    	printf("Block size needs to be multiple of 4!\n");
    	return 0;
    }
    
    long block_ct = atoi(argv[4]);
    long buf_size = block_size * block_ct; // file size in bytes

    
    double start,end;
    
    if(strcmp(argv[2],"-r")==0){
    	//printf("1\n");
    	int fd = open(argv[1],O_RDONLY);
    	if(fd == -1){
    	    printf("Fail to open file: %s\n",argv[1]);
    	    return 0;
    	}
    	unsigned int* buf= (unsigned int*) malloc(buf_size);
    	start = now();
    	long byte_read,total_read;
    	for(long i =0;i<block_ct;i++){
    	    byte_read = read(fd,buf+i*block_size/sizeof(int),block_size);
    	    if (byte_read == -1){
    	    printf("Fail to read from: %s\n",argv[1]);
    	    return 0;
    		}
    	    total_read += byte_read;
    	}
    	end = now();
    	
    	printf("%ld bytes read in %f milliseconds\n",total_read,end-start);
    	/**
    	for(int i  =0;i<buf_size/sizeof(int);i++){
    	    printf("%d ",buf[i]);
    	}
    	**/
    	//printf("check: 23 %d 499 %d 501 %d",buf[23],buf[499],buf[501]);
    	
    	printf("xor result is %08x\n",xorbuf(buf,buf_size/sizeof(int)));
    	//int SIZE = buf_size/sizeof(int);
    	//unsigned int xor1 = xorbuf(buf, SIZE / 2);
    	//unsigned int xor2 = xorbuf(buf + SIZE / 2, SIZE / 2);
    	//printf("xor result is %08x\n",xor1^xor2);
    	double MiB = total_read / pow(2.0,20.0);
    	double seconds = (end - start) /1000.0;
    	//printf("%f MiB %f seconds\n",MiB,seconds);
    	
    	printf("reading speed is %f MiB/s\n",MiB/seconds);
        free(buf);
    	close(fd);
    
    }
    else if(strcmp(argv[2],"-w")==0){
    	//printf("2\n");
    	int fd = open(argv[1],O_RDWR|O_CREAT|O_TRUNC,S_IRWXU);
    	if(fd == -1){
    	    printf("Fail to open file: %s\n",argv[1]);
    	    return 0;
    	}
    	unsigned int* buf= malloc(buf_size);
    	srandom(time(NULL));
    	for(long i = 0; i<buf_size/sizeof(unsigned int);i++){
    	    buf[i] = random();
    	}
    	
	//printf("xor result is %08x\n",xorbuf(buf,buf_size/sizeof(int)));
	
    	start = now();
    	//long byte_written = write(fd,buf,buf_size);
    	long byte_written,total_written;
    	for(long i =0;i<block_ct;i++){
    	    byte_written = write(fd,buf+i*block_size/sizeof(int),block_size);
    	    if (byte_written == -1){
    	    printf("Fail to write to: %s\n",argv[1]);
    	    return 0;
    		}
    	    total_written += byte_written;
    	    //printf("%ld,%ld,%ld, total %ld\n",buf_size,i,byte_written,total_written);
    	}
    	end = now();
    	
    	//printf("check: 23 %d 499 %d 501 %d",buf[23],buf[499],buf[501]);
    	
    	printf("%ld bytes written in %f milliseconds\n",total_written,end-start);
    	
    	double MB = total_written / pow(10.0,6.0);
    	double seconds = (end - start) /1000.0;
    	//printf("%f MB %f seconds\n",MB,seconds);
    	printf("writing speed is %f MB/s\n",MB/seconds);
        free(buf);
    	close(fd);
    }
    else{
    printf("Check usage : ./run <filename> [-r|-w] <block_size> <block_count>\n");
    }
   return 0;
}
