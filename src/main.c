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


#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

#include "utils/file.h"
#include "lexer/lexer.h"
#include "common/token_type.h"


void compile(char const * source_path);


int main(int argc, char * argv[])
{
    // At the moment, we do not allow arguments
    // This is motivated by the fact that the generated code will be running on a QC that do not provide access to command line arguments
    // And since our goals is to immediately target such systems, we ommit this feature for now.

    if (argc != 2) {
        printf("Usage: avalon program\n");
    }
    else {
        char * source_path = argv[1];
        compile(source_path);
    }

    return 0;
}

void compile(char const * source_path) {
    /* We begin by making sure the given source path exists */
    if (fileExists(source_path) == false) {
        fprintf(stderr, "File <%s> was not found.\n", source_path);
        return;
    }

    /* 1. Configure the compiler */
    // Add the current directory to the search path

    // Add the given source directory to the search path

    // Add the AVALON_HOME directory to the search path

    // Add the AVALON_PATH directories to the search path

    /* 2. Invoke the compiler */
    char * source = readFile(source_path);
    printf("%s\n", source);

    struct Lexer * lexer = newLexer(source_path, source);
    size_t line = 0;
    for (;;) {
        struct Token token = lexToken(lexer);
        if (token.line != line) {
            printf("%4zu ", token.line);
            line = (int) token.line;
        } else {
            printf("   | ");
        }
        if (token.type == AVL_NEWLINE || token.type == AVL_DEDENT || token.type == AVL_INDENT)
            printf("%-20s ''\n", tokenTypeToString(token.type));
        else
            printf("%-20s '%.*s'\n", tokenTypeToString(token.type), (int) token.length, token.start);

        if (token.type == AVL_EOF)
            break;
    }

    free(source);
    deleteLexer(& lexer);
}
