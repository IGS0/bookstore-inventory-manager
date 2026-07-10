# Bookstore Inventory Manager / Raamatulao haldur

A console application written in C for managing bookstore inventory, editing book information, calculating potential profits, and logging program activity.

C-keeles kirjutatud konsooliprogramm raamatulao haldamiseks, raamatute andmete muutmiseks, kasumi arvutamiseks ning programmi töö logimiseks.

---

# English

## Features

* Loads book information from a text file.
* Loads inventory information from a separate text file.
* Uses dynamic memory allocation for storing book and inventory records.
* Displays all books together with their inventory locations.
* Edit book titles.
* Edit stock quantities for selected inventory records.
* Calculates potential profit for each book based on purchase and selling prices.
* Saves modified data back to the original files.
* Logs program activity with configurable log levels.
* Supports book titles and author names containing multiple words.
* Uses semicolon (`;`) as the field separator.
* Includes input validation and file error handling.

## Project Structure

```text
bookstore.c          Main application
bookstore.h          Data structures and function declarations
logger.c             Logging implementation
logger.h             Logging interface
books.txt            Book database
inventory.txt        Inventory database
Makefile             Build configuration
```

## Requirements

* GCC compiler
* GNU Make

## Building

Compile the project using:

```bash
make
```

The Makefile automatically compiles all source files and links them into the executable.

## Running

```bash
./bookstore
```

## Program Workflow

1. The program loads all book records.
2. Inventory information is loaded into memory.
3. A menu is displayed.
4. The user selects one of the available operations.
5. Changes are applied immediately in memory.
6. Updated data can be saved back to the text files.
7. Program activity is written to the log file.
8. Before exiting, allocated memory is released and files are closed.

## Menu Functions

* Display all books and inventory locations.
* Edit a book title.
* Update inventory stock.
* Calculate potential profits.
* Save changes.
* Exit the application.

## Logging

The project contains a dedicated logging module.

Supported features:

* Configurable log file.
* Multiple log levels:

  * INFO
  * WARNING
  * ERROR
* Timestamped log entries.
* Formatted log messages.

## Input File Format

### books.txt

Each line contains:

```text
BookCode;Title;Author;Year;PurchasePrice;SellingPrice
```

Example:

```text
1234567890;The Great Gatsby;F. Scott Fitzgerald;1925;10.00;15.00
```

### inventory.txt

Each line contains:

```text
BookCode;InventoryID;ShelfStock;WarehouseStock
```

Example:

```text
1234567890;1;5;10
```

## Technical Details

* Written in C.
* Modular project structure.
* Dynamic memory management.
* Separate logging module.
* Error handling for invalid input.
* Memory cleanup before program termination.
* Compatible with GCC and Make.

---

# Eesti

## Funktsioonid

* Loeb raamatute andmed tekstifailist.
* Loeb laoandmed eraldi failist.
* Kasutab dünaamilist mälu andmete hoidmiseks.
* Kuvab kõik raamatud koos nende laoasukohaga.
* Võimaldab muuta raamatu pealkirja.
* Võimaldab muuta laoseisu.
* Arvutab iga raamatu võimaliku kasumi.
* Salvestab muudatused tagasi failidesse.
* Logib programmi töö eraldi logifaili.
* Toetab mitmesõnalisi raamatupealkirju ja autorinimesid.
* Kasutab väljade eraldajana semikoolonit (`;`).
* Sisaldab sisendi ja failide vigade kontrolli.

## Projekti struktuur

```text
bookstore.c          Programmi põhifail
bookstore.h          Struktuurid ja funktsioonide deklaratsioonid
logger.c             Logimise moodul
logger.h             Logimise päisefail
books.txt            Raamatute andmed
inventory.txt        Laoandmed
Makefile             Projekti kompileerimine
```

## Nõuded

* GCC
* GNU Make

## Kompileerimine

```bash
make
```

## Käivitamine

```bash
./bookstore
```

## Programmi töövoog

1. Programm loeb raamatute andmed.
2. Loetakse inventari andmed.
3. Kuvatakse menüü.
4. Kasutaja valib soovitud tegevuse.
5. Muudatused tehakse mälus.
6. Andmed salvestatakse tagasi failidesse.
7. Programmi töö logitakse.
8. Programmi lõpetamisel vabastatakse mälu ja suletakse failid.

## Menüü võimalused

* Kuvada kõik raamatud ja nende laoseis.
* Muuta raamatu pealkirja.
* Muuta inventari koguseid.
* Arvutada võimalik kasum.
* Salvestada muudatused.
* Programmist väljuda.

## Logimine

Projekt sisaldab eraldi logimise moodulit.

Toetatud võimalused:

* Logifaili määramine.
* Logimise tasemed:

  * INFO
  * WARNING
  * ERROR
* Ajatempliga logikirjed.
* Vormindatud logisõnumid.

## Sisendfailide vorming

### books.txt

```text
RaamatuKood;Pealkiri;Autor;Aasta;Omahind;Müügihind
```

Näide:

```text
1234567890;The Great Gatsby;F. Scott Fitzgerald;1925;10.00;15.00
```

### inventory.txt

```text
RaamatuKood;InventariID;Riiuliseis;Laoseis
```

Näide:

```text
1234567890;1;5;10
```

## Tehnilised omadused

* Kirjutatud C-keeles.
* Modulaarne projekti ülesehitus.
* Dünaamiline mäluhaldus.
* Eraldi logimise moodul.
* Sisendi valideerimine.
* Mälu korrektne vabastamine programmi lõpus.
* Sobib kompileerimiseks GCC ja Make abil.
