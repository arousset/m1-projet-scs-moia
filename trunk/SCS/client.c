/*
 **********************************************************
 *
 *  Programme : client.c
 *
 *  ecrit par : Rousset Alban & Kawczak Clement
 *
 *  resume : client pour jeux foursight
 *
 *  date :      05/03/12
 *
 ***********************************************************
 */

/* inclusions standards */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "fonctionsTCP.h"
#include "protocol.h"


int main(int argc, char** argv) {
  int sock,               /* descripteur de la socket locale */
      err;                /* code d'erreur */
  /*
   * verification des arguments
   */
  if (argc != 4) {
    printf("usage : client nom_machine no_port login \n");
    exit(1);
  }
  
  /* 
   * creation d'une socket
   */
  printf("client : connect to %s, %d\n avec le login : %s", argv[1], atoi(argv[2]), argv[3]);
  sock = socketClient(argv[1], atoi(argv[2]));
  if (sock < 0) { 
    printf("client : erreur socketClient\n");
    exit(2);
  }
  
	
	
	TypErreur ret_erreur;
	TypIdentificationRep ident_rep;
	TypIdentificationReq ident_req;
	TypPartieReq partie_req;
	TypPartieRep partie_rep;

	// Création d'une requete 
	ident_req.idRequest = IDENTIFICATION;
	strcpy(ident_req.nom, argv[3]);

	  /*
	   * envoi de la requete d'identification
	   */
	  err = send(sock, &ident_req, sizeof(TypIdentificationReq), 0);
	  if (err != sizeof(TypIdentificationReq)) {
	    perror("client : erreur sur le send");
	    shutdown(sock, 2); close(sock);
	    exit(3);
	  }
	  printf("client : envoi realise identification\n");

	// On attend la réponse du serveur
	err = recv(sock, &ident_rep, sizeof(TypIdentificationRep), 0);
  	if (err < 0) {
	    perror("client -> erreur dans la reception");
	    shutdown(sock, 2); close(sock); close(sock);
	    exit(4);
  	}
	
	
	printf("Code erreur : %d\n", ident_rep.err);
	printf("Identifiant Joueur :%d\n", ident_rep.joueur);

	// Création de la requete de partie
	partie_req.idRequest = PARTIE;
	partie_req.joueur = ident_rep.joueur;

	
	
	// Envoi de la requete pour une partie
	err = send(sock, &partie_req, sizeof(TypPartieReq), 0);
	  if (err != sizeof(TypPartieReq)) {
	    perror("client : erreur sur le send");
	    shutdown(sock, 2); close(sock);
	    exit(3);
	  }
	  printf("client : envoi realise pour une partie\n");

	
	err = recv(sock, &partie_rep, sizeof(TypPartieRep), 0);
  	if (err < 0) {
	    perror("client -> erreur dans la reception");
	    shutdown(sock, 2); close(sock); close(sock);
	    exit(4);
  	}


	// On affiche reception demande partie
	printf("%d\n", partie_rep.err);
	printf("%d\n", partie_rep.adversaire);
	printf("%d\n", partie_rep.finTournoi);
	printf("%d\n", partie_rep.premier);
}

