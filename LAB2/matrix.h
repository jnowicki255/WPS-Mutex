#include <string>

#ifndef MATRIX_H
#define MATRIX_H

using namespace std;

const int MAX_TAB_SIZE = 700;

void genMatrix(char tab2D[][MAX_TAB_SIZE], int tabSize, int min, int max);
void copyMatrix(char tab2Dcopy[][MAX_TAB_SIZE], char tab2D[][MAX_TAB_SIZE], int tabSize);
void dispMatrix(char tab2D[][MAX_TAB_SIZE], int tabSize);
void matrixToFile(string filename, char tab2D[][MAX_TAB_SIZE], int tabSize);

void sortRowsMatrix(char tab2D[][MAX_TAB_SIZE], int tabSize, int delay);
void sortRowsMatrixThread(char tab2D[][MAX_TAB_SIZE], int tabSize, int selectedRow, int delay);
#endif