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

#ifndef UTILS_RESULT_H
#define UTILS_RESULT_H

/**
 * Type of pointer in the returned result.
 */
enum ResultType {
    GENERIC_ERROR,
    COMPILER_ERROR,
    PCHAR_VALUE,
};

/**
 * Struct containing the type of result that was returned from the callee (error or value) and a generic pointer to the actual result.
 */
struct Result {
    enum ResultType type;
    void * data;
};

inline struct Result createResult(enum ResultType type, void * data);


/**
 * Types of errors that may be encountered in a returned result.
 */
enum ErrorType {
    LEXER_ERROR,
    PARSER_ERROR,
    IMPORTER_ERROR,
    CHECKER_ERROR,
    INTERPRETER_ERROR,
    GENERATOR_ERROR
};

/**
 * Struct containing the error and its type.
 */
struct Error {
    enum ErrorType type;
    char * file;
    int line;
    int column;
    int lexeme_length;
    char * message;
};

inline struct Error createError(enum ErrorType type, char * file, int line, int column, int lexeme_length, char * message);

#endif
