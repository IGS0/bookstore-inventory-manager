#ifndef BOOKSTORE_H
#define BOOKSTORE_H

#define INITIAL_BOOK_CAPACITY 10
#define INITIAL_INVENTORY_CAPACITY 10
#define BUFFER_SIZE 256
#define RIDADE_ARV 11

#define BOOKS_FILE "books.txt"
#define INVENTORY_FILE "inventory.txt"

typedef struct {
    char code[RIDADE_ARV];
    char *title;
    char *author;
    int year;
    float self_cost;
    float sell_price;
} Book;

typedef struct {
    char book_code[RIDADE_ARV];
    int inventory_id;
    int shelf_number;
    int stock;
} Inventory;

Book *load_books(const char *filename, int *count);
Inventory *load_inventory(const char *filename, int *count);
void save_books(const char *filename, Book *books, int count);
void save_inventory(const char *filename, Inventory *inventory, int count);
void free_books(Book *books, int count);
void free_inventory(Inventory *inventory);
void display_menu();
void display_books(Book *books, int book_count, Inventory *inventory, int inventory_count);
void edit_book_title(Book *books, int count);
void edit_inventory_stock(Inventory *inventory, int count);
void find_potential_profits(Book *books, int book_count);
float calculate_profit(float sell_price, float self_cost, float tax_rate);

#endif // BOOKSTORE_H
