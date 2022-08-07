#include <stdio.h>
#include <stdlib.h>
#include<stdint.h>
#include"fat.h"
uint32_t key=-1;
uint8_t num_entry=0; 
data_boot_sector boot_sector;
data_entry entry[10];
int i;
uint8_t read_director(uint8_t key)
{
	read_root_directory(entry,&num_entry,(entry[key-1].cluster+31),1);
	printf("\nso entry hien tai=%d",num_entry);
	printf("\n\t\t ____________________________director____________________________________");
	for(i=0;i<num_entry;i++)
   {
    	if(entry[i].wide_dissection[0]==32)
    	{
  	    	printf("\n\t\t|%d%8s",i+1,entry[i].name);
    		printf("%3s",entry[i].wide_dissection);
			printf("      %d-%02d-%02d",1980+entry[i].date.years,entry[i].date.months,entry[i].date.days);
	    	printf("     %02d:%02d:%02d",entry[i].time.hours,entry[i].time.minutes,entry[i].time.seconds*2);
			printf("       \t\tFolder\t|");
		}
		else
		{
	    	printf("\n\t\t|%d%8s",i+1,entry[i].name);
	    	printf(".%3s",entry[i].wide_dissection);
			printf("     %d-%02d-%02d",1980+entry[i].date.years,entry[i].date.months,entry[i].date.days);
	    	printf("     %02d:%02d:%02d",entry[i].time.hours,entry[i].time.minutes,entry[i].time.seconds*2);
	    	printf("     %7d Byte",entry[i].size);
	    	printf("      %s\t|",entry[i].wide_dissection);
		}	    	
	}
	printf("\n\t\t|_______________________________________________________________________|");
	return key;
}
uint8_t read_file(uint8_t key)
{
	LinkedList a;
	struct Node head; /*declrale a is struct LinkedList*/
	a.Head = &head;/*declare address of node head*/ 
			printf("\ngia tri cua node head:%x",a.Head);
	uint8_t *data;
	uint16_t num_sector=entry[key-1].size/512+1;
	printf("\nsize of entry %d",entry[key-1].size);
	printf("\n so sector la%d",num_sector);
	data=(uint8_t*)calloc(num_sector,512);
	a.Head->data=entry[key-1].cluster;
	printf("\nso cluster bat dau  la%d",a.Head->data);
    a.Head->pNext=NULL;
	read_fat_table(a.Head->data,14,(LinkedList*)&a);
	struct Node *b;
	b=a.Head;
	while(b->pNext!=NULL)
	{
		read_data(b->data+31,data);
		for(i=0;i<512;i++)
		{
			if(i%128==0)
			{
				printf("\n");
			}
			printf("%c",data[i]);
		}
		b=b->pNext;
	}
	free_allocate_dynamic((LinkedList*)&a);
	free(data);
	uint8_t t;
	do
	{
		printf("\nenter '0' to back:");
		scanf("%d",&t);
		if(t!=0)
		{
			printf("\nplease enter return");
		}
	}
	while(t!=0);
	return key;
}
int main(void)
{
	//typedef struct LinkedList *node
	char filepath[50];
	scanf("%s",filepath);
	printf("\ngia tri cua *filepath:%s",filepath);
	if(open_file(filepath)==0U)
	{
		printf("\n open file error");
	}
	else
	{
		printf("\n open file sucessful");
	}
	if(read_boot_sector(&boot_sector)==1)
	{
		printf("\n error read");
	}
	printf("\n\t\t+------------------------boot sector------------------------+");
    printf("\n\t\t| <> Sector per cluster                | %d  \t            |",boot_sector.clusterPerSector);
	printf("\n\t\t|-----------------------------------------------------------|");
    printf("\n\t\t| <> Number of sector in FAT           | %d  \t            |",boot_sector.sectorPerFat12);
    printf("\n\t\t|-----------------------------------------------------------|");
    printf("\n\t\t| <> Size of file                      | %dKB\t            |",boot_sector.numSector*512/1024);
    printf("\n\t\t|-----------------------------------------------------------|");
    printf("\n\t\t| <> Number of sector in root director | %d  \t            |",boot_sector.sectorBootDirector/16);
    printf("\n\t\t|-----------------------------------------------------------|");
    printf("\n\t\t| <> Byte per sector                   | %d  \t            |",boot_sector.bytePerSector);
    printf("\n\t\t|-----------------------------------------------------------|");
    printf("\n\t\t| <> File sytem                        | %s           |",boot_sector.typeFat);
    printf("\n\t\t+-----------------------------------------------------------+");
	printf("\nso entry =%d",num_entry);
	while(key!=0)
	{
		read_root_directory(entry,&num_entry,(1+boot_sector.sectorPerFat12*2),boot_sector.sectorBootDirector/16);
		printf("\nso sector la:%d ",boot_sector.sectorBootDirector/16);
	    printf("\nso entry =%d",num_entry);
	    printf("\n\t\t+--------------------------------root director----------------------------------+");
	    printf("\n\t\t| Option |   Name        |       Date modifiled       |   Size   |      Type    |");
	    for(i=0;i<num_entry;i++)
	    {
	    	if(entry[i].wide_dissection[0]==32)
	    	{
	    	printf("\n\t\t|  %3d   |%8s",i+1,entry[i].name);
	    	printf("%3s\t |",entry[i].wide_dissection);
			printf("     %d-%02d-%02d",1980+entry[i].date.years,entry[i].date.months,entry[i].date.days);
	    	printf("     %02d:%02d:%02d|",entry[i].time.hours,entry[i].time.minutes,entry[i].time.seconds*2);
			printf("\t\t |\tFolder\t|");
			}
			else
			{
	    	printf("\n\t\t|  %3d   |%8s",i+1,entry[i].name);
	    	printf(".%3s\t |",entry[i].wide_dissection);
			printf("     %d-%02d-%02d",1980+entry[i].date.years,entry[i].date.months,entry[i].date.days);
	    	printf("     %02d:%02d:%02d|",entry[i].time.hours,entry[i].time.minutes,entry[i].time.seconds*2);
	    	printf("%7d KB|",entry[i].size/512 +1);
	    	printf("      %s\t|",entry[i].wide_dissection);
			}	    	
		}
		printf("\n\t\t+-------------------------------------------------------------------------------+");
		printf("\nenter option you want open option or '0' to exit program: ");
		scanf("%d",&key);
		printf("\ngia tri cua key=%d",key);
		if(key==0)
		{
			printf("\nexit program");
		}
		else if(entry[key-1].wide_dissection[0]==32)
		{
			uint8_t key1=key;
			while(key!=0)
			{
				read_director(key1);
				printf("\nenter option you want open option or '0' to back:");
			    scanf("%d",&key);
			    if(key==0)
				{
					printf("\nback");
				}
				else if(entry[key-1].wide_dissection[0]==32)
				{
					read_director(key);
				}
				else
				{
					read_file(key);	
				}
		  	}
		  	key=-1;
		}
		else //if(entry[key-1].wide_dissection[0]==00)
		{
			read_file(key);
		}
 	}

	if(close_file()==0U)
	{
		printf("\nclose file sucessful");
	}
	return 0;
}
/*    a.Head->data=8;
    a.Head->pNext=NULL;
	read_fat_table(8,14,(LinkedList*)&a);
	struct Node *b;
	b=a.Head;
	while(b->pNext!=NULL)
	{
		printf("\ndata of fat table is %d",b->data);
		b=b->pNext;
	}
	*/
