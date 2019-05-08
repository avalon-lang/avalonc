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
#include "utils/stack.h"


struct Lexer {
    char const * file;
    char const * start;
    char const * current;
    size_t line;
    size_t column;

    /* The following members help us keep track of spaces since whitespace is significant in Avalon */

    /* Indentation tracking
     * Let's us kow well indented the source code is and if we should ignore any found indentation.
     * Keeps track of how deeply nested the current line(block) is from the start of the line.
     */
    struct SizeTStack * indentation_stack;

    /* Since indentation is ignored within parenthesis, braces and brackets;
     * we must keep track of how deep within said braces we are so we can know when to re-enable indentation.
     *
     * parens_levels                : how deep we are within parentheses.
     * braces_levels                : how deep we are within curly braces.
     * brackets_levels              : how deep we are within square braces.
     */
    size_t parens_levels;
    size_t braces_levels;
    size_t brackets_levels;

    /* We dinstinguish between logical lines and physical lines.
     * A line that ends with a backlash (and not within a character or a string)
     * is considered to signal the end of a phyical line but to be part of a single
     * logical line that starts on the next physical line.
     *
     * So we must know whether we have a new logical line in order to know if a new line is significant.
     * 
     * is_new_logical_line          : a flag that indicates whether we have a new logical line
     * in_new_logical_line          : a flag that indicates wheter we are currently lexing a single logical like that may be spread over multiple physical lines.
     * ignore_new_line              : when we meet a new line, this flag is set to true if we intend to mark the coming line as part of a single logical line
     */
    bool is_new_logical_line;
    bool in_new_logical_line;
    bool ignore_new_line;

    /* We do not allow indentation to at the very beginning of the source code.
     * We also require that all indentations found in the source file be a multiple of the very first indentation found.
     * Also, since spaces or tabs can be used for indentation, we require that only one of them be used throughout a source.
     *
     * is_first_indentation         : signals whether we have our first indentation.
     * is_first_indentation_space   : signals whehter the first indentation found is a space or a tab.
     * indentation_space_count      : tabs are transformed into logical spaces. Spaces remain as such. Either way, we must know the number of empty spaces used at the first indentation so we can enforce the multiplicity rule.
     * indentation_space_count_line : for error reporting purposes, we keep track of the line where the first indentation was found. This is so we can let users know how many spaces (tabs) their first indentation is made of.
     */
    bool is_first_indentation;
    bool is_first_indentation_space;
    size_t indentation_space_count;
    size_t indentation_space_count_line;
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
