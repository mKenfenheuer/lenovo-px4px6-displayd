#include "network.h"
#include "../strtools.h"
#include <algorithm> 
#include <cctype>
#include <locale>

vector<string> page_network_ips()
{
    string info = exec("hostname -i");
    string line; 
    stringstream ss(info);
    vector<string> devices;

    while(getline(ss, line, ' ')){
        string line_trim = trim_copy(line);
        if(line_trim.length() > 0)            
        {
            devices.push_back(line_trim);
        }
    }
    return devices;
}

void page_network_display(string ipaddress) {
    reset_fbuffer();
    draw_text_center((char *)"IP Address:", &Roboto_Regular6pt7b, 63, 27, true);
    draw_text_center((char *)ipaddress.c_str(), &Roboto_Regular6pt7b, 63 , 45, true);
    update_display();        
}