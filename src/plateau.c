#include "plateau.h"

/* TOUS LES COMMENTAIRES NECESSAIRES SE TROUVES DANS PLATEAU.H*/

/**************************************************/
/******************** ABEILLES ********************/
/**************************************************/


Unite* initialisation_unite_A(char type, int force)
{
	Unite* unite_Abeille = NULL;
    if (NULL == (unite_Abeille = (Unite *)malloc(sizeof(Unite)))) {
        fprintf(stderr, "Pas de mémoire pour unite abeille\n");
		return NULL;
	}
	unite_Abeille->camp = ABEILLES;
	unite_Abeille->type = type;
	unite_Abeille->force = force;
	unite_Abeille->posx = 0, unite_Abeille->posy = 0;
	unite_Abeille->destx = 0, unite_Abeille->desty = 0;
	unite_Abeille->production = '0';
	unite_Abeille->temps = 0;
	unite_Abeille->toursrestant = 0;
	unite_Abeille->usuiv = NULL, unite_Abeille->uprec = NULL;
	unite_Abeille->colsuiv = NULL, unite_Abeille->colprec = NULL;
	unite_Abeille->vsuiv = NULL, unite_Abeille->vprec = NULL;

	return unite_Abeille;
}


void initialisation_abeilles(Grille **grille)
{
	// Camp ABEILLES au debut: 1 ruche, 1 reine, 1 guerrière et 1 ouvrière
	Unite *A_Ruche = initialisation_unite_A(RUCHE, 0);
	Unite *A_Reine = initialisation_unite_A(REINE, FREINE);
	Unite *A_Guerriere = initialisation_unite_A(GUERRIERE, FGUERRIERE);
	Unite *A_Ouvriere = initialisation_unite_A(OUVRIERE, FOUVRIERE);

    (*grille)->abeille = A_Ruche;      // modification de la grille

    ajoute_Insecte(&A_Ruche, A_Reine);

    ajoute_Insecte(&A_Ruche, A_Guerriere);

    ajoute_Insecte(&A_Ruche, A_Ouvriere);


    ajoute_Unite_Case(grille, &A_Ruche, 0, 0);

    ajoute_Unite_Case(grille, &A_Reine, 0, 0);

    ajoute_Unite_Case(grille, &A_Guerriere, 0, 0);

    ajoute_Unite_Case(grille, &A_Ouvriere, 0, 0);
}



/*************************************************/
/******************** FRELONS ********************/
/*************************************************/



Unite* initialisation_unite_F(char type, int force)
{
    Unite* unite_Frelon = NULL;
    if (NULL == (unite_Frelon = (Unite *)malloc(sizeof(Unite)))) {
        fprintf(stderr, "Pas de mémoire pour unite Frelon\n");
		return NULL;
	}
	unite_Frelon->camp = FRELONS;
	unite_Frelon->type = type;
	unite_Frelon->force = force;
	unite_Frelon->posx = NB_COLONNES-1, unite_Frelon->posy = NB_LIGNES-1;
	unite_Frelon->destx = 0, unite_Frelon->desty = 0;
	unite_Frelon->production = '0';
	unite_Frelon->temps = 0;
	unite_Frelon->toursrestant = 0;
	unite_Frelon->usuiv = NULL, unite_Frelon->uprec = NULL;
	unite_Frelon->colsuiv = NULL, unite_Frelon->colprec = NULL;
	unite_Frelon->vsuiv = NULL, unite_Frelon->vprec = NULL;

	return unite_Frelon;
}



void initialisation_frelons(Grille **grille)
{
    // Camp FRELONS au debut: 1 nid, 1 reine, 2 frelons
	Unite *F_Nid = initialisation_unite_F(NID, 0);
	Unite *F_Reine = initialisation_unite_F(REINE, FREINE);
	Unite *F_Frelon_un = initialisation_unite_F(FRELON, FFRELON);
	Unite *F_Frelon_deux = initialisation_unite_F(FRELON, FFRELON);

    (*grille)->frelon = F_Nid;     // modification du Nids

    ajoute_Insecte(&F_Nid, F_Reine);

    ajoute_Insecte(&F_Nid, F_Frelon_un);

    ajoute_Insecte(&F_Nid, F_Frelon_deux);

    
    ajoute_Unite_Case(grille, &F_Nid, NB_COLONNES-1, NB_LIGNES-1);

    ajoute_Unite_Case(grille, &F_Reine, NB_COLONNES-1, NB_LIGNES-1);

    ajoute_Unite_Case(grille, &F_Frelon_un, NB_COLONNES-1, NB_LIGNES-1);

    ajoute_Unite_Case(grille, &F_Frelon_deux, NB_COLONNES-1, NB_LIGNES-1);
}



/*******************************************************/
/************** LES FONCTIONS pour grille **************/
/*******************************************************/



Grille *initialiserGrille(void)
{

    Grille *grille = NULL;

    if (NULL == (grille = (Grille *)malloc(sizeof(Grille))) ){
        fprintf(stderr, "Malloc n'a pas reussi a allouer la memoire pour GRILLE, reessayez.\n");
        return NULL;
    }

    for (int i = 0; i < NB_COLONNES; ++i) {
        for (int j = 0; j < NB_LIGNES; ++j) {
			// Chaque case de plateau est initialisee par defaut NULL
            grille->plateau[i][j].colonie = NULL;	
            grille->plateau[i][j].occupant = NULL;
        }
    }

    grille->abeille = NULL;     // RUCHE -> Reine_A -> Guerriere -> Ouvriere
    grille->frelon = NULL;      // NID -> Reine_F -> Frelon_un -> Frelon_deux

	grille->plateau[0][0].colonie = grille->abeille;                        // RUCHE
    grille->plateau[NB_COLONNES-1][NB_LIGNES-1].colonie = grille->frelon;   // NID

    grille->tour = 0;                   // apres chaque tour on l'incremente
    grille->ressourcesAbeille = 10;		// par defaut 10
    grille->ressourcesFrelon = 10;		// par defaut 10

    return grille;
}



void liberer_Grille(Grille **grille)
{
    if (!(*grille)->frelon) { fprintf(stderr, "Il n'y a pas de colonie des frelons\n"); }

    if (!(*grille)->abeille) { fprintf(stderr, "Il n'y a pas de colonie des abeilles\n"); }

    liberer_des_colonies(grille, &((*grille)->abeille));   // liberer les colonies des abeilles

    liberer_des_colonies(grille, &((*grille)->frelon));    // liberer les colonies des frelons

    // On ne libere pas les lists "colonie" et "occupant" du Case [colonne][ligne]
    // Car ce sont des pointeurs
    free(*grille);
}



Unite *creation_Unite(UListe *colonie, char type)
{
    if (! (*colonie) ){
        fprintf(stderr, "Colonie est vide");
        return NULL;
    }
    Unite *new_unite = NULL;
    if (NULL == (new_unite = (Unite *)malloc(sizeof(Unite)))){
        fprintf(stderr, "Unite n'a pas ete cree, malloc ...\n");
        return NULL;
    }
    new_unite->camp = (*colonie)->camp;
    new_unite->type = type;
    new_unite->force = force_Unite(type);
    new_unite->posx = (*colonie)->posx, new_unite->posy = (*colonie)->posy;
    new_unite->destx = 0, new_unite->desty = 0;
    new_unite->production = '0';
    new_unite->temps = 0;
    new_unite->toursrestant = 0;

    // usuiv, uprec
    new_unite->usuiv = NULL, new_unite->uprec = NULL;

    new_unite->colsuiv = NULL, new_unite->colprec = NULL;

    // vsuiv, vprec
    new_unite->vsuiv = NULL, new_unite->vprec = NULL;
    
    return new_unite;
}



UListe creation_Colonie(Unite **reine)
{
    if (!(*reine)){ fprintf(stderr, "Reine n'existe pas\n"); return NULL; }

    UListe new_colonie = NULL;
    if (NULL == (new_colonie = (UListe) malloc(sizeof(Unite)))){
        fprintf(stderr, "Unite n'est pas cree, malloc ...\n");
        return NULL;
    }
    
    new_colonie->camp = (*reine)->camp;
    new_colonie->type = ((*reine)->camp == ABEILLES) ? (RUCHE) : (NID);
    new_colonie->force = 0;
    new_colonie->posx = (*reine)->posx, new_colonie->posy = (*reine)->posy;
    new_colonie->destx = 0, new_colonie->desty = 0;
    new_colonie->production = '0';      // quel type d'unite est en train d'etre produit
    new_colonie->temps = 0;
    new_colonie->toursrestant = 0;


    // colsuiv, colprec
    new_colonie->colsuiv = NULL, new_colonie->colprec = NULL;
    // unites affilees a cette nouvelle colonie
    new_colonie->usuiv = NULL, new_colonie->uprec = NULL;

    new_colonie->vsuiv = NULL, new_colonie->vprec = NULL;

    return new_colonie;
}



int achat_Unite(Grille **grille, UListe *colonie, char type)
{

    if ((*colonie)->toursrestant){     // si toursrestant n'est pas egale a 0
        fprintf(stderr, "Creation interdite, l'unite est deja en production\n");
        return 0;
    }

    int prix = prix_Unite((*colonie)->camp, type);
    if ( (*colonie)->camp == ABEILLES && (*grille)->ressourcesAbeille < prix){          // pour les abeilles
        fprintf(stderr, "Pas assez de ressources pour acheter une unite des abeilles\n");
        return 0;
    } else if ( (*colonie)->camp == FRELONS && (*grille)->ressourcesFrelon < prix){     // pour les frelons
        fprintf(stderr, "Pas assez de ressources pour acheter une unite des frelons\n");
        return 0;
    }

    int temps = temps_Unite((*colonie)->camp, type);
    (*colonie)->production = type;      // type d'unite en cours de production
    (*colonie)->temps = temps;          // nbr tours totales
    (*colonie)->toursrestant = temps;   // nbr tours restants

    // on diminie les ressources a la fin du tour
    if ( (*colonie)->camp == ABEILLES ){
        (*grille)->ressourcesAbeille -= prix;
    } else if ( (*colonie)->camp == FRELONS ){
        (*grille)->ressourcesFrelon -= prix;
    }

    return 1;
}



int achat_Colonie(Grille **grille, Unite **reine){
    if ((*reine)->toursrestant){     // si toursrestant n'est pas egale a 0
        fprintf(stderr, "Creation interdite, la colonie est deja en production\n");
        return 0;
    }
    char camp = (*reine)->camp;
    char type = (ABEILLES == camp) ? RUCHE : NID;
    int prix = 10;

    if ( camp == ABEILLES && (*grille)->ressourcesAbeille < prix){          // pour les abeilles
        fprintf(stderr, "Pas assez de ressources pour acheter une Ruche\n");
        return 0;
    } else if ( camp == FRELONS && (*grille)->ressourcesFrelon < prix){     // pour les frelons
        fprintf(stderr, "Pas assez de ressources pour acheter un Nid\n");
        return 0;
    }

    (*reine)->production = type;
    (*reine)->temps = 1;
    (*reine)->toursrestant = 1;

    if ( camp == ABEILLES ){
        (*grille)->ressourcesAbeille -= prix;
    } else if ( camp == FRELONS ){
        (*grille)->ressourcesFrelon -= prix;
    }

    return 1;
}

// Renvoie: "-1" s'il y un problem
//          "1" si la reine a deja construit une colonie
//          "0" donc la reine a droit de construire et on l'ajoute dans "reines_liste"
int reine_deja_contruit(UListe *reines_liste, int *nbr_reines, Unite *reine)
{
    if ( (*nbr_reines) < 0 || !(reine) ){
        fprintf(stderr, "Il y un problem a la fonction reine_deja_contruit()\n");
        return -1;  // exceptionnellement
    }

    for (int i = 0; i < (*nbr_reines); ++i){
        if (reine->type == REINE && reines_liste[i] == reine){ return 1; }
    }

    reines_liste[(*nbr_reines)] = reine;    // la reine prend la place

    ++(*nbr_reines);    // incrementation
    return 0;
}



int nbr_Unite_Case(Case caseActuelle, char type)
{
    int quantite = 0;
    Unite *curr = caseActuelle.colonie;
    if (!curr) { fprintf(stderr, "Il n'y pas d'occupants donc: 0\n"); return quantite; }
    while (curr){
        if (curr->type == type) { ++quantite; }
        curr = curr->vsuiv;
    }
    return quantite;
}



int nbr_Unites_totale_Case(Case caseActuelle)
{
    int nb_unites = 0;

    Unite *curr = caseActuelle.occupant;
    while (curr){
        ++nb_unites;
        curr = curr->vsuiv;
    }
    return nb_unites;
}



void start_recolte_Ouvrierre(Unite **ouvriere){
    if (!(*ouvriere)){
        fprintf(stderr, "Ouvriere n'existe pas, recolte est annulee\n");
        return;
    }
    if ((*ouvriere)->toursrestant){
        fprintf(stderr, "Ouvriere est en train de recolter\n");
        return;
    }
    (*ouvriere)->production = RECOLTE;
    (*ouvriere)->temps = TRECOLTE;
    (*ouvriere)->toursrestant = TRECOLTE;   // cela diminue avec du temps
}



int nb_recolte(UListe colonie)
{
    if (!colonie){
        fprintf(stderr, "Colonie est vide\n");
        return 0;
    }
    int quantite = 0;
    Unite *curr_col = colonie;

    while (curr_col) {
        Unite *curr_unite = curr_col;
        while (curr_unite) {
            if (OUVRIERE == curr_unite->type && curr_unite->toursrestant) { ++quantite; }
            curr_unite = curr_unite->usuiv;
        }
        curr_col = curr_col->colsuiv;
    }
    return quantite;
}



void ajouter_unite_selon_type(Grille *grille, char type, int tour)  // tour vaut 0 (abeilles) ou 1 (frelons)
{
    //Unite *nouvelle_unite = NULL;
    UListe colonie = (tour) ? grille->abeille : grille->frelon;
    if (REINE == type || GUERRIERE == type || FRELON == type || OUVRIERE == type || ESCADRON == type){
        achat_Unite(&grille, &colonie, type);
        return;
    }
}



void ajouter_colonie_selon_type(Grille *grille, Unite *reine, UListe *reines_liste, int *nbr_reines, char type)  // tour vaut 0 (abeilles) ou 1 (frelons)
{
    if (RUCHE == type || NID == type){
        if (reine_deja_contruit(reines_liste, nbr_reines, reine)){
            fprintf(stderr, "La reine ne peut constuire des colonies\n");
            return;
        }
        achat_Colonie(&grille, &reine);
        return;
    }
    // fprintf(stderr, "Type non reconnue : %c\n", type);
}



// La fonction qui commence la création d'une ruche ou d'un nid
void start_creation_Colonie(Unite **reine) {
    if (!(*reine)) {
        fprintf(stderr, "Reine n'existe pas, creation de ruche/nid annulée.\n");
        return;
    }

    if ((*reine)->toursrestant) {
        fprintf(stderr, "Reine est en train de creer quelque chose\n");
        return;
    }

    (*reine)->production = ((*reine)->camp == ABEILLES) ? RUCHE : NID ;
    (*reine)->temps = 1;
    (*reine)->toursrestant = 1;   // cela diminue avec du temps
}
