// ConsoleApplication1.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include "pch.h"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
//#include <iomanip> //for std::setw
//#include <stdio.h> //for printf
#include <algorithm>
#include <random>

struct listElement {
	int id;
	int zip;
	short int timesSwapped = 0;
	std::string firstName;
	std::string lastName;
	std::string email;
	std::string hash;
};

struct clientRequest {
	std::string organizationName;
	int preferredZipCode = 0;
	int numOfNames;
};

struct zip_code {
	short int first = 0;
	short int secThd = 0;
	short int forFth = 0;
};

struct zipBounds {
	int lower;
	int upper;
};

static std::vector<std::string> convertCsvHashToVector(const std::string& list_path);
static std::vector<std::vector<std::string>> generateListOfLists(const std::vector<std::string>& list_paths);
static std::vector<listElement> convertFullListToVector(const std::string& full_list_path);
static std::vector<clientRequest> convertRequestsToVector(const std::string& req_list_path);
static std::vector<std::vector<int>> listSplitter(const clientRequest& client_req, const std::vector<listElement>& full_list);
static void generateExportableList(const std::vector<clientRequest>& client_req, std::vector<listElement>& full_list, const std::vector<std::vector<std::string>>& hash);
static zipBounds getStateZipBounds(const int& zip);

int main() {
	std::vector<std::string> LIST_PATHS{ 
		"C:\\Users\\bluei\\Desktop\\hash1.csv",
		"C:\\Users\\bluei\\Desktop\\hash2.csv", 
		"C:\\Users\\bluei\\Desktop\\hash3.csv", 
		"C:\\Users\\bluei\\Desktop\\hash4.csv", 
		"C:\\Users\\bluei\\Desktop\\hash5.csv"
	};

	std::string FULL_LIST_PATH = "C:\\Users\\bluei\\Desktop\\fullList.csv";
	std::string REQ_LIST_PATH = "C:\\Users\\bluei\\Desktop\\testRequests.csv";


	std::vector<clientRequest> clientRequests = convertRequestsToVector(REQ_LIST_PATH);
	std::vector<std::vector<std::string>> listOfHashes = generateListOfLists(LIST_PATHS);
	std::vector<listElement> fullList = convertFullListToVector(FULL_LIST_PATH);

	//debug mode breakpoint
	int jeff = 0;

	generateExportableList(clientRequests, fullList, listOfHashes);
	  
	//**************for (size_t i = 0; i<clientRequests.size(); i++) {
	//**************	std::ofstream outfile(("C:\\Users\\bluei\\Desktop\\"+clientRequests[i].organizationName+"_JA_complete.csv"));
	//**************	std::vector<listElement> exporter = generateExportableList(clientRequests[i], fullList, listOfHashes[i]);
	//**************  
	//**************outfile << "id,firstName,lastName,email,timesSwapped,zip,hash" << std::endl;
	//**************  
	//**************for (listElement& el : exporter)
	//**************	outfile << el.id << ',' << el.firstName << ',' << el.lastName << ',' << el.email << ',' << el.timesSwapped << ',' << el.zip << ',' << el.hash << std::endl;
	//**************  
	//**************	outfile.close();
	//**************}
	//**************  
	std::ofstream outfile(("C:\\Users\\bluei\\Desktop\\full_list_out.csv"));
	outfile << "id,firstName,lastName,email,timesSwapped,zip,hash" << std::endl;
	  
	for (listElement& el : fullList)
		outfile << el.id << ',' << el.firstName << ',' << el.lastName << ',' << el.email << ',' << el.timesSwapped << ',' << el.zip << ',' << el.hash << std::endl;
	  
	outfile.close();
	//**************//remember to use this syntax when iterating through vectors
	//**************//for (std::string& s : LIST_PATHS)
	//**************//	std::cout << s << std::endl;

	std::cout << "Press any key to exit the terminal fam." <<std::endl;
	std::cin.get();

	return 0;
}

//   Reads a single column spreadsheet in CSV format and
//   converts it into a vector of strings with each vector
//   element corresponding to a spreadsheet cell
//   
//   @param string containing the path to a CSV to import
//   @return a vector a of strings with each vector
//   element corresponding to a spreadsheet cell
static std::vector<std::string> convertCsvHashToVector(const std::string& list_path) {
	std::ifstream ifs(list_path, std::ifstream::in);
	char c = ifs.get();
	int counter = 0;
	std::vector<std::string> list;
	std::vector<char> currLine;

	//   The guts of this code block are designed
	//   to read each line of the CSV file and assign
	//   each line to a string that is pushed into a 
	//   vector. Each individual character is read in
	//   on the function call ifs.get().
	//   
	//   Algorithm:
	//   
	//   While ifs is good
	//   Read char and push it into the currLine vector
	//   if c == '\n' convert currLine to a string then push
	//       to list and currLine.clear()

	while ( ifs.good() ) {
		if (c == '\n') {
			currLine.push_back('\0');

			char* arr = new char[currLine.size()];

			for (size_t i = 0; i < currLine.size(); i++) {
				arr[i] = currLine[i];
			}

			std::string stringToPush = arr;

			if (counter != 0) { list.push_back(stringToPush); }
			currLine.clear();
			delete[] arr;
			counter++;
		} else {
			currLine.push_back(c);
		}

		c = ifs.get();
		
	}

	currLine.push_back('\0');

	char* arr = new char[currLine.size()];

	for (size_t i = 0; i < currLine.size(); i++) {
		arr[i] = currLine[i];
	}

	std::string stringToPush = arr;

	if (counter != 0 && stringToPush!="\0\0") { list.push_back(stringToPush); }
	currLine.clear();
	delete[] arr;

	ifs.close();

	return list;
}

//   Passes a vector containing the path locations of all 
//   the CSV hash files that are to be read into the program
//   and passes them to the convertCsvHashToVector function
//   
//   @param vector of strings containing the paths import
//   @return a vector of vectors of strings containing all
//   of the hashes in all of the CSVs
//
static std::vector<std::vector<std::string>> generateListOfLists(const std::vector<std::string>& list_paths) {
	std::vector<std::vector<std::string>> listOfLists;

	for (size_t i = 0; i < list_paths.size(); i++) {
		listOfLists.push_back(convertCsvHashToVector(list_paths[i]));
	}

	return listOfLists;
}

//   Reads the spreadsheet that contains the full collection
//   of joint action names spreadsheet in CSV format and
//   converts it into a vector of structs with each vector
//   element corresponding to a spreadsheet row
//   
//   @param string containing the path to a CSV to import
//   @return a vector a of structs with each vector
//   element corresponding to a spreadsheet row
static std::vector<listElement> convertFullListToVector(const std::string& full_list_path) {
	std::ifstream ifs(full_list_path, std::ifstream::in);
	char c = ifs.get();
	short int stateCounter = 0;
	int counter = 0;
	std::vector<listElement> list;
	list.reserve(58000);
	std::vector<char> currLine;
	currLine.reserve(40);
	listElement elementToPush;

	//   The guts of this code block are designed
	//   to read each line of the CSV file and assign
	//   each line to a struct that is pushed into a 
	//   vector. Each individual character is read in
	//   on the function call ifs.get().
	//   
	//   Algorithm:
	//   
	//   While ifs is good
	//   Read char and push it into the currLine vector
	//   if c == '\n' convert currLine to a string then if necessary  
	//   that string is converted into an integer, assigned to the 
	//   struct, then pushed into a vector and currLine.clear().

	while ( ifs.good() ) {
		if (counter > 0) {
			if (stateCounter == 0) {
				if (c == ',') {
					currLine.push_back('\0');

					char* arr = new char[currLine.size()];

					for (size_t i = 0; i < currLine.size(); i++) {
						arr[i] = currLine[i];
					}

					std::string stringToPush = arr;

					list.push_back(listElement());
					list[(counter-1)].firstName = stringToPush;
					currLine.clear();
					delete[] arr;
					stateCounter++;
				} else {
					currLine.push_back(c);
				}
			} else if (stateCounter == 1) {
				if (c == ',') {
					currLine.push_back('\0');

					char* arr = new char[currLine.size()];

					for (size_t i = 0; i < currLine.size(); i++) {
						arr[i] = currLine[i];
					}

					std::string stringToPush = arr;

					list[(counter - 1)].lastName = stringToPush;
					currLine.clear();
					delete[] arr;
					stateCounter++;
				}
				else {
					currLine.push_back(c);
				}
			} else if (stateCounter == 2) {
				//consider creating a ZIP code data type, populating it and error checking here
				if (c == ',') {
					currLine.push_back('\0');

					char* arr = new char[currLine.size()];

					for (size_t i = 0; i < currLine.size(); i++) {
						arr[i] = currLine[i];
					}

					std::string stringToPush = arr;

					if (!stringToPush.empty()) {
						int integerToPush = std::stoi(stringToPush);
						if (integerToPush > 499) {
							list[(counter - 1)].zip = integerToPush;
						}
					}

					currLine.clear();
					delete[] arr;
					stateCounter++;
				}
				else {
					currLine.push_back(c);
				}
			} else if (stateCounter == 3) {
				if (c == ',') {
					currLine.push_back('\0');

					char* arr = new char[currLine.size()];

					for (size_t i = 0; i < currLine.size(); i++) {
						arr[i] = currLine[i];
					}

					std::string stringToPush = arr;

					list[(counter - 1)].email = stringToPush;
					currLine.clear();
					delete[] arr;
					stateCounter++;
				}
				else {
					currLine.push_back(c);
				}
			} else if (stateCounter == 4) {
				if (c == '\n') {
					currLine.push_back('\0');

					char* arr = new char[currLine.size()];

					for (size_t i = 0; i < currLine.size(); i++) {
						arr[i] = currLine[i];
					}

					std::string stringToPush = arr;

					list[(counter - 1)].hash = stringToPush;
					currLine.clear();
					delete[] arr;

					list[(counter - 1)].id = (counter - 1);

					stateCounter++;
				}
				else {
					currLine.push_back(c);
				}
			}
		} 

		if (c == '\n') {
			counter++;
			stateCounter = 0;
		}

		c = ifs.get();
	}

	currLine.push_back('\0');

	char* arr = new char[currLine.size()];

	for (size_t i = 0; i < currLine.size(); i++) {
		arr[i] = currLine[i];
	}

	std::string stringToPush = arr;

	if (stringToPush != "\0\0") { list[(counter - 1)].hash = stringToPush; }
	currLine.clear();
	delete[] arr;

	list[(counter - 1)].id = (counter - 1);

	ifs.close();

	return list;
}

//   Reads the spreadsheet that contains the client requests
//   spreadsheet in CSV format and converts it into a vector of
//   structs with each vector element corresponding to a spreadsheet row
//   
//   @param string containing the path to a CSV to import
//   @return a vector a of structs with each vector
//   element corresponding to a spreadsheet row
static std::vector<clientRequest> convertRequestsToVector(const std::string& req_list_path) {
	std::ifstream ifs(req_list_path, std::ifstream::in);
	char c = ifs.get();
	short int stateCounter = 0;
	int counter = 0;
	std::vector<clientRequest> list;
	std::vector<char> currLine;
	clientRequest elementToPush;

	//   The guts of this code block are designed
	//   to read each line of the CSV file and assign
	//   each line to a struct that is pushed into a 
	//   vector. Each individual character is read in
	//   on the function call ifs.get().
	//   
	//   Algorithm:
	//   
	//   While ifs is good
	//   Read char and push it into the currLine vector
	//   if c == '\n' convert currLine to a string then if necessary  
	//   that string is converted into an integer, assigned to the 
	//   struct, then pushed into a vector and currLine.clear().

	while (ifs.good()) {
		if (counter > 0) {
			if (stateCounter == 0) {
				if (c == ',') {
					currLine.push_back('\0');

					char* arr = new char[currLine.size()];

					for (size_t i = 0; i < currLine.size(); i++) {
						arr[i] = currLine[i];
					}

					std::string stringToPush = arr;

					list.push_back(clientRequest());
					list[(counter - 1)].organizationName = stringToPush;
					currLine.clear();
					delete[] arr;
					stateCounter++;
				}
				else {
					currLine.push_back(c);
				}
			}
			else if (stateCounter == 1) {
				if (c == ',') {
					currLine.push_back('\0');

					char* arr = new char[currLine.size()];

					for (size_t i = 0; i < currLine.size(); i++) {
						arr[i] = currLine[i];
					}

					std::string stringToPush = arr;

					
					int integerToPush = std::stoi(stringToPush);
						
					list[(counter - 1)].preferredZipCode = integerToPush;
					

					currLine.clear();
					delete[] arr;
					stateCounter++;
				}
				else {
					currLine.push_back(c);
				}
			}
			else if (stateCounter == 2) {
				if (c == '\n') {
					currLine.push_back('\0');

					char* arr = new char[currLine.size()];

					for (size_t i = 0; i < currLine.size(); i++) {
						arr[i] = currLine[i];
					}

					std::string stringToPush = arr;

					int integerToPush = std::stoi(stringToPush);

					list[(counter - 1)].numOfNames = integerToPush;

					currLine.clear();
					delete[] arr;

					stateCounter++;
				}
				else {
					currLine.push_back(c);
				}
			}
		}

		if (c == '\n') {
			counter++;
			stateCounter = 0;
		}

		c = ifs.get();
	}

	currLine.push_back('\0');

	char* arr = new char[currLine.size()];

	for (size_t i = 0; i < currLine.size(); i++) {
		arr[i] = currLine[i];
	}

	std::string stringToPush = arr;

	if (stringToPush != "\0\0") {
		int integerToPush = std::stoi(stringToPush);
		list[(counter - 1)].numOfNames = integerToPush;
	}

	currLine.clear();
	delete[] arr;

	ifs.close();

	return list;
}


static std::vector<std::vector<int>> listSplitter(const clientRequest& client_req, const std::vector<listElement>& full_list) {
	std::vector<std::vector<int>> listToReturn(3);
	listToReturn.reserve(58000);
	std::vector<listElement> listToSplit = full_list;
	auto rng = std::default_random_engine{};
	zipBounds zipBoundaries = getStateZipBounds(client_req.preferredZipCode);

	//adjust this code when the zip_code struct is implemented
	for (listElement& el : listToSplit) {
		bool condition = el.zip<1000 ? ((static_cast<double>(el.zip) / 1000) == (static_cast<double>(client_req.preferredZipCode) / 1000)) : ((el.zip / 1000) == (client_req.preferredZipCode / 1000));

		if (condition) {
			listToReturn[0].push_back(el.id);
		} else if (el.zip >= zipBoundaries.lower && el.zip <= zipBoundaries.upper ) {
			listToReturn[1].push_back(el.id);
		} else {
			listToReturn[2].push_back(el.id);
		}
	}

	// if(el.zip<1000){(static_cast<double>(el.zip)/1000) == (static_cast<double>(client_req.preferredZipCode)/1000)}else{(el.zip/1000) == (client_req.preferredZipCode/1000)}

	std::shuffle(std::begin(listToReturn[0]), std::end(listToReturn[0]), rng);
	std::shuffle(std::begin(listToReturn[1]), std::end(listToReturn[1]), rng);
	std::shuffle(std::begin(listToReturn[2]), std::end(listToReturn[2]), rng);

	return listToReturn;
}

struct abacus {
	size_t countTotal = 0;
	size_t count0 = 0;
	size_t count1 = 0;
	size_t count2 = 0;
};

static void generateExportableList(const std::vector<clientRequest>& client_req_list, std::vector<listElement>& full_list, const std::vector<std::vector<std::string>>& hash) {
	std::ofstream outfile;
	std::vector<abacus> counterArr(client_req_list.size());
	std::vector<std::vector<std::vector<int>>> preferredListIDs;
	preferredListIDs.reserve(58000*client_req_list.size());
	for (size_t i = 0; i < client_req_list.size(); i++) { preferredListIDs.push_back(listSplitter(client_req_list[i], full_list)); }
	
	//ALSO in the if statements make checks to see if the counterArr[i].countX doesn't
	//Exceed the length of preferredListIDs[i][X]

	size_t iterator = 0;
	bool condition = true;

	for (size_t i = 0; i < client_req_list.size(); i++) {
		outfile.open(("C:\\Users\\bluei\\Desktop\\output\\" + client_req_list[i].organizationName + "_JA_complete.csv"), std::ios_base::app);
		outfile << "id,firstName,lastName,email,timesSwapped,zip,hash" << std::endl;
		outfile.close();
	}

	while (condition) {
		for (size_t i = 0; i < client_req_list.size(); i++) {
			if (counterArr[i].countTotal >= client_req_list[i].numOfNames) { continue; } //consider creating a bool array where values are set to true only if countArr[i].countX >= preferredListIDs[i][x] in addition to the current if condition

			if (preferredListIDs[i][0].size() > counterArr[i].count0) { /*************/
				if (!(std::find(hash[i].begin(), hash[i].end(), full_list[preferredListIDs[i][0][counterArr[i].count0]].hash) != hash[i].end()) && full_list[preferredListIDs[i][0][counterArr[i].count0]].timesSwapped < 3) {
					outfile.open(("C:\\Users\\bluei\\Desktop\\output\\" + client_req_list[i].organizationName + "_JA_complete.csv"), std::ios_base::app);
					outfile << full_list[preferredListIDs[i][0][counterArr[i].count0]].id << ','
						<< full_list[preferredListIDs[i][0][counterArr[i].count0]].firstName << ','
						<< full_list[preferredListIDs[i][0][counterArr[i].count0]].lastName << ','
						<< full_list[preferredListIDs[i][0][counterArr[i].count0]].email << ','
						<< full_list[preferredListIDs[i][0][counterArr[i].count0]].timesSwapped << ','
						<< full_list[preferredListIDs[i][0][counterArr[i].count0]].zip << ','
						<< full_list[preferredListIDs[i][0][counterArr[i].count0]].hash << std::endl;
					outfile.close();
					full_list[preferredListIDs[i][0][counterArr[i].count0]].timesSwapped++;
					counterArr[i].countTotal++;

					//std::cout << full_list[preferredListIDs[i][0][counterArr[i].count0]].id << '\t' << full_list[preferredListIDs[i][0][counterArr[i].count0]].firstName << '\t' << full_list[preferredListIDs[i][0][counterArr[i].count0]].lastName << '\t' << full_list[preferredListIDs[i][0][counterArr[i].count0]].email << '\t' << full_list[preferredListIDs[i][0][counterArr[i].count0]].zip << '\t' << full_list[preferredListIDs[i][0][counterArr[i].count0]].hash << '\t' << full_list[preferredListIDs[i][0][counterArr[i].count0]].timesSwapped << std::endl;
				}

				counterArr[i].count0++;

			} else if (preferredListIDs[i][1].size() > counterArr[i].count1) { /**************/
				if (!(std::find(hash[i].begin(), hash[i].end(), full_list[preferredListIDs[i][1][counterArr[i].count1]].hash) != hash[i].end()) && full_list[preferredListIDs[i][1][counterArr[i].count1]].timesSwapped < 3) {
					outfile.open(("C:\\Users\\bluei\\Desktop\\output\\" + client_req_list[i].organizationName + "_JA_complete.csv"), std::ios_base::app);
					outfile << full_list[preferredListIDs[i][1][counterArr[i].count1]].id << ','
						<< full_list[preferredListIDs[i][1][counterArr[i].count1]].firstName << ','
						<< full_list[preferredListIDs[i][1][counterArr[i].count1]].lastName << ','
						<< full_list[preferredListIDs[i][1][counterArr[i].count1]].email << ','
						<< full_list[preferredListIDs[i][1][counterArr[i].count1]].timesSwapped << ','
						<< full_list[preferredListIDs[i][1][counterArr[i].count1]].zip << ','
						<< full_list[preferredListIDs[i][1][counterArr[i].count1]].hash << std::endl;
					outfile.close();
					full_list[preferredListIDs[i][1][counterArr[i].count1]].timesSwapped++;
					counterArr[i].countTotal++;

					//std::cout << full_list[preferredListIDs[i][1][counterArr[i].count1]].id << '\t' << full_list[preferredListIDs[i][1][counterArr[i].count1]].firstName << '\t' << full_list[preferredListIDs[i][1][counterArr[i].count1]].lastName << '\t' << full_list[preferredListIDs[i][1][counterArr[1].count1]].email << '\t' << full_list[preferredListIDs[i][1][counterArr[i].count1]].zip << '\t' << full_list[preferredListIDs[i][1][counterArr[i].count1]].hash << '\t' << full_list[preferredListIDs[i][1][counterArr[i].count1]].timesSwapped << std::endl;
				}

				counterArr[i].count1++;

			} else if (preferredListIDs[i][2].size() > counterArr[i].count2) { /***************/
				if (!(std::find(hash[i].begin(), hash[i].end(), full_list[preferredListIDs[i][2][counterArr[i].count2]].hash) != hash[i].end()) && full_list[preferredListIDs[i][2][counterArr[i].count2]].timesSwapped < 3) {
					outfile.open(("C:\\Users\\bluei\\Desktop\\output\\" + client_req_list[i].organizationName + "_JA_complete.csv"), std::ios_base::app);
					outfile << full_list[preferredListIDs[i][2][counterArr[i].count2]].id << ','
						<< full_list[preferredListIDs[i][2][counterArr[i].count2]].firstName << ','
						<< full_list[preferredListIDs[i][2][counterArr[i].count2]].lastName << ','
						<< full_list[preferredListIDs[i][2][counterArr[i].count2]].email << ','
						<< full_list[preferredListIDs[i][2][counterArr[i].count2]].timesSwapped << ','
						<< full_list[preferredListIDs[i][2][counterArr[i].count2]].zip << ','
						<< full_list[preferredListIDs[i][2][counterArr[i].count2]].hash << std::endl;
					outfile.close();
					full_list[preferredListIDs[i][2][counterArr[i].count2]].timesSwapped++;
					counterArr[i].countTotal++;

					//std::cout << full_list[preferredListIDs[i][2][counterArr[i].count2]].id << '\t' << full_list[preferredListIDs[i][2][counterArr[i].count2]].firstName << '\t' << full_list[preferredListIDs[i][2][counterArr[i].count2]].lastName << '\t' << full_list[preferredListIDs[i][2][counterArr[i].count2]].email << '\t' << full_list[preferredListIDs[i][2][counterArr[i].count2]].zip << '\t' << full_list[preferredListIDs[i][2][counterArr[i].count2]].hash << '\t' << full_list[preferredListIDs[i][2][counterArr[i].count2]].timesSwapped << std::endl;
				}

				counterArr[i].count2++;
			}
		}
	
		size_t points = 0;

		for (size_t i = 0; i < client_req_list.size(); i++) { 
			if (counterArr[i].countTotal >= client_req_list[i].numOfNames ) { 
				points++; 
			} else if (counterArr[i].count0 >= preferredListIDs[i][0].size() 
					&& counterArr[i].count1 >= preferredListIDs[i][1].size() 
					&& counterArr[i].count2 >= preferredListIDs[i][2].size()){
					
				points++;
			}
		}

		if (points == client_req_list.size()) { condition = false; }

		iterator++;
	}
	
	//consider checking ZIP code data type first and second memeber according to ZIP code schema
	//*********if (client_req.preferredZipCode != 0) {
	//*********	std::vector<std::vector<int>> splitLists = listSplitter(client_req, full_list);
	//*********
	//*********	for (size_t i = 0; i < splitLists[0].size(); i++) {
	//*********		if (listToReturn.size() < static_cast<size_t>(client_req.numOfNames)) {
	//*********			if (!(std::find(hash.begin(), hash.end(), full_list[splitLists[0][i]].hash) != hash.end()) && full_list[splitLists[0][i]].timesSwapped < 3) {
	//*********				full_list[splitLists[0][i]].timesSwapped++;
	//*********				listToReturn.push_back(full_list[splitLists[0][i]]);
	//*********			}
	//*********		} else {
	//*********			break;
	//*********		}
	//*********	}
	//*********
	//*********	for (size_t i = 0; i < splitLists[1].size(); i++) {
	//*********		if (listToReturn.size() < static_cast<size_t>(client_req.numOfNames)) {
	//*********			if (!(std::find(hash.begin(), hash.end(), full_list[splitLists[1][i]].hash) != hash.end()) && full_list[splitLists[1][i]].timesSwapped < 3) {
	//*********				full_list[splitLists[1][i]].timesSwapped++;
	//*********				listToReturn.push_back(full_list[splitLists[1][i]]);
	//*********			}
	//*********		}
	//*********		else {
	//*********			break;
	//*********		}
	//*********	}
	//*********
	//*********	for (size_t i = 0; i < splitLists[2].size(); i++) {
	//*********		if (listToReturn.size() < static_cast<size_t>(client_req.numOfNames)) {
	//*********			if (!(std::find(hash.begin(), hash.end(), full_list[splitLists[2][i]].hash) != hash.end()) && full_list[splitLists[2][i]].timesSwapped < 3) {
	//*********				full_list[splitLists[2][i]].timesSwapped++;
	//*********				listToReturn.push_back(full_list[splitLists[2][i]]);
	//*********			}
	//*********		}
	//*********		else {
	//*********			break;
	//*********		}
	//*********	}
	//*********} else {
	//*********	for (size_t i = 0; i < listToShuffle.size(); i++) {
	//*********		if (listToReturn.size() < static_cast<size_t>(client_req.numOfNames)) {
	//*********			if (!(std::find(hash.begin(), hash.end(), listToShuffle[i].hash) != hash.end()) && full_list[listToShuffle[i].id].timesSwapped < 3) {
	//*********				full_list[listToShuffle[i].id].timesSwapped++;
	//*********				listToReturn.push_back(listToShuffle[i]);
	//*********			}
	//*********		}
	//*********		else {
	//*********			break;
	//*********		}
	//*********	}
	//*********}
}

static zipBounds getStateZipBounds(const int& zip) {
	zipBounds returnVal;

	int lowerBounds[] = { 
		35004, 99501, 71601, 85001, 90001,
		80001, 6001, 19701, 32003, 30002,
		96701, 83201, 60001, 46001, 50001,
		66002, 40003, 70001, 3901, 20601,
		1001, 48001, 55001, 38601, 63001,
		59001, 68001, 88901, 3031, 7001,
		87001, 501, 27006, 58001, 43001,
		73001, 97001, 15001, 2801, 29001,
		57001, 37010, 73301, 84001, 5001,
		20101, 98001, 24701, 53001, 82001,
		20001, 920, 801, 96950, 96910, 96799
	};

	int upperBounds[] = { 
		36925, 99950, 72959, 86556, 96162,
		81658, 6928, 19980, 34997, 39901,
		96898, 83877, 62999, 47997, 52809,
		72959, 42788, 71497, 4992, 21930,
		5544, 49971, 56763, 39776, 65899,
		59937, 69367, 89883, 3897, 8989,
		88439, 14925, 28909, 58856, 45999,
		74966, 97920, 19640, 2940, 29945,
		57799, 38589, 88595, 84791, 5907,
		26886, 99403, 26886, 54990, 83414,
		20456, 962, 851, 96952, 96932, 96799
	};

	for (short int i = 0; i < 56; i++) {
		if (zip >= lowerBounds[i] && zip <= upperBounds[i]) {
			returnVal.lower = lowerBounds[i];
			returnVal.upper = upperBounds[i];
		}
	}
	
	return returnVal;
}