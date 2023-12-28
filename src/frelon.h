#ifndef __FRELON__
#define __FRELON__

#include "plateau.h"

// la declaration des fonctions pour les frelons


Unite *creation_Nid(Unite *reineFrelon);

Unite *creation_Reine_frelon(void);

Unite* creation_Frelon(UListe ruche, char type, int force, int temps);




#endif
