/************************************************************************************************
* @file      : Implementation for files fetching class
* @author    : Arun Kumar Devarajulu
* @date      : October 8, 2018
* @copyright : 2018, Arun Kumar Devarajulu
* @license   : MIT License
*
*              Permission is hereby granted, free of charge, to any person obtaining a copy
*              of this software and associated documentation files (the "Software"), to deal
*              in the Software without restriction, including without limitation the rights
*              to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
*              copies of the Software, and to permit persons to whom the Software is
*              furnished to do so, subject to the following conditions:
*
*              The above copyright notice and this permission notice shall be included in all
*              copies or substantial portions of the Software.
*
*              THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*              IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*              FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
*              AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
*              LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
*              OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
*              SOFTWARE.
*
* @brief     : The files class iterates over the files in sequence and feeds to the processing
*              system. The files system relies on boost::filesystem library for robustness.
*              The basic working principle is to get a command-line arg from the user input
*              and then do the following actions in sequence:
*                1.) Ensure that the arg represents a valid directory;
*                2.) Capture all file addresses present inside the
*                    directory into a std::vector type files vector;
*                3.) Do a sort of the file addresses based on alphabetical
*                    order (in ascending order);
*                4.) Return the sorted files path to the called location
*
**************************************************************************************************/
#include "Files.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include <algorithm>
#include <fstream>
#include <ostream>
#include <iomanip>
#include <cmath>
#include <boost/filesystem.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>

namespace FS = boost::filesystem;   // !Short form for boost filesystem

// @Brief: We create some short forms for long type names

// Short form for file name pairs (for example, <200.jpg, 200>)
typedef std::pair<FS::path, int> file_entry;
// Short form for vector of tuples
typedef std::vector<file_entry> vec;
// Short form for iterator of type, boost::filesystem::directory_iterator
typedef FS::directory_iterator dirIter;

// The fileFeeder method reads the command-line args and
// creates a vector of valid file-names existing in a valid
// directory
std::string Files::fileFeeder(std::string commandArgs) {
    FS::path pathObj(commandArgs);

    // We first check if there is a valid directory in the path
    // using the following steps
    while (!is_directory(pathObj)) {
        std::cout << "The path is not a valid directory."
                  "Please enter valid path.";
        std::getline(std::cin, commandArgs);
        FS::path pathObj(commandArgs);
        commandArgs = pathObj.string();
    }
    return commandArgs;   //< Return the valid directory address
}

// The filePicker method reads the command-line args and
// iterates with user input until it returns a valid file name
std::string Files::filePicker(std::string commandArgs) {
    FS::path pathObj(commandArgs);

    // We first check if there is a valid directory in the path
    // using the following steps
    while (!is_regular_file(pathObj)) {
        std::cout << "The path is not a valid filename."
                  "Please enter valid filename.";
        std::getline(std::cin, commandArgs);
        FS::path pathObj(commandArgs);
        commandArgs = pathObj.string();
    }
    return commandArgs;   //< Return the valid filename address
}

// Next we create a string to integer conversion method that
// sorts the numerical file names in ascending order.
// The return type for this method is "int"
int Files::stringToInt(FS::path const& pathObj) {
    return std::stoi(pathObj.filename().string());
}

// Next we create a path sorting method that sorts the path_vector
// into ascending order of files for accessing files in the right order
std::vector<std::pair<boost::filesystem::path, int>> \
Files::pathSorter(FS::path const& pathObj) {
    // First we ensure that our path_vec is a clean container
    path_vec.clear();

    // We create a path vector of the manner [(1.jpg, 1); (5.jpg, 5); ...]
    // Here "stringToInt" method is used to create the second item (integer)
    // of the "path_vec" vector items
    for (dirIter it(pathObj); it != FS::directory_iterator(); ++it) {
        path_vec.emplace_back(*it, stringToInt(*it));
    }

    // Then we sort the previous "path" vector using
    // lambda sort function in C++ 11
    std::sort(path_vec.begin(), path_vec.end(), [] \
    (file_entry const & a, file_entry const & b) {
        return a.second < b.second;
    });
    return path_vec;   //< Return the sorted path vector
                       //  of pairs with type <path, int>
}
