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

using namespace std;
using namespace this_thread;
using namespace chrono;

std::mutex colourMtx;

int logNo = 1;

enum {
    DARKBLUE = FOREGROUND_BLUE,
    DARKGREEN = FOREGROUND_GREEN,
    DARKCYAN = FOREGROUND_GREEN | FOREGROUND_BLUE,
    DARKRED = FOREGROUND_RED,
    DARKMAGENTA = FOREGROUND_RED | FOREGROUND_BLUE,
    DARKYELLOW = FOREGROUND_RED | FOREGROUND_GREEN,
    DARKGRAY = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
    GRAY = FOREGROUND_INTENSITY,
    BLUE = FOREGROUND_INTENSITY | FOREGROUND_BLUE,
    GREEN = FOREGROUND_INTENSITY | FOREGROUND_GREEN,
    CYAN = FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE,
    RED = FOREGROUND_INTENSITY | FOREGROUND_RED,
    MAGENTA = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE,
    YELLOW = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN,
    WHITE = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
};

void printHeader()
{
    printf("********************************************************************************\n");
    printf("Autor programu: Jacek Nowicki\n");
    printf("********************************************************************************\n");
}

int readInt(string message)
{
    int value;

    printf(message.c_str());
    scanf_s("%d", &value);

    return value;
}

bool checkIfRepeat()
{
    printf("\nCzy wykonaæ program ponownie (t\\n) ?");

    do
    {
        char in = _getch();

        if (in == 't' || in == 'T')
        {
            return true;
        }
        else if (in == 'n' || in == 'N')
        {
            return false;
        }

    } while (true);
}

short getConsoleColor() 
{
    CONSOLE_SCREEN_BUFFER_INFO info;
    if (!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info))
        return false;
    return info.wAttributes;
}

void setConsoleColor(short color)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void saveToFile(string filename, string content)
{
    ofstream logFile;
    logFile.open(filename, ios_base::app);
    logFile << content;
    logFile.close();
}

void func(char character, int iterations, int delay, WORD color)
{
    for (int i = 0; i < iterations; i++)
    {
        short previousColor = getConsoleColor();
        setConsoleColor(color);
        printf("%c ", character);
        setConsoleColor(previousColor);
    
        sleep_for(milliseconds(delay));
    }

    setConsoleColor(WHITE);
}

void func_mutex(char character, int iterations, int delay, WORD color) 
{
    for (int i = 0; i < iterations; i++)
    {
        colourMtx.lock();

        short previousColor = getConsoleColor();
        setConsoleColor(color);
        printf("%c ", character);
        setConsoleColor(previousColor);
        
        colourMtx.unlock();

        sleep_for(milliseconds(delay));
    }

    setConsoleColor(WHITE);
}

void func_omp(char character, int iterations, int delay, WORD color)
{
    for (int i = 0; i < iterations; i++)
    {
        #pragma omp critical
        {
            short previousColor = getConsoleColor();
            setConsoleColor(color);
            printf("%c ", character);
            setConsoleColor(previousColor);
        }

        sleep_for(milliseconds(delay));
    }

    setConsoleColor(WHITE);
}

void runTasks(int iterations, int delayTime, int taskCount, bool saveLogs, string filename)
{
    // string repeatChar;
    string letters = "ABCDEFGHIJ";
    WORD colors[] = { BLUE, RED, YELLOW, MAGENTA, DARKGREEN, WHITE, DARKBLUE, CYAN, DARKYELLOW, DARKMAGENTA };

    long sequenceTime;
    long parallelTime;
    long parallelMutexTime;
    long parallelOmpTime;
    long parallelOmpCriticalTime;

#pragma region Sequence
    // Przetwarzanie sekwencyjne
    printf("\n\n ===========> Zadania realizowane SEKWENCYJNIE - liczba zadañ: %d\n", taskCount);
    printf(" KIERUNEK UP£YWU CZASU---- > \n");

    auto start = high_resolution_clock::now();
    for (int i = 0; i < taskCount; i++)
    {
        func(letters[i], iterations, delayTime, colors[i]);
    }
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);
    sequenceTime = duration.count();
    printf("\nCzas przetwarzania: %d ms", duration.count());
#pragma endregion

#pragma region Parallel
    // Przetwarzanie równoleg³e
    printf("\n\n ===========> Zadania realizowane RÓWNOLEGLE - liczba zadañ: %d\n", taskCount);
    printf(" KIERUNEK UP£YWU CZASU---- > \n");

    start = high_resolution_clock::now();
    vector<thread> threads;
    for (int i = 0; i < taskCount; i++)
    {
        threads.emplace_back(thread(func, letters[i], iterations, delayTime, colors[i]));
    }

    for (auto& th : threads)
        th.join();

    stop = high_resolution_clock::now();
    duration = duration_cast<milliseconds>(stop - start);
    parallelTime = duration.count();
    printf("\nCzas przetwarzania: %d ms", duration.count());
#pragma endregion

#pragma region Parallel MUTEX
    // Przetwarzanie równoleg³e z MUTEX'em
    printf("\n\n ===========> Zadania realizowane RÓWNOLEGLE z MUTEXem - liczba zadañ: %d\n", taskCount);
    printf(" KIERUNEK UP£YWU CZASU---- > \n");

    start = high_resolution_clock::now();
    vector<thread> threads_mutex;
    for (int i = 0; i < taskCount; i++)
    {
        threads_mutex.emplace_back(thread(func_mutex, letters[i], iterations, delayTime, colors[i]));
    }

    for (auto& th : threads_mutex)
        th.join();

    stop = high_resolution_clock::now();
    duration = duration_cast<milliseconds>(stop - start);
    parallelMutexTime = duration.count();
    printf("\nCzas przetwarzania: %d ms", duration.count());
#pragma endregion

#pragma region Parallel OpenMP
    // Przetwarzanie równoleg³e Open MP
    printf("\n\n ===========> Zadania realizowane RÓWNOLEGLE OpenMP - liczba zadañ: %d\n", taskCount);
    printf(" KIERUNEK UP£YWU CZASU---- > \n");

    start = high_resolution_clock::now();

    #pragma omp parallel for
    for (int i = 0; i < taskCount; i++)
    {
        func(letters[i], iterations, delayTime, colors[i]);
    }

    stop = high_resolution_clock::now();
    duration = duration_cast<milliseconds>(stop - start);
    parallelOmpTime = duration.count();
    printf("\nCzas przetwarzania: %d ms", duration.count());
#pragma endregion

#pragma region Parallel OpenMP Critical Section
    // Przetwarzanie równoleg³e Open MP z sekcj¹ krytyczn¹
    printf("\n\n ===========> Zadania realizowane RÓWNOLEGLE OpenMP z sekcj¹ krytyczn¹ - liczba zadañ: %d\n", taskCount);
    printf(" KIERUNEK UP£YWU CZASU---- > \n");

    start = high_resolution_clock::now();

    #pragma omp parallel for
    for (int i = 0; i < taskCount; i++)
    {
        func_omp(letters[i], iterations, delayTime, colors[i]);
    }

    stop = high_resolution_clock::now();
    duration = duration_cast<milliseconds>(stop - start);
    parallelOmpCriticalTime = duration.count();
    printf("\nCzas przetwarzania: %d ms", duration.count());
#pragma endregion

#pragma region Save times data to file
    if (saveLogs)
    {
        string content = "\n";
        content += to_string(logNo);
        content += ";";
        content += to_string(sequenceTime);
        content += ";";
        content += to_string(parallelTime);
        content += ";";
        content += to_string(parallelMutexTime);
        content += ";";
        content += to_string(parallelOmpTime);
        content += ";";
        content += to_string(parallelOmpCriticalTime);
        content += ";";

        saveToFile(filename, content);
    }
#pragma endregion
}

int main()
{
    setlocale(LC_CTYPE, "Polish");

    int iterations;
    int delayTime;
    bool repeat = false;
    int taskCount;

    // Wykonanie manualne
    //do
    //{
    //    system("cls");
    //    printHeader();

    //    iterations = readInt(" -> WprowadŸ liczbê iteracji dla zadania - sugerowana wartoœæ [40 - 60]: ");
    //    delayTime = readInt(" -> WprowadŸ wartopœæ opóŸnienia - sugerowane [100 - 500]: ");
    //    taskCount = readInt(" -> WprowadŸ iloœæ wyœwietlanych liter [1 - 10]: ");

    //    runTasks(iterations, delayTime, taskCount, false);

    //    // Ponowne wykonanie
    //    repeat = checkIfRepeat();

    //} while (repeat);

    // Automatyzacja

    system("cls");
    printHeader();

    iterations = readInt(" -> WprowadŸ liczbê iteracji dla zadania - sugerowana wartoœæ [40 - 60]: ");
    delayTime = readInt(" -> WprowadŸ wartopœæ opóŸnienia - sugerowane [100 - 500]: ");
    taskCount = readInt(" -> WprowadŸ iloœæ wyœwietlanych liter [1 - 10]: ");
    int count = 10;

    string filename = "logs//";
    filename += to_string(iterations);
    filename += "-";
    filename += to_string(delayTime);
    filename += "-";
    filename += to_string(count);
    filename += ".csv";

    saveToFile(filename, "No;Sequence;Prallel;ParallelMutex;ParallelOmp;ParallelOmpCrit");

    for (int i = 0; i < count; i++)
    {
        runTasks(iterations, delayTime, taskCount, true, filename);
        logNo++;
    }


    return 0;
}
