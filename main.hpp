/*
 * main.hpp
 *
 *  Created on: Jan 20, 2019
 *      Author: jos
 */

#ifndef MAIN_HPP_
#define MAIN_HPP_

#include <iostream>

#ifdef DEBUG
#define TRACE_IN cout << *this << " entry " << __func__ << endl;
#define TRACE_OUT cout << *this << " exit " << __func__ << endl;
#else
#define TRACE_IN
#define TRACE_OUT
#endif

#define RND(m) (rand() % m);

#endif /* MAIN_HPP_ */
