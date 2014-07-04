
#include "StdAfx.h"

#include "ProjectDoc.h"
#include "PackageBuilder.h"

int main(void)
{
	CProjectDoc *doc = new CProjectDoc("test.wpr");
	CPackageBuilder *builder = new CPackageBuilder(doc);
	
	// compile all
	builder->Compile();

	return 0;
}
