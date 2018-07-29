
#include "stdafx.h"

#include "ProjectDoc.h"
#include "PackageBuilder.h"

CProjectDoc *doc;
CPackageBuilder *builder;


int main(int argc, char **argv)
{
	bool add_crash_lib = false;
	bool enable_log_file_writing = false;
	int build_number = 0;

	char project_path[MAX_PATH];
	char output_path[MAX_PATH];
	char tools_path[MAX_PATH];

	if (argc < 4)
	{
		printf("Need 3 cmdline args (project file, dest path, tools path) at minimum!\n");
		return -1;
	}

	DWORD ffnlength;

	ffnlength = GetFullPathName(argv[1], MAX_PATH, project_path, NULL);
	if (ffnlength == 0) printf("Error converting '%s' to full path!\n", argv[1]);
	ffnlength = GetFullPathName(argv[2], MAX_PATH, output_path, NULL);
	if (ffnlength == 0) printf("Error converting '%s' to full path!\n", argv[2]);
	ffnlength = GetFullPathName(argv[3], MAX_PATH, tools_path, NULL);
	if (ffnlength == 0) printf("Error converting '%s' to full path!\n", argv[3]);


	printf("Compile project '%s' to packages in path '%s', tools path '%s'.\n", project_path, output_path, tools_path);

	if (argc >= 5)
	{
		add_crash_lib = ((strcmpi(argv[4], "true") == 0) ? true : false);
	}
	printf("Add crash lib '%s'.\n",  (add_crash_lib == true) ? "true" : "false");

	if (argc >= 6)
	{
		enable_log_file_writing = ((strcmpi(argv[5], "true") == 0) ? true : false);
	}
	printf("Enable log file writing '%s'.\n",  (enable_log_file_writing == true) ? "true" : "false");

	if (argc >= 7)
	{
		build_number = atoi(argv[6]);
	}
	printf("Repeatable build number: %d.\n", build_number);

	doc = new CProjectDoc(project_path);
	builder = new CPackageBuilder(doc);
	
	// compile all
	builder->Compile(NULL, output_path, tools_path, add_crash_lib, enable_log_file_writing, build_number);

	return 0;
}
