#ifndef __BATAILLE__
#define __BATAILLE__

#include "plateau.h"

// LES FONCTIONS POUR LES BATAILLES


/**
 * @brief Determine s'il existe deux camps sur la meme case
 * 
 * @param caseActuelle 
 * @return int 
 */
int collision(Case caseActuelle);


/**
 * @brief 0 - abeille gagne, 1 - frelon gagne

 * 
 * @param unite_Abeille 
 * @param unite_Frelon 
 * @return int 
 */
int bataille(Unite *unite_Abeille, Unite *unite_Frelon);


/**
 * @brief A la fin de tour, decrementation de tous les colonies et unites d'un camp
 * 
 * @param grille 
 * @param colonie 
 */
void decremente(Grille **grille, UListe *colonie);


/**
 * @brief A la fin de tour, decrementation de deux camps
 * 
 * @param grille 
 */
void decremente_Tout(Grille **grille);



#endif
