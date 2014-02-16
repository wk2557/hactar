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
= `base/const_ptr.hh`

This file consists of class template <<const_ptr>>.
////////////////////////////////////////////////////////////////////////////////
*/

#ifndef HACTAR_CONST_PTR_HH
#define HACTAR_CONST_PTR_HH

#include <stdlib.h>

namespace hactar {
/*
////////////////////////////////////////////////////////////////////////////////
== [[const_ptr]] class template `const_ptr`

class template `const_ptr` is an intrusive smart pointer with const modifier.

`T` in `const_ptr<T>` must have methods `bool retain()` and `bool release()` for
reference counting and have a constructor without arguments. `retain` function
adds references and returns true if it succeeds. `release` function decreases
references and returns true if the object has not be actually released.

As its name suggests, `const_ptr` could only be initialized by another
`const_ptr`, or be used as a const pointer. To create a new `const_ptr` or
modify an existing `const_ptr`, you need to use `mutable_ptr` explicitly.
////////////////////////////////////////////////////////////////////////////////
*/
template<class T>
class const_ptr
{
T* _ptr;

public:
const_ptr(const const_ptr<T>& const_ptr1)
	: _ptr(const_ptr1._ptr)
{
	if (_ptr) {
		_ptr->retain();
	}
}

~const_ptr()
{
	if (!_ptr) {
		return;
	}

	if (_ptr->release()) {
		return;
	}

	typedef char type_must_be_complete[sizeof(T) ? 1 : -1];
	(void) sizeof(type_must_be_complete);
	delete (_ptr);
	_ptr = NULL;
}

const T*
get() const
{
	return _ptr;
}

const T*
operator->() const
{
	return _ptr;
}

bool
operator==(const const_ptr<T>& const_ptr1) const
{
	return const_ptr1.get() == get();
}

private:
template<class>
friend class mutable_ptr;

template<bool(T::*) (), bool(T::*) ()>
class type_must_have_reference_count
{
};

static int validate(type_must_have_reference_count<& T::retain, & T::release>*);

static void validate(...);

enum
{
	_ = sizeof(validate(NULL))
};

const_ptr(T* ptr1)
	: _ptr(ptr1)
{
	if (_ptr) {
		_ptr->retain();
	}
}

const_ptr<T>& operator=(const const_ptr<T>&);

};

}

#endif
////////////////////////////////////////////////////////////////////////////////
