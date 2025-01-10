#ifndef __SPI_DRIVER_H__
#define __SPI_DRIVER_H__
#include <spi.h>


extern void spi_peri_enable(spi_master_regs_t*);
extern void spi_peri_write(spi_master_regs_t*, unsigned int);
extern void spi_peri_select(spi_master_regs_t*,const unsigned int);
extern void spi_peri_deselect(spi_master_regs_t*);
extern void show_distance_display(volatile unsigned int abstand);
extern void display_init();
#endif
