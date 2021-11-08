#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>
#include<algorithm>

using namespace std;
#define NUM_REQS 10

/*
   Planificación del disco: FCFS (First Come, First Served), . El programa debe solicitar la posición inicial y la cola de solicitudes (10 solicitudes). Y como resultado debe
   imprimir PARA TODOS LOS ALGORITMOS de una manera clara la siguiente información:
        * La cadena con los movimientos realizados
        * La cantidad de movimientos realizados
*/
void read_uinput(int &init_pos, vector<int> &reqs){ // read user input
    cout << "Enter the initial position of the reading head: ";
    cin >> init_pos;
    for(int i=0; i<NUM_REQS; i++){
        int tmp;
        cin >> tmp;
        reqs.push_back(tmp);
    }
}

void fcfs(int init_pos, vector<int> reqs){
    cout << "\nFCFS: (init_pos=" << init_pos << ")\n";
    long distance = 0;
    int last_pos = init_pos;;
    for(size_t i=0; i<reqs.size(); i++){
        int v = reqs[i];
        cout << "    " << v << endl;
        distance += labs((long)(v-last_pos));
        last_pos = v;
    }
    cout << "Printing distance traversed:";
    cout << "    DISTANCE=" << distance << endl;
}
/*SSF (Shortest Seek First), Scan (algoritmo del ascensor), 
   C-Scan
*/
void sstf(int init_pos, vector<int> reqs){
    cout << "\nSSTF: (init_pos=" << init_pos << ")\n";
    sort(reqs.begin(), reqs.end());
    long distance = 0;
    int last_pos = init_pos;
    vector<int>::iterator closest;
    int movements =0;
    while(NUM_REQS -movements>0){
        vector<int>::iterator closest_larger = lower_bound(reqs.begin(), reqs.end(), init_pos);
        if(closest_larger == reqs.end()){
            closest = reqs.end()-1;
        }else{
            if(reqs.begin()==closest_larger){
                closest = closest_larger;
            }
            else if((closest_larger-1) != reqs.begin() && (labs((long)(*closest_larger-init_pos)) < labs((long)(*(closest_larger-1)-init_pos)))) {
                closest = closest_larger;
            }else{
                closest = (closest_larger-1);
            }
        }
        distance += labs((long)(*closest-last_pos));
        cout << "    " << *closest << endl;
        movements ++;
        last_pos = *closest;
        reqs.erase(closest);
    }
    cout << "Printing distance traversed:";
    cout << "    DISTANCE=" << distance << endl;
}
int main()
{

    int init_pos;
    vector<int> reqs;
    read_uinput(init_pos, reqs);
    sstf(init_pos, reqs);


    return 0;
}

