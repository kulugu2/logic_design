#include<iostream>
#include"parser.h"
#include<vector>
using namespace std;

class var {
public:
    int val;
    vector<int> pw;
    vector<int> nw;
};
vector<vector<int> >clauses;
int maxVarIndex;

int main(int argc, char** argv) {
    
    parse_DIMACS_CNF(clauses, maxVarIndex, argv[1]);
    cout<<maxVarIndex<<endl;
    cout<<clauses.size()<<endl;
    
    for(int i=0;i<static_cast<int>(clauses.size()); i++) {
        for(int j=0; j< static_cast<int>(clauses[i].size()); j++) {
            cout<<clauses[i][j]<<" ";
        }
        cout<<endl;
    }
      
    var emptyvar;
    emptyvar.val = 0;
    vector<var> var_list(maxVarIndex, emptyvar);
    int two_lit[clauses.size()][2];
    //vector<int[2]> tmp_two_lit(clauses.size(), {-1, -1});
    //two_lit = tmp_two_lit;
    //var_list[0].nw.push_back(0);
    
    for(unsigned int i=0;i<clauses.size(); i++) {
        if(clauses[i].size() >= 2) {
            if(clauses[i][0] > 0) {
                var_list[clauses[i][0]-1].pw.push_back(i);
            } else {
                var_list[-1*clauses[i][0]-1].nw.push_back(i);
            }
            if(clauses[i][1] > 0) {
                var_list[clauses[i][1]-1].pw.push_back(i);
            } else {
                var_list[-1*clauses[i][1]-1].nw.push_back(i);
            }
            two_lit[i][0] = 0;
            two_lit[i][1] = 1;
        } else { 
            if(clauses[i][0] > 0) {
                var_list[clauses[i][0]-1].pw.push_back(i);
                var_list[clauses[i][0]-1].val = 1;
            } else {
                var_list[-1*clauses[i][0]-1].nw.push_back(i);
                var_list[clauses[i][0]-1].val = -1;
            }
            two_lit[i][0] = 0;
            two_lit[i][1] = -1;
        }    
    }
    
/*
    for(int i=0;i<maxVarIndex;i++) {
        cout<<i<<endl;
        for(int j=0;j<var_list[i].pw.size(); j++)
            cout<<var_list[i].pw[j]<<" ";
        cout<<endl;
        for(int j=0;j<var_list[i].nw.size(); j++)
            cout<<var_list[i].nw[j]<<" ";
        cout<<endl;
    }
*/  
    return 0;
}
