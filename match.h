#ifndef MATCH_H
#define MATCH_H

typedef struct match Match;

Match *match_create(int id, int t1, int t2, int g1, int g2);
void match_free(Match *m);

int match_id(Match *m);
int match_t1(Match *m);
int match_t2(Match *m);
int match_g1(Match *m);
int match_g2(Match *m);

#endif
