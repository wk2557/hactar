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
= `base/action.hh`

This file consists of class template <<action>> and 
<<action with true_action_tag>>, <<action with false_action_tag>>.
////////////////////////////////////////////////////////////////////////////////
*/

#ifndef HACTAR_ACTION_HH
#define HACTAR_ACTION_HH

namespace hactar {
/*
////////////////////////////////////////////////////////////////////////////////
== [[action]] class template `action<OUT, IN, TAG>`

Class template `action` is an one-to-one functor `IN -> OUT`.

`OUT`, `IN` in `action<OUT, IN, TAG>` must be a value type that can be copied 
at low cost. `TAG` distinguishes different actions.
////////////////////////////////////////////////////////////////////////////////
*/
struct null_action_tag { };

template<class OUT, class IN, class TAG = null_action_tag>
class action
{
public:
OUT
operator()(const IN& in1)
{
	return static_cast<OUT> (in1);
}

};

/*
////////////////////////////////////////////////////////////////////////////////
== [[action with true_action_tag]] action with true_action_tag

An action with true_action_tag always returns true.
////////////////////////////////////////////////////////////////////////////////
*/
struct true_action_tag { };

template<class IN>
class action<bool, IN, true_action_tag>
{
public:
bool
operator()(const IN& in1) const
{
	return true;
}

};

/*
////////////////////////////////////////////////////////////////////////////////
== [[action with false_action_tag]] action with false_action_tag

An action with true_action_tag always returns false.
////////////////////////////////////////////////////////////////////////////////
*/
struct false_action_tag { };

template<class IN>
class action<bool, IN, false_action_tag>
{
public:
bool
operator()(const IN& in1) const
{
	return false;
}

};

}

#endif
////////////////////////////////////////////////////////////////////////////////
