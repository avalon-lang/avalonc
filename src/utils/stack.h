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

#ifndef UTILS_STACK_H
#define UTILS_STACK_H

#include <stdbool.h>
#include <stddef.h>


struct Stack {
    void ** elements;
    size_t size;
    size_t top;
    char const * message;
};

struct SizeTStack {
    size_t * elements;
    size_t size;
    size_t top;
    char const * message;
};


/**
 * Initializes the stack
 *
 * @return      the newly created stack.
 */
struct Stack * newStack(size_t size, char const * message);
struct SizeTStack * newSizeTStack(size_t size, char const * message);


/**
 * Frees the memory occupied by the stack.
 *
 * @param       stack pointer to memory occupied by the stack.
 */
void deleteStack(struct Stack ** stack);
void deleteSizeTStack(struct SizeTStack ** stack);


/**
 * Check if the stack is empty.
 *
 * @param       stack pointer to the stack which content to check.
 *
 * @return      true if the stack is empty, false otherwise.
 */
bool isStackEmpty(struct Stack const * const stack);
bool isSizeTStackEmpty(struct SizeTStack const * const stack);


/**
 * Returns the top of the stack.
 *
 * @param       stack pointer to the stack which content to check.
 *
 * @return      the element at the top of the stack.
 */
void * stackTop(struct Stack const * const stack);
size_t sizeTStackTop(struct SizeTStack const * const stack);


/**
 * Push an element on top of the stack.
 *
 * @param       stack pointer to stack to push an element on.
 * @param       element pointer to the element to push onto the stack.
 */
void stackPush(struct Stack * const stack, void * element);
void sizeTStackPush(struct SizeTStack * const stack, const size_t element);


/**
 * Pop an element off the top of the stack.
 *
 * @param       stack pointer to the stack to pop the element off.
 *
 * @return      the element popped off the stack top.
 */
void * stackPop(struct Stack * const stack);
size_t sizeTStackPop(struct SizeTStack * const stack);

#endif
