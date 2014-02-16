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
= `base/wrap_action.hh`

This file consists of various class templates in two categories: 
<<function wrap actions>> and <<method wrap actions>>.
////////////////////////////////////////////////////////////////////////////////
*/

#ifndef HACTAR_BIND_ACTION_HH
#define HACTAR_BIND_ACTION_HH

#include "action.hh"

namespace hactar {
/*
////////////////////////////////////////////////////////////////////////////////
== [[function wrap actions]] function wrap actions

Function wrap actions are wrap actions for functions. A function wrap action 
wraps a function with an `IN` argument and some other arguments.

Below is an example:

--------------------------------------------------------------------------------
double add(const double& x, double y) { return x + y; }

wrap(add, 10.0)(5.0); // => 15.0
--------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
*/
struct wrap_action_tag { };

template<class OUT, class IN>
class action<OUT, IN, wrap_action_tag>
{
typedef OUT (* F)();

F _f;

public:
action(F f1)
	: _f(f1)
{
}

OUT
operator()(const IN& in1) const
{
	return _f();
}

};

template<class OUT, class IN>
action<OUT, IN, wrap_action_tag>
wrap(OUT (* f1)())
{
	return action<OUT, IN, wrap_action_tag> (f1);
}

struct wrap0_action_tag { };

template<class OUT, class IN>
class action<OUT, IN, wrap0_action_tag>
{
typedef OUT (* F)(const IN&);

F _f;

public:
action(F f1)
	: _f(f1)
{
}

OUT
operator()(const IN& in1) const
{
	return _f(in1);
}

};

template<class OUT, class IN>
action<OUT, IN, wrap0_action_tag>
wrap(OUT (* f1)(const IN&))
{
	return action<OUT, IN, wrap0_action_tag> (f1);
}

template<class OUT, class IN, class OUTIN>
action<OUT, IN, wrap0_action_tag>
operator*(action<OUTIN, IN, wrap0_action_tag> f1, action<OUT, OUTIN,
		wrap0_action_tag> g1)
{
	return action<OUT, IN, wrap0_action_tag> (f1, g1);
}

template<class A>
struct wrap1_action_tag { };

template<class OUT, class IN, class A>
class action<OUT, IN, wrap1_action_tag<A> >
{
typedef OUT (* F)(const IN&, A);

F _f;
A _a;

public:
action(F f1, A a1)
	: _f(f1)
	, _a(a1)
{
}

OUT
operator()(const IN& in1) const
{
	return _f(in1, _a);
}

};

template<class OUT, class IN, class A>
action<OUT, IN, wrap1_action_tag<A> >
wrap(OUT (* f1)(const IN&, A), A a1)
{
	return action<OUT, IN, wrap1_action_tag<A> > (f1, a1);
}

template<class A, class B>
struct wrap2_action_tag { };

template<class OUT, class IN, class A, class B>
class action<OUT, IN, wrap2_action_tag<A, B> >
{
typedef OUT (* F)(const IN&, A, B);

F _f;
A _a;
B _b;

public:
action(F f1, A a1, B b1)
	: _f(f1)
	, _a(a1)
	, _b(b1)
{
}

OUT
operator()(const IN& in1) const
{
	return _f(in1, _a, _b);
}

};

template<class OUT, class IN, class A, class B>
action<OUT, IN, wrap2_action_tag<A, B> >
wrap(OUT (* f1)(const IN&, A, B), A a1, B b1)
{
	return action<OUT, IN, wrap2_action_tag<A, B> > (f1, a1, b1);
}

/*
////////////////////////////////////////////////////////////////////////////////
== [[method wrap actions]] method wrap actions

Method wrap actions are wrap actions for methods. A method wrap action 
wraps a method with an `X*` pointer, an `IN` argument and some other arguments.

An example:
--------------------------------------------------------------------------------
struct utilities {
	double add(const double& x, double y) { return x + y; }
};

utilities util;

wrap(&util, &utilities::add, 10.0)(5.0); // => 15.0
--------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
*/
template<class X>
struct wrapm_action_tag { };

template<class X, class OUT, class IN>
class action<OUT, IN, wrapm_action_tag<X> >
{
typedef OUT (X::* F)();

X* _px;
F _f;

public:
action(X* px1, F f1)
	: _px(px1)
	, _f(f1)
{
}

OUT
operator()(const IN& in1) const
{
	return (_px->*_f)();
}

};

template<class X, class OUT, class IN>
action<OUT, IN, wrapm_action_tag<X> >
wrap(X* px1, OUT (X::* f1)())
{
	return action<OUT, IN, wrapm_action_tag<X> > (px1, f1);
}

template<class X>
struct wrapm0_action_tag { };

template<class X, class OUT, class IN>
class action<OUT, IN, wrapm0_action_tag<X> >
{
typedef OUT (X::* F)(const IN&);

X* _px;
F _f;

public:
action(X* px1, F f1)
	: _px(px1)
	, _f(f1)
{
}

OUT
operator()(const IN& in1) const
{
	return (_px->*_f)(in1);
}

};

template<class X, class OUT, class IN>
action<OUT, IN, wrapm0_action_tag<X> >
wrap(X* px1, OUT (X::* f1)(const IN&))
{
	return action<OUT, IN, wrapm0_action_tag<X> > (px1, f1);
}

template<class X, class A>
struct wrapm1_action_tag { };

template<class X, class OUT, class IN, class A>
class action<OUT, IN, wrapm1_action_tag<X, A> >
{
typedef OUT (X::* F)(const IN&, A);

X* _px;
F _f;
A _a;

public:
action(X* px1, F f1, A a1)
	: _px(px1)
	, _f(f1)
	, _a(a1)
{
}

OUT
operator()(const IN& in1) const
{
	return (_px->*_f)(in1, _a);
}

};

template<class X, class OUT, class IN, class A>
action<OUT, IN, wrapm1_action_tag<X, A> >
wrap(X* px1, OUT (X::* f1)(const IN&, A), A a1)
{
	return action<OUT, IN, wrapm1_action_tag<X, A> > (px1, f1, a1);
}

template<class X, class A, class B>
struct wrapm2_action_tag { };

template<class X, class OUT, class IN, class A, class B>
class action<OUT, IN, wrapm2_action_tag<X, A, B> >
{
typedef OUT (X::* F)(const IN&, A, B);

X* _px;
F _f;
A _a;
B _b;

public:
action(X* px1, F f1, A a1, B b1)
	: _px(px1)
	, _f(f1)
	, _a(a1)
	, _b(b1)
{
}

OUT
operator()(const IN& in1) const
{
	return (_px->*_f)(in1, _a, _b);
}

};

template<class X, class OUT, class IN, class A, class B>
action<OUT, IN, wrapm2_action_tag<X, A, B> >
wrap(X* px1, OUT (X::* f1)(const IN&, A, B), A a1, B b1)
{
	return action<OUT, IN, wrapm2_action_tag<X, A, B> > (px1, f1, a1, b1);
}

}

#endif
////////////////////////////////////////////////////////////////////////////////
