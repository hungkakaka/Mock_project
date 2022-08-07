#include <stdio.h>
#include <stdlib.h> 
#include<stdint.h>
#include"HAL.h"
FILE *fptr=NULL;
uint8_t init(char *filepath)
{
	uint8_t retval=1U;
	fptr = fopen("D:\\floppy.img", "rb");//(char *)filepath
	if(fptr==NULL)
	{
		retval=0U;
	}
	
	return retval;
}
uint32_t kmc_read_sector(uint32_t index,uint8_t *buff)
{
	uint32_t count;
    fseek(fptr, index*512, SEEK_SET);
    count =fread(buff,1,512, fptr); 
    
    return count;
}
uint32_t kmc_read_multi_sector(uint32_t index,uint32_t num,uint8_t *buff)
{
	int count;
    fseek(fptr,	index*512, SEEK_SET); 
    count =fread(buff,1,512*num, fptr); 
    
    return count;
}
uint8_t de_init(void)
{
	uint8_t retval=1U;
	retval=fclose(fptr);
	
	return retval;
}

