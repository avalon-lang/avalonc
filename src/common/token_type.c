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

#include "common/token_type.h"

/**
 * Returns the string representation of a token type
 *
 * @param       type the token type to get the string form of.
 *
 * @return      the string representation of a token type.
 */
char const * tokenTypeToString(enum TokenType type) {
    char const * token_types[] = {
        "DOT",                  // .
        "LOGICAL_NOT",          // !  (not)
        "BITWISE_NOT",          // ~  (bnot)
        "BITWISE_OR",           // |  (bor)
        "BITWISE_AND",          // &  (band)
        "BITWISE_XOR",          // ^  (xor)
        "LEFT_PAREN",           // (
        "RIGHT_PAREN",          // )
        "LEFT_BRACE",           // {
        "RIGHT_BRACE",          // }
        "LEFT_BRACKET",         // [
        "RIGHT_BRACKET",        // ]
        "VERTICAL_BAR",         // |
        "UNDERSCORE",           // _
        "PLUS",                 // +
        "DIV",                  // /
        "MOD",                  // %
        "QUOTE",                // '
        "COMMA",                // ,
        "NEWLINE",              // \n
        "INDENT",               // space or tab
        "DEDENT",               // backspace(s)
        "NO_INDENT",            // current identation is same as previous indentation

        /* one or two character tokens - even if we get one character, we must make sure it is not actually part of a two or more character lexeme */
        "EQUAL",                // =
        "EQUAL_EQUAL",          // ==
        "MATCH",                // ===
        "NOT_EQUAL",            // !=
        "NOT_MATCH",            // =!=
        "GREATER",              // >
        "GREATER_EQUAL",        // >=
        "LESS",                 // <
        "LESS_EQUAL",           // <=
        "MINUS",                // -
        "RETURN_TYPE",          // ->
        "NS_OPEN",              // -/
        "NS_CLOSE",             // /-
        "LOGICAL_OR",           // || (or)
        "LOGICAL_AND",          // && (and)
        "LEFT_SHIFT",           // << (lsh)
        "RIGHT_SHIFT",          // >> (rsh)
        "COLON",                // :
        "COLON_COLON",          // ::
        "MUL",                  // *
        "POW",                  // **

        /* literals */
        "IDENTIFIER",           // variable, function or type name
        "STRING",               // strings
        "CLASSICAL_INT",        // classical integers
        "CLASSICAL_FLOAT",      // classical floating point
        "CLASSICAL_HEX",        // classical hexadecimal
        "CLASSICAL_OCT",        // classical octal
        "CLASSICAL_BIT",        // classical bitstring
        "CLASSICAL_DEC",        // classical decimal
        "QUANTUM_INT",          // quantum integers
        "QUANTUM_FLOAT",        // quantum floating point
        "QUANTUM_HEX",          // quantum hexadecimal
        "QUANTUM_OCT",          // quantum octal
        "QUANTUM_BIT",          // quantum bitstring
        "QUANTUM_DEC",          // quantum decimal

        /* keywords */
        "IMPORT",               // import
        "NAMESPACE",            // namespace
        "PUBLIC",               // public
        "PRIVATE",              // private
        "PTR",                  // pointer
        "REF",                  // reference
        "DREF",                 // dereference
        "CONST",                // const
        "TYPE",                 // type
        "DEF",                  // function
        "VAR",                  // var
        "VAL",                  // val
        "CAST",                 // cast
        "SWITCH",               // switch for pattern matching
        "CASE",                 // switch instances
        "DEFAULT",              // default case
        "IF",                   // if
        "ELIF",                 // else if
        "ELSE",                 // else
        "FOR",                  // for
        "EMPTY",                // empty
        "WHILE",                // while
        "CONTINUE",             // continue
        "BREAK",                // break
        "RETURN",               // return
        "PASS",                 // pass
        "IN",                   // in
        "NEXT",                 // next
        "PREV",                 // prev
        "IS",                   // is
        "UNIQUE",               // unique

        /* specials */
        "EOF",                  // end file
        "ERROR"                 // token representing a lexer error
    };

    return token_types[(int) type];
}
