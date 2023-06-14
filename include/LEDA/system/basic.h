/*******************************************************************************
+
+  LEDA 7.0  
+
+
+  basic.h
+
+
+  Copyright (c) 1995-2023
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_BASIC_H
#define LEDA_BASIC_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 700171
#include <LEDA/internal/PREAMBLE.h>
#endif

// define LEDA version macro

#define __LEDA__ 700

#define LEDA_CHECK_VERSION check_version_700


// enable stl-style iterators
#ifndef LEDA_STL_ITERATORS
#define LEDA_STL_ITERATORS
#endif

// turn checking off when building the libraries


#if defined(LEDA_INSTALL) && !defined(LEDA_CHECKING_OFF)
#define LEDA_CHECKING_OFF
#endif

// include system config file

#include <LEDA/internal/system.h>


// include basic LEDA headers

#include <LEDA/internal/global.h>

#include <LEDA/system/error.h>
#include <LEDA/system/thread.h>
#include <LEDA/system/memory.h>

#include <LEDA/internal/param_types.h>
#include <LEDA/internal/handle_types.h>
#include <LEDA/internal/iteration.h>

#include <LEDA/core/random_source.h>
#include <LEDA/core/string.h>
#include <LEDA/core/misc.h>



#if LEDA_ROOT_INCL_ID == 700171
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif

#endif
