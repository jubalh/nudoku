// vim: noexpandtab:ts=4:sts=4:sw=4

#ifndef TYPES_H
#define TYPES_H

typedef struct {
	int width;
	int height;
} PAPER_SIZE;

// Size in inches x100, to allow hundredths of an inch.
#define PS_LETTER ((PAPER_SIZE) { 850, 1100 })
#define PS_A4 ((PAPER_SIZE) { 827, 1169 })

#endif // TYPES_H
