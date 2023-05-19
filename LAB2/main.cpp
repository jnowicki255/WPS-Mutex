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

int readInt(string message)
{
    int val;

    cout << message;
    cin >> val;

    return val;
}

int main()
{
    printHeader();

    // Deklaracja zmiennych
    bool dispMatrixes = false;
    bool saveMatrixes = true;
    int tabSize = readInt("Podaj rozmiar tablicy (");
    int min = readInt("Podaj minimalna wartosc [0-127]: ");
    int max = readInt("Podaj maksymalna wartosc [0-127]: ");
    int delay = readInt("Podaj opoznienie [ms]: ");

    // Deklaracja i wyœwietlenie tablicy pierwotnej
    vector<vector<char>> originalTab;




    char tab[tabSize][MAX_TAB_SIZE];
    genMatrix(tab, tabSize, min, max);
    if (dispMatrixes)
    {
        dispMatrix(tab, tabSize);
    }   

    // Sortowanie sekwencyjne
    cout << "--> 1. Sortowanie sekwencyjne..." << endl;
    char tabSeq[tabSize][MAX_TAB_SIZE];
    copyMatrix(tabSeq, tab, tabSize);

    auto start = high_resolution_clock::now();
    sortRowsMatrix(tabSeq, tabSize, delay);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);
    int sequenceTime = duration.count();
    
    if (dispMatrixes)
    {
        dispMatrix(tabSeq, tabSize);
    }
    
    if (saveMatrixes)
    {
        matrixToFile("1-Sequence.csv", tabSeq, tabSize);
    }
    cout << "----> Czas realizacji zadania sekwencyjnego: " << sequenceTime << "[ms]" << endl;


    // Sortowanie równoleg³e (liczba w¹tków równa liczbie wierszy)
    cout << "--> 2. Sortowanie zrownoleglone..." << endl;
    char tabPar[tabSize][MAX_TAB_SIZE];
    copyMatrix(tabPar, tab, tabSize);

    start = high_resolution_clock::now();

    vector<thread> threads;
    for (int i = 0; i < tabSize; i++)
    {
        threads.emplace_back(thread(sortRowsMatrixThread, tabPar, tabSize, i, delay));
    }

    for (auto& th : threads)
        th.join();

    stop = high_resolution_clock::now();
    duration = duration_cast<milliseconds>(stop - start);
    sequenceTime = duration.count();

    if (dispMatrixes)
    {
        dispMatrix(tabPar, tabSize);
    }

    if (saveMatrixes)
    {
        matrixToFile("2-Parallel.csv", tabPar, tabSize);
    }
    cout << "----> Czas realizacji zadania rownoleglego: " << sequenceTime << "[ms]" << endl;










}
