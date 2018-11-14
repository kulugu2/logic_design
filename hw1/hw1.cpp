#include<iostream>
#include<string>
#include<sstream>
#include<vector>
#include<cmath>
#include<fstream>
#include<iomanip>
#include<cstdlib>
#include<string>
using namespace std;

int grid[30][30];
int restrict[30][30][30] = {0};
int N;
int blankN = 0;

int main(int argc, char ** argv) {
    string ss;
    int ni = 0;
    int prefill = 0;
    fstream outfile;
    fstream infile;
    infile.open(argv[1], ios::in);
    outfile.open("SAT_in.txt", ios::out);
    while(getline(infile, ss)) {
        istringstream ii(ss);
        int nj = 0;
        while(ii>>grid[ni][nj++]) {
            if(grid[ni][nj-1] != 0)
                prefill++;
        }
        N = nj-1;
        ni++;
    }
    int total = N*N*(1+N*(N-1)/2)*4+prefill;
    //cout<<total<<endl;
    
    outfile<<"p cnf "<<N*N*N<<" "<<total<<endl;
    
    for(int i=0;i<N;i++) { //exact one digit appears in each cell
        for(int j=0;j<N;j++) {
            for(int k=0;k<N;k++) {
                outfile<<"1"<<setw(2)<<setfill('0')<<i<<setw(2)<<j<<setw(2)<<k<<" ";
            }
            outfile<<"0"<<endl;
            for(int k=0; k<N; k++) {
                outfile<<"1"<<setw(2)<<setfill('0')<<i<<setw(2)<<k<<setw(2)<<j<<" ";
            }
            outfile<<"0"<<endl;
            for(int k=0; k<N; k++) {
                outfile<<"1"<<setw(2)<<setfill('0')<<k<<setw(2)<<i<<setw(2)<<j<<" ";
            }
            outfile<<"0"<<endl;
            for(int a=0;a<N-1;a++) {
                for(int b = a+1; b<N;b++) {
                    outfile<<"-1"<<setw(2)<<setfill('0')<<i<<setw(2)<<j<<setw(2)<<a<<" -1"<<setw(2)<<i<<setw(2)<<j<<setw(2)<<b<<" 0"<<endl;
                    outfile<<"-1"<<setw(2)<<setfill('0')<<i<<setw(2)<<a<<setw(2)<<j<<" -1"<<setw(2)<<i<<setw(2)<<b<<setw(2)<<j<<" 0"<<endl;
                    outfile<<"-1"<<setw(2)<<setfill('0')<<a<<setw(2)<<i<<setw(2)<<j<<" -1"<<setw(2)<<b<<setw(2)<<i<<setw(2)<<j<<" 0"<<endl;
                }
            }
        }
    }
    
    
    int n = sqrt(N); 
    //block
    for(int d=0;d<N;d++) { //digit
        for(int i=0;i<n;i++) { 
            for(int j=0;j<n;j++) {
                for(int ii=0;ii<n;ii++){
                    for(int jj=0;jj<n;jj++) {
                        outfile<<"1"<<setw(2)<<setfill('0')<<n*i+ii<<setw(2)<<n*j+jj<<setw(2)<<d<<" ";
                    }
                }
                outfile<<"0"<<endl;

                for(int ii=0;ii<N-1;ii++) {
                    for(int jj=ii+1; jj<N;jj++) {
                        outfile<<"-"<<"1"<<setw(2)<<setfill('0')<<n*i+ii/n<<setw(2)<<n*j+ii%n<<setw(2)<<d<<" ";
                        outfile<<"-"<<"1"<<setw(2)<<setfill('0')<<n*i+jj/n<<setw(2)<<n*j+jj%n<<setw(2)<<d<<" 0"<<endl;
                    }
                }
            }
        }
    }
    
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++) {
            if(grid[i][j]!=0)
                outfile<<"1"<<setw(2)<<setfill('0')<<i<<setw(2)<<j<<setw(2)<<grid[i][j]-1<<" 0"<<endl;
        }
    }
    outfile.close();
    infile.close();
    string command = "./" + string(argv[3]) + " SAT_in.txt SAT_out.txt";
    system(command.c_str());
    fstream resultf;
    resultf.open("SAT_out.txt", ios::in);
    string s;
    resultf>>s;
    int a;
    int counter = 0;
    if(s == "SAT") {
        while(resultf>>a){
            if(a>0) {
                //cout<<a<<endl;
                grid[counter/N][counter%N] = a%100 + 1;
                counter++;
            }
        }
    }
    resultf.close(); 
    fstream outputfile;
    outputfile.open(argv[2], ios::out);
    if(s == "SAT") {
        for(int i=0;i<N;i++) {
            for(int j=0;j<N;j++) {
                if(j != N-1)
                    outputfile<<grid[i][j]<<" ";
                else
                    outputfile<<grid[i][j];
            }
            outputfile<<endl;
        }
    } else {
        outputfile<<"NO"<<endl;
    }
    return 0;
}
