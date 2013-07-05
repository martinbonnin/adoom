#include <stdio.h>
#include <fcntl.h>
#include <errno.h>

int main( int argc, unsigned char *argv[])
{
	int fd;
	int out;

	fd = open("/dev/tty", O_RDONLY);
	if( fd == -1 ){
printf("cannot open in %s\n", strerror(errno));
	}

	for ( ; ; ) {
		char c;
		int err = read(fd, &c, 1);
		if( err ){
			printf("got %d: c = %d\n", err, c);
		}
	}

	close(fd);
}