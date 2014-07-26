
#include "StdAfx.h"

#include "ProjectDoc.h"
#include "PackageBuilder.h"

CProjectDoc *doc;
CPackageBuilder *builder;


int main(int argc, char **argv)
{
	printf("Compile project '%s' to packages in path '%s', tools path '%s', add crash lib '%s', enable log file writing '%s'.\n",  argv[1], argv[2], argv[3], argv[4], argv[5]);
	doc = new CProjectDoc(argv[1]);
	builder = new CPackageBuilder(doc);
	
	// compile all
	builder->Compile(NULL, argv[2], argv[3], ((strcmpi(argv[4], "true") == 0) ? true : false), ((strcmpi(argv[5], "true") == 0) ? true : false));

	return 0;
}
