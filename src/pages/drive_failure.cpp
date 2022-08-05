#include "drive_failure.h"

vector<string> page_drive_failure_raid_devices()
{
    string info = exec("cat /proc/mdstat | awk '{ if(NR!=1 && $2==\":\") { print $1 } }'");
    string line;
    stringstream ss(info);
    vector<string> devices;

    while (getline(ss, line, '\n'))
    {
        devices.push_back(line);
    }
    return devices;
}

bool device_is_degraded(string device)
{
    int degraded = atoi(trim_copy(exec("cat /sys/class/block/" + device + "/md/degraded")).c_str());
    return degraded == 1;
}

int device_get_total_devices(string device)
{
    string info = exec("cat /sys/class/block/" + device + "/md/raid_disks");
    return atoi(info.c_str());
}

int device_get_working_devices(string device)
{
    string info = exec("echo  /sys/class/block/" + device + "/md/dev-*");
    int devices = 0;
    string line;
    stringstream ss(info);

    while (getline(ss, line, ' '))
    {
        string trim = trim_copy(line);
        if (trim.length() > 0)
            devices++;
    }
    return devices;
}

bool check_drive_failure()
{
    vector<string> devices = page_drive_failure_raid_devices();
    for (int i = 0; i < devices.size(); i++)
    {
        string device = devices.at(i);
        int total = device_get_total_devices(device);
        int working = device_get_working_devices(device);
        bool is_degraded = device_is_degraded(device);
        if (working < total && is_degraded)
            return true;
    }
    return false;
}

int total = 0;
int active = 0;

void page_drive_failure_refresh()
{
    total = 0;
    active = 0;
    vector<string> devices = page_drive_failure_raid_devices();
    for (int i = 0; i < devices.size(); i++)
    {
        string device = devices.at(i);
        total += device_get_total_devices(device);
        active += device_get_working_devices(device);
    }
}

void page_drive_failure_display()
{
    page_drive_failure_refresh();
    reset_fbuffer();
    draw_text_center((char *)"Attention!", &FreeSansBold9pt7b, 63, 20, true);
    char line[50];
    sprintf((char *)&line, "%d of %d drives", total - active, total);
    draw_text_center((char *)&line, &Roboto_Regular6pt7b, 63, 36, true);
    draw_text_center("in RAID failed!", &Roboto_Regular6pt7b, 63, 52, true);
    update_display();
}