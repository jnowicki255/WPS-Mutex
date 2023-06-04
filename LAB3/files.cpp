#include <iostream>
#include <windows.h>
#include <tchar.h>
#include <vector>
#include "files.hpp"

using namespace std;

vector<wstring> getFilesInCurrentDirectory() {
    vector<wstring> files;

    WIN32_FIND_DATA findData;
    HANDLE hFind = INVALID_HANDLE_VALUE;

    TCHAR currentDir[MAX_PATH];
    GetCurrentDirectory(MAX_PATH, currentDir);

    wstring searchPattern = currentDir;
    searchPattern += _T("\\*");

    hFind = FindFirstFile(searchPattern.c_str(), &findData);
    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                continue;  // Pomiñ katalogi
            }
            files.push_back(findData.cFileName);
        } while (FindNextFile(hFind, &findData) != 0);

        FindClose(hFind);
    }

    return files;
}
