#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <iterator>

using namespace std;

int main(int argc, char *argv[]){

  // Inicialização
  // ======================================

  string fileName = argv[1];
  ifstream myfile (fileName);
  string line;
  int num;
  int temp = 0;

  int numberOfServers, numberOfJobs, custo, lineCounter = 1;

  // const int numberOfJobs = n;
  // const int numberOfServers = m;
    
  vector<int> b;
  vector<vector<int>> t;
  vector<vector<int>> c;

  if(myfile.is_open()) {
    while (getline(myfile,line)){
      istringstream is (line);

      if(lineCounter == 1){ // primeira linha
        numberOfJobs = stoi(line); // numero de jobs
        cout << numberOfJobs << endl;
      } 
      else if(lineCounter == 2){ // segunda linha
        numberOfServers = stoi(line); // numero de servidores
        cout << numberOfServers << endl;
      }
      else if(lineCounter == 3){ // terceira linha
        custo = stoi(line); // custo de cada penalidade
        cout << custo << endl;
      }

      // quarta linha em branco
      
      if(lineCounter == 5) { // quinta linha
        // preenchendo o array b
        while( is >> num){
          b.push_back(num);
        }
      }

      // sexta linha em branco

      else if (lineCounter >= 7 && lineCounter < (7 + numberOfServers)){ // setima linha
        t.push_back(vector<int>());
        while( is >> num){
          // cout << num;
          // cout << "; ";
          t[temp].push_back(num);
        }

        temp++;
        if(lineCounter == (7 + numberOfServers-1)){
          temp = 0;    
        }
      }

      // decima segunda linha em branco

      if (lineCounter > (7 + numberOfServers) && lineCounter <= (12 + numberOfServers)){
        // cout << (7 + numberOfServers + 1) << endl;
        // cout << 12 + numberOfServers << endl;
        // cout << temp;
        c.push_back(vector<int>());
        while( is >> num){
          // cout << num;
          // cout << "; ";
          c[temp].push_back(num);
        }
        // cout << endl;
        temp++;
      }

      lineCounter++;
    }

    myfile.close();
  } else {
    cout << "Unable to open file";
  }

  cout << "T size is:";
  cout << t.size();
  cout << endl;

  // for ( int i = 0; i < t.size(); i++ ){
  //   for ( int j = 0; j < t[i].size(); j++) {
  //     cout << t[i][j] << "; ";
  //   }
  // }
  // cout << endl;
  
  // cout << "C size is:";
  // cout << c.size();
  // cout << endl;


  for ( int i = 0; i < c.size(); i++ ){
    for ( int j = 0; j < c[i].size(); j++) {
      cout << c[i][j] << "; ";
    }
  }
  cout << endl;

  // ======================================

  // heuristica de construção
  // to do

  // vizinhaça
  // to do

  // VND
  // to do

  // meta-heuristica
  // to do (opcional)
 
  return 0;
}