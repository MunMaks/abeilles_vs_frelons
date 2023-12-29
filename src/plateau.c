#include "plateau.h"


/**************************************************/
/******************** ABEILLES ********************/
/**************************************************/

UListe initialisation_ruche(void){
	Unite * A_Ruche = NULL;
    if (NULL == (A_Ruche = (Unite *)malloc(sizeof(Unite)))) {
        fprintf(stderr, "Pas de mémoire pour Ruche Abeille\n");
		return NULL;
	}
	A_Ruche->camp = ABEILLES;
	A_Ruche->type = RUCHE;
	A_Ruche->force = 0;
	A_Ruche->posx = 0, A_Ruche->posy = 0;
	A_Ruche->destx = -1, A_Ruche->desty = -1;
	A_Ruche->production = 0;
	A_Ruche->temps = 0;
	A_Ruche->toursrestant = 0; 			// QUESTION
	A_Ruche->usuiv = NULL, A_Ruche->uprec = NULL;
	A_Ruche->colsuiv = NULL, A_Ruche->colprec = NULL;
	A_Ruche->vsuiv = NULL, A_Ruche->vprec = NULL;

	return A_Ruche;
}


Unite *initialisation_reine_abeille(void) {
    Unite *A_Reine = NULL;
    if (NULL == (A_Reine = (Unite *)malloc(sizeof(Unite)))) {
        fprintf(stderr, "Pas de mémoire pour Reine Abeille\n");
        return NULL;
    }

    A_Reine->camp = ABEILLES;
    A_Reine->type = REINE;
    A_Reine->force = FREINE;
    A_Reine->posx = 0, A_Reine->posy = 0;
    A_Reine->destx = 0, A_Reine->desty = 0;
    A_Reine->production = 0;
    A_Reine->temps = 0;
    A_Reine->toursrestant = 0;
    A_Reine->usuiv = NULL, A_Reine->uprec = NULL;
    A_Reine->colsuiv = NULL, A_Reine->colprec = NULL;
    A_Reine->vsuiv = NULL, A_Reine->vprec = NULL;

    return A_Reine;
}


Unite *initialisation_ouvriere(void) {
    Unite *A_Ouvriere = NULL;
    if (NULL == (A_Ouvriere = (Unite *)malloc(sizeof(Unite)))) {
        fprintf(stderr, "Pas de mémoire pour Ouvriere Abeille\n");
        return NULL;
    }

    A_Ouvriere->camp = ABEILLES;
    A_Ouvriere->type = OUVRIERE;
    A_Ouvriere->force = FOUVRIERE;
    A_Ouvriere->posx = 0, A_Ouvriere->posy = 0;
    A_Ouvriere->destx = 0, A_Ouvriere->desty = 0;
    A_Ouvriere->production = 0;
    A_Ouvriere->temps = 0;
    A_Ouvriere->toursrestant = 0;
    A_Ouvriere->usuiv = NULL, A_Ouvriere->uprec = NULL;
    A_Ouvriere->colsuiv = NULL, A_Ouvriere->colprec = NULL;
    A_Ouvriere->vsuiv = NULL, A_Ouvriere->vprec = NULL;

    return A_Ouvriere;
}


Unite *initialisation_guerriere(void) {
    Unite *A_Guerriere = NULL;
    if (NULL == (A_Guerriere = (Unite *)malloc(sizeof(Unite)))) {
        fprintf(stderr, "Pas de mémoire pour Guerriere Abeille\n");
        return NULL;
    }

    A_Guerriere->camp = ABEILLES;
    A_Guerriere->type = GUERRIERE;
    A_Guerriere->force = FGUERRIERE;
    A_Guerriere->posx = 0, A_Guerriere->posy = 0;
    A_Guerriere->destx = 0, A_Guerriere->desty = 0;
    A_Guerriere->production = 0;
    A_Guerriere->temps = 0;
    A_Guerriere->toursrestant = 0;
    A_Guerriere->usuiv = NULL, A_Guerriere->uprec = NULL;
    A_Guerriere->colsuiv = NULL, A_Guerriere->colprec = NULL;
    A_Guerriere->vsuiv = NULL, A_Guerriere->vprec = NULL;

    return A_Guerriere;
}


UListe initialisation_abeilles(void){
	// Camp ABEILLES au debut: 1 ruche, 1 reine, 1 guerrière et 1 ouvrière
	UListe A_Ruche = initialisation_ruche();
	Unite *A_Reine = initialisation_reine_abeille();
	Unite *A_Guerriere = initialisation_guerriere();
	Unite *A_Ouvriere = initialisation_ouvriere();

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


/*************************************************/
/******************** FRELONS ********************/
/*************************************************/

UListe initialisation_nid(void){
    Unite * F_Nid = NULL;
    if (NULL == (F_Nid = (Unite *)malloc(sizeof(Unite)))) {
        fprintf(stderr, "Pas de mémoire pour Nid Frelon\n");
		return NULL;
	}
	F_Nid->camp = FRELONS;
	F_Nid->type = NID;
	F_Nid->force = 0;
	F_Nid->posx = 17, F_Nid->posy = 11;
	F_Nid->destx = -1, F_Nid->desty = -1;
	F_Nid->production = 0;
	F_Nid->temps = 0;
	F_Nid->toursrestant = 0; 			// QUESTION
	F_Nid->usuiv = NULL, F_Nid->uprec = NULL;
	F_Nid->colsuiv = NULL, F_Nid->colprec = NULL;
	F_Nid->vsuiv = NULL, F_Nid->vprec = NULL;

	return F_Nid;
}


Unite *initialisation_reine_frelon(void) {
    Unite *F_Reine = NULL;
    if (NULL == (F_Reine = (Unite *)malloc(sizeof(Unite)))) {
        fprintf(stderr, "Pas de mémoire pour Reine Frelon \n");
        return NULL;
    }

    F_Reine->camp = FRELONS;
    F_Reine->type = REINE;
    F_Reine->force = FREINE;
    F_Reine->posx = 17, F_Reine->posy = 11;
    F_Reine->destx = 0, F_Reine->desty = 0;
    F_Reine->production = 0;
    F_Reine->temps = 0;
    F_Reine->toursrestant = 0;
    F_Reine->usuiv = NULL, F_Reine->uprec = NULL;
    F_Reine->colsuiv = NULL, F_Reine->colprec = NULL;
    F_Reine->vsuiv = NULL, F_Reine->vprec = NULL;

    return F_Reine;
}


Unite *initialisation_frelon(void) {
    Unite *F_Frelon = NULL;
    if (NULL == (F_Frelon = (Unite *)malloc(sizeof(Unite)))) {
        fprintf(stderr, "Pas de mémoire pour Frelon \n");
        return NULL;
    }

    F_Frelon->camp = FRELONS;
    F_Frelon->type = REINE;
    F_Frelon->force = FFRELON;
    F_Frelon->posx = 17, F_Frelon->posy = 12;
    F_Frelon->destx = 0, F_Frelon->desty = 0;
    F_Frelon->production = 0;
    F_Frelon->temps = 0;
    F_Frelon->toursrestant = 0;
    F_Frelon->usuiv = NULL, F_Frelon->uprec = NULL;
    F_Frelon->colsuiv = NULL, F_Frelon->colprec = NULL;
    F_Frelon->vsuiv = NULL, F_Frelon->vprec = NULL;

    return F_Frelon;
}


UListe initialisation_frelons(void){
    // Camp FRELONS au debut: 1 nid, 1 reine, 2 frelons
	UListe F_Nid = initialisation_nid();
	Unite *F_Reine = initialisation_reine_frelon();
	Unite *F_Frelon_un = initialisation_frelon();
	Unite *F_Frelon_deux = initialisation_frelon();

	F_Nid->usuiv = F_Reine;
    F_Nid->uprec = NULL;

    F_Reine->usuiv = F_Frelon_un;
    F_Reine->uprec = F_Nid;

    F_Frelon_un->usuiv = F_Frelon_deux;
    F_Frelon_un->uprec = F_Reine;

    F_Frelon_deux->usuiv = NULL;
    F_Frelon_deux->uprec = F_Frelon_un;

    return F_Nid;
}



















Grille *initialiserGrille(void) {

    Grille *grille = NULL;

    if (NULL == (grille = (Grille *)malloc(sizeof(Grille))) ){
        fprintf(stderr, "Malloc n'a pas reussi a allouer la memoire pour GRILLE, reessayez.\n");
        return NULL; 	// exit(EXIT_FAILURE);
    }

    for (int i = 0; i < LIGNES; ++i) {
        for (int j = 0; j < COLONNES; ++j) {
			// Chaque case de plateau est initialisee par defaut NULL
            grille->plateau[i][j].colonie = NULL;	
            grille->plateau[i][j].occupant = NULL;
        }
    }

    grille->abeille = NULL;     // RUCHE -> Reine -> Guerriere -> Ouvriere
    grille->frelon = NULL;      // NID -> Reine -> Frelon_un -> Frelon_deux

	grille->plateau[0][0].colonie = grille->abeille;                        // RUCHE
    grille->plateau[LIGNES - 1][COLONNES - 1].colonie = grille->frelon;     // NID

    grille->tour = 0;   // apres chaque tour on l'incremente
    grille->ressourcesAbeille = 10;		// par defaut 10
    grille->ressourcesFrelon = 10;		// par defaut 10

    return grille;
}


// liberer toute la memoire alloue
void liberer_Grille(Grille *grille) {

    UListe abeilleCourante = grille->abeille;
    while (abeilleCourante != NULL) {
        UListe suivant = abeilleCourante->vsuiv;
        free(abeilleCourante);
        abeilleCourante = suivant;
    }

    UListe frelonCourant = grille->frelon;
    while (frelonCourant != NULL) {
        UListe suivant = frelonCourant->vsuiv;
        free(frelonCourant);
        frelonCourant = suivant;
    }

    for (int i = 0; i < LIGNES; ++i) {
        for (int j = 0; j < COLONNES; ++j) {
            free(grille->plateau[i][j].colonie);
            free(grille->plateau[i][j].occupant);
        }
    }

    free(grille);
}


/*************************************************/
/************** AJOUT / SUPPRESSION **************/
/*************************************************/


int ajoute_colonie_fin(UListe *colonie_un, UListe colonie_deux){

    // On suppose que colonie contient toujours au moins 1 element (Ruche ou Nid)
    if (! (*colonie_un) ){ // Si colonie_un est vide (par exemple detruite)
        fprintf(stderr, "Colonie_un est vide! \n");
        *colonie_un = colonie_deux;
        return 1; 
    }

    Unite *curr = *colonie_un; // pas obligatoire, mais c'est plus simple

    // Iteration jusqu'a dernier colonne qui est egale a NULL
    while (curr) { curr = curr->colsuiv; }
    curr = colonie_deux;

    colonie_deux->colsuiv = NULL;   // par defaut c'est deja NULL (pas obligatoire)
    colonie_deux->colprec = curr;    

    return 1;   // l'ajout bien s'est passe
}


// A verifier cette fonction
int ajoute_insecte_fin(UListe *colonie, Unite *new_insecte){
    // On suppose que colonie contient toujours au moins 1 element (Ruche ou Nid)

    // Si colonie est vide, donc insecte ne peut pas etre ajoute...
    if (! (*colonie)){ 
        fprintf(stderr, "Colonie est vide! \n");
        return 0;
    }

    Unite *curr = *colonie;     // ce n'est pas obligatoire, mais c'est plus simple

    // Iteration jusqu'a dernier unite qui n'est pas egale a NULL
    while (curr->usuiv != NULL) { curr = curr->usuiv; }
    curr->usuiv = new_insecte;

    new_insecte->usuiv = NULL;  // par defaut c'est deja NULL
    new_insecte->uprec = curr;

    return 1;
}





int ajoute_unite_case(Grille *grille, Unite *unite , int ligne, int colonne){
    if (grille == NULL || unite == NULL || ligne < 0 || ligne >= LIGNES || colonne < 0 || colonne >= COLONNES) {
        fprintf(stderr, "Paramètres invalides sur la fonction ajoute_case().\n");
        return 0;
    }

    Case *caseCourante = &(grille->plateau[ligne][colonne]);

    // Par defaut case[ligne][colonne] est vide

    // Ajoute l'unité (ou colonie) à la fin de la case actuelle
    // On suppose que sur la case on ne peut avoir que la Ruche ou que le Nid (un seul d'entre eux)

    if ( (RUCHE == unite->type || NID == unite->type) && (!caseCourante->colonie) ){
        caseCourante->colonie = unite;
        return 1;   // donc l'ajout d'une nouvelle colonie est bien passe (IL FAUT TESTER)
    } else {    // il existe deja une colonie sur cette case OU unite n'est pas RUCHE/NID
        return ajoute_insecte_fin(&caseCourante->occupant, unite);
    }

    return 0;
}



int supprimerUnite(UListe *colonie, Unite *unite, Unite **deleted_unite) {
    if ( (! colonie) || (! unite) ) {
        fprintf(stderr, "Colonie ou unite est NULL.\n");
        return 0;
    }

    UListe curr = *colonie;
    
    // On ne verifie pas si unite a supprimer est premiere unite
    // car premiere unite est la colonie (Ruche/Nid)

    // Recherche de l'unité à supprimer dans la colonie
    while (curr && curr != unite) { curr = curr->usuiv; }

    // Si l'unité à supprimer n'est pas trouvée dans la colonie
    if (NULL == curr) {
        fprintf(stderr, "Unite a supprimer non trouvee dans la colonie.\n");
        return 0; // Échec
    }

    if (curr->usuiv) { curr->usuiv->uprec = curr->uprec; }  // si unite suivante existe
    if (curr->uprec) { curr->uprec->usuiv = curr->usuiv; }  // si unite precedente existe

    *deleted_unite = curr;  // On prend l'address de cet insecte

    return 1; // Succès
}


int supprime_unite_case(Grille *grille, Unite *unite, int ligne, int colonne) {
    if (grille == NULL || unite == NULL || ligne < 0 || ligne >= LIGNES || colonne < 0 || colonne >= COLONNES) {
        fprintf(stderr, "Paramètres invalides sur la fonction plateau.c *supprime_unite_case()*.\n");
        return 0;
    }

    Case *caseCourante = &(grille->plateau[ligne][colonne]);

    // L'unité à supprimer est la colonie de la case
    if (caseCourante->colonie == unite) {

        if (unite->colsuiv && unite->colprec) {
            unite->colprec->colsuiv = unite->colsuiv;
            unite->colsuiv->colprec = unite->colprec;
        } else if (unite->colsuiv) {
            unite->colsuiv->colprec = NULL; // le lien pour la colonie suivante
            unite->colsuiv = NULL;  // pas obligatoire
        } else if (unite->colprec){
            unite->colprec->colsuiv = NULL; // le lien pour la colonie precedente
            unite->colprec = NULL;  // pas obligatoire
        } else { // LA VICTOIRE car il ne reste plus de RUCHE/NID - AJOUTER la fonction void victory(void);
            
            if (unite->camp == ABEILLES){ printf("FRELONS ONT GAGNE\n"); }
            else{ printf("ABEILLES ONT GAGNE\n"); }
            free(unite);
            return 1;  // Indiquer que les frelons ont gagne
        }
        free(unite);
        return 1; // Succès
    }

    // L'unité à supprimer est dans la liste des occupants de la case
    UListe listeOccupants = caseCourante->occupant;
    while (listeOccupants) {
        if (listeOccupants == unite) {

            if (listeOccupants->uprec) {   // n'est pas premiere unite 
                listeOccupants->uprec->usuiv = listeOccupants->usuiv;
            } else {                       // premiere unite
                caseCourante->occupant = listeOccupants->usuiv;
            }

            if (listeOccupants->usuiv) {   // ni premiere, ni derniere unite
                listeOccupants->usuiv->uprec = listeOccupants->uprec;
            }

            free(listeOccupants);          // derniere unite (tous les liens sont supprimes)
            return 1;
        }
        listeOccupants = listeOccupants->usuiv;     // Iteration
    }

    // L'unité n'a pas été trouvée liste des occupants et l'unité n'est pas egale a colonie
    fprintf(stderr, "L'unité spécifiée n'est pas présente sur la case.\n");
    return 0; // Échec
}



int rss_Abeilles_Colonie(Grille *grille, UListe A_colonie) {
    if ( (! grille) || (! A_colonie) ) {
        fprintf(stderr, "Paramètres invalides.\n");
        return 0;
    }

    int ressources_recues = 0;

    // Parcourir chaque unité affiliée à la ruche détruite
    if (A_colonie->usuiv){  // Si la ruche détruite a au moins une abeille

        UListe curr_unite = A_colonie->usuiv;   // car A_colonie est Ruche
        Unite *unite_suivante = NULL;     // A modifier plus tard

        while (curr_unite) {
        
            // N'OUBLIEZ PAS DE LIBERER LA MEMORIE DES UNITES SUR LES CASES (TO DO)
            switch (curr_unite->type) {
                case REINE:
                    ressources_recues += CREINEA; break;
                case OUVRIERE:
                    ressources_recues += COUVRIERE; break;
                case GUERRIERE:
                    ressources_recues += CGUERRIERE; break;
                case ESCADRON:
                    ressources_recues += CESCADRON; break;
                default:
                    fprintf(stderr, "Type d'unité non reconnu : %c\n", curr_unite->type);
                    break;
            }

            unite_suivante = curr_unite->vsuiv;
            supprime_unite_case(grille, curr_unite, curr_unite->posx, curr_unite->posy);
            // Chercher l'uniter actuelle pour la vider de la case ou elle est presente
            // free(curr_unite);

            curr_unite = unite_suivante;      // iteration
        }

    }

    // POUR LES TESTS, je modifie pour la lisibilite plus tard
    (supprime_unite_case(grille, A_colonie, A_colonie->posx, A_colonie->posy)) ? 
    printf("Colonie : %p est detruite. \n", A_colonie) : 
    printf("Colonie : %p n'est pas detruite. \n", A_colonie);

    // Quantite total pour ajouter au grille->ressourcesAbeille
    return ressources_recues;
}





int main(void){
    return 0;
}