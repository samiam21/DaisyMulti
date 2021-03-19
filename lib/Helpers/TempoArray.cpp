#include "TempoArray.h"

/**
 * Add a new duration to the queue
 */
void TempoArray::push(unsigned long duration)
{
    // Add the item into the array
    durationArray[entryIndex] = duration;

    // Increment the pointer
    entryIndex++;

    // Roll back to 0 if we have gone past the end of the array
    if (entryIndex > MAX_ARRAY_SIZE - 1)
    {
        entryIndex = 0;
    }
}

/**
 * Clear the array
 */
void TempoArray::clear()
{
    // Clear all values out of the array
    for (int i = 0; i < MAX_ARRAY_SIZE; i++)
    {
        durationArray[i] = 0;
    }

    // Reset the index pointer
    entryIndex = 0;
}

/**
 * Return the average value of the items in the array
 */
unsigned long TempoArray::average()
{
    int numElements = MAX_ARRAY_SIZE;
    unsigned long average = 0;

    // Iterate through the array and add up all elements (ignore 0s)
    for (int i = 0; i < MAX_ARRAY_SIZE ; i++)
    {
        if (durationArray[i] == 0)
        {
            // Reached the end of the elements, stop adding
            numElements = i;
            break;
        }
        else
        {
            average += durationArray[i];
        }
    }

    // Compute the averate and return
    return (average / (unsigned long)numElements);
}