/*
 **********************************************************
 *
 *  Programme : 	client.h
 *
 *  ecrit par : 	Rousset Alban & Kawczak Clement
 *
 *  resume : 		entete client pour jeux foursight
 *
 *  date :  		05/03/12
 *
 ***********************************************************
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "fonctionsTCP.h"
#include "protocol.h"


typedef enum { 	CONN_OK, 
				CONN_ERR,
				IDENT_OK,	
				IDENT_ERR,  
				IDENT_LOGIN,
				PARTIE_OK,	
				PARTIE_ERR,  
				PARTIE_JOUEUR,
				COUP_OK,	
				COUP_ERR,   
				COUP_INVALIDE,  
				COUP_TIMEOUT
} CliErreur;



// connexion du client (creation socket) (RqConnexion)
int connexion_client(char ip[], int port, int *sockArbitre);

// identification client pour l'arbitre (RqIdentification)
int authentification_client(int sockArbitre, char loginLDAP[], int *ident_joueur);

// Demande partie (RqPartie)
int askPartie_client(int sockArbitre, int ident_joueur, TypBooleen *finTournoi, TypBooleen *premier, int *ident_adversaire); 

// Envoie un coup (RpCoup)
int sendCoup_client(int sockArbitre, TypCoupReq *lecoup);

// Attente Coup 
int waitCoup_client(int sockArbitre, TypCoupReq *adversaire_coup);

// TimeOut // faudra le gérer directement avec PROLOG se sera plus simple
//int timeOut_client(int sockArbitre); 
