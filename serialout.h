extern int serial_init(void);
extern int sendmessage(unsigned char *data);
extern int sendmessage_size(unsigned char *data, int size);
extern int fd;
extern void waittrans();

