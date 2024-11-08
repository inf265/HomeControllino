#pragma once

#include "ArxContainer.h"

using std::deque;

#define MEDIAN_MAXVALUES 9

template <class T>
deque<T> sort(deque<T> unsorted)
{
    std::deque<T> sorted;

    for (auto pos = unsorted.begin(); pos != unsorted.end(); pos++)
    {

        if (sorted.size() == 0)
        {
            sorted.push_back(*pos);
            continue;
        }
        bool middleInsert = false;
        for (auto posa = sorted.begin(); posa != sorted.end(); posa++)
        {
            if (*posa > *pos)
            {
                sorted.insert(posa, *pos);
                middleInsert = true;
                break;
            }
        }
        if (!middleInsert)
        {
            sorted.push_back(*pos);
        }
    }
    return sorted;
}

class MedianValue
{
public:
    void add(float value)
    {
        if (values.size() >= MEDIAN_MAXVALUES)
        {
            values.pop_front();
        }
        values.push_back(value);
    }
    float get()
    {
        if (!isReady())
        {
            return 0.0;
        }
        deque<float> sorted = sort(values);
        return sorted[4];
    }
    bool isReady()
    {
        return (values.size() == MEDIAN_MAXVALUES);
    }

private:
    deque<float> values;
};