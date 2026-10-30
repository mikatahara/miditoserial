#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <asm/ioctls.h>
#include <termios.h>
#include "serialout.h"

struct termios2
{
    uint32_t c_iflag;
    uint32_t c_oflag;
    uint32_t c_cflag;
    uint32_t c_lflag;
    uint8_t c_line;
    uint8_t c_cc[19];
    uint32_t c_ispeed;
    uint32_t c_ospeed;
};

#define BAUD 31250
#define BOTHER 0010000
#define CSIZE 0000060
#define CS8 0000060
#define CSTOPB 0000100
#define PARENB 0000400
#define CLOCAL 0004000
#define CREAD 0000200

#define BOTHER 0010000
#define TCGETS2 _IOR('T', 0x2A, struct termios2)
#define TCSETS2 _IOW('T', 0x2B, struct termios2)

struct termios2_local
{
    uint32_t c_iflag, c_oflag, c_cflag, c_lflag;
    uint8_t c_line;
    uint8_t c_cc[19];
    uint32_t c_ispeed, c_ospeed;
};

int fd;

int serial_init(void)
{
    fd = open("/dev/ttyUSB0", O_WRONLY | O_NOCTTY);
    
    if (fd < 0)
    {
        perror("open");
        return -1;
    }

    struct termios2_local t;
    if (ioctl(fd, TCGETS2, &t) < 0)
    {
        perror("TCGETS2");
        close(fd);
        return -1;
    }

    t.c_cflag &= ~(CBAUD | CSIZE | CSTOPB | PARENB);
    t.c_cflag |= BOTHER | CS8 | CLOCAL | CREAD;
    t.c_ispeed = BAUD;
    t.c_ospeed = BAUD;

    if (ioctl(fd, TCSETS2, &t) < 0)
    {
        perror("TCSETS2");
        close(fd);
        return -1;
    }
    return 0;
}

int sendmessage(unsigned char *data)
{
    if (write(fd, data, sizeof(data)) != sizeof(data))
    {
        perror("write");
        close(fd);
        return -1;
    }
    tcdrain(fd); // 送信完了待ち
    return 0;
}


int sendmessage_size(unsigned char *data, int size)
{
    write(fd, data, size);
    return 0;

/*    if (write(fd, data, size) != size)
    {
        perror("write");
        close(fd);
        return -1;
    }
    tcdrain(fd); // 送信完了待ち
    return 0;*/
}

void waittrans()
{
    tcdrain(fd);
}

#if 0
int main()
{
    serial_init();
    unsigned char noteon[3]={0x90,0x60, 0x7F};
    unsigned char noteoff[3]={0x80,0x60, 0x7F};
    
    while(1){
        sendmessage(noteon);
        sleep(1);
        sendmessage(noteoff);
        sleep(1);
    }
}
#endif
    