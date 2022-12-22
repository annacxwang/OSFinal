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
    long block_ct = atoi(argv[4]);
    long buf_size = block_size * block_ct; // file size in bytes
    if(buf_size % 4 != 0){
    	buf_size = buf_size - (buf_size %4); 
    }
    
    double start,end;
    
    if(strcmp(argv[2],"-r")==0){
    	//printf("1\n");
    	int fd = open(argv[1],O_RDONLY);
    	if(fd == -1){
    	    printf("Fail to open file: %s\n",argv[1]);
    	    return 0;
    	}
    	unsigned int* buf= malloc(buf_size);
    	start = now();
    	long byte_read = read(fd,buf,buf_size);
    	end = now();
    	
    	if (byte_read == -1){
    	    printf("Fail to read from: %s\n",argv[1]);
    	    return 0;
    	}
    	printf("%ld bytes read in %f milliseconds\n",byte_read,end-start);
    	/**
    	for(int i  =0;i<buf_size/sizeof(int);i++){
    	    printf("%d ",buf[i]);
    	}
    	**/
    	//printf("xor result is %08x\n",xorbuf(buf,buf_size/sizeof(int)));
    	int SIZE = buf_size/sizeof(int);
    	unsigned int xor1 = xorbuf(buf, SIZE / 2);
    	unsigned int xor2 = xorbuf(buf + SIZE / 2, SIZE / 2);
    	printf("xor result is %08x\n",xor1^xor2);
    	double MiB = byte_read / pow(2.0,20.0);
    	double seconds = (end - start) /1000.0;
    	//printf("%f MiB %f seconds\n",MiB,seconds);
    	printf("reading speed is %f MiB/s\n",MiB/seconds);
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
    	for(int i = 0; i<buf_size/sizeof(int);i++){
    	    buf[i] = random();
    	}
    	
    	start = now();
    	long byte_written = write(fd,buf,buf_size);
    	end = now();
    	
    	if (byte_written == -1){
    	    printf("Fail to write to: %s\n",argv[1]);
    	    return 0;
    	}
    	printf("%ld bytes written in %f milliseconds\n",byte_written,end-start);
    	
    	double MB = byte_written / pow(10.0,6.0);
    	double seconds = (end - start) /1000.0;
    	//printf("%f MB %f seconds\n",MB,seconds);
    	printf("writing speed is %f MB/s\n",MB/seconds);
    	close(fd);
    }
    else{
    printf("Check usage : ./run <filename> [-r|-w] <block_size> <block_count>\n");
    }
   return 0;
}
