#include "i2c_driver.h"
#include <stdio.h>
#include <interrupt.h>


unsigned int meas_n = 0; // current measurement
volatile unsigned int state_i2c = S1; // initial state
volatile unsigned int distance_sens = 0; // initial distance 
unsigned int is_write = 1;
unsigned int new_val = 1;

//distance value collection
unsigned int dis_array[MEAS_N] = {0};
unsigned int sort_dis_array[MEAS_N] = {0};

void i2c_peri_enable(i2c_master_regs_t* i2c_master)
{
	i2c_master->ctrl = 0; 
	i2c_master->ctrl = I2C_CTRL_EN | PRESCALER_FACTOR | I2C_CTRL_INTR_EN;
}

void i2c_peri_set_sense_register(i2c_master_regs_t* i2c_master, unsigned int reg_num)
{
	i2c_master->data[0] = I2C_SLAVE_ADDRESS; // set direction bit = 1, master read data from slave
	i2c_master->data[1] = reg_num;
	i2c_master->cmd = I2C_CMD_STA | (2 << 3) | I2C_CMD_STO;
}

void i2c_peri_read_start(i2c_master_regs_t* i2c_master, unsigned int slave_add)
{
	i2c_master->cmd = 0;
	i2c_master->data[0] = slave_add + 1;
	i2c_master->cmd = I2C_CMD_RD | I2C_CMD_STA | (3 << 3) | I2C_CMD_STO; 
}

 unsigned int i2c_peri_read_distance(i2c_master_regs_t* i2c_master)
{
	//diese beiden Register geben zusammen die Entfernungswert zurueck
	unsigned int register_2, register_3, distance=0;
	//Entfernungswert(high byte) in register_2
	register_2 = i2c_master->data[1];
	//Entfernungswert(low byte) in register_3
	register_3 = i2c_master->data[2];
	//distance = (register_2*256) + register_3; // maybe does not belong here
	distance = (register_2<<8) | register_3; // maybe does not belong here
	return distance;
}

void is_measuring(i2c_master_regs_t* i2c_master)
{
	/* The code in this comment is for checking firmware version number
	i2c_master->cmd  = 0; //clear all command register bits == 0
	i2c_master->data[0] = I2C_SLAVE_ADDRESS; //for write
	i2c_master->data[1] = 0x00; //for write
	i2c_master->cmd = I2C_CMD_STA | (2 << 3) | I2C_CMD_STO;	
	*/
	// interrupt ausloesen
	i2c_master->cmd  = 0; //clear all command register bits == 0
	i2c_master->data[0] = I2C_SLAVE_ADDRESS; //for write
	i2c_master->cmd = I2C_CMD_STA | (1 << 3) | I2C_CMD_STO;	
}


void i2c_peri_write(i2c_master_regs_t* i2c_master)
{
	//registernummer of slave 0: Befehlsregister
	i2c_master->cmd = 0; //clear all command register bits == 0
	i2c_master->data[0] =  I2C_SLAVE_ADDRESS; //0xE0 == 1110 0000 
	i2c_master->data[1] = 0x00; // srv05 datenblatt seite 6 (ultraschallsensor)
	i2c_master->data[2] = 0x51;
	
	// send write request to the slave
	i2c_master->cmd = I2C_CMD_STA | (3 << 3) | I2C_CMD_STO;
}


// interrupt service routine
ISR(0)()
{
			switch(state_i2c)
			{
				case S1: // wait measurmement
					if(is_write)
					{
						is_measuring(&I2C_MASTER);
						is_write = 0;

					}
					else if(I2C_MASTER.stat & I2C_STA_NO_ACK)
					{
						is_measuring(&I2C_MASTER);

					}
					else
					{
						i2c_peri_set_sense_register(&I2C_MASTER, 0x02);
						state_i2c = S2;
					}
					
					/* The code in this comment is for checking firmware version number
					if(is_write)
					{
						is_measuring(&I2C_MASTER);
						is_write = 0;

					}
					else if(I2C_MASTER.data[1] == 255)
					{
						is_measuring(&I2C_MASTER);

					}
					else
					{
						i2c_peri_set_sense_register(&I2C_MASTER, 0x02);
						state_i2c = S2;
					}*/
					break;
				case S2:// set read register
					i2c_peri_read_start(&I2C_MASTER, I2C_SLAVE_ADDRESS);
					state_i2c = S3;
					break;
				case S3: // calculate the distance and start a new measurement
					dis_array[meas_n] = i2c_peri_read_distance(&I2C_MASTER);
					meas_n++;
					if(meas_n >= MEAS_N) // get new distance when 10 new measurements are collected 
					{
						meas_n = 0;
					}
					i2c_peri_write(&I2C_MASTER);
					is_write = 1;
					new_val = 1;
					state_i2c = S1;
					break;
				default:
					printf("shouldnt occur");
					break;
			} // end of switch(state)
}


