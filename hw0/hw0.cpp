#include<iostream>
#include<string>
#include<sstream>
#include<vector>
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
int grid[30][30];
int restrict[30][30][30] = {0};
int N;
int main() {
    string ss;
    int ni = 0;
    while(getline(cin, ss)) {
        istringstream ii(ss);
        int nj = 0;
        while(ii>>grid[ni][nj++]) {
            
        }
        N = nj-1;
        ni++;
    }
    
    vector<point> blank_list;
    for(int i=0;i<N;i++) {
        for(int j=0;j<N;j++) {
            if(grid[i][j] == 0) {
                point p(i,j);
                blank_list.push_back(p);
            } else {
                int num = grid[i][j];
                for(int ii = 0;ii<N;ii++) {
                    restrict[ii][j] = 1;
                    restrict[i][ii] = 1;
                }
            }
        }
    }
    cout<<N<<endl;
    for(int i=0;i<N;i++) {

        for(int j=0;j<N;j++)
            cout<<grid[i][j]<<" ";
        cout<<endl;
    }

    return 0;
}
