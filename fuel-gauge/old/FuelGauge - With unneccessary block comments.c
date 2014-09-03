#include <stdio.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>
//#include <unistd.h>
#include "FuelGauge.h"
//#include <time.h>

/*
Compiler GCC 
sudo gcc FuelGauge.c -o fuelgauge

Run with ./fuelgauge
*/


int fd=NULL;

int main (void){
	tDat16 dat;
	unsigned char scan[20];
	int retval;
	//int address = 0x55;
	char buf[2];
	unsigned char cmd[4] = {0x00,0x01};
	unsigned char sha1[20] = { KEY_SHA_1 };
	int i;
	float f = 0.47095;
	unsigned char* rv;
	float* pf = &f;
	if ((fd = open("/dev/i2c-1", O_RDWR)) < 0) {        // Open port for reading and writing
                printf("Failed to open i2c port\n");
                printf("FD: %i \n",fd);
                exit(1);
	}
	
	if (ioctl(fd, I2C_SLAVE, I2C_ADDR) < 0) {        // Set the port options and set the address of the device we wish to speak to
		printf("Address Set failed\n");
		exit(1);
	}
	
	
	
	//printf("%02X %02X %02X %02X\n", scan[0], scan[1], scan[2], scan[3]);
	

	/*f = 5.595e5;
	printf("%02X %02X %02X %02X\n", ((unsigned char*) pf)[3], ((unsigned char*) pf)[2], ((unsigned char*) pf)[1], ((unsigned char*) pf)[0]);
	f = 0.47095;
	printf("%02X %02X %02X %02X\n", ((unsigned char*) pf)[3], ((unsigned char*) pf)[2], ((unsigned char*) pf)[1], ((unsigned char*) pf)[0]);
	*/
	rv = DFtoEVSW(CC_GAIN);
	PrintFloatLittleEndian(rv);
	free(rv);
	//printf("%02X %02X %02X %02X\n", rv[0], rv[1], rv[2], rv[3]);
	rv = DFtoEVSW(CC_DELTA);
	//printf("%02X %02X %02X %02X\n", rv[0], rv[1], rv[2], rv[3]);
	PrintFloatLittleEndian(rv);
	free(rv);
	
	/*((unsigned char*) pf)[0]=0xC0;
	((unsigned char*) pf)[1]=0x98;
	((unsigned char*) pf)[2]=0x08;
	((unsigned char*) pf)[3]=0x49;
	printf("%f\n", f);
	*/
	//FG_SetDesignCapacity(7200);
	
	//Control(CONTROL_STATUS);
	
	//dat = Reg16Read( CNTL );
	//printf("%X %X\n", dat.dat_high, dat.dat_low);
	
	//dat = Reg16Read( CNTL );
	//printf("%X %X\n", dat.dat_high, dat.dat_low);
	

	//Sealed2Unsealed();
	//Unsealded2FullAccess();
	
	PrintStatus();
	
	FG_BatReadTemp();
	//Configuration();
	
	
	//buf[0] = 0xff;
	//buf[1] = 0xaa;
	//ConfigData(104, 14, NULL , 2);
	ConfigData(104, 0, NULL , 4);
	
	//Seal();
	close(fd);
	return 0;
}

int PrintInterface(){
	int i = 0;
	
	return i;
}

void Configuration(){
	unsigned int i;
	unsigned char buf[2];
	Sealed2Unsealed();
	Unsealded2FullAccess();
	
	PrintStatus();
	
	printf("Voltage Divider: ");
	ScanIntCharOut(buf);
	//printf("Buf: %02X %02X\n", buf[0], buf[1]);
	ConfigData(104, 14, buf , 2);
	
	//Seal();
}

void ScanIntCharOut(unsigned char* output){
	int i;
	scanf("%u", &i);
	output[0] = (i & 0xFF00)>>8;
	output[1] = i & 0xFF;
}

void PrintFloatLittleEndian(char* float_){
	float2bytes f2b;
	f2b.b[0] = float_[3];
	f2b.b[1] = float_[2];
	f2b.b[2] = float_[1];
	f2b.b[3] = float_[0];
	printf("float: %f\n", f2b.f);
}

unsigned char* DFtoEVSW(char switch_val){
	
	float2bytes f2b;
	int i;
	int day, month, year;
	float f;
	unsigned char *rv=NULL;
	switch(switch_val){
		case MAN_DATE:
			printf("Manufacture Date\n");
			printf("Day: ");
			scanf("%d", &day); 
			printf("Month: ");
			scanf("%d", &month);
			printf("Year: ");
			scanf("%d", &year);
			i = day + month*32+(year-1980)*256;
			rv = malloc(2*sizeof(unsigned char));
			rv[0] = (i & 0xFF00)>>8;
			rv[1] = i & 0xFF;
			break;
		case CC_GAIN:
			printf("CC Gain: ");
			scanf("%f", &f);
			f =4.768/f; 
			rv = malloc(4 * sizeof(unsigned char));
			f2b.f = f;
			
			rv[0] = f2b.b[3];
			rv[1] = f2b.b[2];
			rv[2] = f2b.b[1];
			rv[3] = f2b.b[0];
			
			break;
		case CC_DELTA:
			printf("CC Delta: ");
			scanf("%f", &f);
			f =5677445/f; 
			
			rv = malloc(4*sizeof(unsigned char));
			f2b.f = f;
			
			rv[0] = f2b.b[3];
			rv[1] = f2b.b[2];
			rv[2] = f2b.b[1];
			rv[3] = f2b.b[0];
			
			break;
		case USER_RATE:
			printf("User Rate: ");
			scanf("%d", & i);
			i = i/10;
			rv = malloc(2*sizeof(unsigned char));
			rv[0] = (i & 0xFF00)>>8;
			rv[1] = i & 0xFF;
			break;
		case RESERVE_CAP:
			printf("Reseve Cap: ");
			scanf("%d", & i);
			i = i/10;
			rv = malloc(2*sizeof(unsigned char));
			rv[0] = (i & 0xFF00)>>8;
			rv[1] = i & 0xFF;
			break;
		default:
			printf("Alert!!\n");
			break;
		}
	return rv;
}

//Doesn't Work!!
void FG_SetDesignCapacity(unsigned int mAh){
	tDat16 TmAh;
	TmAh.dat_low = LOW_BYTE(mAh);
	TmAh.dat_high = HIGH_BYTE(mAh);
	Write2Reg16( DCAP , TmAh );
}

void PrintStatus(){
	tDat16 dat;
	Control( CONTROL_STATUS );
	Reg16Read( CNTL, &dat );
	//printf("%X %X\n", dat.dat_high, dat.dat_low);
	if(dat.dat_high&0x20){
		printf("Sealed\n");
	}else if(dat.dat_high&0x40){
		printf("Full Access Sealed\n");
	} else {
		printf("Unsealed\n");
	}
	
	if(dat.dat_high & 0x10){
		printf("CALMOD\t");
	}
	if(dat.dat_high & 0x08){
		printf("CCA\t");
	}
	if(dat.dat_high & 0x04){
		printf("BCA\t");
	}
	if(dat.dat_high & 0x02){
		printf("CSV\t");
	}
	printf("\n");
	
	if(dat.dat_low & 0x20){
		printf("FULLSLEEP\t");
	}
	if(dat.dat_low & 0x10){
		printf("SLEEP\t");
	}
	if(dat.dat_low & 0x08){
		printf("LDMD\t");
	}
	if(dat.dat_low & 0x04){
		printf("RUP_DIS\t");
	}
	if(dat.dat_low & 0x02){
		printf("VOK\t");
	}
	if(dat.dat_low & 0x01){
		printf("QEN\t");
	}
	printf("\n");
}

void Seal(){
	Control(SEALED);
}

void Sealed2Unsealed(){
	Control((tDat16){0x14, 0x04});
	Control((tDat16){0x72, 0x36});
}

void Unsealded2FullAccess(){
	Control((tDat16){0xff, 0xff});
	Control((tDat16){0xff, 0xff});
}

void Control( tDat16 dat){
	Write2Reg16( CNTL, dat);
}

void Write2Reg16( tReg16 reg, tDat16 dat){
	unsigned char buf[2];
	//write to lower register
	buf[0] = reg.reg_low;
	buf[1] = dat.dat_low;
	
	I2C_Write(buf, 2);
	
	//write to higher register
	buf[0] = reg.reg_high;
	buf[1] = dat.dat_high;
	
	I2C_Write(buf, 2);
}

/*
tDat16 Reg16Read( tReg16 reg){
	tDat16 rv;
	//Read from lower reg
	I2C_Write(&reg.reg_low, 1);
	//rv.dat_low= (I2C_Read(1))[0];
	I2C_Read(1, &rv.dat_low);
	
	
	//Read from higher reg
	
	I2C_Write(&reg.reg_high, 1);
	//rv.dat_high= (I2C_Read(1))[0];
	I2C_Read(1, &rv.dat_high);
	return rv;
}
*/
void Reg16Read( tReg16 reg, tDat16 *dest){
	//Read from lower reg
	I2C_Write(&reg.reg_low, 1);
	I2C_Read(1, &((*dest).dat_low));
	
	//Read from higher reg
	I2C_Write(&reg.reg_high, 1);
	I2C_Read(1, &((*dest).dat_high));
}
/*
char* I2C_Read(int quantity){
	char *rv;
	rv = malloc ( quantity * sizeof(unsigned char));
	if (read(fd, rv, quantity) != quantity) {                        // Read back data into buf[]
		printf("Unable to read from slave\n");
		exit(1);
	}
	return rv;
}
*/
void I2C_Read(int quantity, char* dest){
	if (read(fd, dest, quantity) != quantity) {                        // Read back data into buf[]
		printf("Unable to read from slave\n");
		exit(1);
	}
}

void I2C_Write(char* buf, int quantity){
	int retval;
	if ((retval = write(fd, buf, quantity)) != quantity) {        //STANDRD io                        // Send register to $
		printf("Error writing to i2c slave retval %d\n",retval);
	}
}

float FG_BatReadTemp(){
	tDat16 T_temp;
	float temp_c;
	Reg16Read( TEMP, &T_temp );
	temp_c = (T_temp.dat_low | (T_temp.dat_high<<8)) / 10.0 - 273.15;
	printf("%4.1f\n", temp_c);
	return temp_c;
}

unsigned char CalculateChecksum( char* ar, unsigned char len){
	unsigned char checksum = 0x00;
	unsigned char i;
	//printf("SHA1-Hash: 0x");
	for( i = 0; i< len; i++){
		//printf("%02X", ar[i]);
		checksum = checksum + ar[i];
	}
	checksum = checksum ^ 0xFF;
	//printf("\nChecksum: %02X\n", checksum);
	return checksum;
}

void Authenticate(char* arSHA1 ){
	int i = 0;
	unsigned char checksum;
	unsigned char buf[2];
	unsigned char arSHA1_result[ SHA1_LEN ];
	tDat16 dat;
	
	Control(CONTROL_STATUS);
	Reg16Read( CNTL,  &dat );
	printf("Control Status bevore: %X %X\n", dat.dat_high, dat.dat_low);
	
	checksum = CalculateChecksum(arSHA1, SHA1_LEN );
	
	//0x00 to  DataFlashBlock() = DFBLK
	
	
	//For Sealed Mode
	buf[0] = DFBLK;	//Commandbyte
	buf[1] = 0x00;
	I2C_Write(buf, 2);
	/*
	//For Unsealed Mode
	buf[0] = DFDCNTL;	//Commandbyte
	buf[1] = 0x01;
	I2C_Write(buf, 2);
	*/
	
	//Write 20 Byte Authenticate Challange
	for(i = 0; i < SHA1_LEN; i++){
		buf[0] = A_DF + i;
		buf[1] = arSHA1[i];
		I2C_Write(buf, 2);
		//printf("Write SHA1 %d \n", i);
	}
	
	//usleep(1000000);
	
	//Write Checksum
	buf[0] = ACKS;
	buf[1] = checksum;
	I2C_Write(buf, 2);
	
	/*
	//Read Checksum
	buf[0] = ACKS;
	I2C_Write(buf, 1);
	buf[1] = I2C_Read(0)[0];
	
	printf("Checksum: %02X\n", buf[1]);
	*/
	
	//Wait a sec
	
	//usleep(1000000);
	
	//Reading Authenticate Challenge
	printf("Answer: 0x");
	for(i=0; i < SHA1_LEN; i++){
		buf[0] = A_DF + i;
		I2C_Write(buf, 1);
		I2C_Read(1, &arSHA1_result[i]);
		//arSHA1_result[i] = (I2C_Read(1))[0];
		
		printf("%02X", arSHA1[i]);
	}
	printf("\n");
	
	//Compare Written Hash with Read Hash
	
	for(i=0; (i< SHA1_LEN) && (arSHA1[i] == arSHA1_result[i]); i++);
	if(i >= (SHA1_LEN-1)) printf("Key Matches\n");
	
	//Double-Check Unsealed Successfully
	Control(CONTROL_STATUS);
	Reg16Read( CNTL, &dat );
	printf("%X %X\n", dat.dat_high, dat.dat_low);
	if((dat.dat_high & 0x40)>0){
		printf("Full Access Sealed state\n");
	}
}


char* ReadBlock(unsigned char reg_scr, unsigned char len){
	char* rv;
	//int i;
	rv = malloc(len * sizeof(unsigned char));
	
	//printf("Read From Address: %02X\n", reg_scr);
	/*
	for(i = 0; i< len; i++){
		I2C_Write( &reg_scr, 1);
		I2C_Read(1, &(rv[i]));
		printf("Read From Address: %02X %02X\n", reg_scr, rv[i]);
		reg_scr = reg_scr + 1;
	}
	*/
	I2C_Write(& reg_scr, 1);
	I2C_Read(len, rv);
	//PrintBlock(rv, BLOCK_LEN);
	return rv;
}

void WriteBlock(unsigned char *block, unsigned char dest, unsigned char len){
	int i;
	//Write Block at once
	unsigned char buf[len+1];
	buf[0] = dest;
	for(i = 0; i<len; i++){
		buf[i+1] = block[i];
	}
	I2C_Write(buf, len+1);
	
	//Write Block Byte after Byte
	/*
	unsigned char buf[2];
	for(i = 0; i<len; i++){
		buf[0] = dest + i;
		buf[1] = block[i];
		I2C_Write(buf, 2);
	}
	*/
}

void PrintBlock(char* block, unsigned char len){
	int i;
	printf("Block_Data: ");
	for(i=0; i<len; i++){
		printf("%02X ", block[i]);
	}
	printf("\n");
}

void ConfigData(unsigned char subclass, unsigned char offset, char* writedata, unsigned char data_len){
	unsigned char buf[2];
	unsigned char mod_offset;
	unsigned char block_offset;
	char* data_block;
	unsigned char checksum = 0;
	int i = 0;
	
	float* f;
	
	//1: Write to BlockDataControl 0x00 
	buf[0] = DFDCNTL;
	buf[1] = 0x00;
	I2C_Write(buf, 2);
	
	//2: Set Class
	buf[0] = DFCLS;
	buf[1] = subclass;
	I2C_Write(buf, 2);
	
	//3: Set Offset
	mod_offset = offset % BLOCK_LEN;
	block_offset = offset / BLOCK_LEN;
	printf("Subclass: %02X\n", subclass);
	printf("Modulo Offset: %02X\n", mod_offset);
	printf("Block Offset: %02X\n", block_offset);
	buf[0] = DFBLK;
	buf[1] = block_offset;
	I2C_Write(buf, 2);	
	
	//4: Readblock
	data_block = ReadBlock(A_DF, BLOCK_LEN);
	
	printf("Before modify:\n");
	PrintBlock(data_block, BLOCK_LEN);
	PrintBlock(& (data_block[mod_offset]), data_len);
	
	/*
	//Debug: Read Checksum
	checksum = CalculateChecksum(data_block, BLOCK_LEN);
	buf[0] = DFDCKS;
	I2C_Write(buf, 1);
	I2C_Read(1, buf);
	printf("Calculated Checksum: %02X Real Checksum: %02X\n", checksum, buf[0]);
	
	f = (float* ) (&data_block[mod_offset]);
	printf("Float: %f\n", *f);
	*/
	//This Function reads, modifies and writes whole 32-byte-block if writedata is valid!!!
	if(writedata!=NULL){
		//5: Modify Changes
		for(i = 0; i<data_len; i++){
			data_block[mod_offset + i] = writedata[i];
		}
		//6: Write Block
		WriteBlock(data_block, A_DF, BLOCK_LEN);
		//printf("Block Written\n");
		//7: Write Checksum
		checksum = CalculateChecksum(data_block, BLOCK_LEN);
		buf[0] = DFDCKS;
		buf[1] = checksum;
		I2C_Write(buf, 2);
		
		printf("After Before modify:\n");
		PrintBlock(data_block, BLOCK_LEN);
		PrintBlock(& (data_block[mod_offset]), data_len);
	}
	
	
	
	free(data_block);
}