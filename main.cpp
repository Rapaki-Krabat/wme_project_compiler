
#include "StdAfx.h"

#include "ProjectDoc.h"
#include "PackageBuilder.h"

CProjectDoc *doc;
CPackageBuilder *builder;


int main(int argc, char **argv)
{
	doc = new CProjectDoc(argv[1]);
	builder = new CPackageBuilder(doc);
	
	// compile all
	builder->Compile();

	return 0;
}
