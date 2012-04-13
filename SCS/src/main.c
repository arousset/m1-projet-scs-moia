/*
 **********************************************************
 *
 *  Programme : 	main.c
 *
 *  ecrit par : 	Rousset Alban & Kawczak Clement
 *
 *  resume : 		main client pour jeux foursight
 *
 *  date :  		05/03/12
 *
 ***********************************************************
 */

#include "client.h"

int main(int argc, char** argv) {
		printf("C'est partie mon kiki !\n");
		
		int port;
		char *ip_arbitre;
		char login[10];
		
		int sockArbitre;
		int joueur;
		int adversaire;
		int nbCoup;
		
		TypBooleen finTournoi;
		TypBooleen finPartie;
		TypBooleen premier;
		
		CliErreur clienterreur;
		
		login[0] = 'A'; 
		login[1] = 'l';
		login[2] = 'b';
		login[3] = 'a';
		login[4] = 'n';
		login[5] = '\0';
		
		if(argc == 3) {
			ip_arbitre = argv[1];
			port = atoi(argv[2]);
			printf("ip arbitre : %s\n",ip_arbitre);
			printf("port %d\n",port);
			
			// on tente de se connecter au serveur et on s'authentifie !!! 
			clienterreur = connexion_client(ip_arbitre, port, &sockArbitre);
			clienterreur = authentification_client(sockArbitre, login, &joueur);
			
			
			// On déclare les requetes & rep
			TypCoupReq* coup = malloc(sizeof(TypCoupReq));

		
			// On doit créer une parti et jouer tantque ce n'est pas la fin
			do{
				// on instancie une partie
				clienterreur = askPartie_client(sockArbitre, joueur, &finTournoi, &premier, &adversaire); 
				
				// test erreur
				if(clienterreur != PARTIE_OK) {
					printf("c'est zouber pour cette fois !!!\n");
					return 1;
				}
				
				printf("c'est bon sa !\n");
			} while(!finTournoi);
			
		} else {
			printf("Erreur : Probleme passage parametres !!!\n");
			return -1;
		}
		
		// On free 
		free(coup);
		
		return 0;
}
