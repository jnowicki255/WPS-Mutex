#include <iostream>
#include <fstream>
#include "images.hpp"

using namespace std;

vector<Pixel> readBMP(const string& filename) 
{
    vector<Pixel> pixels;

    ifstream file(filename, ios::binary);
    if (!file) {
        cerr << "Nie mo¿na otworzyæ pliku " << filename << endl;
        return pixels;
    }

    // Nag³ówek BMP sk³ada siê z 54 bajtów, pomijamy go
    file.seekg(54, ios::beg);

    // Szerokoœæ i wysokoœæ obrazu
    int width, height;
    file.read(reinterpret_cast<char*>(&width), sizeof(width));
    file.read(reinterpret_cast<char*>(&height), sizeof(height));

    // Pomijamy zarezerwowane pola
    file.seekg(18, ios::cur);

    // Obliczamy rozmiar danych pikseli
    int dataSize = width * height * 3;

    // Odczytujemy piksele
    vector<unsigned char> pixelData(dataSize);
    file.read(reinterpret_cast<char*>(pixelData.data()), dataSize);

    // Konwertujemy dane pikseli na wektor struktur Pixel
    for (int i = 0; i < dataSize; i += 3) {
        Pixel pixel;
        pixel.blue = pixelData[i];
        pixel.green = pixelData[i + 1];
        pixel.red = pixelData[i + 2];
        pixels.push_back(pixel);
    }

    file.close();

    return pixels;
}

void writeBMP(const string& filename, const vector<Pixel>& pixels, int width, int height) 
{
    ofstream file(filename, ios::binary);
    if (!file) {
        cerr << "Nie mo¿na utworzyæ pliku " << filename << endl;
        return;
    }

    // Nag³ówek BMP sk³ada siê z 54 bajtów

    // Typ pliku BMP
    file.write("BM", 2);

    // Rozmiar pliku (w bajtach)
    int fileSize = 54 + width * height * 3;
    file.write(reinterpret_cast<const char*>(&fileSize), sizeof(fileSize));

    // Zarezerwowane pola
    int reserved = 0;
    file.write(reinterpret_cast<const char*>(&reserved), sizeof(reserved));
    file.write(reinterpret_cast<const char*>(&reserved), sizeof(reserved));

    // Offset danych pikseli
    int dataOffset = 54;
    file.write(reinterpret_cast<const char*>(&dataOffset), sizeof(dataOffset));

    // Rozmiar nag³ówka informacji o pliku BMP
    int infoHeaderSize = 40;
    file.write(reinterpret_cast<const char*>(&infoHeaderSize), sizeof(infoHeaderSize));

    // Szerokoœæ i wysokoœæ obrazu
    file.write(reinterpret_cast<const char*>(&width), sizeof(width));
    file.write(reinterpret_cast<const char*>(&height), sizeof(height));

    // Liczba kolorów p³aszczyzn (1)
    short planes = 1;
    file.write(reinterpret_cast<const char*>(&planes), sizeof(planes));

    // Liczba bitów na piksel (24)
    short bitsPerPixel = 24;
    file.write(reinterpret_cast<const char*>(&bitsPerPixel), sizeof(bitsPerPixel));

    // Kompresja (brak)
    int compression = 0;
    file.write(reinterpret_cast<const char*>(&compression), sizeof(compression));

    // Rozmiar danych pikseli (mo¿e byæ 0 dla braku kompresji)
    int dataSize = width * height * 3;
    file.write(reinterpret_cast<const char*>(&dataSize), sizeof(dataSize));

    // Rozdzielczoœæ pozioma i pionowa (w pikselach na metr, mo¿na u¿yæ domyœlnych wartoœci)
    int horizontalResolution = 0;
    int verticalResolution = 0;
    file.write(reinterpret_cast<const char*>(&horizontalResolution), sizeof(horizontalResolution));
    file.write(reinterpret_cast<const char*>(&verticalResolution), sizeof(verticalResolution));

    // Liczba kolorów w palecie (0 dla obrazów 24-bitowych)
    int colorsInPalette = 0;
    file.write(reinterpret_cast<const char*>(&colorsInPalette), sizeof(colorsInPalette));

    // Liczba istotnych kolorów (0 oznacza wszystkie)
    int importantColors = 0;
    file.write(reinterpret_cast<const char*>(&importantColors), sizeof(importantColors));

    // Zapis pikseli w kolejnoœci BGR (dla BMP)
    for (const auto& pixel : pixels) {
        file.write(reinterpret_cast<const char*>(&pixel.blue), sizeof(pixel.blue));
        file.write(reinterpret_cast<const char*>(&pixel.green), sizeof(pixel.green));
        file.write(reinterpret_cast<const char*>(&pixel.red), sizeof(pixel.red));
    }

    file.close();
}

vector<Point> bresenhamLine(int x1, int y1, int x2, int y2) 
{
    vector<Point> points;

    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    while (true) {
        points.push_back({ x1, y1 });

        if (x1 == x2 && y1 == y2) {
            break;
        }

        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }

    return points;
}

vector<Point> bresenhamLine(Point p1, Point p2) 
{
    return bresenhamLine(p1.x, p1.y, p2.x, p2.y);
}