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
= `base/mutable_ptr.hh`

This file consists of class template <<mutable_ptr>>.
////////////////////////////////////////////////////////////////////////////////
*/

#ifndef HACTAR_MUTABLE_PTR_HH
#define HACTAR_MUTABLE_PTR_HH

#include "const_ptr.hh"

#include <new>

namespace hactar {
/* 
////////////////////////////////////////////////////////////////////////////////
== [[mutable_ptr]] class template `mutable_ptr`

class template `mutable_ptr` is an intrusive smart pointer to build `const_ptr`.

`T` in `mutable_ptr<T>` requires same methods as in `const_ptr<T>`.

`mutable_ptr<T>` would create a new pointer of type `T` in the default
constructor. `mutable_ptr<T>` can also be constructed by a pointer of type
`T*`, or another `mutable_ptr`. It could be used as a non-const pointer.

The method `build` would generate a new `const_ptr` from the `mutable_ptr`.

We use `const_ptr` and `mutable_ptr` together to seperate const functionalities
and non-const functionalities explicitly.

Below is an example:

--------------------------------------------------------------------------------
const_ptr<T> p = mutable_ptr<T>()->build(); // Assumes T is valid
--------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
*/
template<class T>
class mutable_ptr
{
T* _ptr;

public:
mutable_ptr()
	: _ptr(new (std::nothrow) T())
{
	if (_ptr) {
		_ptr->retain();
	}
}

mutable_ptr(T* ptr1)
	: _ptr(ptr1)
{
	if (_ptr) {
		_ptr->retain();
	}
}

mutable_ptr(const const_ptr<T>& const_ptr1)
	: _ptr(const_ptr1._ptr)
{
	if (_ptr) {
		_ptr->retain();
	}
}

~mutable_ptr()
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

const_ptr<T>
build() const
{
	return const_ptr<T> (_ptr);
}

T*
get() const
{
	return _ptr;
}

T*
operator->() const
{
	return _ptr;
}

mutable_ptr<T>&
operator=(const const_ptr<T>& const_ptr1)
{
	mutable_ptr mutable_ptr1(const_ptr1);

	T* ptr = mutable_ptr1._ptr;
	mutable_ptr1._ptr = _ptr;
	_ptr = ptr;

	return *this;
}

bool
operator==(const mutable_ptr<T>& mutable_ptr1) const
{
	return mutable_ptr1.get() == get();
}

private:
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

};

}

#endif
////////////////////////////////////////////////////////////////////////////////
