#ifndef _READ_FILE_
#define _READ_FILE_
typedef struct{
	uint16_t seconds:5;
	uint16_t minutes:6;
	uint16_t hours :5;
} time;
typedef struct{
	uint16_t days:5;
	uint16_t months:4;
	uint16_t years:7;
} date;
typedef struct {
	uint16_t bytePerSector;
	uint8_t clusterPerSector;
	uint16_t sectorPerFat12;
	uint16_t sectorBootDirector;
	uint32_t numSector;
	uint8_t typeFat[9];
} data_boot_sector;
typedef struct{
	uint8_t name[9];
	uint8_t wide_dissection[4];
	time time;
	date date;
	uint32_t size;
	uint32_t cluster;
} data_entry;
enum code_erorr{
	correct,
	incorrect
};
struct Node {
    uint16_t data;
    struct Node *pNext;
};
typedef struct {
    struct Node *Head;
}LinkedList;
uint8_t AddTail(LinkedList*, uint16_t value);
uint8_t free_allocate_dynamic(LinkedList*);
uint8_t open_file(char*);
uint8_t close_file(void);
uint8_t read_boot_sector(data_boot_sector *);
uint8_t read_root_directory(data_entry*, uint8_t*, uint32_t, uint16_t);
uint8_t read_data(uint32_t,  uint8_t*);
uint32_t read_fat_table(uint16_t clusStr, uint16_t secPerFat, LinkedList *);
#endif /*_READ_FILE_*/
