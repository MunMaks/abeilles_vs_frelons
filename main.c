#include <stdio.h>
#include <stdlib.h>
// #include <MLV/MLV_all.h>
#include <time.h>
// #include <math.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

// Dimensions de la grille en nombre de cases (origine en haut a gauche) :
// début: (0, 0), fin: (17, 11) 
#define LIGNES 18
#define COLONNES 12

// Les deux camps :
#define ABEILLES 'A'
#define FRELONS 'F'
#define MAX_REINES 10   // la quantite des reines qui on deja construit une colonie

// Les types d'unites :
#define REINE 'r'
#define OUVRIERE 'o'
#define ESCADRON 'e'
#define GUERRIERE 'g'
#define FRELON 'f'
#define RUCHE 'R'
#define NID 'N'

// Pour la recolte de pollen
#define RECOLTE 'p'

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
	struct unite *colsuiv, *colprec;		// liste des autres ruches ou nids (colonies) du même camp
	struct unite *vsuiv, *vprec;			// liste des autres unites sur la meme case
} Unite, *UListe;


// La structure Case :
typedef struct {
	Unite *colonie; 		// S'il y a une ruche ou un nid sur la case
	UListe occupant; 		// les autres occupants de la case
} Case;


// La structure Grille :
typedef struct {
	Case plateau[LIGNES][COLONNES];
	UListe abeille, frelon;
	int tour; 				// Numero du tour
	int ressourcesAbeille, ressourcesFrelon;
} Grille;



/**************************************************/
/******************** ABEILLES ********************/
/**************************************************/



Unite* initialisation_ruche(void){
	Unite* A_Ruche = NULL;
    if (NULL == (A_Ruche = (Unite*)malloc(sizeof(Unite)))) {
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


void initialisation_abeilles(Grille *grille){
	// Camp ABEILLES au debut: 1 ruche, 1 reine, 1 guerrière et 1 ouvrière
	Unite *A_Ruche = initialisation_ruche();
	Unite *A_Reine = initialisation_reine_abeille();
	Unite *A_Guerriere = initialisation_guerriere();
	Unite *A_Ouvriere = initialisation_ouvriere();

    grille->abeille = A_Ruche;      // modification de la Ruche

	A_Ruche->usuiv = A_Reine;
    A_Ruche->uprec = NULL;

    A_Reine->usuiv = A_Guerriere;
    A_Reine->uprec = A_Ruche;

    A_Guerriere->usuiv = A_Ouvriere;
    A_Guerriere->uprec = A_Reine;

    A_Ouvriere->usuiv = NULL;
    A_Ouvriere->uprec = A_Guerriere;

    Case *caseCourante = &(grille->plateau[A_Ruche->posx][A_Ruche->posy]);

    caseCourante->colonie = A_Ruche;
    caseCourante->occupant = A_Reine;
    
    A_Reine->vsuiv = A_Guerriere;
    A_Guerriere->vsuiv = A_Ouvriere;
    A_Guerriere->vprec = A_Reine;
    A_Ouvriere->vprec = A_Guerriere;

}



/*************************************************/
/******************** FRELONS ********************/
/*************************************************/



Unite* initialisation_nid(void){
    Unite* F_Nid = NULL;
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
    F_Frelon->type = FRELON;
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


void initialisation_frelons(Grille *grille){
    // Camp FRELONS au debut: 1 nid, 1 reine, 2 frelons
	Unite* F_Nid = initialisation_nid();
	Unite *F_Reine = initialisation_reine_frelon();
	Unite *F_Frelon_un = initialisation_frelon();
	Unite *F_Frelon_deux = initialisation_frelon();

    grille->frelon = F_Nid;     // modification du Nids

	F_Nid->usuiv = F_Reine;
    F_Nid->uprec = NULL;

    F_Reine->usuiv = F_Frelon_un;
    F_Reine->uprec = F_Nid;

    F_Frelon_un->usuiv = F_Frelon_deux;
    F_Frelon_un->uprec = F_Reine;

    F_Frelon_deux->usuiv = NULL;
    F_Frelon_deux->uprec = F_Frelon_un;

    Case *caseCourante = &(grille->plateau[F_Nid->posx][F_Nid->posy]);

    caseCourante->colonie = F_Nid;
    caseCourante->occupant = F_Reine;
    
    F_Reine->vsuiv = F_Frelon_un;
    F_Frelon_un->vsuiv = F_Frelon_deux;
    F_Frelon_un->vprec = F_Reine;
    F_Frelon_deux->vprec = F_Frelon_un;
}


/*************************************************/
/************** Les fonctions utiles *************/
/*************************************************/



// Affichage d'une colonie souhaitee
void afficheColonie(UListe colonie)
{
    if (!colonie) { printf("Colonie est vide\n"); return; }
    Unite *curr = colonie;
    while (curr) {
        printf("le type est %c avec l'adress: %p\n", curr->type, curr);
        curr = curr->usuiv;
    }
}



// 1 - cette unite existe, 0 sinon
int uniteExiste(UListe colonie, Unite *unite)
{
    if (!colonie || !unite){
        fprintf(stderr, "Colonie est vide ou unite est NULL (exists)\n");
        return 0;
    }
    while(colonie){
        if (colonie == unite) { return 1; }
        colonie = colonie->usuiv;
    }
    return 0;
}



// Trouver colonie d'une unite
UListe trouver_Colonie(Unite *unite)
{
    if (!unite){
        fprintf(stderr, "On peut pas trouver la colonie, unite est vide\n");
        return NULL;
    }

    // Iteration jusqu'a sa colonie 
    // (on suppose que colonie n'a pas d'unites precedentes)
    while (unite->uprec){ unite = unite->uprec; }

    // "operateur ternaire" autrement dit si colonie donc on la retourne, NULL sinon
    return (unite->camp == RUCHE || unite->camp == NID) ? unite : NULL;
}



int prix_Unite(char camp, char type){
    int prix = 0;
    switch (type) {
        case RUCHE: prix = CRUCHE; break;
        case NID: prix = CNID; break;
        case REINE: prix = (camp == ABEILLES) ? CREINEA : CREINEF; break;
        case GUERRIERE: prix = CGUERRIERE; break;
        case ESCADRON: prix = CESCADRON; break;
        case OUVRIERE: prix = COUVRIERE; break;
        case FRELON: prix = CFRELON; break;

        default: fprintf(stderr, "Unite n'existe pas, reessayez"); break;
    }
    return prix;
}


int force_Unite(char type){
    int force = 0;
    switch (type) {
        case RUCHE: force = 0; break;
        case NID: force = 0; break;
        case REINE: force = FREINE; break;
        case GUERRIERE: force = FGUERRIERE; break;
        case ESCADRON: force = FESCADRON; break;
        case OUVRIERE: force = FOUVRIERE; break;
        case FRELON: force = FFRELON; break;

        default: fprintf(stderr, "Unite n'existe pas, reessayez"); break;
    }
    return force;
}


int temps_Unite(char camp, char type){
    int temps = 0;
    switch (type) {
        case RUCHE: temps = 0; break;
        case NID: temps = 0; break;
        case REINE: temps = (camp == ABEILLES) ? (TREINEA) : (TREINEF); break;
        case GUERRIERE: temps = TGUERRIERE; break;
        case ESCADRON: temps = TESCADRON; break;
        case OUVRIERE: temps = TOUVRIERE; break;    // plus tard TRECOLTE
        case FRELON: temps = TFRELON; break;

        default: fprintf(stderr, "Unite n'existe pas, reessayez"); break;
    }
    return temps;
}





/***********************************************/
/******************** AJOUT ********************/
/***********************************************/



int ajoute_Colonie(UListe *colonie_un, UListe colonie_deux)
{
    // On suppose que colonie contient toujours au moins 1 element (Ruche ou Nid)
    if (!colonie_deux) {
        fprintf(stderr, "Colonie_2 est vide\n");
        return 0;
    }

    // If colonie_un is empty, set it to colonie_deux
    if (!(*colonie_un)) {
        *colonie_un = colonie_deux;
        return 1;
    }

    // Iteration jusqu'a dernier colonne qui n'est egale a NULL
    Unite* curr = *colonie_un;
    while (curr->colsuiv) { curr = curr->colsuiv; }
    
    /*for (; (*colonie_un)->colsuiv; colonie_un = &((*colonie_un)->colsuiv)){
        ;   // boucle vide
    } */
    
    curr->colsuiv = colonie_deux;
    colonie_deux->colprec = curr;
    colonie_deux->colsuiv = NULL;

    return 1;   // l'ajout bien s'est passe
}



// A verifier cette fonction
int ajoute_Insecte(UListe *colonie, Unite *new_insecte)
{
    if (!new_insecte) { fprintf(stderr, "Impossible d'ajouter cette unite\n"); return 0; }

    // Si colonie est vide, on l'ajoute
    if (! (*colonie)){ 
        *colonie = new_insecte;
        return 1;
    }

    Unite *curr = *colonie;

    // Iteration jusqu'a dernier unite qui n'est pas egale a NULL
    while (curr->usuiv) { curr = curr->usuiv; }

    curr->usuiv = new_insecte;
    new_insecte->uprec = curr;
    new_insecte->usuiv = NULL;  // par defaut c'est deja NULL

    return 1;
}



int ajoute_Unite_Case(Grille *grille, Unite **unite , int ligne, int colonne)
{
    if (!grille || !(*unite) || ligne < 0 || ligne >= LIGNES || colonne < 0 || colonne >= COLONNES) {
        fprintf(stderr, "Paramètres invalides sur la fonction ajoute_case().\n");
        return 0;
    }

    Case *caseCourante = &(grille->plateau[ligne][colonne]);

    // Ajoute l'unité (ou colonie) à la fin de la case actuelle
    // On suppose que sur la case on ne peut avoir que la Ruche ou que le Nid (un seul d'entre eux)

    if ( (RUCHE == (*unite)->type || NID == (*unite)->type)){
        if (!caseCourante->colonie) {

            caseCourante->colonie = (*unite);
            (*unite)->posx = ligne, (*unite)->posy = colonne;
            return 1;
        } else {
            fprintf(stderr, "Case contient deja une RUCHE / un NID!\n");
            return 0;
        }
    }

    // On supprime les liaisons du case precedente
    if ((*unite)->vsuiv) { (*unite)->vsuiv->vprec = (*unite)->vprec; }
    if ((*unite)->vprec) { (*unite)->vprec->vsuiv = (*unite)->vsuiv; }


    if (!caseCourante->occupant) {  // Si case est vide
        caseCourante->occupant = (*unite);
        (*unite)->vsuiv = NULL, (*unite)->vprec = NULL;
    } else {
        Unite *curr = caseCourante->occupant;
        // Iteration jusqu'a dernier (*unite) qui n'est pas egale a NULL
        while (curr->vsuiv) { curr = curr->vsuiv; }

        curr->vsuiv = (*unite);
        (*unite)->vprec = curr;
        (*unite)->vsuiv = NULL;
    }
    (*unite)->posx = ligne, (*unite)->posy = colonne;
    return 1;
}



/***********************************************/
/***************** SUPPRESSION *****************/
/***********************************************/



int supprime_Unite(UListe *colonie, Unite *unite)   // On supprimes tous les liens
{
    if ( (! (*colonie)) || (! unite) ) {
        fprintf(stderr, "Colonie ou unite est NULL.\n");
        return 0;
    }
    UListe curr = *colonie;
    
    while (curr && curr != unite) { curr = curr->usuiv; }   // Recherche de l'unité

    // Si l'unité à supprimer n'est pas trouvée dans la colonie
    if (! curr ) {
        fprintf(stderr, "Unite a supprimer non trouvee dans la colonie.\n");
        return 0;
    }

    // La connexion avec sa colonie
    if (curr->usuiv){ curr->usuiv->uprec = curr->uprec; }

    if (curr->uprec){ curr->uprec->usuiv = curr->usuiv; }

    // La connexion avec d'autre unite de sa case
    if (curr->vsuiv){ curr->vsuiv->vprec = curr->vprec; }

    if (curr->vprec){ curr->vprec->vsuiv = curr->vsuiv; }

    // DISCUTABLE
    free(unite);    // unite a supprimer
    return 1;
}



// Supprime de liaisons avec d'autres colonies
int supprime_Colonie(UListe *colonie)
{
    // Si colonie existe
    if (!(*colonie) ){
        fprintf(stderr, "On ne peut pas supprimer une colonie, reessayez (supprime_Colonie)\n");
        return 0; }

    // si colonie suivante existe
    if ((*colonie)->colsuiv)
        (*colonie)->colsuiv->colprec = (*colonie)->colprec;
    
    // si colonie precedente existe
    if ((*colonie)->colprec)
        (*colonie)->colprec->colsuiv = (*colonie)->colsuiv;

    return 1;
}


// suppression des liaisons
int supprime_Unite_Case(Grille *grille, Unite **unite, int ligne, int colonne)
{
    if (!grille || !unite || ligne < 0 || ligne >= LIGNES || colonne < 0 || colonne >= COLONNES) {
        fprintf(stderr, "Parametres invalides: *supprime_Unite_Case()*.\n");
        return 0;
    }
    Case *caseCourante = &(grille->plateau[ligne][colonne]);

    // L'unité à supprimer est la colonie de la case
    if (caseCourante->colonie == (*unite)) {
        supprime_Colonie(unite);
        caseCourante->colonie = NULL;
        return 1;
    }
    if (! uniteExiste(caseCourante->occupant, *unite)){
        fprintf(stderr, "L'unite n'est pas presente sur la case.\n");
        return 0;
    }

    if ((*unite)->vprec) { (*unite)->vprec->vsuiv = (*unite)->vsuiv; }  // si unite precedente existe
    
    if ((*unite)->vsuiv) { (*unite)->vsuiv->vprec = (*unite)->vprec; }  // si unite suivante existe

    return 1;
}


void detruire_Colonie(UListe *colonie)
{
    if ((! (*colonie) )) {
        fprintf(stderr, "Colonie est vide\n");
        return;
    }

    // Suppression des liaisons avec d'autres colonies
    if ((*colonie)->colsuiv) { (*colonie)->colsuiv->colprec = (*colonie)->colprec; }

    if ((*colonie)->colprec) { (*colonie)->colprec->colsuiv = (*colonie)->colsuiv; }

    // Une seule colonie a supprimer donc victory()
    // if (!unite->colsuiv && !unite->colprec) { printf((unite->camp == ABEILLES) ? "FRELONS ONT GAGNE\n" : "ABEILLES ONT GAGNE\n"); }

    if (!( (*colonie)->usuiv) ){
        fprintf(stderr, "Colonie n'as pas d'unites\n");
        free(colonie);  // colonie n'as pas d'unites
        *colonie = NULL;
        return;
    }

    Unite *curr = *colonie;
    Unite *suiv = NULL;
    // libere toute la memoire occupée par les unites du milieu (O-O-O devient O-X-O)
    while (curr){
        suiv = curr->usuiv;
        free(curr);
        curr = suiv;
    }

    *colonie = NULL;  // Le pointeur doit etre NULL a la fin (donc terminer)
}



// Cette fonction va chercher tous les unites sur la grille pour supprimer les liaisons
int supprime_Liaisons_Colonie(Grille *grille, UListe colonie){
    if (!grille || !colonie){
        fprintf(stderr, "La suppression est impossbile\n");
        return 0;
    }
    Unite *curr = colonie;
    while (curr){
        if (!supprime_Unite_Case(grille, &curr, curr->posx, curr->posy)){
            fprintf(stderr, "La suppression des liaisons d'une colonie n'est pas atteinte\n");
            return 0;
        }
        curr = curr->usuiv;
    }
    return 1;
}




int detruire_Colonie_et_rss_abeilles(Grille *grille, UListe A_colonie)
{
    if ( (! grille) || (! A_colonie) ) {
        fprintf(stderr, "Parametres invalides.\n");
        return 0;
    }

    int ressources_recues = 0;

    // operateur ternaire (pour etre sur)
    UListe curr_unite = (A_colonie->type == RUCHE) ? A_colonie->usuiv : A_colonie;

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
                fprintf(stderr, "Type d'unite n'est pas reconnu : %c\n", curr_unite->type); break;
        }
        curr_unite = curr_unite->usuiv;      // iteration
    }
    supprime_Liaisons_Colonie(grille, A_colonie);   // suppression de toutes les liaisons
    detruire_Colonie(&A_colonie);       // Libere la memorie

    // ajouter au grille->ressourcesAbeille
    return ressources_recues;
}



/*******************************************************/
/************** LES FONCTIONS pour grille **************/
/*******************************************************/


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

    grille->abeille = NULL;     // RUCHE -> Reine_A -> Guerriere -> Ouvriere
    grille->frelon = NULL;      // NID -> Reine_F -> Frelon_un -> Frelon_deux

	grille->plateau[0][0].colonie = grille->abeille;                        // RUCHE
    grille->plateau[LIGNES-1][COLONNES-1].colonie = grille->frelon;         // NID

    grille->tour = 0;                   // apres chaque tour on l'incremente
    grille->ressourcesAbeille = 10;		// par defaut 10
    grille->ressourcesFrelon = 10;		// par defaut 10

    return grille;
}



void liberer_des_colonies(UListe *colonie)
{
    if (!(*colonie)){ fprintf(stderr, "Colonie est vide\n"); return; }

    // Si la colonie est unique
    /* if (!(*colonie)->colsuiv && !(*colonie)->colprec) {
        fprintf(stderr, "Colonie est unique liberer_des_colonies\n");
        detruire_colonie(colonie);
        *colonie = NULL;
        return;
    } */

    Unite *col_Courante = *colonie;
    UListe col_Suivante = NULL;
    int i = 1;
    char camp = (*colonie)->camp;
    //printf("Colonie n'est pas unique\n");
    while (col_Courante) {
        col_Suivante = col_Courante->colsuiv;
        detruire_Colonie(&col_Courante);
        printf("Colonie #%d est detruite, camp: %c\n", i, camp);
        col_Courante = col_Suivante;
        ++i;
    }
    // free(colonie);      // liberer le pointeur mais je pense que c'est pas obligatoire 
    *colonie = NULL;
}




// liberer toute la memoire alloue
void liberer_Grille(Grille *grille) {

    if (!grille->abeille) {
        fprintf(stderr, "Il n'y a pas de colonie des abeilles\n");
    }
    if (!grille->frelon) {
        fprintf(stderr, "Il n'y a pas de colonie des frelons\n");
    }

    liberer_des_colonies(&(grille->abeille));   // liberer les colonies des abeilles

    liberer_des_colonies(&(grille->frelon));    // liberer les colonies des frelons

    // On ne libere pas les lists "colonie" et "occupant" du "Case plateau[ligne][colonne]""
    // Car ce sont des pointeurs

    free(grille);
}




Unite *creation_Unite(UListe* colonie, char type, int force, int temps){
    Unite *new_unite = NULL;
    if (NULL == (new_unite = (Unite *)malloc(sizeof(Unite)))){
        fprintf(stderr, "Unite n'est pas cree, malloc ...\n");
        return NULL;
    }
    new_unite->camp = (*colonie)->camp;
    new_unite->type = type;
    new_unite->force = force;
    new_unite->posx = (*colonie)->posx, new_unite->posy = (*colonie)->posy;
    new_unite->destx = 0, new_unite->desty = 0;     // pour l'instant elle bouge pas
    new_unite->production = '0';
    new_unite->temps = temps;
    new_unite->toursrestant = temps;    // avec du temps cela decremente

    // avant d'affiler il faut que (*colonie)->toursrestant soit 0
    new_unite->usuiv = NULL, new_unite->uprec = NULL;   

    new_unite->colsuiv = NULL, new_unite->colprec = NULL;

    // vsuiv, vprec (dehors de cette fonction)
    new_unite->vsuiv = NULL, new_unite->vprec = NULL;   

    return new_unite;
}



UListe creation_Colonie(Unite **reine, char type, int temps)
{
    // C'est impossible, mais pour etre sur.
    if (!(*reine)){ fprintf(stderr, "Reine n'existe pas\n"); return NULL; }

    UListe new_colonie = NULL;
    if (NULL == (new_colonie = (UListe)malloc(sizeof(Unite)))){
        fprintf(stderr, "Unite n'est pas cree, malloc ...\n");
        return NULL;
    }
    new_colonie->camp = (*reine)->camp;
    new_colonie->type = type;
    new_colonie->force = 0;
    new_colonie->posx = (*reine)->posx, new_colonie->posy = (*reine)->posy;
    new_colonie->destx = 0, new_colonie->desty = 0;     // elle bouge pas
    new_colonie->production = '0';      // quel type d'unite est en train d'etre produit
    new_colonie->temps = 0;
    new_colonie->toursrestant = 0;

    UListe colonie_de_reine = trouver_Colonie(*reine);
    if (!ajoute_Colonie(&colonie_de_reine, new_colonie)){
        new_colonie->colsuiv = NULL, new_colonie->colprec = NULL;
    }
    // supprimer les liens precedente de la reine
    if ((*reine)->usuiv){ (*reine)->usuiv->uprec = (*reine)->uprec; }
    if ((*reine)->uprec){ (*reine)->uprec->usuiv = (*reine)->usuiv; }

    new_colonie->usuiv = *reine, new_colonie->uprec = NULL;

    new_colonie->vsuiv = NULL, new_colonie->vprec = NULL;    // Pour les colonie, cela n'est pas necessaire

    // il faut pas oublier grille->plateau[i][j].colonie = new_colonie

    // ajoute_Unite_Case(grille, unite, ligne, colonne) A FAIRE DEHORS
    return new_colonie;
}


// L'achat d'une unite avec son samp et son type
int achat_unite(Grille *grille, UListe *colonie, Unite **ptr_unite,char camp, char type){

    if (!(*colonie)->toursrestant){     // si toursrestant n'est pas egale a 0
        fprintf(stderr, "Creation interdite, l'unite est deja en production\n");
        return 0;
    }

    int prix = prix_Unite(camp, type);
    if (camp == ABEILLES && grille->ressourcesAbeille < prix){          // pour les abeilles
        fprintf(stderr, "Pas assez de ressources pour acheter une unite des abeilles\n");
        return 0;
    } else if (camp == FRELONS && grille->ressourcesFrelon < prix){     // pour les frelons
        fprintf(stderr, "Pas assez de ressources pour acheter une unite des frelons\n");
        return 0;
    }
    int force = force_Unite(type);
    int temps = temps_Unite(camp, type);
    
    Unite *new_unite = NULL;
    new_unite = creation_Unite(colonie, type, force, temps);
    (*colonie)->production = type;      // type d'unite en cours de production
    (*colonie)->temps = temps;          // nbr tours totales
    (*colonie)->toursrestant = temps;   // nbr tours restants

    if (!new_unite){ fprintf(stderr, "Impossbile de creer une nouvelle unite\n"); return 0; }   // optionel
    *ptr_unite = new_unite;


    // J'ai commenter car cela nous servira dehors de cette fonction

    /*
    // grille->ressourcesAbeille -= prix;  // on diminie les ressources a la fin du tours
    if (! ajoute_Insecte(colonie, new_unite) ){ // l'ajout a la colonie actuelle
        fprintf(stderr, "Probleme ajoute unite, apres avoir achete\n");
        free(new_unite);    // on ne peut pas l'ajouter, donc on la libere
        return 0;
    }
    if (! ajoute_Unite_Case(grille, new_unite, new_unite->posx, new_unite->posy) ){ // l'ajout sur la case necessaire (a la fin)
        fprintf(stderr, "Probleme ajoute unite, apres avoir achete\n");
        if (new_unite) { free(new_unite); }
        return 0;
    } */

    return 1;
}


int deplacer_unite(Grille *grille, Unite **unite){
    if (!grille || !(*unite)){
        fprintf(stderr, "Impossible, il y a un probleme: grille ou unite a deplacer\n");
        return 0;
    }

    // les nouvelles coordonnees de cette unite
    int x = (*unite)->posx + (*unite)->destx;
    int y = (*unite)->posy + (*unite)->desty;

    if (x < 0 || x >= LIGNES || y < 0 || y >= COLONNES) {
        fprintf(stderr, "Deplacement en dehors de la grille est interdite\n"); return 0;
    }

    // cette fonction supprime automatiquement la connexion precedente et ajoute a la fin d'une nouvelle case
    if (! ajoute_Unite_Case(grille, unite, x, y) ){
        fprintf (stderr, "On n'a pas reussie de faire la connexion de sa nouvelle case\n"); return 0;
    }

    // maintenant deplacement est finie, donc on met a jour de destx et desty
    (*unite)->destx = 0;
    (*unite)->desty = 0;

    return 1;   
}

// Renvoie: "-1" s'il y un problem
//          "1" si la reine a deja construit une colonie
//          "0" donc la reine a droit de construire et on l'ajoute dans "reines_liste"
int reine_deja_contruit(UListe *reines_liste, int *nbr_reines, Unite *reine){

    if ( (*nbr_reines) < 0 || !reine){
        fprintf(stderr, "Il y un problem a la fonction reine_deja_contruit()\n");
        return -1;  // exceptionnellement
    }
    if (! (*reines_liste) && !(*nbr_reines) ){
        *reines_liste = malloc(MAX_REINES * sizeof(Unite *));
        *reines_liste = reine;
        ++(*nbr_reines);
        return 0;
    }

    for (int i = 0; i < (*nbr_reines); ++i){
        if (reine->type == REINE && reines_liste[i] == reine){ return 1; }
    }

    reines_liste[(*nbr_reines)] = reine;    // la reine prend la place

    ++(*nbr_reines);    // incrementation
    return 0;
}


// quantite totale du type souhaite
int nbr_Unite_Case(Case caseActuelle, char type){
    int quantite = 0;
    Unite *curr = caseActuelle.colonie;
    if (!curr) { fprintf(stderr, "La caseActuelle.occupant est vide donc: 0\n"); return quantite; }
    while (curr){
        if (curr->type == type){ ++quantite; }
        curr = curr->vsuiv;
    }
    return quantite;
}


// nb totale sur Case
int nbr_Unites_totale_Case(Case caseActuelle){
    int nb_unites = 0;
    // si tu veux tout dans cette fonction, donc il faut utiliser SWITCH(){...}
    // int nb_reines_A = 0, nb_reines_F = 0, nb_escadron = 0, nb_guerriere = 0, nb_frelon = 0, nb_ouvriere = 0;

    Unite *curr = caseActuelle.occupant;
    while (curr){
        ++nb_unites;
        curr = curr->vsuiv;
    }
    return nb_unites;
}





// TO DO une fonction qui garde l'adresses des reines qui ont deja contruit une Ruche / un Nid
// TO DO ajouter l'unite achetee a sa colonie et a la case de sa colonie (OBLIGATOIRE)
// TO DO supprimer les ressources depensees
// TO DO fonction RECOLTE du pollen

unsigned int tirage_au_hasard(unsigned int x, unsigned int y) {
    
    unsigned int resultat = x + (rand() % (y - x + 1));

    return resultat;
}



int main(int argc, char *argv[]){
    srand( (unsigned int) time(NULL));  // aletoire
    unsigned int x = 0, y = 1;  // On dit ABEILLES - 0, FRELONS - 1
    unsigned int resultat_tirage = tirage_au_hasard(x, y);
    printf("\nResultat du tirage aleatoire: %u\n", resultat_tirage);     // a supprimer plus tard


    // TEST une array des reines qui ont deja contruit une colonie
/*     Unite *reine = initialisation_reine_abeille();
    int nbr_reines = 0;
    UListe *reines_liste = (UListe *) malloc (MAX_REINES * sizeof(UListe));
    reine_deja_contruit(reines_liste, &nbr_reines, reine);
    free(reines_liste); */


    Grille *grille = initialiserGrille();

    initialisation_abeilles(grille);
    initialisation_frelons(grille);
    //UListe colonie_2 = initialisation_abeilles();
    //ajoute_Colonie(&(grille->abeille), colonie_2);
    // ajoute_Unite_Case(grille, colonie_2, );
    // grille->frelon = initialisation_frelons();
    // UListe new_col = initialisation_abeilles();
    // ajoute_colonie(&(grille->abeille), new_col);
    // int count = detruire_colonie_et_rss_abeilles(grille, colonie_2);
    // printf("On a recu: %d \n", count);
    afficheColonie(grille->abeille);
    printf("\n\n");
    // afficheColonie(grille->frelon);

    // detruire_colonie(&(grille->abeille));
    // afficheColonie(grille->abeille);

    liberer_Grille(grille);

    fprintf(stderr, "\nla memorie liberee MAIN\n");
    
    return 0;
}
