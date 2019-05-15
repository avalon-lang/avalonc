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


#include <stdlib.h>
#include <stdio.h>

#include "common/ast/program.h"


/**
 * Create and allocate memory for a new program and return it.
 *
 * @param       fqn the fully qualified path as a string.
 * @param       message error message to display in case any operation on the program fails.
 *
 * @return      the new program declaration.
 */
struct Program * newProgram(char const * fqn_path, char const * message) {
    struct Program * program = malloc(sizeof *program);
    if (program == NULL)
        goto exit;

    program -> fqn = fqnFromPath(fqn_path, message);
    program -> declarations = newVector(50, message);
    program -> message = message;
    return program;

exit:
    fprintf(stderr, "File: %s.\nLine: %d.\nOperation: newProgram.\nMessage: %s\n", __FILE__, __LINE__, message);
    exit(74);
}


/**
 * Free the memory used by a program and associated data structures.
 * It doesn't free the memory occupied by declarations contained in the program. If that is what you wish to do, please use deepDeleteProgram().
 *
 * @param       program the program to free from memory.
 */
void deleteProgram(struct Program ** const program) {
    if (program == NULL)
        return;

    if (* program == NULL)
        return;

    deleteFQN(& (* program) -> fqn);
    deleteVector(& (* program) -> declarations);
    free(* program);
    * program = NULL;
}


/**
 * Free the memory used the program, associated data structures and the memory occupied by all declarations contained within the program.
 *
 * @param       program the program to free from memory including the declarations it contains.
 */
void deepDeleteProgram(struct Program ** const program) {
    if (program == NULL)
        return;

    if (* program == NULL)
        return;

    // Before deleting declarations, we make sure to free the memory held by each individual declaration
    size_t declarations_count = vectorSize((* program) -> declarations);
    while (declarations_count > 0)
        ;

    deleteFQN(& (* program) -> fqn);
    deleteVector(& (* program) -> declarations);
    free(* program);
    * program = NULL;
}


/**
 * Sets the program FQN.
 * We return the old FQN and expect the user to free the memory used by it.
 *
 * @param       program the program which FQN to set.
 * @param       fqn the string rep
 */
struct FQN * setFQN(struct Program * program, struct FQN * const new_fqn) {
    char const * message = "The parameter <program> cannot be a null pointer";
    if (program == NULL)
        goto exit;

    struct FQN * old_fqn = program -> fqn;
    program -> fqn = new_fqn;
    return old_fqn;

exit:
    fprintf(stderr, "File: %s.\nLine: %d.\nOperation: setFQN.\nMessage: %s\n", __FILE__, __LINE__, program ? program -> message : message);
    exit(74);
}


/**
 * Returns this program's FQN.
 *
 * @param       program the program which FQN to set.
 *
 * @return      this program fully qualified name.
 */
struct FQN * getFQN(struct Program * program) {
    char const * message = "The parameter <program> cannot be a null pointer";
    if (program == NULL)
        goto exit;

    return program -> fqn;

exit:
    fprintf(stderr, "File: %s.\nLine: %d.\nOperation: setFQN.\nMessage: %s\n", __FILE__, __LINE__, program ? program -> message : message);
    exit(74);
}
