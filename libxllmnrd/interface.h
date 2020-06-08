/*
 * interface.h
 * Copyright (C) 2013-2020 Kaz Nishimura
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

#ifndef INTERFACE_H
#define INTERFACE_H 1

#include "posix.h"
#include <netinet/in.h>
#include <mutex>
#include <unordered_map>
#include <set>
#include <atomic>
#include <cstddef>

/*
 * Specializations of 'std::less'.
 */

template <>
struct std::less<struct in_addr>
{
    bool operator ()(const struct in_addr &x, const struct in_addr &y) const;
};

template <>
struct std::less<struct in6_addr>
{
    bool operator ()(const struct in6_addr &x, const struct in6_addr &y) const;
};

namespace xllmnrd
{
    using std::size_t;

    /// Interface change event class.
    struct interface_change_event
    {
        enum event_type: int
        {
            REMOVED,
            ADDED,
        };

        event_type type;
        unsigned int interface_index;
        int address_family;

        constexpr interface_change_event(event_type type,
            unsigned int interface_index, int address_family = AF_UNSPEC)
        :
            type {type},
            interface_index {interface_index},
            address_family {address_family}
        {}
    };

    // Pointer to an interface change handler.
    typedef void (*interface_change_handler)(const interface_change_event *);

    /// Abstract interface manager class.
    class interface_manager
    {
    protected:
        struct interface
        {
            std::set<struct in_addr> in_addresses;
            std::set<struct in6_addr> in6_addresses;

            /// Returns true if no address is stored, false otherwise.
            bool empty() const
            {
                return in_addresses.empty() && in6_addresses.empty();
            }
        };

    private:
        /// Interface change handler.
        std::atomic<interface_change_handler> _interface_change {nullptr};

        mutable std::recursive_mutex _interfaces_mutex;

        /// Map from interface indices to interfaces.
        std::unordered_map<unsigned int, interface> _interfaces;

    protected:
        /// Constructs an interface manager.
        interface_manager();

        // The copy constructor is deleted.
        interface_manager(const interface_manager &) = delete;

        // The copy assignment operator is deleted.
        void operator =(const interface_manager &) = delete;

    public:
        /// Destructs an interface manager.
        virtual ~interface_manager();

    public:
        /// Set the interface change handler.
        ///
        /// This function is thread-safe.
        interface_change_handler set_interface_change(
            interface_change_handler interface_change);

        /**
         * Returns a copy of the IPv4 addresses of an interface.
         *
         * This function is thread-safe.
         *
         * @param {unsigned int} index an interface index
         * @return a copy of the IPv4 addresses of the interface
         */
        std::set<struct in_addr> in_addresses(unsigned int index) const;

        /**
         * Returns a copy of the IPv6 addresses of an interface.
         *
         * This function is thread-safe.
         *
         * @param {unsigned int} index an interface index
         * @return a copy of the IPv6 addresses of the interface
         */
        std::set<struct in6_addr> in6_addresses(unsigned int index) const;

        // Refreshes the interface addresses.
        //
        // This function is thread safe.
        virtual void refresh() = 0;

    protected:
        /// Returns the reference to the mutex object.
        std::recursive_mutex &mutex() const
        {
            return _interfaces_mutex;
        }

        /// Removes all the interfaces.
        void remove_interfaces();

        void add_interface_address(unsigned int index, int family,
            const void *address, size_t address_size);

        void remove_interface_address(unsigned int index, int family,
            const void *address, size_t address_size);

    private:
        // Fires an interace change event.
        void fire_interface_change(const interface_change_event *event);
    };
}

#endif
