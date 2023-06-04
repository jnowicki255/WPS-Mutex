#ifndef IMAGES_HPP
#define IMAGES_HPP

#include <vector>
#include <string>

using namespace std;

struct Pixel 
{
    unsigned char blue;
    unsigned char green;
    unsigned char red;
};

struct Point
{
    int x;
    int y;
};

vector<Pixel> readBMP(const string& filename);
void writeBMP(const string& filename, const vector<Pixel>& pixels, int width, int height);

vector<Point> bresenhamLine(int x1, int y1, int x2, int y2);
vector<Point> bresenhamLine(Point p1, Point p2);

#endif