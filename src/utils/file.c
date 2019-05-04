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
#include <stdio.h>

#include "utils/file.h"
#include "utils/result.h"

/**
 * Verify if a file exists given the path to the file. Returns true if it does, false otherwise.
 */
bool fileExists(const char * path) {
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
struct Result readFile(const char * path) {
    FILE * file = fopen(path, "rb");
    struct Result result;

    if (file == NULL) {
        char * error = malloc(24 + strlen(path));

        // If we run out of heap memory, we rely of fprintf to display the error without attempting further heap allocation
        if (error == NULL) {
            fprintf(stderr, "Ran out of memory while trying to read the file <%s>.\n", path);
            result = createResult(GENERIC_ERROR, NULL);
        }
        // If we failed to find the file, we leave it to the caller to decide how to display the error
        else {
            sprintf(error, "Failed to open file <%s>.", path);
            result = createResult(GENERIC_ERROR, error);
        }
    }
    else {
        fseek(file, 0L, SEEK_END);
        size_t file_size = ftell(file);
        rewind(file);

        char * buffer = malloc(file_size + 1);                    
        size_t bytes_read = fread(buffer, sizeof(char), file_size, file);
        buffer[bytes_read] = '\0';

        result = createResult(PCHAR_VALUE, (void *)buffer);
        fclose(file);
    }
    
    return result;
}
