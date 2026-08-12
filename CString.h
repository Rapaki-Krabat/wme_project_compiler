#ifndef CSTRING_H_DEFINED
#define CSTRING_H_DEFINED

#include <string>

class CString
{
public:
	CString() { internal_val = ""; }
	CString(char* value) { internal_val = std::string(value); }
	CString(std::string value) { internal_val = value; }
	~CString() {};
	CString(const CString& other) { internal_val = other.value(); }
	CString& operator=(const CString& other) { internal_val = other.value(); return *this; }
	CString& operator=(char* value) { internal_val = std::string(value); return *this; }
	CString& operator+(const CString& other) { internal_val += other.value(); return *this; }
	CString& operator+=(const CString& other) { internal_val += other.value(); return *this; }
	bool operator<(const CString& other) const { return internal_val < other.value(); }
	bool operator!=(const CString& other) { return internal_val != other.value(); }
	bool operator!=(char* value) { return internal_val != std::string(value); }
	char operator[](size_t index) { return internal_val[index]; }
	const char* c_str(void) { return internal_val.c_str(); }
	std::string value(void) const { return internal_val; }
	size_t GetLength(void) { return internal_val.length(); }
	CString Right(int nCount) { return CString(const_cast<char*>(internal_val.substr(internal_val.length() - nCount).c_str())); }
private:
	std::string internal_val;
};


#endif // CSTRING_H_DEFINED
