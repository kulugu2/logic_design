#include<iostream>
#include<string>
#include<sstream>
#include<vector>
#include<cmath>
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
int blankN = 0;

vector<point> blank_list;
bool solve(int n) {
    if(n>=blankN)
        return true;
    int x=blank_list[n].i;
    int y=blank_list[n].j;
    //cout<<x<<" "<<y<<endl;
    for(int i=0;i<N;i++) {
        if(restrict[x][y][i] == 0) { // can fill
            int flag = 0;
            for(int ni=0;ni<N;ni++) {
                if(grid[ni][y] == i+1) {
                    flag = 1;
                    break;
                }
                if(grid[x][ni] == i+1) {
                    flag = 1;
                    break;
                }
            }
            
            //for(int )
            if(flag == 0) {
                grid[x][y] = i+1;
                if(solve(n+1))
                    return true;
            }
        }
    }
    return false;
}
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
    
    for(int i=0;i<N;i++) {
        for(int j=0;j<N;j++) {
            if(grid[i][j] == 0) {
                point p(i,j);
                blank_list.push_back(p);
                cout<<i<<" "<<j<<endl;
                blankN++;
            } else {
                int num = grid[i][j] - 1;
                for(int ii = 0;ii<N;ii++) {
                    restrict[ii][j][num] = 1;
                    restrict[i][ii][num] = 1;
                }
                int n = sqrt(N);
                for(int ti=0;ti<n;ti++)
                    for(int tj=0;tj<n;tj++)
                        restrict[i/n+ti][j/n+tj][num] = 1;
            }
        }
    }
    cout<<blankN<< " "<<blank_list.size()<<endl;
    cout<<solve(0)<<endl;
    cout<<N<<endl;
    for(int i=0;i<N;i++) {

        for(int j=0;j<N;j++)
            cout<<grid[i][j]<<" ";
        cout<<endl;
    }

    return 0;
}
