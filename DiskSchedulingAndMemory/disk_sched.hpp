#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <vector>
#define NUM_REQS 10 //

using namespace std;
void read_uinput(int& init_pos, vector<int>& reqs)
{ // read user input
    cout << "Enter the initial position of the reading head: ";
    cin >> init_pos;
    for (size_t i = 0; i < NUM_REQS; i++) {
        cout << "Enter the a position to read (" << NUM_REQS - i << " left): ";
        int tmp;
        cin >> tmp;
        if (tmp < 0) {
            cerr << "ERROR: Entered negative number.\n";
            exit(2);
        }
        reqs.push_back(tmp);
    }
}
void fcfs(int init_pos, vector<int> reqs)
{
    cout << "\nFCFS: (init_pos=" << init_pos << ")\nREAD SEQUENCE:\n";
    long distance = 0;
    int last_pos = init_pos;
    for (size_t i = 0; i < reqs.size(); i++) {
        int v = reqs[i];
        cout << "    " << v << endl;
        distance += labs((long)(v - last_pos));
        last_pos = v;
    }
    cout << "Printing distance traversed:";
    cout << "    DISTANCE=" << distance << endl;
}

int find_closest(int val, vector<int>& v, vector<int>::iterator& closest)
{
    vector<int>::iterator closest_larger = lower_bound(v.begin(), v.end(), val);
    int distance;
    if (closest_larger == v.end()) { // not found larger in v
        closest = v.end() - 1;
        distance = labs((long)(*closest - val));
    } else {
        int dist1 = labs((long)(*closest_larger - val));
        if (closest_larger == v.begin()) {
            closest = closest_larger;
            distance = dist1;
        } else {
            int dist2 = labs((long)(*(closest_larger - 1) - val));
            if ((closest_larger - 1) != v.begin() && (dist1 < dist2)) {
                closest = closest_larger;
                distance = dist1;
            } else {
                closest = (closest_larger - 1);
                distance = dist2;
            }
        }
    }
    return distance;
}
void sstf(int init_pos, vector<int> reqs)
{
    cout << "\nSSTF: (init_pos=" << init_pos << ")\nREAD SEQUENCE:\n";
    sort(reqs.begin(), reqs.end());
    int size = reqs.size();
    long distance = 0;
    int last_pos = init_pos;
    vector<int>::iterator closest;
    int movements = 0;
    while (size - movements > 0) {
        vector<int>::iterator closest;
        int dist_closest = find_closest(last_pos, reqs, closest);
        distance += dist_closest;
        cout << "    " << *closest << endl;
        movements++;
        last_pos = *closest;
        reqs.erase(closest);
    }
    cout << "Printing distance traversed:";
    cout << "    DISTANCE=" << distance << endl;
}

int scan_flow_right(vector<int>& reqs, vector<int>::iterator it, int& distance, int init_pos)
{
    int last_pos = init_pos;
    while (it != reqs.end()) {
        cout << "    " << *it << endl;
        distance += labs((long)(last_pos - *it));
        last_pos = *it;
        it = reqs.erase(it);
    }
    return last_pos;
}
int scan_flow_left(vector<int>& reqs, vector<int>::iterator it, int& distance, int init_pos)
{
    int last_pos = init_pos;
    while (it != reqs.begin() - 1) {
        cout << "    " << *it << endl;
        distance += labs((long)(last_pos - *it));
        last_pos = *it;
        it = reqs.erase(it);
        if (it != reqs.end()) {
            if (*it != last_pos) { // to not ignore when repeated numbers
                it--;
            }
        } else {
            it--;
        }
    }
    return last_pos;
}
void scan(int init_pos, vector<int> reqs)
{
    cout << "\nSCAN: (init_pos=" << init_pos << ")\nREAD SEQUENCE:\n";
    sort(reqs.begin(), reqs.end());
    int distance = 0;
    vector<int>::iterator first;
    find_closest(init_pos, reqs, first);
    int last_pos;
    if (labs((long)(*first - reqs.front())) < labs((long)(*first - reqs.back()))) {
        last_pos = scan_flow_left(reqs, first, distance, init_pos);
        scan_flow_right(reqs, reqs.begin(), distance, last_pos);
    } else {
        last_pos = scan_flow_right(reqs, first, distance, init_pos);
        scan_flow_left(reqs, reqs.end() - 1, distance, last_pos);
    }
    cout << "Printing distance traversed:";
    cout << "    DISTANCE=" << distance << endl;
}
void c_scan(int init_pos, vector<int> reqs)
{
    cout << "\nC-SCAN: (init_pos=" << init_pos << ")\nREAD SEQUENCE:\n";
    sort(reqs.begin(), reqs.end());
    int distance = 0;
    vector<int>::iterator first;
    find_closest(init_pos, reqs, first);
    int last_pos;
    if (labs((long)(*first - reqs.front())) < labs((long)(*first - reqs.back()))) {
        last_pos = scan_flow_left(reqs, first, distance, init_pos);
        scan_flow_left(reqs, reqs.end() - 1, distance, last_pos);
    } else {
        last_pos = scan_flow_right(reqs, first, distance, init_pos);
        scan_flow_right(reqs, reqs.begin(), distance, last_pos);
    }
    cout << "Printing distance traversed:";
    cout << "    DISTANCE=" << distance << endl;
}
