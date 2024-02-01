The example code is just to show
how to deal with multiple clients
at once.

* Rewrite this to better understand it.
* Write a client that sends messages.

After we do this, we can look at the
client sending commands, server processing
those commands.

Cool ideas:

* users
    - Create
    - Login
* encryption
* chat with other clients
* 

----
##
# Quick note about poll()
##

For initializing:
```
struct pollfd *pfds = malloc(sizeof *pfds * fd_size);

// Add the listener to set
pfds[0].fd = listener;
pfds[0].events = POLLIN; // Report ready to read on incoming connection

```

For classes:
```
// Add a new file descriptor to the set
void add_to_pfds(struct pollfd *pfds[], int newfd, int *fd_count, int *fd_size)
{
    // If we don't have room, add more space in the pfds array
    if (*fd_count == *fd_size) {
        *fd_size *= 2; // Double it

        *pfds = realloc(*pfds, sizeof(**pfds) * (*fd_size));
    }

    (*pfds)[*fd_count].fd = newfd;
    (*pfds)[*fd_count].events = POLLIN; // Check ready-to-read

    (*fd_count)++;
}

// Remove an index from the set
void del_from_pfds(struct pollfd pfds[], int i, int *fd_count)
{
    // Copy the one from the end over this one
    pfds[i] = pfds[*fd_count-1];

    (*fd_count)--;
}
```

For the main loop:
    Check: poll-example.cpp


From a quick skim, I think the way this works is that it doesn't 
wait for an interaction, but rather loops through, does a quick check
for an update, and then moves on if one isn't detected. I could be wrong,
so I'll need to look over it and everything.

This is probably the last real peice of the puzzle before I can get
genuine stuff going.

On a side note, it might be a good idea to go ahead and get a
ncurse client working while trying to figure this out.


##
# CURRENTLY
##

* How to send a message to all connected clients?
    It bugs out pretty bad if I try to, so I
    commented out what I was trying. Also, I need
    to make polling for the client. So, I'm taking
    a break from that until I can get polling to
    work in the client. 

* SIDE MISSION:
    The main loop will get massive if I don't create
    a separate file that can deal with the commands
    from the clients. I should probably deal with that.



