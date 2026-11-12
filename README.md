# TP-Alex-Medeiros-de-Carvalho
Trabalho de Programação: Fechando o Campeonato_ Alex Medeiros de Carvalho _ 20252bsi0410
Este projeto implementa o sistema de apuração do Campeonato Computacional de Futebol em C. Os dados dos times e os resultados das partidas são lidos a partir de arquivos CSV, processados para atualizar as estatísticas de cada clube e, por fim, ordenados para gerar a tabela de classificação final, que é impressa na tela e salva em um novo arquivo.
Estrutura do Repositório
├── times.csv                 # Base de dados dos times inscritos (ID, Nome)
├── partidas.csv              # Resultados das partidas disputadas
├── classificacao.csv         # Tabela de classificação gerada pelo programa (saída)
├── Makefile                  # Automação de compilação, execução e limpeza
├── main.c                    # Ponto de entrada (leitura, apuração, ordenação e relatório)
├── team.c / team.h           # TAD Team para gerenciamento individual das estatísticas do time
├── teamlist.c / teamlist.h   # TAD TeamList (Lista Duplamente Encadeada) para a coleção de times
├── match.c / match.h         # TAD Match para representação de uma partida
└── matchlist.c / matchlist.h # TAD MatchList (Lista Encadeada) para a coleção de partidas

Sobre os arquivos:
 * times.csv e partidas.csv: Arquivos de dados de entrada que devem estar no diretório principal. times.csv segue o formato ID,Nome e partidas.csv segue ID,Time1,Time2,GolsTime1,GolsTime2.
 * classificacao.csv: Arquivo de saída gerado pelo programa contendo a tabela final ordenada.
 * Makefile:
   * make ou make all: compila todos os arquivos e gera o executável ./campeonato.
   * make run: compila (se necessário) e executa o programa.
   * make clean: remove os arquivos objeto (.o), o executável e o arquivo classificacao.csv.
 * main.c: Arquivo principal que não possui interação com o usuário. Ele carrega os CSVs, apura os resultados cruzando os dados das partidas com os times, ordena a classificação, imprime o relatório de destaques e salva o resultado final.
 * team.c / team.h:
   Implementam o TAD Team:
   * Criação e liberação de um time.
   * Acesso (getters) para os campos do time.
   * Funções para registrar resultados de partidas e calcular pontos e saldo de gols dinamicamente.
 * teamlist.c / teamlist.h:
   Implementam o TAD TeamList (lista duplamente encadeada):
   * Inserção no final (tl_append).
   * Inserção ordenada (tl_insert_ordered) com base nos critérios de desempate do campeonato.
   * Liberação de memória com flag de controle para evitar double-free.
 * match.c / match.h:
   Definem o TAD Match, contendo apenas os dados brutos de uma partida isolada (ID, times envolvidos e gols).
 * matchlist.c / matchlist.h:
   Implementam o TAD MatchList (lista encadeada simples) para armazenar o histórico de todas as partidas lidas do arquivo.
TADs utilizados:
1. Team
O TAD Team, definido em team.c, representa um clube de futebol e possui os seguintes campos:
 * id (int): Identificador único do time.
 * name (char*): Nome do time (alocado dinamicamente).
 * wins (int): Total de vitórias.
 * draws (int): Total de empates.
 * losses (int): Total de derrotas.
 * goals_scored (int): Gols marcados (GM).
 * goals_conceded (int): Gols sofridos (GS).
Nota: Saldo de Gols (S) e Pontos Ganhos (PG) não são armazenados na struct, sendo calculados em tempo real por funções auxiliares para evitar redundância de dados.
2. TeamList (Lista Duplamente Encadeada)
O TAD TeamList gerencia a coleção de times. É composto por nós (struct node), onde cada nó aponta para o próximo e para o anterior.
 * node:
   * team (Team*): Ponteiro para os dados do time.
   * next (node*): Ponteiro para o próximo nó.
   * prev (node*): Ponteiro para o nó anterior.
 * TeamList:
   * head (node*): Início da lista.
   * tail (node*): Fim da lista.
   * size (int): Quantidade de times armazenados.
3. Match
O TAD Match encapsula os dados de um jogo:
 * id (int): Identificador da partida.
 * t1, t2 (int): IDs do Time 1 e Time 2.
 * g1, g2 (int): Gols marcados pelo Time 1 e Time 2.
4. MatchList (Lista Encadeada Simples)
O TAD MatchList guarda o histórico de partidas:
 * Possui estrutura semelhante à TeamList, porém utiliza encadeamento simples (apenas ponteiro next), ideal para inserções rápidas no final da lista durante a leitura do arquivo.
Instruções para Execução
 * Pré-requisitos:
 * Compilador GCC (recomendado ambiente Linux Ubuntu).
 * Arquivos times.csv e partidas.csv no mesmo diretório do código-fonte. Você pode renomear um dos arquivos de cenário (ex: partidas_completo.csv) para partidas.csv para testar diferentes momentos do campeonato.
 * Compilar
   No terminal, execute o comando:
   make

 * Executar o programa
   ./campeonato

 * Limpeza dos arquivos gerados
   make clean

Principais Decisões de Implementação
 * Listas Encadeadas: Utilização de lista duplamente encadeada para times (facilitando inserções ordenadas) e lista simples para partidas (otimizando a leitura sequencial).
 * Ordenação Direta (Inserção Ordenada): Em vez de implementar um algoritmo de ordenação complexo (como Merge Sort ou Bubble Sort), foi criada a função tl_insert_ordered. Ela percorre os times processados e os insere em uma nova lista já na posição correta, respeitando rigorosamente a hierarquia de desempate (PG > V > S > GM > ID).
 * Tratamento de Alinhamento UTF-8: Foi mantida a função utf8_continuation_bytes para contar os bytes extras gerados por caracteres acentuados (ex: "ESCorpiões"). Isso garante que a impressão da tabela no terminal fique perfeitamente alinhada.
 * Gerenciamento de Memória Avançado: Como a lista original de times e a lista classificada guardam ponteiros para as mesmas structs de Team, a função tl_free recebeu uma flag especial (free_teams). Isso permite limpar os nós da lista classificada sem dar free duplo nos dados dos times, prevenindo Segmentation Fault e vazamentos de memória (Memory Leaks).
 * Geração de Arquivo CSV: A gravação de classificacao.csv ocorre em paralelo com a impressão na tela, dentro do mesmo loop de iteração da lista ordenada, garantindo eficiência máxima (O(n)).
