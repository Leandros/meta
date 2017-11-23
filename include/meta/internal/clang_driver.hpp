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

#ifndef CLANG_DRIVER
#define CLANG_DRIVER
#pragma once

#include <stdio.h>
#include <a/file.hxx>
#include <a/error.hxx>
#include <a/string.hxx>
#include <a/buffer.hxx>
#include <a/platform.hxx>
#include <a/utilities.hxx>
#include <a/tl/optional.hpp>


namespace clang
{

/* ========================================================================= */
/* CLANG.EXE driver                                                          */
/* ========================================================================= */
class driver
{
    friend tl::optional<driver> make_driver(char const *);
#if USING(OS_WINDOWS)
    static constexpr char const *CLANG_EXE = "clang++.exe";
#elif USING(OS_MAC)
    static constexpr char const *CLANG_EXE = "clang++";
#endif


public:
    /* ===================================================================== */
    /* Initialization                                                        */
    inline driver() {}

    /*!
     * \brief
     * \param[in] llvm_path
     * \param[in] path_override
     * \return
     */
    inline a::err_code
    init(char const *llvm_path, char const *path_override = nullptr)
    {
        char exe_path[a::PATHSIZE];
        if (!a::fs::get_exe_path(exe_path, sizeof(exe_path)))
            return false;
        if (path_override == nullptr)
            path_override = exe_path;

        int nlen = snprintf(nullptr, 0, "%s%c%s", path_override, PATH_SEPARATOR, llvm_path);
        if (nlen > a::PATHSIZE - 1)
            return false;
        snprintf(m_llvm_base, a::PATHSIZE, "%s%c%s", path_override, PATH_SEPARATOR, llvm_path);

        return true;
    }

    /*!
     * \brief
     * \param[in] args
     * \param[in] nargs
     * \return
     */
    inline a::buffer<char>
    invoke(char const **args, size_t nargs)
    {
        a::string_builder<1024> str;
        str.append_format("%s%cbin%c%s",
            m_llvm_base, PATH_SEPARATOR, PATH_SEPARATOR, CLANG_EXE);
        for (size_t i = 0; i < nargs; ++i)
            str.append_format(" %s", args[i]);

        a::buffer_builder<char> builder;
        a::process proc;
        if (!proc.create(str.get(), "r"))
            return {};

        char buf[4096];
        size_t n;
        while (proc.alive()) {
            while ((n = proc.read(buf, 4096)) > 0) {
                builder.append(buf, n);
            }
        }

        return builder.to_buffer();
    }

    /*!
     * \brief
     * \param[in]
     * \return
     * \remark Will call \c invoke internally, with the right flags.
     */
    inline a::buffer<char>
    preprocess(char const *path)
    {
        char const *args[] = {
            "-x c++",
            "-E",
            path
        };

        return invoke(args, ARR_SIZE(args));
    }


private:
    char m_llvm_base[a::PATHSIZE];
};


tl::optional<driver>
make_driver(char const *llvm_path, char const *path_override = nullptr)
{
    driver r;
    if (!r.init(llvm_path, path_override))
        return tl::nullopt;
    return std::move(r);
}

} /* namespace clang */


#endif /* CLANG_DRIVER */

