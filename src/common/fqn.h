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


#ifndef COMMON_FQN_H
#define COMMON_FQN_H

struct FQN {
    char * name;
    char * path;
};


/**
 * Given the fully qualified name of a program, create an FQN struct from which we can retrieve both the name and the path of the program.
 *
 * @param       name fully qualified name.
 * @param       message error message to be displayed in case a non-recoverable error occurs such failed memory allocation.
 *
 * @return      FQN from which we can retrieve the name and path.
 */
struct FQN * fqnFromName(char const * name, char const * message);


/**
 * Given the path to a program, create an FQN struct from which we can retrieve both the name and the path of the program.
 *
 * @param       path file system path to the program.
 * @param       message error message to be displayed in case a non-recoverable error occurs such failed memory allocation.
 *
 * @return      FQN from which we can retrieve the name and path.
 */
struct FQN * fqnFromPath(char const * path, char const * message);


/**
 * Given an FQN, free the memory occupied by it and all associated structures.
 *
 * @param       FQN FQN struct which memory it occupies must be freed.
 */
void deleteFQN(struct FQN ** const fqn);


/**
 * Given the FQN struct, return the string representation of its name.
 *
 * @param       fqn struct FQN from which to retrieve the name.
 *
 * @return      the name of the FQN.
 */
char const * fqnName(struct FQN const * const fqn);


/**
 * Given the FQN struct, return the string representation of its path.
 *
 * @param       fqn struct FQN from which to retrieve the path.
 *
 * @return      the path of the FQN.
 */
char const * fqnPath(struct FQN const * const fqn);

#endif
