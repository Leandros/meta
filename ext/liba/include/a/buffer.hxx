/* ========================================================================= */
/* Copyright (C) 2017-2017 Arvid Gerstmann                                   */
/*                                                                           */
/* This file is part of liba.                                                */
/*                                                                           */
/* liba is free software: you can redistribute it and/or modify              */
/* it under the terms of the GNU General Public License as published by      */
/* the Free Software Foundation, either version 3 of the License, or         */
/* (at your option) any later version.                                       */
/*                                                                           */
/* liba is distributed in the hope that it will be useful,                   */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of            */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             */
/* GNU General Public License for more details.                              */
/*                                                                           */
/* You should have received a copy of the GNU General Public License         */
/* along with liba.  If not, see <http://www.gnu.org/licenses/>.             */
/* ========================================================================= */

#ifndef LIBA_BUFFER_HXX
#define LIBA_BUFFER_HXX
#pragma once

#include <string.h>
#include <assert.h>
#include <a/allocator.hxx>
#include <a/error.hxx>
#include <a/utilities.hxx>
#include <algorithm>

namespace a {

/* ========================================================================= */
/* Buffer                                                                    */
/* ========================================================================= */
template<class T, class Allocator = a::allocator>
class buffer
{
    using Buffer = buffer<T, Allocator>;

public:
    inline buffer(Allocator alloc = Allocator()) : m_alloc(alloc) {}
    inline buffer(T const *ptr, size_t num, Allocator alloc = Allocator())
        : m_ptr(ptr)
        , m_num(num)
        , m_alloc(alloc)
    {
    }

    inline buffer(Buffer const &other) { _copy(other); }
    inline buffer(Buffer &&other) { _move(static_cast<Buffer &&>(other)); }
    inline Buffer &operator=(Buffer const &other) { return _copy(other); }
    inline Buffer &operator=(Buffer &&other) { return _move(static_cast<Buffer &&>(other)); }
    inline explicit operator bool() const { return m_ptr != nullptr; }

    inline ~buffer()
    {
        m_alloc.deallocate(m_ptr, sizeof(T) * m_num);
    }


    inline a::err_code
    allocate(size_t num)
    {
        assert(m_ptr == nullptr && "use re-allocate for reallocation");
        m_ptr = (T *)m_alloc.allocate(sizeof(T) * num);
        if (m_ptr == nullptr)
            return false;
        m_num = num;
        return true;
    }

    /* Accessor */
    inline T const * get() const { return m_ptr; }
    inline T * get() { return m_ptr; }
    inline char *bytes() { return (char *)m_ptr; }
    inline char const *bytes() const { return (char const *)m_ptr; }
    inline size_t num() { return m_num; }
    inline size_t size() const { return m_num * sizeof(T); }

    /* Allocator */
    inline Allocator & get_allocator() { return m_alloc; }
    inline Allocator const & get_allocator() const { return m_alloc; }


private:
    inline Buffer &
    _copy(Buffer const &other)
    {
        assert(m_ptr == nullptr && "cannot copy into allocated buffer");
        if (other.m_ptr != nullptr) {
            m_ptr = (T *)m_alloc.allocate(other.m_num);
            if (m_ptr != nullptr) {
                m_num = other.m_num;
                memcpy(m_ptr, other.m_ptr, m_num * sizeof(T));
            }
        }

        return *this;
    }

    inline Buffer &
    _move(Buffer &&other)
    {
        assert(m_ptr == nullptr && "cannot move into allocated buffer");
        m_ptr = other.m_ptr;
        m_num = other.m_num;
        other.m_ptr = nullptr;
        other.m_num = 0;
        return *this;
    }

private:
    Allocator m_alloc = Allocator();
    T *m_ptr = nullptr;
    size_t m_num = 0;
};


/* ========================================================================= */
/* Buffer Builder                                                            */
/* ========================================================================= */
template<class T, size_t MIN_SIZE = 16384, class Allocator = a::allocator>
class buffer_builder
{
    using Buffer = buffer<T, Allocator>;
    using Builder = buffer_builder<T, MIN_SIZE, Allocator>;

    buffer_builder(Builder const &) = delete;
    Builder &operator=(Builder const &) = delete;

    struct node {
        node *next;
        uint32_t full;
        uint32_t left;
        char buf[1];
    };

public:
    inline buffer_builder() {}
    inline ~buffer_builder()
    {
        dealloc(m_first);
    }

    inline buffer_builder(Builder &&other) { _move(static_cast<Builder &&>(other)); }
    inline Builder &operator=(Builder &&other) { return _move(static_cast<Builder &&>(other)); }

    inline size_t
    append(char const *buf, size_t nbytes)
    {
        assert(nbytes < (0x1 << 31) && "nbytes may not be larger than 4 GiB");
        dlog("-- 0\n");

        /* Alloc first node. */
        if (m_first == nullptr) {
            size_t const size = nbytes < MIN_SIZE ? MIN_SIZE : nbytes;
            void *mem = m_alloc.allocate(sizeof(node) + size);
            if (mem == nullptr)
                return 0;
            m_first = m_last = new(mem) node;
            m_first->next = nullptr;
            m_first->full = uint32_t(size);
            m_first->left = uint32_t(size - nbytes);
            dlog("size = %lld | nbytes = %lld\n", size, nbytes);
            memcpy(m_first->buf, buf, nbytes);
        } else {

            /* Enough space inside the last node. */
            if (m_last->left >= nbytes) {
                size_t const offset = m_last->full - m_last->left;
                m_last->left -= uint32_t(nbytes);
                memcpy(m_last->buf + offset, buf, nbytes);
            } else {

                dlog("-- 1\n");
                size_t left = nbytes;
                dlog("left = %lld\n", left);
                dlog("-- 2\n");
                for (node *cur = m_last; left > 0 && cur != nullptr; cur = cur->next) {
                    size_t const offset = cur->full - cur->left;
                    size_t const size = nbytes - cur->left;
                    dlog("offset = %lld\n", offset);
                    dlog("size = %lld\n", size);
                    if (size > 0) {
                        size_t const len = std::min(size, left);
                        dlog("len = %lld\n", len);
                        memcpy(cur->buf + offset, buf + (nbytes - left), len);
                        left -= len;
                        cur->left -= uint32_t(len);
                    }
                    m_last = cur;
                }

                dlog("-- 3\n");
                dlog("left = %lld\n", left);
                /* If we still have bytes over, alloc a new node. */
                if (left > 0) {
                    size_t const newsize = left < MIN_SIZE ? MIN_SIZE : left;
                    void *mem = m_alloc.allocate(sizeof(node) + newsize);
                    if (mem == nullptr)
                        return nbytes - left;

                    /* Fill new node. */
                    m_last->next = new(mem) node;
                    m_last = m_last->next;
                    m_last->next = nullptr;
                    m_last->full = uint32_t(newsize);
                    m_last->left = uint32_t(newsize - left);
                    memcpy(m_last->buf, buf + (nbytes - left), left);
                }
            }
        }

        dlog("-- OUT\n");
        m_len += nbytes;
        return nbytes;
    }

    inline Buffer
    to_buffer() const
    {
        Buffer buf;
        if (!buf.allocate(m_len))
            return {};
        size_t off = 0;
        for (node *cur = m_first; cur != nullptr; cur = cur->next) {
            size_t const size = cur->full - cur->left;
            memcpy(buf.bytes() + off, cur->buf, size);
            off += size;
        }

        return buf;
    }

    inline size_t
    length() const
    {
        return m_len;
    }

    inline void
    reset()
    {
        m_last = m_first;
        m_len = 0;
        for (node *cur = m_first; cur != nullptr; cur = cur->next)
            cur->left = cur->full;
    }

private:
    inline Builder &
    _move(Builder &&other)
    {
        this->m_alloc = other.m_alloc;
        this->m_len = other.m_len;
        this->m_first = other.m_first;
        this->m_last = other.m_last;
        other.m_alloc = Allocator();
        other.m_len = 0;
        other.m_first = nullptr;
        other.m_last = nullptr;
        return *this;
    }

    inline void
    dealloc(node *cur)
    {
        /* Either iterate backwards: */
#if 0
        if (cur == nullptr)
            return;
        dealloc(cur->next);
        m_alloc.deallocate(cur, ((cur->len >> 32) & 0xFFFFFFFF + sizeof(node));
#else

        /* or reverse it first: */
        node *last = nullptr;
        for (node *next; cur != nullptr; cur = next) {
            next = cur->next;
            cur->next = last;
            last = cur;
        }

        for (node *next; last != nullptr; last = next) {
            next = last->next;
            last->next = cur;
            cur = last;
            m_alloc.deallocate(last, last->full + sizeof(node));
        }
#endif
    }


private:
    Allocator m_alloc = Allocator();
    size_t m_len = 0;
    node *m_first = nullptr,
         *m_last = nullptr;
};

} /* namespace a */

#endif /* LIBA_BUFFER_HXX */

