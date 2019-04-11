//////////////////////////////////////////////////////////////////////////////////
// Client.cpp     : To demonstrate all the requirements of this project         //
// ver 1.0																		//
//																				//
// Application    : CSE-775 Project 1					                        //
// Platform       : Visual Studio 2017 Community Edition                        //
//                  Windows 10 Professional 64-bit, Acer Aspire R5-571TG        //
// Author         : Jaskaran Singh, EECS Department, Syracuse University        //
//                  315-925-9941, jsingh09@syr.edu							    //
//////////////////////////////////////////////////////////////////////////////////
/*
 * Package Operations:
 * -------------------
 * This package demonstrates all the functionalities of this project.
 *
 * Required File:
 * --------------
 * FileMgrATL_i.h							//FileMgr header file created by ATL Libraries
 * TextSearchATL_i.h						//TextSearch header file created by ATL Libraries
 * CodeUtilities.h, CodeUtilities.cpp		//Helper class to process command line arguments
 * StringUtilities.h, StringUtilities.cpp	//Helper class for string convertions
 *
 * Maintenance History:
 * --------------------
 *
 * -April 7th, 2019
 *  first release
 */

#pragma once

#include <atlbase.h>
#include <atlsafe.h>
#include <iostream>
#include <string>
#include "../FileMgrATL/FileMgrATL_i.h"
#include "../TextSearchATL/TextSearchATL_i.h"
#include "../CodeUtilities/CodeUtilities.h"
#include "../StringUtilities/StringUtilities.h"
#include <conio.h>

using namespace Utilities;

//A function which is invoked in case user pass invalid commandline arguments
std::string customUsage()
{
	std::string usage;
	usage += "\n  Command Line: path [/pattern]* [/Regex for String]";
	usage += "\n    path is relative or absolute path where processing begins";
	usage += "\n    [pattern]* are one or more pattern strings of the form:";
	usage += "\n      *.h *.cpp *.cs *.txt or *.*";
	usage += "\n    [/Regex] is a string to be search in the set of given files";
	usage += "\n";
	return usage;
}

//A function to process commandline arguments and to display them on the console
void processCmdLine(ProcessCmdLine &pcl) {

	pcl.usage(customUsage());

	preface("Command Line:   "); pcl.showCmdLine();
	preface("path:           "); pcl.showPath();
	preface("Patterns:       "); pcl.showPatterns();
	preface("Regexes:        "); pcl.showRegexes();
	std::cout << "\n";
	putline();
}

//A function to convert commandline arguments into CComBSTR
void setArguments(ProcessCmdLine &cmdargs, CComBSTR &path_, CComBSTR &regex_, CComBSTR &patterns_) {
	std::string t1 = cmdargs.path();
	char *ct1 = new char[t1.length() + 1];
	strcpy_s(ct1, t1.length() + 1, t1.c_str());
	path_ = CComBSTR(ct1);
	delete[] ct1;

	for (size_t i = 0; i < cmdargs.regexes().size(); i++) {
		std::string t2 = cmdargs.regexes()[i];
		char *ct2 = new char[t2.length() + 1];
		strcpy_s(ct2, t2.length() + 1, t2.c_str());
		regex_.Append(ct2);
		regex_.Append("\n");
		delete[] ct2;
	}

	for (size_t i = 0; i < cmdargs.patterns().size(); i++) {
		std::string t3 = cmdargs.patterns()[i];
		char *ct3 = new char[t3.length() + 1];
		strcpy_s(ct3, t3.length() + 1, t3.c_str());
		patterns_.Append(ct3);
		patterns_.Append(" ");
		delete[] ct3;
	}
}

//A function to display result on to the console
void parseOutput(BSTR result) {
	CW2A result_ = result;
	std::string resultS = std::string(result_).c_str();
	std::cout << "\n" << resultS;
}

//Main function to show the functionalities of the project
int main(int argc, char** argv) {
	std::cout << "\n================= Client Demonstration ======================";
	ProcessCmdLine cmdargs(argc, argv);
	cmdargs.usage(customUsage());
	if (cmdargs.parseError())
	{
		std::cout << "\n Command line parse error";
		cmdargs.usage();
		std::cout << "\n\n";
		_getche();
		return -1;
	}

	processCmdLine(cmdargs);
	CComBSTR regex_, path_, patterns_;
	setArguments(cmdargs, path_, regex_, patterns_);

	CoInitialize(NULL); 
	{
		CComQIPtr<IFileMgr> pFileMgr;
		pFileMgr.CoCreateInstance(CLSID_FileMgr);
		if (pFileMgr) {
			pFileMgr->getArguments(path_, patterns_);
		}
		else
			std::cout << "\n Failed to load FileMgr component";
		
		CComQIPtr<ITextSearch> pTextSearch;
		pTextSearch.CoCreateInstance(CLSID_TextSearch);
		if (pTextSearch) {
			pTextSearch->getIUknownP(pFileMgr);
			BSTR result_;
			pTextSearch->searchFile(regex_, &result_);
			parseOutput(result_);
		}
		else
			std::cout << "\n Failed to load TextSearch component";
	}
	CoUninitialize();
	_getche();
	return 0;
}