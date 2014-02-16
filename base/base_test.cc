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
= `base/base_test.cc`
////////////////////////////////////////////////////////////////////////////////
*/

#include "base_test.h"

#include "hactar.hh"
#include <iostream>

namespace hactar {

class calc
{
volatile size_t _rc;
double _value;
double _mvalue;

public:
calc()
	: _rc(0)
	, _value(0)
	, _mvalue(0)
{
}

~calc()
{
}

bool
retain()
{
	return ++_rc;
}

bool
release()
{
	return --_rc;
}

double
value() const
{
	return _value;
}

void
set_value(const double& in1)
{
	_value = in1;
}

double
mvalue() const
{
	return _mvalue;
}

void
set_mvalue(const double& min1)
{
	_mvalue = min1;
}

private:
calc(const calc&);

calc& operator=(const calc&);

};

template<>
const_ptr<calc>
unit(const double& in1)
{
	mutable_ptr<calc> mutable_ptr1;
	mutable_ptr1->set_value(in1);

	return mutable_ptr1.build();
}

template<class TAG>
const_ptr<calc>
operator&(const const_ptr<calc>& const_ptr1, 
	const action<double, double, TAG>& f1)
{
	if (const_ptr1.get() == NULL) {
		return const_ptr1;
	}

	const_ptr<calc> const_ptr2(unit<calc> (f1(const_ptr1->value())));

	if (const_ptr2.get() == NULL) {
		return const_ptr2;
	}

	mutable_ptr<calc> (const_ptr2)->set_mvalue(const_ptr1->mvalue());
	std::cout << const_ptr2->value() << "\t" << const_ptr2->mvalue() <<
		std::endl;

	return const_ptr2;
}

template<class TAG>
struct mplus_tag { };

template<class TAG>
class action<double, double, mplus_tag<TAG> >
{
action<double, double, TAG> _f;

public:
action(action<double, double, TAG> f1)
	: _f(f1)
{
}

double
operator()(const double& in1) const
{
	return _f(in1);
}

};

template<class TAG>
action<double, double, mplus_tag<TAG> >
mplus(action<double, double, TAG> f1)
{
	return action<double, double, mplus_tag<TAG> > (f1);
}

template<class TAG>
const_ptr<calc>
operator&(const const_ptr<calc>& const_ptr1,
	const action<double, double, mplus_tag<TAG> >& f1)
{
	if (const_ptr1.get() == NULL) {
		return const_ptr1;
	}

	const_ptr<calc> const_ptr2(unit<calc> (f1(const_ptr1->value())));

	if (const_ptr2.get() == NULL) {
		return const_ptr2;
	}

	mutable_ptr<calc> (const_ptr2)->set_mvalue(const_ptr1->mvalue() +
		const_ptr2->value());
	std::cout << const_ptr2->value() << "\t" << const_ptr2->mvalue() <<
		std::endl;

	return const_ptr2;
}

struct mclean_tag { };

template<>
class action<double, double, mclean_tag>
{
public:
action()
{
}

double
operator()(const double& in1) const
{
	return in1;
}

};

action<double, double, mclean_tag>
mclean()
{
	return action<double, double, mclean_tag> ();
}

template<>
const_ptr<calc>
operator&(const const_ptr<calc>& const_ptr1,
	const action<double, double, mclean_tag>& f1)
{
	if (const_ptr1.get() == NULL) {
		return const_ptr1;
	}
	
	const_ptr<calc> const_ptr2(unit<calc> (f1(const_ptr1->value())));
	
	if (const_ptr2.get() == NULL) {
		return const_ptr2;
	}
	
	std::cout << const_ptr2->value() << "\t" << const_ptr2->mvalue() <<
		std::endl;

	return const_ptr2;
}

double
add(const double& x, double y)
{
	return x + y;
}

double
multiply(const double& x, double y)
{
	return x * y;
}

}

int
hactar::base_test(int argc, const char* argv[])
{
	unit<calc> (3.14) &
	(wrap(multiply, 2.0) | wrap(add, 2.0)) & (wrap(add, 10.0) * 4) &
	(wrap(add, 2.5) & wrap(multiply, 1.1) & wrap(add, 2.2)) &
	mplus(wrap(multiply, 2.4)) & mclean();
	
	return 0;
}

////////////////////////////////////////////////////////////////////////////////
