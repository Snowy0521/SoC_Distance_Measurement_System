#include <spi_driver.h>
#include <spi.h>
#include "peripherals.h"
#include "display.h"
#include <stdio.h>

void spi_peri_select(spi_master_regs_t* spi_master, const unsigned int slave)
{
	/* activate the slave */
	spi_master->spi.spi_control |= (slave << 4);
}

void spi_peri_deselect(spi_master_regs_t* spi_master)
{
	/* deactivate the slave */
	while(spi_master->spi.spi_status & SPI_MASTER_STAT_FILL);
	spi_master->spi.spi_control &= ~SPI_MASTER_CTRL_SLAVE;
}

void spi_peri_enable(spi_master_regs_t* spi_master)
{
	spi_master->spi.spi_control &= 0;
 	spi_master->spi.spi_control |= SPI_MASTER_CTRL_EN; // enable the master
 	spi_master->spi.spi_control &= ~SPI_MASTER_CTRL_BITCNT; //clear the default number of bits in one frame
 	spi_master->spi.spi_control |= (9<<8);  // set the number of bits in one frame to 9
 	spi_master->spi.spi_control |= (1 << 13); // set the clock devider
}
 
void spi_peri_write(spi_master_regs_t* spi_master, unsigned int dt)
{
	/* data written into spi_data_out will be sent */
	while(spi_master->spi.spi_status & SPI_MASTER_STAT_FILL);
	spi_master->spi.spi_data_out = dt;
}


void display_init()
{
	char *data = "Der Abstand ist:";
	//spi master activ 
	char *unit = "cm";
	spi_peri_select(&SPI_MASTER,1);
	
	//initialize display; 
	OLED_Init_25664();
	Checkerboard_25664();
	Fill_RAM_25664(0);
	//show first text
	Show_String_25664(1, data, 0, 0);
	Show_String_25664(1, unit, 40, 0);
	// maybe deselect
	spi_peri_deselect(&SPI_MASTER);
}

void show_distance_display(volatile unsigned int abstand) 
{
	char result[10];
	// transform unsigned int to char
	snprintf(result, 3, "%u\n", abstand);
	//always show 3 bytes value
	if(abstand < 100)
	{
		int index = 3;
		while(index)
		{
			result[index] = result[index-1];
			index--;
		}
		result[0] = '0';
	}
	//show result distance in display 
	spi_peri_select(&SPI_MASTER,1);
	Show_String_25664(1, result, 32, 0);
	spi_peri_deselect(&SPI_MASTER);
}

 
