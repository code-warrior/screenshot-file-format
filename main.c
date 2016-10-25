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

int main (int argc, char **argv)
{
   FILE *command_response;
   char line_from_command_response[RESPONSE_LENGTH];
   char current_file_format[MAX_FILE_FORMAT_LENGTH];
   char type_found = 'f';
   char valid_type_found = 'f';
   char *token_from_line = "";

   /*
    * Run the screen capture read command, which will return NULL, nothing, or a string that may or may not look like the following:

       {
         "disable-shadow" = 0;
         type = gif;
       }

    */
   if (NULL == (command_response = popen("defaults read com.apple.screencapture", "r"))) {
      printf("\n\t\t%sUnable to access the required screenshot command.%s\n", RED, NORMAL);

      exit(-1);

   } else {

      //
      // Read the response of the command, line by line
      //
      while (NULL != fgets(line_from_command_response, sizeof(line_from_command_response) - 1, command_response)) {
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
      }
   }

   //
   // If a type was NOT found, then the default must be PNG.
   //
   if ('f' == type_found) {
      printf("The screenshot file format setting has not been modified, meaning that the default format is PNG.\n");
   } else {
      if ('f' == valid_type_found) {
         printf("The screenshot file format is not of type PNG, PDF, PSD, JPG, TIF, or GIF.\n");
      } else {
         printf("The screenshot file format is %s\n", current_file_format);
      }
   }

   return 0;
}
