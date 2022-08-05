#include "button_input.h"

int fd = NULL;

int upCount = 0;
int downCount = 0;

bool check_button_input()
{
    if (fd == NULL)
        fd = open("/dev/input/by-path/platform-asus_laptop-event", O_RDWR | O_NOCTTY | O_NDELAY);

    input_event input;
    int len = read(fd, &input, sizeof(input));
    while (len > 0)
    {
        if (input.type == 4 && input.code == 4)
        {
            if (input.value == 226)
            {
                printf("Button Up pressed!\n");
                upCount++;
            }
            else
            {
                printf("Button Down pressed!\n");
                downCount++;
            }
        }
        len = read(fd, &input, sizeof(input));
    }
}

bool isUpPressed()
{
    if (upCount > 0)
    {
        upCount = 0;
        return true;
    }
    return false;
}
bool isDownPressed()
{
    if (downCount > 0)
    {
        downCount = 0;
        return true;
    }
    return false;
}