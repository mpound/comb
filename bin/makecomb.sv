#!/bin/sh  -x
#
# Usage: makecomb [install/depend] [nohistory]
#
# if install is given, then ldcmb is run and comb binary
# is overwritten. If an HP machine the binary is named Comb.
#
# if depend is given, then the file 'dependencies' in each
# source directory is constructed.
#
# if nohistory is given then comb is compiled without the
# history mechanism
#
# if no options are given then all source is compiled but not linked
# with ldcmb.
#
# Last Change: Mon Dec  6 18:12:03 EST 1999  
#   automated the history mechanism compilation. MWP
#

umask 022
run_ldcmb=no   # are we installing?

#
# Default is history is on
#
history=true
today=`date`
#cat > $COMB/src/main/History.h << EOF
#/* This file is automatically generated on $today. Do not edit.*/
#/* HISTORY is 1 if compiling with comb history mechanism ON
# *            0 otherwise
# *            default is ON
# */
##define HISTORY 1
#EOF

case $# in
0)  ;;
1)  case $1 in
#
# We must test for existence of gcc here, since dependency files
# are written from redirected stdout (>). The test is done by
# grepping for 'version' in the output of gcc -v (which is written
# to stderr, so must be redirected to stdout!)
#
	depend) m=`gcc -v 2>&1 | grep -c version`;
		if test $m -ne 1
		then
			echo You do not have gcc, so you cannot make dependencies.;
			echo Bye;
			exit;
		fi;
		run_ldcmb=d;
		make_option=depend;
		echo make_option=$make_option;;
	install) run_ldcmb=yes;;
	nohistory) echo Compiling with history OFF;
		   cat > $COMB/src/main/History.h << NOHISTORY ;;
/* This file is automatically generated on $today. Do not edit.*/
/* HISTORY is 1 if compiling with comb history mechanism ON
 *            0 otherwise
 *            default is ON
 */
#define HISTORY 0
NOHISTORY

	*) echo "Usage: $0 [install/depend] [nohistory]";exit;;
    esac;;
2) case $2 in
        depend) m=`gcc -v 2>&1 | grep -c version`;
                if test $m -ne 1
                then
                        echo You do not have gcc, so you cannot make dependencies.;
                        echo Bye;
                        exit;
                fi;
                run_ldcmb=d;
                make_option=depend;
                echo make_option=$make_option;;
        install) run_ldcmb=yes;;
  	nohistory) history=false;
		echo Compiling with history OFF;
		cat > $COMB/src/main/History.h << NOHISTORYX ;;
/* This file is automatically generated on $today. Do not edit.*/
/* HISTORY is 1 if compiling with comb history mechanism ON
 *            0 otherwise
 *            default is ON
 */
#define HISTORY 0
NOHISTORYX

	*) echo "Usage: $0 [install/depend] [nohistory]";exit;;
*) echo "Usage: $0 [install/depend] [nohistory]";exit;;
esac

echo comb path is ${COMB}
cd ${COMB}/src

#-------------------------------------------------
# NB: HISTORY IS NOW DECIDED ON THE COMMAND LINE 
#-------------------------------------------------
# To make comb with a history mechanism, set HISTORY to 1 in $COMB/src/main/C.h
# and comment out the 3rd and 4th lines below (the ones without libedit in
# them).  If  the history mechanism seems not to work, set HISTORY to 0
# and comment out the lines below with libedit in them (and uncomment the other 
# ones)
#--------------------------------

if  [ $history = true ]; then
    for i in coordsys error graphics image main misc misc/libut misc/libedit\
	parse parse/entree scan2 scan stacks user fits/fitshd
    do
	echo "$i	\c"
	(cd $i;make $make_option)
    done
else
 for i in coordsys error graphics image main misc misc/libut\
	parse parse/entree scan2 scan stacks user fits/fitshd
    do
	echo "$i	\c"
	(cd $i;make $make_option)
    done
fi

cd cmd
for i in *
do
	echo "$i	\c"
	(cd $i;make $make_option)
done

# now decide whether or not to run ldcmb and mvcmb

case $run_ldcmb in
yes) echo  ;echo doing ldcmb...;  ldcmb; mvcmb;;
no)  echo ;echo ldcmb bypassed - do manually to install cmbt;
	echo then use 'mvcmb' to mv cmbt to [cC]omb;;
d) ;;
esac

exit