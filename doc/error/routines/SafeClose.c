.sh 2 "SafeClose(<fildes>)"
.pp
<fildes> is an integer.
.pp
SafeClose closes a file that was opened with SafeOpen.
<fildes> is the file descriptor of the file to be closed.
.pp
If <fildes> is not the file descriptor of a file that was opened with
SafeOpen, then SafeClose will error out with the message
"Trying to close a file that was not properly opened."
