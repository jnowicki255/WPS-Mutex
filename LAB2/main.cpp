#include <iostream>
#include <chrono>
#include <thread>
#include <Windows.h>
#include <conio.h>
#include <locale.h>
#include <mutex>
#include <vector>
#include <omp.h>
#include <stdio.h>
#include <fstream>
#include <format>
#include <string>
#include <cmath>
#include <cstdlib>
#include "matrix.h"

using namespace std;
using namespace this_thread;
using namespace chrono;

void printHeader()
{
    cout << "********************************************************************************" << endl;
    cout << "Autor programu: Jacek Nowicki" << endl;
    cout << "********************************************************************************" << endl;
}

char** initArray(int size)
{
    char** array = new char* [size];

    for (int i = 0; i < size; i++)
    {
        array[i] = new char[size];
    }

    return array;
}

int readInt(string message)
{
    int val;

    cout << message;
    cin >> val;

    return val;
}

int main(int argc, char *argv[])
{
    printHeader();

    // Deklaracja zmiennych
    bool dispMatrixes = false;
    bool saveMatrixes = false;
    int size = atoi(argv[1]);//readInt("Podaj wymiar macierzy: ");
    int min = atoi(argv[2]);//readInt("Podaj minimalna wartosc [0-127]: ");
    int max = atoi(argv[3]);//readInt("Podaj maksymalna wartosc [0-127]: ");
    int delay = atoi(argv[4]);//readInt("Podaj opoznienie [ms]: ");
    int groupSize = atoi(argv[5]);//readInt("Podaj maksymaln¹ iloœæ wierszy dla jednego w¹tku: ");
    int iterations = atoi(argv[6]);//ilosc powtórzeñ
    double noOfGroups = ceil(size / groupSize);

    for (int i = 0; i < iterations; i++)
    {
        // Deklaracja i wyœwietlenie tablicy pierwotnej
        auto originalTab = initArray(size);
        genMatrix(originalTab, size, min, max);
        if (dispMatrixes)
        {
            dispMatrix(originalTab, size);
        }

        //==============================================================================================
        // Sortowanie sekwencyjne
        //==============================================================================================
        //cout << "--> 1. Sortowanie sekwencyjne..." << endl;
        auto sequenceTab = initArray(size);
        copyMatrix(originalTab, sequenceTab, size);

        auto start = high_resolution_clock::now();
        sortRowsMatrix(sequenceTab, size, delay);
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(stop - start);
        int seqDuration = duration.count();
        if (dispMatrixes)
        {
            dispMatrix(sequenceTab, size);
        }

        if (saveMatrixes)
        {
            matrixToFile("1-Sequence.csv", sequenceTab, size);
        }
        //cout << "----> Czas realizacji zadania sekwencyjnego: " << duration.count() << "[ms]" << endl;
        //==============================================================================================


        //==============================================================================================
        // Sortowanie równoleg³e (liczba w¹tków równa liczbie wierszy)
        //==============================================================================================
        //cout << "--> 2. Sortowanie zrownoleglone..." << endl;
        auto parallelTab = initArray(size);
        copyMatrix(originalTab, parallelTab, size);
        vector<thread> threads;

        start = high_resolution_clock::now();
        for (int i = 0; i < size; i++)
        {
            threads.emplace_back(thread(sortRowsMatrixThread, parallelTab, size, i, delay));
        }

        for (auto& th : threads)
            th.join();

        stop = high_resolution_clock::now();
        duration = duration_cast<milliseconds>(stop - start);
        int parallelDuration = duration.count();

        if (dispMatrixes)
        {
            dispMatrix(parallelTab, size);
        }

        if (saveMatrixes)
        {
            matrixToFile("2-Parallel.csv", parallelTab, size);
        }
        //cout << "----> Czas realizacji zadania rownoleglego: " << duration.count() << "[ms]" << endl;
        //==============================================================================================


        //==============================================================================================
        // Sortowanie równoleg³e (liczba w¹tków mniejsza ni¿ liczba wierszy)
        //==============================================================================================
        //cout << "--> 3. Sortowanie zrownoleglone (mniejsza liczba watkow)..." << endl;
        auto groupTab = initArray(size);
        copyMatrix(originalTab, groupTab, size);
        vector<thread> threads_groups;
        vector<vector<int>> groups;

        int rowNo = 0;
        for (int i = 0; i < noOfGroups; i++)
        {
            vector<int> group;
            for (int j = 0; j < groupSize; j++)
            {
                if (rowNo < size)
                {
                    group.push_back(rowNo);
                    rowNo++;
                }
            }
            groups.push_back(group);
        }

        start = high_resolution_clock::now();
        for (int i = 0; i < groups.size(); i++)
        {
            threads_groups.emplace_back(thread(sortRowsMatrixThreadGroup, groupTab, size, groups[i], delay));
        }

        for (auto& th : threads_groups)
            th.join();

        stop = high_resolution_clock::now();
        duration = duration_cast<milliseconds>(stop - start);
        int groupDuration = duration.count();

        if (dispMatrixes)
        {
            dispMatrix(groupTab, size);
        }

        if (saveMatrixes)
        {
            matrixToFile("3-Parallel_2.csv", groupTab, size);
        }
        //cout << "----> Czas realizacji zadania rownoleglego (mniejsza liczba watkow): " << duration.count() << "[ms]" << endl;
        //==============================================================================================


        //==============================================================================================
        // Sortowanie równoleg³e OpenMP
        //==============================================================================================
        //cout << "--> 4. Sortowanie zrownoleglone OpenMP..." << endl;
        auto parallelOmpTab = initArray(size);
        copyMatrix(originalTab, parallelOmpTab, size);

        start = high_resolution_clock::now();

#pragma omp parallel for
        for (int i = 0; i < size; i++)
        {
            sortRowsMatrixThread(parallelOmpTab, size, i, delay);
        }

        stop = high_resolution_clock::now();
        duration = duration_cast<milliseconds>(stop - start);
        int openMpDuration = duration.count();

        if (dispMatrixes)
        {
            dispMatrix(parallelOmpTab, size);
        }

        if (saveMatrixes)
        {
            matrixToFile("4-ParallelOMP.csv", parallelOmpTab, size);
        }
        //cout << "----> Czas realizacji zadania rownoleglego OpenMP: " << duration.count() << "[ms]" << endl;
        //==============================================================================================


        //==============================================================================================
        // Sortowanie równoleg³e OpenMP (liczba w¹tków mniejsza ni¿ liczba wierszy)
        //==============================================================================================
        //cout << "--> 5. Sortowanie zrownoleglone OpenMP (mniejsza liczba watkow)..." << endl;
        auto groupOmpTab = initArray(size);
        copyMatrix(originalTab, groupOmpTab, size);
        vector<vector<int>> groupsOmp;

        rowNo = 0;
        for (int i = 0; i < noOfGroups; i++)
        {
            vector<int> group;
            for (int j = 0; j < groupSize; j++)
            {
                if (rowNo < size)
                {
                    group.push_back(rowNo);
                    rowNo++;
                }
            }
            groupsOmp.push_back(group);
        }

        start = high_resolution_clock::now();

#pragma omp parallel for
        for (int i = 0; i < groupsOmp.size(); i++)
        {
            sortRowsMatrixThreadGroup(groupOmpTab, size, groupsOmp[i], delay);
        }

        stop = high_resolution_clock::now();
        duration = duration_cast<milliseconds>(stop - start);
        int groupOpenMpDuration = duration.count();

        if (dispMatrixes)
        {
            dispMatrix(groupOmpTab, size);
        }

        if (saveMatrixes)
        {
            matrixToFile("3-Parallel_2.csv", groupOmpTab, size);
        }
        //cout << "----> Czas realizacji zadania rownoleglego OpenMP (mniejsza liczba watkow): " << duration.count() << "[ms]" << endl;
        //==============================================================================================

        cout << seqDuration << ";" << parallelDuration << ";" << groupDuration << ";" << openMpDuration << ";" << groupOpenMpDuration << endl;
    }
}
