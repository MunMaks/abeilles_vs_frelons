#include <stdio.h>
#include <stdlib.h>
// #include <MLV/MLV_all.h>
#include <time.h>
// #include <math.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

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


unsigned int tirage_au_hasard(unsigned int x, unsigned int y) {
    
    unsigned int resultat = x + (rand() % (y - x + 1));

    return resultat;
}

int tirageDe(void) {
    return rand() % 60 + 1;
}





/***********************************************/
/***************** SUPPRESSION *****************/
/***********************************************/


int supprime_Liaisons_Unite(Unite **unite){
    if (! (*unite)){
        fprintf(stderr, "Unite n'existe pas\n");
        return 0;
    }
    if ((*unite)->vsuiv){ (*unite)->vsuiv->vprec = (*unite)->vprec; }   // unite suivante (sur case)
    if ((*unite)->vprec){ (*unite)->vprec->vsuiv = (*unite)->vsuiv; }   // unite precedente (sur case)
    return 1;
}


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
    supprime_Liaisons_Unite(&curr);
    
    // DISCUTABLE
    free(unite);    // unite a supprimer
    return 1;
}



// Supprime de liaisons avec d'autres colonies
int supprime_Colonie(UListe *colonie)
{
    // Si colonie existe
    if (!(*colonie) ){
        fprintf(stderr, "Colonie n'existe pas, reessayez (supprime_Colonie)\n");
        return 0;
    }
    // si colonie suivante existe
    if ((*colonie)->colsuiv) { (*colonie)->colsuiv->colprec = (*colonie)->colprec; }
    
    // si colonie precedente existe
    if ((*colonie)->colprec) { (*colonie)->colprec->colsuiv = (*colonie)->colsuiv; }

    return 1;
}





// suppression des liaisons
int supprime_Unite_Case(Grille **grille, Unite **unite)
{
    if (!grille || !(*unite)) {
        fprintf(stderr, "Parametres invalides: *supprime_Unite_Case()*.\n");
        return 0;
    }
    Case *caseCourante = &((*grille)->plateau[(*unite)->posx][(*unite)->posy]);

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

    supprime_Liaisons_Unite(unite);
    return 1;
}


void detruire_Colonie(UListe *colonie)
{
    if ((! (*colonie) )) {
        fprintf(stderr, "Colonie est vide\n");
        return;
    }

    // Suppression des liaisons avec d'autres colonies
    supprime_Colonie(colonie);

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
int supprime_Liaisons_Colonie(UListe *colonie){
    if (! (*colonie)){
        fprintf(stderr, "La suppression est impossbile\n");
        return 0;
    }
    Unite *curr = *colonie;
    while (curr){
        if (!supprime_Liaisons_Unite(&curr)){
            fprintf(stderr, "La suppression des liaisons d'une colonie n'est pas atteinte\n");
            return 0;
        }
        curr = curr->usuiv;
    }
    return 1;
}




int detruire_Colonie_et_rss_abeilles(UListe A_colonie)
{
    if ( (! A_colonie) ) {
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
    supprime_Liaisons_Colonie(&A_colonie);   // suppression de toutes les liaisons
    detruire_Colonie(&A_colonie);       // Libere la memorie

    // ajouter au grille->ressourcesAbeille
    return ressources_recues;
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



int ajoute_Unite_Case(Grille **grille, Unite **unite , int ligne, int colonne)
{
    if (!(*grille) || !(*unite) || ligne < 0 || ligne >= LIGNES || colonne < 0 || colonne >= COLONNES) {
        fprintf(stderr, "Paramètres invalides sur la fonction ajoute_case().\n");
        return 0;
    }

    Case *caseCourante = &((*grille)->plateau[ligne][colonne]);

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
    supprime_Liaisons_Unite(unite);

    if (!caseCourante->occupant) {  // Si case est vide
        caseCourante->occupant = (*unite);
        (*unite)->vsuiv = NULL, (*unite)->vprec = NULL;     // optinel
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






/**************************************************/
/******************** ABEILLES ********************/
/**************************************************/



Unite* initialisation_unite_A(char type, int force){
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
	unite_Abeille->toursrestant = 0; 			// QUESTION
	unite_Abeille->usuiv = NULL, unite_Abeille->uprec = NULL;
	unite_Abeille->colsuiv = NULL, unite_Abeille->colprec = NULL;
	unite_Abeille->vsuiv = NULL, unite_Abeille->vprec = NULL;

	return unite_Abeille;
}


void initialisation_abeilles(Grille **grille){
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



Unite* initialisation_unite_F(char type, int force){
    Unite* unite_Frelon = NULL;
    if (NULL == (unite_Frelon = (Unite *)malloc(sizeof(Unite)))) {
        fprintf(stderr, "Pas de mémoire pour unite Frelon\n");
		return NULL;
	}
	unite_Frelon->camp = FRELONS;
	unite_Frelon->type = type;
	unite_Frelon->force = force;
	unite_Frelon->posx = 17, unite_Frelon->posy = 11;
	unite_Frelon->destx = 0, unite_Frelon->desty = 0;
	unite_Frelon->production = '0';
	unite_Frelon->temps = 0;
	unite_Frelon->toursrestant = 0; 			// QUESTION
	unite_Frelon->usuiv = NULL, unite_Frelon->uprec = NULL;
	unite_Frelon->colsuiv = NULL, unite_Frelon->colprec = NULL;
	unite_Frelon->vsuiv = NULL, unite_Frelon->vprec = NULL;

	return unite_Frelon;
}



void initialisation_frelons(Grille **grille){
    // Camp FRELONS au debut: 1 nid, 1 reine, 2 frelons
	Unite *F_Nid = initialisation_unite_F(NID, 0);
	Unite *F_Reine = initialisation_unite_F(REINE, FREINE);
	Unite *F_Frelon_un = initialisation_unite_F(FRELON, FFRELON);
	Unite *F_Frelon_deux = initialisation_unite_F(FRELON, FFRELON);

    (*grille)->frelon = F_Nid;     // modification du Nids

    ajoute_Insecte(&F_Nid, F_Reine);

    ajoute_Insecte(&F_Nid, F_Frelon_un);

    ajoute_Insecte(&F_Nid, F_Frelon_deux);

    
    ajoute_Unite_Case(grille, &F_Nid, 17, 11);

    ajoute_Unite_Case(grille, &F_Reine, 17, 11);

    ajoute_Unite_Case(grille, &F_Frelon_un, 17, 11);

    ajoute_Unite_Case(grille, &F_Frelon_deux, 17, 11);

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
void liberer_Grille(Grille **grille) {

    if (!(*grille)->abeille) {
        fprintf(stderr, "Il n'y a pas de colonie des abeilles\n");
    }
    if (!(*grille)->frelon) {
        fprintf(stderr, "Il n'y a pas de colonie des frelons\n");
    }

    liberer_des_colonies(&((*grille)->abeille));   // liberer les colonies des abeilles

    liberer_des_colonies(&((*grille)->frelon));    // liberer les colonies des frelons

    // On ne libere pas les lists "colonie" et "occupant" du "Case plateau[ligne][colonne]""
    // Car ce sont des pointeurs

    free(*grille);
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
    if (NULL == (new_colonie = (UListe) malloc(sizeof(Unite)))){
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
int achat_unite(Grille **grille, UListe *colonie, Unite **ptr_unite,char camp, char type){

    if (!(*colonie)->toursrestant){     // si toursrestant n'est pas egale a 0
        fprintf(stderr, "Creation interdite, l'unite est deja en production\n");
        return 0;
    }

    int prix = prix_Unite(camp, type);
    if (camp == ABEILLES && (*grille)->ressourcesAbeille < prix){          // pour les abeilles
        fprintf(stderr, "Pas assez de ressources pour acheter une unite des abeilles\n");
        return 0;
    } else if (camp == FRELONS && (*grille)->ressourcesFrelon < prix){     // pour les frelons
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


int deplacer_unite(Grille **grille, Unite **unite){
    if (!(*grille) || !(*unite)){
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
    if (!curr) { fprintf(stderr, "Il n'y pas d'occupants donc: 0\n"); return quantite; }
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



void recolte_Ouvrierre(Unite **ouvriere){
    if (!(*ouvriere)){
        fprintf(stderr, "Ouvriere est NULL, recolte annulee\n");
        return;
    }
    (*ouvriere)->temps = TRECOLTE;
    (*ouvriere)->toursrestant = TRECOLTE;   // cela diminue avec du temps
    (*ouvriere)->production = RECOLTE;
}


// Cette fonction calcule le nombre d'ouvrire qui recolte en ce moment la
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


int deux_camps_sur_case(Case caseActuelle){
    if (!(caseActuelle.occupant) ) { fprintf(stderr, "Case ne contient pas d'unites\n"); return 0; }

    char first_camp = (caseActuelle.colonie) ? caseActuelle.colonie->camp : caseActuelle.occupant->camp;
    Unite *curr_camp = caseActuelle.occupant;
    while (curr_camp){
        if (curr_camp->camp != first_camp) { return 1; }    // On a deux camps sur la meme case
        curr_camp = curr_camp->vsuiv;
    }

    return 0;
}


/*     for (int i = 0; i < LIGNES; ++i){
        for (int j = 0; j < COLONNES; ++j){
            Case caseActuelle = grille->plateau[i][j];
            if (deux_camps_sur_case(caseActuelle)){
                
            }
        }
    } */


// 0 - abeille a gagne, 1 - frelon a gagne
int bataille(Unite *unite_Abeille, Unite *unite_Frelon){
    int puissance_A = tirageDe() * unite_Abeille->force;
    int puissance_F = tirageDe() * unite_Frelon->force;
    
    // si la puissance est identique
    if (puissance_A == puissance_F){
        puissance_A = tirageDe() * unite_Abeille->force;
        puissance_F = tirageDe() * unite_Frelon->force;
    }

    return (puissance_A > puissance_F) ? (0) : (1) ;
}


/* int detruire_abeille(Unite **unite_Abeille){
    if (! (*unite_Abeille) ){
        fprintf(stderr, "Abeille n'existe pas\n");
        return 0;
    }
    if 

} */



// TO DO ajouter l'unite achetee a sa colonie et a la case de sa colonie (OBLIGATOIRE)
// TO DO supprimer les ressources depensees




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

    initialisation_abeilles(&grille);
    initialisation_frelons(&grille);

    // int count = detruire_colonie_et_rss_abeilles(grille, grille->abeille);
    // printf("On a recu: %d \n", count);
    afficheColonie(grille->abeille);
    printf("\n\n");
    // afficheColonie(grille->frelon);

    // detruire_colonie(&(grille->abeille));
    // afficheColonie(grille->abeille);

    liberer_Grille(&grille);

    fprintf(stderr, "\nla memorie liberee MAIN\n");
    
    return 0;
}
