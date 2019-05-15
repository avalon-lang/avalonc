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

#ifndef UTILS_VECTOR_H
#define UTILS_VECTOR_H

#include <stdbool.h>
#include <stddef.h>


struct Vector {
    void ** elements;
    size_t capacity;
    size_t size;
    char const * message;
};


/**
 * Initializes the vector.
 *
 * @return      the newly created vector.
 */
struct Vector * newVector(size_t initial_capacity, char const * message);


/**
 * Frees the memory occupied by the vector.
 *
 * @param       vector pointer to memory occupied by the vector.
 */
void deleteVector(struct Vector ** const vector);


/**
 * Check if the vector is empty.
 *
 * @param       vector pointer to the vector which content to check.
 *
 * @return      true if the vector is empty, false otherwise.
 */
bool isVectorEmpty(struct Vector const * const vector);


/**
 * Return the current number of elements in the vector.
 *
 * @param       vector pointer to the vector.
 *
 * @return      the number of elements in the vector.
 */
size_t vectorSize(struct Vector const * const vector);


/**
 * Resizes the vector to hold less or more elements.
 * Since the vector used memory may grow faster than they are elements in it, for efficiency purposes, this function can be used to reduce memory usage.
 *
 * @param       vector pointer to the vector.
 * @param       new_capacity the new capacity of the vector after resizing.
 */
void vectorResize(struct Vector * const vector, size_t new_capacity);


/**
 * Inserts the given element at the back of the vector.
 *
 * @param       vector pointer to the vector.
 * @param       element element to insert into the vector.
 */
void vectorPushback(struct Vector * const vector, void * element);


/**
 * Inserts the given element at the given position.
 * Fails early if the given position is not within the vector bounds.
 *
 * @param       vector pointer to the vector.
 * @param       position index within the vector where to insert the element.
 * @param       element element to insert into the vector.
 */
void vectorInsert(struct Vector * const vector, size_t position, void * element);


/**
 * Returns the element at the given position.
 * Fails early if the given position is not within the vector bounds.
 *
 * @param       vector pointer to the vector.
 * @param       position index within the vector where to insert the element.
 *
 * @return      element that was found at the given position in the vector.
 */
void * vectorAt(struct Vector const * const vector, size_t position);


/**
 * Deletes the element at the given position and returns it.
 * The vector is not reponsible to free memeory for individual elements it contains since it didn't allocate their memory to begin with.
 * It is the reponsibility of the caller to free the memory occupied by said elements so we return the element that was eliminated from vector.
 *
 * @param       vector pointer to the vector.
 * @param       position index within the vector which to delete.
 *
 * @return      element that was found at the given position in the vector.
 */
void * vectorErase(struct Vector * const vector, size_t position);


/**
 * Returns the last element in the vector and removes it from the vector.
 * The vector is not reponsible to free memeory for individual elements it contains since it didn't allocate their memory to begin with.
 * It is the reponsibility of the caller to free the memory occupied by said elements so we return the element that was eliminated from vector.
 *
 * @param       vector pointer to the vector.
 * @param       position index within the vector which to delete.
 *
 * @return      the last element in the vector.
 */
void * vectorPopback(struct Vector * const vector);

#endif
