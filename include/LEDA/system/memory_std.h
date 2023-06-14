/*******************************************************************************
+
+  LEDA 7.0  
+
+
+  memory_std.h
+
+
+  Copyright (c) 1995-2023
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_MEMORY_STD_H
#define LEDA_MEMORY_STD_H

#include <LEDA/system/memory_manager.h>

LEDA_BEGIN_NAMESPACE

class __exportC memory_manager_init {

public:

static unsigned count;

  memory_manager_init();
 ~memory_manager_init();
};


// standard memory manager

extern __exportD memory_manager std_memory_mgr;

static memory_manager_init std_memory_mgr_init;

LEDA_END_NAMESPACE


#endif
