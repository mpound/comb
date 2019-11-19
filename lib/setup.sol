#!/bin/sh
#This version is for the SUNWspro compiler.
#echo setup:COMB is $COMB
	cd $COMB/bin; cp ldcmb.std ldcmb; cp mvcmb.std mvcmb;chmod 777 mvcmb
	cd $COMB/src/main;  cp include.mk.sunpro include.mk
	cd $COMB/src/main; cp machinedep.h.solaris machinedep.h
	cd $COMB/lib; cp xref.solaris xref
