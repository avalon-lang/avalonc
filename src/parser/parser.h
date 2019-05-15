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

#ifndef PARSER_H
#define PARSER_H

#include "common/ast/program.h"
#include "common/token.h"
#include "lexer/lexer.h"


struct Parser {
    struct Lexer * lexer;
    struct Program * program;

    struct Token previous;
    struct Token current;
    struct Token next;

    char * namespace;
};


/**
 * Initializes the parser and returns it, ready to parse.
 *
 * @param       program the program that is to hold all the declarations found.
 * @param       lexer the lexer to give us tokens
 *
 * @return      the parser that is was initialized.
 */
struct Parser * newParser(struct Lexer * const lexer, struct Program * const program);


/**
 * Frees the memory allocated for the parser.
 *
 * @param       parser the parser which memory to free.
 */
void deleteParser(struct Parser ** const parser);


/**
 * The parser entry point. It receives tokens from the lexer and determines what to do with them.
 *
 * @param       parser the parser with information needed for parsing.
 *
 * @return      the program with all the declarations set.
 */
struct Program * parse(struct Parser * const parser);

#endif
