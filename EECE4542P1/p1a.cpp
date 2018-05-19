// Project 1a: Solving knapsack using exhaustive search
// Authors: Patricia Gavelek, Emily Simoneau


#include <iostream>
#include <limits.h>
#include <list>
#include <fstream>
#include <queue>
#include <vector>
#include <time.h>
#include <stack>
#include <math.h>

using namespace std;

#include "d_except.h"
#include "d_matrix.h"
#include "knapsack.h"


// prints arrays
void printArray(int arr[], int size) {
    for ( int i = 0; i < size; i++ ) {
        cout << arr[i] << ' ';
    }
    cout << endl;
}

// copy one array into another
void copyArray(int arrs[], int arrd[], int size) {
    for ( int i = 0; i < size; i++ ) {
        arrd[i] = arrs[i];
    }
}

// initialize an array with 0's
void initArray(int arr[], int size) {
    for (int j = 0; j < size; j++)
        arr[j] = 0;
}

//takes in a binary array and updates the selected knapsack items
void arrtoKnapsack(int arr[], knapsack &k) {
    for (int i = 0; i < k.getNumObjects(); i++) {
        arr[i]? k.select(i) : k.unSelect(i);
    }
}

//takes a knapsack and creates an array for selected items
void knapsacktoArr(int arr[], knapsack &k) {
    for (int i = 0; i < k.getNumObjects(); i++) {
        if (k.isSelected(i))
            arr[i] = 1;
        else
            arr[i] = 0;
    }
}


//gets the time in seconds
float gettime(clock_t start, clock_t end)
{
    float diff = end-start;
    float total = diff / CLOCKS_PER_SEC;
    return total;
}

//boolean if time has exceeded limit
bool timeup(clock_t start, clock_t curr, int t)
{
    float total = gettime(start, curr);
    return total > (float) t;
}

//increments a binary array by 1
void binarycounter(int arr[], int size) {
    int i = size-1;
loop:
    if(arr[i] == 0) {
        arr[i] = 1;
    }
    else {
        arr[i] = 0;
        i--;
        goto loop;
    }
}

//helper function which recurses itself over both options for each item
void exhaustivehelper(int i, knapsack &k, int &bestval, int arr[], clock_t start, int t) {
    //if it has reached the last item in list, compare value
    if (i==0 && !timeup(start, clock(), t)) {
        k.select(i);
        if(k.getValue() > bestval && k.getCost() <= k.getCostLimit()){
            bestval = k.getValue();
            knapsacktoArr(arr, k);
        }
        k.unSelect(i);
        if(k.getValue() > bestval && k.getCost() <= k.getCostLimit()) {
            knapsacktoArr(arr, k);
            bestval = k.getValue();
        }
    }
    //recurse 
    else if (!timeup(start, clock(), t)){
        k.select(i);
        exhaustivehelper(i-1, k, bestval, arr, start, t);
        k.unSelect(i);
        exhaustivehelper(i-1, k, bestval, arr, start, t);
    }
}

//brute force but recursive
void exhaustiveKnapsackv2(knapsack &k, int t) {
    int num = k.getNumObjects();
    int bestVal =0;
    int curarray[num];
    initArray(curarray, num);
    clock_t startTime = clock();
    exhaustivehelper(num - 1, k, bestVal, curarray, startTime, t);
    arrtoKnapsack(curarray, k);
    cout << "time: " << gettime(startTime, clock()) << endl;
}

//brute force search for knapsack
//uses a binary number counter to find combinations
void exhaustiveKnapsack(knapsack &k, int t)
{
    int num = k.getNumObjects();
    long long combos = pow(2, num);
    
    int currentV =0, i = 0;
    int curarray[num];
    int array[num];
    initArray(curarray, num);
    initArray(array, num);
    
    clock_t startTime = clock();
    
    while (i < combos - 1 && !timeup(startTime, clock(), t)) {
        
        binarycounter(array, num);
        arrtoKnapsack(array, k);
        
        if (k.getCostLimit()>= k.getCost() && k.getValue() > currentV) {
            currentV = k.getValue();
            copyArray(array, curarray, num);
        }
        
        i++;
    }
    
    arrtoKnapsack(curarray, k);
    cout << "time: " << gettime(startTime, clock()) << endl;
}



int main()
{
    char x;
    ifstream fin;
    stack <int> moves;
    string fileName;
    
    // Read the name of the file from the keyboard or
    // hard code it here for testing.
    
    fileName = "knapsack28.input";
    
    //   cout << "Enter filename" << endl;
    //   cin >> fileName;
    
    fin.open(fileName.c_str());
    if (!fin)
    {
        cerr << "Cannot open " << fileName << endl;
        exit(1);
    }
    
    try
    {
        cout << "Reading knapsack instance" << endl;
        knapsack k(fin);
        
        exhaustiveKnapsack(k, 600);
        
        cout << endl << "Best solution v1" << endl;
        k.printSolution();
        
        exhaustiveKnapsackv2(k, 600);
        
        cout << endl << "Best solution v2" << endl;
        k.printSolution();
        //k.printSolution("knapsack1024.output");
        
    }
    
    catch (indexRangeError &ex)
    {
        cout << ex.what() << endl; exit(1);
    }
    catch (rangeError &ex)
    {
        cout << ex.what() << endl; exit(1);
    }
}





