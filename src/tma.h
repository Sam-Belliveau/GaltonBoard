#ifndef TIME_MOVING_AVERAGE_H
#define TIME_MOVING_AVERAGE_H

#define TIME_MOVING_AVERAGE_SIZE 16

typedef struct {
    int32_t sum;
    size_t index;
    int32_t values[TIME_MOVING_AVERAGE_SIZE];
} tma_t;

static inline void tma_init(tma_t *x) {
    x->sum = 0;
    x->index = 0;
    for (int i = 0; i < TIME_MOVING_AVERAGE_SIZE; i++) {
        x->values[i] = 0;
    }
}

static inline void tma_add(tma_t *tma, int32_t value) {
    tma->sum += value - tma->values[tma->index];
    tma->values[tma->index] = value;
    tma->index = (tma->index + 1) & (TIME_MOVING_AVERAGE_SIZE - 1);
}

static inline int32_t tma_get(tma_t *tma) {
    return tma->sum / TIME_MOVING_AVERAGE_SIZE;
}

#endif // TIME_MOVING_AVERAGE_H