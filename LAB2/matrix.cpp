#include <iostream>
#include <cstdlib>
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <filesystem>
#include <sstream>
#include <chrono>
#include <thread>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <windows.h>
#include <conio.h>
#include <time.h>
#include <chrono>
#include <cstring>
#include <omp.h>
#include "matrix.h"

using namespace std;

void bubbleSort(char* tab, int size, int delay)
{
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {

            if (tab[j] > tab[j + 1]) {
                swap(tab[j], tab[j + 1]);
            }

            if (delay > 0)
                Sleep(delay);
        }
    }
}

int getMax(char* tab, int size)
{
    int max = tab[0];
    for (int i = 1; i < size; i++)
        if (tab[i] > max)
            max = tab[i];
    return max;
}

void countSort(char* tab, int size, int exp)
{
    char* output = new char [size];
    int i, count[10] = { 0 };

    for (i = 0; i < size; i++)
        count[(tab[i] / exp) % 10]++;

    for (i = 1; i < 10; i++)
        count[i] += count[i - 1];

    for (i = size - 1; i >= 0; i--) {
        output[count[(tab[i] / exp) % 10] - 1] = tab[i];
        count[(tab[i] / exp) % 10]--;
    }

    for (i = 0; i < size; i++)
        tab[i] = output[i];
}

void radixSort(char* tab, int size)
{
    int max = getMax(tab, size);

    for (int exp = 1; max / exp > 0; exp *= 10)
        countSort(tab, size, exp);
}


void genMatrix(char** tab2D, int size, int min, int max)
{
    srand(time(nullptr));

    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            int random_num = min + (rand() % (max - min + 1));
            tab2D[i][j] = (char)random_num;
        }
    }
}

void dispMatrix(char** tab2D, int size)
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            int value = (int)tab2D[i][j];
            int value_abs = abs(value);

            cout << " ";

            if (value_abs <= 9)
                cout << " " << value << " ";
            else if (value_abs <= 99)
                cout << value << " ";
            else
                cout << value;

            cout << " ";
        }

        cout << endl;
    }
}

void copyMatrix(char** source, char** desination, int size)
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            desination[i][j] = source[i][j];
        }
    }
}

void matrixToFile(string filename, char** tab2D, int size)
{
    stringstream content;

    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            content << (int)tab2D[i][j] << ";";
        }

        content << endl;
    }

    ofstream file;
    file.open(filename, ios_base::app);
    file << content.str();
    file.close();
}


void sortRowsMatrix(char** tab2D, int size, int delay)
{
    for (int i = 0; i < size; i++)
    {
        bubbleSort(tab2D[i], size, delay);
        //radixSort(tab2D[i], size);
    }
}

void sortRowsMatrixThread(char** tab2D, int size, int selectedRow, int delay)
{
    bubbleSort(tab2D[selectedRow], size, delay);
    //radixSort(tab2D[selectedRow], size);
}

void sortRowsMatrixThreadGroup(char** tab2D, int size, vector<int> selectedRows, int delay)
{
    for (int i = 0; i < selectedRows.size(); i++)
    {
        bubbleSort(tab2D[selectedRows[i]], size, delay);
       // radixSort(tab2D[selectedRows[i]], size);
    }
}