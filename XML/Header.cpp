#include "Header.h"

void PropertyTree::CreateDirFromXML(std::string File){
	//read XML file
	read_xml(File, pt);
	//obtain location for the program
	bool format = false; //format of address ('/' false,'\' true)
	int idx;
	idx = File.find_last_of("/");
	if (idx == -1) {
		idx = File.find_last_of("\\");
		format = true;
	}
	std::string Directory = File.substr(0, idx); //Directory of the data (removing the name of XML file from File)
	/******************/
	//create directories from the property tree
	_CreateDir(Directory, pt, format);
}

void PropertyTree::InsertDir(std::string MSSV,std::string DateAndTime){
	//A.update property tree
	//S1: Convert date and time to proper Windows folder name.
	//index based on date and time format
	DateAndTime[10] = '_';
	DateAndTime[13] = '~';
	DateAndTime[16] = '~';
	DateAndTime[19] = ',';
	//S2: Update to property tree
	pt.get_child("Submission").put(MSSV+"."+DateAndTime," ");
	//S3: Update to XML file
	write_xml(Directory, pt);
	//B.add directory
	//S1:Create directory
	int idx;
	idx = Directory.find_last_of("/");
	if (idx == -1)
		idx = Directory.find_last_of("\\");
	std::string dir = Directory.substr(0, idx) + "\\Submission\\" + MSSV + "\\" + DateAndTime;
	boost::filesystem::create_directories(dir);
}

void PropertyTree::DeleteDir(std::string MSSV, std::string DateAndTime){
	//A.update property tree
	//S1: Convert date and time to proper Windows folder name.
	//index based on date and time format
	DateAndTime[10] = '_';
	DateAndTime[13] = '~';
	DateAndTime[16] = '~';
	DateAndTime[19] = ',';
	//S2: delete path in property tree
	if (NoOfChild(pt.get_child("Submission").get_child(MSSV)) > 1)
		pt.get_child("Submission").get_child(MSSV).erase(DateAndTime);
	else if (NoOfChild(pt.get_child("Submission")) > 1) {
		pt.get_child("Submission").erase(MSSV);
		pt.get_child("Submission").add(MSSV, " ");
	}
	else {
		boost::property_tree::ptree newTree;
		newTree.add("Submission", " ");
		pt = newTree;
	}
	//S3: Update to XML file
	write_xml(Directory, pt);
	//B.delete directory
	//S1:Create directory
	int idx;
	idx = Directory.find_last_of("/");
	if (idx == -1)
		idx = Directory.find_last_of("\\");
	std::string dir = Directory.substr(0, idx) + "\\Submission\\" + MSSV + "\\" + DateAndTime;
	boost::filesystem::remove(dir);
}

int NoOfChild(boost::property_tree::ptree &pt){
	using boost::property_tree::ptree;
	int count = 0;
	for (ptree::const_iterator it = pt.begin(); it != pt.end(); ++it) {
		++count;
	}
	return count;
}

void PropertyTree::_CreateDir(std::string Dir, boost::property_tree::ptree const& pt, bool format) {
	using boost::property_tree::ptree;
	for (ptree::const_iterator it = pt.begin(); it != pt.end(); ++it) {
		std::string newDir = (format) ? (Dir + "\\" + it->first) : (Dir + "/" + it->first);
		boost::filesystem::create_directories(newDir);
		_CreateDir(newDir,it->second,format);
	}
}

void PropertyTree::setDir(std::string Address){
	Directory = Address;
}

std::string PropertyTree::getDir(){
	return Directory;
}
