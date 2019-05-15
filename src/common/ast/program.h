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


#ifndef COMMON_AST_PROGRAM_H
#define COMMON_AST_PROGRAM_H

#include "utils/vector.h"
#include "common/fqn.h"


struct Program {
    /* The FQN represents the fully qualified name of this program and allows us to derive the location of the source program from the file system. */
    struct FQN * fqn;

    /* A vector of all the declarations in this program */
    struct Vector * declarations;

    /* The message to show in case an operation on this structure fails. */
    char const * message;
};


/**
 * Create and allocate memory for a new program and return it.
 *
 * @param       fqn_path the fully qualified path as a string.
 * @param       message error message to display in case any operation on the program fails.
 *
 * @return      the new program declaration.
 */
struct Program * newProgram(char const * fqn_path, char const * message);


/**
 * Free the memory used by a program and associated data structures.
 * It doesn't free the memory occupied by declarations contained in the program. If that is what you wish to do, please use deepDeleteProgram().
 *
 * @param       program the program to free from memory.
 */
void deleteProgram(struct Program ** const program);


/**
 * Free the memory used the program, associated data structures and the memory occupied by all declarations contained within the program.
 *
 * @param       program the program to free from memory including the declarations it contains.
 */
void deepDeleteProgram(struct Program ** const program);


/**
 * Sets the program FQN.
 * We return the old FQN and expect the user to free the memory used by it.
 *
 * @param       program the program which FQN to set.
 * @param       fqn the string rep
 */
struct FQN * setFQN(struct Program * const program, struct FQN * const fqn);


/**
 * Returns this program's FQN.
 *
 * @param       program the program which FQN to set.
 *
 * @return      this program fully qualified name.
 */
struct FQN * getFQN(struct Program const * const program);

#endif
