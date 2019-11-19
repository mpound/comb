.sh 2 "SetStkDir(<directory>)"
.pp
<directory> is an integer.
If <directory> is not FOREGROUND or BACKGROUND,
then SetStkDir will error out with the
message: "Bad foreground/background specifier."
.pp
SetStkDir sets the stacks directory to either foreground or background
depending on whether <directory> is FOREGROUND or BACKGROUND respectively.
All future operations will access only the directory specified by
SetStkDir.
.pp
If no foreground directory has been opened with OpenStkDir, and <directory>
is FOREGROUND, then SetStkDir errors out with the message "No open
foreground stack directory."
Similarly, if no background directory has been opened with OpenStkDir, and
<directory> is BACKGROUND, then SetStkDir errors out with the message
"No open background stack directory."
