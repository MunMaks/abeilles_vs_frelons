// la déclaration des fonctions de abeille.c
#ifndef __ABEILLE__
#define __ABEILLE__

#include "plateau.h"

// LES FONCTIONS pour les abeilles

/**
 * @brief Creation une nouvelle Ruche par une reine des abeilles
 * 
 * @param reine 
 * @return Unite* 
 */
Unite *create_Ruche(Unite *reine);


/**
 * @brief Creation une nouvelle abeille par une ruche
 * 
 * @param ruche 
 * @param type 
 * @param force 
 * @param temps 
 * @return Unite* 
 */
Unite* create_Abeille(UListe ruche, char type, int force, int temps);

/**
 * @brief Detruit l'abeille par son adress
 * 
 * @param abeille 
 * @return int 
 */
int supprimer_Abeille(UListe abeille); 


#endif
