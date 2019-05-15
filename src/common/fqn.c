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
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>

#include "common/fqn.h"

static bool isAlpha(char c);
static bool isDot(char c);
static bool isSlash(char c);
static bool isValidName(char const * name);
static bool isValidPath(char const * path);
static void nameFromPath(char * name, char const * path);
static void pathFromName(char * path, char const * name);


/**
 * Given the fully qualified name of a program, create an FQN struct from which we can retrieve both the name and the path of the program.
 *
 * @param       name fully qualified name
 *
 * @return      FQN from which we can retrieve the name and path.
 */
struct FQN * fqnFromName(char const * name, char const * message) {
    // We make sure that the name we are given is a valid FQN
    if (isValidName(name) == false)
        goto exit;

    // We proceed to allocate create the FQN struct
    struct FQN * fqn = malloc(sizeof *fqn);
    if (fqn == NULL)
        goto exit;

    // Set up the FQN struct fields
    size_t length = strlen(name);

    fqn -> name = malloc((1 + length) * sizeof *fqn -> name);
    if (fqn -> name == NULL)
        goto exit;
    memcpy(fqn -> name, name, 1 + length);

    fqn -> path = malloc((1 + length) * sizeof *fqn -> path);
    if (fqn -> path == NULL)
        goto exit;
    pathFromName(fqn -> path, name);

exit:
    fprintf(stderr, "%s\n", message);
    exit(74);
}


/**
 * Given the path to a program, create an FQN struct from which we can retrieve both the name and the path of the program.
 *
 * @param       path file system path to the program.
 *
 * @return      FQN from which we can retrieve the name and path.
 */
struct FQN * fqnFromPath(char const * path, char const * message) {
    // We make sure that the path we are given is a valid path for FQN
    if (isValidPath(path) == false)
        goto exit;

    // We proceed to allocate create the FQN struct
    struct FQN * fqn = malloc(sizeof *fqn);
    if (fqn == NULL)
        goto exit;

    // Set up the FQN struct fields
    size_t length = strlen(path);

    fqn -> name = malloc((1 + length) * sizeof *fqn -> name);
    if (fqn -> name == NULL)
        goto exit;
    nameFromPath(fqn -> name, path);

    fqn -> path = malloc((1 + length) * sizeof *fqn -> path);
    if (fqn -> path == NULL)
        goto exit;
    memcpy(fqn -> path, path, 1 + length);

exit:
    fprintf(stderr, "%s\n", message);
    exit(74);
}


/**
 * Given an FQN, free the memory occupied by it and all associated structures.
 *
 * @param       FQN FQN struct which memory it occupies must be freed.
 */
void deleteFQN(struct FQN ** const fqn) {
    if (fqn == NULL)
        return;

    if (* fqn == NULL)
        return;

    free(* fqn);
    * fqn = NULL;
}


/**
 * Given the FQN struct, return the string representation of its name.
 *
 * @param       fqn struct FQN from which to retrieve the name.
 *
 * @return      the name of the FQN.
 */
char const * fqnName(struct FQN const * const fqn) {
    return fqn -> name;
}


/**
 * Given the FQN struct, return the string representation of its path.
 *
 * @param       fqn struct FQN from which to retrieve the path.
 *
 * @return      the path of the FQN.
 */
char const * fqnPath(struct FQN const * const fqn) {
    return fqn -> path;
}


/**
 * Returns true if the given character can be used as a valid indentifier and therefore can appear as part of an import statement.
 *
 * @param       c the character to validate.
 *
 * @return      true if the character is made of the English alphabet or an underscore, false otherwise.
 */
static bool isAlpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}


/**
 * Returns true if the current character is a dot.
 *
 * @param       c the character to validate.
 *
 * @return      true if the given character is a dot.
 */
static bool isDot(char c) {
    return c == '.';
}


/**
 * Returns true if we have a slash character. This is a valid character to appear in a file path as directory separator.
 *
 * @param       c the character to validate.
 *
 * @return      true if the given character is a frontslash or a backlash.
 */
static bool isSlash(char c) {
    return c == '/' || c == '\\';
}


/**
 * Returns true of the given FQN name is a valid name.
 *
 * @param       name string representation of a fully qualified name.
 *
 * @return      true of the given name contains only letters, underscores and dots.
 */
static bool isValidName(char const * name) {
    size_t i = 0;
    size_t length = strlen(name);

    while (i < length) {
        char c = name[i];

        if (isAlpha(c) == false && isDot(c) == false)
            return false;

        i++;
    }

    return true;
}


/**
 * Returns true of the given filesystem path is a valid FQN path.
 *
 * @param       name string representation of filesystem path.
 *
 * @return      true of the given name contains only letters, underscores and slashes.
 */
static bool isValidPath(char const * path) {
    size_t i = 0;
    size_t length = strlen(path);

    while (i < length) {
        char c = path[i];

        if (isAlpha(c) == false && isSlash(c) == false)
            return false;

        i++;
    }

    return true;
}


/**
 * Given a fully qualified path, create a string representation of its name.
 *
 * @param       name the fully qualified name string representation.
 * @param       path the fully qualified path string representation.
 */
static void nameFromPath(char * name, char const * path) {
    size_t i = 0;
    size_t length = strlen(path);

    while (i < length) {
        char c = path[i];
        
        if (isSlash(c))
            name[i] = '.';
        else
            name[i] = c;

        i++;
    }

    name[length] = '\0';
}


/**
 * Given a fully qualified name, create a system path from it and set it on the given destination path string.
 *
 * @param       path the fully qualified path string representation.
 * @param       name the fully qualified name string representation.
 */
static void pathFromName(char * path, char const * name) {
    size_t i = 0;
    size_t length = strlen(name);

    while (i < length) {
        char c = name[i];
        
        if (isDot(c))
            path[i] = '/'; // Windows accepts '/' at the API level so we don't have to worry about differentiating frontslashes from backslashes
        else
            path[i] = c;

        i++;
    }

    path[length] = '\0';
}
