/*
    Screenshot File Format:
    A Mac OS X command line utility that allows users to choose between PNG, PDF,
    PSD, JPG, TIF, or GIF as file formats for screenshots.

    Copyright (C) 2016  Roy Vanegas

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or any
    later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/gpl-3.0.html>.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "constants.h"

int main (void)
{

#ifdef __APPLE__

   FILE *command_response;
   char line_from_command_response[RESPONSE_LENGTH];
   char current_file_format[MAX_FILE_FORMAT_LENGTH];
   char current_file_format_untreated[MAX_FILE_FORMAT_LENGTH];
   char type_found = 'f';
   char valid_type_found = 'f';
   char *token_from_line = "";
   char intro_content[MAX_INTRO_CONTENT_LENGTH];
   char input[MAX_INPUT_LENGTH];

   if (NULL == (command_response = popen("defaults read com.apple.screencapture", "r"))) {
      printf("\n\t\t%sUnable to access the required screenshot command.%s\n", RED, NORMAL);

      exit(EXIT_FAILURE);

   } else {

      /*
       * Read the response of the command, line by line
       */
      while (NULL != fgets(line_from_command_response, (int) sizeof(line_from_command_response) - 1, command_response)) {
         line_from_command_response[strlen(line_from_command_response)] = '\0';

         /*
          * Parse every token from each line in the command response
          */
         token_from_line = strtok(line_from_command_response, " ={};");

         /*
          * If “type” is set, it would appear as the first token. Thus, this is the
          * appropriate place to check for its presence.
          */
         if (0 == strcmp("type", token_from_line)) {
            /*
             * A string for the screenshot file format has been set. However, it may
             * or may not be valid.
             */
            type_found = 't';
         }

         while (token_from_line) {
            if (0 == strcmp("png", token_from_line) ||
                0 == strcmp("pdf", token_from_line) ||
                0 == strcmp("psd", token_from_line) ||
                0 == strcmp("jpg", token_from_line) ||
                0 == strcmp("tif", token_from_line) ||
                0 == strcmp("gif", token_from_line)) {

               strcpy(current_file_format_untreated, token_from_line);

               {
                  int i = -1;

                  while ('\0' != token_from_line[i++]) {
                     token_from_line[i] = toupper(token_from_line[i]);
                  }
               }

               strcpy(current_file_format, BLUE);
               strcat(current_file_format, token_from_line);
               strcat(current_file_format, NORMAL);

               valid_type_found = 't';

               break;
            }

            token_from_line = strtok(NULL, " ={};\n\r\v\f");
         }

         token_from_line = NULL;
      }
   }

   /*
    * If a type was NOT found, then the default must be PNG.
    */
   if ('f' == type_found) {
      strcpy(intro_content,"The screenshot file format setting has not been modified, meaning the default format is ");
      strcat(intro_content, BLUE);
      strcat(intro_content, "PNG");
      strcat(intro_content, NORMAL);
   } else {
      if ('f' == valid_type_found) {
         strcpy(intro_content,"The current setting for the screenshot file format is not of type PNG, PDF, PSD, JPG, TIF, or GIF.");
      } else {
         strcpy(intro_content,"The current screenshot file format is ");
         strcat(intro_content, current_file_format);
         strcat(intro_content, ".");
      }
   }

   printf("\n\t%s\n"
          "\n\t%sChoose a format from the options below, or enter %sq%s to exit and leave things as is.\n"
          "\n\tEnter %sp%s for PNG\n"
          "\t      %ss%s for PSD\n"
          "\t      %sd%s for PDF\n"
          "\t      %sj%s for JPG\n"
          "\t      %st%s for TIF\n"
          "\t      %sg%s for GIF\n",
          intro_content, /* First paragraph */
          NORMAL, RED, NORMAL,
          GREEN, NORMAL, /* PNG */
          GREEN, NORMAL, /* PSD */
          GREEN, NORMAL, /* PDF */
          GREEN, NORMAL, /* JPG */
          GREEN, NORMAL, /* TIF */
          GREEN, NORMAL  /* GIF */
   );

   (void) fgets(input, (int) sizeof(input), stdin);

   /*
    * Append the null character
    */
   input[strlen(input)] = '\0';

   /*
    * If the user’s input contains only one character (the null character assigned in the previous statement), then nothing was entered at the prompt.
    */
   if (1 == strlen(input)) {
      printf("\n%s\tEmpty entry. Nothing was changed.%s Please run this program again...\n\n", RED, NORMAL);

      exit(EXIT_FAILURE);
   } else {
      switch (input[0]) {
         case 'P':
         case 'p':
            if (0 == strcmp(current_file_format_untreated, "png")) {
               printf("\t%sPNG%s is the current file format. No change has been made to your system. Exiting…\n\n", BLUE, NORMAL);

               exit(EXIT_SUCCESS);

            } else {
               if (NULL == (command_response = popen("defaults write com.apple.screencapture type png", "r"))) {
                  printf("Unable to set PNG. Either memory could not be allocated, or forking or piping failed. Please report this issue on GitHub at https://github.com/code-warrior/screenshot-file-format/issues or via email at roy@thecodeeducators.com.\n");

                  exit(EXIT_FAILURE);

               } else {
                  printf("\tThe screenshot file format is now %sPNG%s. Exiting…\n\n", BLUE, NORMAL);
               }
            }

            break;

         case 'S':
         case 's':
            if (0 == strcmp(current_file_format_untreated, "psd")) {
               printf("\t%sPSD%s is the current file format. No change has been made to your system. Exiting…\n\n", BLUE, NORMAL);

               exit(EXIT_SUCCESS);

            } else {
               if (NULL == (command_response = popen("defaults write com.apple.screencapture type psd", "r"))) {
                  printf("Unable to set PSD. Either memory could not be allocated, or forking or piping failed. Please report this issue on GitHub at https://github.com/code-warrior/screenshot-file-format/issues or via email at roy@thecodeeducators.com.\n");

                  exit(EXIT_FAILURE);

               } else {
                  printf("\tThe screenshot file format is now %sPSD%s. Exiting…\n\n", BLUE, NORMAL);
               }
            }

            break;

         case 'D':
         case 'd':
            if (0 == strcmp(current_file_format_untreated, "pdf")) {
               printf("\t%sPDF%s is the current file format. No change has been made to your system. Exiting…\n\n", BLUE, NORMAL);

               exit(EXIT_SUCCESS);

            } else {
               if (NULL == (command_response = popen("defaults write com.apple.screencapture type pdf", "r"))) {
                  printf("Unable to set PDF. Either memory could not be allocated, or forking or piping failed. Please report this issue on GitHub at https://github.com/code-warrior/screenshot-file-format/issues or via email at roy@thecodeeducators.com.\n");

                  exit(EXIT_FAILURE);

               } else {
                  printf("\tThe screenshot file format is now %sPDF%s. Exiting…\n\n", BLUE, NORMAL);
               }
            }

            break;

         case 'J':
         case 'j':
            if (0 == strcmp(current_file_format_untreated, "jpg")) {
               printf("\t%sJPG%s is the current file format. No change has been made to your system. Exiting…\n\n", BLUE, NORMAL);

               exit(EXIT_SUCCESS);

            } else {
               if (NULL == (command_response = popen("defaults write com.apple.screencapture type jpg", "r"))) {
                  printf("Unable to set JPG. Either memory could not be allocated, or forking or piping failed. Please report this issue on GitHub at https://github.com/code-warrior/screenshot-file-format/issues or via email at roy@thecodeeducators.com.\n");

                  exit(EXIT_FAILURE);

               } else {
                  printf("\tThe screenshot file format is now %sJPG%s. Exiting…\n\n", BLUE, NORMAL);
               }
            }

            break;

         case 'T':
         case 't':
            if (0 == strcmp(current_file_format_untreated, "tif")) {
               printf("\t%sTIF%s is the current file format. No change has been made to your system. Exiting…\n\n", BLUE, NORMAL);

               exit(EXIT_SUCCESS);

            } else {
               if (NULL == (command_response = popen("defaults write com.apple.screencapture type tif", "r"))) {
                  printf("Unable to set TIF. Either memory could not be allocated, or forking or piping failed. Please report this issue on GitHub at https://github.com/code-warrior/screenshot-file-format/issues or via email at roy@thecodeeducators.com.\n");

                  exit(EXIT_FAILURE);

               } else {
                  printf("\tThe screenshot file format is now %sTIF%s. Exiting…\n\n", BLUE, NORMAL);
               }
            }

            break;

         case 'G':
         case 'g':
            if (0 == strcmp(current_file_format_untreated, "gif")) {
               printf("\t%sGIF%s is the current file format. No change has been made to your system. Exiting…\n\n", BLUE, NORMAL);

               exit(EXIT_SUCCESS);

            } else {
               if (NULL == (command_response = popen("defaults write com.apple.screencapture type gif", "r"))) {
                  printf("Unable to set GIF. Either memory could not be allocated, or forking or piping failed. Please report this issue on GitHub at https://github.com/code-warrior/screenshot-file-format/issues or via email at roy@thecodeeducators.com.\n");

                  exit(EXIT_FAILURE);

               } else {
                  printf("\tThe screenshot file format is now %sGIF%s. Exiting…\n\n", BLUE, NORMAL);
               }
            }

            break;
      }
   }

   pclose(command_response);

#else

   printf("\n\nThis program is designed to only work in a Mac OS X environment.\n\n");

#endif

   return 0;
}
