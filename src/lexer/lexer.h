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

#ifndef LEXER_H
#define LEXER_H

#include <stdbool.h>
#include <stddef.h>

#include "common/token_type.h"
#include "common/token.h"


struct Lexer {
    char const * file;
    char const * start;
    char const * current;
    size_t line;
    size_t column;

    /* The following members help us keep track of spaces since whitespace is significant in Avalon */

    /* Indentation tracking.
     * This flag allows to know if we can ignore whitespace.
     * We use this knowledge to figure out when we need to emit INDENT and DEDENT tokens.
     */
    bool ignore_whitespace;

    /* We do not allow indentation to at the very beginning of the source code.
     * We also require that all indentations found in the source file be a multiple of the very first indentation found.
     * Also, since spaces or tabs can be used for indentation, we require that only one of them be used throughout a source.
     *
     * first_indentation_found      : signals whether we have our first indentation.
     * is_first_indentation_space   : signals whehter the first indentation found is a space or a tab.
     * first_indentation_line       : for error reporting purposes, we keep track of the line where the first indentation was found. This is so we can let users know how many spaces (tabs) their first indentation is made of.
     */
    bool first_indentation_found;
    bool is_first_indentation_space;
    size_t first_indentation_line;

    /* Indentation tracking
     * This helps determine if we should emit an INDENT token or a DEDENT token.
     */
    size_t last_indentation_count;
};


/**
 * Initializes the lexer by allocate memory for it and setting up the Lexer struct fields.
 *
 * @param       file pointer to the file name which is used to show meaningful error messages.
 * @param       source pointer to the string containing the entirety of the source code contained in the source file.
 */
struct Lexer * newLexer(char const * file, char const * source);


/**
 * Frees the memory occupied by the lexer and associated structures.
 *
 * @param       lexer pointer to pointer to the memory occupied by the lexer.
 */
void deleteLexer(struct Lexer ** lexer);


/**
 * Performs the lexing process until all the entire content of the source has been read or an error is encountered.
 *
 * @param       lexer pointer to the lexer.
 *
 * @return      the token that was just read.
 */
struct Token lexToken(struct Lexer * const lexer);

#endif
