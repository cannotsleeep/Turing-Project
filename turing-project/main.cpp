#include <iostream>
#include <string>
#include <vector>
#include <fstream>
using namespace std;

typedef struct _slist* slist; 
vector<string> state, sym, t_sym, t_dir, t_sta; 
vector<string> Q, S, G, F;
string q0, B,c_state;
const int Ftape = 100000, ntape = 2000;
int tx =0, tapeN, c_index, tmp, flagv=0, step;
int c_i[10];
char t[10][Ftape]; 
void pr(int step);
//10 tapes, -200+400 symbols
int run(string input);
string p(string s){
    for(int i = 0;i<s.length();i++){
        if(s[i]==';') return s.substr(0,i);
    }
    return s;
}
vector<string> pp(string s){
    vector<string> re;
    int i=0, j=1;
    while(j<=s.length()-1){
        while(s[j]!= ',' && s[j]!='}')j++;
        re.push_back(s.substr(i,j-i));
        i = ++j;
    }
    //fprintf(stderr, "%s\n", re[0].c_str());
    return re;
}
int main(int argc, char* argv[]){
    
    if(argc == 1){
        return 1;
    }
    //fprintf(stderr, "%s\n", argv[1]);
    if((string)argv[1] == "--help" || (string)argv[1] == "-h") {
        fprintf(stdout, "usage: turing [-v|--verbose] [-h|--help] <tm> <input>\n");
        return 0;}
    int arg = 0;
    if((string)argv[1] == "-v" || (string)argv[1] == "--verbose") {arg = 1;flagv=1;}
    ifstream ifs;
    ifs.open(argv[1+arg], ios::in);
    //fprintf(stderr, "%s\n",argv[1]);
    if(!ifs.is_open()){fprintf(stderr, "文件打开失败\n");return -1;}
    string str;
    while(getline(ifs, str)){
        if(str[0]==';'|| str=="") continue;
        if(tx<=6&&tx>=0 ) {
            string s = p(str);
            int t1=0,t2=0,t3=0,t4=0;
            if(tx==3||tx==4||tx==6) t3=1;
            if(tx==3||tx==4||tx==6) t4=1;
            for(int i =0;i<s.length();i++){
                if(s[i]=='#') t1++;
                if(s[i]=='=') t2++;
                if(s[i]=='{') t3++;
                if(s[i]=='}') t4++;
            }
            if(!(t1==1&&t2==1&&t3==1&&t4==1)) {
                fprintf(stderr, "syntax error\n");
                return -1;
            }

        }
        if(tx==0 && str.substr(0,2)=="#Q"){
            str = p(str);
            if(str[str.length()-1] != '}') {fprintf(stderr, "syntax error\n"); return -2;}
            Q = pp(str.substr(6, str.length()-6));
            tx++;
        }
        if(tx==1 && str.substr(0,2)=="#S"){
            str = p(str);
            if(str[str.length()-1] != '}') {fprintf(stderr, "syntax error\n"); return -2;}
            S = pp(str.substr(6, str.length()-6));
            tx++;
        }
        if(tx==2 && str.substr(0,2)=="#G"){
            str = p(str);
            if(str[str.length()-1] != '}') {fprintf(stderr, "syntax error\n"); return -2;}
            G = pp(str.substr(6, str.length()-6));
            tx++;
        }
        if(tx==3 && str.substr(0,3)=="#q0"){
            q0 = p(str).substr(6,p(str).length()-6);
            tx++;
        }
        if(tx==4 && str.substr(0,2)=="#B"){
            B = p(str).substr(5,p(str).length()-5);
            tx++;
        }
        if(tx==5 && str.substr(0,2)=="#F"){
            str = p(str);
            if(str[str.length()-1] != '}') {fprintf(stderr, "syntax error\n"); return -2;}
            F = pp(str.substr(6, str.length()-6));
            tx++;
        }
        if(tx==6 && str.substr(0,2)=="#N"){
            tapeN = atoi(p(str).substr(5,p(str).length()-5).c_str());
            //fprintf(stderr, "%d\n", tapeN);
            tx++;
        }
        if(tx==7 && str.length()>4 && str.substr(0,2)!="#N"){
            //fprintf(stderr, "%s\n",p(str).c_str());
            str = p(str);
            int i = 0, j=1;
            while(str[j]!=' ') j++; 
            state.push_back(str.substr(0,j));
            i = ++j;
            while(str[j]!=' ') j++; 
            sym.push_back(str.substr(i,j-i));
            i = ++j;
            while(str[j]!=' ') j++; 
            t_sym.push_back(str.substr(i,j-i));
            i = ++j;
            while(str[j]!=' ') j++; 
            t_dir.push_back(str.substr(i,j-i));
            t_sta.push_back(str.substr(j+1,str.length()-j-1));
            //fprintf(stderr, "%s %s %s %s %s\n",state[state.size()-1].c_str(),sym[sym.size()-1].c_str(),t_sym[t_sym.size()-1].c_str(),t_dir[t_dir.size()-1].c_str(),t_sta[t_sta.size()-1].c_str());
        }

    }
    ifs.close();
    //for(int i =0;i<G.size();i++) cout<<G[i]<<endl;
    string input = argv[2+arg];
    for(int i = 0;i<input.length();i++){
        int flag = 0;
        for(int j = 0;j < S.size();j++) {if(S[j][0]==input[i]) flag =1;}
        if(flag == 0) {
            if(flagv == 1){
                fprintf(stderr,"==================== ERR ====================\n");
                fprintf(stderr,"error: '%c' was not declared in the set of input symbols\n",input[i]);
                fprintf(stderr,"Input: %s\n", input.c_str());
                fprintf(stderr,"       ");
                for(int k =0;k<i;k++) fprintf(stderr," ");
                fprintf(stderr,"^\n");
                fprintf(stderr,"==================== END ====================\n");
                return -1 ; //Verbose mode
            } else {
                fprintf(stderr,"illegal input\n");
                return -1 ; //Normal mode
            }

        }
    }
    //cout<<input<<endl;
    if(flagv ==1 ){
        fprintf(stdout, "Input: %s\n", input.c_str());
        fprintf(stdout, "==================== RUN ====================\n");
    }
    run(input);

    return 0;
}

int find(string cur){
    string a;
    for(int i = 0; i<tapeN;i++) a += t[i][c_i[i]];
    //fprintf(stdout, "%s step:%d\n", a.c_str(), step);
    for(int i = 0;i< state.size();i++){
        int flags = 0;
        if( state[i]==cur ){
            flags= 1;
            //fprintf(stdout, "find sym:%s\n", sym[i].c_str());
            for(int k = 0;k<a.length();k++){
                if(sym[i][k]!=a[k] && (sym[i][k]!='*' || a[k] == '_')) flags=0;
            }
        }
        if(flags==1) return i;
    }
    return -1;
}; //return index

int run(string input){
    for(int i=0;i<tapeN;i++){
        for(int j = 0; j<Ftape ;j++){
            t[i][j]='_';
        }
        c_i[i] = ntape;
    }
    for(int i = 0; i < input.length(); i++){
        t[0][ntape+i] = input[i]; 
    }
    c_state = q0;
    tmp = find(c_state);
    step = 0;
    if(flagv==1) pr(step);
    while(tmp!=-1){
        for(int i = 0; i<tapeN; i++){
            if(t_sym[tmp][i]!='*') t[i][c_i[i]] = t_sym[tmp][i];
        }
        c_state = t_sta[tmp];
        for(int i = 0; i<tapeN;i++){
            if(t_dir[tmp][i] == 'l'){
                c_i[i]--;
            } else if(t_dir[tmp][i] == 'r'){
                c_i[i]++;
            };
        }
        step++;
        if(flagv==1) pr(step);
        tmp = find(c_state);
    }
    //fprintf(stderr, "Halt!%d\n", step);
    int i = 0, j = Ftape-1;
    while(t[0][i]=='_'&& i<Ftape) i++;
    while(t[0][j]=='_'&& j>-1) j--;
    if(flagv == 1) fprintf(stdout, "Result: ");
    for(int k=i;k<=j;k++){
        fprintf(stdout, "%c", t[0][k]);
    }
    fprintf(stdout, "\n");
    if(flagv == 1) fprintf(stdout, "==================== END ====================\n");
    
};

void pr(int step){
    fprintf(stdout, "Step   : %d\n", step);
    fprintf(stdout, "State  : %s\n", c_state.c_str());
    for(int n = 0;n<tapeN;n++){
        int i = 0, j = Ftape-1;
        while(t[n][i]=='_'&& i<Ftape) i++;
        while(t[n][j]=='_'&& j>-1) j--;
        if(j<i){
            int kk = c_i[n]-ntape;
            if(kk<0) kk = -kk;
            fprintf(stdout, "Index%d : %d\n",n , kk);
            fprintf(stdout, "Tape%d  : _\n",n);
            fprintf(stdout, "Head%d  : ^\n",n);
        } else{
            fprintf(stdout, "Index%d :",n);
            i = min(i,c_i[n]);
            j = max(j,c_i[n]);
            for(int k = i;k<=j;k++){
                int kk = k-ntape;
                if(kk< 0) kk = -kk;
                fprintf(stdout, " %d", kk);
            }
            fprintf(stdout, "\nTape%d  :",n);
            for(int k =i;k<=j;k++){
                if( k> ntape+99 || k< ntape-99){
                    fprintf(stdout, " %c  ", t[n][k]);
                }else if(k>ntape+9 || k <ntape-9){
                    fprintf(stdout, " %c ", t[n][k]);
                } else fprintf(stdout, " %c", t[n][k]);
            }
            fprintf(stdout, "\nHead%d  :",n);
            for(int k=i;k<c_i[n];k++) {
                if( k> ntape+99 || k<ntape-99){
                    fprintf(stdout, "    ");
                } else if(k>ntape+9 || k <ntape-9){
                    fprintf(stdout, "   ");
                } else fprintf(stdout, "  ");
            }
            fprintf(stdout, " ^\n");

        }
    }
    fprintf(stdout, "---------------------------------------------\n");
};
