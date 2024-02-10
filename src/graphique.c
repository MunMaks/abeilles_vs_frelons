#include "graphique.h"

/* TOUS LES COMMENTAIRES NECESSAIRES SE TROUVES DANS GRAPHIQUE.H*/


/***********************************************/
/***************** GRAPHIQUE *******************/
/***********************************************/


void afficher_infos_unite_en_attente(Unite *unite)
{
    while (unite != NULL) {
        if (unite->toursrestant != 0 && (unite->production != '0' && unite->production != RECOLTE && unite->production != NID && unite->production != RUCHE)) {
            char message[100];
            sprintf(message, "En attente: %c - Tours restants: %d", unite->production, unite->toursrestant);
            MLV_draw_text_box(SIZE_X_BOARD +10, 150, 270, 320, message, 5, MLV_COLOR_BLACK, MLV_COLOR_RED, MLV_COLOR_BLACK,
            MLV_TEXT_LEFT, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);
        }

        unite = unite->usuiv;
    }
}



void afficher_infos_colonie_en_attente(UListe colonie)
{
    UListe curr_col = colonie;
    while (curr_col != NULL) {
        afficher_infos_unite_en_attente(curr_col);
        curr_col = curr_col->colsuiv;
    }
}



void affiche_cases(void)
{
    for (int ligne = 0; ligne < NB_LIGNES; ++ligne) {
        for (int colonne = 0; colonne < NB_COLONNES; ++colonne) {
            int x = colonne * LARGEUR_CASE;
            int y = ligne * LARGEUR_CASE;

            MLV_draw_filled_rectangle(x, y, LARGEUR_CASE, LARGEUR_CASE, MLV_COLOR_WHITE);
            MLV_draw_rectangle(x, y, LARGEUR_CASE, LARGEUR_CASE, MLV_COLOR_BLACK);
        }
    }
}



void affiche_info(Grille *grille)
{

    MLV_Font* font_big = MLV_load_font( "Heavitas.ttf" , 40 );
    MLV_Font* font_small = MLV_load_font( "Heavitas.ttf" , 15 );

    char text_abeille[100];
    char text_frelon[100];
    char text_tour[20];

    sprintf(text_abeille, "Abeilles :\n Pollen : %d", grille->ressourcesAbeille);
    sprintf(text_frelon, "Frelons :\n Ressources : %d", grille->ressourcesFrelon);

    sprintf(text_tour, "Tour : %d", grille->tour);

    MLV_draw_adapted_text_box_with_font(120, SIZE_Y - 150, text_abeille, font_big, 10, MLV_COLOR_BLACK, MLV_COLOR_YELLOW2, MLV_COLOR_BLACK, MLV_TEXT_CENTER);
    MLV_draw_adapted_text_box_with_font(SIZE_X_BOARD / 2 + 70, SIZE_Y - 150, text_frelon,font_big, 10, MLV_COLOR_BLACK, MLV_COLOR_ORANGE1, MLV_COLOR_BLACK, MLV_TEXT_CENTER);
    
    MLV_draw_rectangle(0, SIZE_Y_BOARD, SIZE_X_BOARD / 2, 200, MLV_COLOR_WHITE);
    MLV_draw_rectangle(SIZE_X_BOARD / 2, SIZE_Y_BOARD, SIZE_X_BOARD / 2, 200, MLV_COLOR_WHITE);
    MLV_draw_rectangle(SIZE_X_BOARD, 0, 300, SIZE_Y_BOARD, MLV_COLOR_WHITE);

    MLV_draw_text_with_font(SIZE_X_BOARD + 60, SIZE_Y_BOARD + 75,text_tour,font_big,MLV_COLOR_WHITE);

    MLV_free_font( font_small );
    MLV_free_font( font_big );
}



void affiche_ruches(Grille *grille)
{

    MLV_Image *image = NULL;


    UListe listeRuches = grille->abeille;
    while (listeRuches) {
        if (RUCHE == listeRuches->type) {
            int posX = listeRuches->posx;
            int posY = listeRuches->posy;


            int xFenetre = posX * LARGEUR_CASE + (LARGEUR_CASE - 20) / 2;
            int yFenetre = posY * LARGEUR_CASE + (LARGEUR_CASE - 20) / 2;

            image = MLV_load_image( "images/Ruche.png" );
            MLV_resize_image_with_proportions( image, 20, 20);
            MLV_draw_image( image, xFenetre, yFenetre);
            
        }

        listeRuches = listeRuches->colsuiv;
    }

    UListe listeNid = grille->frelon;
    while (listeNid) {
        if (NID == listeNid->type) {
            int posX = listeNid->posx;
            int posY = listeNid->posy;

            int xFenetre = posX * LARGEUR_CASE + (LARGEUR_CASE - 20) / 2;
            int yFenetre = posY * LARGEUR_CASE + (LARGEUR_CASE - 20) / 2;

            image = MLV_load_image( "images/Nid.png" );
            MLV_resize_image_with_proportions( image, 20, 20);
            MLV_draw_image( image, xFenetre, yFenetre);
           
        }

        listeNid = listeNid->colsuiv;
    }
}



MLV_Image** afficher_unites(UListe colonie,int *nombre_images)
{
    if (!colonie) {
        fprintf(stderr, "Colonne est vide\n");
        return NULL;
    }

    int nombre_images_total = 0; // Pour compter le nombre d'images creees
    MLV_Image **images = NULL;

    Unite *curr_col = colonie;
    while (curr_col) {
        Unite *curr = curr_col;
        while (curr) {
            if (!curr) {
                break;
            }

            int posX = curr->posx;
            int posY = curr->posy;

            int xFenetre = 0, yFenetre = 0;

            // Verifier si le type n'est ni 'R' ni 'N' (ruches ni nids)
            if (RUCHE != curr->type && NID != curr->type) {
                MLV_Image *image = NULL;    // Nouvelle image pour chaque unite

                // Déterminer la position specifique en fonction du type d'unite
                if (OUVRIERE == curr->type) {
                    xFenetre = posX * LARGEUR_CASE;
                    yFenetre = posY * LARGEUR_CASE + 20;
                    image = MLV_load_image("images/Ouvrier.png");
                    MLV_resize_image_with_proportions(image, 20, 20);
                } else if (REINE == curr->type && ABEILLES == curr->camp) {
                    xFenetre = posX * LARGEUR_CASE - 10;
                    yFenetre = posY * LARGEUR_CASE - 10;
                    image = MLV_load_image("images/Reine_abeille.png");
                    MLV_resize_image_with_proportions(image, 40, 40);
                } else if (REINE == curr->type && FRELONS == curr->camp) {
                    xFenetre = posX * LARGEUR_CASE + LARGEUR_CASE / 2.5;
                    yFenetre = posY * LARGEUR_CASE - 10;
                    image = MLV_load_image("images/Reine_frelon.png");
                    MLV_resize_image_with_proportions(image, 40, 40);
                } else if (ESCADRON == curr->type) {
                    xFenetre = posX * LARGEUR_CASE + LARGEUR_CASE - 33;
                    yFenetre = posY * LARGEUR_CASE - 10;
                    image = MLV_load_image("images/Escadron.png");
                    MLV_resize_image_with_proportions(image, 40, 40);
                } else if (FRELON == curr->type) {
                    xFenetre = posX * LARGEUR_CASE + LARGEUR_CASE - 20;
                    yFenetre = posY * LARGEUR_CASE + LARGEUR_CASE - 40;
                    image = MLV_load_image("images/Frelon.png");
                    MLV_resize_image_with_proportions(image, 20, 20);
                } else if (GUERRIERE == curr->type) {
                    xFenetre = posX * LARGEUR_CASE - 10;
                    yFenetre = posY * LARGEUR_CASE + LARGEUR_CASE - 30;
                    image = MLV_load_image("images/Guerrier.png");
                    MLV_resize_image_with_proportions(image, 40, 40);
                } else {
                    printf("Type d'unité inconnu : %c\n", curr->type);
                    xFenetre = posX * LARGEUR_CASE;
                    yFenetre = posY * LARGEUR_CASE + LARGEUR_CASE;
                    image = MLV_load_image("images/Autre.png");
                    MLV_resize_image_with_proportions(image, 40, 40);
                }

                // Ajoute l'image a la liste
                images = realloc(images, (nombre_images_total + 1) * sizeof(MLV_Image *));
                images[nombre_images_total++] = image;

                // Affiche l'image de l'unite
                MLV_draw_image(image, xFenetre, yFenetre);
            }
            curr = curr->usuiv;
        }
        curr_col = curr_col->colsuiv;
    }
    *nombre_images = nombre_images_total;
    return images;
}



void liberer_images(MLV_Image** images, int nombre_images)
{
    for (int i = 0; i < nombre_images; ++i) {
        MLV_free_image(images[i]);
    }
    free(images);
}



void afficher_bouton(Bouton bouton)
{
    MLV_draw_filled_rectangle(bouton.x, bouton.y, bouton.largeur, bouton.hauteur, MLV_COLOR_WHITE);
    MLV_draw_text(bouton.x + 10, bouton.y + 10, bouton.texte, MLV_COLOR_BLACK);
}



Bouton clic_sur_bouton(int x, int y, Bouton* boutons, int nb_boutons)
{
    for (int i = 0; i < nb_boutons; i++) {
        if (x >= boutons[i].x && x <= boutons[i].x + boutons[i].largeur &&
            y >= boutons[i].y && y <= boutons[i].y + boutons[i].hauteur) {
            return boutons[i];
        }
    }
    // Retourner un bouton "vide" si aucun bouton n'est clique
    Bouton bouton_vide = {0, 0, 0, 0, ""};
    return bouton_vide;
}



Bouton* afficher_commandes_prod(Grille *grille, int joueur, int* nb_boutons)
{
    MLV_Font* font_small = MLV_load_font( "Heavitas.ttf" , 15 );
    Bouton* boutons = NULL;
    char* text = NULL;
    MLV_Color txt_color = MLV_COLOR_WHITE;
    UListe colonie = NULL;
    

    if (joueur == 1) {
        *nb_boutons = 5;
        boutons = (Bouton*)malloc(*nb_boutons * sizeof(Bouton));

        boutons[0] = (Bouton){SIZE_X_BOARD + 20, 100, 250, 50, "Passer", 'p'};
        boutons[1] = (Bouton){SIZE_X_BOARD + 20, 175, 250, 50, "Ouvrière  Coût : 3  Temps : 2", OUVRIERE};
        boutons[2] = (Bouton){SIZE_X_BOARD + 20, 250, 250, 50, "Guerrière  Coût : 5  Temps : 4", GUERRIERE};
        boutons[3] = (Bouton){SIZE_X_BOARD + 20, 325, 250, 50, "Escadron  Coût : 6  Temps : 6", ESCADRON};
        boutons[4] = (Bouton){SIZE_X_BOARD + 20, 400, 250, 50, "Reine Coût : 7  Temps : 8", REINE};

        text = "Actions : Ruches Abeilles";
        txt_color = MLV_COLOR_YELLOW1;
        colonie = grille->abeille;
    } else {
        *nb_boutons = 3;
        boutons = (Bouton*)malloc(*nb_boutons * sizeof(Bouton));

        boutons[0] = (Bouton){SIZE_X_BOARD + 20, 100, 250, 50, "Passer", 'p'};
        boutons[1] = (Bouton){SIZE_X_BOARD + 20, 175, 250, 50, "Frelon  Coût : 3  Temps : 5", FRELON};
        boutons[2] = (Bouton){SIZE_X_BOARD + 20, 250, 250, 50, "Reine Coût : 8  Temps : 8", REINE};

        text = "Actions : Nids Frelons";
        txt_color = MLV_COLOR_ORANGE2;
        colonie = grille->frelon; 
    }

    for (int i = 0; i < *nb_boutons; i++) {
        afficher_bouton(boutons[i]);
    }

    MLV_draw_text_with_font(SIZE_X_BOARD + 40, 10, text,font_small, txt_color);
    afficher_infos_colonie_en_attente(colonie);

    return boutons;
}



void deplacer_unite_selon_clic(Grille *grille, Unite *unite)
{
    int ligne = 0;
    int colonne = 0;

    do {
        int x = 0, y = 0;
        MLV_wait_mouse(&x, &y);

        ligne = y / LARGEUR_CASE;
        colonne = x / LARGEUR_CASE;

        // Verifier si les indices de case sont valides
        if (ligne >= 0 && ligne < NB_LIGNES && colonne >= 0 && colonne < NB_COLONNES) {
            int diff_ligne = abs(ligne - unite->posy);
            int diff_colonne = abs(colonne - unite->posx);


            if ((diff_ligne >= 0 && diff_ligne <= 1) && (diff_colonne >= 0 && diff_colonne <= 1)) {

                if (ajoute_Unite_Case(&grille, &unite, colonne, ligne)) {
                    MLV_actualise_window();
                }
            } 
        } 
    } while (!((ligne >= 0 && ligne < NB_LIGNES && colonne >= 0 && colonne < NB_COLONNES)
        && (abs(ligne - unite->posy) >= 0 && abs(ligne - unite->posy) <= 1)
        && (abs(colonne - unite->posx) >= 0 && abs(colonne - unite->posx) <= 1)));
}



Bouton* afficher_commandes_unite(int* nb_boutons, Unite* curr, int tour)
{
    MLV_Font* font_small = MLV_load_font( "Heavitas.ttf" , 15 );
    int x = curr->posx;
    int y = curr->posy;
    char position_texte[50];
    sprintf(position_texte, "Position : (%d, %d)", x, y);
    char* text = NULL;

    switch (curr->type) {
        case REINE:
            *nb_boutons = 3;
            text = "Actions : REINE";
            break;
        case GUERRIERE:
            *nb_boutons = 2;
            text = "Actions : Guerrier";
            break;
        case ESCADRON:
            *nb_boutons = 2;
            text = "Actions : Escadron";
            break;
        case FRELON:
            *nb_boutons = 2;
            text = "Actions : Frelon";
            break;
        case OUVRIERE:
            *nb_boutons = 2;
            text = "Actions : Ouvrière";
            break;
        default:
            *nb_boutons = 0;
            text = "Actions : Inconnu";
    }

    Bouton* boutons = NULL;
    if (REINE == curr->type) {
        *nb_boutons = 3;
        boutons = (Bouton*)malloc(*nb_boutons * sizeof(Bouton));

        boutons[0] = (Bouton){SIZE_X_BOARD + 20, 100, 250, 50, "Passer", 'p'};
        boutons[1] = (Bouton){SIZE_X_BOARD + 20, 175, 250, 50, "Se déplacer", 'd'};
        boutons[2] = (Bouton){SIZE_X_BOARD + 20, 250, 250, 50, "Founder Colonie, temps : 1 tour", 
        ((curr->camp == ABEILLES) ? RUCHE : NID)};
    }
    else if (GUERRIERE == curr->type || ESCADRON == curr->type || FRELON == curr->type) {
        *nb_boutons = 2;
        boutons = (Bouton*)malloc(*nb_boutons * sizeof(Bouton));

        boutons[0] = (Bouton){SIZE_X_BOARD + 20, 100, 250, 50, "Passer", 'p'};
        boutons[1] = (Bouton){SIZE_X_BOARD + 20, 175, 250, 50, "Se déplacer", 'd'};
    }
    else if (curr->type == OUVRIERE) {
        *nb_boutons = 3;
        boutons = (Bouton*)malloc(*nb_boutons * sizeof(Bouton));

        boutons[0] = (Bouton){SIZE_X_BOARD + 20, 100, 250, 50, "Passer", 'p'};
        boutons[1] = (Bouton){SIZE_X_BOARD + 20, 175, 250, 50, "Se déplacer", 'd'};
        boutons[2] = (Bouton){SIZE_X_BOARD + 20, 250, 250, 50, "Récolter du pollen temps : 4 tours", RECOLTE};

    }

    MLV_Color text_color = (!tour) ? MLV_COLOR_ORANGE : MLV_COLOR_YELLOW;

    MLV_draw_text_with_font(SIZE_X_BOARD + 80, 10, text, font_small, text_color);
    MLV_draw_text_with_font(SIZE_X_BOARD + 80, 50, position_texte,font_small, text_color);


    for (int i = 0; i < (*nb_boutons); ++i) {
        afficher_bouton(boutons[i]);
    }
    
    // Dessiner un carre rouge sur la case de l'unite
    int xFenetre = curr->posx * LARGEUR_CASE;
    int yFenetre = curr->posy * LARGEUR_CASE;
    MLV_draw_rectangle(xFenetre, yFenetre, 60, 60, MLV_COLOR_RED);

    // Si l'unite est en train de recolter, affiche un message specifique
    if (curr->production == RECOLTE && curr->toursrestant > 0) {
        char message[100];
        sprintf(message, "En train de récolter du pollen \n Tours restants : %d", curr->toursrestant);
        MLV_draw_text_box(SIZE_X_BOARD +10, 150, 270, 320, message, 5, MLV_COLOR_BLACK, MLV_COLOR_RED, MLV_COLOR_BLACK,
        MLV_TEXT_LEFT, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);
    }

    // Si l'unite est en train de creer une colonie
    if ((curr->production == RUCHE || curr->production == NID)  && curr->toursrestant > 0) {
        char message[100];
        sprintf(message, "En train de créer une colonie\n Tours restants : %d", curr->toursrestant);
        MLV_draw_text_box(SIZE_X_BOARD +10, 150, 270, 320, message, 5, MLV_COLOR_BLACK, MLV_COLOR_RED, MLV_COLOR_BLACK,
        MLV_TEXT_LEFT, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);
    }

    MLV_actualise_window();
    return boutons;
}



void afficher_cases_deplacement(Unite* unite)
{
    int x = unite->posx;
    int y = unite->posy;

    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) {
            // Verifier si la case est à l'intérieur du plateau
            if (x + i >= 0 && x + i < NB_COLONNES && y + j >= 0 && y + j < NB_LIGNES) {
                // Eviter de dessiner sur la case actuelle de l'unité
                if (!(i == 0 && j == 0)) {
                    MLV_draw_rectangle((x + i) * LARGEUR_CASE, (y + j) * LARGEUR_CASE, LARGEUR_CASE, LARGEUR_CASE , MLV_COLOR_GREEN);
                    MLV_draw_rectangle((x + i) * LARGEUR_CASE, (y + j) * LARGEUR_CASE , LARGEUR_CASE - 1, LARGEUR_CASE - 1, MLV_COLOR_GREEN);
                    MLV_draw_rectangle((x + i) * LARGEUR_CASE, (y + j) * LARGEUR_CASE, LARGEUR_CASE - 2, LARGEUR_CASE - 2, MLV_COLOR_GREEN);
                }
            }
        }
    }

    MLV_actualise_window();
}



void actions_boutons(Grille *grille, Unite *unite, char action)
{

    switch (action) {
        case 'd':
            afficher_cases_deplacement(unite);
            deplacer_unite_selon_clic(grille, unite);
            break;

        case RECOLTE:
            start_recolte_Ouvrierre(&unite); break;

        // On peut ajouter le bouton pour finir la partie
        
        // On peut ajouter le bouton pour sauvgarder la partie

        // On peut ajouter le bouton pour charger la partie

        case 'p':
            break;
    }
}
