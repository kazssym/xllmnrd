/*
 * Interface address lookups (interface)
 * Copyright (C) 2013  Kaz Nishimura
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef IFADDR_H
#define IFADDR_H 1

#include <netinet/in.h>

/**
 * Initializes this module.
 * @param __signo signal number that will be used to interrupt the worker
 * thread; if its value is 0, no signal will be used.
 * @return 0 on success, or non-zero error number on failure.
 */
extern int ifaddr_initialize(int __signo);

extern void ifaddr_finalize(void);

/**
 * Starts the internal worker thread.
 * This function will be called implicitly if any other function that requires
 * the worker thread is called.
 * This function will do nothing and return 0 if already started.
 * @return 0 on success, or non-zero error number on failure.
 */
extern int ifaddr_start(void);

/**
 * Refreshes the interface address table.
 * @return 0 on success, -1 on failure
 */
extern int ifaddr_refresh(void);

extern int ifaddr_lookup(unsigned int __ifindex, struct in6_addr *__addr);

#endif
