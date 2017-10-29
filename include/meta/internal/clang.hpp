/* ========================================================================= */
/* Copyright (C) 2017-2017 Arvid Gerstmann                                   */
/*                                                                           */
/* This file is part of meta.                                                */
/*                                                                           */
/* meta is free software: you can redistribute it and/or modify              */
/* it under the terms of the GNU General Public License as published by      */
/* the Free Software Foundation, either version 3 of the License, or         */
/* (at your option) any later version.                                       */
/*                                                                           */
/* meta is distributed in the hope that it will be useful,                   */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of            */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             */
/* GNU General Public License for more details.                              */
/*                                                                           */
/* You should have received a copy of the GNU General Public License         */
/* along with meta.  If not, see <http://www.gnu.org/licenses/>.             */
/* ========================================================================= */
#ifndef CLANG_HPP
#define CLANG_HPP
#pragma once

namespace clang
{

/* ========================================================================= */
/* Command-line argument wrapper                                             */
/* ========================================================================= */
class Args
{
    friend class TLU;

public:
    inline Args() {}

    inline
    Args(int nargs)
    {
        m_args.reserve(nargs);
    }

    inline void
    add_arg(char const *s)
    {
        assert(s != nullptr && "s may not be null");
        m_args.push_back(s);
    }


private:
    std::vector<char const *> m_args;
};


/* ========================================================================= */
/* CXString Wrapper                                                          */
/* ========================================================================= */
class CXStringRAII
{
    inline CXStringRAII(CXStringRAII const &) = delete;
    inline CXStringRAII &operator=(CXStringRAII const &) = delete;
    inline CXStringRAII(CXStringRAII &&) = delete;
    inline CXStringRAII &operator=(CXStringRAII &&) = delete;

public:
    inline CXStringRAII(CXString const &s)
        : m_string(s)
    {}

    inline ~
    CXStringRAII()
    {
        clang_disposeString(m_string);
    }

    inline char const *
    c_str()
    {
        return clang_getCString(m_string);
    }

private:
    CXString m_string;
};


/* ========================================================================= */
/* Cursor                                                                    */
/* ========================================================================= */
class Cursor
{
    friend class TLU;

public:
    inline Cursor(CXCursor const &cursor, CXCursor const &parent)
        : m_cursor(cursor)
        , m_parent(parent)
    {}

    inline CXCursorKind
    kind()
    {
        return clang_getCursorKind(m_cursor);
    }

    inline CXStringRAII
    name()
    {
        return clang_getCursorSpelling(m_cursor);
    }

    inline CXStringRAII
    to_string()
    {
        return clang_getCursorKindSpelling(clang_getCursorKind(m_cursor));
    }

private:
    CXCursor m_cursor;
    CXCursor m_parent;
};


/* ========================================================================= */
/* Translation Unit                                                          */
/* ========================================================================= */
class TLU
{
    friend class Args;
    friend class Cursor;

    inline TLU(TLU const &) = delete;
    inline TLU &operator=(TLU const &) = delete;

public:
    inline TLU()
    {
    }

    inline
    ~TLU()
    {
        if (m_idx != 0)
            clang_disposeIndex(m_idx);
        if (m_tlu != nullptr)
            clang_disposeTranslationUnit(m_tlu);
    }

    inline
    TLU(TLU &&other)
    {
        move(std::move(other));
    }

    inline TLU &
    operator=(TLU &&other)
    {
        move(std::move(other));
        return *this;
    }

    inline bool
    init(char const *path, Args *args = nullptr)
    {
        m_idx = clang_createIndex(0, 0);
        CXErrorCode err = clang_parseTranslationUnit2(
            m_idx,
            path,
            args != nullptr ? args->m_args.data() : nullptr,
            args != nullptr ? (int)args->m_args.size() : 0,
            nullptr,
            0,
            CXTranslationUnit_SkipFunctionBodies,
            &m_tlu);
        if (err != 0)
            return false;

        return true;
    }

    inline void
    visit(void (*cb)(Cursor, void *), void *ctx)
    {
        assert(m_idx != 0 && "not correctly initialized");
        assert(m_tlu != nullptr && "not correctly initialized");

        struct Data {
            void *ctx;
            void (*cb)(Cursor, void *);
        } data;
        data.ctx = ctx;
        data.cb = cb;

        CXCursor cursor = clang_getTranslationUnitCursor(m_tlu);
        clang_visitChildren(cursor,
            [](CXCursor c, CXCursor parent, CXClientData data)
            {
                ((Data *)data)->cb(Cursor(c, parent), ((Data *)data)->ctx);
                return CXChildVisit_Recurse;
            }, &data);
    }


private:
    inline void
    move(TLU &&other)
    {
        this->m_idx = other.m_idx;
        this->m_tlu = other.m_tlu;
        other.m_idx = 0;
        other.m_tlu = nullptr;
    }

private:
    CXIndex m_idx = 0;
    CXTranslationUnit m_tlu = nullptr;
};

} /* namespace clang */


#endif /* CLANG_HPP */


