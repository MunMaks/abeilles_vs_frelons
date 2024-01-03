#include <stdio.h>
#include <stdlib.h>
// #include <MLV/MLV_all.h>
#include <time.h>
// #include <math.h>
#include <string.h>


// Dimensions de la grille en nombre de cases (origine en haut a gauche) :
// début: (0, 0), fin: (17, 11) 
#define LIGNES 18
#define COLONNES 12

// Les deux camps :
#define ABEILLES 'A'
#define FRELONS 'F'

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


Unite* initialisation_abeilles(void){
	// Camp ABEILLES au debut: 1 ruche, 1 reine, 1 guerrière et 1 ouvrière
	Unite *A_Ruche = initialisation_ruche();
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


Unite* initialisation_frelons(void){
    // Camp FRELONS au debut: 1 nid, 1 reine, 2 frelons
	Unite* F_Nid = initialisation_nid();
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


/*************************************************/
/************** Les fonctions utiles *************/
/*************************************************/

void afficheColonie(UListe colonie){
    if (!colonie) { printf("Colonie est vide\n"); return; }

    while (colonie) {
        printf("le type est %c avec l'adress: %p\n", colonie->type, colonie);
        colonie = colonie->usuiv;
    }
}

// 1 - cette unite existe, 0 sinon
int UniteExiste(UListe colonie, Unite *unite){
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









/*************************************************/
/************** AJOUT / SUPPRESSION **************/
/*************************************************/


int ajoute_colonie(UListe *colonie_un, UListe colonie_deux)
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

    // Iteration jusqu'a dernier colonne qui est egale a NULL

    for (; (*colonie_un)->colsuiv; colonie_un = &((*colonie_un)->colsuiv)){
        ;   // boucle vide
    }

    /* while (curr) {
        curr = curr->colsuiv;
    } */

    (*colonie_un)->colsuiv = colonie_deux;
    colonie_deux->colsuiv = NULL;
    colonie_deux->colprec = (*colonie_un);

    return 1;   // l'ajout bien s'est passe
}


// A verifier cette fonction
int ajoute_insecte(UListe *colonie, Unite *new_insecte)
{
    // On suppose que colonie contient toujours au moins 1 element (Ruche ou Nid)

    // Si colonie est vide, donc insecte ne peut pas etre ajoute...
    if (! (*colonie)){ 
        fprintf(stderr, "Colonie est vide! \n");
        return 0;
    }

    Unite *curr = *colonie;     // ce n'est pas obligatoire, mais c'est plus simple

    // Iteration jusqu'a dernier unite qui n'est pas egale a NULL
    while (curr->usuiv) { curr = curr->usuiv; }
    curr->usuiv = new_insecte;

    new_insecte->usuiv = NULL;  // par defaut c'est deja NULL
    new_insecte->uprec = curr;

    return 1;
}



int ajoute_unite_case(Grille *grille, Unite *unite , int ligne, int colonne)
{
    if (!grille || !unite || ligne < 0 || ligne >= LIGNES || colonne < 0 || colonne >= COLONNES) {
        fprintf(stderr, "Paramètres invalides sur la fonction ajoute_case().\n");
        return 0;
    }

    Case *caseCourante = &(grille->plateau[ligne][colonne]);

    // Par defaut case[ligne][colonne] est vide

    // Ajoute l'unité (ou colonie) à la fin de la case actuelle
    // On suppose que sur la case on ne peut avoir que la Ruche ou que le Nid (un seul d'entre eux)

    if ( (RUCHE == unite->type || NID == unite->type)){
        if (!caseCourante->colonie) {
            caseCourante->colonie = unite; 
            return 1;
        } else {
            fprintf(stderr, "Case contient deja une RUCHE / un NID!\n");
            return 0;
        }
    }

    return ajoute_insecte(&caseCourante->occupant, unite);
}



// SUPPRESSION



int supprimeUnite(UListe *colonie, Unite *unite)   // On supprimes tous les liens
{
    if ( (! (*colonie)) || (! unite) ) {
        fprintf(stderr, "Colonie ou unite est NULL.\n");
        return 0;
    }
    UListe curr = *colonie;

    // On ne verifie pas si unite a supprimer est premiere unite car premiere unite est la colonie (Ruche/Nid)
    
    while (curr && curr != unite) { curr = curr->usuiv; }   // Recherche de l'unité

    // Si l'unité à supprimer n'est pas trouvée dans la colonie
    if (! curr ) {
        fprintf(stderr, "Unite a supprimer non trouvee dans la colonie.\n");
        return 0;
    }

    if (curr->usuiv && curr->uprec) {       // milieu
        curr->usuiv->uprec = curr->uprec; 
        curr->uprec->usuiv = curr->usuiv;
    } else if (curr->uprec) {               // fin
        curr->uprec->usuiv = NULL;
    } else if (curr->usuiv){                // debut
        curr->usuiv->uprec = NULL;
    }
    return 1;
}


void detruire_colonie(UListe *colonie)
{
    if ((! (*colonie) )) {
        fprintf(stderr, "Colonie est vide\n");
    }

    Unite * unite = *colonie;

    // Suppression des liaisons avec d'autres colonies
    if (unite->colsuiv) { unite->colsuiv->colprec = unite->colprec; }

    if (unite->colprec) { unite->colprec->colsuiv = unite->colsuiv; }

    // Une seule colonie a supprimer donc victory()
    // if (!unite->colsuiv && !unite->colprec) { printf((unite->camp == ABEILLES) ? "FRELONS ONT GAGNE\n" : "ABEILLES ONT GAGNE\n"); }

    if (!( (*colonie)->usuiv) ){
        fprintf(stderr, "Colonie n'as pas d'unites\n");
        free(colonie);  // colonie n'as pas d'unites
        return;
    }

    Unite *curr = (*colonie)->usuiv;
    Unite *suiv = NULL;
    // libere toute la memoire occupée par les unites du milieu (O-O-O devient O-X-O)
    while (curr){
        suiv = curr->usuiv;
        free(curr);
        curr = suiv;
    }

    *colonie = NULL;  // Le pointeur doit rester non libéré (a gerer dehors de cette fonction)
}



int supprimeUnite_case(Grille *grille, Unite *unite, int ligne, int colonne)
{
    if (!grille || !unite || ligne < 0 || ligne >= LIGNES || colonne < 0 || colonne >= COLONNES) {
        fprintf(stderr, "Paramètres invalides sur la fonction plateau.c *supprime_unite_case()*.\n");
        return 0;
    }

    Case *caseCourante = &(grille->plateau[ligne][colonne]);

    // L'unité à supprimer est la colonie de la case
    if (caseCourante->colonie == unite) {

        // cette fonction va detruire toutes les liaisons et tous les unites qui appartient a cette colonie
        detruire_colonie( &unite );   
        // TODO supprimer tous ces unites sur chaque case ou ils sont situes

        free(unite);
        return 1;
    }

    // L'unité à supprimer est dans la liste des occupants de la case
    UListe listeOccupants = caseCourante->occupant;
    while (listeOccupants) {
        if (listeOccupants == unite) {
            
            if (listeOccupants->uprec && listeOccupants->usuiv){        // milieu
                listeOccupants->uprec->usuiv = listeOccupants->usuiv;
                listeOccupants->usuiv->uprec = listeOccupants->uprec;
            } 
            else if (listeOccupants->uprec){                            // fin
                listeOccupants->uprec->usuiv = NULL;
            } 
            else if (listeOccupants->usuiv){                            // debut
                listeOccupants->usuiv->uprec = NULL;
            }

            free(listeOccupants);
            return 1;
        }
        listeOccupants = listeOccupants->usuiv;     // iteration
    }

    // L'unité n'a pas été trouvée liste des occupants et l'unité n'est pas egale a colonie
    fprintf(stderr, "L'unité n'est pas présente sur la case.\n");
    return 0;
}







// A supprimer plus tard
int supprime_colonie(UListe *colonie_un, UListe colonie_deux)
{
    // On peut ajouter la verification que ce sont deux colonie mais j'espere que c'est clair
    // Cette fonction supprime colonie_deux de la colonie_un
    if (!(*colonie_un) || !colonie_deux || ((*colonie_un)->camp != colonie_deux->camp)){
        fprintf(stderr, "On ne peut pas supprimer une colonie, reessayez (supprime_colonie)\n");
        return 0;
    }
    if (*colonie_un == colonie_deux){
        fprintf(stderr, "Il ne reste qu'une seule colonie\n");
        return 0;
    }

    // Jusqu'a la fin ou jusqu'a la colonie souhaitee (la version while et for)
    // while (curr && curr != colonie_deux) { curr = curr->colsuiv; }
    for (; (*colonie_un)->colsuiv; colonie_un = &((*colonie_un)->colsuiv)){
        
        if ((*colonie_un) == colonie_deux){
            if ((*colonie_un)->colsuiv){
                (*colonie_un)->colsuiv->colprec = (*colonie_un)->colprec;
            }
            if ((*colonie_un)->colprec){
                (*colonie_un)->colprec->colsuiv = (*colonie_un)->colsuiv;
            }
            return 1;
        }
    }

    Unite * curr = (*colonie_un);
    if (!curr){ fprintf(stderr, "Colonie n'est pas trouvee!\n"); return 0; }

    if (curr->colprec && curr->colsuiv) {   // milieu
        curr->colprec->colsuiv = curr->colsuiv;
        curr->colsuiv->colprec = curr->colprec;
    } else if (curr->colprec) {     // fin
        curr->colprec->colsuiv = NULL;
    } else if (curr->colsuiv) {     // debut
        curr->colsuiv->colprec = NULL;
    } else {
        fprintf(stderr, "La partie est finie, car il ne reste plus de colonie");
    }

    return 1;
    
}






int detruire_colonie_et_rss_abeilles(Grille *grille, UListe A_colonie)
{
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
            supprimeUnite_case(grille, curr_unite, curr_unite->posx, curr_unite->posy);
            // Chercher l'uniter actuelle pour la vider de la case ou elle est presente
            // free(curr_unite);

            curr_unite = unite_suivante;      // iteration
        }

    }

    // POUR LES TESTS
    (supprimeUnite_case(grille, A_colonie, A_colonie->posx, A_colonie->posy)) ? 
    printf("Colonie : %p est detruite RSS. \n", A_colonie) : 
    printf("Colonie : %p n'est pas detruite RSS. \n", A_colonie);

    // Quantite total pour ajouter au grille->ressourcesAbeille
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

    grille->abeille = NULL;     // RUCHE -> Reine -> Guerriere -> Ouvriere
    grille->frelon = NULL;      // NID -> Reine -> Frelon_un -> Frelon_deux

	grille->plateau[0][0].colonie = grille->abeille;                        // RUCHE
    grille->plateau[LIGNES - 1][COLONNES - 1].colonie = grille->frelon;     // NID

    grille->tour = 0;   // apres chaque tour on l'incremente
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
        detruire_colonie(&col_Courante);
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

    liberer_des_colonies(&(grille->abeille));   // liberer la colonie des abeilles
    //free(&(grille->abeille));
    liberer_des_colonies(&(grille->frelon));    // liberer la colonie des frelons
    //free(&(grille->frelon));
    // On ne libere pas les lists "colonie" et "occupant"
    // car ce sont des colonies qu'on a vient de libere

    free(grille);
}






Unite * creation_unite(UListe* colonie, char type, int force, int temps){
    Unite *new_unite = NULL;
    if (NULL == (new_unite = (Unite *)malloc(sizeof(Unite)))){
        fprintf(stderr, "Unite n'est pas cree\n");
        return NULL;
    }
    new_unite->camp = (*colonie)->camp;
    new_unite->type = type;
    new_unite->force = force;
    new_unite->temps = temps;
    new_unite->posx = (*colonie)->posx, new_unite->posy = (*colonie)->posy;
    new_unite->destx = 0, new_unite->desty = 0; // pour l'instant elle bouge pas
    new_unite->production = '0';
    new_unite->toursrestant = temps;
    if (!ajoute_insecte(colonie, new_unite)){   // si c'est mal passe
        new_unite->usuiv = NULL, new_unite->uprec = NULL;
    }
    new_unite->colsuiv = NULL, new_unite->colprec = NULL;
    new_unite->vsuiv = NULL, new_unite->vprec = NULL;

}







int main(int argc, char *argv[]){
    Grille *grille = initialiserGrille();
    grille->abeille = initialisation_abeilles();
    grille->frelon = initialisation_frelons();
    UListe colonie_2 = initialisation_abeilles();
    ajoute_colonie(&(grille->abeille), colonie_2);

    // grille->frelon = initialisation_frelons();
    // UListe new_col = initialisation_abeilles();
    // ajoute_colonie(&(grille->abeille), new_col);

    //afficheColonie(grille->abeille);
    fprintf(stderr, "\n\n");
    // afficheColonie(grille->frelon);

    //detruire_colonie(&(grille->abeille));
    //afficheColonie(grille->abeille);

    liberer_Grille(grille);

    fprintf(stderr, "\nla memorie liberee MAIN\n");

    return 0;
}
