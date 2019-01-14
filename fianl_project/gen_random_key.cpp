#include<iostream>
#include<string>
#include<fstream>
#include<vector>
#include<algorithm>
#include<chrono>
#include<random>
using namespace std;

int main(int argc, char** argv) {
    
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
    int N = atoi(argv[2]);
    vector<int> arr;
    for(int i=0;i<key.size();i++) {
        arr.push_back(i);
    }
    //cout<<"aa"<<endl;
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();

    //cout<<"aa"<<endl;
    shuffle(arr.begin(), arr.end(), default_random_engine(seed));

    string outputkey(key);
    //cout<<"aa"<<endl;
    
    for(int i=0;i<key.size()*N/100; i++) {
        outputkey[arr[i]] = 'x';
    }
    cout<<outputkey<<endl;
}
