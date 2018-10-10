/************************************************************************************************
* @file      : Header file for files fetching class
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
#pragma once
#include <vector>
#include <string>
#include <utility>
#include <iostream>
#include <boost/filesystem.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>

namespace FS = boost::filesystem;   //! Short form for boost filesystem

class Files {
 public:
    // @Brief: We create some short forms for long type names

    // Short form for file name pairs (for example, <200.jpg, 200>)
    typedef std::pair<FS::path, int> file_entry;

    // Short form for vector of tuples
    typedef std::vector<file_entry> vec;

    // Short form for iterator of type, boost::filesystem::directory_iterator
    typedef FS::directory_iterator dirIter;

    Files() {}  // !Default constructor
    ~Files() {}   // !Default destructor

    // The fileFeeder method reads the command-line args and
    // iterates with user input until it returns a valid directory
    std::string fileFeeder(std::string commandArgs);

    // The filePicker method reads the command-line args and
    // iterates with user input until it returns a valid file name
    std::string filePicker(std::string commandArgs);

    // Next we create a string to integer conversion method that
    // sorts the numerical file names in ascending order.
    // The return type for this method is "int"
    int stringToInt(boost::filesystem::path const& pathObj);

    // Next we create a path sorting method that sorts the path_vector
    // into ascending order of files for accessing files in the right order
    std::vector<std::pair<boost::filesystem::path, int>> \
            pathSorter(boost::filesystem::path const& boostPathObj);

 private:
    // The path_vec vector is a vector of pairs <path, int>
    std::vector<std::pair<boost::filesystem::path, int>> path_vec;
    std::string dirAddress;   //< String object for storing valide dir address
    std::string fileAddress;   //< String object for storing valide file address
};
