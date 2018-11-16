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
    var() {
        val = 0;
    }
    var(const var &p) {
        val = p.val;
        pw = p.pw;
        nw = p.nw;
    }
};
vector<vector<int> >clauses;
vector<var> ans;
int maxVarIndex;
int clauses_size;
int bcp(int (*two_lit)[2] , vector<var>& var_list, int update) {
    cout<<"bcp "<<update<<endl;
    if(update > 0) {
        var_list[update-1].val = 1;
        for(list<int>::iterator it = var_list[update-1].nw.begin(); it!= var_list[update-1].nw.end(); it++) {
            int i;
            if(two_lit[*it][1] == -1)
                continue;
            for(i=0;i<clauses[*it].size();i++) {
                if((i == two_lit[*it][0]) || (i == two_lit[*it][1])) //not watched
                    continue;
                else {
                    if(clauses[*it][i] * var_list[abs(clauses[*it][i])-1].val >=0 ) { //check not zero
                        if(clauses[*it][two_lit[*it][0]] == -update)  //update two_lit[0]
                            two_lit[*it][0] = i;
                        else                                         //update two_lit[1]
                            two_lit[*it][1] = i;
                        if(clauses[*it][i] > 0) {
                            var_list[clauses[*it][i]-1].pw.push_back(*it);
                        }
                        else
                            var_list[-clauses[*it][i]-1].nw.push_back(*it);
                        it = var_list[update-1].nw.erase(it);
                        it--;
                        break;
                    }
                }
            }
        } 
    } 
    else {  //update < 0
        var_list[abs(update)-1].val = -1;
        for(list<int>::iterator it = var_list[abs(update)-1].pw.begin(); it!= var_list[abs(update)-1].pw.end(); it++) {
            int i;
            if(two_lit[*it][1] == -1)
                continue;
            for(i=0;i<clauses[*it].size();i++) {
                if((i == two_lit[*it][0]) || (i == two_lit[*it][1])) //not watched
                    continue;
                else {
                    if(clauses[*it][i] * var_list[abs(clauses[*it][i])-1].val >=0 ) { //check not zero
                        if(clauses[*it][two_lit[*it][0]] == -update)  //update two_lit[0]
                            two_lit[*it][0] = i;
                        else                                         //update two_lit[1]
                            two_lit[*it][1] = i;
                        if(clauses[*it][i] > 0) {
                            var_list[clauses[*it][i]-1].pw.push_back(*it);
                        }
                        else
                            var_list[-clauses[*it][i]-1].nw.push_back(*it);
                        it = var_list[abs(update)-1].pw.erase(it);
                        it--;
                        break;
                    }
                }
            }
        } 
    }
    return 1;
}

int check_uni(int (*two_lit)[2], vector<var>& var_list) {
    for(int i=0; i<clauses_size; i++) {
        if(two_lit[i][1] == -1)
            continue;
        int a = clauses[i][two_lit[i][0]];
        int b = clauses[i][two_lit[i][1]];
        if((a * var_list[abs(a)-1].val < 0 && var_list[abs(b)-1].val == 0) ||
           (b * var_list[abs(b)-1].val < 0 && var_list[abs(a)-1].val == 0)  )
            return i;
    }
    return -1;
}

int DPLL(int (*two_lit)[2], vector<var>& var_list) {
    int t;
    while((t = check_uni(two_lit, var_list)) != -1) {
        //cout<<"imply"<<t<<endl;
        if(var_list[abs(clauses[t][two_lit[t][0]])-1].val == 0) //not assign
            bcp(two_lit, var_list, clauses[t][two_lit[t][0]]);
        else
            bcp(two_lit, var_list, clauses[t][two_lit[t][1]]);
    }
    //   check sat
    int i;
    for(i =0;i<clauses_size; i++) {
        int a = clauses[i][two_lit[i][0]];
        int b = clauses[i][two_lit[i][1]];
        if(a * var_list[abs(a)-1].val <= 0 && b * var_list[abs(b)-1].val <= 0)
            break;
    }
    if(i == clauses_size) { //SAT
        ans = var_list;
        return 1;
    }
    //    check unsat
    for(i =0;i<clauses_size; i++) {
        int a = clauses[i][two_lit[i][0]];
        int b = clauses[i][two_lit[i][1]];
        if(a * var_list[abs(a)-1].val < 0 && b * var_list[abs(b)-1].val < 0){
            //cout<<"back"<<endl;
            return 0;
        }
    }
    //    make decision
    vector<var> new_var_list(var_list);
    for(i = 0; i < maxVarIndex; i++) {
        if(var_list[i].val == 0)
            break;
    }
    cout<<"d "<<i+1<<endl;
    bcp(two_lit, new_var_list, i+1);
    //cout<<var_list[0].val<<" "<<new_var_list[0].val<<endl;
    //cout<<two_lit[5][0]<< " "<<two_lit[5][1]<<endl;
    if(DPLL(two_lit, new_var_list))
        return 1;
    else {
        new_var_list = var_list;
        bcp(two_lit, new_var_list, -(i+1));
        cout<<"d "<<-(i+1)<<endl;
        return DPLL(two_lit, new_var_list);
    }
}
int main(int argc, char** argv) {
    
    parse_DIMACS_CNF(clauses, maxVarIndex, argv[1]);
    cout<<maxVarIndex<<endl;
    cout<<clauses.size()<<endl;
/*    
    for(int i=0;i<static_cast<int>(clauses.size()); i++) {
        for(int j=0; j< static_cast<int>(clauses[i].size()); j++) {
            cout<<clauses[i][j]<<" ";
        }
        cout<<endl;
    }
*/
      
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
            bcp(&two_lit[0], var_list, clauses[i][0]);
        }
    }

    
    //bcp(&two_lit[0], var_list, 5);
    //bcp(&two_lit[0], var_list, 3);
    cout<<"GO"<<endl;
    int sat = DPLL(&two_lit[0], var_list); 
    cout<<sat<<endl;
    if(sat){
        for(int i=0;i<maxVarIndex;i++){
            if((i+1)*ans[i].val > 0)
                cout<<(i+1)*ans[i].val<<" ";
        }
        cout<<endl;
    }
    cout<<"two lit"<<endl;
    /*
    for(int i=0;i<clauses_size;i++)
        cout<<two_lit[i][0]<< "  "<<two_lit[i][1]<<endl;
    cout<<"pwnw"<<endl<<endl;
    for(int i=0;i<maxVarIndex;i++) {
        cout<<i<<">>>"<<var_list[i].val<<endl;
        for(list<int>::iterator j=var_list[i].pw.begin();j!=var_list[i].pw.end(); j++)
            cout<<*j<<" ";
        cout<<endl;
        for(list<int>::iterator j=var_list[i].nw.begin();j!=var_list[i].nw.end(); j++)
            cout<<*j<<" ";
        cout<<endl;
    }
    */
 /* 
    vector<var> tmp(var_list);
    tmp[0].pw.push_back(20);
    
    for(int i=0;i<maxVarIndex;i++) {
        cout<<i<<">>>"<<endl;
        for(list<int>::iterator j=var_list[i].pw.begin();j!=var_list[i].pw.end(); j++)
            cout<<*j<<" ";
        cout<<endl;
        for(list<int>::iterator j=var_list[i].nw.begin();j!=var_list[i].nw.end(); j++)
            cout<<*j<<" ";
        cout<<endl;
    }
    for(int i=0;i<maxVarIndex;i++) {
        cout<<i<<">>>"<<endl;
        for(list<int>::iterator j=tmp[i].pw.begin();j!=tmp[i].pw.end(); j++)
            cout<<*j<<" ";
        cout<<endl;
        for(list<int>::iterator j = tmp[i].nw.begin();j!=tmp[i].nw.end(); j++)
            cout<<*j<<" ";
        cout<<endl;
    }
 */
    return 0;
}
