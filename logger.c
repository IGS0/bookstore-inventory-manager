#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdarg.h>
#include "logger.h"

static FILE *log_file = NULL;
static LogLevel current_log_level = LOG_LEVEL_INFO;
static char log_file_name[BUFFER_SIZE] = "application.log";


// Close the log file
void close_log_file() {
    if (log_file != NULL) {
        fclose(log_file);
        log_file = NULL;
    }
}


/**
 * Description:    Määrab logifaili logimise eesmärgil. Kui logifail on juba avatud,
 * 					see sulgeb praeguse logifaili ja avab uue antud failinimega.
 *
 * Parameters:     filename - millest fali loeb

 * 				   
 * Return:         Pole tagastusväärtust.
 */
 void set_log_file(const char *filename) {
    if (log_file != NULL) {
        fclose(log_file);
        log_file = NULL;
    }
    snprintf(log_file_name, sizeof(log_file_name), "%s", filename);
    log_file = fopen(log_file_name, "a");
    if (log_file == NULL) {
        fprintf(stderr, "Failed to open log file: %s\n", log_file_name);
        exit(1);
    }
}

//~ void set_log_file(const char *filename) {
    //~ if (log_file != NULL) {
        //~ fclose(log_file);
    //~ }
    //~ snprintf(log_file_name, sizeof(log_file_name), "%s", filename);
    //~ log_file = fopen(log_file_name, "a");
    //~ if (log_file == NULL) {
        //~ fprintf(stderr, "Failed to open log file: %s\n", log_file_name);
		//~ free(log_file);
        //~ exit(1);
    //~ }
//~ }

/**
 * Kirjeldus: Määrab praeguse logimise taseme.
 *
 * Parameetrid: level - logimise tase, mis määrab milliseid sõnumeid logitakse.
 *
 * Tagastab: Pole tagastusväärtust.
 */
void set_log_level(LogLevel level) {
    current_log_level = level;
}


/**
 * Kirjeldus: Logib sõnumi antud logimise tasemel koos ajamärgisega logifaili.
 *
 * Parameetrid: level - logimise tase (INFO, WARNING, ERROR).
 *              format - sõnumi vorming string, nagu printf.
 *              ... - muutujate arv, mis vastavad format stringile.
 *
 * Tagastab: Pole tagastusväärtust.
 */
void log_message(LogLevel level, const char *format, ...) {
    if (level < current_log_level) {
        return;
    }
    if (log_file == NULL) {
        log_file = fopen(log_file_name, "a");
        if (log_file == NULL) {
            fprintf(stderr, "Failed to open log file: %s\n", log_file_name);
            exit(1);
        }
    }

    time_t now;
    time(&now);
    struct tm *local = localtime(&now);

    fprintf(log_file, "[%02d-%02d-%02d %02d:%02d:%02d] ",
            local->tm_year + 1900, local->tm_mon + 1, local->tm_mday,
            local->tm_hour, local->tm_min, local->tm_sec);

    switch (level) {
        case LOG_LEVEL_INFO:
            fprintf(log_file, "[INFO] ");
            break;
        case LOG_LEVEL_WARNING:
            fprintf(log_file, "[WARNING] ");
            break;
        case LOG_LEVEL_ERROR:
            fprintf(log_file, "[ERROR] ");
            break;
    }

    va_list args;
    va_start(args, format);
    vfprintf(log_file, format, args);
    va_end(args);

    fprintf(log_file, "\n");
    fflush(log_file);

}

