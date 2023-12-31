#include "frelon.h"
// les fonctions pour les frelons
Unite* creation_Frelon(UListe ruche, char type, int force, int temps);


Unite *creation_Nid(Unite *reineFrelon) {
    // Allocate memory for a new nid
    Unite *new_NID = (Unite *)malloc(sizeof(Unite));
    if (new_NID == NULL) {
        fprintf(stderr, "Malloc n'a pas reussi de creer un nouveau nid, memoire...\n");
        return NULL;
    }

    // Initialize the attributes of the new nid
    new_NID->camp = FRELONS;
    new_NID->type = NID;
    new_NID->force = 0;
    new_NID->posx = reineFrelon->posx;
    new_NID->posy = reineFrelon->posy;
    new_NID->destx = 0;
    new_NID->desty = 0;
    new_NID->production = '0'; // Initial state, adjust if needed
    new_NID->temps = 1;
    new_NID->toursrestant = 1;

    // Affiliation to the provided reine frelon
    new_NID->usuiv = reineFrelon;
    new_NID->uprec = NULL;

    // Connection to other units (adjust based on your logic)
    new_NID->colsuiv = NULL;
    new_NID->colprec = NULL;
    new_NID->vsuiv = NULL;
    new_NID->vprec = NULL;

    // Ensure the previous nid is no longer linked to the provided reine frelon
    if (reineFrelon != NULL && reineFrelon->uprec != NULL) {
        reineFrelon->uprec->usuiv = NULL;
    }

    return new_NID;
}
