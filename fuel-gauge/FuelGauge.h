#ifndef __FUELGAUGE_H
	#define __FUELGAUGE_H
	
	//For registers and how-to-program-fuelgauge see datasheet
	
	//Makros
	#define LOW_BYTE(x)     ((unsigned char)((x)&0xFF))
	#define HIGH_BYTE(x)    ((unsigned char)(((x)>>8)&0xFF))
	
	#define I2C_ADDR 0x55

	//2Byte-Commands - Standard
	#define CNTL	(tReg16){0x00,0x01}
	#define SOC		(tReg16){0x02,0x03}
	#define RM		(tReg16){0x04,0x05}
	#define FCC		(tReg16){0x06,0x07}
	#define VOLT	(tReg16){0x08,0x09}
	#define AI		(tReg16){0x0a,0x0b}
	#define TEMP	(tReg16){0x0c,0x0d}
	#define FLAGS	(tReg16){0x0e,0x0f}
	#define DATE	(tReg16){0x6B,0x6C}
	#define SERNUM	(tReg16){0x7E,0x7F}

	//1Byte-Command - Standard
	#define NAMEL	0x6D
	#define NAMEL	0x6E
	#define	CHEML	0x79
	#define CHEM	0x7A

	//CNTL-Subcommands "DATA"
	#define	CONTROL_STATUS	(tDat16) {0x00, 0x00}
	#define DEVICE_TYPE		(tDat16) {0x01, 0x00}
	#define FW_VERSION		(tDat16) {0x02, 0x00}
	#define HW_VERSION		(tDat16) {0x03, 0x00}
	#define RESET_DATA		(tDat16) {0x05, 0x00}
	#define	BOARD_OFFSET	(tDat16) {0x07, 0x00}
	#define CHEM_ID			(tDat16) {0x08, 0x00}
	#define CC_OFFSET		(tDat16) {0x0A, 0x00}
	#define CC_OFFSET_SAVE	(tDat16) {0x0B, 0x00}
	#define DF_VERSION		(tDat16) {0x0C, 0x00}
	#define SET_FULLSLEEP	(tDat16) {0x10, 0x00}
	#define STATIC_CHEM_CHKSUM		(tDat16) {0x17, 0x00}
	#define CURRENT			(tDat16) {0x18, 0x00}
	#define SEALED			(tDat16) {0x20, 0x00}
	#define IT_ENABLE		(tDat16) {0x21, 0x00}
	#define CAL_ENABLE		(tDat16) {0x2D, 0x00}
	#define RESET			(tDat16) {0x41, 0x00}
	#define EXIT_CAL		(tDat16) {0x80, 0x00}
	#define ENTER_CAL		(tDat16) {0x81, 0x00}
	#define OFFSET_CAL		(tDat16) {0x82, 0x00}
	
	//Extended Commands "REG"
	#define	AR 		(tReg16) {0x10, 0x11}
	#define	ARTTE	(tReg16) {0x12, 0x13}
	#define	NAC		(tReg16) {0x14, 0x15}
	#define	FAC		(tReg16) {0x16, 0x17}
	#define	TTE		(tReg16) {0x18, 0x19}
	#define	TTF		(tReg16) {0x1a, 0x1b}
	#define	SI		(tReg16) {0x1c, 0x1d}
	#define	STTE	(tReg16) {0x1e, 0x1f}
	#define	MLI		(tReg16) {0x20, 0x21}
	#define	MLTTE 	(tReg16) {0x22,0x23}
	#define	AE		(tReg16) {0x24, 0x25}
	#define	AP		(tReg16) {0x26, 0x27}
	#define	TTECP	(tReg16) {0x28, 0x29}
	#define	INTTEMP	(tReg16) {0x2a, 0x2b}
	#define	CC		(tReg16) {0x2c, 0x2d}
	#define	SOH		(tReg16) {0x2e, 0x2f}
	#define	CHGV	(tReg16) {0x30, 0x31}
	#define	CHGI	(tReg16) {0x32, 0x33}
	#define	PCHG	(tReg16) {0x34, 0x35}
	#define	DOD0	(tReg16) {0x36, 0x37}
	#define	SDSG	(tReg16) {0x38, 0x39}
	#define	PKCFG	(tReg16) {0x3a, 0x3b}
	#define	DCAP	(tReg16) {0x3c, 0x3d}
	
	//Extended Commands 1 Byte Register
	#define	DFCLS		0x3e
	#define	DFBLK		0x3f
	#define	A_DF		0x40
	#define	ACKS		0x54
	#define	DFD			0x55
	#define	DFDCKS		0x60
	#define	DFDCNTL		0x61
	#define	DNAMELEN	0x62
	#define	DNAME		0x63
	#define	RSVD		0x6a
	
	#define KEY_PLANE_TEXT	"0x0123456789ABCDEFFEDCBA987654321"
	#define KEY_SHA_1		0x0a, 0xc7, 0x7f, 0xf9, 0x89, 0xb0, 0xb8, 0x5c, 0x30, 0x28, 0x5b, 0x18, 0x2b, 0xd8, 0x5c, 0x9a, 0x5c, 0x53, 0xc3, 0x91
	#define SHA1_LEN		20
	#define BLOCK_LEN		0x20
	
	#define MAN_DATE	1
	#define CC_GAIN		2
	#define CC_DELTA	3
	#define USER_RATE	4
	#define RESERVE_CAP	5
	#define READ_INT16	7
	#define READ_UINT16	8
	#define READ_UINT8	9
	#define HEX_READ_UINT16 10

	//Union for converting float to char array
	typedef union float2byte {
		float f;
		unsigned char b[sizeof(float)];
	} float2bytes;
	//Union for converting unsigned 32bit integer to char array
	typedef union Tuint32tobyte {
		uint32_t i;
		unsigned char b[sizeof(int32_t)];
	} uint32tobytes;
	//Union for converting unsigned 16bit integer to char array
	typedef union Tuint16tobyte {
		uint16_t i;
		unsigned char b[sizeof(int16_t)];
	} uint16tobytes;
	//Union for converting unsigned 8bit integer to char array
	typedef union Tuint8tobyte {
		uint8_t i;
		unsigned char b[sizeof(uint8_t)];
	} uint8tobytes;
	//Union for converting signed 16bit integer to char array
	typedef union Tint16tobyte {
		int16_t i;
		unsigned char b[sizeof(int16_t)];
	} int16tobytes;
	
	
	//structure for a 16 bit register
	typedef struct Reg16{
	   unsigned char reg_low;
	   unsigned char reg_high;
	} tReg16;
	//structure for 16 bit data
	typedef struct Dat16{
	   unsigned char dat_low;
	   unsigned char dat_high;
	} tDat16;
	
	//Writes 16bit data to 16bit register on fuelgauge
	void Write2Reg16( tReg16 reg, tDat16 dat);
	
	//Reads 16 bit to given addresspointer destination from 16bit register
	void Reg16Read( tReg16 reg, tDat16 *dest);
	
	//Reads quantity of bytes from register
	void I2C_Read(int quantity, char* dest);
	//Writes quantity of bytes from buffer-pointer to fuelgauge
	void I2C_Write(char* buf, int quantity);
	//Fuelgauge-Control-Command, writing data to control register
	void Control( tDat16 dat);
	//Reads battery temperature
	float FG_BatReadTemp();
	//Calculates Checksum (summing all bytes and inverting result)
	unsigned char CalculateChecksum( unsigned char* arSHA1, unsigned char len);
	//Autenticates SHA-1-Chellange
	void Authenticate(char* arSHA1);
	//Prints status from control register
	void PrintStatus();
	//Set fuelgauge from sealed to unsealed mode
	void Sealed2Unsealed();
	//Set fuelgauge from unsealed to full access mode
	void Unsealded2FullAccess();
	//Prints battery/charging-current (now)
	void PrintCurrent();
	//Seals the fuelgauge
	void Seal();
	//Sets voltselect, i=true or false
	void SetVOLTSEL(unsigned char i);
	//Prints float beginning with little endian, only for debugging
	void PrintFloatLittleEndian(char* float_);
	//Prints command, only for debugging
	void CommandPrint( tReg16 cmd );
	//Performs dataflash (see datasheet of fuelgauge).
	void ConfigData(unsigned char subclass, unsigned char offset, char* writedata, unsigned char data_len);
	//Scans integer from shell and writes this char array to the output directory
	void ScanIntCharOut(unsigned char* output);
	//Configuration Routine
	void Configuration();
	//Reads a whole data-block, returns malloced char array
	char* ReadBlock(unsigned char reg_scr, unsigned char len);
	//Writes Block-Data
	void WriteBlock(unsigned char *block, unsigned char dest, unsigned char len);
	//Prints Block-Data
	void PrintBlock(char* block, unsigned char len);

	//Reads config data out of a data-block
	char* ReadConfData(unsigned char subclass, unsigned char offset);
	//Converts to fuelgauge-format => see datasheet
	unsigned char* DFtoEVSW(char switch_val);
#endif //__FUELGAUGE_H