#pragma once
#include <iostream>
#include <string>
//Used for creating date and time object
#include "boost\date_time\posix_time\posix_time.hpp"
//The two library below is used for reading and parsing XML file
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/ptree.hpp>
//Used for create, delete...directories
#include <boost/filesystem.hpp>

#define DEFAULT_MAX_SUBMISSION 10000

//through PropertyTree:
//Insert and create new directories
//delete if necessary
//Modify and update XML file (depend on Students' submit)
class PropertyTree {
	//Main tree
	boost::property_tree::ptree pt;
	//Directory of XML file
	std::string Directory;
	friend int NoOfChild(boost::property_tree::ptree&);
	void _CreateDir(std::string, boost::property_tree::ptree const&, bool);
public:
	void setDir(std::string);
	std::string getDir();
	void CreateDirFromXML(std::string);
	void InsertDir(std::string,std::string);//Insert directory and update XML file
	void DeleteDir(std::string,std::string);//delete directory and update XML file
	std::string* ReadDir(); //Read new directory from current "Submission" folder, insert into XML file , create directories, and return list of new submission
};

struct StudentInfo {
	std::string Location;// current address of file group in computer (date time format)
	boost::posix_time::ptime time; //get from Location
	float score; //students' score
	bool isChecked; //check whether student's file has been processed
	std::string MSSV; //Student's ID
};

class Submission {
	//an array of students, each contain a list of file group student submitted
	StudentInfo *Submit;
	//current property tree
	PropertyTree ptree;
	//current size of array above
	int size;
	int MaxSize;
	//private methods
	void reHeapUp();
	void reHeapDown();
public:
	//constructor
	Submission(std::string Address) {
		ptree.setDir(Address);
		//Check existence of XML file
		if (!exists(boost::filesystem::path(ptree.getDir())))
			throw("XML File does not exist!");
		ptree.CreateDirFromXML(ptree.getDir());
		Submit = new StudentInfo[DEFAULT_MAX_SUBMISSION];
		MaxSize = DEFAULT_MAX_SUBMISSION;
		size = 0;
	}
	Submission(std::string Address, int MAX_ELEMENT){
		ptree.setDir(Address);
		//Check existence of XML file
		if (!exists(boost::filesystem::path(ptree.getDir())))
			throw("XML File does not exist!");
		ptree.CreateDirFromXML(ptree.getDir());
		Submit = new StudentInfo[MAX_ELEMENT];
		MaxSize = MAX_ELEMENT;
		size = 0;
	}
	//public methods
	void InsertData(std::string*); //Insert (Heap-style) new submission information from the directory given by PropertyTree
	StudentInfo* Scan(); //scan new submissions from student and update property tree

};

