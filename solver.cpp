#include<iostream>
#include<string>
#include<map>
#include<vector>
#include<list>
#include<queue>
#include<fstream>
#include<algorithm>
#include<sstream>
#include<cmath>
using namespace std;

enum gate_type{INPUT, OUTPUT, BUF, NOT, OR, NOR, AND, NAND, XOR, XNOR};

class gate{
public:
    string name;
    gate_type type;
    int key_input;
    int mutabl; //-1: others, 0: non_mutable, 1:mutable, 2:input, 3:key_input
    int represent;
    gate() {
        
    }
    gate(string n, gate_type g) {
        name = n;
        type = g;
        mutabl = -1;
        if(type == 0 && name.find("keyinput") != string::npos)
            key_input = 1;
        else
            key_input = 0;
        int represent = -1;
    }
};
string key;
map<string, int> gate_index;
vector<list<int> > adjacency_list;
vector<gate> gate_list;
map<string, gate_type> gate_type_map = {{"INPUT", INPUT}, {"OUTPUT", OUTPUT}, {"BUF", BUF},
                                        {"NOT", NOT}, {"OR", OR}, {"NOR", NOR}, {"AND", AND},
                                        {"NAND", NAND}, {"XOR", XOR}, {"XNOR", XNOR}};

int gate_count = 0;

void create_gate(gate g) {

    list<int> empty_list;
    adjacency_list.push_back(empty_list);
    gate_list.push_back(g);
    gate_index[g.name] = gate_count;
    gate_count++;
    return;
}
bool sortbysec(const pair<int, int> &a, const pair<int ,int> &b) {
    return (a.second>b.second);
}

bool sortbysecb(const pair<int, double> &a, const pair<int ,double> &b) {
    return (a.second>b.second);
}
int main(int argc, char** argv) {
    
    fstream enc_circ;
    enc_circ.open(argv[1], ios::in);
    if(!enc_circ) {
        cout<<"read file failed"<<endl;
            return 1;
    }
    string buffer;

    getline(enc_circ, buffer);
    key = buffer.substr(buffer.find("=")+1);
    //cout<<key<<endl;
    
    while(getline(enc_circ, buffer)) {
        if(buffer.length() == 0)
            continue;
        size_t found = buffer.find("=");
        if(found == string::npos) {   //INPUT and OUTPUT
            
            if(buffer.find("INPUT") != string::npos) {  //INPUT
                string name(buffer.begin()+6, buffer.end()-1);
                gate new_gate(name, INPUT);
                create_gate(new_gate);
                
            }
            else if(buffer.find("OUTPUT") != string::npos) {   //OUTPUT
                string name(buffer.begin()+7, buffer.end()-1);
                gate new_gate(name, OUTPUT);
                create_gate(new_gate);
            }
            else {
                cout<<"intput error!!"<<endl;
            }
        }
        else {  //other gate
            string name(buffer, 0, found-1);
            map<string, int>::iterator it;
            it = gate_index.find(name);
            if(it != gate_index.end()) {
                continue;
            }
            string gate_t(buffer.begin()+found+2, buffer.begin()+buffer.find("("));
            transform(gate_t.begin(), gate_t.end(), gate_t.begin(), ::toupper);
            //cout<<gate_t<<endl;
            gate new_gate(name, gate_type_map[gate_t]);
            create_gate(new_gate);
     
        }
    }
    enc_circ.close();
    
    enc_circ.open(argv[1], ios::in);
    if(!enc_circ) {
        cout<<"read file failed"<<endl;
        return 1;
    }

    getline(enc_circ, buffer);
    //key = buffer.substr(buffer.find("=")+1);

    while(getline(enc_circ, buffer)) {
        if(buffer.length() == 0)
            continue;
        size_t found = buffer.find("=");
        if(found == string::npos) {   //INPUT and OUTPUT
           
            continue;
            if(buffer.find("INPUT") != string::npos) {  //INPUT
                string name(buffer.begin()+6, buffer.end()-1);
                //gate new_gate(name, INPUT);
                //cout<<name<<" ";
                
            }
            else if(buffer.find("OUTPUT") != string::npos) {   //OUTPUT
                string name(buffer.begin()+7, buffer.end()-1);
                //gate new_gate(name, OUTPUT);
                //cout<<name<<" ";
            }
            else {
                cout<<"intput error!!"<<endl;
            }
        }
        else {  //other gate
            string name(buffer, 0, found-1);
            string gate_t(buffer.begin()+found+2, buffer.begin()+buffer.find("("));
            transform(gate_t.begin(), gate_t.end(), gate_t.begin(), ::toupper);
            //cout<<name<<" ";
            //if(gate_t == "NOT" || gate_t == "BUF")
            //    continue;
            int this_gate_id = gate_index[name];

            found = buffer.find("(");
            size_t found2 = found;
            while((found2 = buffer.find(",", found+1)) != string::npos) {
                string input_gate(buffer.begin()+found+1, buffer.begin()+found2);
                if(input_gate[0] == ' ')
                    input_gate.erase(0,1);
                adjacency_list[gate_index[input_gate]].push_back(this_gate_id);
                //cout<<input_gate<<endl;
                //cout<<input_gate<<" ";
                found = found2;
            }
            
            
            string input_gate(buffer.begin()+found+1, buffer.end()-1);
            if(input_gate[0] == ' ')
                input_gate.erase(0,1);
            adjacency_list[gate_index[input_gate]].push_back(this_gate_id);
            //cout<<input_gate<<endl;
            
        }
    }
    enc_circ.close();

    //////////////       find mutable  ///////////////
    for(int i=0; i<gate_count;i++) {
        if(gate_list[i].type != INPUT)
             continue;
        if(gate_list[i].key_input == 0) {
            //cout<<i<<endl;
            int *color = new int[gate_count]();
            queue<int> q;
            color[i] = 1;
            q.push(i);
            gate_list[i].mutabl = 2;
            while(!q.empty()) {
                int u = q.front();
                for(list<int>::iterator it = adjacency_list[u].begin();
                        it != adjacency_list[u].end(); it++) {
                    if(color[*it] == 0) {
                        if(gate_list[*it].type == BUF || gate_list[*it].type == NOT) {
                            color[*it] = 1;
                            q.push(*it);
                            gate_list[*it].mutabl = 2;
                        }
                        else {
                            gate_list[*it].mutabl = 1;
                        }
                    }
                }
                color[u] = 2;
                q.pop();
            }
            //cout<<endl;
            delete(color);
            //-1: others, 0: non_mutable, 1:mutable, 2:input, 3:key_input
            
        }
    }
    //////////////       find nonmutable  ///////////////
    for(int i=0; i<gate_count;i++) {
        if(gate_list[i].type != INPUT)
             continue;
        if(gate_list[i].key_input == 1   ) {
            //cout<<i<<endl;
            int *color = new int[gate_count]();
            queue<int> q;
            color[i] = 1;
            q.push(i);
            gate_list[i].mutabl = 3;
            while(!q.empty()) {
                int u = q.front();
                for(list<int>::iterator it = adjacency_list[u].begin();
                        it != adjacency_list[u].end(); it++) {
                    if(color[*it] == 0) {
                        if(gate_list[*it].type == BUF || gate_list[*it].type == NOT) {
                            color[*it] = 1;
                            q.push(*it);
                            gate_list[*it].mutabl = 3;
                        }
                        else {
                            gate_list[*it].mutabl = 0;
                            gate_list[*it].represent = i;
                        }
                    }
                }
                color[u] = 2;
                q.pop();
            }
            //cout<<endl;
            delete(color);
            //-1: others, 0: non_mutable, 1:mutable, 2:input, 3:key_input
            
        }
    }
    
    //////////////   count ////////////////////
    vector<pair<int, int> > score;
    vector<pair<int, int> > minus_score;
    vector<pair<int, int> > pass_through;

    for(int i=0;i<gate_count;i++) {
        score.push_back(make_pair(i, 0));
    }
    for(int i=0;i<gate_count;i++) {
        minus_score.push_back(make_pair(i, 0));
    }
    for(int i=0;i<gate_count;i++) {
        pass_through.push_back(make_pair(i, 0));
    }

    for(int i=0; i<gate_count; i++) {
        if(gate_list[i].type == INPUT && gate_list[i].key_input == 1) {
            int pos_s = 0;
            int neg_s = 0;
            int *color = new int[gate_count]();
            queue<int> q;
            color[i] = 1;
            q.push(i);
            while(!q.empty()) {
                int u = q.front();
                for(list<int>::iterator it = adjacency_list[u].begin();
                        it != adjacency_list[u].end(); it++) {
                    if(color[*it] == 0) {
                        if(gate_list[*it].mutabl == 1) {
                            minus_score[i].second++;
                            color[*it] = 2;
                        }
                        else if(gate_list[*it].mutabl == 0) {
                            score[i].second++;
                            color[*it] = 1;
                            q.push(*it);
                            pass_through[gate_list[*it].represent].second++;
                        }
                        else {
                            color[*it] = 1;
                            q.push(*it);
                        }
                    }
          
                }
                color[u] = 2;
                q.pop();
            }
            
            //cout<<endl;
            delete(color);
            //-1: others, 0: non_mutable, 1:mutable, 2:input, 3:key_input
            
        }
    }
    //////////////////////////////////////////////////////
    vector<pair<int, int> > reach_out;
    for(int i=0;i<gate_count;i++) {
        reach_out.push_back(make_pair(i, 0));
    }
    for(int i=0; i<gate_count; i++) {
        if(gate_list[i].type == INPUT && gate_list[i].key_input == 1) {
            int out = 0;
            int *color = new int[gate_count]();
            queue<int> q;
            color[i] = 1;
            q.push(i);
            while(!q.empty()) {
                int u = q.front();
                if(gate_list[u].type == OUTPUT)
                    reach_out[i].second++;
                for(list<int>::iterator it = adjacency_list[u].begin();
                        it != adjacency_list[u].end(); it++) {
                    if(color[*it] == 0) {
                        
                        color[*it] = 1;
                        q.push(*it);
                    }
          
                }
                color[u] = 2;
                q.pop();
            }
            
            //cout<<endl;
            delete(color);
            //-1: others, 0: non_mutable, 1:mutable, 2:input, 3:key_input
            
        }
    }
    //////////////////////////////////////////////////////
    //for(int i=0;i<score.size();i++) {
    //    score[i].second += pass_through[score[i].first].second;
    //}
    int k;
    for( k=0;k<gate_count;k++) {
        if(gate_list[k].type==INPUT && gate_list[k].key_input == 1)
            break;
    }
    int key_start_id = k;
    double avg_score = 0;
    double avg_minus_score = 0;
    double avg_reach_out = 0;
    double avg_pass_through = 0;
    double std_score = 0;
    double std_minus_score = 0;
    double std_reach_out = 0;
    double std_pass_through = 0;
    for(int i=key_start_id; i<key_start_id+key.size(); i++) {
        avg_score += score[i].second;
        avg_minus_score += minus_score[i].second;
        avg_reach_out += reach_out[i].second;
        avg_pass_through += pass_through[i].second;
        
        std_score += pow(score[i].second,2.0);
        std_minus_score += pow(minus_score[i].second,2.0);
        std_reach_out += pow(reach_out[i].second,2.0);
        std_pass_through += pow(pass_through[i].second,2.0);
    }
    avg_score /= key.size();
    avg_minus_score /= key.size();
    avg_reach_out /= key.size();
    avg_pass_through /= key.size();
    std_score = sqrt((std_score/key.size() - pow(avg_score,2)) / key.size());
    std_minus_score = sqrt((std_minus_score/key.size() - pow(avg_minus_score,2)) / key.size());
    std_reach_out = sqrt((std_reach_out/key.size() - pow(avg_reach_out,2)) / key.size());
    std_pass_through = sqrt((std_pass_through/key.size() - pow(avg_pass_through,2)) / key.size());
    vector<pair<int, double> >total_score;
    for(int i=key_start_id; i<key_start_id+key.size(); i++) {
        total_score.push_back(make_pair(i, (score[i].second-avg_score)/std_score
                    +3*(reach_out[i].second-avg_reach_out)/std_reach_out + (pass_through[i].second-avg_pass_through)/std_pass_through));
    }

    int key_size = score.size();
    sort(total_score.begin(), total_score.end(), sortbysecb);
    sort(score.begin(), score.end(), sortbysec);
    sort(minus_score.begin(), minus_score.end(), sortbysec);
    sort(reach_out.begin(), reach_out.end(), sortbysec);
    sort(pass_through.begin(), pass_through.end(), sortbysec);
    //for(int i=0;i<score.size();i++) {
    //    cout<<score[i].first<<" "<<score[i].second<<" "<<minus_score[i].first<<" "<<minus_score[i].second<<endl;
    //}

    int N = atoi(argv[5]);
    //cout<<N<<endl;
    string outputkey(key);
    int xsize = key.size() * (100-N) / 100;
    for(int i=0;i<xsize; i++) {
        string tmp(gate_list[total_score[i].first].name.begin()+8, gate_list[total_score[i].first].name.end());
        int id = stoi(tmp);
        //cout<<id<<endl;
        outputkey[id] = 'x';
    }
    fstream outf;
    outf.open(argv[3], ios::out);
    outf<<outputkey<<endl;
    outf.close();
    cout<<outputkey<<endl;
    /*
    for(int i=0;i<gate_count;i++) {
        cout<<gate_list[i].name<<" "<<gate_list[i].type<<" ";
        for(list<int>::iterator it=adjacency_list[i].begin();it!=adjacency_list[i].end();it++) {
            cout<<*it<<" ";
        }
        cout<<endl;
    }
    */
    /*
    for(int i=0;i<gate_count;i++) {
        if(i == 1984) {
            cout<<gate_list[i].name<<endl;
            for(list<int>::iterator it=adjacency_list[i].begin();
                    it != adjacency_list[i].end();it++) {
                cout<<gate_list[*it].name<<" ";
            }
        }
    }
    cout<<endl;
    */
    return 0;
}
