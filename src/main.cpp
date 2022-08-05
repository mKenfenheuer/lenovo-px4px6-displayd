#include "gfx.h"
#include "pages/all_pages.h"
#include "button_input.h"
#include <chrono>
#include <thread>
#include <vector>

using namespace std;

vector<string> devices;
vector<string> ips;
vector<string> rebuilds;
int page_index = 0;
int ip_index = 0;
int rebuild_index = 0;
long last_page_switch = 0;
int device_index = 0;
bool drive_failure = false;

void display_page()
{

    if (!check_drive_failure() && page_index == 3)
    {
        page_index++;
    }

    if (!check_drive_rebuild() && page_index == 4)
    {
        page_index = 0;
    }

    switch (page_index)
    {
    case 0:
        page_fsstat_display(devices.at(device_index));
        break;
    case 1:
        page_network_display(ips.at(ip_index));
        break;
    case 2:
        page_time_display();
        break;
    case 3:
        page_drive_failure_display();
        break;
    case 4:
        page_drive_rebuild_display(rebuilds.at(rebuild_index));
        break;
    }
}

void refresh_display()
{
    if (page_index >= PAGE_COUNT)
        page_index = 0;

    if (page_index < 0)
        page_index = 0;

    display_page();
}

void next_page()
{
    if (page_index == 0 && device_index < devices.size() - 1)
    {
        device_index++;
    }
    else if (page_index == 1 && ip_index < ips.size() - 1)
    {
        ip_index++;
    }
    else if (page_index == 4 && rebuild_index < rebuilds.size() - 1)
    {
        rebuild_index++;
    }
    else
    {
        page_index++;
        ip_index = 0;
        device_index = 0;
        rebuild_index = 0;
    }

    last_page_switch = 0;
}

void prev_page()
{
    if (page_index == 0 && device_index > 0)
    {
        device_index--;
    }
    else if (page_index == 1 && ip_index > 0)
    {
        ip_index--;
    }
    else if (page_index == 4 && rebuild_index > 0)
    {
        rebuild_index--;
    }
    else if (page_index == 0)
    {
        if (!check_drive_failure() && !check_drive_rebuild())
        {
            page_index = 2;
        }
        else if (!check_drive_failure() && check_drive_rebuild())
        {
            page_index = 3;
        }
        else
        {
            rebuild_index = rebuilds.size() - 1;
            page_index = 4;
        }
    }
    else if (page_index == 1)
    {
        device_index = devices.size() - 1;
        page_index = 0;
    }
    else if (page_index == 2)
    {
        ip_index = ips.size() - 1;
        page_index = 1;
    }
    else if (page_index == 4)
    {
        if (!check_drive_failure())
        {
            page_index = 2;
        }
        else
        {
            page_index = 3;
        }
    }
    else
    {
        page_index--;
        ip_index = 0;
        device_index = 0;
        rebuild_index = 0;
    }

    last_page_switch = 0;
}

int main(int argc, char *argv[])
{
    if (argc >= 3)
    {

        if (argc == 3 && strcmp(argv[1], "boot") == 0)
        {
            reset_fbuffer();

            draw_text_center((char *)"Booting System:", &Roboto_Regular6pt7b, 63, 27, true);
            draw_text_center(argv[2], &Roboto_Regular6pt7b, 63, 45, true);

            update_display();
            return 0;
        }

        if (argc == 4 && strcmp(argv[1], "boot") == 0)
        {
            reset_fbuffer();

            draw_text_center((char *)"Booting System:", &Roboto_Regular6pt7b, 63, 17, true);
            draw_text_center(argv[2], &Roboto_Regular6pt7b, 63, 30, true);
            int progress = atoi(argv[3]);
            draw_progressbar(10, 40, 128 - 20, 14, progress);
            update_display();
            return 0;
        }
    }

    devices = page_fsstat_devices();
    ips = page_network_ips();
    rebuilds = page_drive_rebuild_raid_devices();
    page_index = 0;
    refresh_display();

    uint8_t count = 0;
    while (true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        check_button_input();

        if (count % 10 == 0)
        {
            count = 0;
            last_page_switch++;

            drive_failure = check_drive_failure();

            if (drive_failure)
            {
                exec("/usr/bin/sysbeep");
            }

            if (last_page_switch >= 15)
            {
                if (page_index == 0)
                {
                    devices = page_fsstat_devices();
                }

                if (page_index == 1)
                {
                    ips = page_network_ips();
                }

                if (page_index == 4)
                {
                    rebuilds = page_drive_rebuild_raid_devices();
                }

                //next_page();
            }

            refresh_display();
        }

        if (isUpPressed())
        {
            next_page();
            refresh_display();
        }

        if (isDownPressed())
        {
            prev_page();
            refresh_display();
        }

        count++;
    }
}