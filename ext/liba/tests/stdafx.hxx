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

#ifndef STDAFX_HXX
#define STDAFX_HXX
#pragma once

/* ========================================================================= */
/* Library                                                                   */
/* ========================================================================= */


/* ========================================================================= */
/* Catch2                                                                    */
/* ========================================================================= */
#if defined(USING_CATCH)
    #define CATCH_CONFIG_ALL_PARTS
    #include "catch.hpp"
#endif


#endif /* STDAFX_HXX */

