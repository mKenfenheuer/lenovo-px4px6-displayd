#include <stdio.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <sys/time.h>

struct input_event
{
    struct timeval time;
    unsigned short type;
    unsigned short code;
    unsigned int value;
};

bool check_button_input();
bool isUpPressed();
bool isDownPressed();