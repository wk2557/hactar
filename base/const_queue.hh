////////////////////////////////////////////////////////////////////////////////
/*

Copyright (c) 2014 Sam Yuen

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or altertantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

////////////////////////////////////////////////////////////////////////////////
= `base/const_queue.hh`

This file consists of class template <<const_queue>>.
////////////////////////////////////////////////////////////////////////////////
*/

#ifndef HACTAR_CONST_QUEUE_HH
#define HACTAR_CONST_QUEUE_HH

#include <memory>

namespace hactar {
/*
////////////////////////////////////////////////////////////////////////////////
== [[const_queue]] class template `const_queue`

class template `const_queue` is a queue with const modifier.

`X` in `const_queue<X>` must be a value type without RAII tricks.

As its name suggests, `const_queue` could only be initialized by merging other 
`const_queue`s, or assigning other `const_ptr` with an extra value, or just a 
value, and no modifications to it is allowed.

For simple readonly copy cases, namely 'shadow copy's, it would not release the 
memory. Otherwise, `const_queue` would release memory automatically.
////////////////////////////////////////////////////////////////////////////////
*/
template<class X>
class const_queue
{
unsigned int _size;
X* _ptr;
bool _is_shadow;

public:
const_queue()
	: _size(0)
	, _ptr(NULL)
	, _is_shadow(false)
{
}

const_queue(const X& x1)
	: _size(1)
	, _ptr((X*) malloc(sizeof(X)))
	, _is_shadow(false)
{
	*_ptr = x1;
}

const_queue(const const_queue<X>& array1, const X& x1)
	: _size(array1.size() + 1)
	, _ptr((X*) malloc(_size * sizeof(X)))
	, _is_shadow(false)
{
	for (unsigned int i = 0; i < array1.size(); i++) {
		_ptr[i] = array1[i];
	}

	_ptr[array1.size()] = x1;
}

const_queue(const const_queue<X>& array1, const const_queue<X>& array2)
	: _size(array1.size() + array2.size())
	, _ptr((X*) malloc(_size * sizeof(X)))
	, _is_shadow(false)
{
	for (unsigned int i = 0; i < array1.size(); i++) {
		_ptr[i] = array1[i];
	}

	for (unsigned int i = 0; i < array2.size(); i++) {
		_ptr[array1.size() + i] = array2[i];
	}
}

const_queue(const const_queue<X>& array1)
	: _size(array1._size)
	, _ptr(array1._ptr)
	, _is_shadow(true)
{
}

~const_queue()
{
	if (_ptr && !_is_shadow) {
		free(_ptr);
		_ptr = NULL;
	}
}

unsigned int
size() const
{
	return _size;
}

const X&
operator[](const unsigned int i) const
{
	return _ptr[i];
}

private:
const_queue<X>& operator=(const const_queue<X>&);

bool operator==(const const_queue<X>&);

};

}

#endif
////////////////////////////////////////////////////////////////////////////////
