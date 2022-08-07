#include <stdio.h>
#include <stdlib.h>
#include<stdint.h>
#include"HAL.h"
#include"read_file.h"
int main(void)
{
	char filepath[50];
	uint32_t key=-1;
	uint8_t num_entry=0;
//	scanf("%s",filepath);
//	printf("\ngia tri cua *filepath:%s",filepath);
	if(init(filepath)==0U)
	{
		printf("\n open file error");
	}
	else
	{
		printf("\n open file sucessful");
	}
	int i;
	data_boot_sector boot_sector;
	data_entry entry[10];
	if(read_boot_sector(&boot_sector)==1)
	{
		printf("\n loooix dooc");
	}
	printf("\n ________________________boot sector________________________");
    printf("\n| <> Sector per cluster                : %d  \t            |",boot_sector.clusterPerSector);
    printf("\n| <> Number of sector in FAT           : %d  \t            |",boot_sector.sectorPerFat12);
    printf("\n| <> Size of file                      : %dKB\t            |",boot_sector.numSector*512/1024);
    printf("\n| <> Number of sector in root director : %d  \t            |",boot_sector.sectorBootDirector/16);
    printf("\n| <> Byte per sector                   : %d  \t            |",boot_sector.bytePerSector);
    printf("\n| <> File sytem                        : %s           |",boot_sector.typeFat);
    printf("\n|___________________________________________________________|");
	//printf("\nso entry =%d",num_entry);
	while(key!=0)
	{
		read_root_directory(entry,&num_entry,(1+boot_sector.sectorPerFat12*2));//
	    printf("\nso entry =%d",num_entry);
	    for(i=0;i<num_entry;i++)
	    {
	    	if(entry[i].wide_dissection[0]==32)
	    	{
	    	printf("\n%d%8s",i+1,entry[i].name);
	    	printf("%3s",entry[i].wide_dissection);
			printf("      %d-%02d-%02d",1980+entry[i].date.years,entry[i].date.months,entry[i].date.days);
	    	printf("     %02d:%02d:%02d",entry[i].time.hours,entry[i].time.minutes,entry[i].time.seconds*2);
			printf("       \t\tFolder");
			}
			else
			{
	    	printf("\n%d%8s",i+1,entry[i].name);
	    	printf(".%3s",entry[i].wide_dissection);
			printf("     %d-%02d-%02d",1980+entry[i].date.years,entry[i].date.months,entry[i].date.days);
	    	printf("     %02d:%02d:%02d",entry[i].time.hours,entry[i].time.minutes,entry[i].time.seconds*2);
	    	printf("     %7d Byte",entry[i].size);
	    	printf("      %s",entry[i].wide_dissection);
			}	    	
		}
		printf("\nenter option you want open option or '0' to back");
		scanf("%d",&key);
		printf("\ngia tri cua key=%d",key);
		if(key==0)
		{
			printf("\nexit program");
		}
		else if(entry[key-1].wide_dissection[0]==32)
		{
			read_root_directory(entry,&num_entry,(entry[key-1].cluster+31));
			printf("\nso entry hien tai=%d",num_entry);
		    for(i=0;i<num_entry;i++)
		    {
		    	if(entry[i].wide_dissection[0]==32)
		    	{
		    	printf("\n%d%8s",i+1,entry[i].name);
		    	printf("%3s",entry[i].wide_dissection);
				printf("      %d-%02d-%02d",1980+entry[i].date.years,entry[i].date.months,entry[i].date.days);
		    	printf("     %02d:%02d:%02d",entry[i].time.hours,entry[i].time.minutes,entry[i].time.seconds*2);
				printf("       \t\tFolder");
				}
				else
				{
		    	printf("\n%d%8s",i+1,entry[i].name);
		    	printf(".%3s",entry[i].wide_dissection);
				printf("     %d-%02d-%02d",1980+entry[i].date.years,entry[i].date.months,entry[i].date.days);
		    	printf("     %02d:%02d:%02d",entry[i].time.hours,entry[i].time.minutes,entry[i].time.seconds*2);
		    	printf("     %7d Byte",entry[i].size);
		    	printf("      %s",entry[i].wide_dissection);
				}	    	
			}
			printf("\ngia tri cua key=%d",key);
		}
		else //if(entry[key-1].wide_dissection[0]==00)
		{
		uint8_t *data;
		uint8_t num_sector=entry[key-1].size/512+1;
		printf("\n so sector la%d",num_sector);
		data=(uint8_t*)calloc(num_sector,512);
		kmc_read_multi_sector(entry[key-1].cluster+31,num_sector,data);
		for(i=0;i<512*num_sector;i++)
		{
			if(i%128==0)
			{
				printf("\n");
			}
			printf("%c",data[i]);
		}
		uint8_t t;
		do
		{
			printf("\nenter '0' to back");
			scanf("%d",&t);
			if(t!=0)
			{
				printf("\nplease enter return");
			}
		}
		while(t!=0);	
		}
		printf("\ngia tri cua key=%d",key);
 	}
	if(de_init()==0U)
	{
		printf("\nclose file sucessful");
	}
	return 0;
}
