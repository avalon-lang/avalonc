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
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

#include "utils/stack.h"


/**
 * Initializes the stack
 *
 * @return      the newly created stack.
 */
struct Stack * newStack(size_t initial_size, char const * message) {
    if(initial_size == 0)
        goto exit;

    struct Stack * stack = malloc(sizeof *stack);
    if (stack == NULL)
        goto exit;

    stack -> elements = malloc(initial_size * sizeof *stack -> elements);
    if (stack -> elements == NULL)
        goto exit;
    stack -> size = initial_size;
    stack -> top = 0;
    stack -> message = message;

    return stack;

exit:
    fprintf(stderr, "File: %s.\nLine: %d.\nOperation: newStack.\nMessage: %s\n", __FILE__, __LINE__, message);
    exit(74);
}

struct SizeTStack * newSizeTStack(size_t initial_size, char const * message) {
    if(initial_size == 0)
        goto exit;

    struct SizeTStack * stack = malloc(sizeof *stack);
    if (stack == NULL)
        goto exit;

    stack -> elements =  malloc(initial_size * sizeof *stack -> elements);
    if (stack -> elements == NULL)
        goto exit;
    stack -> size = initial_size;
    stack -> top = 0;
    stack -> message = message;

    return stack;

exit:
    fprintf(stderr, "File: %s.\nLine: %d.\nOperation: newSizeTStack.\nMessage: %s\n", __FILE__, __LINE__, message);
    exit(74);
}


/**
 * Frees the memory occupied by the stack.
 *
 * @param       stack pointer to memory occupied by the stack.
 */
void deleteStack(struct Stack ** const stack) {
    if (stack == NULL)
        return;

    if (* stack == NULL)
        return;

    free((* stack) -> elements);
    free(* stack);
    * stack = NULL;
}

void deleteSizeTStack(struct SizeTStack ** const stack) {
    if (stack == NULL)
        return;

    if (* stack == NULL)
        return;

    free((* stack) -> elements);
    free(* stack);
    * stack = NULL;
}


/**
 * Check if the stack is empty.
 *
 * @param       stack pointer to the stack which content to check.
 *
 * @return      true if the stack is empty, false otherwise.
 */
bool isStackEmpty(struct Stack const * const stack) {
    char const * message = "The parameter <stack> cannot be NULL.";
    if (stack == NULL)
        goto exit;

    return stack -> top == 0;

exit:
    fprintf(stderr, "File: %s.\nLine: %d.\nOperation: isStackEmpty.\nMessage: %s\n", __FILE__, __LINE__, stack ? stack -> message : message);
    exit(74);
}

bool isSizeTStackEmpty(struct SizeTStack const * const stack) {
    char const * message = "The parameter <stack> cannot be NULL.";
    if (stack == NULL)
        goto exit;

    return stack -> top == 0;

exit:
    fprintf(stderr, "File: %s.\nLine: %d.\nOperation: isSizeTStackEmpty.\nMessage: %s\n", __FILE__, __LINE__, stack ? stack -> message : message);
    exit(74);
}


/**
 * Returns the top of the stack.
 *
 * @param       stack pointer to the stack which content to check.
 *
 * @return      the element at the top of the stack.
 */
void * stackTop(struct Stack const * const stack) {
    char const * message = "The parameter <stack> cannot be NULL.";
    if (stack == NULL)
        goto exit;

    return stack -> elements[stack -> top];

exit:
    fprintf(stderr, "File: %s.\nLine: %d.\nOperation: stackTop.\nMessage: %s\n", __FILE__, __LINE__, stack ? stack -> message : message);
    exit(74);
}

size_t sizeTStackTop(struct SizeTStack const * const stack) {
    char const * message = "The parameter <stack> cannot be NULL.";
    if (stack == NULL)
        goto exit;

    return stack -> elements[stack -> top];

exit:
    fprintf(stderr, "File: %s.\nLine: %d.\nOperation: sizeTStackTop.\nMessage: %s\n", __FILE__, __LINE__, stack ? stack -> message : message);
    exit(74);
}


/**
 * Push an element on top of the stack.
 *
 * @param       stack pointer to stack to push an element on.
 * @param       element pointer to the element to push onto the stack.
 */
void stackPush(struct Stack * const stack, void * element) {
    char const * message = "The parameter <stack> cannot be NULL.";
    if (stack == NULL)
        goto exit;

    if (stack -> top == stack -> size) {
        size_t new_size = 1.75 * sizeof(*stack -> elements);
        void ** new_elements = realloc(stack -> elements, new_size * sizeof *stack -> elements);
        if (new_elements == NULL)
            goto exit;

        stack -> elements = new_elements;
        stack -> size = new_size;
    }

    stack -> elements[stack -> top++] = element;

exit:
    fprintf(stderr, "File: %s.\nLine: %d.\nOperation: stackPush.\nMessage: %s\n", __FILE__, __LINE__, stack ? stack -> message : message);
    exit(74);
}

void sizeTStackPush(struct SizeTStack * const stack, const size_t element) {
    char const * message = "The parameter <stack> cannot be NULL.";
    if (stack == NULL)
        goto exit;

    if (stack -> top == stack -> size) {
        size_t new_size = 1.75 * sizeof(*stack -> elements);
        size_t * new_elements = realloc(stack -> elements, new_size * sizeof *stack -> elements);
        if (new_elements == NULL)
            goto exit;

        stack -> elements = new_elements;
        stack -> size = new_size;
    }

    stack -> elements[stack -> top++] = element;

exit:
    fprintf(stderr, "File: %s.\nLine: %d.\nOperation: sizeTStackPush.\nMessage: %s\n", __FILE__, __LINE__, stack ? stack -> message : message);
    exit(74);
}



/**
 * Pop an element off the top of the stack.
 *
 * @param       stack pointer to the stack to pop the element off.
 *
 * @return      pointer to the element popped off the stack top.
 */
void * stackPop(struct Stack * const stack) {
    char const * message = "The parameter <stack> cannot be NULL.";
    if (stack == NULL)
        goto exit;

    if (stack -> top == stack -> size)
        goto exit;

    return stack -> elements[stack -> top--];

exit:
    fprintf(stderr, "File: %s.\nLine: %d.\nOperation: stackPop.\nMessage: %s\n", __FILE__, __LINE__, stack ? stack -> message : message);
    exit(74);
}

size_t sizeTStackPop(struct SizeTStack * const stack) {
    char const * message = "The parameter <stack> cannot be NULL.";
    if (stack == NULL)
        goto exit;

    if (stack -> top == stack -> size)
        goto exit;

    return stack -> elements[stack -> top--];

exit:
    fprintf(stderr, "File: %s.\nLine: %d.\nOperation: sizeTStackPop.\nMessage: %s\n", __FILE__, __LINE__, stack ? stack -> message : message);
    exit(74);
}
