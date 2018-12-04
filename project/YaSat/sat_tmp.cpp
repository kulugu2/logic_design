#include<iostream>
#include"parser.h"
#include<vector>
#include<list>
#include<stack>
#include<cstdlib>
#include<algorithm>
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
stack<stack<int> > decision_stack;
int maxVarIndex;
int clauses_size;
int bcp(vector<vector<int> > &two_lit, vector<var> &var_list, int update, vector<int> &scoreboard) {
    if(update > 0) {
        var_list[update-1].val = 1;
        for(list<int>::iterator it = var_list[update-1].pw.begin(); it != var_list[update-1].pw.end(); it++) {
            int a = clauses[*it][two_lit[*it][0]];
            int b = clauses[*it][two_lit[*it][1]];
            if(a*var_list[abs(a)-1].val>0 && b*var_list[abs(b)-1].val>0) //already sat
                continue;
            for(int i=0; i<clauses[*it].size();i++) {
                if(clauses[*it][i]>0) {
                    scoreboard[2*(clauses[*it][i]-1)]--;
                }
                else {
                    scoreboard[2*(-1*clauses[*it][i]-1)+1]--;
                }
            }
        }
        for(list<int>::iterator it = var_list[update-1].nw.begin(); it != var_list[update-1].nw.end(); it++) {
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
        for(list<int>::iterator it = var_list[abs(update)-1].nw.begin(); it != var_list[abs(update)-1].nw.end(); it++) {
            int a = clauses[*it][two_lit[*it][0]];
            int b = clauses[*it][two_lit[*it][1]];
            if(a*var_list[abs(a)-1].val>0 && b*var_list[abs(b)-1].val>0) //already sat
                continue;
            for(int i=0; i<clauses[*it].size();i++) {
                if(clauses[*it][i]>0) {
                    scoreboard[2*(clauses[*it][i]-1)]--;
                }
                else {
                    scoreboard[2*(-1*clauses[*it][i]-1)+1]--;
                }
            }
        }
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

int check_uni(vector<vector<int> > &two_lit, vector<var>& var_list) {
    for(int i=0; i<clauses_size; i++) {
        if(two_lit[i][1] == -1)
            continue;
        int a = clauses[i][two_lit[i][0]];
        int b = clauses[i][two_lit[i][1]];
        int vala = var_list[abs(a)-1].val;
        int valb = var_list[abs(b)-1].val;
        //if((a * var_list[abs(a)-1].val < 0 && var_list[abs(b)-1].val == 0) ||
        //   (b * var_list[abs(b)-1].val < 0 && var_list[abs(a)-1].val == 0)  )
        if(a*vala<0 && valb == 0 || b*valb<0 && vala == 0)
            return i;
    }
    return -1;
}

int DPLL(vector<vector<int> > &two_lit, vector<var> &var_list, vector<int> scoreboard) {
    int t;
    stack<int> decision;
    while((t = check_uni(two_lit, var_list)) != -1) {
        //cout<<"imply"<<t<<endl;
      
        if(var_list[abs(clauses[t][two_lit[t][0]])-1].val == 0){ //not assign
            decision_stack.top().push(abs(clauses[t][two_lit[t][0]]));
            bcp(two_lit, var_list, clauses[t][two_lit[t][0]], scoreboard);
        }
        else {
            decision_stack.top().push(abs(clauses[t][two_lit[t][1]]));
            bcp(two_lit, var_list, clauses[t][two_lit[t][1]], scoreboard);
        }
    }
    //   check sat
    int i;
    int flag_sat = 1;
    for(i=0;i<clauses_size; i++) {
        if(two_lit[i][1] == -1)
            continue;
        int a = clauses[i][two_lit[i][0]];
        int b = clauses[i][two_lit[i][1]];
        int vala = var_list[abs(a)-1].val;
        int valb = var_list[abs(b)-1].val;
        //if(a * var_list[abs(a)-1].val <= 0 && b * var_list[abs(b)-1].val <= 0) {
        if(a * vala <= 0 && b * valb <= 0) {    
            flag_sat = 0;
            if(vala != 0 && valb != 0) {
                while(!decision_stack.top().empty()) {
                    var_list[decision_stack.top().top()-1].val = 0;
                    decision_stack.top().pop();
                }
                decision_stack.pop();
                return 0;
            }
        }
            //break;
    }
    //if(i == clauses_size) { //SAT
    if(flag_sat == 1) {
        ans = var_list;
        return 1;
    }
    //    check unsat
    /*
    for(i=0;i<clauses_size; i++) {
        if(two_lit[i][1] == -1)
            continue;
        int a = clauses[i][two_lit[i][0]];
        int b = clauses[i][two_lit[i][1]];
        if(a * var_list[abs(a)-1].val < 0 && b * var_list[abs(b)-1].val < 0){
            //cout<<"back"<<i<<" "<<clauses[i][two_lit[i][0]]<<" "<<clauses[i][two_lit[i][1]]<<endl;
            while(!decision_stack.top().empty()) {
                var_list[decision_stack.top().top()-1].val = 0;
                decision_stack.top().pop();
            }
            decision_stack.pop();
            return 0;
        }
    }
    */
    //    make decision
    /*for(i = 0; i < maxVarIndex; i++) {
        if(var_list[i].val == 0)
            break;
    }*/
    vector<int> dlis_score(scoreboard);
    for(int i=0;i<maxVarIndex;i++) {
        if(var_list[i].val != 0) {
            dlis_score[i*2] = 0;
            dlis_score[i*2+1] = 0;
        }
    }
    vector<int>::iterator max_ele;
    max_ele = max_element(dlis_score.begin(), dlis_score.end());
    int max_index = distance(dlis_score.begin(), max_ele);
    while(var_list[max_index/2].val != 0) {
        cout<<"error"<<endl;
    }
    decision.push(max_index/2+1);
    decision_stack.push(decision);
    vector<int> new_scoreboard(scoreboard);
    if(max_index % 2 == 0) {
        bcp(two_lit, var_list, (max_index/2+1), new_scoreboard);
        //cout<<"dpll "<< max_index/2+1<<endl;
    }
    else {
        bcp(two_lit, var_list, -(max_index/2+1), new_scoreboard);
        //cout<<"dpll "<<-(max_index/2+1)<<endl;
    }
    
    if(DPLL(two_lit, var_list, new_scoreboard))
        return 1;
    else {
        decision_stack.push(decision);
        new_scoreboard = scoreboard;
        if(max_index % 2 == 0) {
            bcp(two_lit, var_list, -(max_index/2+1), new_scoreboard);
          //  cout<<"dpll "<<-(max_index/2+1)<<endl;
        }
        else {
            bcp(two_lit, var_list, (max_index/2+1), new_scoreboard);
            //cout<<"dpll "<<(max_index/2+1)<<endl;
        }
        //cout<<"d "<<(i+1)<<endl;
        
        if(DPLL(two_lit, var_list, new_scoreboard)) {
            return 1;
        } 
        else {
            while(!decision_stack.top().empty()) {
                var_list[decision_stack.top().top()-1].val = 0;
                decision_stack.top().pop();
            }
            decision_stack.pop();
            return 0;
        }
    }
}
int main(int argc, char** argv) {
    
    parse_DIMACS_CNF(clauses, maxVarIndex, argv[1]);
    


    var emptyvar;
    emptyvar.val = 0;
    vector<var> var_list(maxVarIndex, emptyvar);
    clauses_size = clauses.size();
    vector<int> init_two(2, 0);
    vector<vector<int> > two_lit(clauses_size, init_two);
    vector<int> scoreboard(maxVarIndex*2, 0);
    
    //int two_lit[clauses.size()][2];
    //vector<int[2]> tmp_two_lit(clauses.size(), {-1, -1});
    //two_lit = tmp_two_lit;
    //var_list[0].nw.push_back(0);
//////////////////////////////////////////////
///////////      init     ////////////////////
//////////////////////////////////////////////
    for( int i=0;i<clauses.size(); i++) {
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
                var_list[-1*clauses[i][0]-1].val = -1;
            }
            two_lit[i][0] = 0;
            two_lit[i][1] = -1;
        } 
        for(int j=0;j<clauses[i].size();j++) {
            if(clauses[i][j] > 0) {
                scoreboard[(clauses[i][j]-1)*2]++;
            }
            else {
                scoreboard[(-1*clauses[i][j]-1)*2+1]++;
            }
        }
    }
    for(int i=0;i<clauses_size;i++) {
        if(two_lit[i][1] == -1) {   //  uni-clause
            //cout<<"uni "<<i<<endl;
            //cout<<"bcp "<<clauses[i][0]<<endl;
            bcp(two_lit, var_list, clauses[i][0], scoreboard);
        }
    }
    //cout<<"GO"<<endl;
    stack<int> init;
    decision_stack.push(init);
    int sat = DPLL(two_lit, var_list, scoreboard); 
    if(sat) {
        cout<<"s SATISFIABLE"<<endl;
    }
    else {
        cout<<"s UNSATISFIABLE"<<endl;
    }
    if(sat){
        cout<<"v ";
        for(int i=0;i<maxVarIndex;i++){
            if((i+1)*ans[i].val != 0)
                cout<<(i+1)*ans[i].val<<" ";
            else
                cout<<(i+1)*1<<" ";
        }
        cout<<"0"<<endl;
    }
    
    //cout<<"two lit"<<endl;
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
