#ifndef _HAL_
#define _HAL_
#include<stdint.h>
uint8_t init(char *);
uint8_t de_init(void);
uint32_t kmc_read_sector(uint32_t index,uint8_t *buff);
uint32_t kmc_read_multi_sector(uint32_t index,uint32_t num,uint8_t *buff);
#endif /*_HAL_*/
