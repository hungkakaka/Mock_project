#include <stdio.h>
#include <stdlib.h>
#include<stdint.h>
#include"HAL.h"
#include"fat.h"
uint8_t open_file(char *filepath)
{
	uint8_t retval = 1U;
	if(init(filepath) == 0U)
	{
		retval = 0U;
	}
	
	return retval;
}
uint8_t close_file(void)
{
	uint8_t retval = 1U;
	retval=de_init();
	
	return retval;
}
uint8_t read_boot_sector(data_boot_sector *a)
{
	uint8_t data[512];
	uint8_t i;
	uint8_t retval=correct;
	if(kmc_read_sector(0,(uint8_t *)&data) == 0)
	{
		retval=incorrect;
	}
	a->bytePerSector= data[11] + (data[12]<<8);
    a->clusterPerSector= data[13];
    a->sectorPerFat12= data[22] + (data[23]<<8);
    a->numSector= data[19]  +(data[20]<<8);
    a->sectorBootDirector= data[17] + (data[18]<<8);
    for(i=0; i<8; i++)
    {
    a->typeFat[i] = data[54+i];
    a->typeFat[8] = '\0';
	}

    return retval;
}
uint8_t read_root_directory(data_entry *a, uint8_t *num_entry, uint32_t cluster,uint16_t num_sec)
{
	printf("\n cluster bat dau %d",cluster);
	*num_entry = 0;
	uint16_t e;
	uint8_t  *data;
	data = (uint8_t *)calloc(num_sec,512);
	int retval = correct;
	printf("\nso sector la11111:%d ",num_sec);
	if(kmc_read_multi_sector(cluster,num_sec, data) == 0)
	{
		retval = incorrect;
		printf("\n erroroororor");
	}
	//printf("\ndata 1:%X",data[0]);
	int i;
	int j=0;
	int k=0;
	for(i=0; i<512*num_sec; i++)
	{
		j= i/32;
		if(data[j*32+11] != 15 && data[j*32] != 0 &&data[j*32+11] != 0x28 )//&data[j*32+11]!=16.&& data[j*32] != 46
		{
			if(i%32 == 0)
			{
				*num_entry += 1;
			}
			if(i < (j*32+8))
			{
				(a + (k/32))->name[i - 32*j] = data[i];
				(a + (k/32))->name[8] = '\0';
			}
			else if(i > (j*32 + 7) & i < (j*32 + 11))
			{
				(a + (k/32))->wide_dissection[i - 32*j - 8] = data[i];
				(a + (k/32))->wide_dissection[3] ='\0';				
			}
			else if(i == j*32 + 22)
			{
				(a + (k/32))->time.seconds=data[i] + (data[i + 1] << 8);
				(a + (k/32))->time.minutes=(data[i] + (data[i + 1] << 8)) >> 5;
				(a + (k/32))->time.hours=(data[i] + (data[i + 1] << 8)) >> 11;					
			}
			else if(i == j*32 + 24)
			{
				(a + (k/32))->date.days=data[i] + (data[i + 1] << 8);
				(a + (k/32))->date.months=(data[i] + (data[i + 1] << 8)) >> 5;
				(a + (k/32))->date.years=(data[i] + (data[i + 1] << 8)) >> 9;			
			}
			else if(i == j*32 + 28)
			{
				(a + (k/32))->size=data[i] + (data[i + 1] << 8) + (data[i + 2] << 16)+(data[i + 3] <<24);
			}
			else if(i == j*32 + 26)
			{
				(a + (k/32))->cluster=data[i] + (data[i + 1] << 8);
			}
			k++;
		}
		else if(data[j*32] == 0)
		{
			i = 512*num_sec;
		}
	}

    return retval;
}
uint8_t read_data(uint32_t index, uint8_t *buff)
{
	uint8_t retval = 1U;
	if(kmc_read_sector(index, buff) == 0U)
	{
		retval = 0U;
	}
	return retval;
}
uint32_t read_fat_table(uint16_t clusStr, uint16_t secPerFat, LinkedList *Head)
{
	uint8_t  retval = 1U;
	uint16_t entry = 0;
	uint16_t pos;
	uint8_t  *data;
	
	data = (uint8_t *)calloc(secPerFat,512);
	if(kmc_read_multi_sector(1, secPerFat, data) == 0U)
	{
		retval=0U;
	}
	//read FAT table
	while(entry < 0xFF8)
	{
		pos = clusStr*1.5;
		if(clusStr % 2 == 0)//neu vi tri cluster la chan
		{
			entry = data[pos] + ((data[pos+1] & 0x0f) << 8);
		}
		else
		{
			entry = ((data[pos] & 0xf0) >> 4) + (data[pos + 1] << 4);
		}
		printf("\n%d",entry);
		AddTail(Head, entry);
		clusStr = entry;	
	}
	
	return retval;
}
uint8_t AddTail(LinkedList *head, uint16_t value)
{
    struct Node *temp;
	struct Node *p;
	
    temp = (struct Node *)malloc(sizeof(*temp));
    temp->pNext = NULL;
    temp->data = value; 
    p  = head->Head;
    while(p->pNext != NULL)
	{
        p = p->pNext;
    }
    p->pNext = temp;
    
    return 0;
}
uint8_t free_allocate_dynamic(LinkedList *a)
{
	uint16_t num_node = 0;
	
	struct Node *temp = NULL;
	struct Node *c = a->Head->pNext;
	while(c != NULL)
	{
		temp = c;
		c = c->pNext;
		free(temp);
		num_node++;
	}
	a->Head->pNext = NULL;
	
	return num_node;
}
 
 
