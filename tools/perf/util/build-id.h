#ifndef PERF_BUILD_ID_H_
#define PERF_BUILD_ID_H_ 1

<<<<<<< HEAD
#include "session.h"

<<<<<<< HEAD
extern struct perf_event_ops build_id__mark_dso_hit_ops;
=======
extern struct perf_tool build_id__mark_dso_hit_ops;
>>>>>>> refs/remotes/origin/cm-10.0

char *dso__build_id_filename(struct dso *self, char *bf, size_t size);

=======
#define BUILD_ID_SIZE 20

#include "tool.h"
#include "types.h"

extern struct perf_tool build_id__mark_dso_hit_ops;
struct dso;

int build_id__sprintf(const u8 *build_id, int len, char *bf);
char *dso__build_id_filename(const struct dso *dso, char *bf, size_t size);

int build_id__mark_dso_hit(struct perf_tool *tool, union perf_event *event,
			   struct perf_sample *sample, struct perf_evsel *evsel,
			   struct machine *machine);
>>>>>>> refs/remotes/origin/master
#endif
