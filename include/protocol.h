/**************************************
 *
 * Programme : protocolArbitre.h
 *
 * Synopsis : entete du protocole d'acces à l'arbitre
 *            pour Foursight
 *
 * Ecrit par : , 05 / 04 / 12
 * 
 ***************************************/

#ifndef _protocolArbitre_h
#define _protocolArbitre_h

/* Taille des chaines */
#define TAIL_CHAIN 30

/* Identificateurs des requetes */
typedef enum { IDENTIFICATION, PARTIE, COUP } TypIdRequest;

/* Types d'erreur */
typedef enum { ERR_OK,             /* Validation de la requete */
	       ERR_NOM,        /* Nom inconnu */
	       ERR_JOUEUR,     /* Identificateur de joueur inconnu */
	       ERR_COUP,       /* Erreur sur le coup joue */
	       ERR_TYP         /* Erreur sur le type de requete */
} TypErreur;

/* 
 * Structures d'identification 
 */
typedef struct {

  TypIdRequest idRequest;         /* Identificateur de la requete */
  char nom[TAIL_CHAIN];           /* Nom du joueur */

} TypIdentificationReq;

typedef struct{

  TypErreur err;                  /* Code d'erreur */
  int       joueur;               /* Identificateur du joueur */

} TypIdentificationRep;

/* 
 * Structures demande de partie
 */
typedef enum { FAUX, VRAI } TypBooleen;

typedef struct{

  TypIdRequest idRequest;         /* Identificateur de la requete */
  int          joueur;            /* Identificateur du joueur */

} TypPartieReq;

typedef struct {

  TypErreur  err;                 /* Code d'erreur */
  int        adversaire;          /* Identificateur de l'adversaire */
  TypBooleen finTournoi;          /* Indicateur de fin */
  TypBooleen premier;             /* Pour savoir qui debute la partie  */

} TypPartieRep;

/*
 * Definition des lignes et colonnes
 */
typedef enum { LI_ZERO, LI_UN, LI_DEUX, LI_TROIS } TypLigne;
typedef enum { CO_ZERO, CO_UN, CO_DEUX, CO_TROIS } TypColonne;

/* 
 * Definition d'une case
 */
typedef struct {

  TypLigne   ligne;
  TypColonne colonne;

} TypPosition ;

/* 
 * Structures coup du joueur pour l'arbitre
 */

/* Propriete des coups */
typedef enum { POSE, GAGNE, PERD, NULLE, PASSE } TypPropCoup;

typedef enum { VIDE, BLANC, ROUGE, JAUNE } TypPiece;

typedef struct {

  TypIdRequest idRequest;    /* Identificateur de la requete */
  TypPropCoup  propCoup;     /* Propriete du coup */
  TypPiece     typePiece;   /* Type de la piece */
  TypPosition  caseArrivee;  /* Position de la piece donnee */
  int          numeroDuCoup; /* numero du coup */

} TypCoupReq;

/* Validite du coup */
typedef enum { VALID, TIMEOUT, TRICHE } TypValCoup;

/* Reponse a un coup */
typedef struct {

  TypErreur err;                  /* Code d'erreur */
  TypValCoup validCoup;           /* Validite du coup */

} TypCoupRep;

#endif
