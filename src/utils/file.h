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

#ifndef UTILS_FILE_H
#define UTILS_FILE_H

#include <stdbool.h>


/**
 * Verify if a file exists given the path to the file. Returns true if it does, false otherwise.
 */
bool fileExists(char const * path);


/**
 * Reads the content of the file at the given path and returns a pointer to said content.
 */
char * readFile(char const * path);

#endif
