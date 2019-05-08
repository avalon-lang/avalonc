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
#include "lexer/lexer.h"


static struct Token handleSpace(struct Lexer * const lexer);
static struct Token handleTabulation(struct Lexer * const lexer);
static void skipWhitespace(struct Lexer * const lexer);
static bool isAtEnd(struct Lexer * const lexer);
static char advance(struct Lexer * const lexer);
static bool match(struct Lexer * const lexer, char expected);
static char peek(struct Lexer const * const lexer);
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

    lexer -> ignore_whitespace = true;
    lexer -> first_indentation_found = false;
    lexer -> is_first_indentation_space = false;
    lexer -> first_indentation_line = 0;
    lexer -> last_indentation_count = 0;

    return lexer;
}


/**
 * Frees the memory occupied by the lexer and associated structures.
 *
 * @param       lexer pointer to pointer to the memory occupied by the lexer.
 */
void deleteLexer(struct Lexer ** lexer) {
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
    // Before lexing the next token, we make sure to skip any unnecessary whitespace if we are allowed to.
    if (lexer -> ignore_whitespace == true)
        skipWhitespace(lexer);

    lexer -> start = lexer -> current;

    if (isAtEnd(lexer))
        return makeToken(lexer, AVL_EOF);
    
    char c = advance(lexer);
    switch (c) {
        case '.':
            return makeToken(lexer, AVL_DOT);

        case '!':
            return makeToken(lexer, match(lexer, '=') ? AVL_NOT_EQUAL : AVL_LOGICAL_NOT);

        case '~':
            return makeToken(lexer, AVL_BITWISE_NOT);

        case '^':
            return makeToken(lexer, AVL_BITWISE_XOR);

        case '+':
            return makeToken(lexer, AVL_PLUS);

        case '-':
            // if (match(lexer, '-')) return comment(lexer);
            // if (match(lexer, '[')) return multiComment(lexer);
            if (match(lexer, '>')) return makeToken(lexer, AVL_RETURN_TYPE);
            if (match(lexer, '/')) return makeToken(lexer, AVL_NS_OPEN);
            return makeToken(lexer, AVL_MINUS);

        case '*':
            return makeToken(lexer, match(lexer, '*') ? AVL_POW : AVL_MUL);

        case '/':
            return makeToken(lexer, match(lexer, '-') ? AVL_NS_CLOSE : AVL_MINUS);

        case '%':
            return makeToken(lexer, AVL_MOD);

        case '\'':
            return makeToken(lexer, AVL_QUOTE);

        case '"':
            return errorToken(lexer, "Strings are not currently supported.");

        case ',':
            return makeToken(lexer, AVL_COMMA);

        case ':':
            return makeToken(lexer, AVL_COLON);

        case '=':
            if (match(lexer, '=')) return match(lexer, '=') ? makeToken(lexer, AVL_MATCH) : makeToken(lexer, AVL_EQUAL_EQUAL);
            if (match(lexer, '!')) return match(lexer, '=') ? makeToken(lexer, AVL_NOT_MATCH) : errorToken(lexer, "Unterminated match operator: expected =!= but found =!.");
            return makeToken(lexer, AVL_EQUAL);

        case '<':
            if (match(lexer, '=')) return makeToken(lexer, AVL_LESS_EQUAL);
            if (match(lexer, '<')) return makeToken(lexer, AVL_LEFT_SHIFT);
            if (match(lexer, '>')) return makeToken(lexer, AVL_NOT_EQUAL);
            return makeToken(lexer, AVL_LESS);

        case '>':
            if (match(lexer, '=')) return makeToken(lexer, AVL_GREATER_EQUAL);
            if (match(lexer, '>')) return makeToken(lexer, AVL_RIGHT_SHIFT);
            return makeToken(lexer, AVL_GREATER);

        case '|':
            return makeToken(lexer, match(lexer, '|') ? AVL_LOGICAL_OR : AVL_VERTICAL_BAR);

        case '&':
            return makeToken(lexer, match(lexer, '&') ? AVL_LOGICAL_AND : AVL_BITWISE_AND);

        case '(':
            return makeToken(lexer, AVL_LEFT_PAREN);

        case ')':
            return makeToken(lexer, AVL_RIGHT_PAREN);

        case '[':
            return makeToken(lexer, AVL_LEFT_BRACKET);

        case ']':
            return makeToken(lexer, AVL_RIGHT_PAREN);

        case '{':
            return makeToken(lexer, AVL_LEFT_BRACE);

        case '}':
            return makeToken(lexer, AVL_RIGHT_BRACE);

        // TODO: Handle identifiers
        case '_':
            return makeToken(lexer, AVL_UNDERSCORE);

        case '\n':
            lexer -> line++;
            lexer -> column = 1;
            lexer -> ignore_whitespace = false;
            return makeToken(lexer, AVL_NEWLINE);

        case '\t':
            return handleTabulation(lexer);

        case ' ':
            return handleSpace(lexer);

        default:
            ;
    }

    return errorToken(lexer, "Unexpected character.");
}


/**
 * When we accounter a blank space, we need to figure out whether to emit an INDENT token or a DEDENT token.
 *
 * @param       lexer pointer to the lexer.
 *
 * @return      an INDENT or a DEDENT token.
 */
static struct Token handleSpace(struct Lexer * const lexer) {
    // If we are allowed to ignore whitespace then we cannot handle blank spaces here
    if (lexer -> ignore_whitespace == true)
        return errorToken(lexer, "Unexpected blank space.");

    // Now we don't consider the previous line to be terminated by a new line
    lexer -> ignore_whitespace = true;

    // If the first indentation was already found and it is not a space (but a tabulation), we have an error
    if (lexer -> first_indentation_found && lexer -> is_first_indentation_space == false)
        return errorToken(lexer, "Indentation using tabulation is already is effect hence blank space cannot be used for the same.");

    // We read all the blank spaces we can find and count how many there are.
    // This counting serves two purposes: if we have the first indentation, it gives the number that represents the divisor of all future indentation spaces.
    // Second, it tells us if we need to emit a DEDENT or INDENT token.
    size_t spaces_count = 0;
    for (;;) {
        char c = peek(lexer);
        if (c == ' ') {
            advance(lexer);
            spaces_count++;
        }
    }

    // Figure out if this is the first indentation and if not then this is our first indentation
    if (lexer -> first_indentation_found == false) {
        lexer -> first_indentation_found = true;
        lexer -> is_first_indentation_space = true;
        lexer -> last_indentation_count = spaces_count;
        lexer -> first_indentation_line = lexer -> line;

        lexer -> last_indentation_count = spaces_count;
        return makeToken(lexer, AVL_INDENT);
    }
    // If this is not our first indentation, we make sure that the number of spaces matched is a multiple of the number of spaces matched for the first indentation
    else  {
        if (spaces_count % lexer -> last_indentation_count != 0)
            return errorToken(lexer, "Expected a valid indentation: the number of spaces that form a valid indentation must be a multiple of the number of spaces that form the first indentation.");

        // We figure out if we must emit a INDENT or DEDENT token
        // If the number of spaces found is equal to the number of spaces of the last indentation (dedentation), we emit an INDENT token
        if (spaces_count == lexer -> last_indentation_count) {
            return makeToken(lexer, AVL_INDENT);
        }
        // If the number of spaces found is greater than the number of spaces of the last indentation (dedentation), we emit an INDENT token
        else if (spaces_count > lexer -> last_indentation_count) {
            lexer -> last_indentation_count = spaces_count;
            return makeToken(lexer, AVL_INDENT);
        }
        // If the number of spaces found is less thank the number of spaces of the last indentation (dedentation), we emit a DEDENT token
        else {
            lexer -> last_indentation_count = spaces_count;
            return makeToken(lexer, AVL_DEDENT);
        }
    }
}


/**
 * When we accounter a tabulation, we need to figure out whether to emit an INDENT token or a DEDENT token.
 *
 * @param       lexer pointer to the lexer.
 *
 * @return      an INDENT or a DEDENT token.
 */
static struct Token handleTabulation(struct Lexer * const lexer) {
    // If we are allowed to ignore whitespace then we cannot handle tabulations here
    if (lexer -> ignore_whitespace == true)
        return errorToken(lexer, "Unexpected tabulation.");

    // Now we don't consider the previous line to be terminated by a new line
    lexer -> ignore_whitespace = true;

    // If the first indentation was already found and it is a space (but not a tabulation), we have an error
    if (lexer -> first_indentation_found && lexer -> is_first_indentation_space == true)
        return errorToken(lexer, "Indentation using blank spaces is already is effect hence tabulation cannot be used for the same.");

    // We read all the tabulations we can find and count how many there are.
    // This counting serves two purposes: if we have the first indentation, it gives the number that represents the divisor of all future indentation spaces.
    // Second, it tells us if we need to emit a DEDENT or INDENT token.
    size_t tabs_count = 0;
    for (;;) {
        char c = peek(lexer);
        if (c == '\t') {
            advance(lexer);
            tabs_count++;
        }
    }

    // Figure out if this is the first indentation and if not then this is our first indentation
    if (lexer -> first_indentation_found == false) {
        lexer -> first_indentation_found = true;
        lexer -> is_first_indentation_space = false;
        lexer -> last_indentation_count = tabs_count;
        lexer -> first_indentation_line = lexer -> line;

        lexer -> last_indentation_count = tabs_count;
        return makeToken(lexer, AVL_INDENT);
    }
    // If this is not our first indentation, we make sure that the number of tabulations matched is a multiple of the number of tabulations matched for the first indentation
    else  {
        if (tabs_count % lexer -> last_indentation_count != 0)
            return errorToken(lexer, "Expected a valid indentation: the number of tabulations that form a valid indentation must be a multiple of the number of tabulations that form the first indentation.");

        // We figure out if we must emit a INDENT or DEDENT token
        // If the number of tabulations found is equal to the number of tabulations of the last indentation (dedentation), we emit an INDENT token
        if (tabs_count == lexer -> last_indentation_count) {
            return makeToken(lexer, AVL_INDENT);
        }
        // If the number of tabulations found is greater than the number of tabulations of the last indentation (dedentation), we emit an INDENT token
        else if (tabs_count > lexer -> last_indentation_count) {
            lexer -> last_indentation_count = tabs_count;
            return makeToken(lexer, AVL_INDENT);
        }
        // If the number of tabulations found is less thank the number of tabulations of the last indentation (dedentation), we emit a DEDENT token
        else {
            lexer -> last_indentation_count = tabs_count;
            return makeToken(lexer, AVL_DEDENT);
        }
    }
}


/**
 * Ignores whitespace when appropriate. Whitespace includes: spaces, tabulations and carriage returns.
 *
 * @param       lexer pointer to the lexer.
 */
static void skipWhitespace(struct Lexer * const lexer) {
    for (;;) {
        char c = peek(lexer);
        switch (c) {
            case '\r':
            case '\t':
            case ' ':
                advance(lexer);
                break;

            default:
                return;
        }
    }
}


/**
 * Returns true if we encounter the null byte character which signals the end of the source file.
 *
 * @param       lexer pointer to the lexer.
 *
 * @return      true if we are at the end of the source, false otherwise.
 */
static bool isAtEnd(struct Lexer * const lexer) {
    return * lexer -> current == '\0';
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
    lexer -> column++;
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

    lexer -> column++;
    lexer -> current++;
    return true;
}


/**
 * Returns the current character pointed to by the lexer in the stream.
 *
 * @param       lexer pointer to the lexer.
 *
 * @return      the current character in the source.
 */
static char peek(struct Lexer const * const lexer) {
    return * lexer -> current;
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
