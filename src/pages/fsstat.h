#include "../gfx.h"
#include "../process.h"
#include "../strtools.h"
#include <vector>
#include <iostream>
#include <iterator>
#include <sys/statvfs.h>
using namespace std;
vector<string> page_fsstat_devices();
void page_fsstat_display(string device);