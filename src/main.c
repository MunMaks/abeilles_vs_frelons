#include "graphique.h"


// pour compiler ecrivez: make
// pour executer ecrivez: ./bin/AF
// apres avoir fini, ecrivez: make uninstall


int main(int argc, char *argv[]){

    Grille *grille = initialiserGrille();

    initialisation_abeilles(&grille);
    initialisation_frelons(&grille);

    UListe *reines_liste = (UListe *)malloc(MAX_REINES * sizeof(UListe));
    int nbr_reines = 0;
    
    MLV_create_window("Fenetre MLV", "Fenetre", SIZE_X, SIZE_Y);

    affiche_cases();
    affiche_info(grille);
    affiche_ruches(grille);
    int nombre_images_abeille;
    MLV_Image** images_abeille = afficher_unites(grille->abeille, &nombre_images_abeille);

    int nombre_images_frelon;
    MLV_Image** images_frelon = afficher_unites(grille->frelon, &nombre_images_frelon);
    MLV_actualise_window();
    // Libérer les images
    liberer_images(images_abeille, nombre_images_abeille);
    liberer_images(images_frelon, nombre_images_frelon);


    int tour = tirage_au_hasard(0, 1);

    while (1) {

        /*********************************************/
        /*** CHOIX DES ACTIONS POUR CHAQUE COLONIE ***/
        /*********************************************/

        int nb_boutons = 0;
        Bouton* boutons = afficher_commandes_prod(grille,tour,&nb_boutons);

        MLV_actualise_window();
        
        int x = 0, y = 0;
        Bouton bouton_clique =  {0, 0, 0, 0, ""};   // par defaut

        do {
            MLV_wait_mouse(&x, &y);

            // a l'interieur d'un bouton
            bouton_clique = clic_sur_bouton(x, y, boutons, nb_boutons);

        } while (bouton_clique.texte[0] == '\0' || bouton_clique.x == 0 || bouton_clique.y == 0);

        MLV_clear_window( MLV_COLOR_BLACK );

        // ajouter une unite en fonction de l'action du bouton
        ajouter_unite_selon_type(grille, bouton_clique.action, tour);

        affiche_cases();
        affiche_info(grille);

        affiche_ruches(grille);

        int nombre_images_abeille;
        MLV_Image** images_abeille = afficher_unites(grille->abeille, &nombre_images_abeille);

        int nombre_images_frelon;
        MLV_Image** images_frelon = afficher_unites(grille->frelon, &nombre_images_frelon);

        
        MLV_actualise_window();
        // Liberer la memoire prise par les images
        liberer_images(images_abeille, nombre_images_abeille);
        liberer_images(images_frelon, nombre_images_frelon);


        if (boutons != NULL) {
            free(boutons);
        }


        /*******************************************/
        /*** CHOIX DES ACTIONS POUR CHAQUE UNITE ***/
        /*******************************************/

        UListe colonie = (tour) ? grille->abeille : grille->frelon;

        int nb_boutons_unite = 0;
        Bouton bouton_clique_unite = {0, 0, 0, 0, ""};

        Unite *curr_col = colonie;
        while (curr_col){
            Unite *curr = curr_col;
            while(curr) {

                if (RUCHE != curr->type && NID != curr->type){

                    // Afficher les boutons appropriés en fonction du type d'unité
                    Bouton* boutons_unite = afficher_commandes_unite(&nb_boutons_unite,curr,tour);
                    
                    MLV_actualise_window();

                    do {
                        MLV_wait_mouse(&x, &y);

                        // Verification si le clic est à l'intdrieur d'un bouton
                        bouton_clique_unite = clic_sur_bouton(x, y, boutons_unite, nb_boutons_unite);
                        

                    } while (bouton_clique_unite.texte[0] == '\0' || bouton_clique_unite.x == 0 || bouton_clique_unite.y == 0);

                    // Appeler la fonction actions_boutons avec l'unite actuelle et le bouton clique
                    actions_boutons(grille, curr, bouton_clique_unite.action);

                    if (NID == bouton_clique_unite.action || RUCHE == bouton_clique_unite.action ){
                        Unite *reine_tmp = trouver_Reine(colonie);
                        ajouter_colonie_selon_type(grille, reine_tmp, reines_liste, &nbr_reines, bouton_clique_unite.action);
                    }

                    MLV_clear_window(MLV_COLOR_BLACK);
            
                    affiche_cases();
                    affiche_info(grille);

                    affiche_ruches(grille);

                    int nombre_images_abeille;
                    MLV_Image** images_abeille = afficher_unites(grille->abeille, &nombre_images_abeille);

                    int nombre_images_frelon;
                    MLV_Image** images_frelon = afficher_unites(grille->frelon, &nombre_images_frelon);
  
                    MLV_actualise_window();
                    // Liberer la memoire prise par les images
                    liberer_images(images_abeille, nombre_images_abeille);
                    liberer_images(images_frelon, nombre_images_frelon);

                    
                    if (boutons_unite != NULL) {
                        free(boutons_unite);
                    }
                }

                curr = curr->usuiv;
            }

            curr_col = curr_col->colsuiv;
        }
        tour = 1 - tour;

        if (tour){

			// Le combat a la fin de tour
            grille->tour ++;
            decremente_Tout(&grille);
        }

    }
    
    MLV_actualise_window();

    MLV_wait_seconds(10);

    MLV_free_window();

    free(reines_liste);     // liste des reines qui ont deja construit la colonie

    liberer_Grille(&grille);

    return 0;
}

