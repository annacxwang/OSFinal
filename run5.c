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


double now(){
    struct timeval tv;
    gettimeofday(&tv,0);
    return tv.tv_usec/1000.0 + tv.tv_sec*1000.0;

}

void readFile(char* filename,long block_size,long block_ct,double* time){
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

    	double MiB = total_read / pow(2.0,20.0);
    	double seconds = (end - start) /1000.0;
    	*time = seconds; //time elapsed in seconds
    	
    	printf("%ld bytes read in %f milliseconds\n",total_read,end-start);
    	printf("reading speed is %f MiB/s\n",MiB/seconds);
    	printf("performance is %f B/s\n",total_read/seconds)
        free(buf);
    	close(fd);
	
}

void seekLoop();

int main(int argc,char * argv[])
{
   /* printf function displays the content that is
    * passed between the double quotes.
    */
    
    if(argc != 4){
    	printf("Check usage : ./run <filename> <csv> <file_size>\n");
        return 0;
    }
    //1- filename 2-csv 3- file_size
    if(!isNumber(argv[3]) ){
    	printf("Check usage : ./run <filename> <csv> <file_size>\n");
        return 0;
    }
    
    long file_size = atoi(argv[3]); // file size in bytes

    double time = 0.0;
    FILE* f = fopen(argv[2],"a+");
    readFile(argv[1],1,file_size,&time); 
    fprintf(f,"%ld,%f,",file_size,time);
    //
    fprintf(f,"%f,",time);
    
    fclose(f);
   return 0;
}
