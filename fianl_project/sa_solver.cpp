#include<iostream>
#include<string>
#include<fstream>
#include<vector>
#include<algorithm>
#include<chrono>
#include<random>
#include<ctime>
#include<sstream>
#include<cmath>
using namespace std;

int main(int argc, char** argv) {
    const double e = 2.718281828;  
    fstream enc_circ;
    enc_circ.open(argv[1], ios::in);
    if(!enc_circ) {
        cout<<"read file failed"<<endl;
        return 1;
    }
    string buffer;
    string key;
    getline(enc_circ, buffer);
    key = buffer.substr(buffer.find("=")+1);
    //cout<<key<<endl;
    int N = atoi(argv[5]);
    vector<int> arr;
    for(int i=0;i<key.size();i++) {
        arr.push_back(i);
    }
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();

    shuffle(arr.begin(), arr.end(), default_random_engine(seed));

    string outputkey(key);
    for(int i=0;i<key.size()*N/100; i++) {
        outputkey[arr[i]] = 'x';
    }
    double alpha = 0.9;
    int L = 0;
    for(double T = 40; T > 0.0008; T*= alpha) {
        for(int i=0;i<10;i++) {
            clock_t t;
            stringstream s;
            s<<"./"<<argv[4]<<" "<<argv[1]<<" "<<argv[2]<<" -k"<<outputkey<<" > tmp.txt"<<endl;
            //s<<"./"<<argv[4]<<" "<<argv[1]<<" "<<argv[2]<<endl;
            cout<<s.str();
            //FILE *fd = popen(s.str().c_str(), "r");
            //pclose(fd);
            int a = rand() % key.size();
            int b = rand() % key.size();
            while((a == b) || (outputkey[a] == 'x'&& outputkey[b] == 'x')
                    || (outputkey[a] != 'x' && outputkey[b] != 'x')){
                a = rand() % key.size();
                b = rand() % key.size();
            }
            string newkey(outputkey);
            if(outputkey[a] == 'x') {
                newkey[a] = key[a];
                newkey[b] = 'x';
            }
            else {
                newkey[a] = 'x';
                newkey[b] = key[b];
            }
            t = clock();
            system(s.str().c_str());
            
            int newL  = clock()-t;
            if(newL > L || (rand() / (double)RAND_MAX) <= pow(e, -(L-newL)/T)) {
                L = newL;
                outputkey = newkey;
            }
            cout<<newL<<endl;
            //break;
        }
        
    }

    //for(int i=0;i<key.size()*N/100; i++) {
    //    outputkey[arr[i]] = 'x';
    //}
    //cout<<outputkey<<endl;
}
