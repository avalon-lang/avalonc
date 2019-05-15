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

#include "common/ast/program.h"
#include "common/token_type.h"
#include "parser/parser.h"
#include "common/token.h"
#include "lexer/lexer.h"


static void completeParserInit(struct Parser * const parser);
static struct Token invalidToken(struct Parser const * const parser);


/**
 * Initializes the parser and returns it, ready to parse.
 *
 * @param       program the program that is to hold all the declarations found.
 * @param       lexer the lexer to give us tokens
 *
 * @return      the parser that is was initialized.
 */
struct Parser * newParser(struct Lexer * const lexer, struct Program * const program) {
    char const * message = "Parser initialization failed.";

    // We make sure we are given valid parameters
    if (lexer == NULL)
        goto exit;

    if (program == NULL)
        goto exit;

    // We proceed to allocate memory for the parser
    struct Parser * parser = malloc(sizeof *parser);
    if (parser == NULL)
        goto exit;

    parser -> lexer = lexer;
    parser -> program = program;
    parser -> namespace = "*";

    // We need to set tokens before the parsing begins so we call the function below to do the same
    completeParserInit(parser);

exit:
    fprintf(stderr, "File: %s.\nLine: %d.\nOperation: newParser.\nMessage: %s\n", __FILE__, __LINE__, message);
    exit(74);
}


/**
 * Frees the memory allocated for the parser.
 *
 * @param       parser the parser which memory to free.
 */
void deleteParser(struct Parser ** const parser) {
    if (parser == NULL)
        return;

    if (* parser == NULL)
        return;

    free(* parser);
    * parser = NULL;
}


/**
 * The parser entry point. It receives tokens from the lexer and determines what to do with them.
 *
 * @param       parser the parser with information needed for parsing.
 *
 * @return      the program with all the declarations set.
 */
struct Program * parse(struct Parser * const parser) {

    return parser -> program;
}


/**
 * Sets up the previous, current and next tokens on the parser.
 *
 * @param       parser the parser on which to set said tokens.
 */
static void completeParserInit(struct Parser * const parser) {
    parser -> previous = invalidToken(parser);
    parser -> current = lexToken(parser -> lexer);
    parser -> next = invalidToken(parser);
}


/**
 * Creates an invalid token that is used when trying to access a non-existent token.
 *
 * @param       lexer pointer to the lexer.
 *
 * @return      a token indicating invalid access.
 */
static struct Token invalidToken(struct Parser const * const parser) {
    char const * message = "[Parser bug] Invalid access of non-existent token";
    struct Token token;
    token.type = AVL_ERROR;
    token.file = parser -> lexer -> file;
    token.start = message;
    token.length = strlen(message);
    token.line = 0;
    token.column = 0;

    return token;
}
