#!/bin/csh -f
#
# script to generate HTML page from "lc" command.
# MWP - Fri Aug 23 10:56:07 PDT 1996
#

onintr Intrupt

/bin/rm -rf rm $HOME/comb.out
echo 'ro ;lc' | $COMB/bin/comb >&/dev/null

set lines=`wc -l $HOME/comb.out`

printf '<HTML>\n<HEAD>\n <TITLE>Summary of Comb Commands </TITLE>\n'
printf '<LINK REV="MADE" HREF="mailto:mpound@astro.umd.edu">\n</HEAD>\n'
printf '<BODY>\n<H3> A Summary of COMB Commands </H3>\n<HR>\n'

set n=1
@ lines++
while ($n < $lines)
  set var=`head -$n $HOME/comb.out|tail -1`
  set elem=$#var
  echo -n '<BR><A HREF="doc/'$var[1]'.html">'$var[1]'</A> -'
  set j=3
  @ elem++
  while ($j<$elem)
     echo -n ' '$var[$j]
     @ j++
  end
  printf "\n"
  @ n++
end

printf "<HR><H5><TT>\nThis document was last updated on\n" 
echo `date`
printf "by <A HREF=http://astro.berkeley.edu/pound/home.html>Marc W. Pound</A>.\n"
printf "</TT> </H5> </BODY> </HTML>\n"

goto TheEnd

Intrupt:
echo "###### "$0" Interrupted -- OK ######"
 
TheEnd:
/bin/rm -rf rm $HOME/comb.out
exit 0

