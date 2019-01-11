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
vector<vector<int> > decision_vector;
stack<stack<int> > decision_stack;
int maxVarIndex;
int clauses_size;
int input_clauses_size;
int return_level;
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
bool abs_cmp(int i, int j) {
    return (abs(i)<abs(j));
}
vector<int> resolve(vector<int> conflict_clause, int clause_num, int p) {
    //cout<<"resolve"<<endl;
    //cout<<clause_num<<endl;
    vector<int> tmp(clauses[clause_num]);
    sort(conflict_clause.begin(), conflict_clause.end(), abs_cmp);
    sort(tmp.begin(), tmp.end(), abs_cmp);

    vector<int> result;
    vector<int>::iterator it1 = conflict_clause.begin();
    vector<int>::iterator it2 = tmp.begin();
    while(it1 != conflict_clause.end() && it2 != tmp.end()) {
        if(abs(*it1) == p) {
            it1++;
            continue;
        }
        if(abs(*it2) == p) {
            it2++;
            continue;
        }
        if(abs(*it1) < abs(*it2)) {
            result.push_back(*it1);
            it1++;
        }
        else if(abs(*it1) > abs(*it2)){
            result.push_back(*it2);
            it2++;
        }
        else {
            result.push_back(*it1);
            it1++;
            it2++;
        }
    }
    while(it2 != tmp.end()) {
        if(abs(*it2) != p)
            result.push_back(*it2);
        it2++;
    }
    while(it1 != conflict_clause.end()) {
        if(abs(*it1) != p)
            result.push_back(*it1);
        it1++;
    }
    //for(int i=0;i<result.size();i++) {
    //    cout<<result[i]<<" ";
    //}
    //cout<<endl;
    return result;
    
    
}
vector<int> conflict_learning(stack<int> decision, int clause_num) {
    vector<int> dv;
    //cout<<"c1"<<endl;
    while(!decision.empty()) {
        dv.push_back(decision.top());
        //cout<<decision.top()<<" ";
        decision.pop();
    }
    //cout<<endl;
    vector<int> conflict_clause(clauses[clause_num]);
    while(1) {

        int count = 0;
        int most_recent = -1;
        for(int i=0;i<dv.size();i++) {
            for(int j=0;j<conflict_clause.size();j++) {
                if(dv[i] == abs(conflict_clause[j])) {
                    count++;
                    if(most_recent == -1)
                        most_recent = dv[i];
                    break;
                }
            }
        }
        if(count <= 1)
            break;
        conflict_clause = resolve(conflict_clause, decision_vector[most_recent-1][1], most_recent);
    }
    //for(int i=0;i<conflict_clause.size();i++) {
    //    cout<<conflict_clause[i]<<" ";
    //}
    //cout<<endl;
    return conflict_clause;

}
void add_clauses(vector<int> learned, vector<vector<int> > &two_lit, vector<var> &var_list, int watch_literal) {

    int tmpi;
    for(tmpi = input_clauses_size; tmpi < clauses_size; tmpi++) {
        if(clauses[tmpi] == learned)
            break;
    }
    if(tmpi == clauses_size) {
        clauses.push_back(learned);
        clauses_size++;
        int two0, two1;
        if(learned.size() == 1) {
            two0 = 0;
            two1 = -1;
        }
        else {
            two0 = watch_literal;
            int second_big = 0;
            for(int i=0;i<learned.size();i++) {
                if(i == two0)
                    continue;
                else if (decision_vector[abs(learned[i])-1][0] >= second_big) {
                    second_big = decision_vector[abs(learned[i])-1][0];
                    two1 = i;
                }
                    
            }
            if(clauses[clauses_size-1][two0] > 0) {
                var_list[abs(clauses[clauses_size-1][two0])-1].pw.push_back(clauses_size-1);
            }
            else {
                var_list[abs(clauses[clauses_size-1][two0])-1].nw.push_back(clauses_size-1);                
            }
            if(clauses[clauses_size-1][two1] > 0) {
                var_list[abs(clauses[clauses_size-1][two1])-1].pw.push_back(clauses_size-1);
            }
            else {
                var_list[abs(clauses[clauses_size-1][two1])-1].nw.push_back(clauses_size-1);                
            }
        }
        vector<int> new_two;
        new_two.push_back(two0);
        new_two.push_back(two1);
        two_lit.push_back(new_two);
        
    }
    return;
}
int DPLL(vector<vector<int> > &two_lit, vector<var> &var_list, vector<int> scoreboard, int level) {

    int t;
    //stack<int> decision;
label_bcp:
    while((t = check_uni(two_lit, var_list)) != -1) {
        //cout<<"imply"<<t<<endl;
        
        if(var_list[abs(clauses[t][two_lit[t][0]])-1].val == 0){ //not assign
            decision_stack.top().push(abs(clauses[t][two_lit[t][0]]));
            decision_vector[abs(clauses[t][two_lit[t][0]])-1][0] = level;
            decision_vector[abs(clauses[t][two_lit[t][0]])-1][1] = t;

            bcp(two_lit, var_list, clauses[t][two_lit[t][0]], scoreboard);
            //cout<<"bcp "<<clauses[t][two_lit[t][0]]<<" "<<t<<" level "<<level<<endl;
        }
        else {
            decision_stack.top().push(abs(clauses[t][two_lit[t][1]]));
            decision_vector[abs(clauses[t][two_lit[t][1]])-1][0] = level;
            decision_vector[abs(clauses[t][two_lit[t][1]])-1][1] = t;
            bcp(two_lit, var_list, clauses[t][two_lit[t][1]], scoreboard);
            //cout<<"bcp "<<clauses[t][two_lit[t][1]]<<" "<<t<<" level "<<level<<endl;
        }
    }
    //   check sat
    static int i;
    static int flag_sat;
    flag_sat = 1;
    for(i=0;i<clauses_size; i++) {
        if(two_lit[i][1] == -1) {
            if(clauses[i][0]*var_list[abs(clauses[i][0])-1].val < 0)
                return 0;
            else
                continue;
        }
        int a = clauses[i][two_lit[i][0]];
        int b = clauses[i][two_lit[i][1]];
        int vala = var_list[abs(a)-1].val;
        int valb = var_list[abs(b)-1].val;
        //if(a * var_list[abs(a)-1].val <= 0 && b * var_list[abs(b)-1].val <= 0) {
        if(a * vala <= 0 && b * valb <= 0) {    //check unsat
            flag_sat = 0;
            if(vala != 0 && valb != 0) {
                //cout<<"conflict "<<i<<endl;
                vector<int> learned(conflict_learning(decision_stack.top(), i));
                int watch_lit;
                int second_big = 0;
                for(int tmpi = 0;tmpi<learned.size();tmpi++) {
                    if(decision_vector[abs(learned[tmpi])-1][0] == level)
                        watch_lit = tmpi;
                    else if(decision_vector[abs(learned[tmpi])-1][0] > second_big)
                        second_big = decision_vector[abs(learned[tmpi])-1][0]; 
                }
                add_clauses(learned, two_lit, var_list, watch_lit);

                //cout<<"push"<<endl;
                while(!decision_stack.top().empty()) {
                    var_list[decision_stack.top().top()-1].val = 0;
                    //cout<<"pop "<<decision_stack.top().top()<<endl;
                    decision_stack.top().pop();
                }
                decision_stack.pop();
                return_level = second_big;
                //cout<<"return c"<<endl;
                
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
    static vector<int> dlis_score;
    dlis_score = scoreboard;
    for(int i=0;i<maxVarIndex;i++) {
        if(var_list[i].val != 0) {
            dlis_score[i*2] = 0;
            dlis_score[i*2+1] = 0;
        }
    }
    static vector<int>::iterator max_ele;
    max_ele = max_element(dlis_score.begin(), dlis_score.end());
    int max_index = distance(dlis_score.begin(), max_ele);
    while(var_list[max_index/2].val != 0) {
        cout<<"error"<<endl;
    }
    stack<int> decision;
    decision.push(max_index/2+1);
    decision_stack.push(decision);
    //cout<<"push "<<max_index/2+1<<endl;
    decision_vector[max_index/2][0] = level+1;
    decision_vector[max_index/2][1] = -1;
    static int old_clauses_size; 
    old_clauses_size = clauses_size;
    static vector<int> new_scoreboard;
    new_scoreboard = scoreboard;
    if(max_index % 2 == 0) {
        bcp(two_lit, var_list, (max_index/2+1), new_scoreboard);
        //cout<<"dpll "<< max_index/2+1<<endl;
    }
    else {
        bcp(two_lit, var_list, -(max_index/2+1), new_scoreboard);
        //cout<<"dpll "<<-(max_index/2+1)<<endl;
    }
    
    if(DPLL(two_lit, var_list, new_scoreboard, level+1))
        return 1;
    else {
        if(return_level != level) {
            while(!decision_stack.top().empty()) {
                var_list[decision_stack.top().top()-1].val = 0;
                //cout<<"pop "<<decision_stack.top().top()<<endl;
                decision_stack.top().pop();
            }
            decision_stack.pop();
            //cout<<"return non_cho "<<return_level<<endl;
            return 0;
        }
        else {
            if(clauses_size != old_clauses_size) {
                for(int i=old_clauses_size;i<clauses_size;i++) {
                    for(int j=0;j<clauses[i].size();j++) {
                        if(clauses[i][j] > 0) {
                            scoreboard[2*(abs(clauses[i][j])-1)]++;
                        }
                        else {
                            scoreboard[2*(abs(clauses[i][j])-1)+1]++;
                        }
                    }
                }
                //cout<<"goto"<<endl;
                goto label_bcp;
            }
            //cout<<"learn nothing"<<endl;
        }
        stack<int> decision;
        decision.push(max_index/2+1);
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
        //cout<<"pass"<<endl;
        if(DPLL(two_lit, var_list, new_scoreboard, level+1)) {
            return 1;
        } 
        else {
            if(return_level != level) {
                while(!decision_stack.top().empty()) {
                    var_list[decision_stack.top().top()-1].val = 0;
                    //cout<<"pop "<<decision_stack.top().top()<<endl;
                    decision_stack.top().pop();
                }
                decision_stack.pop();
                //cout<<"return non_cho "<<return_level<<endl;
                return 0;
            }
            else {
                if(clauses_size != old_clauses_size) {
                    for(int i=old_clauses_size;i<clauses_size;i++) {
                        for(int j=0;j<clauses[i].size();j++) {
                            if(clauses[i][j] > 0) {
                                scoreboard[2*(abs(clauses[i][j])-1)]++;
                            }
                            else {
                                scoreboard[2*(abs(clauses[i][j])-1)+1]++;
                            }
                        }
                    }
                    //cout<<"goto"<<endl;
                    goto label_bcp;
                }
            }
            return_level = level-1;
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
    input_clauses_size = clauses_size;
    vector<int> init_two(2, 0);
    vector<vector<int> > two_lit(clauses_size, init_two);
    for(int i=0;i<maxVarIndex;i++) {
        vector<int> tmp(2, 0);
        decision_vector.push_back(tmp);
    }
    vector<int> scoreboard(maxVarIndex*2, 0);
    
    stack<int> init;
    decision_stack.push(init);
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
            decision_stack.top().push(abs(clauses[i][two_lit[i][0]]));
            decision_vector[abs(clauses[i][two_lit[i][0]])-1][0] = 0;
            decision_vector[abs(clauses[i][two_lit[i][0]])-1][1] = i;
            bcp(two_lit, var_list, clauses[i][0], scoreboard);
        }
    }
    //cout<<"GO"<<endl;
    cout<<clauses_size<<endl;
    int sat = DPLL(two_lit, var_list, scoreboard, 0); 
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
    
    cout<<clauses_size<<endl;
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
