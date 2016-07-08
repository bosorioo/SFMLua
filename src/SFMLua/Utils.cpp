#include "SFMLua/Utils.h"
#include <fstream>

bool Utils::isFile(std::string path)
{
    std::ifstream file(path.c_str());
    return file.is_open();
}

bool Utils::checkFileExtension(std::string path, std::string extension)
{
    if (extension.empty())
        return true;

    if (path.size() < extension.size())
        return false;

    try {return path.substr(path.size() - extension.size()) == extension; } catch (...) {}

    return false;
}

std::string Utils::getFileName(std::string path)
{
    unsigned index;

    path = Utils::getPrettyPath(path);
    index = path.rfind(SLASH);

    if (index == std::string::npos)
        return path;

    if (index + 1 >= path.size())
        return std::string();

    return path.substr(index + 1);
}

std::string Utils::getDirectory(std::string path)
{
    unsigned index;

    path = Utils::getPrettyPath(path);
    index = path.rfind(SLASH);

    if (index == std::string::npos)
        return std::string();

    return path.substr(0, index + 1);
}

std::string Utils::getParentFolder(std::string path)
{
    unsigned index;
    path = Utils::getPrettyPath(path);
    index = path.rfind(SLASH);

    if (index == std::string::npos)
    // Path has no backslashes, example: "myfile.exe".
    // In this case, parent folder is empty string
        return std::string();

    path.erase(index); // Removes everything after last backslash
    // example: "folder/myfile.exe" -> "folder"

    index = path.rfind(SLASH);

    // Finds again other backslash, if found, return the folder after it.
    // If not, returns the current string. example:
    // "folder" -> "folder"
    // "abc/folder" -> "folder"
    path = (index == std::string::npos ? path : path.substr(index + 1));
    return path.empty() ? path : path + SLASH;
}

std::string Utils::getPrettyPath(std::string path, bool lastSlash)
{
    std::string betterPath;
    char c;

    for (unsigned i = 0; i < path.size(); i++)
    {
        c = path[i];

        // If double slashes are found, or path starts with slashes, ignore it
        if ((c == BSLASH || c == SLASH) && (betterPath.empty() || *betterPath.rbegin() == SLASH))
            continue;
        else if (c == BSLASH)
        // Convert any backslash to slash
            betterPath += SLASH;
        else
            betterPath += c;
    }

    if (lastSlash && *betterPath.rbegin() != SLASH)
        betterPath += SLASH;
    else if (!lastSlash && *betterPath.rbegin() == SLASH)
        betterPath.erase(betterPath.end() - 1);

    return betterPath;
}

std::string Utils::getDoubleBackslash(std::string path)
{
    std::string result;

    for (std::string::iterator it = path.begin(); it != path.end(); it++)
    {
        result += *it;

        if (*it == BSLASH)
            result += BSLASH;
    }

    return result;
}

std::vector<std::string> Utils::splitString(std::string str, char d)
{
    std::vector<std::string> result;
    std::string::size_type s = str.size(), index;
    std::string temp;

    for (index = 0; index <= s; index++)
    {
        if (index == s || str[index] == d)
        {
            if (!temp.empty())
                result.push_back(temp);

            temp.clear();
        }
        else
            temp += str[index];
    }

    return result;
}

std::string Utils::joinString(std::vector<std::string> str, std::string joinstr)
{
    std::string result;

    if (str.empty())
        return result;

    result = *str.begin();

    std::vector<std::string>::iterator it;

    for (it = str.begin() + 1; it != str.end(); it++)
        result += joinstr += *it;

    return result;
}

float Utils::getFactorial(unsigned char number)
{
    if (number <= 1)
        return 1.0;

    int result = number;

    for (number--; number > 1; number--)
        result *= number;

    return (float)result;
}

Utils::BezierPoint Utils::BezierCurvePoint(Utils::BezierPoint points[], float t, int n)
{
    float inv_t = 1 - t;
    int i;

    BezierPoint temp[n];

    for (i = 0; i < n; i++)
        temp[i] = points[i];

    while (--n > 0)
        for (i = 0; i < n; i++)
            temp[i] = temp[i] * inv_t + temp[i + 1] * t;

    return *temp;
}
