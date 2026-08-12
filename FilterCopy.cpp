// #include "stdafx.h"
#include "FilterCopy.h"
#include "utils_mfc.h"

//////////////////////////////////////////////////////////////////////////
CFilterCopy::CFilterCopy():CPackagerFilter()
{
	m_Type = FILTER_COPY;
	DefaultSettings();
}

//////////////////////////////////////////////////////////////////////////
CFilterCopy::~CFilterCopy(void)
{
}

//////////////////////////////////////////////////////////////////////////
HRESULT CFilterCopy::DefaultSettings()
{
	HRESULT ret = CPackagerFilter::DefaultSettings();
	if(FAILED(ret)) return ret;

	m_Active = true;

	m_Masks.Add("*.avi");

	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
CString CFilterCopy::GetSection()
{
	return "Filter:Copy";
}


//////////////////////////////////////////////////////////////////////////
CPackagerFilter::TProcessedType CFilterCopy::ProcessFile(CString FullFilename, CString Filename, CString OutputPath, CString& NewFilename, BYTE* Buffer, DWORD Size, BYTE** NewBuffer, DWORD* NewSize)
{
	NewFilename = Filename;
	*NewSize = 0;
	*NewBuffer = NULL;

	CString TargetFile = OutputPath + Filename;
	//MakePath(TargetFile);

	printf("FilterCopy: ignore copy command from %s to %s.\n", FullFilename.c_str(), TargetFile.c_str());
	
	//if(::CopyFile(FullFilename, TargetFile, FALSE))
	//	return CPackagerFilter::PROC_IGNORE;
	//else
	//	return CPackagerFilter::PROC_ERROR;
	return CPackagerFilter::PROC_IGNORE;
}
