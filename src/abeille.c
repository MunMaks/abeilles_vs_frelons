#include "plateau.h"


Unite* create_Abeille(UListe ruche, char type, int force, int temps) {
    Unite* abeille = NULL;
    if (NULL == (abeille = (Unite*)malloc(sizeof(Unite))) ) {
        fprintf(stderr, "Malloc n'a pas reussi de creer une abeille, memoire...\n");
        return NULL;    //exit(EXIT_FAILURE);
    }

    abeille->camp = ABEILLES;
    abeille->type = type;
    abeille->force = force;
    abeille->posx = ruche->posx;    // cet insecte prend les coordonnées de sa ruche
    abeille->posy = ruche->posy;
    abeille->destx = 0, abeille->desty = 0;     // (x, y), (0, 0) elle ne bouge pas
    abeille->production = '0';       // Je ne comprens pas mais si c'est ouvriere donc c'est RECOLTE
    abeille->temps = temps;
    abeille->toursrestant = temps;              // ça va décrémenter avec du temps
    abeille->usuiv = NULL, abeille->uprec = NULL;
    abeille->colsuiv = NULL, abeille->colprec = NULL;
    abeille->vsuiv = NULL, abeille->vprec = NULL;

    return abeille;
}


Unite *create_Ruche(Unite *reine){
    Unite *new_Ruche = NULL;

    if (NULL == (new_Ruche = (Unite *) malloc (sizeof(Unite)))){
        fprintf(stderr, "Malloc n'a pas reussi de creer une nouvelle ruche, memoire...\n");
        return NULL;    //exit(EXIT_FAILURE);
    }
    new_Ruche->camp = ABEILLES;
    new_Ruche->type = RUCHE;
    new_Ruche->force = 0;
    new_Ruche->posx = reine->posx;
    new_Ruche->posy = reine->posy;
    new_Ruche->destx = 0, new_Ruche->desty = 0;
    new_Ruche->production = '0';
    new_Ruche->temps = 1;
    new_Ruche->toursrestant = 1;
    new_Ruche->usuiv = reine, new_Ruche->uprec = NULL;      // Supprimer la lieson reine de sa ruche precedente
    new_Ruche->colsuiv = NULL; new_Ruche->colprec = NULL;   // Ajouter la logique pour lier nouvelle col
    new_Ruche->vsuiv = NULL, new_Ruche->vprec = NULL;       // Ajouter la logique pour lier sur la case


    // Il faut retravailler ici et ajouter la logique pour supprimer la reine de sa ruche precedente
    if (reine != NULL && reine->uprec != NULL) {
        reine->uprec->usuiv = NULL;
    }


    return new_Ruche;
}

