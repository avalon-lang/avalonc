/*  This file is part of the Avalon programming language
 * 
 *  Copyright (c) 2018-2019 Ntwali Bashige Toussaint
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>

#include "utils/file.h"

/**
 * Verify if a file exists given the path to the file. Returns true if it does, false otherwise.
 */
bool fileExists(char const * path) {
    FILE * file = fopen(path, "rb");
    if (file == NULL) {
        return false;
    }
    else {
        fclose(file);
        return true;
    }
}


/**
 * Reads the content of the file at the given path and returns a pointer to said content.
 */
char * readFile(char const * path) {
    FILE * file = fopen(path, "rb");
    
    // We could not open the file
    if (file == NULL) {
        fprintf(stderr, "Failed to open file <%s>.", path);
        exit(74);
    }

    // Get the file size
    fseek(file, 0L, SEEK_END);
    size_t file_size = ftell(file);
    rewind(file);

    // Attempt to allocate enough memory to hold the file content
    char * buffer = malloc(file_size + 1);  
    if (buffer == NULL)                   {
        fprintf(stderr, "Failed to allocate enough memory to hold the file at <%s>.", path);
        exit(74);
    }

    // Place the file content into the buffer
    size_t bytes_read = fread(buffer, sizeof(char), file_size, file);
    if (bytes_read < file_size) {
        fprintf(stderr, "Could not read the content of the file at <%s>.\n", path);
        exit(74);
    }
    buffer[bytes_read] = '\0';
    fclose(file);
    return buffer;
}
