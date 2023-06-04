#include <iostream>
#include "images.hpp"
#include "files.hpp"

using namespace std;

int main()
{
    //vector<wstring> files = getFilesInCurrentDirectory();

    //cout << "Dostepne pliki:\n";
    //for (const auto& file : files) {
    //    wcout << file << endl;
    //}

    vector<Pixel> lena = readBMP("bmp_24.bmp");

}
