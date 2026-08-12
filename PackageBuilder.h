// PackageBuilder.h: interface for the CPackageBuilder class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PACKAGEBUILDER_H__0CE0EEB1_9089_47F2_A110_3ADE72F73954__INCLUDED_)
#define AFX_PACKAGEBUILDER_H__0CE0EEB1_9089_47F2_A110_3ADE72F73954__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "wintypes.h"
#include "coll_templ.h"
#include "Package.h"

// class CProjectDoc;
class CCompileDlg;
class CPackagerFilter; 

class CPackageBuilder  
{
public:	
	// bool AppendFiles(CString File1, CString File2);
	typedef struct{
		CString Name;
		CString FullName;
		CString ShortName;
		DWORD Length;
		DWORD CompressedLength;
		DWORD PackageOffset;
		DWORD Flags;
		DWORD TimeDate1;
		DWORD TimeDate2;
		bool Valid;
	} TFile;


	typedef struct{
		CString Name;
		CString FullName;	
		BYTE Priority;
		BYTE CD;
		int GameVersion;
		CString Description;
		CBArray<TFile*, TFile*> m_Files;
	} TPackage;

	static CString Copyright;
	// void DeleteAllPAckages(CString Path, CPackage* SinglePackage);
	bool CreatePackage(TPackage* Package, void* /*CCompileDlg*/ dlg, CString OutputPath, int build_number);
	// bool CreateMasterPackage(void* /*CCompileDlg*/ dlg, CString OutputPath);
	void Cleanup();
	bool GetAllFiles(TPackage* Package, CString Path, int build_number);
	int m_TotalFiles;
	int m_ProcessedFiles;
	bool Compile(const char *inputFolder, const char *outputFolder, const char *outputName, int build_number);
	CPackageBuilder();
	virtual ~CPackageBuilder();
	// CProjectDoc* m_Doc;
	CBArray<TPackage*, TPackage*> m_Packages;
	CBArray<CPackagerFilter*, CPackagerFilter*> m_Filters;
	void WriteString(FILE* f, const char* str, BYTE xor_val=0);
	// bool AddGDF(CString ExeName);
	static bool PackageFileSortFunction(const TFile *first, const TFile *second);
};

#endif // !defined(AFX_PACKAGEBUILDER_H__0CE0EEB1_9089_47F2_A110_3ADE72F73954__INCLUDED_)
