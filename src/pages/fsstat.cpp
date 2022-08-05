#include "fsstat.h"

using namespace std;

vector<string> page_fsstat_devices()
{
    string info = exec("mount | awk '{ if($3 ~ /^\\/srv\\/dev-disk-.*/) { print $1 } }'");
    string line;
    stringstream ss(info);
    vector<string> devices;

    while (getline(ss, line, '\n'))
    {
        devices.push_back(line);
    }
    return devices;
}

string device_get_mountpoint(string device)
{
    return trim_copy(exec("mount | awk '{ if($1 == \"" + device + "\") { print $3 } }'"));
}

long device_get_used(string device)
{
    string info = exec("df /srv/dev* | awk '{ if (NR!=1 && $1==\"" + device + "\") { {print $3} } }'");
    return atol(info.c_str());
}

long device_get_total(string device)
{
    string info = exec("df /srv/dev* | awk '{ if (NR!=1 && $1==\"" + device + "\") { {print $4} } }'");
    return atol(info.c_str());
}

char *kbytes_to_human_readable(long bytes /*in bytes*/, char *buf)
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

    sprintf(buf, "%.02lf %s", dblBytes, suffix[i]);
    return buf;
}

void page_fsstat_display(string device)
{
    string mountpoint = device_get_mountpoint(device);
    
    struct statvfs info;
    statvfs(mountpoint.c_str(), &info);
    unsigned long total = (double)(info.f_blocks * info.f_frsize) / 1024;
    unsigned long available = (double)(info.f_bfree * info.f_frsize) / 1024;
    unsigned long used = total - available;
    double percent = used * 100.0 / total;

    reset_fbuffer();
    char line[200];
    sprintf((char *)&line, "Space usage:");
    draw_text_center((char *)&line, &Roboto_Regular6pt7b, 63, 10, true);
    draw_text_center((char *)device.c_str(), &Roboto_Regular6pt7b, 63, 26, true);
    char used_space[20];
    char total_space[20];
    kbytes_to_human_readable(used, (char *)&used_space);
    kbytes_to_human_readable(total, (char *)&total_space);
    sprintf((char *)&line, "%s of %s", (char *)&used_space, (char *)&total_space);
    draw_text_center((char *)&line, &Roboto_Regular6pt7b, 63, 42, true);
    draw_progressbar(10, 48, 128 - 20, 14, percent);
    update_display();
}