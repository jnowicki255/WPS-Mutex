#include <iostream>
#include <chrono>
#include <thread>
#include <Windows.h>
#include <conio.h>
#include <locale.h>
#include <mutex>
#include <vector>

using namespace std;
using namespace this_thread;
using namespace chrono;

std::mutex colourMtx;

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

int main()
{
    setlocale(LC_CTYPE, "Polish");

    int iterations;
    int delayTime;
    string repeatChar;
    bool repeat = false;
    int taskCount;
    string letters = "ABCDEFGHIJ";
    WORD colors[] = { BLUE, RED, YELLOW, MAGENTA, DARKGREEN, WHITE, DARKBLUE, CYAN, DARKGRAY, DARKMAGENTA };


    do
    {
        system("cls");
        printHeader();

        iterations = readInt(" -> WprowadŸ liczbê iteracji dla zadania - sugerowana wartoœæ [40 - 60]: ");
        delayTime = readInt(" -> WprowadŸ wartopœæ opóŸnienia - sugerowane [100 - 500]: ");
        taskCount = readInt(" -> WprowadŸ iloœæ wyœwietlanych liter [1 - 10]: ");

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
        printf("\nCzas przetwarzania: %d ms", duration.count());

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
        printf("\nCzas przetwarzania: %d ms", duration.count());

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
        printf("\nCzas przetwarzania: %d ms", duration.count());

        // Ponowne wykonanie
        repeat = checkIfRepeat();

    } while (repeat);

    return 0;
}
