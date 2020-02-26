#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

int main()
{
	int fd;
    char *testt = "hello world";
	char buf[1000] = {0};
	
	
	fd = open("/dev/my_driver",O_RDWR);
    if (fd == -1) printf("file open failed\n");
	
	int w = write(fd,testt,strlen(testt));
	
	int r =  read(fd,buf,100);
	
	printf("%s\n",buf);

	
	close(fd);
	
	return 0;
}
