
#include "StdAfx.h"

#include "PackageBuilder.h"

int main(void)
{
	CProjectDoc *doc = NULL;
	CPackageBuilder *builder = new CPackageBuilder(doc);
	
	// compile all
	builder->Compile();

	return 0;
}
