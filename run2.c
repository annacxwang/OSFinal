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

void newFile(char*filename,long size){
	int fd = open(filename,O_RDWR|O_CREAT|O_TRUNC,S_IRWXU);
    	if(fd == -1){
    	    printf("Fail to open file: %s\n",filename);
    	    exit(0);
    	}
    	unsigned int* buf= malloc(size);
    	srandom(time(NULL));
    	for(int i = 0; i<size/sizeof(int);i++){
    	    buf[i] = random();
    	}
    	long byte_written = write(fd,buf,size);
    	
    	if (byte_written == -1){
    	    printf("Fail to write to: %s\n",filename);
    	    exit(0);
    	}
    	free(buf);
    	close(fd);
}

void readFile(char* filename,long buf_size,double* time, unsigned int* result ){
	double start,end;
	int fd = open(filename,O_RDONLY);
    	if(fd == -1){
    	    printf("Fail to open file: %s\n",filename);
    	    exit(0);
    	}
    	unsigned int* buf= malloc(buf_size);
    	start = now();
    	long byte_read = read(fd,buf,buf_size);
    	end = now();
    	
    	if (byte_read == -1){
    	    printf("Fail to read from: %s\n",filename);
    	    exit(0);
    	}
	*time = end-start;
    	int SIZE = buf_size/sizeof(int);
    	unsigned int xor1 = xorbuf(buf, SIZE / 2);
    	unsigned int xor2 = xorbuf(buf + SIZE / 2, SIZE / 2);
    	//printf("xor result is %08x\n",xor1^xor2);
    	*result= xor1^xor2;
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
    
    long block_ct = 1;
    double time;
    unsigned int result;
    newFile(filename,block_ct*block_size);
    readFile(filename,block_ct*block_size,&time,&result);
    printf("reading time is %f seconds, block count is %ld\n",time/1000.0,block_ct);
    while(time < 5000.0){
    	block_ct *=2;
    	newFile(filename,block_ct*block_size);
    	readFile(filename,block_ct*block_size,&time,&result);
    	printf("reading time is %f seconds, block count is %ld\n",time/1000.0,block_ct);
    }
    
    while(time > 15000.0){
    	block_ct *=2;
    	block_ct /=3;
    	newFile(filename,block_ct*block_size);
    	readFile(filename,block_ct*block_size,&time,&result);
    	printf("reading time is %f seconds, block count is %ld\n",time/1000.0,block_ct);
    }
    
    
    double MiB = block_ct*block_size / pow(2.0,20.0);
    double seconds = time /1000.0;
    //printf("%f MiB %f seconds\n",MiB,seconds);
    printf("xor result is %08x\n",result);
    printf("size of file is %ld bytes\n",block_ct*block_size);
    printf("reading speed is %f MiB/s\n",MiB/seconds);
    
    
   return 0;
}
