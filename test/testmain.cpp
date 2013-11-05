/*
 * Unit test driver for xllmnrd (main)
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

#if HAVE_CONFIG_H
#include <config.h>
#endif
#define _GNU_SOURCE 1

#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cstdlib>

using namespace std;
using CppUnit::TextUi::TestRunner;
using CppUnit::TestFactoryRegistry;

int main(int argc, char *argv[]) {
    TestRunner runner;
    runner.addTest(TestFactoryRegistry::getRegistry().makeTest());

    if (!runner.run()) {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
