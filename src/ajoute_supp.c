#include "ajoute_supp.h"

/* TOUS LES COMMENTAIRES NECESSAIRES SE TROUVES DANS AJOUTE_SUPP.H*/



/**
 * @brief Verifie si l'unite cherche existe dans cette caseActuelle
 * 
 * @param caseActuelle 
 * @param unite 
 * @return int 1 oui, 0 non
 */
static int uniteExiste(Case *caseActuelle, Unite *unite)
{
    if (!caseActuelle || !unite){
        fprintf(stderr, "Case actuelle est vide ou unite est NULL (exists)\n");
        return 0;
    }
    UListe curr = caseActuelle->occupant;
    while (curr){
        if (curr == unite) { return 1; }
        curr = curr->usuiv;
    }
    return 0;
}




/***********************************************/
/***************** SUPPRESSION *****************/
/***********************************************/



int supprime_Insecte_Case(Unite **unite)
{
    if ( !(*unite) ){
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



int supprime_Insecte_Col(Unite **unite)
{
    if ( !(*unite) ){
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



int supprime_Colonie(UListe *colonie)
{
    // Si colonie existe
    if ( !(*colonie) ){
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



int supprime_Unite_Case(Grille **grille, Unite **unite)
{
    if (!(*grille) || !(*unite)) {
        fprintf(stderr, "Parametres invalides: `supprime_Unite_Case()`.\n");
        return 0;
    }

    /* vérifier plus tard plateau[posx][posy] */
    Case *caseCourante = &((*grille)->plateau[(*unite)->posx][(*unite)->posy]);

    if (NID == caseCourante->colonie->type || 
        RUCHE == caseCourante->colonie->type){
        // L'unité à supprimer est la colonie de la case
        if (caseCourante->colonie == (*unite)) {
            caseCourante->colonie = NULL;
            return 1;
        }
        return 0;
    }


    if ( !uniteExiste(caseCourante, *unite) ){
        fprintf(stderr, "Unite %c sur l'adress: %p n'est pas present sur la case\n", (*unite)->type, (void *)(*unite));
        return 0;
    }

    // Case contient un seul insecte
    if ( !(caseCourante->occupant->vsuiv) ){
        caseCourante->occupant = NULL;
        return 1;
    }

    // Case contient au moins deux insecte et c'est premiere unite a supprimer
    if ( (caseCourante->occupant == (*unite) ) && (caseCourante->occupant->vsuiv) ){
        caseCourante->occupant = caseCourante->occupant->vsuiv;
        return 1;
    }

    if ( !supprime_Insecte_Case(unite) ){
        fprintf(stderr, "On n'a pas reussi a supprimer de lien avec sa colonie\n");
        return 0;
    }

    return 1;
}



void detruire_Unite(Unite **unite)
{   
    if (! (*unite) ) {
        fprintf(stderr, "Unite est NULL.\n");
        return;
    }
    if ( !supprime_Insecte_Col(unite)){
        fprintf(stderr, "On n'a pas reussi a supprimer de lien d'une unite avec sa colonie\n");
        return;
    }
    
    if ( !supprime_Insecte_Case(unite)){
        fprintf(stderr, "On n'a pas reussi a supprimer de lien d'une unite sur sa case\n");
        return;
    }
    free(*unite);   /* liberer la memoire */
}



void detruire_Colonie(Grille **grille, UListe *colonie)
{
    if ( (!(*grille) || !(*colonie)) ) {
        fprintf(stderr, "Colonie / grille est vide\n");
        return;
    }

    // Suppression des liaisons avec d'autres colonies
    supprime_Colonie(colonie);

    if (!( (*colonie)->usuiv) ){
        fprintf(stderr, "Colonie n'as pas d'unites\n");
        free(*colonie);
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

    *colonie = NULL;    // a la fin le pointeur doit etre NULL
}



int detruire_Colonie_et_rss_abeilles(Grille **grille, UListe *A_colonie)
{
    if ( !(*grille) || (! A_colonie) ) {
        fprintf(stderr, "Parametres invalides.\n");
        return 0;
    }

    int ressources_totales = 0;

    // operateur ternaire
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

    return ressources_totales;
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



/***********************************************/
/******************** AJOUT ********************/
/***********************************************/



int ajoute_Colonie(UListe *colonie_un, UListe colonie_deux)
{
    // On suppose que colonie contient toujours au moins 1 element (Ruche ou Nid)

    if (!colonie_deux) {
        fprintf(stderr, "Colonie_2 est vide, rien a ajouter\n"); return 0;
    }

    if (!(*colonie_un)) {
        (*colonie_un) = colonie_deux;
        return 1;
    }

    // Iteration jusqu'a dernier colonne qui n'est egale a NULL
    Unite* curr = *colonie_un;
    while (curr->colsuiv) { curr = curr->colsuiv; }
    
    curr->colsuiv = colonie_deux;
    colonie_deux->colprec = curr;
    colonie_deux->colsuiv = NULL;

    return 1;   // l'ajout bien s'est passe
}



int ajoute_Insecte(UListe *colonie, Unite *new_insecte)
{
    if (! (*colonie) || !new_insecte) { 
        fprintf(stderr, "Impossible d'ajouter cette unite, reessayez\n");
        return 0;
    }
    Unite *curr = *colonie;

    if (!curr){ fprintf(stderr, "Impossible d'ajouter il n'y pas de colonie\n"); return 0; }
    
    if (curr->usuiv){
        // Iteration jusqu'a dernier unite qui n'est pas egale a NULL
        while (curr->usuiv) { curr = curr->usuiv; }
    }
    curr->usuiv = new_insecte;
    curr->usuiv->uprec = curr;
    
    return 1;
}



int ajoute_Unite_Case(Grille **grille, Unite **unite, int x, int y)
{
    if (!(*grille) || !(*unite) || x < 0 || x >= NB_COLONNES || y < 0 || y >= NB_LIGNES) {
        fprintf(stderr, "Paramètres invalides a la fonction ajoute_case() x: %d y: %d.\n", x, y);
        return 0;
    }

    Case *caseCourante = &((*grille)->plateau[x][y]);

    // Ajoute l'unité (ou colonie) à la fin de la case actuelle
    // On suppose que sur la case on ne peut avoir que la Ruche ou que le Nid (un seul d'entre eux)

    if ( (RUCHE == (*unite)->type || NID == (*unite)->type)){
        if (!caseCourante->colonie) {

            caseCourante->colonie = (*unite);
            (*unite)->posx = x, (*unite)->posy = y;
            return 1;
        } else {
            fprintf(stderr, "Case [%d][%d] contient deja une RUCHE / un NID!\n", x, y);
            return 0;
        }
    }

    // On supprime les liaisons du case precedente (si elles existent)
    if ((*unite)->vsuiv || (*unite)->vprec){
        supprime_Insecte_Case(unite);
    }
    
    if (! caseCourante->occupant) { caseCourante->occupant = (*unite); }     // Il n'y pas d'insecte sur la case
    else {
        Unite *curr = caseCourante->occupant;
        // Iteration jusqu'a derniere unite qui n'est pas egale a NULL
        while (curr->vsuiv) { curr = curr->vsuiv; }

        curr->vsuiv = (*unite);
        (*unite)->vprec = curr;
        (*unite)->vsuiv = NULL;
    }
    (*unite)->posx = x, (*unite)->posy = y;
    return 1;
}



/*************************************************/
/************** Les fonctions utiles *************/
/*************************************************/



void afficheColonie(UListe colonie)
{
    if (!colonie) { printf("Colonie est vide\n"); return; }

    while (colonie) {
        if (!colonie) { break; }
        colonie = colonie->usuiv;
    }
}


void afficheCase(Case caseCourante)
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



UListe trouver_Colonie(Unite *unite)
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
    return NULL;
}


Unite *trouver_Reine(UListe colonie)
{
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



int case_Est_Vide(Case caseCourante)
{
    // Operateur ternaire, s'il n'y ni colonie ni unites sur la case courante
    return ( !(caseCourante.colonie) && !(caseCourante.occupant) ) ? (1) : (0) ;
}


// return 1 si la Colonie est en danger, return 0 si la colonie n'est pas en danger
int case_Colonie_seul(Case caseActuelle)
{
    if (!caseActuelle.colonie){ return 0; }
    char camp_colonie = caseActuelle.colonie->camp;
    int unites_alliees = 0;             // s'il existe au moins une unite de sa camp
    int unites_adversaires = 0;         // quantite des unites de camp differents
    if (!caseActuelle.occupant){ return 1; }    // colonie est seule

    Unite *curr = caseActuelle.occupant;
    while (curr){
        if (curr->camp == camp_colonie) { ++unites_alliees; }
        else { ++unites_adversaires; }
        curr = curr->vsuiv;
    }

    if (! unites_alliees && unites_adversaires){    // pas d'unites alliees, il y a des unites adversaires
        return 1;
    }
    return 0;

}



int prix_Unite(char camp, char type)
{
    // On evite le cas avec (FRELONS, ESCADRON)
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


int force_Unite(char type)
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



int temps_Unite(char camp, char type)
{
    // On evite le cas avec (ABEILLES, NID)
    int temps = 0;
    switch (type) {
        case RUCHE: temps = (camp == ABEILLES) ? 0 : -1; break;
        case NID: temps = (camp == FRELONS) ? 0 : -1; break;
        case REINE: temps = (camp == ABEILLES) ? (TREINEA) : (TREINEF); break;
        case GUERRIERE: temps = (camp == ABEILLES) ? TGUERRIERE : -1 ; break;
        case ESCADRON: temps = (camp == ABEILLES)? TESCADRON : -1 ; break;
        case OUVRIERE: temps = (camp == ABEILLES) ? TOUVRIERE : -1; break;
        case FRELON: temps = (camp == FRELONS) ? TFRELON : -1; break;

        default: fprintf(stderr, "Unite n'existe pas, reessayez"); break;
    }
    return temps;
}


int tirage_au_hasard(int x, int y)
{
    int resultat = x + (rand() % (y - x + 1));
    return resultat;
}


int tirageDe(void)
{
    // D'apres PDF sur e-learning:
    // L'issue du combat est déterminée par tirage aléatoire d'un dé à 60 faces
    return rand() % 60 + 1; // entre 0 et 59, avec +1 on aura de 1 a 60 (inclus)
}
