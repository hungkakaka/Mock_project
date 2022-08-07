#include <stdio.h>
#include <stdlib.h>
#include<stdint.h>
#include"HAL.h"
#include"read_file.h"
int read_boot_sector(data_boot_sector *a)
{
	uint8_t data[512];
	uint8_t i;
	uint8_t retval=correct;
	if(kmc_read_sector(0,(uint8_t *)&data)==0)
	{
		retval=incorrect;
	}
	a->bytePerSector=data[11]+(data[12]<<8);
    a->clusterPerSector=data[13];
    a->sectorPerFat12=data[22]+(data[23]<<8);
    a->numSector=data[19]+(data[20]<<8);
    a->sectorBootDirector=data[17]+(data[18]<<8);
    for(i=0;i<8;i++)
    {
    a->typeFat[i]=data[54+i];
    a->typeFat[8]='\0';
	}
	printf("\nloai fat:%s",a->typeFat);
//    a->typeFat[0]=data[54];
//    a->typeFat[1]=data[55];
//    a->typeFat[2]=data[56];

    return retval;
}
int read_root_directory(data_entry *a,uint8_t *b,uint32_t cluster)
{
	*b=0;
	uint16_t e;
	uint8_t data[512];
	int retval=correct;
	if(kmc_read_sector(cluster,(uint8_t *)&data)==0)
	{
		retval=incorrect;
	}
	int i;
	int j=0;
	int k=0;
	for(i=0;i<352;i++)
	{
		j=i/32;
		if(data[j*32+11]!=15&data[j*32]!=0&data[j*32]!=46)//&data[j*32+11]!=16
		{
			if(i%32==0)
			{
				*b+=1;
			}
			if(i<(j*32+8))
			{
				(a+(k/32))->name[i-32*j]=data[i];
				(a+(k/32))->name[8]='\0';
			}
			else if(i>(j*32+7)&i<(j*32+11))
			{
				(a+(k/32))->wide_dissection[i-32*j-8]=data[i];
				(a+(k/32))->wide_dissection[3]='\0';				
			}
			else if(i==j*32+22)
			{
				(a+(k/32))->time.seconds=data[i]+(data[i+1]<<8);
				(a+(k/32))->time.minutes=(data[i]+(data[i+1]<<8))>>5;
				(a+(k/32))->time.hours=(data[i]+(data[i+1]<<8))>>11;					
			}
			else if(i==j*32+24)
			{
				(a+(k/32))->date.days=data[i]+(data[i+1]<<8);
				(a+(k/32))->date.months=(data[i]+(data[i+1]<<8))>>5;
				(a+(k/32))->date.years=(data[i]+(data[i+1]<<8))>>9;			
			}
			else if(i==j*32+28)
			{
				(a+(k/32))->size=data[i]+(data[i+1]<<8)+(data[i+2]<<8)+(data[i+3]<<8);
			}
			else if(i==j*32+26)
			{
				(a+(k/32))->cluster=data[i]+(data[i+1]<<8);
			}
			k++;
		}
//		else if(data[j*32+11]==16)
//		{
//			*b+=1;
//			if(i<(j*32+7))
//			{
//				(a+(k/32))->name[i-32*j]=data[i];
////				if(data[i]==' ')
////				{
////					(a+(k/32))->name[i-32*j]='\0';
////				}
//				(a+(k/32))->name[8]='\0';
//			}
//			else if(i>(j*32+7)&i<(j*32+11))
//			{
//				(a+(k/32))->wide_dissection[i-32*j-8]=data[i];	
//				(a+(k/32))->wide_dissection[3]='\0';				
//			}
//			else if(i==j*32+22)
//			{
//				(a+(k/32))->time.seconds=data[i]+(data[i+1]<<8);
//				(a+(k/32))->time.minutes=(data[i]+(data[i+1]<<8))>>5;
//				(a+(k/32))->time.hours=(data[i]+(data[i+1]<<8))>>11;					
//			}
//			else if(i==j*32+24)
//			{
//				(a+(k/32))->date.days=data[i]+(data[i+1]<<8);
//				(a+(k/32))->date.months=(data[i]+(data[i+1]<<8))>>5;
//				(a+(k/32))->date.years=(data[i]+(data[i+1]<<8))>>9;			
//			}
//			else if(i==j*32+28)
//			{
//				(a+(k/32))->size=data[i]+(data[i+1]<<8)+(data[i+2]<<8)+(data[i+3]<<8);
//			}
//			else if(i==j*32+26)
//			{
//				(a+(k/32))->cluster=data[i]+(data[i+1]<<8);
//			}
//			k++;
//		}
	}

    return retval;
}
