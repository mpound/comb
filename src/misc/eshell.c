/*
 * eshell - execute a shell command
 */
void eshell_(char *command)
{
	int system();

	if(system(command))
		error_("Error executing shell");
}
