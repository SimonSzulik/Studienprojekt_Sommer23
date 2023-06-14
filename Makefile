WFLAGS = -DWINAIN

LROOT   = ../..
UNIXLIB = -lleda $(XLIB) -lm
DOSLIB  = ..\\..\\leda.lib $(XLIB)
DOSLIB1 = ..\\..\\GeoWin.lib

include ../Make.pro

