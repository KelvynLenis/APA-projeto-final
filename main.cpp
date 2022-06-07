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
vector<vector<int>> t; // vetor de tempo
vector<vector<int>> c; // vetor de custo
vector<bool> unallocatedJobs; // vetor de flags para cada job alocado ou não
int numberOfServers, numberOfJobs, penalty = 0;


class Server {
  public:    
    int id;         // identificador do servidor
    int cost = 0;   // custo total de cada servidor
    int time = 0;   // tempo total alocado de cada servidor
    int maxTime = 0; // capacidade total de cada servidor
    vector<pair<int,int>> jobs; // vetor de jobs alocados

    void printJobsContent() {
      cout << "[jobIndex, jobCost]: ";
      for(auto job : jobs){
        cout << "["<< job.first << ", " << job.second << "], "; 
      }
      cout << endl;
    }
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
  cout << "Solution cost: " << cost << endl;
  return cost;
}

void getUnallocatedJobs(vector<bool> unallocatedJobs){
  int count = 0;
  for(auto job : unallocatedJobs){
    if(job == true) count++;
  }
  cout << "Number of unallocated jobs: " << count << endl;
}

void swap(vector<Server> &servers, vector<bool> &unallocatedJobs, int i, int j, int k, vector<vector<int>> &t, vector<vector<int>> &c){
  int totalCost;
  int newTime, newCost; // variaveis para comparar com os valores antigos
  vector<Server> auxServers(servers); // copia do vetor original (sujeito a otimização)
  int jobIndex = auxServers[i].jobs[j].first;

  // variaveis para otimizar a função
  newCost = servers[i].cost - c[i][jobIndex] + c[i][k];
  newTime = servers[i].time - t[i][jobIndex] + t[i][k];
  
  auxServers[i].cost = auxServers[i].cost - c[i][jobIndex] + c[i][k];
  auxServers[i].time = auxServers[i].time - t[i][jobIndex] + t[i][k];

  auxServers[i].jobs[j].first = k;
  auxServers[i].jobs[j].second = c[i][k];

  // troca não otimizada
  if(auxServers[i].cost < servers[i].cost){
    if(auxServers[i].time <= auxServers[i].maxTime){
      cout << "Found a better solution for server " << i << endl;
      servers[i].printJobsContent();

      auxServers[i].printJobsContent();
      cout << "[server " << i << "] cost is " << servers[i].cost << endl;
      cout << "[server " << i << "] new cost is " << auxServers[i].cost << endl;
      // cout << "[server " << i << "] time allocated is " << servers[i].time << endl;
      // cout << "[server " << i << "] new time allocated is " << auxServers[i].time << endl;
      totalCost = calculateCost(auxServers, unallocatedJobs, c);
    }
  }

  // otimização
  // if(newCost < servers[i].cost){
  //   if(newTime <= auxServers[i].maxTime){
  //     cout << "Found a better solution for server " << i << endl;
  //     servers[i].printJobsContent();

  //     // auxServers[i].printJobsContent();
  //     cout << "[server " << i << "] cost is " << servers[i].cost << endl;
  //     cout << "[server " << i << "] new cost is " << newCost << endl;
  //     // cout << "[server " << i << "] time allocated is " << servers[i].time << endl;
  //     // cout << "[server " << i << "] new time allocated is " << auxServers[i].time << endl;
  //     // totalCost = calculateCost(auxServers, unallocatedJobs, c);
  //   }
  // }
}

void swap2(vector<Server> &servers, int i, int j, int k){
  int totalCost;
  int newTime, newCost; // variaveis para comparar com os valores antigos
  vector<Server> auxServers(servers); // copia do vetor original (sujeito a otimização)
  int jobIndex = auxServers[i].jobs[j].first;
  if(unallocatedJobs[k] == true){
    
    auxServers[i].cost = auxServers[i].cost - c[i][jobIndex] + c[i][k];
    auxServers[i].time = auxServers[i].time - t[i][jobIndex] + t[i][k];

    auxServers[i].jobs[j].first = k;
    auxServers[i].jobs[j].second = c[i][k];

    // troca não otimizada
    if(auxServers[i].cost < servers[i].cost){
      if(auxServers[i].time <= auxServers[i].maxTime){
        cout << "Found a better solution for server " << i << endl;
        servers[i].printJobsContent();

        auxServers[i].printJobsContent();
        cout << "[server " << i << "] cost is " << servers[i].cost << endl;
        cout << "[server " << i << "] new cost is " << auxServers[i].cost << endl;
        totalCost = calculateCost(auxServers, unallocatedJobs, c);
      }
    }
  }
}

void swapShort(vector<Server> &servers, int i, int j, int k){
  int totalCost;
  int newTime, newCost; // variaveis para comparar com os valores antigos
  vector<Server> auxServers(servers); // copia do vetor original (sujeito a otimização)
  int jobIndex = auxServers[i].jobs[j].first;

  // variaveis para otimizar a função
  newCost = servers[i].cost - c[i][jobIndex] + c[i][k];
  newTime = servers[i].time - t[i][jobIndex] + t[i][k];

  if(newTime <= auxServers[i].maxTime){
    auxServers[i].cost = auxServers[i].cost - c[i][jobIndex] + c[i][k];
    auxServers[i].time = auxServers[i].time - t[i][jobIndex] + t[i][k];

    auxServers[i].jobs[j].first = k;
    auxServers[i].jobs[j].second = c[i][k];
    totalCost = calculateCost(auxServers, unallocatedJobs, c);
  }
}

int swapShortSingle(Server server, int i, int j){
  if(unallocatedJobs[j] == true){
    int newTime, newCost; // variaveis para comparar com os valores antigos
    Server auxServer(server); // copia do servidor original (sujeito a otimização)
    int jobIndex = auxServer.jobs[i].first;

    newCost = auxServer.cost - c[server.id][jobIndex] + c[server.id][j];
    newTime = auxServer.time - t[server.id][jobIndex] + t[server.id][j];

    if(newTime <= auxServer.maxTime){
      auxServer.cost = auxServer.cost - c[server.id][jobIndex] + c[server.id][j];
      auxServer.time = auxServer.time - t[server.id][jobIndex] + t[server.id][j];

      auxServer.jobs[i].first = j;
      auxServer.jobs[i].second = c[server.id][j];

      return auxServer.cost;
    }
  }

  return -1;
}

// to do
void vnd(vector<Server> servers, int r){
  int k = 1;

  while(k <= r) {
    for(int i = 0; i < servers.size(); i++){ // quantidade de servidores
      for(int j = 0; j < servers[i].jobs.size(); j++){ // quantidade de jobs de cada servidor
        for(int k = j + 1; k < numberOfJobs; k++){ // quantidade total de jobs
          if(swapShortSingle(servers[i], j, k) > 0 && swapShortSingle(servers[i], j, k) < servers[i].cost){
            cout << "Encontrado uma solução melhor, substitua a original" << endl;
          }
        }
      }
    }
  }
}

void generateNeighborhood(vector<Server> &servers, int &numberOfJobs){
  for(int i = 0; i < servers.size(); i++){ // quantidade de servidores
    for(int j = 0; j < servers[i].jobs.size(); j++){ // quantidade de jobs de cada servidor
      for(int k = j + 1; k < numberOfJobs; k++){ // quantidade total de jobs
        swap2(servers, i, j, k);
      }
    }
  }
}

void generateNeighborhoodSingle(Server server, int &numberOfJobs){
  for(int i = 0; i < server.jobs.size(); i++){ // quantidade de jobs de cada servidor
    for(int j = i + 1; j < numberOfJobs; j++){ // quantidade total de jobs
      swapShortSingle(server, i, j);
    }
  }
}

int main(int argc, char *argv[]){

  // ========== definição de variáveis ===================

  vector<int> b; // vetor de capacidade
  // vector<vector<int>> t; // vetor de tempo
  // vector<vector<int>> c; // vetor de custo
  vector<Server> servers; // vetor de servidores
  // vector<bool> unallocatedJobs; // vetor de flags para cada job alocado ou não
  // int numberOfServers, numberOfJobs, penalty = 0;

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


  // ========================================================
  // ===== heuristica de construção (algoritmo guloso) ======
  // ========================================================

  // Implementação simples de um algoritmo guloso longe do ideal

  int totalCost = 0; // custo da solução

  // criando os servidores e alocando tantos jobs quanto possivel
  // precisa de uma refatoração ainda.
  for(int i = 0; i < numberOfServers; i++){
    servers.push_back(Server());
    servers[i].id = i;
    servers[i].maxTime = b[i];
    for(int j = 0; j < numberOfJobs; j++){
      if(t[i][j] + servers[i].time <= servers[i].maxTime){
        if(unallocatedJobs[j] == true){
          pair<int, int> currentJob (j, c[i][j]); // pegando separadamente o indice do job e seu custo

          servers[i].jobs.push_back(currentJob); // insere o job e seu custo no vetor de jobs
          unallocatedJobs[j] = false; // mudar a flag
          servers[i].cost += c[i][j]; // atualiza custo
          servers[i].time += t[i][j]; // atualiza tempo alocado
        }
      }
    }
  }

  // showAllocationLogs(servers, b, totalCost, unallocatedJobs, t);
  // print2DVector(t);
  // getUnallocatedJobs(unallocatedJobs);


  // ====== vizinhaça ==========
  totalCost = calculateCost(servers, unallocatedJobs, c);

  // swap()
  for(int i = 0; i < servers.size(); i++){ // quantidade de servidores
    for(int j = 0; j < servers[i].jobs.size(); j++){ // quantidade de jobs de cada servidor
      for(int k = j + 1; k < numberOfJobs; k++){ // quantidade total de jobs
        swap2(servers, i, j, k);
      }
    }
  }

  // swap(servers, unallocatedJobs, 0, 1, 6, t, c); // test
  // swapShort(servers, 0, 1, 6);
  // showAllocationLogs(auxServers, b, totalCost, unallocatedJobs, t);

  // VND
  // to do

  // meta-heuristica
  // to do (opcional)
 
  return 0;
}