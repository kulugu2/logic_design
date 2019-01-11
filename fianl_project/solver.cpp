#include<iostream>
#include<string>
#include<map>
#include<vector>
#include<list>
#include<fstream>
#include<algorithm>

using namespace std;

enum gate_type{INPUT, OUTPUT, BUF, NOT, OR, NOR, AND, NAND, XOR, XNOR};

class gate{
public:
    string name;
    gate_type type;
    gate() {
        
    }
    gate(string n, gate_type g) {
        name = n;
        type = g;
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
            string gate_t(buffer.begin()+found+2, buffer.begin()+buffer.find("("));
            transform(gate_t.begin(), gate_t.end(), gate_t.begin(), ::toupper);
            cout<<gate_t<<endl;
            gate new_gate(name, gate_type_map[gate_t]);
            create_gate(new_gate);
            found = buffer.find("(");
            size_t found2 = found;
            while((found2 = buffer.find(",", found+1)) != string::npos) {
                string input_gate(buffer.begin()+found+1, buffer.begin()+found2);
                if(input_gate[0] == ' ')
                    input_gate.erase(0,1);
                
            }
            break;
        }
    }
    enc_circ.close();
    return 0;
}
