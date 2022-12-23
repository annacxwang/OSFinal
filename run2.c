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

void newFile(char*filename,long block_size,long block_ct){
	double start,end;
	int fd = open(filename,O_RDWR|O_CREAT|O_TRUNC,S_IRWXU);
    	if(fd == -1){
    	    printf("Fail to open file: %s\n",filename);
    	    exit(0);
    	}
    	long buf_size = block_size*block_ct;
    	unsigned int* buf= malloc(buf_size);
    	srandom(time(NULL));
    	for(long i = 0; i<buf_size/sizeof(unsigned int);i++){
    	    buf[i] = random();
    	}
    	
    	start = now();

    	long byte_written,total_written;
    	for(long i =0;i<block_ct;i++){
    	    byte_written = write(fd,buf+i*block_size/sizeof(int),block_size);
    	    if (byte_written == -1){
    	    printf("Fail to write to: %s\n",filename);
    	    exit(0);
    		}
    	    total_written += byte_written;
    	}
    	end = now();
    	
    	/**
    	printf("%ld bytes written in %f milliseconds\n",total_written,end-start);
    	
    	double MB = total_written / pow(10.0,6.0);
    	double seconds = (end - start) /1000.0;
    	//printf("%f MB %f seconds\n",MB,seconds);
    	printf("writing speed is %f MB/s\n",MB/seconds);
    	**/
        free(buf);
    	close(fd);
}

void readFile(char* filename,long block_size,long block_ct,double* time, unsigned int* result ){
	double start,end;
	long buf_size = block_size*block_ct;
	int fd = open(filename,O_RDONLY);
    	if(fd == -1){
    	    printf("Fail to open file: %s\n",filename);
    	    exit(0);
    	}
    	unsigned int* buf= (unsigned int*) malloc(buf_size);
    	start = now();
    	long byte_read,total_read;
    	for(long i =0;i<block_ct;i++){
    	    byte_read = read(fd,buf+i*block_size/sizeof(int),block_size);
    	    if (byte_read == -1){
    	    printf("Fail to read from: %s\n",filename);
    	    exit(0);
    		}
    	    total_read += byte_read;
    	}
    	end = now();
    	*time = end - start;
	*result = xorbuf(buf,buf_size/sizeof(int));
    	double MiB = total_read / pow(2.0,20.0);
    	double seconds = (end - start) /1000.0;
    	//printf("%f MiB %f seconds\n",MiB,seconds);
    	
    	//printf("reading speed is %f MiB/s\n",MiB/seconds);
        free(buf);
    	close(fd);
	
}

int main(int argc,char * argv[])
{
   if(argc != 3){
    	printf("Check usage : ./run2 <filename> <block_size>\n");
    	return 0;
    }
    //1- filename 2-block size 
    if(!isNumber(argv[2])){
    	printf("Check usage : ./run2 <filename> <block_size>\n");
    	return 0;
    }
    char *filename = argv[1];
    long block_size = atoi(argv[2]);
    if(block_size % 4 != 0){
    	printf("Block size needs to be multiple of 4!\n");
    	return 0;
    }
    long block_ct = 1;
    double time = 0.0;
    unsigned int result;
    //newFile(filename,block_ct*block_size);
    //readFile(filename,block_ct*block_size,&time,&result);
    //printf("reading time is %f seconds, block count is %ld\n",time/1000.0,block_ct);
    long file_size;
    while(time < 5000.0 ){
    	if(block_ct*block_size>(1.2*pow(10,9))){
    	    break;
    	}
    	file_size = block_ct*block_size;
    	newFile(filename,block_size,block_ct);
    	readFile(filename,block_size,block_ct,&time,&result);
    	//printf("reading time is %f seconds, size is %ld\n",time/1000.0,file_size);
    	if(time <5000.0){
    	    block_ct *=2;
    	}
    	else if(time > 15000.0){
    	    block_ct *=2;
    	    block_ct /=3;
    	}
    }
    
    double MiB = file_size / pow(2.0,20.0);
    double seconds = time /1000.0;
    //printf("%f MiB %f seconds\n",MiB,seconds);
    printf("xor result is %08x\n",result);
    printf("size of file is %ld bytes\n",file_size);
    printf("reading speed is %f MiB/s\n",MiB/seconds);
    
    
   return 0;
}
