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


static struct Token handleWhitespace(struct Lexer * const lexer, bool is_space);
static void skipWhitespace(struct Lexer * const lexer);
static void skipSingleComment(struct Lexer * const lexer);
static void skipMultiComment(struct Lexer * const lexer);
static bool isAtEnd(struct Lexer const * const lexer);
static char advance(struct Lexer * const lexer);
static bool match(struct Lexer * const lexer, char expected);
static char peek(struct Lexer const * const lexer);
static char peekNext(struct Lexer const * const lexer);
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

        case '\n': {
            struct Token token = makeToken(lexer, AVL_NEWLINE);
            lexer -> line++;
            lexer -> column = 1;
            lexer -> ignore_whitespace = false;
            return token;
        }

        case '\t':
            return handleWhitespace(lexer, false);

        case ' ':
            return handleWhitespace(lexer, true);

        default:
            ;
    }

    return errorToken(lexer, "Unexpected character.");
}


/**
 * Handle blank spaces and tabulations where they are significant.
 *
 * @param       lexer pointer to the lexer.
 * @param       is_space true if we have space, false if we have tabulations.
 *
 * @return      an INDENT or DEDENT token.
 */
static struct Token handleWhitespace(struct Lexer * const lexer, bool is_space) {
    // If we are allowed to ignore whitespace then we cannot handle tabulations here
    if (lexer -> ignore_whitespace == true)
        return errorToken(lexer, is_space ? "Unexpected blank space." : "Unexpected tabulation.");

    // Now we don't consider the previous line to be terminated by a new line
    lexer -> ignore_whitespace = true;

    // If the first indentation was already found and it is a space (or a tabulation), we have an error
    if (lexer -> first_indentation_found) {
        if (is_space == true && lexer -> is_first_indentation_space == false)
            return errorToken(lexer, "Indentation using tabulation is already is effect hence blank space cannot be used for the same.");
        else if (is_space == false && lexer -> is_first_indentation_space == true)
            return errorToken(lexer, "Indentation using blank spaces is already is effect hence tabulation cannot be used for the same.");
    }

    // We read all the blank spaces (tabulations) we can find and count how many there are.
    // This counting serves two purposes: if we have the first indentation, it gives the number that represents the divisor of all future indentation spaces.
    // Second, it tells us if we need to emit a DEDENT or INDENT token.
    size_t whitespace_count = 0;
    if (is_space) {
        while (peek(lexer) == ' ') {
            advance(lexer);
            whitespace_count++;
        }
    }
    else {
        while (peek(lexer) == '\t') {
            advance(lexer);
            whitespace_count++;
        }
    }

    // Figure out if this is the first indentation and if not then this is our first indentation
    if (lexer -> first_indentation_found == false) {
        lexer -> first_indentation_found = true;
        lexer -> is_first_indentation_space = is_space;
        lexer -> last_indentation_count = whitespace_count;
        lexer -> first_indentation_line = lexer -> line;

        lexer -> last_indentation_count = whitespace_count;
        return makeToken(lexer, AVL_INDENT);
    }
    // If this is not our first indentation, we make sure that the number of blank spaces (tabulations) matched is a multiple of the number of blank spaces (tabulations) matched for the first indentation
    else  {
        if (whitespace_count % lexer -> last_indentation_count != 0) {
            if (is_space)
                return errorToken(lexer, "Expected a valid indentation: the number of spaces that form a valid indentation must be a multiple of the number of spaces that form the first indentation.");
            else
                return errorToken(lexer, "Expected a valid indentation: the number of tabulations that form a valid indentation must be a multiple of the number of tabulations that form the first indentation.");
        }

        // We figure out if we must emit a INDENT or DEDENT token
        // If the number of blank spaces (tabulations) found is equal to the number of blank spaces (tabulations) of the last indentation (dedentation), we emit an INDENT token

        printf("Whitespace count = %zu\n", whitespace_count);
        printf("Last indentation count = %zu\n", lexer -> last_indentation_count);
        
        if (whitespace_count == lexer -> last_indentation_count) {
            return makeToken(lexer, AVL_INDENT);
        }
        // If the number of blank spaces (tabulations) found is greater than the number of blank spaces (tabulations) of the last indentation (dedentation), we emit an INDENT token
        else if (whitespace_count > lexer -> last_indentation_count) {
            lexer -> last_indentation_count = whitespace_count;
            return makeToken(lexer, AVL_INDENT);
        }
        // If the number of blank spaces (tabulations) found is less thank the number of blank spaces (tabulations) of the last indentation (dedentation), we emit a DEDENT token
        else {
            lexer -> last_indentation_count = whitespace_count;
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
            // We handle generic whitespace
            case '\r':
            case '\t':
            case ' ':
                advance(lexer);
                break;

            // We treat comments as whitespace and handle them here
            case '-':
                if (peekNext(lexer) == '-') {
                    skipSingleComment(lexer);
                }
                else if (peekNext(lexer) == '[') {
                    // since we are sure we have a multi line comment, we consume the MINUS token in order to avoid clashing with nested comments
                    advance(lexer);
                    skipMultiComment(lexer);
                }
                else {
                    return;
                }

            default:
                return;
        }
    }
}


/**
 * Skips single line comments.
 *
 * @param       lexer pointer to the lexer.
 */
static void skipSingleComment(struct Lexer * const lexer) {
    while (peek(lexer) != '\n' && isAtEnd(lexer) == false)
        advance(lexer);

    // we consume the newline since comments may appear at the beginning of a source
    if (peek(lexer) == '\n' && isAtEnd(lexer) == false) {
        advance(lexer);
        lexer -> line++;
        lexer -> column = 1;
    }
}


/**
 * Skips multiple lines comments.
 *
 * @param       lexer pointer to the lexer.
 */
static void skipMultiComment(struct Lexer * const lexer) {
    size_t levels = 0;
    size_t line = lexer -> line;
    bool terminated = false;

    while (isAtEnd(lexer) == false) {
        // if we have nested comments
        if (peek(lexer) == '-' && peekNext(lexer) == '[') {
            levels++;
        }
        
        // We handle comment closing
        if (peek(lexer) == ']' && peekNext(lexer) == '-') {
            // we make sure to consume the closing characters because the advance() at the end of the loop cannot
            advance(lexer);
            advance(lexer);

            if (levels == 0) {
                terminated = true;
                break;
            }
            else {
                levels--;
            }
        }
        
        // Gracefully handle newlines inside comments
        if (peek(lexer) == '\n') {
            lexer -> line++;
            lexer -> column = 1;
        }

        advance(lexer);
    }

    if (isAtEnd(lexer) && terminated == false)
        fprintf(stderr, "Unterminated multi line comment starting at line %zu.\n", line);

    // we consume the newline since comments may appear at the beginning of a source
    if (peek(lexer) == '\n' && isAtEnd(lexer) == false) {
        advance(lexer);
        lexer -> line++;
        lexer -> column = 1;
    }
}


/**
 * Returns true if we encounter the null byte character which signals the end of the source file.
 *
 * @param       lexer pointer to the lexer.
 *
 * @return      true if we are at the end of the source, false otherwise.
 */
static bool isAtEnd(struct Lexer const * const lexer) {
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
 * Returns the next character to be pointed to by the lexer in the stream.
 *
 * @param       lexer pointer to the lexer.
 *
 * @return      the next character to be pointed to in the source.
 */
static char peekNext(struct Lexer const * const lexer) {
    if (isAtEnd(lexer))
        return '\0';

    return lexer -> current[1];
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
