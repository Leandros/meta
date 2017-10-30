# Meta <a href="https://ci.appveyor.com/project/Leandros/meta"><image src="https://ci.appveyor.com/api/projects/status/c23wea646bhtov50/branch/master" align="right"></a> <a href="https://travis-ci.org/Leandros/meta"><image src="https://travis-ci.org/Leandros/meta.svg?branch=master" align="right"></a>

An exercise in exploring compile time code generation in C++.

# Table of Contents

- [Requirements](#requirements)
- [Getting Started](#getting-started)
- [Branch Policy](#branch-policy)
- [Contributing](#contributing)
- [Changelog](#changelog)
- [License](#license)


# Requirements

To compile meta, you require the following:

- [CMake 3.2 or higher](https://cmake.org/download/)
- A C++ compiler, with C++14 support or higher

Currently explicitly supported are [clang](https://clang.llvm.org/) (windows, linux and mac),
[gcc](https://gcc.gnu.org/) (linux and mac), and [msvc](https://www.visualstudio.com/) (windows).

Any other compiler will not receive special treatment, but as long as it's supported
by CMake, and has decent C++14 support should work.


# Getting Started

It's recommended to compile meta out-of-source. Open your shell and generate
the build files with cmake. The following example will use [ninja](https://ninja-build.org/)
as generator, but you're free to use any generator you like:

```
$ git clone https://github.com/Leandros/meta.git
$ cd meta
$ mkdir build
$ cmake .. -G"Ninja"
$ cmake --build .
```


# Branch Policy

The `master` branch is stable at all times, and will only be updated for new
releases or bugfixes.

The `dev` branch is the main development branch, **all pull-requests should be
submitted to `dev`.** It might be unstable at times.

Any other branch is likely a working branch, and might contain incomplete changes.
Do not attempt to deploy, or file any pull-request against them.


# Contributing

Please read the [CONTRIBUTING].


# Changelog

The changelog contains the changes between versions.

Read the [CHANGELOG on dev] for latest bleeding edge changes.

Read the [CHANGELOG on master] for latest stable changes.


# License

Copyright (C) 2017 Arvid Gerstmann

meta is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

meta is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with meta.  If not, see <http://www.gnu.org/licenses/>.


[CONTRIBUTING]:         https://github.com/Leandros/meta/blob/dev/CONTRIBUTING.md#
[CHANGELOG on master]:  https://github.com/Leandros/meta/blob/master/CHANGELOG.md#
[CHANGELOG on dev]:     https://github.com/Leandros/meta/blob/dev/CHANGELOG.md#

