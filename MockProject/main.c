#include <stdio.h>
#include <stdlib.h>
#include<stdint.h>
#include"fat.h"
uint32_t key=-1;
uint8_t num_entry=0; 
data_boot_sector boot_sector;
data_entry entry[50];
int i;
LinkedList a;
int main(void)
{
	uint16_t pre_director = -1;
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
		printf("\n thu muc goc");
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
	    	printf("%7d KB|",entry[i].size/1024 +1);
	    	printf("      %s\t|",entry[i].wide_dissection);
			}	    	
		}
		printf("\n\t\t+-------------------------------------------------------------------------------+");
		printf("\nenter option you want open option or '0' to exit program: ");
		scanf("%d",&key);
		system("cls");
		if(key == 0)
		{
			printf("\nexit program");
		}
		else if(entry[key-1].wide_dissection[0]==32)
		{
			printf("\n thu muc con 1");
			read_root_directory(entry,&num_entry,(entry[key-1].cluster+31),1);
			printf("\n\t\t+----------------------------------director-------------------------------------+");
			printf("\n\t\t| Option |   Name        |       Date modifiled       |   Size   |      Type    |");
			for(i=0;i<num_entry;i++)
		   {
		   	if(entry[i].wide_dissection[0] == 32 &&  entry[i].name[0] != 0x2e)
		    	{
		    	printf("\n\t\t|  %3d   |%8s",i-1,entry[i].name);
		    	printf("%3s\t |",entry[i].wide_dissection);
				printf("     %d-%02d-%02d",1980+entry[i].date.years,entry[i].date.months,entry[i].date.days);
		    	printf("     %02d:%02d:%02d|",entry[i].time.hours,entry[i].time.minutes,entry[i].time.seconds*2);
				printf("\t\t |\tFolder\t|");
				}
				else if(entry[i].name[0] != 0x2e)
				{
		    	printf("\n\t\t|  %3d   |%8s",i-1,entry[i].name);
		    	printf(".%3s\t |",entry[i].wide_dissection);
				printf("     %d-%02d-%02d",1980+entry[i].date.years,entry[i].date.months,entry[i].date.days);
		    	printf("     %02d:%02d:%02d|",entry[i].time.hours,entry[i].time.minutes,entry[i].time.seconds*2);
		    	printf("%7d KB|",entry[i].size/1024 +1);
		    	printf("      %s\t|",entry[i].wide_dissection);
				}	    	
			}
			printf("\n\t\t+-------------------------------------------------------------------------------+");
			while(key != 0 || pre_director!=0 )
			{
				pre_director=entry[1].cluster;
				printf("\nsector chua no la %d ",entry[1].cluster);
				printf("\nenter option you want open option or '0' to back:");
			    scanf("%d",&key);
			    system("cls");
			    if(key == 0 && pre_director != 0)
				{
					printf("\nback");
					read_root_directory(entry,&num_entry,(entry[1].cluster+31),1);
					printf("\n\t\t+----------------------------------director-------------------------------------+");
					printf("\n\t\t| Option |   Name        |       Date modifiled       |   Size   |      Type    |");
					for(i=0;i<num_entry;i++)
				   {
				   	if(entry[i].wide_dissection[0] == 32 &&  entry[i].name[0] != 0x2e)
				    	{
				    	printf("\n\t\t|  %3d   |%8s",i-1,entry[i].name);
				    	printf("%3s\t |",entry[i].wide_dissection);
						printf("     %d-%02d-%02d",1980+entry[i].date.years,entry[i].date.months,entry[i].date.days);
				    	printf("     %02d:%02d:%02d|",entry[i].time.hours,entry[i].time.minutes,entry[i].time.seconds*2);
						printf("\t\t |\tFolder\t|");
						}
						else if(entry[i].name[0] != 0x2e)
						{
				    	printf("\n\t\t|  %3d   |%8s",i-1,entry[i].name);
				    	printf(".%3s\t |",entry[i].wide_dissection);
						printf("     %d-%02d-%02d",1980+entry[i].date.years,entry[i].date.months,entry[i].date.days);
				    	printf("     %02d:%02d:%02d|",entry[i].time.hours,entry[i].time.minutes,entry[i].time.seconds*2);
				    	printf("%7d KB|",entry[i].size/512 +1);
				    	printf("      %s\t|",entry[i].wide_dissection);
						}	    	
					}
					printf("\n\t\t+-------------------------------------------------------------------------------+");
					//read_director(2);
				}
				else if(entry[key+1].wide_dissection[0] == 32 && key!=0)
				{
					//read_director(key + 2);
					printf("\n thu muc con 2");
					read_root_directory(entry,&num_entry,(entry[key+1].cluster+31),1);
					printf("\n\t\t+----------------------------------director-------------------------------------+");
					printf("\n\t\t| Option |   Name        |       Date modifiled       |   Size   |      Type    |");
					for(i=0;i<num_entry;i++)
				   {
				   	if(entry[i].wide_dissection[0] == 32 &&  entry[i].name[0] != 0x2e)
				    	{
				    	printf("\n\t\t|  %3d   |%8s",i-1,entry[i].name);
				    	printf("%3s\t |",entry[i].wide_dissection);
						printf("     %d-%02d-%02d",1980+entry[i].date.years,entry[i].date.months,entry[i].date.days);
				    	printf("     %02d:%02d:%02d|",entry[i].time.hours,entry[i].time.minutes,entry[i].time.seconds*2);
						printf("\t\t |\tFolder\t|");
						}
						else if(entry[i].name[0] != 0x2e)
						{
				    	printf("\n\t\t|  %3d   |%8s",i-1,entry[i].name);
				    	printf(".%3s\t |",entry[i].wide_dissection);
						printf("     %d-%02d-%02d",1980+entry[i].date.years,entry[i].date.months,entry[i].date.days);
				    	printf("     %02d:%02d:%02d|",entry[i].time.hours,entry[i].time.minutes,entry[i].time.seconds*2);
				    	printf("%7d KB|",entry[i].size/1024 +1);
				    	printf("      %s\t|",entry[i].wide_dissection);
						}	    	
					}
					printf("\n\t\t+-------------------------------------------------------------------------------+");
				}
				else if(key != 0)
				{
					//read_file(key + 2);
					struct Node head; 
					a.Head = &head;/*declare address of node head*/
					uint8_t data[512];
					a.Head->data=entry[key+1].cluster;
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
					do
					{
						printf("\nenter '0' to back:");
						scanf("%d",&key);
						if(key!=0)
						{
							printf("\nplease enter return");
						}
					}
					while(key!=0);
					read_root_directory(entry,&num_entry,(entry[0].cluster+31),1);
					printf("\n\t\t+----------------------------------director-------------------------------------+");
					printf("\n\t\t| Option |   Name        |       Date modifiled       |   Size   |      Type    |");
					for(i=0;i<num_entry;i++)
				   {
				   	if(entry[i].wide_dissection[0] == 32 &&  entry[i].name[0] != 0x2e)
				    	{
				    	printf("\n\t\t|  %3d   |%8s",i-1,entry[i].name);
				    	printf("%3s\t |",entry[i].wide_dissection);
						printf("     %d-%02d-%02d",1980+entry[i].date.years,entry[i].date.months,entry[i].date.days);
				    	printf("     %02d:%02d:%02d|",entry[i].time.hours,entry[i].time.minutes,entry[i].time.seconds*2);
						printf("\t\t |\tFolder\t|");
						}
						else if(entry[i].name[0] != 0x2e)
						{
				    	printf("\n\t\t|  %3d   |%8s",i-1,entry[i].name);
				    	printf(".%3s\t |",entry[i].wide_dissection);
						printf("     %d-%02d-%02d",1980+entry[i].date.years,entry[i].date.months,entry[i].date.days);
				    	printf("     %02d:%02d:%02d|",entry[i].time.hours,entry[i].time.minutes,entry[i].time.seconds*2);
				    	printf("%7d KB|",entry[i].size/1024 +1);
				    	printf("      %s\t|",entry[i].wide_dissection);
						}	    	
					}
					printf("\n\t\t+-------------------------------------------------------------------------------+");
					key = -1;
					//read_director(1);	
				}
		  	}
		  	key=-1;
		  	pre_director=-1;
		}
		else
		{
			///read_file(key);
			struct Node head; 
			a.Head = &head;/*declare address of node head*/
			uint8_t data[512];
			a.Head->data=entry[key-1].cluster;
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
			do
			{
				printf("\nenter '0' to back:");
				scanf("%d",&key);
				if(key!=0)
				{
					printf("\nplease enter return");
				}
			}
			while(key!=0);
			key = -1;
		}
 	}

	if(close_file()==0U)
	{
		printf("\nclose file sucessful");
	}
	return 0;
}
