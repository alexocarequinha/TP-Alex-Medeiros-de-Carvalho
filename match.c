#include "match.h"
#include <stdlib.h>

struct match {
    int id;
    int t1, t2;
    int g1, g2;
};

Match *match_create(int id, int t1, int t2, int g1, int g2) {
    Match *m = (Match *)malloc(sizeof(struct match));
    if (m != NULL) {
        m->id = id;
        m->t1 = t1;
        m->t2 = t2;
        m->g1 = g1;
        m->g2 = g2;
    }
    return m;
}

void match_free(Match *m) { free(m); }
int match_id(Match *m) { return m->id; }
int match_t1(Match *m) { return m->t1; }
int match_t2(Match *m) { return m->t2; }
int match_g1(Match *m) { return m->g1; }
int match_g2(Match *m) { return m->g2; }
