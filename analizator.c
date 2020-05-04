/*
 ============================================================================
 Name        : 01.c Analizator Pakietów
 Author      : Szymon Makowski
 Version     : 0.02
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <sys/socket.h>
#include <linux/if_ether.h>
#include "header.h"



int main(void) {

	int ileDoBufora=0;
	int temp=0;

	struct ethernet eth;
	struct ethernet *wsk_eth=&eth;

	struct ipv6 ip_6;
	struct ipv6 *wsk_ip_6=&ip_6;

	struct tcp tc_p;
	struct tcp *wsk_tcp=&tc_p;

	struct ramka rmk;
	struct ramka *wsk_rmk=&rmk;

	printf("Uruchamiam odbieranie ramek Ethernet.\n"); /* prints */

	printf("Ile ramek wczytac do bufora?\n");
	while(scanf("%d", &ileDoBufora) != 1) //dopóki nie uda się wczytać
	{
	  printf("BLAD ! Podaj wartosc jeszcze raz.\n");// komunikat błędu
	  int c;
	  while((c = getchar()) != '\n' && c != EOF); //pętla wyciągająca znaki z bufora
	}

	//Utworzenie bufora dla odbieranych ramek Ethernet

	unsigned char* buffer =(void*) malloc(ETH_FRAME_LEN);

	//Otwarcie gniazda pozwalającego na odbiór wszystkich ramek Ethernet

	int iEthSockHandl = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));

	//Kontrola czy gniazdo zostało otwarte poprawnie, w przypadku bledu wyświetlenie komunikatu.

	if (iEthSockHandl<0)

		printf("Problem z otwarciem gniazda : %s!\n", strerror(errno));

	//Zmienna do przechowywania rozmiaru odebranych danych

	int iDataLen = 0;

	//Pętla nieskończona do odbierania ramek Ethernet

	while (1) {

		//Odebranie ramki z utworzonego wcześniej gniazda i zapisanie jej do bufora

		iDataLen = recvfrom(iEthSockHandl, buffer, ETH_FRAME_LEN, 0, NULL, NULL);

		//Kontrola czy nie było bledu podczas odbierania ramki

		if (iDataLen == -1)

			printf("Nie moge odebrac ramki: %s! \n", strerror(errno));

		else {
			if(temp<ileDoBufora){
				if(buffer[12]==134 && buffer[13]==221 && buffer[20]==6) {
					printf("\nOdebrano ramke Ethernet o rozmiarze: %d [B]\n", iDataLen);

					//*****************************Ethernet 2*****************************
					wypisz_eth(buffer,wsk_rmk,wsk_eth);

					//*****************************IPv6***********************************
					wypisz_ip(buffer,wsk_rmk,wsk_ip_6);

    				//******************************TCP************************************
					wypisz_tcp(buffer,wsk_rmk,wsk_tcp);

					dodajElement(wsk_rmk);
					temp++;
				}

			}
			else{
				drukuj();
				return EXIT_SUCCESS;
			}
			}

		}


	return EXIT_SUCCESS;
}
