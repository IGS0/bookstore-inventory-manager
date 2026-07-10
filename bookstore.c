#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bookstore.h"
#include <stdbool.h>
#include "logger.h"

int main() {
    set_log_file("application.log");
    set_log_level(LOG_LEVEL_INFO);

    // Andmete laadimine failidest
    int book_count = 0, inventory_count = 0;
    Book *books = load_books(BOOKS_FILE, &book_count);
    Inventory *inventory = load_inventory(INVENTORY_FILE, &inventory_count);

    // Menüü kuvamine ja kasutaja tegevuste täitmine
    int choice;
    do {
        display_menu();
        if (scanf("%d", &choice) != 1) {
            printf("Vigane sisend. Palun sisesta number.\n");
            while (getchar() != '\n'); // Tühjenda puhver
            continue;
        }
        switch (choice) {
            case 1:
                display_books(books, book_count, inventory, inventory_count);
                break;
            case 2:
                edit_book_title(books, book_count);
                save_books(BOOKS_FILE, books, book_count);
                break;
            case 3:
                edit_inventory_stock(inventory, inventory_count);
                save_inventory(INVENTORY_FILE, inventory, inventory_count);
                break;
            case 4:
                find_potential_profits(books, book_count);
                break;
            case 0:
                printf("Programm lõpetab.\n");
                log_message(LOG_LEVEL_INFO, "Programm lõpetas");
                break;
            default:
                printf("Vigane valik. Palun vali uuesti.\n");
        }
    } while (choice != 0);

    // Vabasta dünaamiliselt reserveeritud mälu
    free_books(books, book_count);
    free_inventory(inventory);

    close_log_file();

    return 0;
}


/**
 * Kirjeldus: Laadib raamatute andmed failist ja tagastab massiivi raamatute struktuuridest.
 *
 * Parameetrid: filename - faili nimi, kust raamatuid laadida.
 *              count - viide muutujale, kuhu salvestatakse raamatute arv.
 *
 * Tagastab: Book* - viide raamatute massiivile.
 */
 Book *load_books(const char *filename, int *count) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Viga avades faili %s\n", filename);
        log_message(LOG_LEVEL_ERROR, "Viga avades raamatute faili");
        exit(1);
        //~ return NULL; // Ei välju programmist
    }
    log_message(LOG_LEVEL_INFO, "Raamatute fail avatud edukalt");

    int capacity = INITIAL_BOOK_CAPACITY;
    Book *books = malloc(sizeof(Book) * capacity);
    if (books == NULL) {
        fprintf(stderr, "Mälu reserveerimine ebaõnnestus\n");
        log_message(LOG_LEVEL_ERROR, "Mälu reserveerimine raamatute jaoks ebaõnnestus");
        fclose(file); // Lisatud faili sulgemine
        exit(1);
        //~ return NULL; // Ei välju programmist
    }

    char buffer[BUFFER_SIZE];
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        if (*count >= capacity) {
            capacity *= 2;
            Book *temp = realloc(books, sizeof(Book) * capacity);
            if (temp == NULL) {
                fprintf(stderr, "Mälu laiendamine ebaõnnestus\n");
                log_message(LOG_LEVEL_ERROR, "Mälu laiendamine raamatute jaoks ebaõnnestus");
                free(books); // Lisatud mälulekke vältimine
                fclose(file); // Lisatud faili sulgemine
                exit(1);
                //~ return NULL; // Ei välju programmist
            }
            books = temp;
        }

        char *token = strtok(buffer, ";");
        strcpy(books[*count].code, token);

        token = strtok(NULL, ";");
        books[*count].title = malloc(strlen(token) + 1); // Allocate memory for the string
        if (books[*count].title == NULL) {
            fprintf(stderr, "Mälu reserveerimine ebaõnnestus\n");
            log_message(LOG_LEVEL_ERROR, "Mälu reserveerimine raamatu pealkirja jaoks ebaõnnestus");
            free_books(books, *count); // Lisatud mälulekke vältimine
            fclose(file); // Lisatud faili sulgemine
            return NULL; // Ei välju programmist
        }
        strcpy(books[*count].title, token); // Copy the string to the allocated memory

        token = strtok(NULL, ";");
        books[*count].author = malloc(strlen(token) + 1); // Allocate memory for the string
        if (books[*count].author == NULL) {
            fprintf(stderr, "Mälu reserveerimine ebaõnnestus\n");
            log_message(LOG_LEVEL_ERROR, "Mälu reserveerimine raamatu autori jaoks ebaõnnestus");
            free_books(books, *count); // Lisatud mälulekke vältimine
            fclose(file); // Lisatud faili sulgemine
            exit(1);
            //~ return NULL; // Ei välju programmist
        }
        strcpy(books[*count].author, token); // Copy the string to the allocated memory

        token = strtok(NULL, ";");
        books[*count].year = atoi(token);

        token = strtok(NULL, ";");
        books[*count].self_cost = atof(token);

        token = strtok(NULL, ";");
        books[*count].sell_price = atof(token);

        (*count)++;
    }

    fclose(file);
    log_message(LOG_LEVEL_INFO, "Raamatud laaditud edukalt");
    return books;
}
 //~ Book *load_books(const char *filename, int *count) {
    //~ FILE *file = fopen(filename, "r");
    //~ if (file == NULL) {
        //~ fprintf(stderr, "Viga avades faili %s\n", filename);
        //~ log_message(LOG_LEVEL_ERROR, "Viga avades raamatute faili");
        //~ exit(1);
    //~ }
    //~ log_message(LOG_LEVEL_INFO, "Raamatute fail avatud edukalt");

    //~ int capacity = INITIAL_BOOK_CAPACITY;
    //~ Book *books = malloc(sizeof(Book) * capacity);
    //~ if (books == NULL) {
        //~ fprintf(stderr, "Mälu reserveerimine ebaõnnestus\n");
        //~ log_message(LOG_LEVEL_ERROR, "Mälu reserveerimine raamatute jaoks ebaõnnestus");
        //~ fclose(file);  // Close the file before exiting
        //~ exit(1);
    //~ }

    //~ char buffer[BUFFER_SIZE];
    //~ while (fgets(buffer, sizeof(buffer), file) != NULL) {
        //~ if (*count >= capacity) {
            //~ capacity *= 2;
            //~ Book *temp = realloc(books, sizeof(Book) * capacity);
            //~ if (temp == NULL) {
                //~ fprintf(stderr, "Mälu laiendamine ebaõnnestus\n");
                //~ log_message(LOG_LEVEL_ERROR, "Mälu laiendamine raamatute jaoks ebaõnnestus");
                //~ free(books); // Lisatud mälulekke vältimine
                //~ fclose(file); // Lisatud faili sulgemine
                //~ exit(1);
                //~ return NULL; // Ei välju programmist
            //~ }
            //~ books = temp;
        //~ }

        //~ char *token = strtok(buffer, ";");
        //~ if (token != NULL) {
            //~ strcpy(books[*count].code, token);
        //~ }

        //~ token = strtok(NULL, ";");
        //~ if (token != NULL) {
            //~ books[*count].title = malloc(strlen(token) + 1);
            //~ if (books[*count].title == NULL) {
                //~ fprintf(stderr, "Memory allocation failed\n");
                //~ log_message(LOG_LEVEL_ERROR, "Memory allocation for book title failed");
                //~ fclose(file);
                //~ free_books(books, *count);  // Free all previously allocated memory
                //~ exit(1);
            //~ }
            //~ strcpy(books[*count].title, token);
        //~ }

        //~ token = strtok(NULL, ";");
        //~ if (token != NULL) {
            //~ books[*count].author = malloc(strlen(token) + 1);
            //~ if (books[*count].author == NULL) {
                //~ fprintf(stderr, "Memory allocation failed\n");
                //~ log_message(LOG_LEVEL_ERROR, "Memory allocation for book author failed");
                //~ fclose(file);
                //~ free_books(books, *count);  // Free all previously allocated memory
                //~ exit(1);
            //~ }
            //~ strcpy(books[*count].author, token);
        //~ }

        //~ token = strtok(NULL, ";");
        //~ if (token != NULL) {
            //~ books[*count].year = atoi(token);
        //~ }

        //~ token = strtok(NULL, ";");
        //~ if (token != NULL) {
            //~ books[*count].self_cost = atof(token);
        //~ }

        //~ token = strtok(NULL, ";");
        //~ if (token != NULL) {
            //~ books[*count].sell_price = atof(token);
        //~ }

        //~ (*count)++;
    //~ }

    //~ fclose(file);
    //~ log_message(LOG_LEVEL_INFO, "Books loaded successfully");
    //~ return books;
//~ }

//~ Book *load_books(const char *filename, int *count) {
    //~ FILE *file = fopen(filename, "r");
    //~ if (file == NULL) {
        //~ fprintf(stderr, "Viga avades faili %s\n", filename);
        //~ log_message(LOG_LEVEL_ERROR, "Viga avades raamatute faili");
        //~ exit(1);
    //~ }
    //~ log_message(LOG_LEVEL_INFO, "Raamatute fail avatud edukalt");

    //~ int capacity = INITIAL_BOOK_CAPACITY;
    //~ Book *books = malloc(sizeof(Book) * capacity);
    //~ if (books == NULL) {
        //~ fprintf(stderr, "Mälu reserveerimine ebaõnnestus\n");
        //~ log_message(LOG_LEVEL_ERROR, "Mälu reserveerimine raamatute jaoks ebaõnnestus");
        //~ exit(1);
    //~ }

    //~ char buffer[BUFFER_SIZE];
    //~ while (fgets(buffer, sizeof(buffer), file) != NULL) {
        //~ if (*count >= capacity) {
            //~ capacity *= 2;
            //~ books = realloc(books, sizeof(Book) * capacity);
            //~ if (books == NULL) {
                //~ fprintf(stderr, "Mälu laiendamine ebaõnnestus\n");
                //~ log_message(LOG_LEVEL_ERROR, "Mälu laiendamine raamatute jaoks ebaõnnestus");
                //~ exit(1);
            //~ }
        //~ }

        //~ char *token = strtok(buffer, ";");
        //~ strcpy(books[*count].code, token);
//~ ########################################################################################################
        //~ token = strtok(NULL, ";");
        //~ books[*count].title = strdup(token);
        //~ if (books[*count].title == NULL) {
            //~ fprintf(stderr, "Mälu reserveerimine ebaõnnestus\n");
            //~ log_message(LOG_LEVEL_ERROR, "Mälu reserveerimine raamatu pealkirja jaoks ebaõnnestus");
            //~ exit(1);
        //~ }

        //~ token = strtok(NULL, ";");
        //~ books[*count].author = strdup(token);
        //~ if (books[*count].author == NULL) {
            //~ fprintf(stderr, "Mälu reserveerimine ebaõnnestus\n");
            //~ log_message(LOG_LEVEL_ERROR, "Mälu reserveerimine raamatu autori jaoks ebaõnnestus");
            //~ exit(1);
        //~ }
        //~ ###############################################################################################################################
        //~ token = strtok(NULL, ";");
		//~ books[*count].title = malloc(strlen(token) + 1); // Allocate memory for the string
		//~ if (books[*count].title == NULL) {
			//~ fprintf(stderr, "Mälu reserveerimine ebaõnnestus\n");
			//~ log_message(LOG_LEVEL_ERROR, "Mälu reserveerimine raamatu pealkirja jaoks ebaõnnestus");
			//~ exit(1);
		//~ }
		//~ strcpy(books[*count].title, token); // Copy the string to the allocated memory

		//~ token = strtok(NULL, ";");
		//~ books[*count].author = malloc(strlen(token) + 1); // Allocate memory for the string
		//~ if (books[*count].author == NULL) {
			//~ fprintf(stderr, "Mälu reserveerimine ebaõnnestus\n");
			//~ log_message(LOG_LEVEL_ERROR, "Mälu reserveerimine raamatu autori jaoks ebaõnnestus");
			//~ exit(1);
		//~ }
		//~ strcpy(books[*count].author, token); // Copy the string to the allocated memory
//~ ###################################################################################################################################################################

        //~ token = strtok(NULL, ";");
        //~ books[*count].year = atoi(token);

        //~ token = strtok(NULL, ";");
        //~ books[*count].self_cost = atof(token);

        //~ token = strtok(NULL, ";");
        //~ books[*count].sell_price = atof(token);

        //~ (*count)++;
    //~ }

    //~ fclose(file);
    //~ log_message(LOG_LEVEL_INFO, "Raamatud laaditud edukalt");
    //~ return books;
//~ }

/**
 * Kirjeldus: Laadib inventari andmed failist ja tagastab massiivi inventari struktuuridest.
 *
 * Parameetrid: filename - faili nimi, kust inventari laadida.
 *              count - viide muutujale, kuhu salvestatakse inventari arv.
 *
 * Tagastab: Inventory* - viide inventari massiivile.
 */
Inventory *load_inventory(const char *filename, int *count) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Viga avades faili %s\n", filename);
        log_message(LOG_LEVEL_ERROR, "Viga avades inventari faili");
        exit(1);
    }
    log_message(LOG_LEVEL_INFO, "Inventari fail avatud edukalt");

    int capacity = INITIAL_INVENTORY_CAPACITY;
    Inventory *inventory = malloc(sizeof(Inventory) * capacity);
    if (inventory == NULL) {
        fprintf(stderr, "Mälu reserveerimine ebaõnnestus\n");
        log_message(LOG_LEVEL_ERROR, "Mälu reserveerimine inventari jaoks ebaõnnestus");
        exit(1);
    }

    char buffer[BUFFER_SIZE];
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        char book_code[RIDADE_ARV];
        int inventory_id, shelf_number, stock;

        if (sscanf(buffer, "%10[^;];%d;%d;%d", book_code, &inventory_id, &shelf_number, &stock) == 4) {
            if (*count >= capacity) {
                capacity *= 2;
                inventory = realloc(inventory, sizeof(Inventory) * capacity);
                if (inventory == NULL) {
                    fprintf(stderr, "Mälu laiendamine ebaõnnestus\n");
                    log_message(LOG_LEVEL_ERROR, "Mälu laiendamine inventari jaoks ebaõnnestus");
                    exit(1);
                }
            }

            strcpy(inventory[*count].book_code, book_code);
            inventory[*count].inventory_id = inventory_id;
            inventory[*count].shelf_number = shelf_number;
            inventory[*count].stock = stock;

            (*count)++;
        }
    }

    fclose(file);
    log_message(LOG_LEVEL_INFO, "Inventar laaditud edukalt");
    return inventory;
}


/**
 * Kirjeldus: Salvestab raamatute andmed faili.
 *
 * Parameetrid: filename - faili nimi, kuhu raamatud salvestada.
 *              books - viide raamatute massiivile.
 *              count - raamatute arv.
 *
 * Tagastab: Pole tagastusväärtust.
 */
void save_books(const char *filename, Book *books, int count) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        fprintf(stderr, "Viga avades faili %s\n", filename);
        log_message(LOG_LEVEL_ERROR, "Viga avades raamatute faili kirjutamiseks");
        exit(1);
    }

    for (int i = 0; i < count; i++) {
        fprintf(file, "%s;%s;%s;%d;%.2f;%.2f\n",
                books[i].code, books[i].title, books[i].author,
                books[i].year, books[i].self_cost, books[i].sell_price);
    }

    fclose(file);
    log_message(LOG_LEVEL_INFO, "Raamatud salvestatud edukalt");
}


/**
 * Kirjeldus: Salvestab inventari andmed faili.
 *
 * Parameetrid: filename - faili nimi, kuhu inventar salvestada.
 *              inventory - viide inventari massiivile.
 *              count - inventari arv.
 *
 * Tagastab: Pole tagastusväärtust.
 */
void save_inventory(const char *filename, Inventory *inventory, int count) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        fprintf(stderr, "Viga avades faili %s\n", filename);
        log_message(LOG_LEVEL_ERROR, "Viga avades inventari faili kirjutamiseks");
        exit(1);
    }

    for (int i = 0; i < count; i++) {
        fprintf(file, "%s;%d;%d;%d\n",
                inventory[i].book_code, inventory[i].inventory_id, 
                inventory[i].shelf_number, inventory[i].stock);
    }

    fclose(file);
    log_message(LOG_LEVEL_INFO, "Inventar salvestatud edukalt");
}


/**
 * Kirjeldus: Vabastab mälu, mis on reserveeritud raamatute jaoks.
 *
 * Parameetrid: books - viide raamatute massiivile.
 *              count - raamatute arv.
 *
 * Tagastab: Pole tagastusväärtust.
 */
void free_books(Book *books, int count) {
    for (int i = 0; i < count; i++) {
        free(books[i].title);
        free(books[i].author);
    }
    free(books);
}


/**
 * Kirjeldus: Vabastab mälu, mis on reserveeritud inventari jaoks.
 *
 * Parameetrid: inventory - viide inventari massiivile.
 *
 * Tagastab: Pole tagastusväärtust.
 */
void free_inventory(Inventory *inventory) {
    free(inventory);
}


/**
 * Kirjeldus: Kuvab kasutajale menüü valikud.
 *
 * Parameetrid: Pole.
 *
 * Tagastab: Pole tagastusväärtust.
 */
void display_menu() {
    printf("\n-- Menüü --\n");
    printf("1. Kuvada kõik raamatud ja nende asukohad\n");
    printf("2. Muuda raamatu pealkirja\n");
    printf("3. Muuda inventari laoseisu\n");
    printf("4. Leia võimalikud kasumid\n");
    printf("0. Välju\n");
    printf("Vali: ");
}


/**
 * Kirjeldus: Kuvab kõik raamatud ja nende asukohad inventaris.
 *
 * Parameetrid: books - viide raamatute massiivile.
 *              book_count - raamatute arv.
 *              inventory - viide inventari massiivile.
 *              inventory_count - inventari arv.
 *
 * Tagastab: Pole tagastusväärtust.
 */
void display_books(Book *books, int book_count, Inventory *inventory, int inventory_count) {
    for (int i = 0; i < book_count; i++) {
        bool found = false; // Lisatud muutuja, et kontrollida, kas raamatule leiti vastav inventari kirje
        printf("Raamatukood: %s\n", books[i].code);
        printf("Pealkiri: %s\n", books[i].title);
        printf("Autor: %s\n", books[i].author);
        printf("Aasta: %d\n", books[i].year);
        printf("Omahind: %.2f\n", books[i].self_cost);
        printf("Müügihind: %.2f\n", books[i].sell_price);
        
        printf("Asukohad:\n");
        for (int j = 0; j < inventory_count; j++) {
            if (strcmp(books[i].code, inventory[j].book_code) == 0) {
                printf(" - Inventari ID: %d, Riiuli number: %d, Laoseis: %d\n",
                       inventory[j].inventory_id, inventory[j].shelf_number, inventory[j].stock);
                found = true; // Märkige, et vastav inventari kirje leiti
            }
        }
        if (!found) { // Kui vastavat inventari kirjet ei leitud
            printf("Anmetes vist on viga või raamatukood(max 10) on erinev. Kontrollige !\n");
        }
        printf("\n");
    }
    log_message(LOG_LEVEL_INFO, "Raamatud ja nende asukohad kuvatud");
}


/**
 * Kirjeldus: Muudab valitud raamatu pealkirja.
 *
 * Parameetrid: books - viide raamatute massiivile.
 *              count - raamatute arv.
 *
 * Tagastab: Pole tagastusväärtust.
 */
 void edit_book_title(Book *books, int count) {
    char code[RIDADE_ARV];
    printf("Sisesta raamatu kood, mille pealkirja soovid muuta: ");
    scanf("%10s", code);
    
    for (int i = 0; i < count; i++) {
        if (strcmp(books[i].code, code) == 0) {
            char new_title[BUFFER_SIZE];
            printf("Sisesta uus pealkiri: ");
            getchar(); // Puhastab eelmise rea newline tähemärgi
            fgets(new_title, BUFFER_SIZE, stdin);
            new_title[strcspn(new_title, "\n")] = '\0'; // Eemalda newline

            // Vabasta eelnev pealkiri ja reserveeri mälu uuele pealkirjale
            free(books[i].title);
            books[i].title = malloc(strlen(new_title) + 1);
            if (books[i].title == NULL) {
                fprintf(stderr, "Mälu reserveerimine ebaõnnestus\n");
                log_message(LOG_LEVEL_ERROR, "Mälu reserveerimine raamatu pealkirja muutmiseks ebaõnnestus");
                exit(1);
            }
            strcpy(books[i].title, new_title);

            printf("Raamatu pealkiri muudetud edukalt.\n");
            log_message(LOG_LEVEL_INFO, "Raamatu pealkiri muudetud edukalt");
            return;
        }
    }
    printf("Raamatut koodiga %s ei leitud.\n", code);
    log_message(LOG_LEVEL_WARNING, "Raamatut koodiga ei leitud pealkirja muutmiseks");
}
//~ void edit_book_title(Book *books, int count) {
    //~ char code[RIDADE_ARV];
    //~ printf("Sisesta raamatu kood, mille pealkirja soovid muuta: ");
    //~ scanf("%10s", code);
    
    //~ for (int i = 0; i < count; i++) {
        //~ if (strcmp(books[i].code, code) == 0) {
            //~ char new_title[BUFFER_SIZE];
            //~ printf("Sisesta uus pealkiri: ");
            //~ getchar(); // Puhastab eelmise rea newline tähemärgi
            //~ fgets(new_title, BUFFER_SIZE, stdin);
            //~ new_title[strcspn(new_title, "\n")] = '\0'; // Eemalda newline

            //~ free(books[i].title);
            //~ books[i].title = strdup(new_title);
            //~ if (books[i].title == NULL) {
                //~ fprintf(stderr, "Mälu reserveerimine ebaõnnestus\n");
                //~ log_message(LOG_LEVEL_ERROR, "Mälu reserveerimine raamatu pealkirja muutmiseks ebaõnnestus");
                //~ exit(1);
            //~ }
            
            //~ printf("Raamatu pealkiri muudetud edukalt.\n");
            //~ log_message(LOG_LEVEL_INFO, "Raamatu pealkiri muudetud edukalt");
            //~ return;
        //~ }
    //~ }
    //~ printf("Raamatut koodiga %s ei leitud.\n", code);
    //~ log_message(LOG_LEVEL_WARNING, "Raamatut koodiga ei leitud pealkirja muutmiseks");
//~ }



/**
 * Kirjeldus: Muudab valitud raamatu laoseisu.
 *
 * Parameetrid: inventory - viide inventari massiivile.
 *              count - inventari arv.
 *
 * Tagastab: Pole tagastusväärtust.
 */
void edit_inventory_stock(Inventory *inventory, int count) {
    char code[RIDADE_ARV];
    int inventory_id;
    printf("Sisesta raamatu kood, mille laoseisu soovid muuta: ");
    scanf("%10s", code);
    printf("Sisesta inventari ID: ");
    if (scanf("%d", &inventory_id) != 1) {
        printf("Vigane sisend. Palun sisesta number.\n");
        while (getchar() != '\n'); // Tühjenda puhver
        return;
    }

    for (int i = 0; i < count; i++) {
        if (strcmp(inventory[i].book_code, code) == 0 && inventory[i].inventory_id == inventory_id) {
            int new_stock;
            printf("Sisesta uus laoseis: ");
            if (scanf("%d", &new_stock) != 1) {
                printf("Vigane sisend. Palun sisesta number.\n");
                while (getchar() != '\n'); // Tühjenda puhver
                return;
            }

            inventory[i].stock = new_stock;
            printf("Laoseis muudetud edukalt.\n");
            log_message(LOG_LEVEL_INFO, "Inventari laoseis muudetud edukalt");
            return;
        }
    }
    printf("Inventari kirjet koodiga %s ja ID-ga %d ei leitud.\n", code, inventory_id);
    log_message(LOG_LEVEL_WARNING, "Inventari kirjet koodiga ja ID-ga ei leitud laoseisu muutmiseks");
}


/**
 * Kirjeldus: Arvutab ja kuvab raamatute potentsiaalsed kasumid.
 *
 * Parameetrid: books - viide raamatute massiivile.
 *              book_count - raamatute arv.
 *
 * Tagastab: Pole tagastusväärtust.
 */
void find_potential_profits(Book *books, int book_count) {
    const float TAX_RATE = 0.2; // 20% tax rate
    for (int i = 0; i < book_count; i++) {
        float profit = calculate_profit(books[i].sell_price, books[i].self_cost, TAX_RATE);
        printf("Raamatu '%s' potentsiaalne kasum on %.2f\n", books[i].title, profit);
    }
    log_message(LOG_LEVEL_INFO, "Potentsiaalsed kasumid leitud ja kuvatud");
}


/**
 * Kirjeldus: Arvutab kasumi antud müügihinna, omahinna ja maksumäära põhjal.
 *
 * Parameetrid: sell_price - müügihind.
 *              self_cost - omahind.
 *              tax_rate - maksumäär.
 *
 * Tagastab: float - arvutatud kasum.
 */
float calculate_profit(float sell_price, float self_cost, float tax_rate) {
    return (sell_price - self_cost) * (1 - tax_rate);
}
