##
# CURRENTLY
##

# Polling

I want it to poll for 2 events:
 - Server message
 - User input

[For Polling]
https://old.reddit.com/r/C_Programming/comments/eeqi8r/understanding_how_to_use_poll_in_socket/
POLLING-EXAMPLE.cpp


# Aesthetic

I want to keep the user input at the bottom of the screen.
Everything above the bottom line should contain input
both from the server and from the user.

Probably just want to look into Curses library, but I'm
curious if it would be stupid to do this:
    1.) Variable that contains all strings from the last n lines.
    2.) append new stdin + \r\n in that line. 
    3.) Print inputted text from user (so far, pre-enter without \r\n.)
    4.) Refresh this every n seconds?
I think that might be called a "screen buffer" probably.

[For nCurses]
https://dev.to/tbhaxor/introduction-to-ncurses-part-1-1bk5
https://www.gnu.org/software/ncurses/
man ncurses

If I do work with ncurses, I should make a small side project
with it as a sort of template or something.

