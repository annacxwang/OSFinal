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

void readFile(char* filename,long block_size,long block_ct,double* time, unsigned int* result, double* performance){
	double start,end;
	long buf_size = block_size*block_ct;
	int fd = open(filename,O_RDONLY);
    	if(fd == -1){
    	    printf("Fail to open file: %s\n",filename);
    	    exit(0);
    	}
    	unsigned int* buf= (unsigned int*) malloc(buf_size);
    	start = now();
    	long byte_read;
    	long total_read = 0;
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
    	
    	printf("%ld bytes read in %f milliseconds\n",total_read,end-start);
    	*performance = MiB/seconds;
    	printf("reading speed is %f MiB/s\n",*performance);
        free(buf);
    	close(fd);
	
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
    if(block_size % 4 != 0){
    	printf("Block size needs to be multiple of 4!\n");
    	return 0;
    }
    long block_ct = atoi(argv[4]);
    long buf_size = block_size * block_ct; // file size in bytes

    double time = 0.0;
    double performance;
    unsigned int result;
    FILE* f = fopen(argv[2],"a+");
    readFile(argv[1],block_size,block_ct,&time,&result,&performance); 
    fprintf(f,"%ld,%f,",block_size,performance);
    printf("xor result is %08x\n",result);
    readFile(argv[1],block_size,block_ct,&time,&result,&performance);
    fprintf(f,"%f\n",performance);
    fclose(f);
   return 0;
}
