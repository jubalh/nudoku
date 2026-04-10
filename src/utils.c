// vim: noexpandtab:ts=4:sts=4:sw=4

#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct {
	bool valid;
	char *name;
	PAPER_UNIT_CODE code;
	double value;
} PAPER_UNIT_TYPE;

typedef struct {
	char *name;
	PAPER_SIZE size;
} PAPER_SIZE_TYPE;

#define UT_INVALID ((PAPER_UNIT_TYPE) { false, "", PUC_INVALID, (double) -1 })

static PAPER_UNIT_TYPE unit_types[] = {
	{ true, "cm", PUC_CM, (double) 28.3465 },
	{ true, "in", PUC_IN, (double) 72.0 },
	{ true, "pt", PUC_PT, (double) 1.0 }
};

// Size in units x100, to allow hundredths of the unit.
static PAPER_SIZE_TYPE size_types[] = {
	{ "a4", PS_DEFAULT },
	{ "a5", { true, PUC_CM, 1480, 2100 } },
	{ "letter", { true, PUC_IN, 850, 1100 } },
	{ "legal", { true, PUC_IN, 850, 1400 } },
	{ "oficio", { true, PUC_IN, 850, 1340 } },
	{ "invoice", { true, PUC_IN, 550, 850 } }
};

static double paperunit_getrate_bycode(PAPER_UNIT_CODE code) {
	double rate = -1;

	size_t n = ARRAY_SIZE(unit_types);
	for (size_t i = 0; i < n; i++) {
		PAPER_UNIT_TYPE ut = unit_types[i];

		if (ut.code == code) {
			rate = ut.value;
			break;
		}
	}

	return rate;
}

// Normalize paper size to points (pt).
PAPER_SIZE papersize_normalize(PAPER_SIZE size) {
	if (!size.valid)
		return size;
	
	PAPER_SIZE ps_converted;

	double rate = paperunit_getrate_bycode(size.unit);

	ps_converted.valid = size.valid;
	ps_converted.unit = PUC_PT;
	ps_converted.width = (size.width * rate) / 100;
	ps_converted.height = (size.height * rate) / 100;

	return ps_converted;
}

static PAPER_UNIT_TYPE paperunit_get_byname(char *name) {
	PAPER_UNIT_TYPE ut_got = UT_INVALID;

	size_t n = ARRAY_SIZE(unit_types);
	for (size_t i = 0; i < n; i++) {
		PAPER_UNIT_TYPE ut = unit_types[i];

		if (strcmp(name, ut.name) == 0) {
			ut_got = ut;
			break;
		}
	}

	return ut_got;
}

// Try to parse the paper size as custom.
static PAPER_SIZE papersize_trycustom(char *spec) {
	PAPER_SIZE ps_got = {};

	double width = 0;
	double height = 0;
	char unit[3] = "";

	int count = sscanf(spec, "%lfx%lf%2s", &width, &height, unit);

	if (count >= 2) {
		char *unit_name = (count == 3) ? unit : "cm";
		PAPER_UNIT_TYPE ut = paperunit_get_byname(unit_name);

		if (ut.valid) {
			ps_got.valid = true;
			ps_got.unit = ut.code;
			ps_got.width = (int) (ABS(width) * (double) 100.0);
			ps_got.height = (int) (ABS(height) * (double) 100.0);
		} else {
			ps_got = PS_INVALID;
		}
	} else {
		ps_got = PS_INVALID;
	}

	return ps_got;
}

// Look for named paper size.
static PAPER_SIZE papersize_trynamed(char *name) {
	PAPER_SIZE ps_got = PS_INVALID;

	size_t n = ARRAY_SIZE(size_types);
	for (size_t i = 0; i < n; i++) {
		PAPER_SIZE_TYPE pt = size_types[i];

		if (strcmp(name, pt.name) == 0) {
			ps_got = pt.size;
			break;
		}
	}

	return ps_got;
}

// Parse paper size (try named first, then custom).
PAPER_SIZE papersize_get(char *spec) {
	PAPER_SIZE ps = papersize_trynamed(spec);
	if (ps.valid)
		return ps;
	
	ps = papersize_trycustom(spec);
	if (ps.valid)
		return ps;
	
	return PS_INVALID;
}
