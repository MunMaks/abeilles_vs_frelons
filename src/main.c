
#include "graphique.h"


void free_abeilles(void){
	fprintf(stderr, "Abeilles sont détruites! \n");
}

void free_frelons(void){
	fprintf(stderr, "Frelons sont détruites! \n");
}


// Il faut retravailler ici
Grille *initialiserGrille(void) {

    Grille *grille = NULL;

    if (NULL == (grille = (Grille *)malloc(sizeof(Grille))) ){
        fprintf(stderr, "Malloc n'a pas reussi a allouer la memoire, reessayez.\n");
        return NULL; 	// exit(EXIT_FAILURE);
    }

    grille->tour = 1;

    grille->ressourcesAbeille = 0;		// par defaut 0
    grille->ressourcesFrelon = 0;		// par defaut 0

 
    for (int i = 0; i < LIGNES; ++i) {
        for (int j = 0; j < COLONNES; ++j) {
			// Chaque case de plateau est initialisee par defaut NULL
            grille->plateau[i][j].colonie = NULL;	
            grille->plateau[i][j].occupant = NULL;
        }
    }
	
    grille->abeille = NULL;
    grille->frelon = NULL;

	grille->plateau[0][0].colonie = grille->abeille;                        // RUCHE
    grille->plateau[LIGNES - 1][COLONNES - 1].colonie = grille->frelon;     // NID


    return grille;
}






void libererPlateau(Grille *grille) {
    free(grille);
}



int main(int argc, char *argv[]){
	Grille *grille = initialiserGrille();

	
	libererPlateau(grille);

	printf("Memoire est liberee !\n");

    return 0;
}
