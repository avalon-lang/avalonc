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

#include "utils/vector.h"


/**
 * Initializes the vector.
 *
 * @return      the newly created vector.
 */
struct Vector * newVector(size_t initial_capacity, char const * message) {
    if(initial_capacity == 0)
        goto exit;

    struct Vector * vector = malloc(sizeof *vector);
    if (vector == NULL)
        goto exit;

    vector -> elements = malloc(initial_capacity * sizeof(void *));
    if (vector -> elements == NULL)
        goto exit;
    vector -> capacity = initial_capacity;
    vector -> size = 0;
    vector -> message = message;

    return vector;

exit:
    fprintf(stderr, "File: %s.\nLine: %d.\nOperation: newVector.\nMessage: %s\n", __FILE__, __LINE__, message);
    exit(74);
}


/**
 * Frees the memory occupied by the vector.
 *
 * @param       vector pointer to memory occupied by the vector.
 */
void deleteVector(struct Vector ** const vector) {
    if (vector == NULL)
        return;

    if (* vector == NULL)
        return;

    free((* vector) -> elements);
    free(* vector);
    * vector = NULL;
}


/**
 * Check if the vector is empty.
 *
 * @param       vector pointer to the vector which content to check.
 *
 * @return      true if the vector is empty, false otherwise.
 */
bool isVectorEmpty(struct Vector const * const vector) {
    if (vector == NULL)
        goto exit;

    return vector -> size == 0;

exit:
    fprintf(stderr, "File: %s.\nLine: %d.\nOperation: isVectorEmpty.\nMessage: %s\n", __FILE__, __LINE__, vector -> message);
    exit(74);
}


/**
 * Return the current number of elements in the vector.
 *
 * @param       vector pointer to the vector.
 *
 * @return      the number of elements in the vector.
 */
size_t vectorSize(struct Vector const * const vector) {
    if (vector == NULL)
        goto exit;

    return vector -> size;

exit:
    fprintf(stderr, "File: %s.\nLine: %d.\nOperation: vectorSize.\nMessage: %s\n", __FILE__, __LINE__, vector -> message);
    exit(74);
}


/**
 * Resizes the vector to hold less or more elements.
 * Since the vector used memory may grow faster than they are elements in it, for efficiency purposes, this function can be used to reduce memory usage.
 *
 * @param       vector pointer to the vector.
 * @param       new_capacity the new capacity of the vector after resizing.
 */
void vectorResize(struct Vector * const vector, size_t new_capacity) {
    if (vector == NULL)
        goto exit;

    // For correctness purposes, we require that the new capacity be at least equal to current size of the vector
    if (new_capacity < vector -> size)
        goto exit;

    // We try to reallocate and if we fail, we inform the client code by early termination
    void ** elements = realloc(vector -> elements, new_capacity * sizeof *vector -> elements);
    if (elements == NULL)
        goto exit;

    // Update the vector elements memory location and the new capacity
    vector -> elements = elements;
    vector -> capacity = new_capacity;

    // We return in order not to fall through the exit label below
    return;

exit:
    fprintf(stderr, "File: %s.\nLine: %d.\nOperation: vectorResize.\nMessage: %s\n", __FILE__, __LINE__, vector -> message);
    exit(74);
}


/**
 * Inserts the given element at the back of the vector.
 *
 * @param       vector pointer to the vector.
 * @param       element element to insert into the vector.
 */
void vectorPushback(struct Vector * const vector, void * element) {
    if (vector == NULL)
        goto exit;

    if (vector -> capacity == vector -> size)
        vectorResize(vector, 2);

    vector -> elements[vector -> size++] = element;
    return;

exit:
    fprintf(stderr, "File: %s.\nLine: %d.\nOperation: vectorPushback.\nMessage: %s\n", __FILE__, __LINE__, vector -> message);
    exit(74);
}


/**
 * Inserts the given element at the given position.
 * Fails early if the given position is not within the vector bounds.
 *
 * @param       vector pointer to the vector.
 * @param       position index within the vector where to insert the element.
 * @param       element element to insert into the vector.
 */
void vectorInsert(struct Vector * const vector, size_t position, void * element) {
    if (vector == NULL)
        goto exit;

    // If the vector is empty, we expect the client to push back into the vector and not use insert
    if (vector -> size == 0)
        goto exit;

    // If the position is not within the array bounds, we fail early
    if (position > vector -> size)
        goto exit;

    vector -> elements[position] = element;
    return;

exit:
    fprintf(stderr, "File: %s.\nLine: %d.\nOperation: vectorInsert.\nMessage: %s\n", __FILE__, __LINE__, vector -> message);
    exit(74);
}


/**
 * Returns the element at the given position.
 * Fails early if the given position is not within the vector bounds.
 *
 * @param       vector pointer to the vector.
 * @param       position index within the vector where to insert the element.
 *
 * @return      element that was found at the given position in the vector.
 */
void * vectorAt(struct Vector const * const vector, size_t position) {
    if (vector == NULL)
        goto exit;

    // If the vector is empty, we fail early
    if (vector -> size == 0)
        goto exit;

    // If the position is not within the array bounds, we fail early
    if (position > vector -> size)
        goto exit;

    return vector -> elements[position];

exit:
    fprintf(stderr, "File: %s.\nLine: %d.\nOperation: vectorAt.\nMessage: %s\n", __FILE__, __LINE__, vector -> message);
    exit(74);
}


/**
 * Deletes the element at the given position and returns it.
 * The vector is not reponsible for free memeory for elements it contains since it didn't allocate their memory to begin with.
 * It is the reponsibility of the caller to free the memory occupied by said elements.
 *
 * @param       vector pointer to the vector.
 * @param       position index within the vector which to delete.
 *
 * @return      element that was found at the given position in the vector.
 */
void * vectorErase(struct Vector * const vector, size_t position) {
    if (vector == NULL)
        goto exit;

    // If the vector is empty, we fail early
    if (vector -> size == 0)
        goto exit;

    // If the position is not within the array bounds, we fail early
    if (position > vector -> size)
        goto exit;

    void * element = vector -> elements[position];
    vector -> elements[position] = NULL;
    vector -> size--;

    for (size_t i = position; i < vector -> size - 1; ++i) {
        vector -> elements[i] = vector -> elements[i + 1];
        vector -> elements[i + 1] = NULL;
    }

    return element;

exit:
    fprintf(stderr, "File: %s.\nLine: %d.\nOperation: vectorErase.\nMessage: %s\n", __FILE__, __LINE__, vector -> message);
    exit(74);
}


/**
 * Returns the last element in the vector and removes it from the vector.
 * The vector is not reponsible to free memeory for individual elements it contains since it didn't allocate their memory to begin with.
 * It is the reponsibility of the caller to free the memory occupied by said elements so we return the element that was eliminated from vector.
 *
 * @param       vector pointer to the vector.
 *
 * @return      the last element in the vector.
 */
void * vectorPopback(struct Vector * const vector) {
    if (vector == NULL)
        goto exit;

    // If the vector is empty, we fail early
    if (vector -> size == 0)
        goto exit;

    void * element = vector -> elements[vector -> size];
    vector -> elements[vector -> size] = NULL;
    vector -> size--;
    return element;

exit:
    fprintf(stderr, "File: %s.\nLine: %d.\nOperation: vectorPopback.\nMessage: %s\n", __FILE__, __LINE__, vector -> message);
    exit(74);

}

