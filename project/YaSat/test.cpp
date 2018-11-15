#include<iostream>

using namespace std;

void func(int (*a)[2], int n) {
    for(int i=0;i<n;i++) {
        cout<<a[i][0]<<" "<<a[i][1]<<endl;
    }
}

int main() {
    int a[4][2] = { {0,1}, {2,3}, {4,5}, {6,7}};
    func(&a[0] , 4);
    return 0;
}
