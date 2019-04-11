//////////////////////////////////////////////////////////////////////////////////////////////////////////
// TextSearch.cpp : Implementation of TextSearch component												//
// ver 1.0																								//																				//
// Application    : CSE-775 Project 1																	//
// Platform       : Visual Studio 2017 Community Edition												//	
//                  Windows 10 Professional 64-bit, Acer Aspire R5-571TG								//
// Author         : Jaskaran Singh, EECS Department, Syracuse University								//
//                  315-925-9941, jsingh09@syr.edu														//
//////////////////////////////////////////////////////////////////////////////////////////////////////////


// TextSearch.cpp : Implementation of CTextSearch
#pragma once

#include "stdafx.h"
#include "TextSearch.h"
#include "../FileMgrATL/FileMgrATL_i.h"
#include <regex>
#include <fstream>
#include <iostream>

// CTextSearch
using namespace std;
using namespace CsHelper;

//A function which gets pointer to Iunknown of FileMgr
STDMETHODIMP CTextSearch::getIUknownP(IUnknown* pIUFileMgr)
{
	// TODO: Add your implementation code here
	p2FileMgr = pIUFileMgr;
	return S_OK;
}

//A function which a string which contains set of files into a vector of string
std::vector<std::string> infile_parse(std::string stdFiles)
{
	int f = 0, temp = 0;
	std::vector<std::string> files;
	string file;
	while (f < int(stdFiles.length()))
	{
		int search = stdFiles.find("\\n");
		file = stdFiles.substr(f, search);
		files.push_back(file);
		temp++;
		f += search + 1;
		if (f >= int(stdFiles.length() - 1))
			break;
		stdFiles = stdFiles.substr(f + 1, stdFiles.length());
		f = 0;
	}
	return files;
}

//A function which searches for a given regular expreession in a given file, If found adds it to the hash map
void CTextSearch::handleRegularExp(std::string regularExp, std::string text, size_t lineno, std::string file) {
	smatch match;
	regex expr(regularExp);

	if (regex_search(text, match, expr)) {
		auto findFile = CTextSearch::locations_.find(file);
		if (findFile == CTextSearch::locations_.end())
		{
			std::vector<int> temp;
			temp.push_back(lineno);
			CTextSearch::locations_.insert(std::pair<std::string, std::vector<int>>(file, temp));
		}
		else
		{
			findFile->second.push_back(lineno);
		}
	}
}

//A function to handle doc files, uses managed class1 (CsHelper package)
void CTextSearch::handleDocFiles(std::string file, List<int>^ loc) {
	auto findFile = CTextSearch::locations_.find(file);
	if (findFile == CTextSearch::locations_.end())
	{
		std::vector<int> temp;
		for(int i=0; i<loc->Count; i++)
			temp.push_back(loc[i]);
		CTextSearch::locations_.insert(std::pair<std::string, std::vector<int>>(file, temp));
	}
	else
	{
		for (int i = 0; i < loc->Count; i++)
			findFile->second.push_back(loc[i]);
	}
}

//A function to open set of given files using ifstream and searches for a given regular expression
void CTextSearch::searchText(std::vector<std::string> files, std::string regularExp)
{
	ifstream in;
	string line;
	size_t lineno;
	for (size_t i = 0; i < files.size(); i++) {
		lineno = 0;
		size_t doc = files[i].find("doc");
		if (doc != files[i].npos) {
			CsHelper::class1^ obj = gcnew CsHelper::class1();
			String^ file = gcnew String(files[i].c_str());
			String^ reg = gcnew String(regularExp.c_str());
			List<int>^ loc = gcnew List<int>();
			loc = obj->searchDoc(file,reg);
			handleDocFiles(files[i], loc);
		}
		in.open(files[i]);
		if (!in.good()) {
			cout << "\n in.good = false for file:" << files[i];
			continue;
		}
		while (getline(in, line)) {
			lineno++;
			handleRegularExp(regularExp, line, lineno, files[i]);
		}
		in.close();
	}
}

//A helper function which gets files from FileMgr, convert result into BSTR, convert BSTR to CComBSTR
STDMETHODIMP CTextSearch::searchFile(BSTR searchString, BSTR* result)
{
	// TODO: Add your implementation code here
	CComQIPtr<IFileMgr> ptrIFile = p2FileMgr;

	if (!ptrIFile)
		return E_NOINTERFACE;

	CW2A searchString_ = searchString;
	std::string stdsearchString_ = std::string(searchString_).c_str();
	size_t find = stdsearchString_.find("\n");
	if (find != stdsearchString_.npos)
		stdsearchString_ = stdsearchString_.substr(0, find);

	CComBSTR files_;
	ptrIFile->getAllFiles(&files_);
	if (files_ == NULL) {
		std::cout << "\n No files found matching the given patterns";
		std::cout << "\n Exiting now!!!";
		getchar();
		exit(0);
	}
	CW2A files__ = files_;
	std::string stdFiles = std::string(files__).c_str();
	std::vector<std::string> files = infile_parse(stdFiles);

	CComBSTR result_;
	searchText(files, stdsearchString_);
	auto iter = locations_.begin();
	while (iter != locations_.end()) {
		std::string name = iter->first;
		name += ":";
		CComBSTR t1(name.c_str());
		result_.Append(t1);
		std::string lines = "";
		for (size_t i = 0; i < iter->second.size(); i++) {
			size_t lineno = iter->second[i];
			lines += to_string(lineno);
			lines += " ";
		}
		CComBSTR t2(lines.c_str());
		result_.Append(t2);
		result_.Append("\n");
		iter++;
	}

	*result = result_.Detach();
	return S_OK;
}
