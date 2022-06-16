#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <iterator>
#include <map>
#include <utility> 
#include <algorithm>
#include <chrono>

using namespace std;

vector<vector<int>> t; // vetor de tempo
vector<vector<int>> c; // vetor de custo
vector<bool> unallocatedJobs; // vetor de flags para cada job alocado ou não
int numberOfServers = 0, numberOfJobs = 0, penalty = 0;

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

    void updateCost(){
      cost = 0;
      for(auto job : jobs){
        cost += job.second;
      }
    }

    int getIndex(int value){
      for(int i = 0; i < jobs.size(); i++){
        if(jobs[i].first == value){
          return i;
        }
      }

      return -1;
    }
};

// Função que printa uma matriz de duas dimensões feita com vetores.
void print2DVector(vector<vector<int>> vec){
  for ( int i = 0; i < vec.size(); i++ ){
    for ( int j = 0; j < vec[i].size(); j++) {
      cout << vec[i][j] << "; ";
    }
  }
  cout << endl;
}

// Função que calcula o custo total dos jobs alocados
int calculateCost(vector<Server> servers){
  int cost = 0;
 
  for(auto server : servers){
    cost += server.cost;
  }

  for(auto job : unallocatedJobs){
    if(job == true){
      cost += 1000;
    }
  }
  return cost;
}

// Função que verifica quantos jobs não foram alocados.
void getUnallocatedJobs(vector<bool> unallocatedJobs){
  int count = 0;
  for(auto job : unallocatedJobs){
    if(job == true) count++;
  }
  cout << "Number of unallocated jobs: " << count << endl;
}

// Função que simula a operação de swap retornando o custo da nova solução.
int simulateSwapUnallocatedJobs(Server server, int j, int k){
  if(unallocatedJobs[k] == true){
    int newTime, newCost; // variaveis para comparar com os valores antigos
    int jobIndex = server.jobs[j].first;

    newTime = server.time - t[server.id][jobIndex] + t[server.id][k];

    if(newTime <= server.maxTime){
      newCost = server.cost - c[server.id][jobIndex] + c[server.id][k];
      return newCost;
    }

  }
  return -1; // falhou ao encontrar um job não alocado ou um job com tamanho suficiente para ser alocado
}

// Função que dado um job encontrar em qual servidor ele está inserido.
// Retorno é -1 caso não esteja alocado em nenhum servidor.
int getServer(vector<Server> servers, int job){
  for(int i = 0; i < servers.size(); i++){
    for(int j = 0; j < servers[i].jobs.size(); j++){
      if(servers[i].jobs[j].first == job){
        int serverAndJobIndex = i;
        return serverAndJobIndex;
      }
    }
  }
  return -1;
}

// Função que simula a troca de jobs entre servidores
int simulateExchangeJobs(vector<Server> servers, Server server, int j, int k){
  if(unallocatedJobs[k] == false){
    int newTime, newTime2, newCost, newCost2; // variaveis para comparar com os valores antigos
    int jobIndex = server.jobs[j].first;
    int targetServerIndex = getServer(servers, k); // par que contém o servidor de origem e o indice do job nesse servidor.
    
    if( targetServerIndex < 0){
      return -1;
    }

    newTime = server.time - t[server.id][jobIndex] + t[server.id][k];
    newTime2 = servers[targetServerIndex].time - t[targetServerIndex][k] + t[targetServerIndex][jobIndex];
    
    if(newTime <= server.maxTime && newTime2 <= servers[targetServerIndex].maxTime){
      newCost = server.cost - c[server.id][jobIndex] + c[server.id][k];
      newCost2 = servers[targetServerIndex].cost - c[targetServerIndex][k] + c[targetServerIndex][jobIndex];
      if(newCost2 < servers[targetServerIndex].cost){
        return newCost;
      }
    }

  }
  return -1; // falhou ao encontrar um job não alocado ou um job com tamanho suficiente para ser alocado
}

// swap de jobs não alocados.
void swap(vector<Server> &servers,int serverId, int j, int k){
  if(getServer(servers, k) != -1){
    int jobIndex = servers[serverId].jobs[j].first;
    servers[serverId].cost = servers[serverId].cost - c[serverId][jobIndex] + c[serverId][k];
    servers[serverId].time = servers[serverId].time - t[serverId][jobIndex] + t[serverId][k];

    servers[serverId].jobs[j].first = k;
    servers[serverId].jobs[j].second = c[serverId][k];
    unallocatedJobs[jobIndex] = true;
    unallocatedJobs[k] = false;
  }
}

// Função que tenta inserir jobs não alocados nos servidores com espaço.
void refit(vector<Server> &servers, vector<int> &b){
  for(int i = 0; i < servers.size(); i++){
    for(int j = 0; j < unallocatedJobs.size(); j++){
      if(unallocatedJobs[j] == true){
        int time = t[i][j];
        if(servers[i].time + time <= servers[i].maxTime){
          pair<int, int> newJob;
          newJob.first = j;
          newJob.second = c[i][j];
          servers[i].jobs.push_back(newJob);
          servers[i].cost += c[i][j];
          servers[i].time += time;
          unallocatedJobs[j] = false;
        }
      }
    }
  }
}

/* Função que faz a troca de dois jobs entre servidores
Parametros:
  Servers: apontador para um vetor de servidores
  Inteiro i: indice do servidor origem.
  Inteiro j: índice do job do servidor origem.
  Inteiro k: índice do job do servidor de destino.*/
void exchange(vector<Server> &servers, int i, int j, int k){
  int jobIndex = servers[i].jobs[j].first;  // resgatando o indice do job j na tabela de tempo e custo.
  int targetServerIndex = getServer(servers, k); // indice do servidor que contem o job de indice k.

  servers[i].time = servers[i].time - t[i][jobIndex] + t[i][k];
  servers[targetServerIndex].time = servers[targetServerIndex].time - t[targetServerIndex][k] + t[targetServerIndex][jobIndex];
  
  servers[i].cost = servers[i].cost - c[i][jobIndex] + c[i][k];
  servers[targetServerIndex].cost = servers[targetServerIndex].cost - c[targetServerIndex][k] + c[targetServerIndex][jobIndex];

  servers[i].jobs[j].first = k; //substituindo o índice geral do job.
  servers[i].jobs[j].second = c[i][k]; // substituindo o custo dos jobs.

  int index = servers[targetServerIndex].getIndex(k);
  servers[targetServerIndex].jobs[index].first = jobIndex; //substituindo o índice geral do job.
  servers[targetServerIndex].jobs[index].second = c[targetServerIndex][jobIndex]; // substituindo o custo dos jobs.
}

// função que printa o vetor de jobs não alocados.
void printUnallocated(){
  cout << "Unallocated jobs: ";
  for(int i = 0; i < unallocatedJobs.size(); i++){
    if(unallocatedJobs[i] == true){
      cout << "Job " << i << "; "; 
    }
  }
  cout << endl;
}

// vnd para encontrar melhores vizinhanças
vector<Server> vnd(vector<Server> servers, vector<int> b){

  for(int i = 0; i < servers.size(); i++){ // quantidade de servidores
    int bestCost = servers[i].cost; // inicia a variável bestCost com o custo do servidor atual.
    pair <int, int> bestSwap; // posição dos jobs a serem trocados.
    int move = 1; // controle de movimento de vizinhança

    while(move <= 2){
      bool foundBest = false; // flag para indicar se encontrou uma melhor solução.
      for(int j = 0; j < servers[i].jobs.size(); j++){ // quantidade de jobs de cada servidor
      
        for(int k = j + 1; k < numberOfJobs; k++){ // quantidade total de jobs
          int tempCost = servers[i].cost; // resetando a variavel de custo temporário.
          
          if(move == 1){
            tempCost = simulateSwapUnallocatedJobs(servers[i], j, k);
          }
          else if(move == 2){
            tempCost = simulateExchangeJobs(servers, servers[i], j, k);            
          }

          if(tempCost > 0 && tempCost < servers[i].cost){
            if(tempCost < bestCost){
              bestCost = tempCost; // salva o melhor tempo encontrado.
              bestSwap.first = j; // salva a posição do job atual.
              bestSwap.second = k; // define o alvo da troca.
              foundBest = true; // seta a flag indicando que encontrou uma solução melhor.
            }
          }
        }
      }
      if (foundBest && move == 1) {
        swap(servers, i, bestSwap.first, bestSwap.second);
      }
      else if (foundBest && move == 2){
        exchange(servers, i, bestSwap.first, bestSwap.second);
      }
      move++;
    }
  }

  return servers;
}

// Função para trazer uma analise da alocação de cada servidor.
void showAllocationLogs(vector<Server> &servers, vector<int> b, int &totalCost, vector<bool> unallocatedJobs, vector<vector<int>> t){
  for(int i = 0; i < servers.size(); i++){
    int tempTime = 0;
    for(int j = 0; j < servers[i].jobs.size(); j++){
      int jobIndex = servers[i].jobs[j].first; // Key-value na qual a key é o número do job e o value é o custo associado

      cout << servers[i].jobs[j].first << ", "; // número de cada job
      // cout << servers[i].jobs[j].second << "; "; // custo c de cada job
      tempTime += t[i][jobIndex]; // somando o tempo relativo alocado para cada servidor
    }
    cout << endl;
    cout << "[Server " << i << "] capacity is " << b[i] << endl;
    cout << "[Server "<< i << "] Total time allocated is " << tempTime << endl;
    cout << "[Server " << i << "] Cost is " << servers[i].cost << endl;
    cout << endl;
  }
  int cost = calculateCost(servers);
  cout << "Total cost " << cost << endl;
}

// Função que cria os servidores aloca tantos jobs quanto possivel
vector<Server> heuristicaDeContrucao(vector<Server> servers, vector<int> b){
  for(auto job : unallocatedJobs){
    job = true;
  }

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

  return servers;
}

// Função para lidar com o input do arquivo e organizar as estruturas de dados do programa.
void handleInputFile(ifstream &myfile, vector<int> &b){
  string line;
  int lineCounter = 1;
  int num;
  int temp = 0;
  int fimT = 0;

  if(myfile.is_open()) {
    while (getline(myfile,line)){
      istringstream is (line);

      if(lineCounter == 1){ // primeira linha
        numberOfJobs = stoi(line); // numero de jobs
        // cout << numberOfJobs << endl;
      } 
      else if(lineCounter == 2){ // segunda linha
        numberOfServers = stoi(line); // numero de servidores
        fimT = 7 + numberOfServers;
        // cout << numberOfServers << endl;
      }
      else if(lineCounter == 3){ // terceira linha
        penalty = stoi(line); // custo de cada penalty
        // cout << penalty << endl;
      }
      
      if(lineCounter == 5) { // quinta linha
        // preenchendo o vetor b
        while( is >> num){
          cout << num << ", ";
          b.push_back(num);
        }
      }
      else if (lineCounter >= 7 && lineCounter < fimT){ // setima linha
        t.push_back(vector<int>());

        while( is >> num){
          if(unallocatedJobs.size() < numberOfJobs){
            unallocatedJobs.push_back(true);
          }
          t[temp].push_back(num);
        }

        temp++;
        if(lineCounter == fimT - 1){
          temp = 0;    
        }
      }

      if (lineCounter > fimT && lineCounter <= fimT + numberOfServers) {
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
}

// Função para calcular o Gap.
float calculateGap(float vHeuristica, float vOtimo){
  auto result = 0;
  result = ((vHeuristica - vOtimo) / vOtimo) * 100;

  return result;
}

int main(int argc, char *argv[]){

  // ========== definição de variáveis ===================

  vector<int> b; // vetor de capacidade
  vector<Server> servers; // vetor de servidores
  int totalCost = 0; // custo da solução
  int valorOtimo = 0; // valor ótimo da instância passada

  // ============== Input do arquivo =====================

  string fileName = argv[1];
  valorOtimo = stoi(argv[2]);
  ifstream myfile (fileName);
  handleInputFile(myfile, b);

  // ===== heuristica de construção (algoritmo guloso) ======

  float meanHDCTime = 0;
  float meanHDCCost = 0;

  vector<Server> copyServers;

  for(int i = 0; i < 10; i++){
    auto tInitHeuristicaDeConstrucao = chrono::steady_clock::now();
    copyServers = heuristicaDeContrucao(servers, b);
    auto tEndHeuristicaDeConstrucao = chrono::steady_clock::now();
    
    auto durationtHeuristicaDeConstrucao = (chrono::duration_cast<chrono::nanoseconds>( tEndHeuristicaDeConstrucao - tInitHeuristicaDeConstrucao ).count());
    float timeHeuristicaDeConstrucao = (float)durationtHeuristicaDeConstrucao/1000000;
    float HDCCost = calculateCost(copyServers);
    meanHDCTime += timeHeuristicaDeConstrucao;
    meanHDCCost += HDCCost;
  }

  servers = copyServers;

  meanHDCTime = meanHDCTime / 10;
  meanHDCCost = meanHDCCost / 10;

  // ============== vizinhaça ==============================
  float meanVNDTime = 0;
  float meanVNDCost = 0;

  vector<Server> copy2Servers;

  for(int i = 0; i < 10; i++){
    auto tInitVND = chrono::steady_clock::now();
    copy2Servers = vnd(servers, b);
    auto tEndVND = chrono::steady_clock::now();

    auto durationVND = (chrono::duration_cast<chrono::nanoseconds>( tEndVND - tInitVND ).count());
    float timeVND = (float) durationVND/1000000;
    float VNDCost = calculateCost(copy2Servers);
    meanVNDTime += timeVND;
    meanVNDCost += VNDCost;
  }

  servers = copy2Servers;

  showAllocationLogs(servers, b, totalCost, unallocatedJobs, t);
  printUnallocated();
  cout << endl;
  cout << endl;
  refit(servers, b);
  showAllocationLogs(servers, b, totalCost, unallocatedJobs, t);
  printUnallocated();

  meanVNDTime = meanVNDTime / 10;
  meanVNDCost = meanVNDCost / 10;
  
  // Resultados para o GAP
  cout << endl;
  cout << "============= Results ============="<< endl;
  cout << "Tempo médio de processamento da Heurística de construção: " << meanHDCTime << endl;
  cout << "Tempo médio de processamento do VND: " << meanVNDTime << endl;
  cout << "Custo médio da heurísitca de construção: " << meanHDCCost << endl;
  cout << "Custo médio do VND: " << meanVNDCost << endl;

  // GAP
  float heuristicaGap = calculateGap((float) meanHDCCost, valorOtimo);
  float vndGap = calculateGap((float) meanVNDCost, valorOtimo);

  cout << endl;
  cout << "============= GAP ================="<< endl;
  cout << "Gap da heurística: " << heuristicaGap << endl;
  cout << "Gap do VND: " << vndGap << endl;

  return 0;
}