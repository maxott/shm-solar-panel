#include <stdio.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <errno.h>


#define DEBUG 1
#define I2C_ADDR 0x55

#define	CNTL_L	0x00
#define CNTL_H	0x01
#define SOC_L	0x02
#define SOC_H	0x03
#define RM_L	0x04
#define RM_H	0x05
#define FCC_L	0x06
#define FCC_H	0x07
#define VOLT_L	0x08
#define VOLT_H	0x09
#define AI_L	0x0A
#define AI_H	0x0B
#define TEMP_L	0x0C
#define TEMP_H	0x0D
#define FLAGS_L	0x0e
#define FLAGS_H	0x0F
#define DATE_L	0x6B
#define DATE_H	0x6C
#define NAMEL	0x6D
#define NAME	0x6E
#define CHEML	0x79
#define	CHEM	0x7A
#define SERNUM_L	0x7E
#defnie	SERNUM_H	0x7F

unsigned int foo(const char *s);
char* Read16_WriteN(int* fd, char* buf, int n);

void Control_Sub(int* fd, char * cmd, int n);
char* Control(int* fd, char *cmd, int sub);
char* scan_buf(int n);
void print_buf(char *buf);

int main (void){
	char scan[20];
	int retval;
	int fd=NULL;
	//int address = 0x55;
	char* read_buf=NULL;
	char cmd[4] = {0x00,0x01};
	if ((fd = open("/dev/i2c-1", O_RDWR)) < 0) {        // Open port for reading and writing
                printf("Failed to open i2c port\n");
                printf("FD: %i \n",fd);
                exit(1);
        }
	if (ioctl(fd, I2C_SLAVE, I2C_ADDR) < 0) {        // Set the port options and set the address of the device we wish to speak to
		printf("Address Set failed\n");
		exit(1);
	}
	Control(&fd, scan_buf(4), 1);
/**
	printf("Lower Command Byte: ");
	scanf("%s", scan);
	cmd[0]=foo(scan);
	printf("Higher Command Byte: ");
        scanf("%s", scan);
        cmd[1]=foo(scan);

	read_buf= Read16_WriteN(&fd, cmd, 2);
	printf("Read16: LSB= 0x%X MSB=0x%X\n", read_buf[0], read_buf[1]);


	printf("Lower Command Byte: ");
        scanf("%s", scan);
        cmd[0]=foo(scan);
        printf("Higher Command Byte: ");
        scanf("%s", scan);
        cmd[1]=foo(scan);
	printf("Lower Command Byte: ");
        scanf("%s", scan);
        cmd[2]=foo(scan);
        printf("Higher Command Byte: ");
        scanf("%s", scan);
        cmd[3]=foo(scan);

        read_buf= Read16_WriteN(&fd, cmd, 4);

	printf("Read16: LSB= 0x%X MSB=0x%X\n", read_buf[0], read_buf[1]);
**/
	close(fd);
	return 0;
}

char* Read16_WriteN(int* fd, char* buf, int n){
	char rv[2];
	int retval = NULL;
	//printf("CMD: %X %X\n", buf[0], buf[1]);
	if ((retval = write(*fd, buf, n)) != n) {        //STANDRD io                        // Send register to $
                printf("Error writing to i2c slave retval %d\n",retval);
        }
	if (read(*fd, rv, 2) != 2) {                        // Read back data into buf[]
                printf("Unable to read from slave\n");
                exit(1);
        }
	//printf("Read16: LSB= 0x%X MSB= 0x%X\n", rv[0], rv[1]);
	return rv;
}

char* scan_buf(int n){
	char rv[n];
	char scan[5];
	int i;
	for(i = 0; i<n; i++){
		printf("Byte %d: ", i);
		scanf("%s", scan);
		rv[i]=foo(scan);
	}
	return rv;
}

void print_buf(char *buf){
	
	printf("Buf: LSB= 0x%X MSB=0x%X\n", buf[0], buf[1]);
}

unsigned int foo(const char * s) {
	unsigned int result = 0;
 	int c ;
 	if ('0' == *s && 'x' == *(s+1)) { s+=2;
  		while (*s) {
   			result = result << 4;
   			if (c=(*s-'0'),(c>=0 && c <=9)) result|=c;
   			else if (c=(*s-'A'),(c>=0 && c <=5)) result|=(c+10);
   			else if (c=(*s-'a'),(c>=0 && c <=5)) result|=(c+10);
   			else break;
   			++s;
  		}
 	}
 	return result;
}

char* Control(int* fd, char *cmd, int sub){
	char rv[2];
	char *prv;
	//printf("CMD: %X %X\n", cmd[0], cmd[1]);
	
	if(sub == 1){	//Subcommand
		Control_Sub(fd, cmd, 4);
		prv = Control(fd, cmd, 0);
		rv[0] = prv[0];
		rv[1] = prv[1];
		/**
		if (read(*fd, rv, 2) != 2) {                        // Read back data into cmd[]
			printf("Unable to read from slave\n");
			exit(1);
		}
		printf("RV: %X %X\n", rv[0], rv[1]);
		**/
	}else{
		Control_Sub(fd, cmd, 2);
		if (read(*fd, rv, 2) != 2) {                        // Read back data into cmd[]
			printf("Unable to read from slave\n");
			exit(1);
		}
		printf("RV: %X %X\n", rv[0], rv[1]);
	}
	return rv;
}

void Control_Sub(int* fd, char * cmd, int n){
	int retval;
	if ((retval = write(*fd, cmd, n)) != n) {        //STANDRD io                        // Send register to $
        printf("Error writing to i2c slave retval %d\n",retval);
    }
}
