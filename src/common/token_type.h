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

#ifndef COMMON_TOKEN_TYPE_H
#define COMMON_TOKEN_TYPE_H

enum TokenType {
    /* single-character tokens - these tokens can happen in circumstances where they serve different purposes */
    AVL_DOT = 0,            // .
    AVL_LOGICAL_NOT,        // !  (not)
    AVL_BITWISE_NOT,        // ~  (bnot)
    AVL_BITWISE_OR,         // |  (bor)
    AVL_BITWISE_AND,        // &  (band)
    AVL_BITWISE_XOR,        // ^  (xor)
    AVL_LEFT_PAREN,         // (
    AVL_RIGHT_PAREN,        // )
    AVL_LEFT_BRACE,         // {
    AVL_RIGHT_BRACE,        // }
    AVL_LEFT_BRACKET,       // [
    AVL_RIGHT_BRACKET,      // ]
    AVL_VERTICAL_BAR,       // |
    AVL_UNDERSCORE,         // _
    AVL_PLUS,               // +
    AVL_DIV,                // /
    AVL_MOD,                // %
    AVL_QUOTE,              // '
    AVL_COMMA,              // ,
    AVL_NEWLINE,            // \n
    AVL_INDENT,             // space or tab
    AVL_DEDENT,             // backspace(s)

    /* one or two character tokens - even if we get one character, we must make sure it is not actually part of a two or more character lexeme */
    AVL_EQUAL,              // =
    AVL_EQUAL_EQUAL,        // ==
    AVL_MATCH,              // ===
    AVL_NOT_EQUAL,          // !=
    AVL_NOT_MATCH,          // =!=
    AVL_GREATER,            // >
    AVL_GREATER_EQUAL,      // >=
    AVL_LESS,               // <
    AVL_LESS_EQUAL,         // <=
    AVL_MINUS,              // -
    AVL_RETURN_TYPE,        // ->
    AVL_NS_OPEN,            // -/
    AVL_NS_CLOSE,           // /-
    AVL_LOGICAL_OR,         // || (or)
    AVL_LOGICAL_AND,        // && (and)
    AVL_LEFT_SHIFT,         // << (lsh)
    AVL_RIGHT_SHIFT,        // >> (rsh)
    AVL_COLON,              // :
    AVL_COLON_COLON,        // ::
    AVL_MUL,                // *
    AVL_POW,                // **

    /* literals */
    AVL_IDENTIFIER,         // variable, function or type name
    AVL_INTEGER,            // integers
    AVL_FLOATING_POINT,     // floating point numbers
    AVL_DECIMAL,            // decimal numbers
    AVL_STRING,             // strings
    AVL_BITS,               // binary strings
    AVL_QUBITS,             // qubits

    /* keywords */
    AVL_IMPORT,             // import
    AVL_NAMESPACE,          // namespace
    AVL_PUBLIC,             // public
    AVL_PRIVATE,            // private
    AVL_PTR,                // pointer
    AVL_REF,                // reference
    AVL_DREF,               // dereference
    AVL_CONST_TYPE,         // const
    AVL_TYPE,               // type
    AVL_FUNCTION,           // function
    AVL_VAR,                // var
    AVL_VAL,                // val
    AVL_CAST,               // cast
    AVL_SWITCH,             // switch for pattern matching
    AVL_CASE,               // switch instances
    AVL_DEFAULT,            // default case
    AVL_IF,                 // if
    AVL_ELIF,               // else if
    AVL_ELSE,               // else
    AVL_FOR,                // for
    AVL_EMPTY,              // empty
    AVL_WHILE,              // while
    AVL_CONTINUE,           // continue
    AVL_BREAK,              // break
    AVL_RETURN,             // return
    AVL_PASS,               // pass
    AVL_IN,                 // in
    AVL_NOT_IN,             // not in
    AVL_NEXT_IN,            // next in
    AVL_PREV_IN,            // previously in
    AVL_IS,                 // is
    AVL_IS_NOT,             // is not
    AVL_IS_UNIQUE,          // is unique

    /* specials */
    AVL_EOF,                // end file
    AVL_ERROR               // token representing a lexer error
};

#endif
