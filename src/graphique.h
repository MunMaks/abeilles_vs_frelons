#ifndef __GRAPHIQUE__
#define __GRAPHIQUE__

#include "bataille.h"


// structure pour les boutons
typedef struct {
    int x, y, largeur, hauteur;
    char texte[50];
    char action;
} Bouton;



/***********************************************/
/***************** GRAPHIQUE *******************/
/***********************************************/


/**
 * @brief Affiche l unite en attente et le nombre de tours restant avant sa création par la ruche.
 * 
 * @param unite 
 */
void afficher_infos_unite_en_attente(Unite *unite);


/**
 * @brief Affiche la ruche ou nid en attente et le nombre de tours restant avant sa creation par la reine.
 * 
 * @param colonie 
 */
void afficher_infos_colonie_en_attente(UListe colonie);


/**
 * @brief Affiche la grille de 18x12 cases
 * 
 */
void affiche_cases(void);


/**
 * @brief Affiche les ressources des deux camps ainsi que quelques elements esthétique pour les bords de la fenetre
 * 
 * @param grille 
 */
void affiche_info(Grille *grille);



/**
 * @brief Affiche une image sur les cases du plateau qui contiennent une ruche ou un Nid
 * 
 * @param grille 
 */
void affiche_ruches(Grille *grille);


/**
 * @brief Affiche l’image correspondante pour toutes les unités de la colonie passé en paramètre
 * 
 * @param colonie 
 * @param nombre_images 
 * @return MLV_Image** 
 */
MLV_Image** afficher_unites(UListe colonie,int *nombre_images);


/**
 * @brief liberer en memoire toutes les images MLV
 * 
 * @param images 
 * @param nombre_images 
 */
void liberer_images(MLV_Image** images, int nombre_images);


/**
 * @brief Affiche un rectangle et le texte correspondant aux paramètres de bouton
 * 
 * @param bouton 
 */
void afficher_bouton(Bouton bouton);


/**
 * @brief verifie si le clic de l’utilisateur se trouve sur un des bouton de la liste Bouton* bouton
 * 
 * @param x 
 * @param y 
 * @param boutons 
 * @param nb_boutons 
 * @return Bouton 
 */
Bouton clic_sur_bouton(int x, int y, Bouton* boutons, int nb_boutons);


/**
 * @brief affiche et renvoie la liste des boutons lies au actions de la ruche ou du nid en fonction du tour
 * 
 * @param grille 
 * @param joueur 
 * @param nb_boutons 
 * @return Bouton* 
 */
Bouton* afficher_commandes_prod(Grille *grille, int joueur, int* nb_boutons);


/**
 * @brief Deplace l’unité si l'utilisateur clique sur une case adjacente.
 * 
 * @param grille 
 * @param unite 
 */
void deplacer_unite_selon_clic(Grille *grille, Unite *unite);


/**
 * @brief affiche et renvoie la liste des boutons liés au actions de l unite, affiche également un carre rouge sur la case de l unite en cours.
 * 
 * @param nb_boutons 
 * @param curr 
 * @param tour 
 * @return Bouton* 
 */
Bouton* afficher_commandes_unite(int* nb_boutons, Unite* curr, int tour);


/**
 * @brief affiche un carre vert sur toutes les cases sur lesquels l’unité peut se déplacer
 * 
 * @param unite 
 */
void afficher_cases_deplacement(Unite* unite);


/**
 * @brief Effectue les fonctions liees à l’action choisie pour les Unites
 * 
 * @param grille 
 * @param unite 
 * @param action 
 */
void actions_boutons(Grille *grille, Unite *unite, char action);


#endif
