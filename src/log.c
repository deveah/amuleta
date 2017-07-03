
/*
 *  log.c
 *  Part of Amuleta, a traditional roguelike - https://deveah.github.io/amuleta
 *  (c) Vlad Dumitru, <dalv.urtimud@gmail.com>
 *  Licensed under the terms and conditions of the MIT License. Please consult
 *  the LICENSE file included with this project.
 */

#include <assert.h>
#include <stdio.h>
#include <time.h>
#include "amuleta.h"

/*  pointer to the log file handle */
FILE *log_file = NULL;

/*
 *  initializes the log file
 *
 *  void return
 */
void initialize_log(void)
{
  log_file = fopen(LOG_FILE_PATH, "w");
  assert(log_file != NULL);

  INFO("Amuleta logging initialized at timestamp %i\n", time(NULL));
}

/*
 *  terminates the log file
 *
 *  void return
 */
void terminate_log(void)
{
  /*  if the log file is not open, do nothing */
  if (log_file == NULL) {
    return;
  }

  INFO("Amuleta logging terminated at timestamp %i\n", time(NULL));
  fclose(log_file);
}

/*
 *  appends a log entry
 *
 *  parameters similar to the printf(3) function
 *  void return
 */
void append_log(char *format, ...)
{
  va_list args;

  /*  if there's nothing to format, do nothing */
  if (format == NULL) {
    return;
  }

  /*  if the log file is not open, do nothing */
  if (log_file == NULL) {
    return;
  }

  va_start(args, format);
  vfprintf(log_file, format, args);
  va_end(args);
}

