// PackageBuilder.cpp: implementation of the CPackageBuilder class.
//
//////////////////////////////////////////////////////////////////////

// #include "stdafx.h"
// #include "projectman.h"
#include "PackageBuilder.h"

// #include "ProjectDoc.h"
#include "PackagerFilter.h"
// #include "CompileDlg.h"
// #include "utils_mfc.h"
// #include <Shlwapi.h>
#include "./engine_core/wme_base/dcpackage.h"
#include "zlib.h"
// #include "UtilIcon.h"
#include "Package.h"
#include <algorithm>
#include <cstring>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define LOC(X) X

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CString CPackageBuilder::Copyright = "    (Wintermute Engine © DEAD:CODE 2013)";


CPackageBuilder::CPackageBuilder()
{
	// m_Doc = Doc;
	m_TotalFiles = 0;
	m_ProcessedFiles = 0;
}


//////////////////////////////////////////////////////////////////////////
CPackageBuilder::~CPackageBuilder()
{
	Cleanup();
}


//////////////////////////////////////////////////////////////////////////
void CPackageBuilder::Cleanup()
{
	for(int i=0; i<m_Packages.GetSize(); i++){
		for(int j=0; j<m_Packages[i]->m_Files.GetSize(); j++){
			delete m_Packages[i]->m_Files[j];
		}
		m_Packages[i]->m_Files.RemoveAll();

		delete m_Packages[i];
	}
	m_Packages.RemoveAll();

	m_TotalFiles = 0;
	m_ProcessedFiles = 0;
}



//////////////////////////////////////////////////////////////////////////
bool CPackageBuilder::Compile(const char *inputFolder, const char *outputFolder, const char *outputName, int build_number)
// bool CPackageBuilder::Compile(CPackage* SinglePackage, char *outputFolder, char *toolsFolder, bool addCrashLib, bool enableLogWriting, int build_number)
{
	int i;
	bool ret = true;

	CString NewExeName = "";


	/*
	CCompileDlg dlg;
	AfxGetMainWnd()->EnableWindow(FALSE);

	dlg.Create(IDD_COMPILE);
	dlg.ShowWindow(SW_SHOW);

	dlg.m_Title1.SetWindowText("");
	dlg.m_Title2.SetWindowText("");
	dlg.m_Progress.SetPos(0);
	*/

	//	CString OutputPath = m_Doc->m_PackOutputFolder;
#if 0	
	CString OutputPath = CString(outputFolder);
	if(OutputPath[OutputPath.GetLength()-1]!='\\') OutputPath+="\\";
	if(PathIsRelative(OutputPath)){
		char Temp[MAX_PATH];
		printf("Path to canonicalize: '%s' + '%s'.\n", m_Doc->m_ProjectRoot, OutputPath);
		if(PathCanonicalize(Temp, m_Doc->m_ProjectRoot + OutputPath)) OutputPath = CString(Temp);
		printf("Path after canonicalize: '%s'.\n", OutputPath);
	}

	// prepare output
	printf("/str0108/Preparing output folder '%s' ...\n", OutputPath);
	// dlg.Update();
	if(MakePath(OutputPath)==""){
		printf(CString(LOC("/str0109/Cannot create output folder")) + " '" + OutputPath + "'\n");
		ret = false;
		goto finish;
	}
#endif

	// DeleteAllPAckages(OutputPath, SinglePackage);

	// prepare packages/files
	Cleanup();
	
	printf(LOC("/str0110/Preparing build...\n"));
	//dlg.Update();	

	bool IsNT = false;

#if 0
	OSVERSIONINFO osvi;
	ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx((OSVERSIONINFO*) &osvi);
	IsNT = (osvi.dwPlatformId==VER_PLATFORM_WIN32_NT);
#endif

	printf("IsNT=%s.\n", (IsNT == true) ? "TRUE" : "FALSE");

#if 0
	if(!SinglePackage)
	{
		// copy WMELITE exe
		if(m_Doc->m_PackCopyExe){
			NewExeName = OutputPath + m_Doc->m_PackExeName;
			if(GetExt(NewExeName).CompareNoCase("EXE")!=0) NewExeName += ".exe";

			if(!::CopyFile(CString(toolsFolder) + "\\wme_D3D9.exe", NewExeName, FALSE)){
				printf(LOC("/str0148/Error copying the engine runtime to the output folder\n"));
			}
			else if(m_Doc->m_PackChangeIcon)
			{
				// change icon
				if(!IsNT)
				{
					printf(LOC("/str0149/Icon changing is not supported on this Windows version\n"));
				}
				else{

					CString IconName = m_Doc->m_PackIconName;
					if(PathIsRelative(IconName)){
						char Temp[MAX_PATH];
						if(PathCanonicalize(Temp, m_Doc->m_ProjectRoot + IconName)) IconName = CString(Temp);
					}


					HANDLE h = BeginUpdateResource(NewExeName, FALSE);
					if(h){
						if(!AddIconToRes(h, IconName, 1, 101))printf("/str0150/Error changing icon - function call failed, icon file '%s'\n", IconName);
						EndUpdateResource(h, FALSE);
					}
					else printf("/str0150/Error changing icon - no handle to '%s'\n", NewExeName);
				}
			}
		}

		// copy WME exe as 2nd option
		if(m_Doc->m_PackCopyExe){
			NewExeName = OutputPath + "alternate" + m_Doc->m_PackExeName;
			if(GetExt(NewExeName).CompareNoCase("EXE")!=0) NewExeName += ".exe";

			if(!::CopyFile(CString(toolsFolder) + "\\wme.exe", NewExeName, FALSE)){
				printf(LOC("/str0148/Error copying the engine runtime to the output folder\n"));
			}
			else if(m_Doc->m_PackChangeIcon)
			{
				// change icon
				if(!IsNT)
				{
					printf(LOC("/str0149/Icon changing is not supported on this Windows version\n"));
				}
				else{

					CString IconName = m_Doc->m_PackIconName;
					if(PathIsRelative(IconName)){
						char Temp[MAX_PATH];
						if(PathCanonicalize(Temp, m_Doc->m_ProjectRoot + IconName)) IconName = CString(Temp);
					}


					HANDLE h = BeginUpdateResource(NewExeName, FALSE);
					if(h){
						if(!AddIconToRes(h, IconName, 1, 101))printf("/str0150/Error changing icon - function call failed, icon file '%s'\n", IconName);
						EndUpdateResource(h, FALSE);
					}
					else printf("/str0150/Error changing icon, no handle to '%s'\n", NewExeName);
				}
			}
		}

		// copy settings.exe
		if(m_Doc->m_PackCopySet)
		{
			CString NewSetName = OutputPath + m_Doc->m_PackSetName;
			if(GetExt(NewSetName).CompareNoCase("EXE")!=0) NewSetName += ".exe";

			CString ToolsPath = toolsFolder;
			if (ToolsPath.GetLength() < 1) {
				printf("Tools path not found!\n");
			} else {
				if(ToolsPath[ToolsPath.GetLength()-1]!='\\') ToolsPath+="\\";
				ToolsPath+="settings.exe";

				if(!::CopyFile(ToolsPath, NewSetName, FALSE))
				{
					printf(LOC("/str1162/Error copying settings.exe to the output folder\n"));
				}
				else if(m_Doc->m_PackChangeIconSet)
				{
					// change icon
					if(!IsNT)
					{
						printf(LOC("/str0149/Icon changing is not supported on this Windows version\n"));
					}
					else
					{
						CString IconName = m_Doc->m_PackIconNameSet;
						if(PathIsRelative(IconName)){
							char Temp[MAX_PATH];
							if(PathCanonicalize(Temp, m_Doc->m_ProjectRoot + IconName)) IconName = CString(Temp);
						}

						HANDLE h = BeginUpdateResource(NewSetName, FALSE);
						if(h){
							if(!AddIconToRes(h, IconName, 1, 101))printf("/str0150/Error changing icon - function call failed, icon file '%s'\n", IconName);
							EndUpdateResource(h, FALSE);
						}
						else printf("/str0150/Error changing icon - no handle to '%s'\n", NewSetName);
					}
				}	
			}
		}

		// copy D3DX
		if(m_Doc->m_PackCopyD3DX)
		{
			CString TargetFile = OutputPath + "d3dx9_34.dll";

			CString ToolsPath = GetRegString(HKEY_CURRENT_USER, DCGF_TOOLS_REG_PATH, "ToolsPath");
			if (ToolsPath.GetLength() < 1) {
				printf("Tools path not found!\n");
			} else {
				if(ToolsPath[ToolsPath.GetLength()-1]!='\\') ToolsPath+="\\";
				ToolsPath+="d3dx9_34.dll";

				if(!::CopyFile(ToolsPath, TargetFile, FALSE))
				{
					printf(LOC("/str1185/Error copying D3DX library to the output folder"));
				}
			}
		}

		// GDF data
		if(m_Doc->m_PackAddGeData)
		{
			if(!IsNT)
			{
				printf(LOC("/str1168/Inserting Game Explorer data is not supported on this Windows version"));
			}
			else AddGDF(NewExeName);
		}


		// copy plugins
		for(i=0; i<m_Doc->m_PluginList.GetSize(); i++)
		{
			// find plugin
			bool PluginFound = false;
			for(int j=0; j<m_Doc->Game->m_PluginMgr->m_Plugins.GetSize(); j++)
			{
				CBPlugin* Plugin = m_Doc->Game->m_PluginMgr->m_Plugins[j];
				CString ShortName = GetFName(Plugin->m_DllPath);

				if(m_Doc->m_PluginList[i].CompareNoCase(ShortName)==0)
				{
					if(TRUE==::CopyFile(Plugin->m_DllPath, OutputPath + ShortName, FALSE))
					{
						PluginFound = true;
						printf(LOC("/str1124/Copying plugin") + CString(" ") + ShortName + "\n");
					}
					break;
				}
			}
			if(!PluginFound)printf(LOC("/str1125/Error copying plugin") + CString(" ") + m_Doc->m_PluginList[i] + "\n");
		}

		// debugging stuff
		if(addCrashLib)
		{
			CString TargetFile = OutputPath + "wme_report.dll";

			CString ToolsPath = toolsFolder;
			if(ToolsPath[ToolsPath.GetLength()-1]!='\\') ToolsPath+="\\";
			ToolsPath+="wme_report.dll";

			if(!::CopyFile(ToolsPath, TargetFile, FALSE))
			{
				printf(LOC("/str1176/Error copying wme_report.dll to the output folder\n"));
			}
		}
		if(m_Doc->m_PackAddDebugMode || m_Doc->m_PackAddFpsDisplay || enableLogWriting)
		{
			CString IniName = OutputPath + "wme.ini";
			if(m_Doc->m_PackAddFpsDisplay)
				WritePrivateProfileString("Debug", "ShowFPS", "1", IniName);
			if(m_Doc->m_PackAddDebugMode)
				WritePrivateProfileString("Debug", "DebugMode", "1", IniName);
			if (enableLogWriting)
				WritePrivateProfileString("Debug", "LogWriteMode", "1", IniName);
		}
	} // SinglePackage
	
#endif

#if 0
	// get packages
	for(i=0; i<m_Doc->m_Packages.GetSize(); i++)
	{
		if(SinglePackage && m_Doc->m_Packages[i] != SinglePackage) continue;

		TPackage* package = new TPackage;
		package->Name = m_Doc->m_Packages[i]->m_Folder;
		package->CD = m_Doc->m_Packages[i]->m_CD;
		package->Priority = m_Doc->m_Packages[i]->m_Priority;
		package->Description = m_Doc->m_Packages[i]->m_Description;
		if(package->Description.GetLength() + Copyright.GetLength()<100) package->Description += Copyright;
		package->GameVersion = m_Doc->m_PackGameVersion;
		package->FullName = m_Doc->m_ProjectRoot + m_Doc->m_Packages[i]->m_Folder + "\\";

		GetAllFiles(package, package->FullName, build_number);
		if(package->m_Files.GetSize()>0) m_Packages.Add(package);
		else{
			printf(CString(LOC("/str0111/Package")) + " '" + package->Name + "': " + LOC("/str0112/no files to add\n"));
			delete package;
		}
	}
#else
	TPackage* package = new TPackage;
	package->Name = CString(const_cast<char*>(outputName));
	package->CD = 0;
	package->Priority = 0; // TBD
	package->Description = Copyright;
	package->GameVersion = 0; // TBD
	package->FullName = CString(const_cast<char*>(inputFolder));
	GetAllFiles(package, package->FullName, build_number);
	if(package->m_Files.GetSize()>0) m_Packages.Add(package);
	else{
		printf("%s: no files to add\n", (package->Name).c_str());
		delete package;
	}
#endif

	/*
	if(dlg.m_CloseRequest){
		m_Doc->AddWarning(LOC("/str0113/Cancelled by user"));
		goto finish;
	}
	*/
	
	if(m_Packages.GetSize()==0){
		printf(LOC("/str0114/All packages are empty.\n"));
		goto finish;
	}


	// dlg.m_Progress.SetRange32(0, m_TotalFiles);

	// build packages
	for(i=0; i<m_Packages.GetSize(); i++){		
		ret = CreatePackage(m_Packages[i], NULL/* &dlg */, CString(const_cast<char*>(outputFolder)), build_number);
		if(!ret) break;

		//if(m_Doc->m_PackCopyExe && m_Doc->m_BindPackage==m_Packages[i]->Name){
		//	AppendFiles(NewExeName, OutputPath + m_Packages[i]->Name + "." + PACKAGE_EXTENSION);
		//}

		/*
		if(dlg.m_CloseRequest){
			printf(LOC("/str0113/Cancelled by user"));
			goto finish;
		}
		*/
	}

	// build master index
//	if(ret && m_Doc->m_PackBuildMasterIndex)
//		ret = CreateMasterPackage(NULL/*&dlg */, OutputPath);




finish:
	//dlg.EndDialog(IDOK);
	//dlg.DestroyWindow();
	//AfxGetMainWnd()->EnableWindow(TRUE);
	Cleanup();

	if(!ret) printf("Package build ERROR!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n"); // DeleteAllPAckages(CString(const_cast<char*>(outputFolder)), SinglePackage);
	else printf(LOC("/str0115/Packages created successfuly\n"));
	// MessageBeep(ret?MB_OK:MB_ICONERROR);

	return ret;
}

//////////////////////////////////////////////////////////////////////////
bool CPackageBuilder::GetAllFiles(TPackage *Package, CString Path, int build_number)
{
	if(Path[Path.GetLength()-1]!='\\') Path+="\\";

	CFileFind finder;
	BOOL working = finder.FindFile(Path + "*.*");
	while(working){
		working = finder.FindNextFile();
		if(finder.IsDots()) continue;

		if(finder.IsDirectory()) GetAllFiles(Package, Path + finder.GetFileName() + "\\", build_number);
		else{
			m_TotalFiles++;
			TFile* file = new TFile;
			file->PackageOffset = 0;
			file->Length = 0;
			file->CompressedLength = 0;
			file->Flags = 0;
			file->FullName = finder.GetFilePath();
			file->ShortName = finder.GetFileName();
			file->Name = file->FullName.Right(file->FullName.GetLength() - Package->FullName.GetLength());
			file->Valid = true;

			if (build_number == 0)
			{
				CTime time;
				finder.GetLastWriteTime(time);
				file->TimeDate1 = time.GetTime();
			}
			else
			{
				file->TimeDate1 = (DWORD) build_number;
			}
			file->TimeDate2 = 0;

			Package->m_Files.Add(file);
		}
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////
bool CPackageBuilder::PackageFileSortFunction(const TFile *first, const TFile *second)
{
	if (first->FullName < second->FullName)
	{
		return true;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////
bool CPackageBuilder::CreatePackage(TPackage* Package, void* /*CCompileDlg*/ dlg, CString OutputPath, int build_number)
{
	DWORD dw;
	int i;

	// initialize filters
	printf(LOC("/str0116/Initializing filters..."));
	//dlg->m_Title2.SetWindowText("");
	//dlg->Update();
	for(i=0; i<m_Doc->m_Filters.GetSize(); i++) m_Doc->m_Filters[i]->Initialize(Package);


	printf(CString(LOC("/str0117/Package")) + ": " + Package->Name);
	m_Doc->AddInfo(CString(LOC("/str0118/Creating package")) + " '" + Package->Name + "'");
	CString Filename = OutputPath + Package->Name + "." + PACKAGE_EXTENSION;

	char fullfilename[512];
	DWORD ffnlength;

	ffnlength = GetFullPathNameA(Filename, 512, fullfilename, NULL);
	printf("Filename for writing full path '%s'.\n", fullfilename);

	FILE* f = fopen(Filename, "wb");
	if(!f){
		printf(CString(LOC("/str0119/Error opening file")) + " '" + Filename + "' " + LOC("/str0120/for writing"));
		return false;
	}

	// header
	TPackageHeader hdr;

	hdr.Magic1 = PACKAGE_MAGIC_1;
	hdr.Magic2 = PACKAGE_MAGIC_2;
	hdr.PackageVersion = PACKAGE_VERSION;
	hdr.GameVersion = Package->GameVersion;
	hdr.Priority = Package->Priority;
	hdr.CD = Package->CD;
	hdr.MasterIndex = false;
	if (build_number != 0)
	{
		printf("Repeatable build with number=%d.\n", build_number);
		hdr.CreationTime = build_number;
	}
	else
	{
		_time32(&hdr.CreationTime);
		printf("Non-repeatable build with time stamp=%d.\n", hdr.CreationTime);
	}
	memset(hdr.Desc, 0, 100);
	memcpy(&hdr.Desc, LPCSTR(Package->Description), min(99, Package->Description.GetLength()));
	hdr.NumDirs = 1;
	fwrite(&hdr, sizeof(TPackageHeader), 1, f);

	DWORD DirOffsetPosition = ftell(f);
	dw=0;
	fwrite(&dw, sizeof(DWORD), 1, f);

	// process files
	DWORD PackageOffset = ftell(f);

	// sort files according to their path & name to have repeatable builds
	std::sort(Package->m_Files.GetData(), Package->m_Files.GetData() + Package->m_Files.GetSize(), PackageFileSortFunction);

	DWORD NumFilesWritten = 0;
	for(i=0; i<Package->m_Files.GetSize(); i++){
		TFile* File = Package->m_Files[i];

		m_ProcessedFiles++;
		printf(CString(LOC("/str0121/File")) + ": " + File->Name + "\n");
		// dlg->m_Progress.SetPos(m_ProcessedFiles);
		//dlg->Update();
		/*
		if(dlg->m_CloseRequest){
			fclose(f);
			m_Doc->AddWarning(LOC("/str0113/Cancelled by user"));
			return false;
		}
		*/

		// read file
		FILE* entry = fopen(File->FullName, "rb");
		if(!entry){
			fclose(f);
			printf(CString(LOC("/str0122/Cannot open file")) + " '" + File->FullName + "' " + LOC("/str0123/for reading\n"));
			return false;
		}
		fseek(entry, 0, SEEK_END);
		DWORD Size = ftell(entry);
		fseek(entry, 0, SEEK_SET);
		BYTE* Buffer = new BYTE[Size];
		if(!Buffer){
			fclose(entry);
			fclose(f);
			printf(CString(LOC("/str0124/Cannot allocate memory for file")) + " '" + File->FullName + "'\n");
			return false;
		}

		fread(Buffer, Size, 1, entry);
		fclose(entry);


		// apply filters
		CPackagerFilter::TProcessedType Processed = CPackagerFilter::PROC_USE_ORIGINAL;
		CString NewFilename="";
		BYTE* NewBuffer = NULL;
		DWORD NewSize = 0;

		for(int j=0; j<m_Doc->m_Filters.GetSize(); j++){
			CPackagerFilter* Filter = m_Doc->m_Filters[j];
			if(!Filter->m_Active) continue;

			if(Filter->FilenameMatches(File->ShortName)){
				Processed = Filter->ProcessFile(File->FullName, File->Name, OutputPath, NewFilename, Buffer, Size, &NewBuffer, &NewSize);
				break;
			}
		}

		// error applying filter?
		if(Processed==CPackagerFilter::PROC_ERROR){
			fclose(f);
			printf(CString(LOC("/str0125/Error applying filter to file")) + " '" + File->Name + "'\n");
			return false;
		}

		// should we ignore this file?
		if(Processed==CPackagerFilter::PROC_IGNORE){
			File->Valid = false;
			delete [] Buffer;
			continue;
		}

		// should we use new data?
		if(Processed==CPackagerFilter::PROC_USE_BUFFER || Processed==CPackagerFilter::PROC_USE_BUFFER_UNCOMPRESSED){
			delete [] Buffer;
			Buffer = NewBuffer;
			Size = NewSize;
		}

		// Filename changed?
		if(NewFilename!="" && NewFilename!=File->Name) File->Name = NewFilename;

		// compress?
		DWORD CompressedSize = 0;
		if(Processed==CPackagerFilter::PROC_USE_ORIGINAL || Processed==CPackagerFilter::PROC_USE_BUFFER){
			CompressedSize = 2*Size;
			BYTE* CompBuffer = new BYTE[CompressedSize];
			if(!CompBuffer) CompressedSize = 0;
			else{
				if(Z_OK!=compress(CompBuffer, &CompressedSize, Buffer, Size)){
					printf(CString(LOC("/str0126/Error compressing file")) + " '" + File->FullName + "'", File->Name);
					delete [] CompBuffer;
					CompressedSize = 0;
				}
				else{
					if(CompressedSize<Size){
						delete [] Buffer;
						Buffer = CompBuffer;
					}
					else{
						CompressedSize = 0;
						delete [] CompBuffer;
					}

				}
			}

		}

		File->Length = Size;
		File->CompressedLength = CompressedSize;
		File->PackageOffset = PackageOffset;

		if(CompressedSize!=0){
			PackageOffset+=CompressedSize;
			fwrite(Buffer, CompressedSize, 1, f);
		}
		else{
			PackageOffset+=Size;
			fwrite(Buffer, Size, 1, f);
		}
		
		delete [] Buffer;

		NumFilesWritten++;
	}

	// write directory
	DWORD DirOffset = ftell(f);

	WriteString(f, Package->Name);
	fwrite(&Package->CD, sizeof(BYTE), 1, f);

	//dw = Package->m_Files.GetSize();
	//fwrite(&dw, sizeof(DWORD), 1, f);

	fwrite(&NumFilesWritten, sizeof(DWORD), 1, f);
	

	for(i=0; i<Package->m_Files.GetSize(); i++){

		TFile* File = Package->m_Files[i];
		if(!File->Valid) continue;

		WriteString(f, File->Name, 'D');
		fwrite(&File->PackageOffset,    sizeof(DWORD), 1, f);
		fwrite(&File->Length,           sizeof(DWORD), 1, f);
		fwrite(&File->CompressedLength, sizeof(DWORD), 1, f);
		fwrite(&File->Flags,            sizeof(DWORD), 1, f);
		fwrite(&File->TimeDate1,        sizeof(DWORD), 1, f);
		fwrite(&File->TimeDate2,        sizeof(DWORD), 1, f);
	}



	// write dir offset
	fseek(f, DirOffsetPosition, SEEK_SET);
	fwrite(&DirOffset, sizeof(DWORD), 1, f);

	fclose(f);
	return true;
}

#if 0

#define MASTER_NAME "master"
//////////////////////////////////////////////////////////////////////////
bool CPackageBuilder::CreateMasterPackage(void* /*CCompileDlg*/ dlg, CString OutputPath)
{
	DWORD dw;

	printf(CString(LOC("/str0111/Package")) + ": " + CString(MASTER_NAME));
	// dlg->m_Title2.SetWindowText("");
	printf(LOC("/str0127/Creating master index package"));
	CString Filename = OutputPath + MASTER_NAME + "." + PACKAGE_EXTENSION;
	FILE* f = fopen(Filename, "wb");
	if(!f){
		printf(CString(LOC("/str0119/Error opening file")) + " '" + Filename + "' " + LOC("/str0120/for writing"));
		return false;
	}

	CString Desc = LOC("/str0128/Master index");
	if(Desc.GetLength() + Copyright.GetLength()<100) Desc += Copyright;

	// header
	TPackageHeader hdr;

	hdr.Magic1 = PACKAGE_MAGIC_1;
	hdr.Magic2 = PACKAGE_MAGIC_2;
	hdr.PackageVersion = PACKAGE_VERSION;
	hdr.GameVersion = m_Doc->m_PackGameVersion;
	hdr.Priority = 0;
	hdr.CD = 0;
	hdr.MasterIndex = true;
	_time32(&hdr.CreationTime);
	memset(hdr.Desc, 0, 100);
	memcpy(&hdr.Desc, LPCSTR(Desc), min(99, Desc.GetLength()));
	hdr.NumDirs = m_Packages.GetSize();
	fwrite(&hdr, sizeof(TPackageHeader), 1, f);

	// dirs offset
	dw = ftell(f) + sizeof(DWORD);
	fwrite(&dw, sizeof(DWORD), 1, f);


	// write directories
	for(int j=0; j<m_Packages.GetSize(); j++){
		TPackage* Package = m_Packages[j];
		
		WriteString(f, Package->Name);
		fwrite(&Package->CD, sizeof(BYTE), 1, f);
		
		dw = Package->m_Files.GetSize();
		fwrite(&dw, sizeof(DWORD), 1, f);
		
		for(int i=0; i<Package->m_Files.GetSize(); i++){
			
			TFile* File = Package->m_Files[i];
			if(!File->Valid) continue;
			
			WriteString(f, File->Name, 'D');
			fwrite(&File->PackageOffset,    sizeof(DWORD), 1, f);
			fwrite(&File->Length,           sizeof(DWORD), 1, f);
			fwrite(&File->CompressedLength, sizeof(DWORD), 1, f);
			fwrite(&File->Flags,            sizeof(DWORD), 1, f);
			fwrite(&File->TimeDate1,        sizeof(DWORD), 1, f);
			fwrite(&File->TimeDate2,        sizeof(DWORD), 1, f);
		}
	}


	fclose(f);
	return true;

}

#endif

//////////////////////////////////////////////////////////////////////////
void CPackageBuilder::WriteString(FILE* f, const char* str, BYTE xor_val)
{
	BYTE b;
	if(str){		
		b = strlen(str)+1;
		BYTE* str_xor = new BYTE[b];
		strcpy((char*)str_xor, str);

		for(int i=0; i<b; i++) str_xor[i] ^= xor_val;
		fwrite(&b, 1, 1, f);
		fwrite(str_xor, b, 1, f);

		delete [] str_xor;
	}
	else{
		b = 0;
		fwrite(&b, 1, 1, f);
	}
}


//////////////////////////////////////////////////////////////////////////
#if 0
void CPackageBuilder::DeleteAllPAckages(CString Path, CPackage* SinglePackage)
{
	CFileFind finder;
	BOOL working = finder.FindFile(Path + "*." + PACKAGE_EXTENSION);
	while(working)
	{
		working = finder.FindNextFile();
		if(finder.IsDots() || finder.IsDirectory()) continue;

		if(SinglePackage)
		{
			CString PackName = SinglePackage->m_Folder + "." + PACKAGE_EXTENSION;
			CString FoundFile = finder.GetFileName();
			if(FoundFile.CompareNoCase(PackName) != 0) continue;
		}

		DeleteFile(finder.GetFilePath());
	}
	DeleteFile(Path + "wme_report.dll");
	DeleteFile(Path + "wme.ini");
}
#endif

//////////////////////////////////////////////////////////////////////////
bool CPackageBuilder::AppendFiles(CString File1, CString File2)
{
	BYTE buf[32768];

	FILE* f1 = fopen(File1, "ab");
	if(!f1) return false;

	FILE* f2 = fopen(File2, "rb");
	if(!f2){
		fclose(f1);
		return false;
	}

	fseek(f1, 0, SEEK_END);

	int BytesRead;
	do{
		BytesRead = fread(buf, 1, 32768, f2);
		if(BytesRead>0){
			fwrite(buf, 1, BytesRead, f1);
		}
	} while(BytesRead>0);

	fclose(f1);
	fclose(f2);

	DeleteFile(File2);

	return true;
}

//////////////////////////////////////////////////////////////////////////

#if 0

bool CPackageBuilder::AddGDF(CString ExeName)
{
	CString GdfFile;
	CString ThumbFile;

	GdfFile = m_Doc->m_PackGdfFile;
	if(PathIsRelative(GdfFile))
	{
		char Temp[MAX_PATH];
		if(PathCanonicalize(Temp, m_Doc->m_ProjectRoot + GdfFile)) GdfFile = CString(Temp);
	}

	ThumbFile = m_Doc->m_PackGdfThumbnail;
	if(PathIsRelative(ThumbFile))
	{
		char Temp[MAX_PATH];
		if(PathCanonicalize(Temp, m_Doc->m_ProjectRoot + ThumbFile)) ThumbFile = CString(Temp);
	}

	FILE* f;
	
	BYTE* GdfBuffer = NULL;
	DWORD GdfSize = 0;
	f = fopen(GdfFile, "rb");
	if(f)
	{
		fseek(f, 0, SEEK_END);
		GdfSize = ftell(f);
		fseek(f, 0, SEEK_SET);

		GdfBuffer = new BYTE[GdfSize];
		if(GdfBuffer) fread(GdfBuffer, GdfSize, 1, f);
		fclose(f);
	}

	BYTE* ThumbBuffer = NULL;
	DWORD ThumbSize = 0;
	f = fopen(ThumbFile, "rb");
	if(f)
	{
		fseek(f, 0, SEEK_END);
		ThumbSize = ftell(f);
		fseek(f, 0, SEEK_SET);

		ThumbBuffer = new BYTE[ThumbSize];
		if(ThumbBuffer) fread(ThumbBuffer, ThumbSize, 1, f);
		fclose(f);
	}

	if(GdfBuffer || ThumbBuffer)
	{
		HANDLE h = BeginUpdateResource(ExeName, FALSE);
		if(h)
		{
			if(!GdfBuffer)
				printf(CString(LOC("/str0122/Cannot open file")) + " '" + GdfFile + "' ");
			else
				UpdateResource(h, "DATA", "__GDF_XML", MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL), GdfBuffer, GdfSize);

			if(!ThumbBuffer)
				printf(CString(LOC("/str0122/Cannot open file")) + " '" + ThumbFile + "' ");
			else
				UpdateResource(h, "DATA", "__GDF_THUMBNAIL", MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL), ThumbBuffer, ThumbSize);

			EndUpdateResource(h, FALSE);
		}		
	}


	SAFE_DELETE_ARRAY(GdfBuffer);
	SAFE_DELETE_ARRAY(ThumbBuffer);

	return false;
}

#endif
