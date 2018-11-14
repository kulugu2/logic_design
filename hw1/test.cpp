#include<iostream>
#include<string>
#include<sstream>
#include<vector>
#include<cmath>
#include<fstream>
#include<iomanip>
using namespace std;

class point {
public:
    int i;
    int j;
    point(int a, int b){
        i = a;
        j = b;
    }
};
int main() {
    cout<<"hee"<<endl;
    int a = 2;
    int b = 3;
    int c = -4;
    //cin>>a;
    cout<<b<<setw(2)<<setfill('0')<<a<<setw(2)<<b<<endl;
    return 0;
}
