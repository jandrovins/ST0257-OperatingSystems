#include "disk_sched.hpp"
#include "PageReplacement.hpp"
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
        short mem_type;
        cout << "Algorithm type selected: MEMORY" << endl
             << "Select the algorithm (1=FIFO;2=LRU;3=OPT): ";
        cin >> mem_type;
        if (!(mem_type >= 1 && mem_type <= 3)) {
            cerr << "ERROR: Memory type entered not valid. Must be between 1 and 3\n";
            return -1;
        }
        int n, m, i;
        cout << "Enter number of frames\n";
        cin >> n;
        cout << "Enter number of pages\n";
        cin >> m;
        int pages[m];
        cout << "Enter pages\n";
        for (i = 0; i < m; i++) {
            cin >> pages[i];
        }
        for (i = 0; i < m; i++) {
            cout << pages[i];
        }
        int pf;
        cout << "\n";
        if (mem_type == 1) {
            pf = FIFOPageFaultNum(pages, m, n);
        } else if (mem_type == 2) {
            pf = LRUPageFaultNum(pages, m, n);
        } else if (mem_type == 3) {
            pf = OPTPageFaultNum(pages, m, n);
        }
        cout << "There were " << pf << "page faults" << endl;

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
