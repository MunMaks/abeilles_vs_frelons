#ifndef __AJOUTE_SUPP__
#define __AJOUTE_SUPP__

#include <stdio.h>
#include <stdlib.h>
#include <MLV/MLV_all.h>
#include <time.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

// Affichage de la fenetre MLV
#define LARGEUR_CASE 60
#define NB_LIGNES 12
#define NB_COLONNES 18

// Les deux camps :
#define ABEILLES 'A'
#define FRELONS 'F'
#define MAX_REINES 12   // la quantite des reines qui on deja construit une colonie

#define SIZE_X_BOARD (NB_COLONNES * LARGEUR_CASE)
#define SIZE_Y_BOARD (NB_LIGNES * LARGEUR_CASE)
#define SIZE_X (SIZE_X_BOARD + 300)
#define SIZE_Y (SIZE_Y_BOARD + 200)

// Les types d'unites :
#define REINE 'r'
#define OUVRIERE 'o'
#define ESCADRON 'e'
#define GUERRIERE 'g'
#define FRELON 'f'
#define RUCHE 'R'
#define NID 'N'

// Pour la recolte de pollen
#define RECOLTE 'P'

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
	struct unite *colsuiv, *colprec;		// liste des autres ruches ou nids (colonies) du meme camp
	struct unite *vsuiv, *vprec;			// liste des autres unites sur la meme case
} Unite, *UListe;


// La structure Case :
typedef struct {
	Unite *colonie; 		// S'il y a une ruche ou un nid sur la case
	UListe occupant; 		// les autres occupants de la case
} Case;


// La structure Grille :
typedef struct {
	Case plateau[NB_COLONNES][NB_LIGNES];
	UListe abeille, frelon;
	int tour;
	int ressourcesAbeille, ressourcesFrelon;
} Grille;



/*************************************************/
/****************** SUPPRESSION ******************/
/*************************************************/

/**
 * @brief Effectue la suppression de lien entre unite et sa case
 * 
 * @param unite 
 * @return int 
 */
int supprime_Insecte_Case(Unite **unite);


/**
 * @brief Effectue la suppression de lien entre unite et sa colonie
 * 
 * @param unite 
 * @return int 
 */
int supprime_Insecte_Col(Unite **unite);


/**
 * @brief Effectue la suppression de lien entre colonie et d'autres colonies
 * 
 * @param colonie 
 * @return int 
 */
int supprime_Colonie(UListe *colonie);


/**
 * @brief Suppression des liaisons d'une unite (colonie ou insecte) sur la case
 * 
 * @param grille 
 * @param unite 
 * @return int 
 */
int supprime_Unite_Case(Grille **grille, Unite **unite);

/**
 * @brief Supprime tous les liens et libere la memoire d'une unite
 * 
 * @param unite 
 */
void detruire_Unite(Unite **unite);


/**
 * @brief Supprime tous les liens de la colonie et chaque unite affilees
 * 
 * @param grille 
 * @param colonie 
 */
void detruire_Colonie(Grille **grille, UListe *colonie);

/**
 * @brief Supprime tous les liens de la colonie et chaque unite affilees, ajoute les ressources aux frelons
 * 
 * @param grille 
 * @param A_colonie Abeille colonie
 * @return int quantite de ressources a ajouter pour les frelons
 */
int detruire_Colonie_et_rss_abeilles(Grille **grille, UListe *A_colonie);


/**
 * @brief Detruit tous les colonies d'un camp avec leurs unites et leurs liens sur chaque case
 * 
 * @param grille 
 * @param colonie 
 */
void liberer_des_colonies(Grille **grille, UListe *colonie);


/*************************************************/
/******************** AJOUTE *********************/
/*************************************************/


/**
 * @brief Effectue l'ajout de colonie_deux a la fin de colonie_un (colsuiv et colprec)
 * 
 * @param colonie_un 
 * @param colonie_deux 
 * @return int 
 */
int ajoute_Colonie(UListe *colonie_un, UListe colonie_deux);


/**
 * @brief Effectue l'ajout d'un insecte a la fin de sa colonie
 * 
 * @param colonie 
 * @param new_insecte 
 * @return int 
 */
int ajoute_Insecte(UListe *colonie, Unite *new_insecte);


/**
 * @brief Effectue l'ajout d'une unite (colonie ou unite) sur la case[x][y]
 * 
 * @param grille 
 * @param unite 
 * @param x 
 * @param y 
 * @return int 
 */
int ajoute_Unite_Case(Grille **grille, Unite **unite, int x, int y);



/*************************************************/
/************** Les fonctions utiles *************/
/*************************************************/


/**
 * @brief Affichage dans le terminal une colonie avec ses unites affiliees
 * 
 * @param colonie 
 */
void afficheColonie(UListe colonie);


/**
 * @brief Affichage dans le terminal la case
 * 
 * @param caseCourante 
 */
void afficheCase(Case caseCourante);


/**
 * @brief Verifie si l'unite cherche existe dans cette colonie
 * 
 * @param colonie 
 * @param unite 
 * @return int 1 oui, 0 non
 */
int uniteExiste(UListe colonie, Unite *unite);


/**
 * @brief Trouve la colonie d'une unite
 * 
 * @param unite 
 * @return UListe 
 */
UListe trouver_Colonie(Unite *unite);


/**
 * @brief Cherche la reine d'une colonie, si n'est pas trouvee, renvoie NULL
 * 
 * @param colonie 
 * @return Unite* 
 */
Unite *trouver_Reine(UListe colonie);


/**
 * @brief Renvoie 1 s'il n'y a pas de colonie ou d'unite, sinon 0
 * 
 * @param caseCourante 
 * @return int 
 */
int case_Est_Vide(Case caseCourante);


/**
 * @brief Cette fonction verifie si la colonie est la seule sur la case et s'il y a des unites adversaires
 * 
 * @param caseActuelle 
 * @return int en danger - 1, en securite - 0
 */
int case_Colonie_seul(Case caseActuelle);


/**
 * @brief Renvoie le prix necessaire pour acheter le type souhaitee
 * 
 * @param camp 
 * @param type 
 * @return int 
 */
int prix_Unite(char camp, char type);


/**
 * @brief Renvoie la force d'une unite d'apres son type
 * 
 * @param type 
 * @return int 
 */
int force_Unite(char type);


/**
 * @brief Renvoie le temps necessaire a l'apparition d'une unite.
 * 
 * @param camp 
 * @param type 
 * @return int 
 */
int temps_Unite(char camp, char type);


/**
 * @brief Decide qui fait le premier tour: 0 (abeilles) et 1 (frelons)
 * 
 * @param x 
 * @param y 
 * @return int 
 */
int tirage_au_hasard(int x, int y);


/**
 * @brief tirage aletoire entre 1 et 60 inclus
 * 
 * @return int 
 */
int tirageDe(void);


#endif
