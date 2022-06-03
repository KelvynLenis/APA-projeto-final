#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <iterator>

using namespace std;

void print2DVector(vector<vector<int>> vec){
  for ( int i = 0; i < vec.size(); i++ ){
    for ( int j = 0; j < vec[i].size(); j++) {
      cout << vec[i][j] << "; ";
    }
  }
  cout << endl;
}

void showAllocationLogs(vector<vector<int>> servers, vector<int> b, int totalCost){
  // cout << "Servers running: " << servers.size();
  // cout << endl;

  // cout << "Allocated jobs on server 1: " << servers[0].size();
  // cout << endl;
  for ( int i = 0; i < servers.size(); i++ ){
    int tempTime = 0;
    for ( int j = 0; j < servers[i].size(); j++) {
      cout << servers[i][j] << ", ";
      tempTime += servers[i][j];
    }
    // cout << endl;
    cout << "Server " << i << " capacity is " << b[i] << endl;
    cout << "Total time allocated is " << tempTime;
    cout << endl;
  }

  cout << "Solution cost: " << totalCost << endl;
}

int main(int argc, char *argv[]){

  // ========== definição de variáveis ===================

  vector<int> b; // vetor de capacidade
  vector<vector<int>> t; // vetor de tempo
  vector<vector<int>> c; // vetor de custo
  vector<vector<int>> servers; // vetor de servidores
  int numberOfServers, numberOfJobs, penalty = 0;

  // ============== Input do arquivo =====================

  string fileName = argv[1];
  ifstream myfile (fileName);
  string line;
  int num;
  int temp = 0;

  int lineCounter = 1;

  if(myfile.is_open()) {
    while (getline(myfile,line)){
      istringstream is (line);

      if(lineCounter == 1){ // primeira linha
        numberOfJobs = stoi(line); // numero de jobs
        // cout << numberOfJobs << endl;
      } 
      else if(lineCounter == 2){ // segunda linha
        numberOfServers = stoi(line); // numero de servidores
        // cout << numberOfServers << endl;
      }
      else if(lineCounter == 3){ // terceira linha
        penalty = stoi(line); // penalty de cada penalty
        // cout << penalty << endl;
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

  // cout << "T size is: ";
  // cout << t.size() << " x ";
  // cout << t[0].size() << endl;
  
  // cout << "C size is: ";
  // cout << c.size() << " x ";
  // cout << c[0].size() << endl;

  // ========================================================
  // ===== heuristica de construção (algoritmo guloso) ======
  // ========================================================

  // Implementação simples de um algoritmo guloso longe do ideal

  int totalCost = 0; // custo da solução

  // criando os servidores e alocando tantos jobs quanto possivel
  for(int i = 0; i < numberOfServers; i++){
    servers.push_back(vector<int>());
    int allocatedTime = 0;
    for(int j = 0; j < numberOfJobs; j++){
      if(t[i][j] + allocatedTime <= b[i]){
        servers[i].push_back(t[i][j]);
        allocatedTime += t[i][j];
        totalCost += c[i][j];
      }
      else {
        totalCost += penalty;
      }
    }
  }

  // showAllocationLogs(servers, b, totalCost);
  // print2DVector(t);

  // vizinhaça
  // to do

  // VND
  // to do

  // meta-heuristica
  // to do (opcional)
 
  return 0;
}