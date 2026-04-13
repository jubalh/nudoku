// vim: noexpandtab:ts=4:sts=4:sw=4

#ifndef UTILS_H
#define UTILS_H

#include "gettext.h"		/* gettext */
#include <stdbool.h>		/* bool data type */

#define _(x) gettext(x)

typedef unsigned char PAPER_UNIT_CODE;

typedef struct {
	bool valid;
	PAPER_UNIT_CODE unit;
	int width;
	int height;
} PAPER_SIZE;

#define PUC_INVALID ((PAPER_UNIT_CODE) -1)
#define PUC_PT ((PAPER_UNIT_CODE) 0)
#define PUC_IN ((PAPER_UNIT_CODE) 1)
#define PUC_CM ((PAPER_UNIT_CODE) 2)

#define PS_INVALID ((PAPER_SIZE) { false, PUC_INVALID, -1, -1 })
#define PS_DEFAULT ((PAPER_SIZE) { true, PUC_CM, 2100, 2970 })

#define ABS(n) (((n) > 0) ? (n) : -(n))
#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

PAPER_SIZE papersize_normalize(PAPER_SIZE size);
PAPER_SIZE papersize_get(char *name);

#endif // UTILS_H
