#include "drive_rebuild.h"

vector<string> page_drive_rebuild_raid_devices()
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

bool rebuild_device_is_resyncing(string device)
{
    string action = trim_copy(exec("cat /sys/class/block/" + device + "/md/sync_action"));
    int degraded = atoi(trim_copy(exec("cat /sys/class/block/" + device + "/md/degraded")).c_str());
    return action == "recover" && degraded == 1;
}

int rebuild_device_get_total_devices(string device)
{
    string info = exec("cat /sys/class/block/" + device + "/md/raid_disks");
    return atoi(info.c_str());
}

int rebuild_device_get_working_devices(string device)
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

double rebuild_device_get_progress(string device)
{
    string info = exec("cat /proc/mdstat | awk '{ if(NR!=1 && $2==\":\" && $1 == \"" + device + "\") { getline; getline; gsub(\"%\",\"\",$4); gsub(\"finish=\",\"\",$6); gsub(\"min\",\"\",$6); gsub(\"speed=\",\"\",$7); gsub(\"K/sec\",\"\",$7); print $4; } }'");
    return atof(info.c_str());
}

double rebuild_device_get_time(string device)
{
    string info = exec("cat /proc/mdstat | awk '{ if(NR!=1 && $2==\":\" && $1 == \"" + device + "\") { getline; getline; gsub(\"%\",\"\",$4); gsub(\"finish=\",\"\",$6); gsub(\"min\",\"\",$6); gsub(\"speed=\",\"\",$7); gsub(\"K/sec\",\"\",$7); print $6; } }'");
    return atof(info.c_str());
}

long rebuild_device_get_speed(string device)
{
    string info = exec("cat /sys/class/block/" + device + "/md/sync_speed");
    return atof(info.c_str());
}

bool check_drive_rebuild()
{
    vector<string> devices = page_drive_rebuild_raid_devices();
    for (int i = 0; i < devices.size(); i++)
    {
        if (rebuild_device_is_resyncing(devices.at(i)))
            return true;
    }
    return false;
}

char *recover_kbytes_to_human_readable(long bytes /*in bytes*/, char *buf)
{

    char *suffix[] = {"KB", "MB", "GB", "TB"};
    char length = sizeof(suffix) / sizeof(suffix[0]);

    int i = 0;
    double dblBytes = bytes;

    if (bytes > 1024)
    {
        for (i = 0; (bytes / 1024) > 0 && i < length - 1; i++, bytes /= 1024)
            dblBytes = bytes / 1024.0;
    }

    sprintf(buf, "%.0lf %s", dblBytes, suffix[i]);
    return buf;
}

char *recover_min_to_human_readable(double mins /*in bytes*/, char *buf)
{

    char *suffix[] = {"min", "h", "d", "m", "y"};
    double dividers[] = {60, 24, 30, 12};
    char length = sizeof(suffix) / sizeof(suffix[0]);

    int i = 0;
    double dblmin = mins;

    while (dblmin > dividers[i])
    {
        dblmin /= dividers[i];
        i++;
    }

    sprintf(buf, "%.02lf %s", dblmin, suffix[i]);
    return buf;
}

void page_drive_rebuild_display(string device)
{
    int total = 0;
    int active = 0;

    double percent = rebuild_device_get_progress(device);
    double time = rebuild_device_get_time(device);
    long speed = rebuild_device_get_speed(device);

    reset_fbuffer();
    char line[200];
    sprintf((char *)&line, "Raid recovery:");
    draw_text_center((char *)&line, &Roboto_Regular6pt7b, 63, 10, true);
    draw_text_center((char *)device.c_str(), &Roboto_Regular6pt7b, 63, 26, true);
    char speed_str[20];
    char time_str[20];
    recover_kbytes_to_human_readable(speed, (char *)&speed_str);
    recover_min_to_human_readable(time, (char *)&time_str);
    sprintf((char *)&line, "%s/s %s", (char *)&speed_str, time_str);
    draw_text_center((char *)&line, &Roboto_Regular6pt7b, 63, 42, true);
    draw_progressbar(10, 48, 128 - 20, 14, round(percent));
    update_display();
}