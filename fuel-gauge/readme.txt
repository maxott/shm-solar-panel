Programming the Fuel-Gauge
In the datasheet you will read about some commands. A command is ment to be writing register numbers to the fuelgauge to select those. If you want to read from a register you have to consider the two options.
The first option is to write the first register number of the 2 byte register and read the byte from this register and afterwards write the second byte to the fuelgauge and read it again.
The second option is using the auto-increment function of the fuelgauge. There you only have to write the first register address to the fuelgauge and read as much bytes as you want. You have to pay attention that the I2C-Transmittion isn't interrupted otherwise the data you read doesn't come from the expected register. 
It is important to mention that the fuelgauge uses little-endian architecture and the raspberry pi big endian. So the first byte you read is the highest byte.

The Conrol-Command
Commands as the Control-Command may be a bit more difficult. There you have subcommands. A subcommand is more or less data you write to the register. If you want to read from a subcommand, you have to write at first the subcommand (data) to the command register and the next step is to read from the command register. So as you can see the subcommand selects which data is read from the control-registers.
The subcommand is in detail made of two commands: the first control-command selecting (writing) the subcommand to the control-registers, the second control command contains the return value of the first control command, it is only reading data from the control-command-registers.
Consider that if you want to read from the control-register, you have to select it in the beginning of the I2C-Data-Transmission.

Data-Flash
The Data-Flash-Method is used for configuration and calibrating the fuelgauge.There is a certain window of registers which can be written to (depending on the access-mode). Within this window you can access a lot of the fuelgauge's internal memory. Pay attention that you are writing to non-volatile RAM and there is no protection against wrong data, so if you write nonsense to the fuelgauge it might cause hardware-issues.
There is a subclass-register which selects to which class of data your data should be written. It is necessary to set the offset because the data-flash-window might be to small for big classes.

At next I want to describe how a data-flash is done giving examples from my fuelgauge-c-program:
1. Write to BlockDataControl 0x00 (Full-Access-Mode)
	buf[0] = DFDCNTL;
	buf[1] = 0x00;
	I2C_Write(buf, 2);
	
2. Set Class
	buf[0] = DFCLS;
	buf[1] = subclass;
	I2C_Write(buf, 2);
	
3. Set Offset – Use modulo to calculate the offset considering the block-lengh
	mod_offset = offset % BLOCK_LEN;
	block_offset = offset / BLOCK_LEN;
	buf[0] = DFBLK;
	buf[1] = block_offset;
	I2C_Write(buf, 2);	
	
4. Read the whole data-flash-block
	data_block = ReadBlock(A_DF, BLOCK_LEN);
5. Modify Changes
	for(i = 0; i<data_len; i++){
		data_block[mod_offset + i] = writedata[i];
	}
6. Write modified Block
	WriteBlock(data_block, A_DF, BLOCK_LEN);

7. Write Checksum (summing whole block, complement the result)
	checksum = CalculateChecksum(data_block, BLOCK_LEN);
	buf[0] = DFDCKS;
	buf[1] = checksum;
	I2C_Write(buf, 2);

Only if a valid checksum is written, the fuelgauge stores the written data. If the checksum is incorrect, it will not acknowledge this written byte (I2C-Error). 