#ifndef __PLATEAU__
#define __PLATEAU__

#include <stdio.h>
#include <stdlib.h>
// #include <MLV/MLV_all.h>
#include <time.h>
// #include <math.h>
#include <string.h>


// Dimensions de la grille en nombre de cases (origine en haut a gauche) :
// début: (0, 0), fin: (17, 11) 
#define LIGNES 18
#define COLONNES 12

// Les deux camps :
#define ABEILLES 'A'
#define FRELONS 'F'

// Les types d'unites :
#define REINE 'r'
#define OUVRIERE 'o'
#define ESCADRON 'e'
#define GUERRIERE 'g'
#define FRELON 'f'
#define RUCHE 'R'
#define NID 'N'

// Pour la recolte de pollen
#define RECOLTE 'p'

// Les temps necessaires a la production abeilles :
#define TREINEA 8
#define TOUVRIERE 2
#define TGUERRIERE 4
#define TESCADRON 6
#define TRECOLTE 4

// Les couts necessaires a la production abeilles :
#define CRUCHE 10
#define CREINEA 7
#define COUVRIERE 3
#define CGUERRIERE 5
#define CESCADRON 6


// Les temps necessaires a la production frelons :
#define TREINEF 8
#define TFRELON 5

// Les couts necessaires a la production frelons :
#define CNID 10
#define CREINEF 8
#define CFRELON 3


// La force des unites
#define FREINE 6
#define FOUVRIERE 1
#define FGUERRIERE 5
#define FESCADRON 12
#define FFRELON 8


/********************************************************/
/************** Les structures pour le jeu **************/
/********************************************************/


// La structure Unite :
typedef struct unite {
	char camp; 								// ABEILLES ou FRELONS
	char type; 								// RUCHE, NID, REINE, OUVRIERE, GUERRIERE, ESCADRON ou FRELON
	int force; 								// la force de l'unite
	int posx, posy; 						// position actuelle sur la grille
	int destx, desty; 						// destination (negatif si immobile)
	char production; 						// production d'une ruche ou d'un nid et RECOLTE pour la recolte de pollen
	int temps; 								// nombres de tours total pour cette production
	int toursrestant; 						// tours restant pour cette production
	struct unite *usuiv, *uprec;			// liste des unites affiliees a une ruche ou un nid
	struct unite *colsuiv, *colprec;		// liste des autres ruches ou nids (colonies) du même camp
	struct unite *vsuiv, *vprec;			// liste des autres unites sur la meme case
} Unite, *UListe;


// La structure Case :
typedef struct {
	Unite *colonie; 		// S'il y a une ruche ou un nid sur la case
	UListe occupant; 		// les autres occupants de la case
} Case;


// La structure Grille :
typedef struct {
	Case plateau[LIGNES][COLONNES];
	UListe abeille, frelon;
	int tour; 				// Numero du tour
	int ressourcesAbeille, ressourcesFrelon;
} Grille;



/********************************************************/
/************** LES FONCTIONS pour abeille **************/
/********************************************************/

/**
 * @brief Initialisation de ruche au debut
 * 
 * @return Unite* 
 */
Unite *initialisation_ruche(void);


/**
 * @brief Initialisation de reine abeille au debut
 * 
 * @return Unite* 
 */
Unite *initialisation_reine_abeille(void);


/**
 * @brief Initialisation de ouvriere au debut
 * 
 * @return Unite* 
 */
Unite *initialisation_ouvriere(void);


/**
 * @brief Initialisation de guerriere au debut
 * 
 * @return Unite* 
 */
Unite *initialisation_guerriere(void);


/**
 * @brief Initialisation de CAMP ABEILLE
 * 
 * @return UListe 
 */
UListe initialisation_abeilles(void);


/********************************************************/
/************** LES FONCTIONS pour frelons **************/
/********************************************************/

/**
 * @brief Initialisation un nid au debut
 * 
 * @return Unite* 
 */
Unite *initialisation_nid(void);


/**
 * @brief Initialisation une reine de frelon au debut
 * 
 * @return Unite* 
 */
Unite *initialisation_reine_frelon(void);


/**
 * @brief Initialisation deux frelons au debut
 * 
 * @return Unite* 
 */
Unite *initialisation_frelon(void);


/**
 * @brief Initialisation de CAMP FRELON
 * 
 * @return UListe 
 */
UListe initialisation_frelons(void);


/*******************************************************/
/************** LES FONCTIONS pour grille **************/
/*******************************************************/


/**
 * @brief Liberer la memoire du grille
 * 
 * @param grille 
 */
void liberer_Grille(Grille *grille);

/**
 * @brief Initialisation du plateau au debut
 * 
 * @return Grille* 
 */
Grille *initialiserGrille(void);


/*************************************************/
/************** AJOUT / SUPPRESSION **************/
/*************************************************/


/**
 * @brief Effectue l'ajout d'une colonie a l'autre (reciproquement), return 1 succes ou 0 si echec
 * 
 * @param colonie_un 
 * @param colonie_deux 
 * @return int 
 */
int ajoute_colonie_fin(UListe *colonie_un, UListe colonie_deux);


/**
 * @brief Effectue l'ajout d'un insecte a la fin de la colonie, return 1 succes ou 0 si echec
 * 
 * @param colonie 
 * @param new_insecte 
 * @return int 
 */
int ajoute_insecte_fin(UListe *colonie, Unite *new_insecte);


/**
 * @brief Effectue l'ajout d'une unite(ou colonie) sur la case[ligne][colonne], return 1 succes ou 0 si echec
 * 
 * 
 * @param grille 
 * @param unite 
 * @param ligne 
 * @param colonne 
 * @return int 
 */
int ajoute_unite_case(Grille *grille, Unite *unite, int ligne, int colonne);


/**
 * @brief Effectue la suppression d'une unite d'une Colonie, return 1 succes ou 0 si echec
 * 
 * @param colonie 
 * @param unite 
 * @param deleted_unite 
 * @return int 
 */
int supprimerUnite(UListe *colonie, Unite *unite, Unite **deleted_unite);

/**
 * @brief Effectue la suppression d'une unite de la case[ligne][colonne], return 1 succes ou 0 si echec
 * 
 * @param grille 
 * @param unite 
 * @param ligne 
 * @param colonne 
 * @return int 
 */
int supprime_unite_case(Grille *grille, Unite *unite, int ligne, int colonne);

/**
 * @brief Renvoie le nombre de "ressources abeilles" apres la destruction d'une Ruche par les frelons
 * 
 * @param grille 
 * @param A_colonie 
 * @return int 
 */
int detruire_colonie_et_rss_abeilles(Grille *grille, UListe A_colonie);


#endif
