#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <iterator>
#include <map>
#include <utility> 
#include <algorithm>

using namespace std;

class Server {
  public:    
    int id;         
    int cost = 0;
    int time = 0;        
    vector<pair<int,int>> jobs;
};

void print2DVector(vector<vector<int>> vec){
  for ( int i = 0; i < vec.size(); i++ ){
    for ( int j = 0; j < vec[i].size(); j++) {
      cout << vec[i][j] << "; ";
    }
  }
  cout << endl;
}

void showAllocationLogs(vector<Server> servers, vector<int> b, int &totalCost, vector<bool> unallocatedJobs, vector<vector<int>> t){
  for(int i = 0; i < servers.size(); i++){
    int tempTime = 0;
    for(int j = 0; j < servers[i].jobs.size(); j++){
      int jobIndex = servers[i].jobs[j].first; // Key-value na qual a key é o número do job e o value é o custo associado

      cout << servers[i].jobs[j].second << "; "; // custo c de cada job
      // cout << servers[i].jobs[j].first << "; "; // número de cada job
      tempTime += t[i][jobIndex]; // somando o tempo relativo alocado para cada servidor
    }
    cout << endl;
    cout << "[Server " << i << "] capacity is " << b[i] << endl;
    cout << "[Server "<< i << "] Total time allocated is " << tempTime;
    cout << endl;
  }

  for(auto job : unallocatedJobs){
    if(job == true){
      totalCost += 1000;
    }
  }
  cout << "Solution cost: " << totalCost << endl;
}

int calculateCost(vector<Server> servers, vector<bool> unallocatedJobs, vector<vector<int>> c){
  int cost = 0;
 
  for(int i = 0; i < servers.size(); i++){
    for(int j = 0; j < servers[i].jobs.size(); j++){
      int jobIndex = servers[i].jobs[j].first; // Key-value na qual a key é o número do job e o value é o custo associado
      // cout << servers[i][j].second << "; ";
      cost += servers[i].jobs[j].second;
    }
  }

  for(auto job : unallocatedJobs){
    if(job == true){
      cost += 1000;
    }
  }
  // cout << "Solution cost: " << cost << endl;
  return cost;
}

void getUnallocatedJobs(vector<bool> unallocatedJobs){
  int count = 0;
  for(auto job : unallocatedJobs){
    if(job == true) count++;
  }
  cout << "Number of unallocated jobs: " << count << endl;
}

int main(int argc, char *argv[]){

  // ========== definição de variáveis ===================

  vector<int> b; // vetor de capacidade
  vector<vector<int>> t; // vetor de tempo
  vector<vector<int>> c; // vetor de custo
  vector<Server> servers; // vetor de servidores
  vector<bool> unallocatedJobs; // vetor de flags para cada job alocado ou não
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
        penalty = stoi(line); // custo de cada penalty
        // cout << penalty << endl;
      }
      
      if(lineCounter == 5) { // quinta linha
        // preenchendo o vetor b
        while( is >> num){
          b.push_back(num);
        }
      }
      else if (lineCounter >= 7 && lineCounter < (7 + numberOfServers)){ // setima linha
        t.push_back(vector<int>());

        while( is >> num){
          // cout << num;
          // cout << "; ";
          if(unallocatedJobs.size() < numberOfJobs){
            unallocatedJobs.push_back(true);
          }
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
    servers.push_back(Server());
    servers[i].id = i;
    int allocatedTime = 0;
    for(int j = 0; j < numberOfJobs; j++){
      if(t[i][j] + allocatedTime <= b[i]){
        if(unallocatedJobs[j] == true){
          pair<int, int> currentJob (j, c[i][j]);
          servers[i].jobs.push_back(currentJob);
          unallocatedJobs[j] = false; // mudar a flag
          allocatedTime += t[i][j];
          servers[i].cost += c[i][j];
          servers[i].time += allocatedTime;
          totalCost += c[i][j];
        }
      }
    }
  }

  // showAllocationLogs(servers, b, totalCost, unallocatedJobs, t);
  // print2DVector(t);
  // getUnallocatedJobs(unallocatedJobs);

  // cout << totalCost << endl;

  // ====== vizinhaça ==========
  
  // swap()

  totalCost = calculateCost(servers, unallocatedJobs, c);
  for(int i = 0; i < servers.size(); i++){
    for(int j = 0; j < servers.size(); j++){
      for(int k = 0; k < servers[i].jobs.size(); k++){
        // to do
      }
    }
  }

  // 2Opt()

  // VND
  // to do

  // meta-heuristica
  // to do (opcional)
 
  return 0;
}