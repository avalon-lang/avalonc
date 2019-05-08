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
struct Stack * newStack(size_t initial_size, char const * error_message) {
    if(initial_size == 0) {
        fprintf(stderr, "Stack initial size cannot be 0.\n");
        exit(74);
    }

    struct Stack * stack = malloc(sizeof *stack);
    if (stack == NULL) {
        fprintf(stderr, "%s\n", error_message);
        exit(74);
    }

    stack -> elements = malloc(initial_size * sizeof *stack -> elements);
    if (stack -> elements == NULL) {
        fprintf(stderr, "%s\n", error_message);
        exit(74);
    }
    stack -> size = initial_size;
    stack -> top = 0;
    stack -> message = error_message;

    return stack;
}

struct SizeTStack * newSizeTStack(size_t initial_size, char const * error_message) {
    if(initial_size == 0) {
        fprintf(stderr, "SizeTStack initial size cannot be 0.\n");
        exit(74);
    }

    struct SizeTStack * stack = malloc(sizeof *stack);
    if (stack == NULL) {
        fprintf(stderr, "%s\n", error_message);
        exit(74);
    }

    stack -> elements =  malloc(initial_size * sizeof *stack -> elements);
    if (stack -> elements == NULL) {
        fprintf(stderr, "%s\n", error_message);
        exit(74);
    }
    stack -> size = initial_size;
    stack -> top = 0;
    stack -> message = error_message;

    return stack;
}


/**
 * Frees the memory occupied by the stack.
 *
 * @param       stack pointer to memory occupied by the stack.
 */
void deleteStack(struct Stack ** stack) {
    free((* stack) -> elements);
    free(* stack);
    * stack = NULL;
}

void deleteSizeTStack(struct SizeTStack ** stack) {
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
    if (stack == NULL) {
        fprintf(stderr, "[Utils error] Passed a null pointer to isStackEmpty.\n");
        exit(74);
    }

    return stack -> top == 0;
}

bool isSizeTStackEmpty(struct SizeTStack const * const stack) {
    if (stack == NULL) {
        fprintf(stderr, "[Utils error] Passed a null pointer to isSizeTStackEmpty.\n");
        exit(74);
    }

    return stack -> top == 0;
}


/**
 * Returns the top of the stack.
 *
 * @param       stack pointer to the stack which content to check.
 *
 * @return      the element at the top of the stack.
 */
void * stackTop(struct Stack const * const stack) {
    if (stack == NULL) {
        fprintf(stderr, "[Utils error] Passed a null pointer to stackTop.\n");
        exit(74);
    }

    return stack -> elements[stack -> top];
}

size_t sizeTStackTop(struct SizeTStack const * const stack) {
    if (stack == NULL) {
        fprintf(stderr, "[Utils error] Passed a null pointer to sizeTStackTop.\n");
        exit(74);
    }

    return stack -> elements[stack -> top];
}


/**
 * Push an element on top of the stack.
 *
 * @param       stack pointer to stack to push an element on.
 * @param       element pointer to the element to push onto the stack.
 */
void stackPush(struct Stack * const stack, void * element) {
    if (stack == NULL) {
        fprintf(stderr, "[Utils error] Passed a null pointer to stackPush.\n");
        exit(74);
    }

    if (stack -> top == stack -> size) {
        size_t new_size = 1.75 * sizeof(*stack -> elements);
        void * new_elements = realloc(stack -> elements, new_size);
        if (new_elements == NULL) {
            fprintf(stderr, "%s\n", stack -> message);
            exit(74);
        }

        stack -> elements = new_elements;
        stack -> size = new_size;
    }

    stack -> elements[stack -> top++] = element;
}

void sizeTStackPush(struct SizeTStack * const stack, const size_t element) {
    if (stack == NULL) {
        fprintf(stderr, "[Utils error] Passed a null pointer to sizeTStackPush.\n");
        exit(74);
    }

    if (stack -> top == stack -> size) {
        size_t new_size = 1.75 * sizeof(*stack -> elements);
        size_t * new_elements = realloc(stack -> elements, new_size);
        if (new_elements == NULL) {
            fprintf(stderr, "%s\n", stack -> message);
            exit(74);
        }

        stack -> elements = new_elements;
        stack -> size = new_size;
    }

    stack -> elements[stack -> top++] = element;
}



/**
 * Pop an element off the top of the stack.
 *
 * @param       stack pointer to the stack to pop the element off.
 *
 * @return      pointer to the element popped off the stack top.
 */
void * stackPop(struct Stack * const stack) {
    if (stack == NULL) {
        fprintf(stderr, "[Utils error] Passed a null pointer to stackPop.\n");
        exit(74);
    }

    return stack -> elements[stack -> top--];
}

size_t sizeTStackPop(struct SizeTStack * const stack) {
    if (stack == NULL) {
        fprintf(stderr, "[Utils error] Passed a null pointer to sizeTStackPop.\n");
        exit(74);
    }

    return stack -> elements[stack -> top--];
}
