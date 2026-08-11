/*
This file is part of WME Lite.
http://dead-code.org/redir.php?target=wmelite

Copyright (c) 2011 Jan Nedoma

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#ifndef COLL_TEMPL_H
#define COLL_TEMPL_H


#include <vector>

/////////////////////////////////////////////////////////////////////////////
// CBArray<TYPE, ARG_TYPE>
/////////////////////////////////////////////////////////////////////////////
template<class TYPE, class ARG_TYPE>
class CBArray
{
public:
// Construction
	CBArray();

// Attributes
	int GetSize() const;
	void SetSize(int nNewSize, int nGrowBy = -1);

// Operations
	// Clean up
	void RemoveAll();
	
	// Accessing elements
	TYPE GetAt(int nIndex) const;
	void SetAt(int nIndex, ARG_TYPE newElement);
	TYPE& ElementAt(int nIndex);

	// Direct Access to the element data (may return NULL)
	const TYPE* GetData() const;
	TYPE* GetData();

	// Potentially growing the array
	int Add(ARG_TYPE newElement);
	void Copy(const CBArray& src);

	// overloaded operator helpers
	TYPE operator[](int nIndex) const;
	TYPE& operator[](int nIndex);

	// Operations that move elements around
	void InsertAt(int nIndex, ARG_TYPE newElement, int nCount = 1);
	void RemoveAt(int nIndex, int nCount = 1);

// Implementation
private:
	std::vector<TYPE> dataStorage;

public:
	~CBArray();
};

/////////////////////////////////////////////////////////////////////////////
// CBArray<TYPE, ARG_TYPE> inline functions
/////////////////////////////////////////////////////////////////////////////
template<class TYPE, class ARG_TYPE>
inline int CBArray<TYPE, ARG_TYPE>::GetSize() const
	{ return (int) dataStorage.size(); }
template<class TYPE, class ARG_TYPE>
inline void CBArray<TYPE, ARG_TYPE>::RemoveAll() {
	dataStorage.clear();
	dataStorage.shrink_to_fit();
}
template<class TYPE, class ARG_TYPE>
inline TYPE CBArray<TYPE, ARG_TYPE>::GetAt(int nIndex) const
	{ return dataStorage[nIndex]; }
template<class TYPE, class ARG_TYPE>
inline void CBArray<TYPE, ARG_TYPE>::SetAt(int nIndex, ARG_TYPE newElement)
	{ dataStorage[nIndex] = newElement; }
template<class TYPE, class ARG_TYPE>
inline TYPE& CBArray<TYPE, ARG_TYPE>::ElementAt(int nIndex)
	{ return dataStorage[nIndex]; }
template<class TYPE, class ARG_TYPE>
inline const TYPE* CBArray<TYPE, ARG_TYPE>::GetData() const
	{ return dataStorage.data(); }
template<class TYPE, class ARG_TYPE>
inline TYPE* CBArray<TYPE, ARG_TYPE>::GetData()
	{ return dataStorage.data(); }
template<class TYPE, class ARG_TYPE>
inline int CBArray<TYPE, ARG_TYPE>::Add(ARG_TYPE newElement) {
	int nIndex = (int) dataStorage.size();
	dataStorage.push_back(newElement);
	return nIndex;
}
template<class TYPE, class ARG_TYPE>
inline TYPE CBArray<TYPE, ARG_TYPE>::operator[](int nIndex) const
	{ return GetAt(nIndex); }
template<class TYPE, class ARG_TYPE>
inline TYPE& CBArray<TYPE, ARG_TYPE>::operator[](int nIndex)
	{ return ElementAt(nIndex); }

/////////////////////////////////////////////////////////////////////////////
// CBArray<TYPE, ARG_TYPE> out-of-line functions
/////////////////////////////////////////////////////////////////////////////
template<class TYPE, class ARG_TYPE>
CBArray<TYPE, ARG_TYPE>::CBArray()
{
	dataStorage.clear();
	dataStorage.shrink_to_fit();
}

/////////////////////////////////////////////////////////////////////////////
template<class TYPE, class ARG_TYPE>
CBArray<TYPE, ARG_TYPE>::~CBArray()
{
	dataStorage.clear();
	dataStorage.shrink_to_fit();
}

/////////////////////////////////////////////////////////////////////////////
template<class TYPE, class ARG_TYPE>
void CBArray<TYPE, ARG_TYPE>::SetSize(int nNewSize, int nGrowBy)
{
	dataStorage.resize(nNewSize);
	dataStorage.shrink_to_fit();
}

/////////////////////////////////////////////////////////////////////////////
template<class TYPE, class ARG_TYPE>
void CBArray<TYPE, ARG_TYPE>::Copy(const CBArray& src)
{
	SetSize(src.GetSize());
	for (int i = 0; i < src.GetSize(); i++) {
		dataStorage[i] = src.GetAt(i);
	}
}

/////////////////////////////////////////////////////////////////////////////
template<class TYPE, class ARG_TYPE>
void CBArray<TYPE, ARG_TYPE>::InsertAt(int nIndex, ARG_TYPE newElement, int nCount /*=1*/)
{
	if (nIndex >= dataStorage.size())
	{
		SetSize(nIndex, -1);
	}

	dataStorage.reserve(dataStorage.size() + nCount);
	dataStorage.insert(dataStorage.begin() + nIndex, nCount, newElement);
}

/////////////////////////////////////////////////////////////////////////////
template<class TYPE, class ARG_TYPE>
void CBArray<TYPE, ARG_TYPE>::RemoveAt(int nIndex, int nCount)
{
	dataStorage.erase(dataStorage.begin() + nIndex, dataStorage.begin() + nIndex + nCount);
	dataStorage.shrink_to_fit();
}

#endif // COLL_TEMPL_H
