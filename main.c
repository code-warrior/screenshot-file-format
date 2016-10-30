/*
    Screenshot File Format:
    A Mac OS X command line utility that allows users to choose between PNG, PDF, PSD
    JPG, TIF, or GIF as file formats for screenshots.

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
   FILE *command_response;
   char line_from_command_response[RESPONSE_LENGTH];
   char current_file_format[MAX_FILE_FORMAT_LENGTH];
   char type_found = 'f';
   char valid_type_found = 'f';
   char *token_from_line = "";
   char intro_content[MAX_INTRO_CONTENT_LENGTH];
   char input[MAX_INPUT_LENGTH];

   if (NULL == (command_response = popen("defaults read com.apple.screencapture", "r"))) {
      printf("\n\t\t%sUnable to access the required screenshot command.%s\n", RED, NORMAL);

      exit(EXIT_FAILURE);

   } else {

      //
      // Read the response of the command, line by line
      //
      while (NULL != fgets(line_from_command_response, (int) sizeof(line_from_command_response) - 1, command_response)) {
         line_from_command_response[strlen(line_from_command_response)] = '\0';

         //
         // Parse every token from each line in the command response
         //
         token_from_line = strtok(line_from_command_response, " ={};");

         //
         // If “type” is set, it would appear as the first token. Thus, this is the
         // appropriate place to check for its presence.
         //
         if (0 == strcmp("type", token_from_line)) {
            //
            // A string for the screenshot file format has been set. However, it may
            // or may not be valid.
            //
            type_found = 't';
         }

         while (token_from_line) {
            if (0 == strcmp("png", token_from_line) ||
                0 == strcmp("pdf", token_from_line) ||
                0 == strcmp("psd", token_from_line) ||
                0 == strcmp("jpg", token_from_line) ||
                0 == strcmp("tif", token_from_line) ||
                0 == strcmp("gif", token_from_line)) {

               {
                  int i = -1;

                  while ('\0' != token_from_line[i++]) {
                     token_from_line[i] = toupper(token_from_line[i]);
                  }
               }

               strcpy(current_file_format, GREEN);
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

   //
   // If a type was NOT found, then the default must be PNG.
   //
   if ('f' == type_found) {
      strcpy(intro_content,"The screenshot file format setting has not been modified, meaning the default format is ");
      strcat(intro_content, BLUE);
      strcat(intro_content, "PNG");
      strcat(intro_content, NORMAL);
   } else {
      if ('f' == valid_type_found) {
         strcpy(intro_content,"The current setting for the screenshot file format is not of type PNG, PDF, PSD, JPG, TIF, or GIF.");
      } else {
         strcat(intro_content,"The current screenshot file format is ");
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
          intro_content, // First paragraph
          NORMAL, RED, NORMAL,
          GREEN, NORMAL, // PNG
          GREEN, NORMAL, // PSD
          GREEN, NORMAL, // PDF
          GREEN, NORMAL, // JPG
          GREEN, NORMAL, // TIF
          GREEN, NORMAL  // GIF
   );

   (void) fgets(input, (int) sizeof(input), stdin);

   //
   // Append the null character
   //
   input[strlen(input)] = '\0';

   //
   // If the user’s input contains only one character (the null character assigned in the previous statement), then nothing was entered at the prompt.
   //
   if (1 == strlen(input)) {
      printf("\n%s\tEmpty entry.%s Please run this program again...\n\n", RED, NORMAL);

      exit(EXIT_FAILURE);
   }

   pclose(command_response);

   return 0;
}
