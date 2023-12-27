#include "plateau.h"


Unite * initialisation_ruche(void){
	Unite * A_Ruche = NULL;
    if (NULL == (A_Ruche = (Unite *)malloc(sizeof(Unite)))) {
        fprintf(stderr, "Pas de mémoire pour Abeille Ruche\n");
		return NULL;
	}
	A_Ruche->camp = ABEILLES;
	A_Ruche->type = RUCHE;
	A_Ruche->force = 0;
	A_Ruche->posx = 0, A_Ruche->posy = 0;
	A_Ruche->destx = -1, A_Ruche->desty = -1;
	A_Ruche->production = -1;
	A_Ruche->temps = 0;
	A_Ruche->toursrestant = 0; 			// QUESTION
	A_Ruche->usuiv = NULL, A_Ruche->uprec = NULL;
	A_Ruche->colsuiv = NULL, A_Ruche->colprec = NULL;
	A_Ruche->vsuiv = NULL, A_Ruche->vprec = NULL;

	return A_Ruche;
}


Unite *initialisation_reine(void) {
    Unite *A_Reine = NULL;
    if (NULL == (A_Reine = (Unite *)malloc(sizeof(Unite)))) {
        fprintf(stderr, "Pas de mémoire pour Abeille Reine\n");
        return NULL;
    }

    A_Reine->camp = ABEILLES;
    A_Reine->type = REINE;
    A_Reine->force = 0;
    A_Reine->posx = 0, A_Reine->posy = 0;
    A_Reine->destx = -1, A_Reine->desty = -1;
    A_Reine->production = -1;
    A_Reine->temps = -1;
    A_Reine->toursrestant = 1;
    A_Reine->usuiv = NULL, A_Reine->uprec = NULL;
    A_Reine->colsuiv = NULL, A_Reine->colprec = NULL;
    A_Reine->vsuiv = NULL, A_Reine->vprec = NULL;

    return A_Reine;
}

Unite *initialisation_ouvriere(void) {
    Unite *A_Ouvriere = NULL;
    if (NULL == (A_Ouvriere = (Unite *)malloc(sizeof(Unite)))) {
        fprintf(stderr, "Pas de mémoire pour Abeille Ouvriere\n");
        return NULL;
    }

    A_Ouvriere->camp = ABEILLES;
    A_Ouvriere->type = OUVRIERE;
    A_Ouvriere->force = 0;
    A_Ouvriere->posx = 0, A_Ouvriere->posy = 0;
    A_Ouvriere->destx = -1, A_Ouvriere->desty = -1;
    A_Ouvriere->production = -1;
    A_Ouvriere->temps = -1;
    A_Ouvriere->toursrestant = 1;
    A_Ouvriere->usuiv = NULL, A_Ouvriere->uprec = NULL;
    A_Ouvriere->colsuiv = NULL, A_Ouvriere->colprec = NULL;
    A_Ouvriere->vsuiv = NULL, A_Ouvriere->vprec = NULL;

    return A_Ouvriere;
}

Unite *initialisation_guerriere(void) {
    Unite *A_Guerriere = NULL;
    if (NULL == (A_Guerriere = (Unite *)malloc(sizeof(Unite)))) {
        fprintf(stderr, "Pas de mémoire pour Abeille Guerriere\n");
        return NULL;
    }

    A_Guerriere->camp = ABEILLES;
    A_Guerriere->type = GUERRIERE;
    A_Guerriere->force = 0;
    A_Guerriere->posx = 0, A_Guerriere->posy = 0;
    A_Guerriere->destx = -1, A_Guerriere->desty = -1;
    A_Guerriere->production = -1;
    A_Guerriere->temps = -1;
    A_Guerriere->toursrestant = 1;
    A_Guerriere->usuiv = NULL, A_Guerriere->uprec = NULL;
    A_Guerriere->colsuiv = NULL, A_Guerriere->colprec = NULL;
    A_Guerriere->vsuiv = NULL, A_Guerriere->vprec = NULL;

    return A_Guerriere;
}









UListe initialisation_abeilles(void){
	// Camp ABEILLE au debut: 1 ruche, 1 reine, 1 guerrière et 1 ouvrière
	Unite *A_Ruche = initialisation_ruche();
	Unite *A_Reine = initialisation_reine();
	Unite *A_Ouvriere = initialisation_ouvriere();
	Unite *A_Guerriere = initialisation_guerriere();

	A_Ruche->usuiv = A_Reine;
    A_Ruche->uprec = NULL;

    A_Reine->usuiv = A_Guerriere;
    A_Reine->uprec = A_Ruche;

    A_Guerriere->usuiv = A_Ouvriere;
    A_Guerriere->uprec = A_Reine;

    A_Ouvriere->usuiv = NULL;
    A_Ouvriere->uprec = A_Guerriere;

	return A_Ruche;
}

