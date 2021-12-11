# kkill
a simple process killer by name

simply kkill program1 program2 ...

will send a kill signal to all processes running that match the parameters.

depending on your system, change const char psflags, signal and

PROCESSPOSITION, PROGRAMPOSITION (counting from zero).

as it is now, it is configured for Ubuntu/Debian. I don't plan to expand the code.

use and modfy at your own risk :)
