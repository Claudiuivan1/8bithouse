#include <errno.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

// Initial setting of the serial
int serial_set_interface_attribs(int fd, int speed, int parity) {
	struct termios tty;
	memset (&tty, 0, sizeof tty);
	
	if (tcgetattr (fd, &tty) != 0) {
		printf ("error %d from tcgetattr", errno);
		return -1;
	}
	
	switch (speed){
		case 19200:
		speed=B19200;
		break;
	case 57600:
		speed=B57600;
		break;
	case 115200:
		speed=B115200;
		break;
	default:
		printf("Cannot set baudrate %d\n", speed);
		return -1;
	}
	
	cfsetospeed (&tty, speed);
	cfsetispeed (&tty, speed);
	cfmakeraw(&tty);
	
	// Enable reading
	tty.c_cflag &= ~(PARENB | PARODD);               // shut off parity
	tty.c_cflag |= parity;
	tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;      // 8-bit chars

	if (tcsetattr (fd, TCSANOW, &tty) != 0) {
		printf ("Error %d from tcsetattr", errno);
		return -1;
	}
	return 0;
}


void serial_set_blocking(int fd, int should_block) {
	struct termios tty;
	memset (&tty, 0, sizeof tty);
	
	if (tcgetattr (fd, &tty) != 0) {
		printf ("Error %d from tggetattr", errno);
		return;
	}

	tty.c_cc[VMIN]  = should_block ? 1 : 0;
	tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

	if (tcsetattr (fd, TCSANOW, &tty) != 0)
		printf ("Error %d setting term attributes", errno);
}

// Open serial to start the communication
int serial_open(const char* name) {
	int fd = open (name, O_RDWR | O_NOCTTY | O_SYNC );
	
	if (fd < 0)
		printf ("error %d opening serial, fd %d\n", errno, fd);
	return fd;
}
