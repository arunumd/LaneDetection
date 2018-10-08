#pragma once
/****************************************************
*
*        Class for accessing files in queue
*
****************************************************/
#include <iostream>
#include <boost/filesystem.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>

//@Brief: The files class iterates over the files in sequence
//        and feeds to the processing system. The files system
//        relies on boost::filesystem library for robustness.
//        The basic working principle is to get a command-line
//        arg from the user input and then do the following
//        actions in sequence:
//          1.) Ensure that the arg represents a valid directory;
//          2.) Capture all file addresses present inside the
//              directory into a std::vector type files vector;
//          3.) Do a sort of the file addresses based on alphabetical
//              order (in ascending order);
//          4.) Return the sorted files path to the called location

namespace FS = boost::filesystem; //! Short form for boost filesystem

class Files {

public:

	//@Brief: We create some short forms for long type names

	// Short form for file name pairs (for example, <200.jpg, 200>)
	typedef std::pair<FS::path, int> file_entry;
	// Short form for vector of tuples
	typedef std::vector<file_entry> vec;
	// Short form for iterator of type, boost::filesystem::directory_iterator
	typedef FS::directory_iterator dirIter;

	Files () {}; //! Default constructor
	~Files () {}; //!Default destructor

	//The fileFeeder method reads the command-line args and
	//iterates with user input until it returns a valid directory
	std::string fileFeeder(std::string commandArgs);

	//The filePicker method reads the command-line args and
	//iterates with user input until it returns a valid file name
	std::string filePicker(std::string commandArgs);

	//Next we create a string to integer conversion method that
	//sorts the numerical file names in ascending order.
	//The return type for this method is "int"
	int stringToInt (boost::filesystem::path const& pathObj);

	//Next we create a path sorting method that sorts the path_vector
	//into ascending order of files for accessing files in the right order
	std::vector<std::pair<boost::filesystem::path, int>> pathSorter (boost::filesystem::path const& boostPathObj);

private:
	// The path_vec vector is a vector of pairs <path, int>
	std::vector<std::pair<boost::filesystem::path, int>> path_vec;
	std::string dirAddress; //< String object for storing valide dir address
	std::string fileAddress; //< String object for storing valide file address
};