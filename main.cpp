
#include "StdAfx.h"

#include "ProjectDoc.h"
#include "PackageBuilder.h"

CProjectDoc *doc;
CPackageBuilder *builder;


int main(int argc, char **argv)
{
	printf("Compile project '%s' to packages in path '%s'.\n",  argv[1], argv[2]);
	doc = new CProjectDoc(argv[1]);
	builder = new CPackageBuilder(doc);
	
	// compile all
	builder->Compile(NULL, argv[2]);

	return 0;
}
