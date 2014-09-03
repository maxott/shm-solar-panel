#include <stdio.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdlib.h>
//#include <unistd.h>
#include "FuelGauge.h"
//#include <time.h>
#include "tms_ieee_conv.h"

/*
Compiler GCC 
sudo gcc FuelGauge.c -o fuelgauge

Run with ./fuelgauge
*/


int fd=0;		//Filepointer to I2C-Bus, fuelgauge is set in main-program, fd = Fuelgauge Directory

int main (void){
	tDat16 dat;
	unsigned char scan[20];
	unsigned char buf[4];
	int retval;
	//int address = 0x55;
	//char buf[2];
	unsigned char cmd[4] = {0x00,0x01};
	unsigned char sha1[20] = { KEY_SHA_1 };
	int i;
	float f = 0.47095;
	unsigned char* rv;
	float* pf = &f;
	float2bytes f2b;
	if ((fd = open("/dev/i2c-1", O_RDWR)) < 0) {        // Open port for reading and writing
                printf("Failed to open i2c port\n");
                printf("FD: %i \n",fd);
                exit(1);
	}
	
	if (ioctl(fd, I2C_SLAVE, I2C_ADDR) < 0) {        // Set the port options and set the address of the device we wish to speak to
		printf("Address Set failed\n");
		exit(1);
	}
	//Interface
	system ("clear");
	do{
		i = PrintInterface();
		switch(i){
			case 0:
				//Exit
				break;
			case 1:
				PrintStatus();
				break;
			case 2:
				Configuration();
				break;
			case 3:
				
				break;
			case 4:
				
				break;
			case 5:
				FG_BatReadTemp();
				PrintCurrent();
				printf("Average Current:\t");
				CommandPrint( AI );
				printf("Voltage:\t\t");
				CommandPrint( VOLT );
				printf("Remaining Capacity:\t");
				CommandPrint( RM );
				break;
			case 6:
				/*rv = DFtoEVSW(CC_GAIN);
				PrintFloatLittleEndian(rv);
				free(rv);
				rv = DFtoEVSW(READ_UINT16);
				printf("%02X %02X\n", rv[0], rv[1]);
				free(rv);
				ConfigData(104, 0, NULL , 4);
				*/
				//printf("Serial Number: ");
				//CommandPrint(SERNUM);
				//SetVOLTSEL(1);
				//rv = DFtoEVSW( HEX_READ_UINT16 );
				//printf("%02X %02X\n", rv[0], rv[1]);
				/*printf("Number of Series Cells: ");
				rv = DFtoEVSW( READ_UINT16 );
				ConfigData(48, 15, rv, 2);
				free(rv);
				*/
				Sealed2Unsealed();
				Unsealded2FullAccess();
				SetVOLTSEL(1);
				break;
			default:
				printf("Wrong Input\n");
		}
	}while(i!=0);
	
	//Seal();
	close(fd);
	return 0;
}

int PrintInterface(){
	int i = 0;
	
	printf("1: Print Status\n");
	printf("2: Run Configuration\n");
	printf("3: Start Calibration Circle\n");
	printf("4: Stop Calibration Circle\n");
	printf("5: Show Battery Voltage and Current\n");
	printf("6: Debug\n");
	printf("0: Exit\n");
	scanf("%d", &i);
	return i;
}

void Configuration(){
	unsigned int i;
	unsigned char buf[2];
	unsigned char* rv=NULL;
	
	Sealed2Unsealed();
	Unsealded2FullAccess();
	
	PrintStatus();
	
	//Design Capacity
	printf("Design Capacity ");
	rv = DFtoEVSW( READ_INT16 );
	ConfigData(48, 21, rv, 2);
	free(rv);
	
	//Design Energy
	printf("Design Energy ");
	rv = DFtoEVSW( READ_INT16 );
	ConfigData(48, 23, rv, 2);
	free(rv);
	
	//Cell Charge Voltage
	printf("Cell Charge Voltage t1-t2 ");
	rv = DFtoEVSW( READ_UINT16 );
	ConfigData(48, 28, rv, 2);
	free(rv);
	printf("Cell Charge Voltage t3-t4 ");
	rv = DFtoEVSW( READ_UINT16 );
	ConfigData(48, 30, rv, 2);
	free(rv);
	printf("Cell Charge Voltage t5-t6 ");
	rv = DFtoEVSW( READ_UINT16 );
	ConfigData(48, 32, rv, 2);
	free(rv);
	
	
	//Number of Series Cells
	printf("Number of Series Cells: ");
	rv = DFtoEVSW( READ_UINT16 );
	ConfigData(48, 15, rv, 2);
	i = rv[1];
	free(rv);
	
	if(i>1){
		//Multicell => VOLTSEL set!!
		SetVOLTSEL(1);
	}
	

	//Voltage Divider
	printf("Voltage Divider (Max Voltage): ");
	rv = DFtoEVSW( READ_INT16 );
	ConfigData(104, 14, rv , 2);
	free(rv);
	
	//Sense Resistor
	printf("Sense Resistor\n");
	rv = DFtoEVSW( CC_GAIN );
	ConfigData(104, 0, rv, 4);
	free(rv);
	rv = DFtoEVSW( CC_DELTA );
	ConfigData(104, 4, rv, 4);
	free(rv);
	
	//Load Select
	printf("Load Select (0=Constant Current, 1 = Constant Power): ");
	rv = DFtoEVSW( READ_UINT8 );
	ConfigData(80, 0, rv, 1);
	
	//Load Mode
	printf("Load Mode (Depends on Load Select): ");
	rv = DFtoEVSW( READ_UINT8 );
	ConfigData(80, 1, rv, 1);
	free(rv);
	
	//Cell Terminate Voltage
	printf("Terminate Voltage single cell: ");
	rv = DFtoEVSW( READ_INT16 );
	ConfigData(80, 67, rv, 2);
	free(rv);
	
	//Quit Current
	printf("Quit Current (mA): ");
	rv = DFtoEVSW( READ_INT16 );
	ConfigData(81, 4, rv, 2);
	free(rv);
	
	//QMax Cell 0
	printf("QMax Cell: ");
	rv = DFtoEVSW( READ_INT16 );
	ConfigData(82, 0, rv, 2);
	free(rv);
	
	//Checmical ID
	printf("Chemical ID: ");
	rv = DFtoEVSW( HEX_READ_UINT16 );
	ConfigData(83, 0, rv, 2);
	free(rv);
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
	int16tobytes i2b;
	uint16tobytes ui2b;
	float2bytes f2b;
	uint8tobytes ui82b;
	int i;
	int day, month, year;
	float f;
	unsigned char *rv=NULL;
	uint32tobytes ui322b;
	
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
			ui322b.i = ieee_to_tms(f);
			printf("FloatinTI: %X\n",ieee_to_tms(f));
			rv[0] = (ui322b.b[3]>>1);
			rv[1] = ui322b.b[2];
			rv[2] = ui322b.b[1];
			rv[3] = ui322b.b[0];
			
			break;
		case CC_DELTA:
			printf("CC Delta: ");
			scanf("%f", &f);
			f =5677445/f; 
			
			rv = malloc(4*sizeof(unsigned char));
			ui322b.i = ieee_to_tms(f);
			printf("FloatinTI: %X\n",ieee_to_tms(f));
			rv[0] = (ui322b.b[3]>>1);
			rv[1] = ui322b.b[2];
			rv[2] = ui322b.b[1];
			rv[3] = ui322b.b[0];
			
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
		case READ_INT16:
			scanf("%"SCNd16, &i2b.i);
			rv = malloc(2*sizeof(unsigned char));
			rv[0] = i2b.b[1];
			rv[1] = i2b.b[0];
			break;
		case READ_UINT16:
			//printf("Unsigned Int ");
			scanf("%"SCNu16, &ui2b.i);
			rv = malloc(2*sizeof(unsigned char));
			rv[0] = ui2b.b[1];
			rv[1] = ui2b.b[0];
			break;
		case READ_UINT8:
			scanf("%"SCNu8, &ui82b.i);
			rv = malloc(sizeof(unsigned char));
			rv[0] = ui82b.b[0];
			break;
		case HEX_READ_UINT16:
			scanf("%hx", &i2b.i);
			rv = malloc(2*sizeof(unsigned char));
			rv[0] = i2b.b[1];
			rv[1] = i2b.b[0];
			break;
		default:	
			printf("Alert!!\n");
			break;
		}
	return rv;
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

void PrintCurrent(){
	tDat16 dat;
	int16tobytes i2b;
	Control( CURRENT );
	Reg16Read( CNTL, &dat );
	//printf("%X %X\n", dat.dat_high, dat.dat_low);
	i2b.b[1] = dat.dat_high;
	i2b.b[0] = dat.dat_low;
	printf("Current mA:\t\t%"PRId16"\n", i2b.i);
	
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

void CommandPrint( tReg16 cmd ){
	tDat16 dest;
	int16tobytes i2b;
	Reg16Read(cmd, &dest);
	i2b.b[1] = dest.dat_low;
	i2b.b[0] = dest.dat_high;
	printf("%"PRId16"\n", i2b.i);
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

void Reg16Read( tReg16 reg, tDat16 *dest){
	//Read from lower reg
	I2C_Write(&reg.reg_low, 1);
	//usleep(50);
	I2C_Read(1, &((*dest).dat_low));
	
	//Read from higher reg
	I2C_Write(&reg.reg_high, 1);
	//usleep(50);
	I2C_Read(1, &((*dest).dat_high));
}

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
	printf("Temerature:\t\t%4.1f\n", temp_c);
	return temp_c;
}

unsigned char CalculateChecksum( unsigned char* ar, unsigned char len){
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
}

void PrintBlock(char* block, unsigned char len){
	int i;
	printf("Block_Data: ");
	for(i=0; i<len; i++){
		printf("%02X ", block[i]);
	}
	printf("\n");
}

void SetVOLTSEL(unsigned char i){
	char buf[2];
	/*char* buf;
	char writetoconf[2];
	buf = ReadConfData(64, 0);
	//PrintBlock(buf, BLOCK_LEN);
	//set highbyte bit 3 = VOLTSEL
	printf("%02X %02X\n", buf[0], buf[1]);
	if(i > 0){
		buf[0] = buf[0] | 0x08;
	}else{
		buf[0] = buf[0] & 0xF7;
	}
	printf("%02X %02X\n", buf[0], buf[1]);
	writetoconf[0] = buf[0];
	writetoconf[1] = buf[1];
	//PrintBlock(buf, BLOCK_LEN);
	ConfigData(64, 0, NULL, 2);
	ConfigData(64, 0, writetoconf, 2);
	//PrintBlock(buf, BLOCK_LEN);
	free(buf);
	*/
	buf[0] = 0x09;
	buf[1] = 0x61;
	ConfigData(64, 0, buf, 2);
	
}

char* ReadConfData(unsigned char subclass, unsigned char offset){
	unsigned char buf[2];
	unsigned char mod_offset;
	unsigned char block_offset;
	char* data_block;
	//unsigned char checksum = 0;
	//int i = 0;
	
	//float* f;
	
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
	//printf("Subclass: %02X\n", subclass);
	//printf("Modulo Offset: %02X\n", mod_offset);
	//printf("Block Offset: %02X\n", block_offset);
	buf[0] = DFBLK;
	buf[1] = block_offset;
	I2C_Write(buf, 2);	
	
	//4: Readblock
	data_block = ReadBlock(A_DF, BLOCK_LEN);
	
	//printf("Before modify:\n");
	//PrintBlock(data_block, BLOCK_LEN);
	//PrintBlock(& (data_block[mod_offset]), data_len);

	return data_block;
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
	//printf("Subclass: %02X\n", subclass);
	//printf("Modulo Offset: %02X\n", mod_offset);
	//printf("Block Offset: %02X\n", block_offset);
	buf[0] = DFBLK;
	buf[1] = block_offset;
	I2C_Write(buf, 2);	
	
	//4: Readblock
	data_block = ReadBlock(A_DF, BLOCK_LEN);
	
	printf("Before modify:\n");
	//PrintBlock(data_block, BLOCK_LEN);
	PrintBlock(& (data_block[mod_offset]), data_len);
	
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
		//printf("Checksum: %02X\n", checksum);
		printf("After modify:\n");
		//PrintBlock(data_block, BLOCK_LEN);
		PrintBlock(& (data_block[mod_offset]), data_len);
	}
	//usleep(10000);
	free(data_block);
}

/****************************************************************/
/* ieee_to_tms (texas instruments float)
/*
/* This function takes a C float number and converts it to a
/* hexadecimal C3x floating point value.
/*
/* By: Scott Morrison, morrisos@ufl.edu, 10 July 2002.
/****************************************************************/
uint32 ieee_to_tms(const float floatIn)       /* CONVERSION FROM IEEE FORMAT TO TMS320C30 FORMAT */
{

      ieee.flt = floatIn;

        /* Refer to page 5-15 in the TMS320C3x User's Guide for the */
   /* following case numbers of IEEE-->C3X conversion. */

    if (ieee.str.exponent == 0)
        {
                /* Case 6 -- ZERO */
               return(0x80000000);
     }
     else if (ieee.str.exponent == 255 && ieee.str.sign == 1)
      {
              /* Case 1 -- Maximum Negative Infinity */
            return(0x7F800000);
  }
  else if (ieee.str.exponent == 255 && ieee.str.sign == 0)
   {
           /* Case 2 -- Maximum Positive Infinity */
         return(0x7F7FFFFF);
       }
       else
    {
            if (ieee.str.sign == 0)
            {
                    /* Case 3 */
                       c30.str.exponent = ieee.str.exponent - 127;
                     c30.str.sign = 0;
                 c30.str.mantissa = ieee.str.mantissa;
                 return(c30.hex);
          }
          else if (ieee.str.mantissa != 0)
           {
                   /* Case 4 */
                      c30.str.exponent = ieee.str.exponent - 127;
                    c30.str.sign = 1;
                        c30.str.mantissa = ~(ieee.str.mantissa)+1;
                   return(c30.hex);
            }
            else
         {
                 /* Case 5 */
                    c30.str.exponent = ieee.str.exponent - 128;
                  c30.str.sign = 1;
                      c30.str.mantissa = 0;
                      return(c30.hex);
               }
       }
}

/****************************************************************/
/* tms_to_ieee
/*
/* This function takes a C3x Texas Instruments floating point
/* hexadecimal number and converts it to an IEEE floating point
/* value, returning a C-usable float.
/*
/* By: Scott Morrison, morrisos@ufl.edu, 10 July 2002.
/****************************************************************/
float tms_to_ieee(const uint32 c30in)       /*CONVERSION FROM TMS320C30 TO IEEE FORMAT*/
{

    c30.hex = c30in;

     /* Refer to page 5-15 in the TMS320C3x User's Guide for the */
        /* following case numbers of C3X-->IEEE conversion. */

 if (c30.hex == 0x80000000)
      {
              /* Case 6 -- Zero */
             return(0.0);
  }
  else if (c30.str.exponent == 0x7F)
 {
         /* Case 1 and 2 -- Max Pos/Neg Infinity */
          ieee.str.exponent = 255;
               ieee.str.mantissa = 0;      /* don't care */
             ieee.str.sign = c30.str.sign; /* signs are same for +/- infinity */
               return(ieee.flt);
       }
       else
    {
            ieee.str.sign = c30.str.sign;    /* signs will be same from here on */
                if (c30.str.sign == 0)
         {
                 /* Case 3 */
                    ieee.str.mantissa = c30.str.mantissa;
                    ieee.str.exponent = c30.str.exponent + 0x7F;
                 return(ieee.flt);
         }
         else
              {
                      if (c30.str.mantissa == 0)
                   {
                           /* Case 5 */
                              ieee.str.mantissa = 0;
                             ieee.str.exponent = c30.str.exponent + 0x80;
                          return(ieee.flt);
                  }
                  else
                       {
                               /* Case 4 */
                          ieee.str.mantissa = ~(c30.str.mantissa + 1);
                               ieee.str.exponent = c30.str.exponent + 0x7F;
                            return(ieee.flt);
                    }
            }
    }
}