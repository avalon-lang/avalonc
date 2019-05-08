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
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>

#include "common/token_type.h"
#include "common/token.h"
#include "utils/stack.h"
#include "lexer/lexer.h"


static bool isAtEnd(struct Lexer * const lexer);
static char advance(struct Lexer * const lexer);
static bool match(struct Lexer * const lexer, char expected);
static struct Token makeToken(struct Lexer const * const lexer, enum TokenType type);
static struct Token errorToken(struct Lexer const * const lexer, char const * message);


/**
 * Initializes the lexer by allocate memory for it and setting up the Lexer struct fields.
 *
 * @param       file pointer to the file name which is used to show meaningful error messages.
 * @param       source pointer to the string containing the entirety of the source code contained in the source file.
 */
struct Lexer * newLexer(char const * file, char const * source) {
    struct Lexer * lexer = malloc(sizeof *lexer);

    if (lexer == NULL) {
        fprintf(stderr, "Ran out of memory while initializing the lexer.\n");
        exit(74);
    }

    lexer -> file = file;
    lexer -> start = source;
    lexer -> current = source;
    lexer -> line = 1;
    lexer -> column = 1;

    lexer -> indentation_stack = newSizeTStack(5, "Ran out of memory while initializing the lexer.");
    sizeTStackPush(lexer -> indentation_stack, 0);
    lexer -> parens_levels = 0;
    lexer -> braces_levels = 0;
    lexer -> brackets_levels = 0;
    lexer -> is_new_logical_line = false;
    lexer -> in_new_logical_line = false;
    lexer -> ignore_new_line = false;
    lexer -> is_first_indentation = false;
    lexer -> is_first_indentation_space = false;
    lexer -> indentation_space_count = 0;
    lexer -> indentation_space_count_line = 0;

    return lexer;
}


/**
 * Frees the memory occupied by the lexer and associated structures.
 *
 * @param       lexer pointer to pointer to the memory occupied by the lexer.
 */
void deleteLexer(struct Lexer ** lexer) {
    deleteSizeTStack(& (* lexer) -> indentation_stack);
    free(* lexer);
    * lexer = NULL;
}


/**
 * Performs the lexing process until all the entire content of the source has been read or an error is encountered.
 *
 * @param       lexer pointer to the lexer.
 *
 * @return      dynamic array containing all the tokens that have been read.
 */
struct Token lexToken(struct Lexer * const lexer) {
    lexer -> start = lexer -> current;

    if (isAtEnd(lexer))
        return makeToken(lexer, AVL_EOF);
    
    char c = advance(lexer);
    switch (c) {
        case '.':
            return makeToken(lexer, AVL_DOT);

        case '!':
            if (match(lexer, '=')) {
                return makeToken(lexer, AVL_NOT_EQUAL);
            }
            else {
                if(lexer -> is_new_logical_line) {
                    lexer -> is_new_logical_line = false;
                    
                    while(sizeTStackTop(lexer -> indentation_stack) > 0) {
                        sizeTStackPop(lexer -> indentation_stack);
                        return makeToken(lexer, AVL_DEDENT);
                    }
                }

                lexer -> in_new_logical_line = true;
                return makeToken(lexer, AVL_LOGICAL_NOT);
            }

        default:
            ;
            
    }

    return errorToken(lexer, "Unexpected character.");
}


/**
 * Returns true if we encounter the null byte character which signals the end of the source file.
 *
 * @param       lexer pointer to the lexer.
 *
 * @return      true if we are at the end of the source, false otherwise.
 */
static bool isAtEnd(struct Lexer * const lexer) {
    return *(lexer -> current) == '\0';
}


/**
 * Returns the current character in the source stream and advances the pointer to the next available character.
 *
 * @param       lexer pointer to the lexer.
 *
 * @return      the current character in the source stream.
 */
static char advance(struct Lexer * const lexer) {
    lexer -> current++;
    return lexer -> current[-1];
}


/**
 * Returns true if the current character macthes the given one. It advances pointer to the next available character if the match succeeds.
 *
 * @param       lexer pointer to the lexer.
 * @param       expected the character we are hoping to match against.
 *
 * @return      true if the current character matches the expected character, false otherwise.
 */
static bool match(struct Lexer * const lexer, char expected) {               
    if (isAtEnd(lexer))
        return false;

    if (* lexer -> current != expected)
        return false;

    lexer -> current++;
    return true;
}


/**
 * Creates a token.
 *
 * @param       lexer pointer to the lexer.
 *
 * @return      the newly created token.
 */
static struct Token makeToken(struct Lexer const * const lexer, enum TokenType type) {
    struct Token token;
    token.type = type;
    token.file = lexer -> file;
    token.start = lexer -> start;
    token.length = (size_t)(lexer -> current - lexer -> start);
    token.line = lexer -> line;
    token.column = lexer -> column;

    return token;
}


/**
 * Creates an error token.
 *
 * @param       lexer pointer to the lexer.
 *
 * @return      the newly created token.
 */
static struct Token errorToken(struct Lexer const * const lexer, char const * message) {
    struct Token token;
    token.type = AVL_ERROR;
    token.file = lexer -> file;
    token.start = message;
    token.length = strlen(message);
    token.line = lexer -> line;
    token.column = lexer -> column;

    return token;
}
