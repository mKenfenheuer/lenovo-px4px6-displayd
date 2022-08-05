#include "time.h"
#include <ctime>

void page_time_display()
{
    std::time_t t = std::time(0); // get time now
    std::tm *now = std::localtime(&t);
    reset_fbuffer();
    char time[100];
    char date[100];
    sprintf((char *)&time, "%02d:%02d", now->tm_hour, now->tm_min);
    sprintf((char *)&date, "%02d.%02d.%04d", now->tm_mday, now->tm_mon + 1, now->tm_year + 1900);
    draw_text_center((char *)&time, &FreeSansBold9pt7b, 63, 27, true);
    draw_text_center((char *)&date, &Roboto_Regular6pt7b, 63, 45, true);
    update_display();
}