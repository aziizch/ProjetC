#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Acteur {
    char nom[50];
    char role[50];
    struct Acteur* suivant;
} Acteur;

typedef struct Piece {
    char titre[100];
    char auteur[50];
    char genre[50];
    struct Piece* suivant;
    Acteur* acteurs;
} Piece;

typedef struct Representation {
    Piece* piece;
    char date[20];
    char salle[50];
    struct Representation* suivant;
} Representation;

typedef struct File {
    Representation* debut;
    Representation* fin;
} File;

void init_file(File* file) {
    file->debut = file->fin = NULL;
}

void ajouter_piece(Piece** liste_pieces, const char* titre, const char* auteur, const char* genre) {
    Piece* nouvelle_piece = (Piece*)malloc(sizeof(Piece));
    strcpy(nouvelle_piece->titre, titre);
    strcpy(nouvelle_piece->auteur, auteur);
    strcpy(nouvelle_piece->genre, genre);
    nouvelle_piece->acteurs = NULL;
    nouvelle_piece->suivant = *liste_pieces;
    *liste_pieces = nouvelle_piece;
}

void ajouter_acteur(Piece* piece, const char* nom, const char* role) {
    if (piece == NULL) return;
    Acteur* nouvel_acteur = (Acteur*)malloc(sizeof(Acteur));
    strcpy(nouvel_acteur->nom, nom);
    strcpy(nouvel_acteur->role, role);
    nouvel_acteur->suivant = piece->acteurs;
    piece->acteurs = nouvel_acteur;
}

void ajouter_representation(File* file, Piece* piece, const char* date, const char* salle) {
    Representation* nouvelle_representation = (Representation*)malloc(sizeof(Representation));
    nouvelle_representation->piece = piece;
    strcpy(nouvelle_representation->date, date);
    strcpy(nouvelle_representation->salle, salle);
    nouvelle_representation->suivant = NULL;
    if (file->fin == NULL) {
        file->debut = file->fin = nouvelle_representation;
    } else {
        file->fin->suivant = nouvelle_representation;
        file->fin = nouvelle_representation;
    }
}

void afficher_pieces(Piece* liste_pieces) {
    Piece* current = liste_pieces;
    while (current != NULL) {
        printf("Titre: %s, Auteur: %s, Genre: %s\n", current->titre, current->auteur, current->genre);
        Acteur* acteur = current->acteurs;
        while (acteur != NULL) {
            printf("  - Acteur: %s, Role: %s\n", acteur->nom, acteur->role);
            acteur = acteur->suivant;
        }
        current = current->suivant;
    }
}

void afficher_representations(File* file) {
    Representation* current = file->debut;
    while (current != NULL) {
        printf("Représentation de la pièce '%s' le %s à %s\n", current->piece->titre, current->date, current->salle);
        current = current->suivant;
    }
}

void supprimer_acteur(Piece* piece, const char* nom) {
    Acteur *current = piece->acteurs, *prev = NULL;
    while (current != NULL && strcmp(current->nom, nom) != 0) {
        prev = current;
        current = current->suivant;
    }
    if (current != NULL) {
        if (prev == NULL) {
            piece->acteurs = current->suivant;
        } else {
            prev->suivant = current->suivant;
        }
        free(current);
        printf("Acteur '%s' supprimé.\n", nom);
    } else {
        printf("Acteur '%s' non trouvé.\n", nom);
    }
}

void supprimer_piece(Piece** liste_pieces, const char* titre) {
    Piece *current = *liste_pieces, *prev = NULL;
    while (current != NULL && strcmp(current->titre, titre) != 0) {
        prev = current;
        current = current->suivant;
    }
    if (current != NULL) {
        if (prev == NULL) {
            *liste_pieces = current->suivant;
        } else {
            prev->suivant = current->suivant;
        }
        Acteur* acteur = current->acteurs;
        while (acteur != NULL) {
            Acteur* temp = acteur;
            acteur = acteur->suivant;
            free(temp);
        }
        free(current);
        printf("Pièce '%s' supprimée.\n", titre);
    } else {
        printf("Pièce '%s' non trouvée.\n", titre);
    }
}
void supprimer_representation(File* file, const char* titre) {
    Representation *current = file->debut, *prev = NULL;
    while (current != NULL && strcmp(current->piece->titre, titre) != 0) {
        prev = current;
        current = current->suivant;
    }
    if (current != NULL) {
        if (prev == NULL) {
            file->debut = current->suivant;
        } else {
            prev->suivant = current->suivant;
        }
        free(current);
        printf("Représentation de la pièce '%s' supprimée.\n", titre);
    } else {
        printf("Représentation de la pièce '%s' non trouvée.\n", titre);
    }
}

void charger_fichier(Piece** liste_pieces, File* file) {
    FILE* fichier = fopen("C:\\Users\\21628\\Documents\\charger.txt", "r");
    if (fichier == NULL) {
        printf("Erreur de lecture du fichier.\n");
        return;
    }
    char titre[100], auteur[50], genre[50], acteur_role[100], date[20], salle[50];
    while (fgets(titre, sizeof(titre), fichier) != NULL) {
        titre[strcspn(titre, "\n")] = 0;
        fgets(auteur, sizeof(auteur), fichier);
        auteur[strcspn(auteur, "\n")] = 0;
        fgets(genre, sizeof(genre), fichier);
        genre[strcspn(genre, "\n")] = 0;
        ajouter_piece(liste_pieces, titre, auteur, genre);
        Piece* piece = *liste_pieces;
        while (piece != NULL && strcmp(piece->titre, titre) != 0) {
            piece = piece->suivant;
        }
        while (fgets(acteur_role, sizeof(acteur_role), fichier) != NULL && acteur_role[0] != '\n') {
            acteur_role[strcspn(acteur_role, "\n")] = 0;
            char* acteur = strtok(acteur_role, ",");
            char* role = strtok(NULL, ",");
            if (acteur != NULL && role != NULL) {
                while (*acteur == ' ') acteur++;
                while (*role == ' ') role++;
                ajouter_acteur(piece, acteur, role);
            }
        }
        while (fgets(date, sizeof(date), fichier) != NULL && date[0] != '\n') {
            date[strcspn(date, "\n")] = 0;
            fgets(salle, sizeof(salle), fichier);
            salle[strcspn(salle, "\n")] = 0;
            ajouter_representation(file, piece, date, salle);
        }
    }
    fclose(fichier);
}
void sauvegarder_fichier(Piece* liste_pieces, File* file) {
    FILE* fichier = fopen("C:\\Users\\21628\\Documents\\sauvegarder.txt", "w");
    if (fichier == NULL) {
        printf("Erreur : Impossible d'ouvrir le fichier pour la sauvegarde.\n");
        return;
    }
    Piece* piece = liste_pieces;
    while (piece != NULL) {
        fprintf(fichier, "Titre:%s, Auteur:%s, Genre:%s\n", piece->titre, piece->auteur, piece->genre);
        Acteur* acteur = piece->acteurs;
        while (acteur != NULL) {
            fprintf(fichier, "Nom:%s, role:%s\n", acteur->nom, acteur->role);
            acteur = acteur->suivant;
        }
        Representation* representation = file->debut;
        while (representation != NULL) {
            if (representation->piece == piece) {
                fprintf(fichier, " date:%s, salle:%s\n", representation->date, representation->salle);
            }
            representation = representation->suivant;
        }
        piece = piece->suivant;
    }
    fclose(fichier);
    printf("Données sauvegardées avec succès dans 'sauvegarder.txt'.\n");
}
int main() {
    Piece* liste_pieces = NULL;
    File file_representations;
    init_file(&file_representations);
    int choix;
    do {
        printf("\n----------bienvenue dans notre theatre----------:\n");
        printf("1. Ajouter une pièce\n");
        printf("2. Ajouter un acteur à une pièce\n");
        printf("3. Ajouter une représentation\n");
        printf("4. Afficher toutes les pièces\n");
        printf("5. Afficher toutes les représentations\n");
        printf("6. Supprimer une pièce\n");
        printf("7. Supprimer un acteur\n");
        printf("8. Supprimer une représentation\n");
        printf("9. Sauvegarder les données\n");
        printf("10. Charger les données depuis le fichier\n");
        printf("11. Quitter\n");
        printf("Entrez votre choix: ");
        scanf("%d", &choix);
        getchar();
        switch (choix) {
            case 1: {
                char titre[100], auteur[50], genre[50];
                printf("Entrez le titre de la pièce: ");
                fgets(titre, sizeof(titre), stdin);
                titre[strcspn(titre, "\n")] = 0;
                printf("Entrez l'auteur de la pièce: ");
                fgets(auteur, sizeof(auteur), stdin);
                auteur[strcspn(auteur, "\n")] = 0;
                printf("Entrez le genre de la pièce: ");
                fgets(genre, sizeof(genre), stdin);
                genre[strcspn(genre, "\n")] = 0;

                ajouter_piece(&liste_pieces, titre, auteur, genre);
                break;
            }
            case 2: {
                char nom[50], role[50], titre[100];
                printf("Entrez le titre de la pièce à laquelle ajouter un acteur: ");
                fgets(titre, sizeof(titre), stdin);
                titre[strcspn(titre, "\n")] = 0;

                Piece* piece = liste_pieces;
                while (piece != NULL && strcmp(piece->titre, titre) != 0) {
                    piece = piece->suivant;
                }
                if (piece != NULL) {
                    printf("Entrez le nom de l'acteur: ");
                    fgets(nom, sizeof(nom), stdin);
                    nom[strcspn(nom, "\n")] = 0;
                    printf("Entrez le role de l'acteur: ");
                    fgets(role, sizeof(role), stdin);
                    role[strcspn(role, "\n")] = 0;

                    ajouter_acteur(piece, nom, role);
                } else {
                    printf("Pièce non trouvée.\n");
                }
                break;
            }
            case 3: {
                char titre[100], date[20], salle[50];
                printf("Entrez le titre de la pièce pour la représentation: ");
                fgets(titre, sizeof(titre), stdin);
                titre[strcspn(titre, "\n")] = 0;

                Piece* piece = liste_pieces;
                while (piece != NULL && strcmp(piece->titre, titre) != 0) {
                    piece = piece->suivant;
                }
                if (piece != NULL) {
                    printf("Entrez la date de la représentation (format: JJ/MM/AAAA): ");
                    fgets(date, sizeof(date), stdin);
                    date[strcspn(date, "\n")] = 0;
                    printf("Entrez le nom de la salle: ");
                    fgets(salle, sizeof(salle), stdin);
                    salle[strcspn(salle, "\n")] = 0;
                    ajouter_representation(&file_representations, piece, date, salle);
                } else {
                    printf("Piece non trouvée.\n");
                }
                break;
            }
            case 4:
                afficher_pieces(liste_pieces);
                break;
            case 5:
                afficher_representations(&file_representations);
                break;
            case 6: {
                char titre[100];
                printf("Entrez le titre de la pièce à supprimer: ");
                fgets(titre, sizeof(titre), stdin);
                titre[strcspn(titre, "\n")] = 0;
                supprimer_piece(&liste_pieces, titre);
                break;
            }
            case 7: {
                char titre[100], nom[50];
                printf("Entrez le titre de la pièce: ");
                fgets(titre, sizeof(titre), stdin);
                titre[strcspn(titre, "\n")] = 0;
                Piece* piece = liste_pieces;
                while (piece != NULL && strcmp(piece->titre, titre) != 0) {
                    piece = piece->suivant;
                }
                if (piece != NULL) {
                    printf("Entrez le nom de l'acteur à supprimer: ");
                    fgets(nom, sizeof(nom), stdin);
                    nom[strcspn(nom, "\n")] = 0;
                    supprimer_acteur(piece, nom);
                } else {
                    printf("Piece non trouvée.\n");
                }
                break;
            }
            case 8: {
                char titre[100];
                printf("Entrez le titre de la piece pour la representation à supprimer: ");
                fgets(titre, sizeof(titre), stdin);
                titre[strcspn(titre, "\n")] = 0;
                supprimer_representation(&file_representations, titre);
                break;
            }
            case 9:
                sauvegarder_fichier(liste_pieces, &file_representations);
                break;
            case 10:
                charger_fichier(&liste_pieces, &file_representations);
                break;
            case 11:
                printf("Merci d'avoir utilise le programme.\n");
                break;
            default:
                printf("Choix invalide. Essayez à nouveau.\n");
        }
    } while (choix != 11);
    return 0;
}
