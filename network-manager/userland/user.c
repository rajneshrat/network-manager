#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>

#define PAGE_SIZE 4096
void dump_queue(int total_element);
void set_queue(char *data);

int main ( int argc, char **argv )
{
	int configfd;
	configfd = open("/dev/test", O_RDWR);
	if(configfd < 0) {
		perror("open");
		return -1;
	}
	char * address = NULL;
	address = mmap(NULL, PAGE_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, configfd, 0);
	if (address == MAP_FAILED) {
		perror("mmap");
		return -1;
	}
    int a = 1;
    set_queue(address);
    while(a != 0) {
        scanf("%d",&a);
        dump_queue(10);	
	}
    close(configfd);
	return 0;
}

