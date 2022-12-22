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
    	printf("Check usage : ./run <filename> <csv><block_size> <block_count>\n");
        return 0;
    }
    //1- filename 2-csv 3- block size 4- block count
    if(!isNumber(argv[3]) || !isNumber(argv[4])){
    	printf("Check usage : ./run <filename> <csv><block_size> <block_count>\n");
    	return 0;
    }
    long block_size = atoi(argv[3]);
    long block_ct = atoi(argv[4]);
    long buf_size = block_size * block_ct; // file size in bytes
    if(buf_size % 4 != 0){
    	buf_size = buf_size - (buf_size %4); 
    }
    
    double start,end;
    
    FILE* f = fopen(argv[2],"a+");
    
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
    	//printf("%ld bytes read in %f milliseconds\n",byte_read,end-start);
	
    	int SIZE = buf_size/sizeof(int);
    	unsigned int xor1 = xorbuf(buf, SIZE / 2);
    	unsigned int xor2 = xorbuf(buf + SIZE / 2, SIZE / 2);
    	printf("xor result is %08x\n",xor1^xor2);
    	
    	double MiB = byte_read / pow(2.0,20.0);
    	double seconds = (end - start) /1000.0;
    	//printf("%f MiB %f seconds\n",MiB,seconds);
    	printf("reading speed is %f MiB/s\n",MiB/seconds);
    	fprintf(f,"%ld,%f,",block_size,MiB/seconds);
        free(buf);
    	close(fd);
    	
    	fd = open(argv[1],O_RDONLY);
    	if(fd == -1){
    	    printf("Fail to open file: %s\n",argv[1]);
    	    return 0;
    	}
    	buf= malloc(buf_size);
    	start = now();
    	byte_read = read(fd,buf,buf_size);
    	end = now();
    	
    	if (byte_read == -1){
    	    printf("Fail to read from: %s\n",argv[1]);
    	    return 0;
    	}
    	//printf("%ld bytes read in %f milliseconds\n",byte_read,end-start);
	
    	SIZE = buf_size/sizeof(int);
    	xor1 = xorbuf(buf, SIZE / 2);
    	xor2 = xorbuf(buf + SIZE / 2, SIZE / 2);
    	printf("xor result is %08x\n",xor1^xor2);
    	
    	MiB = byte_read / pow(2.0,20.0);
    	seconds = (end - start) /1000.0;
    	//printf("%f MiB %f seconds\n",MiB,seconds);
    	printf("reading speed is %f MiB/s\n",MiB/seconds);
    	fprintf(f,"%f\n",MiB/seconds);
        free(buf);
    	close(fd);
    	
    	
    	fclose(f);
    
    


   return 0;
}
