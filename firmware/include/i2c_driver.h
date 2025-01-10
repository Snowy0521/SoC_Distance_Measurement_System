#ifndef __I2C_DRIVER_H__
#define __I2C_DRIVER_H__

#include <peripherals.h>

#define PRESCALER_FACTOR 299
#define I2C_SLAVE_ADDRESS 0xE0 
#define MEAS_N 100


#define S1 1
#define S2 2 
#define S3 3 


extern volatile unsigned int distance_sens; // initial distance 
extern unsigned int meas_n;
extern unsigned int dis_array[MEAS_N];
extern unsigned int sort_dis_array[MEAS_N];
extern unsigned int new_val;

extern void i2c_peri_enable(i2c_master_regs_t* i2c_master);
extern void i2c_peri_set_sense_register(i2c_master_regs_t* i2c_master, unsigned int reg_num);
extern void i2c_peri_read_start(i2c_master_regs_t* i2c_master, unsigned int slave_add);
extern  unsigned int i2c_peri_read_distance(i2c_master_regs_t* i2c_master);
extern void is_measuring(i2c_master_regs_t* i2c_master);
extern void i2c_peri_write(i2c_master_regs_t* i2c_master);
#endif
