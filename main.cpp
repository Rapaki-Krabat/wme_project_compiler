
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

	if (argc < 4)
	{
		printf("Need 3 cmdline args (project file, dest path, tools path) at minimum!\n");
		return -1;
	}

	printf("Compile project '%s' to packages in path '%s', tools path '%s'.\n",  argv[1], argv[2], argv[3]);

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

	doc = new CProjectDoc(argv[1]);
	builder = new CPackageBuilder(doc);
	
	// compile all
	builder->Compile(NULL, argv[2], argv[3], add_crash_lib, enable_log_file_writing, build_number);

	return 0;
}
