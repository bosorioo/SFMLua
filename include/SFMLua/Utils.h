#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>

#define SLASH '/'
#define BSLASH '\\'

namespace Utils
{
    bool isFile(std::string path);
    bool checkFileExtension(std::string path, std::string extension);

    std::string getFileName(std::string path);
    std::string getDirectory(std::string path);
    std::string getParentFolder(std::string path);
    std::string getPrettyPath(std::string path, bool lastSlash = false);
    std::string getDoubleBackslash(std::string path);
    std::vector<std::string> splitString(std::string str, char d);
    std::string joinString(std::vector<std::string> str, std::string joinstr = "");

    float getFactorial(unsigned char number);

    struct BezierPoint
    {
        float x, y;
        BezierPoint() {};
        BezierPoint(float _x, float _y) : x(_x), y(_y) {}
        BezierPoint(const BezierPoint& p) : x(p.x), y(p.y) {}
        BezierPoint operator *(const float& a)
        {
            return BezierPoint(a * x, a * y);
        }
        BezierPoint operator +(const BezierPoint& a)
        {
            return BezierPoint(a.x + x, a.y + y);
        }
    };

    BezierPoint BezierCurvePoint(BezierPoint points[], float t, int n);

};

#endif // UTILS_H
