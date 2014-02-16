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
= `base/offer_action.hh`

This file consists of <<action with offer_action_tag>>.
////////////////////////////////////////////////////////////////////////////////
*/

#ifndef HACTAR_OFFER_ACTION_HH
#define HACTAR_OFFER_ACTION_HH

#include "action.hh"
#include "const_queue.hh"

namespace hactar {
/*
////////////////////////////////////////////////////////////////////////////////
== [[action with offer_action_tag]] action with offer_action_tag

An offer action is used for conditional selection of actions. An offer action 
consists of two action choices, one filter action and a cost value, and can 
be constructed by two actions with `operator|`. 

An offer action always choose first one at lowest cost to execute in all 
filtered action choices. function `offer` wraps an action with filter action 
and cost value to construct an offer action.

An offer action consisting of two offer actions can also be constructed by 
two offer action withs same type to reduce templated class code bloat.

Below is an example:

--------------------------------------------------------------------------------
double add(const double& x, double y) { return x + y; }
bool is_small(const double& x) { return x < 100.0; }

wrap(add, 10.0) | wrap(add, 5.0); // => offer action
offer(wrap(add, 10.0), is_small, 5) | wrap(add, 5.0); // => offer action
--------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
*/
template<class TAG1, class TAG2, class TAGF>
struct offer_action_tag { };

template<class OUT, class IN, class TAG1, class TAG2, class TAGF>
class action<OUT, IN, offer_action_tag<TAG1, TAG2, TAGF> >
{
action<OUT, IN, TAG1> _f;
action<OUT, IN, TAG2> _g;
action<bool, IN, TAGF> _filter;
int _cost;

public:
action(const action<OUT, IN, TAG1>& f1, const action<OUT, IN, TAG2>& g1,
	const action<bool, IN, TAGF>& filter1, int cost1)
	: _f(f1)
	, _g(g1)
	, _filter(filter1)
	, _cost(cost1)
{
}

bool
filter(const IN& in1) const
{
	return _filter(in1);
}

int
cost(const IN& in1) const
{
	return _cost;
}

OUT
operator()(const IN& in1) const
{
	return _f(in1);
}

};

template<class OUT, class IN,
	class TAG11, class TAG12, class TAG21, class TAG22, class TAGF>
class action<OUT, IN, offer_action_tag<offer_action_tag<TAG11, TAG12, TAGF>,
		offer_action_tag<TAG21, TAG22, TAGF>, TAGF> >
{
const_queue<action<OUT, IN, offer_action_tag<TAG11, TAG12, TAGF> > > _flist;
const_queue<action<OUT, IN, offer_action_tag<TAG21, TAG22, TAGF> > > _glist;

public:
action(const action<OUT, IN, offer_action_tag<TAG11, TAG12, TAGF> >& f1,
	const action<OUT, IN, offer_action_tag<TAG21, TAG22, TAGF> >& g1)
	: _flist(f1)
	, _glist(g1)
{
}

action(const action<OUT, IN, 
	offer_action_tag<offer_action_tag<TAG11, TAG12, TAGF>, 
	offer_action_tag<TAG21, TAG22, TAGF>, TAGF> >& f1,
	const action<OUT, IN, 
	offer_action_tag<offer_action_tag<TAG11, TAG12, TAGF>,
	offer_action_tag<TAG21, TAG22, TAGF>, TAGF> >& g1)
	: _flist(f1._flist, g1._flist)
	, _glist(f1._glist, g1._glist)
{
}

bool
filter(const IN& in1) const
{
	bool is_valid = false;
	for (unsigned int i = 0; i < _flist.size(); i++) {
		action<OUT, IN, offer_action_tag<TAG11, TAG12, TAGF> > f = _flist[i];
		action<OUT, IN, offer_action_tag<TAG21, TAG22, TAGF> > g = _glist[i];
		is_valid = is_valid || f.filter(in1) || g.filter(in1);
	}

	return is_valid;
}

int
cost(const IN& in1) const
{
	int cost = 0;

	action<OUT, IN, offer_action_tag<TAG11, TAG12, TAGF> > f = _flist[0];
	action<OUT, IN, offer_action_tag<TAG21, TAG22, TAGF> > g = _glist[0];
	if (f.filter(in1) && g.filter(in1)) {
		int fcost = f.cost(in1);
		int gcost = g.cost(in1);
		if (fcost <= gcost) {
			cost = fcost;
		}
		else {
			cost = gcost;
		}
	}
	else if (f.filter(in1)) {
		cost = f.cost(in1);
	}
	else if (g.filter(in1)) {
		cost = g.cost(in1);
	}

	for (unsigned int i = 1; i < _flist.size(); i++) {
		action<OUT, IN, offer_action_tag<TAG11, TAG12, TAGF> > f = _flist[i];
		action<OUT, IN, offer_action_tag<TAG21, TAG22, TAGF> > g = _glist[i];
		if (f.filter(in1) && g.filter(in1)) {
			int fcost = f.cost(in1);
			int gcost = g.cost(in1);
			if (fcost <= gcost) {
				cost = (cost < fcost) ? cost : fcost;
			}
			else {
				cost = (cost < gcost) ? cost : gcost;
			}
		}
		else if (f.filter(in1)) {
			int fcost = f.cost(in1);
			cost = (cost < fcost) ? cost : fcost;
		}
		else if (g.filter(in1)) {
			int gcost = g.cost(in1);
			cost = (cost < gcost) ? cost : gcost;
		}
	}

	return cost;
}

OUT
operator()(const IN& in1) const
{
	int min_cost = cost(in1);

	for (unsigned int i = 0; i < _flist.size(); i++) {
		action<OUT, IN, offer_action_tag<TAG11, TAG12, TAGF> > f = _flist[i];
		action<OUT, IN, offer_action_tag<TAG21, TAG22, TAGF> > g = _glist[i];
		int fcost = f.cost(in1);
		int gcost = g.cost(in1);
		if (f.filter(in1) && (fcost <= min_cost)) {
			return f(in1);
		}
		else if (g.filter(in1) && (gcost <= min_cost)) {
			return g(in1);
		}
	}

	return static_cast<OUT> (in1);
}

};

template<class OUT, class IN, class TAG, class TAGF>
action<OUT, IN, offer_action_tag<TAG, null_action_tag, TAGF> >
offer(const action<OUT, IN, TAG>& f1,
	const action<bool, IN, TAGF>& filter1, int cost1)
{
	return action<OUT, IN, offer_action_tag<TAG, null_action_tag, TAGF> > (
		f1, action<OUT, IN, null_action_tag> (), filter1, cost1);
}

template<class OUT, class IN, class TAG1, class TAG2, class TAGF>
action<OUT, IN, offer_action_tag<TAG1, TAG2, TAGF> >
offer(const action<OUT, IN, TAG1>& f1, const action<OUT, IN, TAG2>& g1,
	const action<bool, IN, TAGF>& filter1, int cost1)
{
	return action<OUT, IN, offer_action_tag<TAG1, TAG2, TAGF> > (f1, g1,
		filter1, cost1);
}

template<class OUT, class IN, class TAG1, class TAG2>
action<OUT, IN, offer_action_tag<TAG1, TAG2, true_action_tag> >
operator|(const action<OUT, IN, TAG1>& f1, const action<OUT, IN, TAG2>& g1)
{
	return action<OUT, IN, offer_action_tag<TAG1, TAG2, true_action_tag> > (
		f1, g1,
		action<bool, IN, true_action_tag> (), 0);
}

template<class OUT, class IN, class TAG1, class TAG2, class TAG3, class TAGF>
action<OUT, IN, offer_action_tag<TAG1, TAG2, TAGF> >
operator|(const action<OUT, IN, offer_action_tag<TAG1, TAG2, TAGF> >& f1,
	const action<OUT, IN, TAG3>& g1)
{
	return f1;
}

template<class OUT, class IN, class TAG0, class TAG1, class TAG2, class TAGF>
action<OUT, IN, offer_action_tag<TAG1, TAG2, TAGF> >
operator|(const action<OUT, IN, TAG0>& f1,
	const action<OUT, IN, offer_action_tag<TAG1, TAG2, TAGF> >& g1)
{
	return g1;
}

template<class OUT, class IN,
	class TAG11, class TAG12, class TAG21, class TAG22, class TAGF>
action<OUT, IN, offer_action_tag<offer_action_tag<TAG11, TAG12, TAGF>,
		offer_action_tag<TAG21, TAG22, TAGF>, TAGF> >
operator|(const action<OUT, IN, offer_action_tag<TAG11, TAG12, TAGF> >& f1,
	const action<OUT, IN, offer_action_tag<TAG21, TAG22, TAGF> >& g1)
{
	return action<OUT, IN,
		offer_action_tag<offer_action_tag<TAG11, TAG12, TAGF>,
			offer_action_tag<TAG21, TAG22, TAGF>, TAGF> > (f1, g1);
}

template<class OUT, class IN, class TAG1, class TAG2, class TAGF>
class action<OUT, IN, offer_action_tag<offer_action_tag<TAG1, TAG2, TAGF>,
		offer_action_tag<TAG1, TAG2, TAGF>, TAGF> >
operator|(const action<OUT, IN,
		offer_action_tag<offer_action_tag<TAG1, TAG2, TAGF>,
			offer_action_tag<TAG1, TAG2, TAGF>, TAGF> >& f1,
	const action<OUT, IN,
		offer_action_tag<offer_action_tag<TAG1, TAG2, TAGF>,
			offer_action_tag<TAG1, TAG2, TAGF>, TAGF> >& g1)
{
	return action<OUT, IN,
		offer_action_tag<offer_action_tag<TAG1, TAG2, TAGF>,
			offer_action_tag<TAG1, TAG2, TAGF>, TAGF> > (f1, g1);
}

}

#endif
////////////////////////////////////////////////////////////////////////////////
