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

void genMatrix(char tab2D[][MAX_TAB_SIZE], int tabSize, int min, int max)
{
    srand(time(nullptr));

    for (int i = 0; i < tabSize; i++)
    {
        for (int j = 0; j < tabSize; j++)
        {
            int random_num = min + (rand() % (max - min + 1));
            tab2D[i][j] = (char)random_num;
        }
    }
}

void copyMatrix(char tab2Dcopy[][MAX_TAB_SIZE], char tab2D[][MAX_TAB_SIZE], int tabSize)
{
    for (int i = 0; i < tabSize; i++)
    {
        for (int j = 0; j < tabSize; j++)
        {
            tab2Dcopy[i][j] = tab2D[i][j];
        }
    }
}

void dispMatrix(char tab2D[][MAX_TAB_SIZE], int tabSize)
{
    for (int i = 0; i < tabSize; i++)
    {
        for (int j = 0; j < tabSize; j++)
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

void matrixToFile(string filename, char tab2D[][MAX_TAB_SIZE], int tabSize)
{
    stringstream content;

    for (int i = 0; i < tabSize; i++)
    {
        for (int j = 0; j < tabSize; j++)
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

void sortRowsMatrix(char tab2D[][MAX_TAB_SIZE], int tabSize, int delay)
{
    for (int i = 0; i < tabSize; i++) {
        for (int j = 0; j < tabSize; j++) {
            for (int k = 0; k < tabSize - 1; k++) {
                if (tab2D[i][k] > tab2D[i][k + 1]) {
                    int temp = tab2D[i][k];
                    tab2D[i][k] = tab2D[i][k + 1];
                    tab2D[i][k + 1] = temp;
                }
                Sleep(delay);
            }
        }
    }
}

void sortRowsMatrixThread(char tab2D[][MAX_TAB_SIZE], int tabSize, int selectedRow, int delay)
{
    for (int i = 0; i < tabSize - 1; i++) {
        for (int j = 0; j < tabSize - i - 1; j++) {
            if (tab2D[selectedRow][j] > tab2D[selectedRow][j + 1]) {
                swap(tab2D[selectedRow][j], tab2D[selectedRow][j + 1]);
            }
            Sleep(delay);
        }
    }
}