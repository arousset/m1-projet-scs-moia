/*
 **********************************************************
 *
 *  Programme : 	client.c
 *
 *  ecrit par : 	Rousset Alban & Kawczak Clement
 *
 *  resume : 		client pour jeux foursight
 *
 *  date :  		05/03/12
 *
 ***********************************************************
 */

#include "client.h"

// connexion du client (creation socket) (RqConnexion)
int connexion_client(char ip[], int port, int *sockArbitre) {
		
	int ginette_la_socket;
	
	// Connexion du client sur le serveur
	ginette_la_socket = socketClient(ip,port);
	
	// Test erreur
	if(ginette_la_socket < 0) {
		printf("client =>  erreur socketClient\n");
		return CONN_ERR;
	}
	
	// On passe la socket
	*sockArbitre = ginette_la_socket;
	
	return CONN_OK;
}
 
 
// identification client pour l'arbitre (RqIdentification)
int authentification_client(int sockArbitre, char loginLDAP[], int *ident_joueur) {
				
	// On déclare la requete et la reponse
	TypIdentificationReq *identification_request = malloc(sizeof(TypIdentificationReq));
	TypIdentificationRep *identification_answer = malloc(sizeof(TypIdentificationRep));
	
	identification_request->idRequest = IDENTIFICATION;
	// On copie le login
	strcpy(identification_request->nom,loginLDAP);
	
	// => La requete d'authentification est remplie il faut l'envoyer
	int err;
	err = send(sockArbitre, identification_request, sizeof(TypIdentificationReq),0);
	
	// Test erreur
	if(err < 0) {
		printf("client =>  erreur identification\n");
		return IDENT_ERR;
	}
	
	// Attente de la réponse de la part de l'arbitre
	err = recv(sockArbitre, identification_answer, sizeof(TypIdentificationRep),0);
	// Test erreur
	if(err < 0) {
		printf("client =>  erreur identification Reponse\n");
		return IDENT_ERR;
	} else if(identification_answer->err == ERR_NOM) {
		printf("client =>  erreur identification loginLDAP\n");
		return IDENT_LOGIN;
	} else if(identification_answer->err == ERR_OK) {
		// tt est bon alors on affecte notre numero de joueur
		*ident_joueur = identification_answer->joueur;
	}
	
	return IDENT_OK;
}

// Demande partie (RqPartie)
int askPartie_client(int sockArbitre, int ident_joueur, TypBooleen *finTournoi, TypBooleen *premier, int *ident_adversaire) {
	
	// On déclare la requete et la reponse
	TypPartieReq *partie_request = malloc(sizeof(TypPartieReq));
	TypPartieRep *partie_answer = malloc(sizeof(TypPartieRep));
	
	partie_request->idRequest = PARTIE;
	partie_request->joueur = ident_joueur;
	
	// => La requete de demande de partie est remplie il faut l'envoyer
	int err;
	err = send(sockArbitre,partie_request,sizeof(TypPartieReq),0);
	// Test erreur
	if(err < 0) {
		printf("client =>  erreur demande partie\n");
		return PARTIE_ERR;
	}
	
	// Attente de la réponse de la part de l'arbitre
	err = recv(sockArbitre,partie_answer,sizeof(TypPartieRep),0);
    if(err < 0) {
		printf("client =>  erreur identification Reponse\n");
		return PARTIE_ERR;
	} else if(partie_answer->err == ERR_OK) {
		*finTournoi = partie_answer->finTournoi;
		
		// On test si fin du tournoi
		if(*finTournoi == FAUX) {
			printf("client =>  Que la fete commence ^^\n");
			// on initialise tt 
			*premier = partie_answer->premier;
			*ident_adversaire = partie_answer->adversaire;		
		} else {
			// fin de tournoi on remet tout a null
			premier = NULL;
			ident_adversaire = NULL;
		}
	} else if(partie_answer->err == ERR_JOUEUR) {
		printf("client =>  erreur partie joueur Reponse\n");
		return PARTIE_JOUEUR;
	} else {
		printf("client =>  erreur partie Reponse\n");
		return PARTIE_ERR;
	}
	
	free(partie_request);
	free(partie_answer);
	return PARTIE_OK;
}

// Envoie un coup (RpCoup)
int sendCoup_client(int sockArbitre, TypCoupReq *lecoup) {
	
	TypCoupRep *coup_reponse = malloc(sizeof(TypCoupRep));
	
	// On envoi le coup passé en paramètre
	int err;
	err = send(sockArbitre, lecoup, sizeof(TypCoupReq),0);
	if(err < 0) {
		printf("client =>  erreur envoi coup\n");
		return COUP_ERR;
	}
	
	err = recv(sockArbitre, coup_reponse, sizeof(TypCoupRep),0);
	if(err < 0) {
		printf("client =>  erreur reception coup\n");
		return COUP_ERR;	
	} else if(coup_reponse->err == ERR_OK) {
			switch(coup_reponse->validCoup) {
				case VALID :
					printf("client ==>  Reception coup valide\n");
				break;
				case TIMEOUT :
					printf("client =>  erreur timeout reception coup\n");
					return COUP_TIMEOUT;
				break;
				case TRICHE :
					printf("client =>  erreur reception coup invalide\n");
					return COUP_INVALIDE;
				break;
			}
	} else {
		printf("client =>  erreur coup\n");
		return COUP_ERR;
	}
	
	free(coup_reponse);
	return COUP_OK;
}

// Attente Coup 
int waitCoup_client(int sockArbitre, TypCoupReq *adversaire_coup) {
	
	TypCoupRep *adversaire_coup_rep = (TypCoupRep*) malloc(sizeof(TypCoupRep));
	
	int err;
	// On receptionne la validation coup de l'adversaire
	err = recv(sockArbitre, adversaire_coup, sizeof(TypCoupRep),0);
	if(err < 0) {
		printf("client =>  erreur wait coup reponse\n");
		return COUP_ERR;
	} else if(adversaire_coup_rep->err == ERR_OK) {
		switch(adversaire_coup_rep->validCoup) {
			case VALID :
				printf("client ==>  Reception coup valide\n");
			break;
			case TIMEOUT :
				printf("client =>  erreur timeout reception coup\n");
				return COUP_TIMEOUT;
			break;
			case TRICHE :
				printf("client =>  erreur reception coup invalide\n");
				return COUP_INVALIDE;
			break;
		}
	} else {
		return COUP_ERR;
	}
	
	
	// Reception du coup de l'adversaire
	err = recv(sockArbitre, adversaire_coup_rep, sizeof(TypCoupReq),0);
	if(err < 0) {
		printf("client => erreur reception coup adversaire");
		return COUP_ERR;
	}
	
	free(adversaire_coup_rep);
	return COUP_OK;
}

// TimeOut 
//int timeOut_client(int sockArbitre); 








