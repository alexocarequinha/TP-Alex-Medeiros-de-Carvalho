#ifndef TEAM_H
#define TEAM_H

typedef struct team Team;

Team *team_create(int id, const char *name);
void team_free(Team *t);

int team_id(Team *t);
const char *team_name(Team *t);
int team_wins(Team *t);
int team_draws(Team *t);
int team_losses(Team *t);
int team_goals_scored(Team *t);
int team_goals_conceded(Team *t);

// Funções novas para calcular resultados e saldo
int team_points(Team *t);
int team_goal_diff(Team *t);
void team_add_result(Team *t, int goals_scored, int goals_conceded);

#endif
