#ifndef TEMPO_ARRAY_H
#define TEMPO_ARRAY_H

#define MAX_ARRAY_SIZE 6

class TempoArray
{
    public:
        // Add a new duration to the array
        void push(unsigned long duration);

        // Clear the array
        void clear();

        // Return the average value of the items in the array
        unsigned long average();

    private:
        // Internal array for holding durations
        unsigned long durationArray[MAX_ARRAY_SIZE] = {0};

        // Current pointer to where to place the next value into the array
        int entryIndex = 0;
};

#endif