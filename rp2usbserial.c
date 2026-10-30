#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <asm/ioctls.h>
#include <termios.h>

struct termios2 {
    uint32_t c_iflag;
    uint32_t c_oflag;
    uint32_t c_cflag;
    uint32_t c_lflag;
    uint8_t  c_line;
    uint8_t  c_cc[19];
    uint32_t c_ispeed;
    uint32_t c_ospeed;
};

#define BAUD 	31250
#define BOTHER  0010000
//#define CBAUD   0010017
#define CSIZE   0000060
#define CS8     0000060
#define CSTOPB  0000100
#define PARENB  0000400
#define CLOCAL  0004000
#define CREAD   0000200

#define BOTHER  0010000
#define TCGETS2 _IOR('T', 0x2A, struct termios2)
#define TCSETS2 _IOW('T', 0x2B, struct termios2)

struct termios2_local {
    uint32_t c_iflag, c_oflag, c_cflag, c_lflag;
    uint8_t c_line;
    uint8_t c_cc[19];
    uint32_t c_ispeed, c_ospeed;
};

int main(void)
{
    int fd = open("/dev/ttyUSB0", O_WRONLY | O_NOCTTY);
    if (fd < 0) {
        perror("open");
        return 1;
    }

    struct termios2_local t;
    if (ioctl(fd, TCGETS2, &t) < 0) {
        perror("TCGETS2");
        close(fd);
        return 1;
    }

    t.c_cflag &= ~(CBAUD | CSIZE | CSTOPB | PARENB);
    t.c_cflag |= BOTHER | CS8 | CLOCAL | CREAD;
    t.c_ispeed = BAUD;
    t.c_ospeed = BAUD;

    if (ioctl(fd, TCSETS2, &t) < 0) {
        perror("TCSETS2");
        close(fd);
        return 1;
    }

    unsigned char data[] = {0x90, 0x3C, 0x64};

	while(1){
	data[0] = 0x90;
    if (write(fd, data, sizeof(data)) != sizeof(data)) {
        perror("write");
        close(fd);
        return 1;
    }
    // 1秒待つ
    tcdrain(fd);	//送信完了待ち
    sleep(1);

	data[0] = 0x80;
    if (write(fd, data, sizeof(data)) != sizeof(data)) {
        perror("write");
        close(fd);
        return 1;
    }
    // 1秒待つ
    tcdrain(fd);	//送信完了待ち
    sleep(1);
    }
    
    close(fd);
    return 0;
}

