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
= `base/loop_action.hh`

This file consists of class template <<loop>> and class template 
<<action with loop_action_tag>>.
////////////////////////////////////////////////////////////////////////////////
*/

#ifndef HACTAR_LOOP_ACTION_HH
#define HACTAR_LOOP_ACTION_HH

#include "action.hh"
#include "const_queue.hh"

namespace hactar {
/*
////////////////////////////////////////////////////////////////////////////////
== [[loop]] class template `loop`

Class template `loop` consists of loop count and loop filter action. Loop count
is the maximum times a loop could take. Loop filter is an action that returns 
true if the loop could continue.
////////////////////////////////////////////////////////////////////////////////
*/
template<class IN, class TAGF>
class loop
{
unsigned int _count;
action<bool, IN, TAGF> _filter;

public:
loop(const int& count1, const action<bool, IN, TAGF>& filter1)
	: _count(count1)
	, _filter(filter1)
{
}

int
count() const
{
	return _count;
}

action<bool, IN, TAGF>
filter() const
{
	return _filter;
}

};

/*
////////////////////////////////////////////////////////////////////////////////
== [[action with loop_action_tag]] action with loop_action_tag

A loop action is used for looping through actions. A loop action consists of 
loop count and loop filter action, and could be constructed by an action 
`IN -> IN` and an `loop` object with `operator*`. An action `IN -> IN` and a 
loop count value could construct a loop action with `operator*` too.

In each loop iteration, the internal action `IN -> IN` would take the output of 
last iteration as input.

Below is an example:

--------------------------------------------------------------------------------
double add(const double& x, double y) { return x + y; }
bool is_small(const double& x) { return x < 100.0; }

wrap(add, 10.0) * loop(10, wrap(is_small)); // => loop action
wrap(add, 10.0) * 10; // => loop action
--------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
*/
template<class TAG, class TAGF>
struct loop_action_tag { };

template<class IN, class TAG, class TAGF>
class action<IN, IN, loop_action_tag<TAG, TAGF> >
{
action<IN, IN, TAG> _f;
unsigned int _count;
action<bool, IN, TAGF> _filter;

public:
action(const action<IN, IN, TAG>& f1,
	const int& count1, const action<bool, IN, TAGF>& filter1)
	: _f(f1)
	, _count(count1)
	, _filter(filter1)
{
}

IN
operator()(const IN& in1) const
{
	IN in = in1;
	for (unsigned int i = 0; _filter(in) && i < _count; i++) {
		in = _f(in);
	}

	return in;
}

};

template<class IN, class TAG, class TAGF>
action<IN, IN, loop_action_tag<TAG, TAGF> >
operator*(const action<IN, IN, TAG>& f1, const loop<IN, TAGF>& loop1)
{
	return action<IN, IN, loop_action_tag<TAG, TAGF> > (f1,
		loop1.count(), loop1.filter());
}

template<class IN, class TAG, class TAGF>
action<IN, IN, loop_action_tag<TAG, TAGF> >
operator*(const loop<IN, TAGF>& loop1, const action<IN, IN, TAG>& f1)
{
	return action<IN, IN, loop_action_tag<TAG, TAGF> > (f1,
		loop1.count(), loop1.filter());
}

template<class IN, class TAG>
action<IN, IN, loop_action_tag<TAG, true_action_tag> >
operator*(const action<IN, IN, TAG>& f1, const unsigned int& count1)
{
	return action<IN, IN, loop_action_tag<TAG, true_action_tag> > (f1,
		count1, action<bool, IN, true_action_tag> ());
}

template<class IN, class TAG>
action<IN, IN, loop_action_tag<TAG, true_action_tag> >
operator*(const unsigned int& count1, const action<IN, IN, TAG>& f1)
{
	return action<IN, IN, loop_action_tag<TAG, true_action_tag> > (f1,
		count1, action<bool, IN, true_action_tag> ());
}

}

#endif
////////////////////////////////////////////////////////////////////////////////
