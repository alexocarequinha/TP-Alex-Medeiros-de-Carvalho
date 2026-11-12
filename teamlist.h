#ifndef TEAMLIST_H
#define TEAMLIST_H
#include "team.h"

typedef struct teamlist TeamList;

TeamList *tl_create(void);
void tl_append(TeamList *tl, Team *t);
void tl_insert_ordered(TeamList *tl, Team *t);
Team *tl_find_by_id(TeamList *tl, int id);
int tl_size(TeamList *tl);
Team *tl_get(TeamList *tl, int i);
void tl_free(TeamList *tl, int free_teams);

#endif
