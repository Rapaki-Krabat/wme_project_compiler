
#include <string>

#include "PackageBuilder.h"

CPackageBuilder *builder;


int main(int argc, char **argv)
{
	int build_number = 0;

	std::string input_path;
	std::string output_path;
	std::string output_name;

	if (argc < 5)
	{
		printf("Usage: ./projcompile input_path output_path output_name build_number");
		return -1;
	}

	input_path   = std::string(argv[1]);
	output_path  = std::string(argv[2]);
	output_name  = std::string(argv[3]);
	build_number = atoi(argv[4]);
	
	printf("Compile project '%s' to packages in path '%s', package name '%s'.\n", input_path.c_str(), output_path.c_str(), output_name.c_str());

	printf("Repeatable build number: %d.\n", build_number);

	builder = new CPackageBuilder();
	
	// compile one single package
	builder->Compile(input_path.c_str(), output_path.c_str(), output_name.c_str(), build_number);

	return 0;
}
