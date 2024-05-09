#include "bataille.h"

/* TOUS LES COMMENTAIRES NECESSAIRES SE TROUVES DANS BATAILLE.H*/


int collision(Case caseActuelle)
{
    if (case_Est_Vide(caseActuelle)) { return 0; }

    // caseActuelle n'est pas vide, donc on peut compter
    int abeilles = 0;
    int frelons = 0;

    if (caseActuelle.colonie){
        if (caseActuelle.colonie->camp == ABEILLES) ++abeilles;
        else ++frelons;
    }

    Unite *curr_unite = caseActuelle.occupant;
    while (curr_unite) {

        if (curr_unite->camp == ABEILLES) ++abeilles;
        else ++frelons; 

        curr_unite = curr_unite->vsuiv;     // iteration
    }
    return abeilles && frelons;
}


int bataille(Unite *unite_Abeille, Unite *unite_Frelon)
{
    int puissance_A = tirageDe() * unite_Abeille->force;
    int puissance_F = tirageDe() * unite_Frelon->force;

    // si la puissance est identique
    if (puissance_A == puissance_F){
        puissance_A = tirageDe() * unite_Abeille->force;
        puissance_F = tirageDe() * unite_Frelon->force;
    }
    // (1:3600 * 1:3600) chance que deux fois la valeur des unites soit egale
    return (puissance_A > puissance_F) ? (0) : (1) ;
}


/*
// 0 - abeille gagne
// 1 - frelon  gagne
int bataille(int force_abeille, int force_frelon){
    int puissance_abeille = tirageDe() * force_abeille;
    int puissance_frelon = tirageDe() * force_frelon;

    if (puissance_abeille == puissance_frelon){
        puissance_abeille = tirageDe() * force_abeille;
        puissance_frelon = tirageDe() * force_frelon;
    }
    return (puissance_abeille > puissance_F) ? (0) : (1) ;
}
*/



Unite *recherche_unite(Case caseActuelle, char camp)
{
    Unite *curr = caseActuelle->occupant;
    while (curr) {
        if (curr->camp == camp) { return curr; }
        curr = curr->vsuiv;
    }
}



int colonie_en_danger(Case caseActuelle)
{
    if (!caseActuelle.colonie){
        return 0;
    }
    char camp_colonie = caseActuelle.colonie->camp;
    int unite_de_colonie = 0;
    int unite_ennemy = 0;

    Unite *curr = caseActuelle.occupant;
    while (curr) {
        
        if (curr->camp == camp_colonie) ++unite_de_colonie;
        else ++unite_ennemy;

        curr = curr->vsuiv;
    }
    if (unite_de_colonie && unite_ennemy) { return 0; }     // Colonie n'est pas en danger
    
    else if (!unite_ennemy) { return 0; }                   // Colonie est seul
    
    return 1;   // unite de colonie vaut 0 et unite ennemy vaut au moins 1, donc colonie est seul
}




void combat(Grille **grille)
{
    for (int i = 0; i < NB_COLONNES; ++i) {
        for (int j = 0; j < NB_LIGNES; ++j) {
            Case caseActuelle = (*grille)->plateau[i][j];
            while (collision(caseActuelle)) {
                Unite *abeille = recherche_unite(caseActuelle, ABEILLES);
                Unite *frelon = recherche_unite(caseActuelle, FRELONS);

                /* Erreur si les uniets sont NULL */
                if (!abeille || !frelon) { return; }

                (bataille(abeille, frelon)) ? detruire_Unite(&abeille) : detruire_Unite(&frelon);
            }
        }
    } 
}






void decremente(Grille **grille, UListe *colonie)
{
    if ( !(*grille) || ! (*colonie) ){
        fprintf(stderr, "Il y a une erreur avec la grille / la colonie decremente()\n");
        return;
    }
    UListe curr_col = *colonie;
    while (curr_col){

        UListe curr_unite = curr_col;
        while(curr_unite){

            if (curr_unite->production == '0') {    // l'unite qui ne produit pas
                curr_unite = curr_unite->usuiv;
                continue;
            }

            if (curr_unite->toursrestant && curr_unite->production != '0') {            // au moins 1
                if (curr_unite->type == OUVRIERE){ (*grille)->ressourcesAbeille++; }    // la recolte
                curr_unite->toursrestant--;
            }

            if (!curr_unite->toursrestant && curr_unite->production != '0'){
                
                // ouvriere meurt sans avoir donne rss pour les frelons
                if (curr_unite->type == OUVRIERE && curr_unite->production == RECOLTE){
                    detruire_Unite(&curr_unite);
                }
                else if (curr_unite->type == REINE && (curr_unite->production == RUCHE || curr_unite->production == NID) ) {
                    
                    UListe new_colonie = creation_Colonie(&curr_unite);
                    UListe colonie_de_reine = trouver_Colonie(curr_unite);

                    if (! ajoute_Colonie(&colonie_de_reine, new_colonie)){  // l'ajout nouvelle colonie avec les autres
                        fprintf(stderr, "l'ajoute d'une nouvelle colonie aux autres n'est pas passe\n");
                        new_colonie->colsuiv = NULL, new_colonie->colprec = NULL;
                    }
                    if (! supprime_Insecte_Col(&curr_unite)){
                        fprintf(stderr, "La suppression des liens precedentes de la reine n'est pas reussite\n");
                        return;
                    }

                    if (! ajoute_Unite_Case(grille, &new_colonie, new_colonie->posx, new_colonie->posy)){
                        fprintf(stderr,"Ajoute unite case n'a pas reussi\n");
                        return;
                    }
                    new_colonie->usuiv = curr_unite;
                    curr_unite->uprec = new_colonie;
                    curr_unite->usuiv = NULL;
                    curr_unite->production = '0';
                    curr_unite->toursrestant = 0;
                    curr_unite->temps = 0;
                }
                else {    // toursrestant vaut 0 et on cree l'unite

                    Unite *new_unite = creation_Unite(&curr_unite, curr_unite->production);

                    if (! ajoute_Insecte(&curr_unite, new_unite)){
                        fprintf(stderr, "L'ajoute sur la colonie n'est pas passe\n"); return;
                    }
                    if (! ajoute_Unite_Case(grille, &new_unite, new_unite->posx, new_unite->posy)){
                        fprintf(stderr, "L'ajoute sur la case n'est pas passe\n"); return;
                    }
                    curr_unite->production = '0';
                    curr_unite->temps = 0;
                    curr_unite->toursrestant = 0;
                }
            }
            curr_unite = curr_unite->usuiv;
        }
        curr_col = curr_col->colsuiv;
    }
}



void decremente_Tout(Grille **grille)
{
    if (!(*grille)){
        fprintf(stderr, "Probleme de la grille\n");
        return;
    }
    // decrementer tous les unites necessaires pour les abeilles
    if ((*grille)->abeille) { decremente(grille, &((*grille)->abeille)); }
    
    // decrementer tous les unites necessaires pour les frelons
    if ((*grille)->frelon) { decremente(grille, &((*grille)->frelon)); }
}
