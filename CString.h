#ifndef CSTRING_H_DEFINED
#define CSTRING_H_DEFINED

#include <string>

class CString
{
public:
	CString() { internal_val = ""; }
	CString(char* value) { internal_val = std::string(value); }
	~CString();
	CString(const CString& other) { internal_val = other.value(); }
	CString& operator=(const CString& other) { internal_val = other.value(); return *this; }
	CString& operator=(char* value) { internal_val = std::string(value); return *this; }
	const char* c_str(void) { return internal_val.c_str(); }
	std::string value(void) const { return internal_val; }
private:
	std::string internal_val;
};


#endif // CSTRING_H_DEFINED
