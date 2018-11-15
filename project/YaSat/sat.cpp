#include<iostream>
#include"parser.h"
#include<vector>
#include<list>
#include<cstdlib>
using namespace std;

class var {
public:
    int val;
    list<int> pw;
    list<int> nw;
};
vector<vector<int> >clauses;
int maxVarIndex;
int clauses_size;
int bcp(int (*two_lit)[2] , vector<var> var_list, int update) {
    if(update > 0) {
        var_list[update-1].val = 1;
        for(list<int>::iterator it = var_list[update-1].nw.begin(); it!= var_list[update-1].nw.end(); it++) {
            int i;
            for(i=0;i<clauses[*it].size();i++) {
                if((i == two_lit[*it][0]) || (i == two_lit[*it][1])) //not watched
                    continue;
                else {
                    if(clauses[*it][i] * var_list[abs(clauses[*it][i])-1].val >=0 ) { //check not zero
                        if(clauses[*it][two_lit[*it][0]] == update)  //update two_lit[0]
                            two_lit[*it][0] = i;
                        else                                         //update two_lit[1]
                            two_lit[*it][1] = i;
                        if(clauses[*it][i] > 0) {
                            var_list[clauses[*it][i]-1].pw.push_back(*it);
                        }
                        else
                            var_list[-clauses[*it][i]-1].nw.push_back(*it);
                        it = var_list[update-1].nw.erase(it);
                        break;
                    }
                }
            }
        } 
    } 
    else {
        var_list[update-1].val = -1;
        for(list<int>::iterator it = var_list[update-1].pw.begin(); it!= var_list[update-1].pw.end(); it++) {
            int i;
            for(i=0;i<clauses[*it].size();i++) {
                if((i == two_lit[*it][0]) || (i == two_lit[*it][1])) //not watched
                    continue;
                else {
                    if(clauses[*it][i] * var_list[abs(clauses[*it][i])-1].val >=0 ) { //check not zero
                        if(clauses[*it][two_lit[*it][0]] == update)  //update two_lit[0]
                            two_lit[*it][0] = i;
                        else                                         //update two_lit[1]
                            two_lit[*it][1] = i;
                        if(clauses[*it][i] > 0) {
                            var_list[clauses[*it][i]-1].pw.push_back(*it);
                        }
                        else
                            var_list[-clauses[*it][i]-1].nw.push_back(*it);
                        it = var_list[update-1].pw.erase(it);
                        break;
                    }
                }
            }
        } 
    }
    return 1;
}

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
    clauses_size = clauses.size();
    int two_lit[clauses.size()][2];
    //vector<int[2]> tmp_two_lit(clauses.size(), {-1, -1});
    //two_lit = tmp_two_lit;
    //var_list[0].nw.push_back(0);
//////////////////////////////////////////////
///////////      init     ////////////////////
//////////////////////////////////////////////
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

    for( int i=0;i<clauses_size; i++) {
        if(two_lit[i][1] == -1) {   //  uni-clause
            if(clauses[i][0] > 0 ) {
                //var_list
            }
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
