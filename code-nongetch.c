//http://forums.codeguru.com/showthread.php?343829-break-blocked-getchar-callint i;
    while (inputReadyWait())
    {
	if ((i = fgetc(stdin)) == EOF)
	    break;
	char c = (char) i;
	// do something with c;
    }
    
    int unblock = 0;		// (write-end of the pipe)
int block_check = 0;	// (read-end of the pipe)

bool inputReadyWait()
{
    if (!unblock)
    {
	// no block/unblock pipe yet....
	int unblock_pipe[2];
	if (pipe(unblock_pipe))
	    return false;
	unblock = unblock_pipe[1];		// unblock key
	block_check = unblock_pipe[0];	// block check
    }

    // find out if (input_is_ready_on_stdin || unblock_key_has_been_activated)
    fd_set read_set;
    FD_ZERO(&read_set);
    FD_SET(0,&read_set);
    FD_SET(block_check,&read_set);	// if there's anything to read on the pipe unblock_key has been activated!

    // wait until input is ready or a cancel wait was issued
    if (select(block_check+1,&read_set,NULL,NULL,NULL) == -1)
	return false;

    if (FD_ISSET(0,&read_set))
	return true;			// input ready on stdin
    else
    {
	if (block_check)		// user issued cancel wait!
	    close(block_check);
	block_check = 0;
	return false;
    }

}

void cancelInputWait()
{
    if (unblock)
    {
	write(unblock,'\0',1);	// stub!
	close(unblock);
	unblock = 0;
    }
}

