//////////////////////////////////////////////////////////////////////////////////////////////////////////
// TextSearch.h   : A component responsible for searching of a given regex in a set of files			//
// ver 1.0																								//																				//
// Application    : CSE-775 Project 1																	//
// Platform       : Visual Studio 2017 Community Edition												//	
//                  Windows 10 Professional 64-bit, Acer Aspire R5-571TG								//
// Author         : Jaskaran Singh, EECS Department, Syracuse University								//
//                  315-925-9941, jsingh09@syr.edu														//
//////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 * Package Operations:
 * -------------------
 * This componenet is responsible for opening a given set of files by creating an instance of
 * ifstream. It then reads line by line and searches for given
 * regex. If found, adds it to the hash map.
 *
 * Public Interface:
 * -----------------
 * CComQIPtr<ITextSearch> pTextSearch;						//Creating a pointer of ITextSearch type
 * pTextSearch.CoCreateInstance(CLSID_TextSearch);			//Creating an instance of TextSearch component which is pointed by pTextSearch
 * pTextSearch->getIUknownP(pFileMgr);						//Getting pointer to IUknown of FileMgr component
 * pTextSearch->searchFile(regex_, &result_);				//Function to search for a given regular expression in a given set of files
 *
 * Required File:
 * --------------
 * TextSearch.h, TextSearch.cpp	
 * dllmain.h, dllmain.cpp
 * Resource.h
 * stdafx.h, stdafx.cpp
 * targetver.h
 * TextSearchATL.cpp, TextSearchATL.idef, TextSearchATL.idl
 * FileMgrATL_i.h
 *
 * Maintenance History:
 * --------------------
 * -April 7th, 2019
 *  first release
 */


// TextSearch.h : Declaration of the CTextSearch

#pragma once
#include "resource.h"       // main symbols
#include "../FileMgrATL/FileMgrATL_i.h"
#include "TextSearchATL_i.h"
#include <string>
#include <vector>
#include <unordered_map>


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

using namespace ATL;
using namespace CsHelper;
using namespace System;
using namespace System::Collections;
using namespace System::Collections::Generic;

// CTextSearch

class ATL_NO_VTABLE CTextSearch :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CTextSearch, &CLSID_TextSearch>,
	public IDispatchImpl<ITextSearch, &IID_ITextSearch, &LIBID_TextSearchATLLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CTextSearch()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_TEXTSEARCH)


BEGIN_COM_MAP(CTextSearch)
	COM_INTERFACE_ENTRY(ITextSearch)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()



	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

public:

private:
	IUnknown *p2FileMgr;
	std::unordered_map<std::string, std::vector<int>> locations_;
	void handleRegularExp(std::string regularExp, std::string text, size_t lineno, std::string file);
	void handleDocFiles(std::string file, List<int>^ loc);
	void searchText(std::vector<std::string> files, std::string regularExp);
public:
	STDMETHOD(getIUknownP)(IUnknown* pIUFileMgr);
	STDMETHOD(searchFile)(BSTR searchString, BSTR* result);
	
};

OBJECT_ENTRY_AUTO(__uuidof(TextSearch), CTextSearch)
