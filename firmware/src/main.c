#include "i2c_driver.h"
#include "spi_driver.h"
#include <stdio.h>
#include <string.h>
#include "display.h"
#include <interrupt.h>
#include "filter.h"

FILE * stdout = &UART_LIGHT_FILE;


void main()
{	
	spi_peri_enable(&SPI_MASTER); // initialize the spi module
	display_init();               // initialize the display with text
	i2c_peri_enable(&I2C_MASTER); // initialize the i2c module 
	interrupt_enable();	      // enable interrupts 
	i2c_peri_write(&I2C_MASTER);  // start a measurement

	while(1) // while loop. Program jumps out of while when i2c interrupt occurs 
	{	
		/*if(meas_n >= MEAS_N) // get new distance when 10 new measurements are collected 
		{
			meas_n = 0;
			distance_sens = median_filter(dis_array, MEAS_N);
		}*/
		if(new_val)
		{
				memcpy(sort_dis_array, dis_array, MEAS_N*sizeof(unsigned int));
				distance_sens = median_filter(sort_dis_array, MEAS_N);
				new_val = 0;
		}

		show_distance_display(distance_sens);		
	} // end of while(1)
} // end of main

