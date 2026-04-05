// vim: noexpandtab:ts=4:sts=4:sw=4

#ifndef TYPES_H
#define TYPES_H

typedef struct {
    int width;
    int height;
} PAPER_SIZE;

// Size in inches x100, to allow hundredths of an inch.
static const PAPER_SIZE PS_LETTER = { 850, 1100 };
static const PAPER_SIZE PS_A4 = { 827, 1169 };

#endif // TYPES_H
