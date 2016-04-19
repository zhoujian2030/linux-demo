/*
 * Sort.h
 *
 *  Created on: Apr 16, 2016
 *      Author: z.j
 */

#ifndef SORT_H
#define SORT_H

namespace ads {

    class Sort {
    public:
        // Insertion Sort (插入排序)
        // Time Complexity: O(n2)
        //   - worst case: 0 + 1 + 2 + ... + (n-2) + (n-1) = n*(n-1)/2 = O(n2)
        //   - best case:  0 + 1 + 1 + ... + 1 + 1 = n-1 = O(n)
        //   - fewer comparisons then selection sort, but more writes - O(n2)
        // Space Complexity: O(1)
        static void insertionSort(int array[], int length);

        // Selection Sort (选择排序)
        // Time Complexity: O(n2)
        //   - all cases: 0 + 1 + 2 + ... + (n-2) + (n-1) = n*(n-1)/2 = O(n2)
        //   - more comparisons then insertion sort, but fewer writes - O(n)
        // Space Complexity: O(1)
        static void selectionSort(int array[], int length);


        static void formatPrint(int array[], int length, int numOfColumn);
        static void generateRandomArray(int array[], int length);
        static void generateSortedArray(int array[], int length, bool ascending = true);
        static void getCurrentTime(long& sec, long& usec);
        
        static void startTimer();
        static void stopTimer();
        static double s_time;
    };
}

#endif