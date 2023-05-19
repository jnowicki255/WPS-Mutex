#include <string>
#include <vector>

#ifndef MATRIX_H
#define MATRIX_H

using namespace std;

const int MAX_TAB_SIZE = 700;

void genMatrix(char** tab2D, int size, int min, int max);
void dispMatrix(char** tab2D, int size);
void copyMatrix(char** source, char** desination, int size);
void matrixToFile(string filename, char** tab2D, int size);

//void bubbleSort(char* tab, int size);
//void quickSort(char* tab, int size);

void sortRowsMatrix(char** tab2D, int size, int delay);
void sortRowsMatrixThread(char** tab2D, int size, int selectedRow, int delay);
void sortRowsMatrixThreadGroup(char** tab2D, int size, vector<int> selectedRows, int delay);
#endif