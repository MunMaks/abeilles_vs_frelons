#ifndef __PLATEAU__
#define __PLATEAU__

#include "ajoute_supp.h"



/******************* ABEILLES ********************/


/**
 * @brief Initialisation au debut une unite des abeilles
 * 
 * @param type 
 * @param force 
 * @return Unite* 
 */
Unite* initialisation_unite_A(char type, int force);

/**
 * @brief Initialisation au debut des unite des abeilles et la Ruche
 * 
 * @param grille 
 */
void initialisation_abeilles(Grille **grille);



/******************** FRELONS ********************/


/**
 * @brief Initialisation au debut une unite des frelons
 * 
 * @param type 
 * @param force 
 * @return Unite* 
 */
Unite* initialisation_unite_F(char type, int force);

/**
 * @brief Initialisation au debut des unite des frelons et le Nid
 * 
 * @param grille 
 */
void initialisation_frelons(Grille **grille);



/******************** GRILLE *********************/


/**
 * @brief Creation de la grille au debut du jeu
 * 
 * @return Grille* 
 */
Grille *initialiserGrille(void);


/**
 * @brief Liberer toutes la memoire prise par la grille
 * 
 * @param grille 
 */
void liberer_Grille(Grille **grille);


/**
 * @brief Creation une unite de type souhaitee
 * 
 * @param colonie 
 * @param type 
 * @return Unite* 
 */
Unite *creation_Unite(UListe *colonie, char type);


/**
 * @brief Creation une colonie de type souhaitee (Ruche / Nid)
 * 
 * @param reine 
 * @return UListe 
 */
UListe creation_Colonie(Unite **reine);


/**
 * @brief Effectue l'achat d'une nouvelle unite par une colonie
 * 
 * @param grille 
 * @param colonie 
 * @param type 
 * @return int 
 */
int achat_Unite(Grille **grille, UListe *colonie, char type);


/**
 * @brief Effectue l'achat d'une nouvelle colonie par une reine
 * 
 * @param grille 
 * @param reine 
 * @return int 
 */
int achat_Colonie(Grille **grille, Unite **reine);


/**
 * @brief // -1: il y un problem, 1 la reine a deja construit, 0 la reine peut construire et on l'ajoute dans "reines_liste"
 * 
 * @param reines_liste 
 * @param nbr_reines 
 * @param reine 
 * @return int 
 */
int reine_deja_contruit(UListe *reines_liste, int *nbr_reines, Unite *reine);


/**
 * @brief Nombre total d'unites du type souhaite sur la case 
 * 
 * @param caseActuelle 
 * @param type 
 * @return int 
 */
int nbr_Unite_Case(Case caseActuelle, char type);


/**
 * @brief Nombre total d'unites sur la case
 * 
 * @param caseActuelle 
 * @return int 
 */
int nbr_Unites_totale_Case(Case caseActuelle);


/**
 * @brief Commence la recolte de pollen par une ouvriere
 * 
 * @param ouvriere 
 */
void start_recolte_Ouvrierre(Unite **ouvriere);


/**
 * @brief Commence la creation d'une nouvelle colonie par une reine
 * 
 * @param reine 
 */
void start_creation_Colonie(Unite **reine);


/**
 * @brief Quantite total d'ouvriere qui collectent de pollen
 * 
 * @param colonie 
 * @return int 
 */
int nb_recolte(UListe colonie);


/**
 * @brief Effectue l’achat d’une unite en fonction de qui achete et de quel type.
 * 
 * @param grille 
 * @param type 
 * @param tour 
 */
void ajouter_unite_selon_type(Grille *grille, char type, int tour);


/**
 * @brief Effectue l’achat d’une colonie en verifiant si la reine a deja constuit la colonie.
 Si elle peut constuire alors cette fonction effectue l'achat
 * 
 * @param grille 
 * @param reine 
 * @param reines_liste 
 * @param nbr_reines 
 * @param type 
 */
void ajouter_colonie_selon_type(Grille *grille, Unite *reine, UListe *reines_liste, int *nbr_reines, char type);


#endif
