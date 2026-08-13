
#include <string>

#include "PackageBuilder.h"

CPackageBuilder *builder;


int main(int argc, char **argv)
{
	int build_number = 0;

	std::string input_path;
	std::string output_path;
	std::string output_name;
	std::string custom_build_paths;

	if (argc < 5)
	{
		printf("Usage: ./projcompile input_path output_path output_name build_number custom_build_paths");
		printf("Example: LD_LIBRARY_PATH=../workspaces/projectcompiler_linux/external_lib/compiler/ ../workspaces/projectcompiler_linux/packagebuilder data/  outpack data.dcp  1234 data:patch\n");
		printf("Example: LD_LIBRARY_PATH=../workspaces/projectcompiler_linux/external_lib/compiler/ ../workspaces/projectcompiler_linux/packagebuilder patch/ outpack patch.dcp 1234 data:patch\n");
		return -1;
	}

	input_path   = std::string(argv[1]);
	output_path  = std::string(argv[2]);
	output_name  = std::string(argv[3]);
	build_number = atoi(argv[4]);
	custom_build_paths = std::string(argv[5]);
	
	printf("Compile project '%s' to packages in path '%s', package name '%s'.\n", input_path.c_str(), output_path.c_str(), output_name.c_str());

	printf("Repeatable build number: %d.\n", build_number);

	printf("Custom build paths '%s'.\n",  custom_build_paths.c_str());
	
	printf("TBD: m_Primary, m_Priority\n");
	
	builder = new CPackageBuilder();
	
	// compile one single package
	builder->Compile(input_path.c_str(), output_path.c_str(), output_name.c_str(), build_number, custom_build_paths.c_str());

	return 0;
}
