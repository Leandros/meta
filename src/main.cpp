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

#include <a/io.hxx>
#include <a/utilities.hxx>
#include <meta/config.h>
#include <meta/parser.hpp>
#include <meta/internal/clang.hpp>
#include <meta/internal/clang_driver.hpp>

#define OPTPARSE_IMPLEMENTATION
#define OPTPARSE_API static
#include <a/optparse/optparse.h>


/* ========================================================================= */
/* Options                                                                   */
/* ========================================================================= */
static struct optparse_long longopts[] = {
    /*
     * 1st arg is long option name. can be null.
     * 2nd arg is short option name. can be null.
     * 3rd arg controls the argument of option. Must be one of:
     *      OPTPARSE_NONE, OPTPARSE_REQUIRED, OPTPARSE_OPTIONAL
     * 4th arg is a short description, printed in help. (EXTENSION!)
     *
     * Arguments may only be added to the end! They're accessed by index!
     */
    { "help", '?', OPTPARSE_NONE, "Print help and exit" },
    { "version", 0, OPTPARSE_NONE, "Print version and exit" },
    { 0, 'I', OPTPARSE_REQUIRED, "Add include directory" },
    { "llvm", 0, OPTPARSE_REQUIRED, "Path to LLVM installation, if non-default" },
    { 0, 0, OPTPARSE_NONE, 0 }
};

static bool opt_help = false;
static bool opt_version = false;
static char const *opt_llvm = ".";

static void print_help(char const *name, bool exit = true);
static void print_version(char const *name);
static void add_include(char const *path);

static int
parse_options(int, char **argv)
{
    int option, index;
    struct optparse options;

    optparse_init(&options, argv);
    while ((option = optparse_long(&options, longopts, &index)) != -1) {
        switch (index) {
        case 0: opt_help = true; break;
        case 1: opt_version = true; break;
        case 2: add_include(options.optarg); break;
        case 3: opt_llvm = options.optarg; break;
        default:
            fprintf(stderr, "%s: %s\n", argv[0], options.errmsg);
            print_help(argv[0], false);
            return false;
        }
    }

    if (opt_help)
        print_help(argv[0], true);
    if (opt_version)
        print_version(argv[0]);

    return options.optind;
}


/* ========================================================================= */
/* Configuration                                                             */
#if USING(OS_WINDOWS)
    #define LLVM_PATH "..\\LLVM-5.0.0-win64"
#elif USING(OS_MAC)
    #define LLVM_PATH "../clang+llvm-5.0.0-x86_64-apple-darwin"
#endif


/* ========================================================================= */
/* ENTRY                                                                     */
/* ========================================================================= */
int
main(int argc, char **argv)
{
    int optind;
    if ((optind = parse_options(argc, argv)) == -1)
        return 1;
    if (optind == argc)
        return fprintf(stderr, "error: requires file argument\n"), 1;

    auto driver = clang::make_driver(LLVM_PATH);
    if (!driver)
        return 1;

    for (int i = optind; i < argc; ++i) {
        /* ==== Preprocess ==== */
        a::buffer<char> buf = driver->preprocess(argv[i]);
        if (!buf)
            return 1;

        /* ==== Parse the internal programs ==== */
        meta::parser p;
        if (!p.init_from_buffer(buf))
            return 1;
        p.run();
    }


#if 0
    clang::TLU tlu;
    if (!tlu.init("test.h"))
        return 1;

    auto cb = [](clang::Cursor c, void *ctx)
    {
        printf("%s => %s\n",
            c.name().c_str(), c.to_string().c_str());
    };
    tlu.visit(cb, nullptr);
#endif

    return 0;
}


/* ========================================================================= */
/* Help / Version                                                            */
/* ========================================================================= */
static void
print_help(char const *name, bool shall_exit)
{
    printf("USAGE: %s [OPTION...] FILE...\n", name);
    printf("\n");
    printf("OPTIONS:\n");
    printf("\n");

    optparse_print_help(longopts);

    if (shall_exit)
        exit(EXIT_SUCCESS);
}

static void
print_version(char const *name)
{
    printf("%s v%s (%s)\n", name, "1.0.0", "debug");
    printf("Copyright (C) 2017 Arvid Gerstmann\n");
    exit(EXIT_SUCCESS);
}

static void
add_include(char const *path)
{
    if (path == nullptr)
        return;

    printf("add_include(%s)\n", path);
}

