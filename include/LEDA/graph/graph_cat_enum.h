/*******************************************************************************
+
+  LEDA 7.0  
+
+
+  graph_cat_enum.h
+
+
+  Copyright (c) 1995-2023
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef GRAPH_CATEGORY
#define GRAPH_CATEGORY

LEDA_BEGIN_NAMESPACE

enum 
{
  leda_graph_category,         
  undirected_graph_category, 
  directed_graph_category,      
  bidirectional_graph_category,
  opposite_graph_category,     
  bidirected_graph_category,
  filter_opposite_graph_category
};

LEDA_END_NAMESPACE

#endif
