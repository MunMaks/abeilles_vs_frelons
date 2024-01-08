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
#define MAX_REINES 12   // la quantite des reines qui on deja construit une colonie

// Les types d'unites :
#define REINE 'r'
#define OUVRIERE 'o'
#define ESCADRON 'e'
#define GUERRIERE 'g'
#define FRELON 'f'
#define RUCHE 'R'
#define NID 'N'

// Pour la recolte de pollen
#define RECOLTE 'P' // preciser dans le rapport

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
	struct unite *colsuiv, *colprec;		// liste des autres ruches ou nids (colonies) du meme camp
	struct unite *vsuiv, *vprec;			// liste des autres unites sur la meme case
} Unite, *UListe;


// La structure Case :
typedef struct {
	Unite *colonie; 		// S'il y a une ruche ou un nid sur la case
	UListe occupant; 		// les autres occupants de la case
} Case;
// JE propose de modifier la structure Case au but d'avoir deux pointeurs:
// pour les abeilles et pour les frelons comme pour grille
// au but d'eviter de chercher chaque fois quel unite est present et de quel camp


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
void afficheColonie(UListe colonie) // tested
{
    if (!colonie) { printf("Colonie est vide\n"); return; }
    Unite *curr = colonie;
    while (curr) {
        printf("le type est %c avec coordonnees [%d][%d] sur l'adress: %p\n", curr->type, curr->posx, curr->posy, (void *)curr);
        curr = curr->usuiv;
    }
}


void afficheCase(Case caseCourante) // tested
{
    Unite *curr_unite = caseCourante.colonie;
    if (caseCourante.colonie){
        printf("Colonie de %c sur case [%d][%d]\n", curr_unite->camp, curr_unite->posx, curr_unite->posy);
    }
    if (caseCourante.occupant){
        curr_unite = caseCourante.occupant;
        while (curr_unite){
            printf("Unite : %c sur case [%d][%d]\n", curr_unite->type, curr_unite->posx, curr_unite->posy);
            curr_unite = curr_unite->vsuiv;
        }
    }
}



// 1 - cette unite existe, 0 sinon
int uniteExiste(UListe colonie, Unite *unite)   // tested
{
    if (!colonie || !unite){
        fprintf(stderr, "Colonie est vide ou unite est NULL (exists)\n");
        return 0;
    }
    UListe curr = colonie;
    while(curr){
        if (curr == unite) { return 1; }
        curr = curr->usuiv;
    }
    return 0;
}



// Trouver colonie d'une unite
UListe trouver_Colonie(Unite *unite)    // tested
{
    if (!unite){
        fprintf(stderr, "On peut pas trouver la colonie, unite n'existe pas\n");
        return NULL;
    }

    // Iteration jusqu'a sa colonie 
    // (on suppose que colonie n'a pas d'unites precedentes)
    Unite *curr = unite;
    while (curr) {
        if ((curr->type == RUCHE || curr->type == NID)){
            return curr;
        }
        curr = curr->uprec;
    }
    fprintf(stderr, "On n'a pas trouve COLONIE\n");
    // "operateur ternaire" autrement dit si colonie donc on la retourne, NULL sinon
    return NULL;
}


Unite *trouver_Reine(UListe colonie){
    if (!colonie){
        fprintf(stderr, "Colonie est vide, on ne trouve pas la reine\n");
        return NULL;
    }
    Unite *curr = colonie;
    while(curr){
        if (REINE == curr->type) { return curr; }
        curr = curr->usuiv;
    }
    return NULL;
}



int case_Est_Vide(Case caseCourante)    // tested
{
    // Operateur ternaire, s'il n'y ni colonie ni unites sur la case courante
    return ( !(caseCourante.colonie) && !(caseCourante.occupant) ) ? (1) : (0) ;
}



int prix_Unite(char camp, char type)    // tested
{
    // Je veux eviter le cas avec (FRELONS, ESCADRON)
    int prix = 0;
    switch (type) {
        case RUCHE: prix = (camp == ABEILLES) ? CRUCHE : -1 ; break;
        case NID: prix = (camp == FRELONS)? CNID : -1; break;
        case REINE: prix = (camp == ABEILLES) ? CREINEA : CREINEF; break;
        case GUERRIERE: prix = (camp == ABEILLES) ? CGUERRIERE : -1 ; break;
        case ESCADRON: prix = (camp == ABEILLES) ? CESCADRON : -1 ; break;
        case OUVRIERE: prix = (camp == ABEILLES) ? COUVRIERE : -1 ; break;
        case FRELON: prix = (camp == FRELONS) ? CFRELON : -1; break;

        default: fprintf(stderr, "Unite n'existe pas, reessayez"); break;
    }
    return prix;
}


int force_Unite(char type)  // tested
{
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



int temps_Unite(char camp, char type)   // tested
{
    // Je veux eviter le cas avec (ABEILLES, NID)
    int temps = 0;
    switch (type) {
        case RUCHE: temps = (camp == ABEILLES) ? 0 : -1; break;
        case NID: temps = (camp == FRELONS) ? 0 : -1; break;
        case REINE: temps = (camp == ABEILLES) ? (TREINEA) : (TREINEF); break;
        case GUERRIERE: temps = (camp == ABEILLES) ? TGUERRIERE : -1 ; break;
        case ESCADRON: temps = (camp == ABEILLES)? TESCADRON : -1 ; break;
        case OUVRIERE: temps = (camp == ABEILLES) ? TOUVRIERE : -1; break;    // plus tard TRECOLTE
        case FRELON: temps = (camp == FRELONS) ? TFRELON : -1; break;

        default: fprintf(stderr, "Unite n'existe pas, reessayez"); break;
    }
    return temps;
}


unsigned int tirage_au_hasard(unsigned int x, unsigned int y)   // tested
{
    unsigned int resultat = x + (rand() % (y - x + 1));
    return resultat;
}

int tirageDe(void)  // tested
{
    // D'apres PDF sur e-learning:
    // L'issue du combat est déterminée par tirage aléatoire d'un dé à 60 faces
    return rand() % 60 + 1; // entre 0 et 59, avec +1 on aura de 1 a 60 (inclus)
}



/***********************************************/
/***************** SUPPRESSION *****************/
/***********************************************/



// Effectue la suppression de lien entre unite et sa case
int supprime_Insecte_Case(Unite **unite)        // tested
{
    if (! (*unite) ){
        fprintf(stderr, "Unite n'existe pas\n");
        return 0;
    }
    // unite suivante (sur case)
    if ((*unite)->vsuiv){ (*unite)->vsuiv->vprec = (*unite)->vprec; }

    // unite precedente (sur case)
    if ((*unite)->vprec){ (*unite)->vprec->vsuiv = (*unite)->vsuiv; }
    
    (*unite)->vprec = NULL, (*unite)->vsuiv = NULL;
    return 1;
}


// Effectue la suppression de lien entre unite et sa colonie
int supprime_Insecte_Col(Unite **unite) // tested
{
    if (! (*unite)){
        fprintf(stderr, "Unite n'existe pas\n");
        return 0;
    }
    // unite suivante de sa colonie
    if ((*unite)->usuiv){ (*unite)->usuiv->uprec = (*unite)->uprec; }

    // unite precedente de sa colonie
    if ((*unite)->uprec){ (*unite)->uprec->usuiv = (*unite)->usuiv; }

    (*unite)->uprec = NULL, (*unite)->usuiv = NULL;
    return 1;
}


// Effectue la suppression de lien entre colonie et d'autres colonies
int supprime_Colonie(UListe *colonie)   // tested
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

    (*colonie)->colsuiv = NULL, (*colonie)->colprec = NULL;
    return 1;
}



// suppression des liaisons d'une unite (colonie ou insecte) sur la case
int supprime_Unite_Case(Grille **grille, Unite **unite) // tested
{
    if (!(*grille) || !(*unite)) {
        fprintf(stderr, "Parametres invalides: *supprime_Unite_Case()*.\n");
        return 0;
    }

    Case *caseCourante = &((*grille)->plateau[(*unite)->posx][(*unite)->posy]);

    if (RUCHE == caseCourante->colonie->type || NID == caseCourante->colonie->type){
        // L'unité à supprimer est la colonie de la case
        if (caseCourante->colonie == (*unite)) {
            caseCourante->colonie = NULL;
            return 1;   // il faut pas oublier de liberer cette colonie dehors
        }
        fprintf(stderr, "Colonie existe deja sur cette case, en plus colonie a supprimer n'est pas ici\n");
        return 0;
    }


    if (! uniteExiste((*grille)->abeille, (*unite)) ){
        fprintf(stderr, "Unite %c sur l'adress: %p n'est pas present sur la case\n", (*unite)->type, (void *)(*unite));
        return 0;
    }

    // Case contient un seul insecte
    if (! (caseCourante->occupant->vsuiv) ){
        caseCourante->occupant = NULL;
        return 1;
    }
    // Case contient au moins deux insecte et c'est premiere unite a supprimer
    if ( (caseCourante->occupant == (*unite) ) && (caseCourante->occupant->vsuiv) ){
        caseCourante->occupant = caseCourante->occupant->vsuiv;
        return 1;
    }
    if (! supprime_Insecte_Case(unite)){
        fprintf(stderr, "On n'a pas reussi a supprimer de lien avec sa colonie\n");
        return 0;
    }

    return 1;
}


// On supprime tous les liens et libere la memoire d'une unite
void detruire_Unite(Unite **unite)  // tested
{   
    if (! (*unite) ) {
        fprintf(stderr, "Unite est NULL.\n");
        return;
    }
    if (! supprime_Insecte_Col(unite)){
        fprintf(stderr, "On n'a pas reussi a supprimer de lien d'une unite avec sa colonie\n");
        return;
    }
    
    if (! supprime_Insecte_Case(unite)){
        fprintf(stderr, "On n'a pas reussi a supprimer de lien d'une unite sur sa case\n");
        return;
    }

    free(*unite);    // unite a supprimer
}



void detruire_Colonie(Grille **grille, UListe *colonie) //tested
{
    if ((! (*grille) || ! (*colonie) )) {
        fprintf(stderr, "Colonie / grille est vide\n");
        return;
    }

    // Suppression des liaisons avec d'autres colonies
    supprime_Colonie(colonie);

    // Une seule colonie a supprimer donc victory()
    // if (!unite->colsuiv && !unite->colprec) { printf((unite->camp == ABEILLES) ? "FRELONS ONT GAGNE\n" : "ABEILLES ONT GAGNE\n"); }

    if (!( (*colonie)->usuiv) ){
        fprintf(stderr, "Colonie n'as pas d'unites\n");
        free(*colonie);      // colonie n'as pas d'unites
        *colonie = NULL;
        return;
    }

    supprime_Unite_Case(grille, colonie);   // la suppression de colonie sur la case
    Unite *curr = (*colonie);
    Unite *suiv = NULL;

    while (curr){
        suiv = curr->usuiv;
        supprime_Insecte_Case(&curr);  // supprimer le lien sur case d'une unite
        free(curr);
        curr = suiv;
    }

    // free(*colonie);     // libere la memoire prise par colonie
    *colonie = NULL;    // a la fin le pointeur doit etre NULL
}



// Cette fonction va chercher tous les unites sur la grille pour supprimer les liaisons
/* int supprime_Liaisons_Colonie(UListe *colonie){
    if (! (*colonie) ){
        fprintf(stderr, "La suppression est impossbile\n");
        return 0;
    }
    Unite *curr = *colonie;
    while (curr){
        if (!supprime_Insecte_Col(&curr)){
            fprintf(stderr, "La suppression des liaisons d'une colonie n'est pas atteinte\n");
            return 0;
        }
        curr = curr->usuiv;
    }
    return 1;
} */




int detruire_Colonie_et_rss_abeilles(Grille **grille, UListe *A_colonie)    // tested
{
    if ( !(*grille) || (! A_colonie) ) {
        fprintf(stderr, "Parametres invalides.\n");
        return 0;
    }

    int ressources_totales = 0;

    // operateur ternaire (pour etre sur)
    Unite *curr_unite = ((*A_colonie)->type == RUCHE) ? (*A_colonie)->usuiv : (*A_colonie);

    while (curr_unite) {
        switch (curr_unite->type) {
            case REINE: ressources_totales += CREINEA; break;

            case GUERRIERE: ressources_totales += CGUERRIERE; break;

            case ESCADRON: ressources_totales += CESCADRON; break;

            case OUVRIERE: ressources_totales += COUVRIERE; break;

            default: fprintf(stderr, "Il y a une erreur de type d'unite, reessayez : %c\n", curr_unite->type); break;
        }
        curr_unite = curr_unite->usuiv;     // iteration
    }
    detruire_Colonie(grille, A_colonie);    // suppression de toutes les liaisons et libere la memorie

    return ressources_totales;       // mis-a-jour: ajouter au grille->ressourcesAbeille
}



/***********************************************/
/******************** AJOUT ********************/
/***********************************************/



int ajoute_Colonie(UListe *colonie_un, UListe colonie_deux)
{
    // On suppose que colonie contient toujours au moins 1 element (Ruche ou Nid)
    if (!colonie_deux) {
        fprintf(stderr, "Colonie_2 est vide, rien a ajouter\n"); return 0;
    }

    // If colonie_un is empty, set it to colonie_deux
    if (!(*colonie_un)) {
        *colonie_un = colonie_deux; return 1;
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
    if (! (*colonie) || !new_insecte) { 
        fprintf(stderr, "Impossible d'ajouter cette unite, reessayez\n");
        return 0;
    }

    Unite *curr = *colonie;

    // Iteration jusqu'a dernier unite qui n'est pas egale a NULL
    while (curr->usuiv) { curr = curr->usuiv; }

    curr->usuiv = new_insecte;
    new_insecte->uprec = curr;
    // new_insecte->usuiv = NULL;  // par defaut c'est deja NULL
    return 1;
}



int ajoute_Unite_Case(Grille **grille, Unite **unite , int ligne, int colonne)
{   // TESTED
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
            fprintf(stderr, "Case [%d][%d] contient deja une RUCHE / un NID!\n", ligne, colonne);
            return 0;
        }
    }

    // On supprime les liaisons du case precedente
    supprime_Insecte_Case(unite);
    
    if (! caseCourante->occupant) { caseCourante->occupant = (*unite); }     // Il n'y pas d'insecte sur la case
    else {
        Unite *curr = caseCourante->occupant;
        // Iteration jusqu'a derniere unite qui n'est pas egale a NULL
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
        exit(EXIT_FAILURE);
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



void liberer_des_colonies(Grille **grille, UListe *colonie)
{
    if ( !(*grille) || !(*colonie)){ 
        fprintf(stderr, "Colonie / grille est vide\n");
        return;
    }
    UListe col_Courante = *colonie;
    UListe col_Suivante = (*colonie)->colsuiv;
    while (col_Courante) {
        col_Suivante = col_Courante->colsuiv;
        detruire_Colonie(grille, &col_Courante);
        col_Courante = col_Suivante;
    }

    *colonie = NULL;
}




// liberer toute la memoire alloue
void liberer_Grille(Grille **grille)
{
    if (!(*grille)->frelon) { fprintf(stderr, "Il n'y a pas de colonie des frelons\n"); }

    if (!(*grille)->abeille) { fprintf(stderr, "Il n'y a pas de colonie des abeilles\n"); }

    liberer_des_colonies(grille, &((*grille)->abeille));   // liberer les colonies des abeilles

    liberer_des_colonies(grille, &((*grille)->frelon));    // liberer les colonies des frelons

    // On ne libere pas les lists "colonie" et "occupant" du Case [ligne][colonne]
    // Car ce sont des pointeurs
    free(*grille);
}


Unite *creation_Unite(Grille **grille, UListe* colonie, char type) // TESTED
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
    new_unite->destx = 0, new_unite->desty = 0;     // pour l'instant elle bouge pas
    new_unite->production = '0';
    new_unite->temps = 0;
    new_unite->toursrestant = 0;    // avec du temps cela decremente

    // usuiv, uprec
    if (! ajoute_Insecte(colonie, new_unite)){
        fprintf(stderr, "On n'a pas reussi a ajouter unite (creation_Unite())\n");
        return NULL;
    }

    new_unite->colsuiv = NULL, new_unite->colprec = NULL;

    // vsuiv, vprec
    ajoute_Unite_Case(grille, &new_unite, new_unite->posx, new_unite->posy);

    return new_unite;
}



UListe creation_Colonie(Unite **reine)    //tested
{
    // C'est impossible, mais pour etre sur.
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
    new_colonie->destx = 0, new_colonie->desty = 0;     // elle bouge pas
    new_colonie->production = '0';      // quel type d'unite est en train d'etre produit
    new_colonie->temps = 0;
    new_colonie->toursrestant = 0;

    UListe colonie_de_reine = trouver_Colonie(*reine);

    // colsuiv, colprec
    if (! ajoute_Colonie(&colonie_de_reine, new_colonie)){  // l'ajout nouvelle colonie avec les autres
        new_colonie->colsuiv = NULL, new_colonie->colprec = NULL;
    }

    // on ne peut pas supprimer les liens tel que la reine n'existe pas 
    if (! supprime_Insecte_Col(reine)){
        fprintf(stderr, "Probleme de creation d'une colonie, la reine n'existe pas\n");
        return NULL;
    }

    // On a supprime tous les liens de la reine avec la colonie precedente
    new_colonie->usuiv = *reine, new_colonie->uprec = NULL;
    (*reine)->uprec = new_colonie;

    new_colonie->vsuiv = NULL, new_colonie->vprec = NULL;    // Pour les colonie, cela n'est pas necessaire

    return new_colonie;
}



// L'achat d'une unite avec son samp et son type
int achat_Unite(Grille **grille, UListe *colonie, char type) // tested
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

    //Unite *new_unite = creation_Unite(colonie, type, force, temps);

    int temps = temps_Unite((*colonie)->camp, type);
    (*colonie)->production = type;      // type d'unite en cours de production
    (*colonie)->temps = temps;          // nbr tours totales
    (*colonie)->toursrestant = temps;   // nbr tours restants


    // tel que la colonie est en train de productoin cette unite
    // On va le sauvegarder et tel que colonie finit sa production
    // On va lier cette unite avec sa colonie et cette unite soit sur la case de sa colonie


    // on diminie les ressources a la fin du tour
    if ( (*colonie)->camp == ABEILLES ){          // pour les abeilles
        (*grille)->ressourcesAbeille -= prix;
    } else if ( (*colonie)->camp == FRELONS ){     // pour les frelons
        (*grille)->ressourcesFrelon -= prix;
    }

    // dehors de cette fonction
    // if (! ajoute_Insecte(colonie, new_unite) ){ // l'ajout a la colonie actuelle
    //     fprintf(stderr, "Probleme ajoute unite, apres avoir achete\n");
    //     free(new_unite);    // on ne peut pas l'ajouter, donc on la libere
    //     return 0;
    // }
    // if (! ajoute_Unite_Case(grille, new_unite, new_unite->posx, new_unite->posy) ){ // l'ajout sur la case necessaire (a la fin)
    //     fprintf(stderr, "Probleme ajoute unite, apres avoir achete\n");
    //     if (new_unite) { free(new_unite); }
    //     return 0;
    // }

    return 1;
}



/* void change_Direction(Unite **unite, int dest_x, int dest_y){
    if (! (*unite) ){
        fprintf("Unite n'existe pas, reesayez\n");
        return;
    }

}

 */

// n'est plus utile
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

    // cette fonction supprime la connexion precedente et ajoute unite a la fin d'une nouvelle case
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






// Quantite totale du type souhaite
int nbr_Unite_Case(Case caseActuelle, char type){
    int quantite = 0;
    Unite *curr = caseActuelle.colonie;
    if (!curr) { fprintf(stderr, "Il n'y pas d'occupants donc: 0\n"); return quantite; }
    while (curr){
        if (curr->type == type) { ++quantite; }
        curr = curr->vsuiv;
    }
    return quantite;
}


// Quantite totale sur Case
int nbr_Unites_totale_Case(Case caseActuelle){
    int nb_unites = 0;

    Unite *curr = caseActuelle.occupant;
    while (curr){
        ++nb_unites;
        curr = curr->vsuiv;
    }
    return nb_unites;
}


// La fonction qui commence la recolte d'une ouvriere
void start_recolte_Ouvrierre(Unite **ouvriere){
    if (!(*ouvriere)){
        fprintf(stderr, "Ouvriere n'existe pas, recolte est annulee\n");
        return;
    }
    if ((*ouvriere)->toursrestant){
        fprintf(stderr, "Ouvriere est en train de recolter\n");
        return;
    }
    (*ouvriere)->temps = TRECOLTE;
    (*ouvriere)->toursrestant = TRECOLTE;   // cela diminue avec du temps
    (*ouvriere)->production = RECOLTE;
}




// le nombre d'ouvrires qui sont en cours de recolte
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


// Determine s'il existe deux camps sur la meme case
int deux_camps_sur_case(Case caseActuelle){
    if (case_Est_Vide(caseActuelle)){
        fprintf(stderr, "Case est vide\n");
        return 0;
    }
    // Case n'est pas vide, donc premier camp est 
    // camp de colonie sur case ou de premiere unite occupant
    char first_camp = (caseActuelle.colonie) ? caseActuelle.colonie->camp : caseActuelle.occupant->camp;

    Unite *curr_camp = caseActuelle.occupant;
    while (curr_camp){
        if (curr_camp->camp != first_camp) { return 1; }    // On a deux camps differents sur la meme case
        curr_camp = curr_camp->vsuiv;
    }

    return 0;   // Un camps sur la case
}

/* 
int colonie_est_seul(Case caseCourante){
    if ()
} */






/*     f */


// 0 - abeille a gagne, 1 - frelon a gagne
int bataille(Unite *unite_Abeille, Unite *unite_Frelon)
{
    int puissance_A = tirageDe() * unite_Abeille->force;
    int puissance_F = tirageDe() * unite_Frelon->force;

    // si la puissance est identique
    if (puissance_A == puissance_F){
        puissance_A = tirageDe() * unite_Abeille->force;
        puissance_F = tirageDe() * unite_Frelon->force;
    }
    // 1:3600 chance que deux fois la valeur des unites soit egale 
    return (puissance_A > puissance_F) ? (0) : (1) ;
}


/* int detruire_abeille(Unite **unite_Abeille){
    if (! (*unite_Abeille) ){
        fprintf(stderr, "Abeille n'existe pas\n");
        return 0;
    }
    if 

} */


void decremente(Grille **grille, UListe *colonie){
    if ( !(*grille) || ! (*colonie) ){
        fprintf(stderr, "Il y a une erreur avec la grille / la colonie decremente()\n");
        return;
    }
    UListe curr_col = *colonie;
    while (curr_col){
        Unite *curr_unite = curr_col;
        while(curr_unite){

                if (curr_unite->toursrestant && curr_unite->production != '0') {            // au moins 1
                    if (curr_unite->type == OUVRIERE){ (*grille)->ressourcesAbeille++; }    // la recolte
                    curr_unite->toursrestant--;
                } 
                else if (!curr_unite->toursrestant && curr_unite->production != '0'){
                    
                    // ouvriere meurt sans avoir donne rss pour les frelons
                    if (curr_unite->type == OUVRIERE && curr_unite->production == RECOLTE){
                        detruire_Unite(&curr_unite);
                    } 
                    else {    // toursrestant vaut 0 et on cree l'unite
                        creation_Unite(grille, &curr_unite, curr_unite->production);
                    }
                }

            curr_unite = curr_unite->usuiv;
        }
        curr_col = curr_col->colsuiv;
    }
}


void decremente_Tout(Grille **grille)
{   // pour les abeilles
    if (!(*grille)){
        fprintf(stderr, "Probleme de la grille\n");
        return;
    }
    // decrementer tous les unites necessaires pour les abeilles
    if ((*grille)->abeille) { decremente(grille, &((*grille)->abeille)); }

    // decrementer tous les unites necessaires pour les frelons
    if ((*grille)->frelon) { decremente(grille, &((*grille)->frelon)); }
}





// TO DO ajouter l'unite achetee a sa colonie et a la case de sa colonie (OBLIGATOIRE)
// TO DO supprimer les ressources depensees

// TO DO fonction qui verifie pour une unite le nbr de tours restant pour etre apparaitre
// TO DO une fonction 


int main(int argc, char *argv[]){

    srand( (unsigned int) time(NULL));  // aletoire
    unsigned int x = 0, y = 1;  // On dit ABEILLES - 0, FRELONS - 1
    unsigned int resultat_tirage = tirage_au_hasard(x, y);
    printf("\nResultat du tirage aleatoire: %u\n\n", resultat_tirage);     // a supprimer plus tard


    // TEST une array des reines qui ont deja contruit une colonie
    // Unite *reine = initialisation_unite_A(REINE, FREINE);
    // int nbr_reines = 0;
    // UListe *reines_liste = (UListe *)malloc(MAX_REINES * sizeof(UListe));
    // if (!reine_deja_contruit(reines_liste, &nbr_reines, reine)){
    //     printf("Cette reine construit maintenant la colonie\n");
    // }
    // if (reine_deja_contruit(reines_liste, &nbr_reines, reine)){
    //     printf("Cette reine a deja construit la colonie\n");
    // }
    // free(reine);
    // free(reines_liste);     // la liste des reines qui ont deja construit une colonie 



    Grille *grille = initialiserGrille();

    initialisation_abeilles(&grille);   // Grille **
    initialisation_frelons(&grille);

    // achat_Unite(Grille **grille, UListe *colonie, Unite **ptr_unite, char type)


    Unite *reine = trouver_Reine(grille->abeille);
    ajoute_Unite_Case(&grille, &reine , 1, 1);

    UListe new_col = creation_Colonie(&reine);
    ajoute_Colonie(&(grille->abeille), new_col);
    ajoute_Unite_Case(&grille, &new_col, 1, 1);
    afficheColonie(new_col);

    // valgrind --leak-check=yes ./test
    // detruire_Colonie(&new_col);

    afficheColonie(grille->abeille);
    printf("\n\n");
    afficheColonie(grille->frelon);

    // detruire_colonie(&(grille->abeille));
    // afficheColonie(grille->abeille);

/*     printf("\n\n");
    afficheCase(grille->plateau[17][11]); */

    liberer_Grille(&grille);

    fprintf(stderr, "\nla memorie liberee MAIN\n");
    
    return 0;
}
