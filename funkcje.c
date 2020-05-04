/*
 * funkcje.c
 *
 *  Created on: 8 gru 2018
 *      Author: Szymon MAKOWSKI
 */
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <linux/if_ether.h>
#include "header.h"


void wypisz_eth(unsigned char* buffer,struct ramka* wsk_rmk,struct ethernet* wsk_eth){


	for(int j=0,k=0; j<6; j++,k++) {
		wsk_eth->MAC_des[k]=buffer[j];
	}

	for(int j=6,k=0; j<12; j++,k++) {
		wsk_eth->MAC_src[k]=buffer[j];
	}

	for(int j=12,k=0; j<14; j++,k++) {
		wsk_eth->typ_ramki[k]=buffer[j];
	}

	wsk_rmk->wsk_rmk_eth=wsk_eth;

}

void wypisz_ip(unsigned char* buffer,struct ramka* wsk_rmk,struct ipv6 * wsk_ip_6){
	wsk_rmk->wsk_rmk_ipv6=wsk_ip_6;

	for(int j=14;j<15;j++) {
		wsk_ip_6->version=(buffer[j]>>4);
	}
	for(int j=14;j<15;j++) {
		wsk_ip_6->traffic_class=((buffer[j]<<4)|(buffer[j+1]>>4));
	}
	for(int j=18,k=0; j<20; j++,k++) {
		wsk_ip_6->payload_length[k]=buffer[j];
	}
	for(int j=20;j<21;j++) {
		wsk_ip_6->next_header=buffer[j];
	}
	for(int j=21;j<22;j++) {
		wsk_ip_6->hop_limit=buffer[j];
	}
	for(int j=22,k=0;j<38;j++,k++) {
		wsk_ip_6->src_ipv6[k]=buffer[j];
	}
	for(int j=38,k=0;j<54;j++,k++) {
		wsk_ip_6->des_ipv6[k]=buffer[j];
	}

}

void wypisz_tcp(unsigned char* buffer,struct ramka* wsk_rmk,struct tcp* wsk_tcp){

	wsk_rmk->wsk_rmk_tcp=wsk_tcp;

	unsigned char tmp;

	for(int j=54,k=0;j<56;j++,k++) {
		wsk_tcp->source_port[k]=buffer[j];
	}
	for(int j=56,k=0;j<58;j++,k++) {
		wsk_tcp->destination_port[k]=buffer[j];
	}
	for(int j=58,k=0;j<62;j++,k++) {
		wsk_tcp->sequence_number[k]=buffer[j];
	}
	for(int j=62,k=0;j<66;j++,k++) {
		wsk_tcp->acknowledgment_number[k]=buffer[j];
	}

	tmp=buffer[66]>>4;
	wsk_tcp->data_offset=tmp;

	tmp=buffer[66]<<4;
	wsk_tcp->reserved_1=tmp>>4;

	tmp=buffer[67]>>6;
	wsk_tcp->reserved_2=tmp;

	tmp=buffer[67]<<2;
	wsk_tcp->control_bits=tmp>>2;

	for(int j=68,k=0;j<70;j++,k++) {
		wsk_tcp->window[k]=buffer[j];
	}

	for(int j=70,k=0;j<72;j++,k++) {
		wsk_tcp->checksum[k]=buffer[j];
	}

	for(int j=72,k=0;j<74;j++,k++) {
		wsk_tcp->urgent_pointer[k]=buffer[j];
	}

	if (((wsk_tcp->data_offset)-20)>0) {
		tmp=(wsk_tcp->data_offset-20);
		for(int j=74,k=0;k<tmp;j++,k++) {
			wsk_tcp->options[k]=buffer[j];
		}
		tmp=0;
	}

}

struct lista* stworzNowaLista(struct ramka* x) {
	struct lista* nowaLista = (struct lista*)malloc(sizeof(struct lista));
	nowaLista->dane=x;
	nowaLista->prev = NULL;
	nowaLista->next = NULL;
	return nowaLista;
	}

void dodajElement(struct ramka* x) {
	struct lista* nowaLista = stworzNowaLista(x);
	if(head == NULL) {
		head = nowaLista;
		return;
	}
	head->prev = nowaLista;
	nowaLista->next = head;
	head = nowaLista;
}

void drukuj() {
	struct lista* temp = head;
	//****************ETH2*****************
	while(temp!=NULL){
	int j=1;
	//	temp->dane->wsk_rmk_eth;
	printf("**********RAMKA %d ***************\n",j);
	printf("ETHERNET\n");
		printf("\n MAC odbiorcy: %.2x:%.2x:%.2x:%.2x:%.2x:%.2x", temp->dane->wsk_rmk_eth->MAC_des[0],
				temp->dane->wsk_rmk_eth->MAC_des[1], temp->dane->wsk_rmk_eth->MAC_des[2], temp->dane->wsk_rmk_eth->MAC_des[3],
				temp->dane->wsk_rmk_eth->MAC_des[4], temp->dane->wsk_rmk_eth->MAC_des[5]);
		printf("\n MAC nadawcy:  %.2x:%.2x:%.2x:%.2x:%.2x:%.2x", temp->dane->wsk_rmk_eth->MAC_src[0],
				temp->dane->wsk_rmk_eth->MAC_src[1], temp->dane->wsk_rmk_eth->MAC_src[2], temp->dane->wsk_rmk_eth->MAC_src[3],
				temp->dane->wsk_rmk_eth->MAC_src[4], temp->dane->wsk_rmk_eth->MAC_src[5]);
		printf("\n Typ:          %.2x:%.2x", temp->dane->wsk_rmk_eth->typ_ramki[0],
				temp->dane->wsk_rmk_eth->typ_ramki[1]);
	printf("\n");
	//****************IPV6*****************

	//temp->dane->wsk_rmk_ipv6;

	printf("\nIPv6\n");
	printf("\n Wersja: %.2x",temp->dane->wsk_rmk_ipv6->version);
	printf("\n Klasa ruchu: %.2x",temp->dane->wsk_rmk_ipv6->traffic_class);
	printf("\n Długosc zawartosci: %.2x:%.2x",temp->dane->wsk_rmk_ipv6->payload_length[0],
			temp->dane->wsk_rmk_ipv6->payload_length[1]);
	printf("\n Nastepny naglowek: %.2x",temp->dane->wsk_rmk_ipv6->next_header);
	printf("\n Limit skokow: %.2x\n",temp->dane->wsk_rmk_ipv6->hop_limit);
	printf(" ADRES zrodla: ");

	for(int j=0;j<16;j++) {

		if(j==15)

			printf("%.2x\n",temp->dane->wsk_rmk_ipv6->src_ipv6[j]);
		else

			printf("%.2x:",temp->dane->wsk_rmk_ipv6->src_ipv6[j]);
	}
	printf(" ADRES celu: ");

	for(int j=0;j<16;j++) {

		if(j==15)

			printf("%.2x\n",temp->dane->wsk_rmk_ipv6->des_ipv6[j]);
		else

			printf("%.2x:",temp->dane->wsk_rmk_ipv6->des_ipv6[j]);
	}
	//*************TCP*************

	//temp->dane->wsk_rmk_tcp;

	printf ("\nTCP\n");
	printf ("\nPort zrodlowy: %.2x:%.2x",temp->dane->wsk_rmk_tcp->source_port[0],temp->dane->wsk_rmk_tcp->source_port[1]);
	printf ("\nPort docelowy: %.2x:%.2x",temp->dane->wsk_rmk_tcp->destination_port[0],
			temp->dane->wsk_rmk_tcp->destination_port[1]);
	printf ("\nNumer sekwencyjny: %.2x:%.2x:%.2x:%.2x",temp->dane->wsk_rmk_tcp->sequence_number[0],
			temp->dane->wsk_rmk_tcp->sequence_number[1],temp->dane->wsk_rmk_tcp->sequence_number[2],
			temp->dane->wsk_rmk_tcp->sequence_number[3]);
	printf ("\nNumer potwierdzenia: %.2x:%.2x:%.2x:%.2x",temp->dane->wsk_rmk_tcp->acknowledgment_number[0],
			temp->dane->wsk_rmk_tcp->acknowledgment_number[1],temp->dane->wsk_rmk_tcp->acknowledgment_number[2],
			temp->dane->wsk_rmk_tcp->acknowledgment_number[3]);
	printf ("\nDlugosc naglowka: %.2x",temp->dane->wsk_rmk_tcp->data_offset);
	printf ("\nZarezerwowane: %.2x",temp->dane->wsk_rmk_tcp->reserved_1);
	printf ("\nZarezerwowane: %.2x",temp->dane->wsk_rmk_tcp->reserved_2);
	printf ("\nBity kontrolne: %.2x",temp->dane->wsk_rmk_tcp->control_bits);
	printf ("\nOkno: %.2x:%.2x", temp->dane->wsk_rmk_tcp->window[0],temp->dane->wsk_rmk_tcp->window[1]);
	printf ("\nCHKS: %.2x:%.2x", temp->dane->wsk_rmk_tcp->checksum[0],temp->dane->wsk_rmk_tcp->checksum[1]);
	printf ("\nPilnosc: %.2x:%.2x\n", temp->dane->wsk_rmk_tcp->urgent_pointer[0],temp->dane->wsk_rmk_tcp->urgent_pointer[1]);


	temp=(temp->next);

	}
}
