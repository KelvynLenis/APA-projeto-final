#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <iterator>

using namespace std;

int main(int argc, char *argv[]){

  // ========== definição de variáveis ===================

  vector<int> b; // vetor de capacidade
  vector<vector<int>> t; // vetor de tempo
  vector<vector<int>> c; // vetor de custo

  // ============== Input do arquivo =====================

  string fileName = argv[1];
  ifstream myfile (fileName);
  string line;
  int num;
  int temp = 0;

  int numberOfServers, numberOfJobs, penalidade = 0;
  int lineCounter = 1;

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
        penalidade = stoi(line); // penalidade de cada penalidade
        cout << penalidade << endl;
      }
      
      if(lineCounter == 5) { // quinta linha
        // preenchendo o array b
        while( is >> num){
          b.push_back(num);
        }
      }
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

      if (lineCounter > (7 + numberOfServers) && lineCounter <= (12 + numberOfServers)){
        c.push_back(vector<int>());
        while( is >> num){
          c[temp].push_back(num);
        }
        temp++;
      }

      lineCounter++;
    }

    myfile.close();
  } 
  else {
    cout << "Unable to open file";
  }

  cout << "T size is: ";
  cout << t.size() << " x ";
  cout << t[0].size() << endl;
  
  cout << "C size is: ";
  cout << c.size() << " x ";
  cout << c[0].size() << endl;

  // ======================================


  // heuristica de construção (algoritmo guloso)
  // to do

  // for ( int i = 0; i < t.size(); i++ ){
  //   for ( int j = 0; j < t[i].size(); j++) {
  //     cout << t[i][j] << "; ";
  //   }
  // }

  // for ( int i = 0; i < c.size(); i++ ){
  //   for ( int j = 0; j < c[i].size(); j++) {
  //     cout << c[i][j] << "; ";
  //   }
  // }

  // vizinhaça
  // to do

  // VND
  // to do

  // meta-heuristica
  // to do (opcional)
 
  return 0;
}