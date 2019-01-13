#include<iostream>
#include<string>
#include<map>
#include<vector>
#include<list>
#include<queue>
#include<fstream>
#include<algorithm>

using namespace std;

enum gate_type{INPUT, OUTPUT, BUF, NOT, OR, NOR, AND, NAND, XOR, XNOR};

class gate{
public:
    string name;
    gate_type type;
    int key_input;
    int mutabl; //-1: others, 0: non_mutable, 1:mutable, 2:input, 3:key_input
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
        if(gate_list[i].key_input ==1   ) {
            cout<<i<<endl;
            int *color = new int[gate_count]();
            queue<int> q;
            color[i] = 1;
            q.push(i);
            gate_list[i].mutabl = 2;
            while(!q.empty()) {
                int u = q.front();
                cout<<gate_list[u].name<<" ";
                for(list<int>::iterator it = adjacency_list[u].begin();
                        it != adjacency_list[u].end(); it++) {
                    if(color[*it] == 0) {
                        color[*it] = 1;
                        q.push(*it);

                    }
                }
                if(gate_list[u].type == OUTPUT)
                    cout<<"out "<<endl;
                color[u] = 2;
                q.pop();
            }
            cout<<endl;
            delete(color);
            //-1: others, 0: non_mutable, 1:mutable, 2:input, 3:key_input
            
        }
    }
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
