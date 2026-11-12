#include "team.h"
#include "teamlist.h"
#include "match.h"
#include "matchlist.h"
#include <stdio.h>
#include <stdlib.h>

// Corrige o alinhamento com caracteres UTF-8[span_12](start_span)[span_12](end_span)
static int utf8_continuation_bytes(const char *s) {
    int n = 0;
    for (const unsigned char *p = (const unsigned char *)s; *p; p++)
        if ((*p & 0xC0) == 0x80) n++;
    return n;
}

int main(void) {
    TeamList *teams = tl_create();
    MatchList *matches = ml_create();
    char buffer[256];
    
    // 1. Carga de Dados[span_13](start_span)[span_13](end_span)
    FILE *f_times = fopen("times.csv", "rt");
    if (f_times) {
        fgets(buffer, sizeof(buffer), f_times); // ignorar cabeçalho[span_14](start_span)[span_14](end_span)
        int id;
        char name[50];
        while (fgets(buffer, sizeof(buffer), f_times)) {
            if (sscanf(buffer, "%d, %49[^\n\r]", &id, name) == 2) {
                tl_append(teams, team_create(id, name));
            }
        }
        fclose(f_times);
    }

    FILE *f_partidas = fopen("partidas.csv", "rt");
    if (f_partidas) {
        fgets(buffer, sizeof(buffer), f_partidas); // ignorar cabeçalho[span_15](start_span)[span_15](end_span)
        int id, t1, t2, g1, g2;
        while (fgets(buffer, sizeof(buffer), f_partidas)) {
            if (sscanf(buffer, "%d,%d,%d,%d,%d", &id, &t1, &t2, &g1, &g2) == 5) {
                ml_append(matches, match_create(id, t1, t2, g1, g2));
            }
        }
        fclose(f_partidas);
    }

    // 2. Apuração[span_16](start_span)[span_16](end_span)
    int total_matches = ml_size(matches);
    for (int i = 0; i < total_matches; i++) {
        Match *m = ml_get(matches, i);
        Team *team1 = tl_find_by_id(teams, match_t1(m));
        Team *team2 = tl_find_by_id(teams, match_t2(m));
        if (team1 && team2) {
            team_add_result(team1, match_g1(m), match_g2(m));
            team_add_result(team2, match_g2(m), match_g1(m));
        }
    }

    // 3. Classificação (inserção ordenada noutra lista)[span_17](start_span)[span_17](end_span)[span_18](start_span)[span_18](end_span)
    TeamList *sorted = tl_create();
    for (int i = 0; i < tl_size(teams); i++) {
        tl_insert_ordered(sorted, tl_get(teams, i));
    }

    // 4. Relatório (Imprimir e Gravar)[span_19](start_span)[span_19](end_span)[span_20](start_span)[span_20](end_span)
    printf("Pos ID Time            V  E  D  GM GS S  PG\n");
    FILE *f_classificacao = fopen("classificacao.csv", "w");
    if (f_classificacao) {
        fprintf(f_classificacao, "Pos,ID,Time,V,E,D,GM,GS,S,PG\n"); // Cabeçalho do novo ficheiro CSV[span_21](start_span)[span_21](end_span)
    }

    for (int i = 0; i < tl_size(sorted); i++) {
        Team *t = tl_get(sorted, i);
        
        // Impressão alinhada da tabela[span_22](start_span)[span_22](end_span)
        printf("%-3d %-2d %-15s", i + 1, team_id(t), team_name(t));
        for (int j = 0; j < utf8_continuation_bytes(team_name(t)); j++) putchar(' ');
        printf(" %-2d %-2d %-2d %-2d %-2d %-2d %d\n",
               team_wins(t), team_draws(t), team_losses(t),
               team_goals_scored(t), team_goals_conceded(t),
               team_goal_diff(t), team_points(t));

        // Gravar no ficheiro CSV de saída[span_23](start_span)[span_23](end_span)
        if (f_classificacao) {
            fprintf(f_classificacao, "%d,%d,%s,%d,%d,%d,%d,%d,%d,%d\n",
                    i + 1, team_id(t), team_name(t),
                    team_wins(t), team_draws(t), team_losses(t),
                    team_goals_scored(t), team_goals_conceded(t),
                    team_goal_diff(t), team_points(t));
        }
    }
    if (f_classificacao) fclose(f_classificacao);

    // Resumo final[span_24](start_span)[span_24](end_span)
    printf("\n===== Resumo do campeonato =====\n");
    printf("Partidas disputadas: %d de 45\n", total_matches);

    if (total_matches > 0) {
        Team *leader = tl_get(sorted, 0);
        if (total_matches == 45) { // 'Campeão' só quando há 45 partidas[span_25](start_span)[span_25](end_span)
            printf("Campeão: %s (%d pontos)\n", team_name(leader), team_points(leader));
        } else {
            printf("Líder: %s (%d pontos)\n", team_name(leader), team_points(leader));
        }

        Team *best_attack = tl_get(sorted, 0);
        Team *best_defense = tl_get(sorted, 0);

        for (int i = 1; i < tl_size(sorted); i++) {
            Team *t = tl_get(sorted, i);
            // Uso de '>' e '<' estritos garante que o critério de desempate prevalece a primeira equipa (melhor classificada)[span_26](start_span)[span_26](end_span)
            if (team_goals_scored(t) > team_goals_scored(best_attack)) best_attack = t;
            if (team_goals_conceded(t) < team_goals_conceded(best_defense)) best_defense = t;
        }

        printf("Melhor ataque: %s (%d gols marcados)\n", team_name(best_attack), team_goals_scored(best_attack));
        printf("Melhor defesa: %s (%d gols sofridos)\n", team_name(best_defense), team_goals_conceded(best_defense));

        Match *biggest_win = ml_get(matches, 0);
        int max_diff = abs(match_g1(biggest_win) - match_g2(biggest_win));

        for (int i = 1; i < ml_size(matches); i++) {
            Match *m = ml_get(matches, i);
            int diff = abs(match_g1(m) - match_g2(m));
            if (diff > max_diff) { // Uso de '>' estrito mantém o empate pela primeira partida lida do ficheiro[span_27](start_span)[span_27](end_span)
                max_diff = diff;
                biggest_win = m;
            }
        }
        
        Team *t1 = tl_find_by_id(teams, match_t1(biggest_win));
        Team *t2 = tl_find_by_id(teams, match_t2(biggest_win));
        printf("Maior goleada: %s %d x %d %s\n", team_name(t1), match_g1(biggest_win), match_g2(biggest_win), team_name(t2));
    }

    // Liberta memórias de forma robusta e encerra a aplicação
    tl_free(teams, 1);   // Apaga nós + times[span_28](start_span)[span_28](end_span)
    tl_free(sorted, 0);  // Apaga apenas nós (os times já foram libertos)[span_29](start_span)[span_29](end_span)
    ml_free(matches);
    
    return 0;
}
