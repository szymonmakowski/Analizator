/*
 * header.h
 *
 *  Created on: 8 gru 2018
 *      Author: Szymon MAKOWSKI
 */


#ifndef HEADER_H_
#define HEADER_H_

struct ethernet {
	unsigned char MAC_des[6];
	unsigned char MAC_src[6];
	unsigned char typ_ramki[2];
};

struct ipv6 {
	unsigned char version : 4;
	unsigned char traffic_class;
	unsigned char payload_length[2];
	unsigned char next_header;
	unsigned char hop_limit;
	unsigned char src_ipv6[16];
	unsigned char des_ipv6[16];
};

struct tcp {
	unsigned char source_port[2];
	unsigned char destination_port[2];
	unsigned char sequence_number[4];
	unsigned char acknowledgment_number[4];
	unsigned char data_offset :4, reserved_1 :4;
	unsigned char reserved_2 :2, control_bits :6;
	unsigned char window[2];
	unsigned char checksum[2];
	unsigned char urgent_pointer[2];
	unsigned char options[32];
};

struct ramka {
	struct ethernet *wsk_rmk_eth;
	struct ipv6 *wsk_rmk_ipv6;
	struct tcp *wsk_rmk_tcp;
};

struct lista {

		struct ramka* dane;
		struct lista* next;
		struct lista* prev;
};


struct lista* head;

void wypisz_eth(unsigned char*,struct ramka*,struct ethernet*);
void wypisz_ip(unsigned char*,struct ramka*,struct ipv6*);
void wypisz_tcp(unsigned char*,struct ramka*,struct tcp*);
struct lista* stworzNowaLista(struct ramka*);
void dodajElement(struct ramka*);
void drukuj();
void usun(struct ramka*);


#endif /* HEADER_H_ */
