/* For general debugging purposes */

#include "../perf.h"

#include <string.h>
#include <stdarg.h>
#include <stdio.h>

#include "cache.h"
#include "color.h"
#include "event.h"
#include "debug.h"
#include "util.h"
<<<<<<< HEAD
=======
#include "target.h"
>>>>>>> refs/remotes/origin/master

int verbose;
bool dump_trace = false, quiet = false;

<<<<<<< HEAD
int eprintf(int level, const char *fmt, ...)
{
	va_list args;
	int ret = 0;

	if (verbose >= level) {
		va_start(args, fmt);
		if (use_browser > 0)
			ret = ui_helpline__show_help(fmt, args);
		else
			ret = vfprintf(stderr, fmt, args);
		va_end(args);
=======
static int _eprintf(int level, const char *fmt, va_list args)
{
	int ret = 0;

	if (verbose >= level) {
		if (use_browser >= 1)
			ui_helpline__vshow(fmt, args);
		else
			ret = vfprintf(stderr, fmt, args);
>>>>>>> refs/remotes/origin/master
	}

	return ret;
}

<<<<<<< HEAD
int dump_printf(const char *fmt, ...)
{
	va_list args;
	int ret = 0;

	if (dump_trace) {
		va_start(args, fmt);
		ret = vprintf(fmt, args);
		va_end(args);
	}
=======
int eprintf(int level, const char *fmt, ...)
{
	va_list args;
	int ret;

	va_start(args, fmt);
	ret = _eprintf(level, fmt, args);
	va_end(args);
>>>>>>> refs/remotes/origin/master

	return ret;
}

<<<<<<< HEAD
#ifdef NO_NEWT_SUPPORT
<<<<<<< HEAD
void ui__warning(const char *format, ...)
=======
int ui__warning(const char *format, ...)
>>>>>>> refs/remotes/origin/cm-10.0
{
	va_list args;

	va_start(args, format);
	vfprintf(stderr, format, args);
	va_end(args);
<<<<<<< HEAD
}
#endif

void ui__warning_paranoid(void)
{
	ui__warning("Permission error - are you root?\n"
=======
	return 0;
}
#endif

int ui__error_paranoid(void)
{
	return ui__error("Permission error - are you root?\n"
>>>>>>> refs/remotes/origin/cm-10.0
		    "Consider tweaking /proc/sys/kernel/perf_event_paranoid:\n"
		    " -1 - Not paranoid at all\n"
		    "  0 - Disallow raw tracepoint access for unpriv\n"
		    "  1 - Disallow cpu events for unpriv\n"
		    "  2 - Disallow kernel profiling for unpriv\n");
=======
/*
 * Overloading libtraceevent standard info print
 * function, display with -v in perf.
 */
void pr_stat(const char *fmt, ...)
{
	va_list args;

	va_start(args, fmt);
	_eprintf(1, fmt, args);
	va_end(args);
	eprintf(1, "\n");
}

int dump_printf(const char *fmt, ...)
{
	va_list args;
	int ret = 0;

	if (dump_trace) {
		va_start(args, fmt);
		ret = vprintf(fmt, args);
		va_end(args);
	}

	return ret;
>>>>>>> refs/remotes/origin/master
}

void trace_event(union perf_event *event)
{
	unsigned char *raw_event = (void *)event;
	const char *color = PERF_COLOR_BLUE;
	int i, j;

	if (!dump_trace)
		return;

	printf(".");
	color_fprintf(stdout, color, "\n. ... raw event: size %d bytes\n",
		      event->header.size);

	for (i = 0; i < event->header.size; i++) {
		if ((i & 15) == 0) {
			printf(".");
			color_fprintf(stdout, color, "  %04x: ", i);
		}

		color_fprintf(stdout, color, " %02x", raw_event[i]);

		if (((i & 15) == 15) || i == event->header.size-1) {
			color_fprintf(stdout, color, "  ");
			for (j = 0; j < 15-(i & 15); j++)
				color_fprintf(stdout, color, "   ");
			for (j = i & ~15; j <= i; j++) {
				color_fprintf(stdout, color, "%c",
					      isprint(raw_event[j]) ?
					      raw_event[j] : '.');
			}
			color_fprintf(stdout, color, "\n");
		}
	}
	printf(".\n");
}
