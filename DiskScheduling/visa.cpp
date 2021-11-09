#include "disk_sched.hpp"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>

using namespace std;
int main()
{
    short algo_type;
    cout << "Select the algorithm type to simulate (1=Memory;2=Disk): ";
    cin >> algo_type;
    if (!(algo_type >= 1 && algo_type <= 2)) {
        cerr << "ERROR: Algorithm type entered not valid. Must be between 1 and 2\n";
        return -1;
    }
    if (algo_type == 1) {
        // CODE FOR PROCESS
    } else if (algo_type == 2) {
        short disk_type;
        cout << "Algorithm type selected: DISK" << endl
             << "Select the algorithm (1=FCFS;2=SSTF;3=SCAN;4=C-SCAN): ";
        cin >> disk_type;
        if (!(disk_type >= 1 && disk_type <= 4)) {
            cerr << "ERROR: Disk type entered not valid. Must be between 1 and 4\n";
            return -1;
        }
        int init_pos;
        vector<int> reqs;
        read_uinput(init_pos, reqs);
        cout << "\n\nSequence entered: ";
        for (auto v : reqs) {
            cout << v << " | ";
        }
        cout << endl;
        if (disk_type == 1) {
            fcfs(init_pos, reqs);
        } else if (disk_type == 2) {
            sstf(init_pos, reqs);
        } else if (disk_type == 3) {
            scan(init_pos, reqs);
        } else if (disk_type == 4) {
            c_scan(init_pos, reqs);
        }
    }

    return 0;
}
