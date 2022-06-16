Seja n o número de jobs e m o número de servidores na nuvem disponíveis no portfólio da
empresa. Cada job j ∈ {1, . . . , n} requer um tempo de processamento igual a tsj para ser
processado no servidor s ∈ {1, . . . , m} e gera um custo csj . Alémm disso, cada servidor s, possui
uma capacidade bs, que especifica a quantidade máxima de tempo que pode ser contratada.
Sendo assim, o objetivo do problema é o de alocar cada job em um dos servidores, sem violar
a capacidade dos mesmos, e de forma a minimizar o custo total para a empresa.

## Tarefas
Esse projeto contém os seguintes entregáveis:
- [x] Implementação de ao menos uma heurística de construção, que nada mais é do que um algoritmo guloso para geração de uma solução viável.
- [x] Implementação de pelo menos 2 movimentos de vizinhança.
- [x] Implementação do algoritmo de busca local chamado VND (Variable Neighborhood Descent)
- [ ] Implementação de uma meta-heurística (OPCIONAL). Sugestões: GRASP ou ILS
- [x] criar uma tabela que contenha os resultados obtidos pela(s) heurística(s)
construtiva(s) e pelo VND, e que compare tais resultados com a solução ótima de cada instância


## Tabela
Essa tabela deverá conter os seguintes dados para cada heurística construtiva e para o VND:

- [x] Média do valor da solução (em no mínimo 10 execuções para cada instância caso exista
algum fator aleatório no algoritmo)
- [x] Melhor solução encontrada
- [x] Média do tempo gasto pelo respectivo algoritmo
- [x] GAP para a solução ótima


|  |  | Heurística construtiva |  |  | VND  | VND |  |
|---|---|:---:|---|---|:---:|---|---|
|  | ótimo | Valor <br>solução | tempo | gap | Valor <br>solução | tempo | gap |
| n5m15A | 261 | 3239 | 0.020 | 1140 | 3214 | 0.7998 | 1131 |
| n5m15B | 269 | 3244 | 0.0175 | 1105 | 1251 | 1.0977 | 365 |
| n25m5A | 438 | 3439 | 0.0208 | 685 | 2503 | 2.9848 | 471 |
| n30m5A | 423 | 2504 | 0.0219 | 491 | 2407 | 4.1786 | 469 |
| n60m10 | 954 | 10979 | 0.0365 | 1050 | 1779 | 15.2754 | 86 |
| n60m10A | 973 | 9068 | 0.0545 | 831 | 1505 | 17.0863 | 54 |


# PRAZO 16/06/2022
