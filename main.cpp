
#include "StdAfx.h"

#include "ProjectDoc.h"
#include "PackageBuilder.h"

CProjectDoc *doc;
CPackageBuilder *builder;


int main(void)
{
	doc = new CProjectDoc("O:\\K2\\game\\K2.wpr");
	builder = new CPackageBuilder(doc);
	
	// compile all
	builder->Compile();

	return 0;
}
