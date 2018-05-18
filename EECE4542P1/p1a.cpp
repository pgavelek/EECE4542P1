// Project 1a: Solving knapsack using exhaustive search
//

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



void printArray(int arr[], int size) {
    for ( int i = 0; i < size; i++ ) {
        cout << arr[i] << ' ';
    }
    cout << endl;
}

void copyArray(int arrs[], int arrd[], int size) {
    for ( int i = 0; i < size; i++ ) {
        arrd[i] = arrs[i];
    }
}

void initArray(int arr[], int size) {
    for (int j = 0; j < size; j++)
        arr[j] = 0;
}

void arrtoKnapsack(int arr[], knapsack &k) {
    for (int i = 0; i < k.getNumObjects(); i++) {
        arr[i]? k.select(i) : k.unSelect(i);
    }
    
}

float gettime(clock_t start, clock_t end)
{
    
    float diff = end-start;
    float total = diff / CLOCKS_PER_SEC;
    
    return total;
}

bool timeup(clock_t start, clock_t curr, int t)
{
    float total = gettime(start, curr);
    
    return total > (float) t;
}

void binarycounter(int arr[], int size) {
    int l = size-1;
h:
    if(arr[l] == 0) {
        arr[l] = 1;
    }
    else {
        arr[l] = 0;
        l--;
        goto h;
    }
    
}

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
}



int main()
{
    char x;
    ifstream fin;
    stack <int> moves;
    string fileName;
    
    // Read the name of the file from the keyboard or
    // hard code it here for testing.
    
    fileName = "knapsack8.input";
    
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
        
        exhaustiveKnapsack(k, 80);
        
        cout << endl << "Best solution" << endl;
        k.printSolution();
        
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





