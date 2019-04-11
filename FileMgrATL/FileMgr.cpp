//////////////////////////////////////////////////////////////////////////////////////////////////////////
// TextSearch.cpp : Implementation of FileMgr component													//
// ver 1.0																								//																				//
// Application    : CSE-775 Project 1																	//
// Platform       : Visual Studio 2017 Community Edition												//	
//                  Windows 10 Professional 64-bit, Acer Aspire R5-571TG								//
// Author         : Jaskaran Singh, EECS Department, Syracuse University								//
//                  315-925-9941, jsingh09@syr.edu														//
//////////////////////////////////////////////////////////////////////////////////////////////////////////

// FileMgr.cpp : Implementation of CFileMgr

#include "stdafx.h"
#include "FileMgr.h"
#include <iostream>

// CFileMgr

//A function which sets path and patterns for FileMgr component
STDMETHODIMP CFileMgr::getArguments(BSTR path, BSTR patterns)
{
	// TODO: Add your implementation code here
	path_.Append(path);
	//std::wcout << "\n" << path;
	//std::wcout << "\n" << path_;
	patterns_ = patterns;
	//std::wcout << "\n" << patterns;
	//std::wcout << "\n" << patterns_;
	return S_OK;
}

//A function which finds all the files matching a given pattern in a given path and result it as BSTR
STDMETHODIMP CFileMgr::getAllFiles(BSTR* files)
{
	// TODO: Add your implementation code here
	CW2A path = path_;
	CW2A patterns = patterns_;

	std::string pattern = std::string(patterns).c_str();
	std::string temp;

	std::vector<std::string> patt;

	int i = 0;
	int j = 0;

	while (i < int(pattern.length())) {
		size_t find = pattern.find(" ");
		if (find != pattern.npos) {
			temp = pattern.substr(i, find);
			patt.push_back(temp);
			j++;
			i += find + 1;
			if (i >= int(pattern.length()))
				break;
			pattern = pattern.substr(i, pattern.length());
			i = 0;
		}
		else {
			patt.push_back(pattern);
			break;
		}
	}

	std::vector<std::string> searchfiles;
	std::vector<std::string> pathfs;
	fs.find(searchfiles, std::string(path), patt);

	CComBSTR infiles;
	for (auto SF : searchfiles) {
		SF.append("\\");
		SF.append("n");
		infiles += CComBSTR(SF.c_str());
		std::string IFS = std::string(CW2A(infiles));
		//std::cout << "\nINFILES" << IFS;		
	}
	*files = infiles.Detach();
	return S_OK;
}
