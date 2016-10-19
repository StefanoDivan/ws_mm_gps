// mm_gps_bacon_h
// "class-like".C

#ifndef mm_gps_bacon_h
#define mm_gps_bacon_h

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>

#define MMGPS_NUMBER_OF_BEACONS 5
#define MMGPS_SEPARATOR "\xFFG" //perchè i primi due sono xff e poi 47, ovvero è la stringa del separatore
#define MMGPS_SEPLEN 2
 
#define MMGPS_MAX_PACKET_LENGTH MMGPS_SEPLEN + 4 + MMGPS_NUMBER_OF_BEACONS * 8 + 2 + MMGPS_SEPLEN

typedef char (*mm_gps_char_getter)(void *data); //definendo un nome che ha come argomento un dato void e restituisce un char

//Union è un modo di C e C++ per accedere alla stringa di una struttura. Una struttura composta da un uint8_t, uint16_t e char[4] è formata da 7 byte. Per accedere ai singoli byte uso la union,
typedef union {
	char b[MMGPS_MAX_PACKET_LENGTH];

	struct __attribute__((packed)) { //mappa che corrisponde al protocollo, __attribute__((packed)) dice di pachettare la struttura il più possibile per non avere buchi. Forza il compilatore.
		uint8_t address;
		uint8_t type;
		uint8_t code;
		uint8_t code_h; //high byte
		uint8_t size;
		uint32_t time;
		uint16_t x;
		uint16_t y;
		uint16_t z;
		uint8_t flag;
		char reserved[5];
		uint16_t crc16;
	} hedge;

	struct __attribute__((packed)){
		uint8_t address;
		uint8_t type;
		uint8_t code;
		uint8_t code_h; //high byte
		uint8_t size;
		uint8_t n_beacons;
		struct
		{
			uint16_t x;
			uint16_t y;
			uint16_t z;
			uint8_t reserved;
		} beacon[MMGPS_NUMBER_OF_BEACONS];
		
	} beacons;

}mm_gps_packet;

typedef struct 
{
	struct __attribute__((packed))
	{
		mm_gps_packet packet;
		char * head;
		uint16_t crc16;
	}buffer;

	void * userdata;
	mm_gps_char_getter get_next_char; //campo get_next_char che deve essere un puntatore o una funzione che ha questa segnatura assegnata sopra, ovvero accetta un void e restituisce un char
	
}mm_gps;

//Inizializzatore
mm_gps * mm_gps_init(void*data);

//Deallocatore
void mm_gps_free(mm_gps *gps);

void mm_gps_set_reader(mm_gps *gps, mm_gps_char_getter f); //funzione, il secondo elemento è il callback. 

uint16_t mm_gps_next_raw_packet(mm_gps *gps);

double mm_gps_time(mm_gps * gps);
double mm_gps_x(mm_gps * gps);
double mm_gps_y(mm_gps * gps);
double mm_gps_z(mm_gps * gps);

void mm_gps_coords(mm_gps * gps, double * coords);


#endif