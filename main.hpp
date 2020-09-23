/*
 * main.hpp
 *
 *  Created on: Jan 20, 2019
 *      Author: jos
 */

#ifndef MAIN_HPP_
#define MAIN_HPP_

#include <iostream>

#ifdef DEBUGGING
#define TRACE_IN cout << *this << " entry " << __func__ << endl;
#define TRACE_OUT cout << *this << " exit " << __func__ << endl;
#define LDEBUG(x) cout << "DEBUG: " << x << endl;
#else
#define TRACE_IN
#define TRACE_OUT
#define LDEBUG(x) do {} while (false);
#endif

#define RND(m) (rand() % m)

#endif /* MAIN_HPP_ */
