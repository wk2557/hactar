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
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

////////////////////////////////////////////////////////////////////////////////
= `base/hactar.hh`

This file consists of function <<unit>> and operator& (<<bind>>).
////////////////////////////////////////////////////////////////////////////////
*/

#ifndef HACTAR_HH
#define HACTAR_HH

#include "const_ptr.hh"
#include "mutable_ptr.hh"
#include "action.hh"
#include "wrap_action.hh"
#include "complex_action.hh"
#include "offer_action.hh"
#include "loop_action.hh"

namespace hactar {
/*
////////////////////////////////////////////////////////////////////////////////
== [[unit]] function `unit`

Function `unit` is a function to be overloaded for converting value of type `X` 
into a `const_ptr` of type `T`.
////////////////////////////////////////////////////////////////////////////////
*/
template<class T, class X>
const_ptr<T>
unit(const X& x)
{
	return mutable_ptr<T> ()->build();
}

/*
////////////////////////////////////////////////////////////////////////////////
== [[bind]] `operator&`

Operator>> is a function template to be overloaded for adding side-effects to 
an action to a `const_ptr`. Actions are kept pure since all side-effects are 
in overloading and specilizing this function template.
////////////////////////////////////////////////////////////////////////////////
*/
template<class T, class TAG, class OUT, class IN>
const_ptr<T>
operator&(const const_ptr<T>& const_ptr1, const action<TAG, OUT, IN>& f1)
{
	return const_ptr1;
}

}

#endif
////////////////////////////////////////////////////////////////////////////////
