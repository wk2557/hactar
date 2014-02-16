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
= `base/complex_action.hh`

This file consists of class template <<action with complex_action_tag>>.
////////////////////////////////////////////////////////////////////////////////
*/

#ifndef HACTAR_COMPLEX_ACTION_HH
#define HACTAR_COMPLEX_ACTION_HH

#include "action.hh"
#include "const_queue.hh"

namespace hactar {
/*
////////////////////////////////////////////////////////////////////////////////
== [[action with complex_action_tag]] action with complex_action_tag

Complex actions are used for composition of actions. A complex action can be 
constructed by two composable actions with `operator&`. A complex action can 
also be constructed by another complex action and an extra action `OUT -> OUT` 
to reduce templated class code bloat.

Below is an example:

--------------------------------------------------------------------------------
double add(const double& x, double y) { return x + y; }

wrap(add, 10.0) & wrap(add, 5.0); // => composite action
--------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
*/
template<class OUTIN, class TAG1, class TAG2>
struct complex_action_tag { };

template<class OUT, class IN, class OUTIN, class TAG1, class TAG2>
class action<OUT, IN, complex_action_tag<OUTIN, TAG1, TAG2> >
{
action<OUTIN, IN, TAG1> _f;
action<OUT, OUTIN, TAG2> _g;

const_queue<action<OUT, OUT, TAG2> > _hlist;

public:
action(const action<OUTIN, IN, TAG1>& f1, const action<OUT, OUTIN, TAG2>& g1)
	: _f(f1)
	, _g(g1)
{
}

action(const action<OUT, IN, complex_action_tag<OUTIN, TAG1, TAG2> >& action1,
	const action<OUT, OUT, TAG2>& h1)
	: _f(action1._f)
	, _g(action1._g)
	, _hlist(action1._hlist, h1)
{
}

OUT
operator()(const IN& in1) const
{
	OUT out = _g(_f(in1));

	for (int i = 0; i < _hlist.size(); i++) {
		action<OUT, OUT, TAG2> hi = _hlist[i];
		out = hi(out);
	}

	return out;
}

};

template<class OUT, class IN, class OUTIN, class TAG1, class TAG2>
action<OUT, IN, complex_action_tag<OUTIN, TAG1, TAG2> >
operator&(const action<OUTIN, IN, TAG1>& f1, 
	const action<OUT, OUTIN, TAG2>& g1)
{
	return action<OUT, IN, complex_action_tag<OUTIN, TAG1, TAG2> > (f1,
		g1);
}

template<class OUT, class IN, class OUTIN, class TAG1, class TAG2>
action<OUT, IN, complex_action_tag<OUTIN, TAG1, TAG2> >
operator&(
	const action<OUT, IN, complex_action_tag<OUTIN, TAG1, TAG2> >& action1,
	const action<OUT, OUT, TAG2>& h1)
{
	return action<OUT, IN, complex_action_tag<OUTIN, TAG1, TAG2> > (
		action1, h1);
}

}

#endif
////////////////////////////////////////////////////////////////////////////////
