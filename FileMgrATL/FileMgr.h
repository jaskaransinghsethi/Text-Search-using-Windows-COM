//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// FileMgr.h      : A component responsible for searching all files matching a given pattern in a given path	//
// ver 1.0																										//																				//
// Application    : CSE-775 Project 1																			//
// Platform       : Visual Studio 2017 Community Edition														//	
//                  Windows 10 Professional 64-bit, Acer Aspire R5-571TG										//
// Author         : Jaskaran Singh, EECS Department, Syracuse University										//
//                  315-925-9941, jsingh09@syr.edu																//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 * Package Operations:
 * -------------------
 * This componenet is responsible for searching all files matching a given pattern in a given path.
 * It then stores it in a BSTR *result.
 *
 * Public Interface:
 * -----------------
 * CComQIPtr<IFileMgr> pFileMgr;							//Creating a pointer of IFileMgr type
 * pFileMgr.CoCreateInstance(CLSID_FileMgr);				//Creating an instance of FileMgr component which is pointed by pTextSearch
 * pFileMgr->getArguments(path_, patterns_);				//A function to set a given path and a pattern in a FileMgr component 
 * ptrIFile->getAllFiles(&files_);							//A function which searches for all files in a given path matching a given pattern
 *
 * Required File:
 * --------------
 * FileMgr.h, FileMgr.cpp
 * dllmain.h, dllmain.cpp
 * Resource.h
 * stdafx.h, stdafx.cpp
 * targetver.h
 * FileMgrATL.cpp, FileMgrATL.idef, FileMgrATL.idl
 * FileMgrATL_i.h
 * FileSystem.h, FileSystem.cpp
 * Maintenance History:
 * --------------------
 * -April 7th, 2019
 *  first release
 */

// FileMgr.h : Declaration of the CFileMgr

#pragma once
#include "resource.h"       // main symbols
#include "FileSystem.h"
#include "FileMgrATL_i.h"



#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

using namespace ATL;


// CFileMgr

class ATL_NO_VTABLE CFileMgr :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CFileMgr, &CLSID_FileMgr>,
	public IDispatchImpl<IFileMgr, &IID_IFileMgr, &LIBID_FileMgrATLLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CFileMgr()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_FILEMGR)


BEGIN_COM_MAP(CFileMgr)
	COM_INTERFACE_ENTRY(IFileMgr)
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



	STDMETHOD(getArguments)(BSTR path, BSTR patterns);
	STDMETHOD(getAllFiles)(BSTR* files);

private:
	FileSystem::FileSystemSearch fs;
	CComBSTR path_;
	CComBSTR  patterns_;
};

OBJECT_ENTRY_AUTO(__uuidof(FileMgr), CFileMgr)
