#ifndef _MINI_TANDEM_H_
#define _MINI_TANDEM_H_
#include <zlib.h>
#include "seq.h"

KSEQ_INIT(gzFile, gzread)

#define THREAD_N 1
#define CHUNK_READ_N 4096 // 1

#define KMER_SIZE 8  // kmer length
#define KMER_WSIZE 1 // window size
#define KMER_SSIZE 1 // step size
#define KMER_MINM 1  // 1: minimizer
#define REP_RANGE -1 // -1: unlimited
#define MIN_COPY 2
#define MAX_DIV 0.25
#define MIN_PERIOD 30
#define MAX_PERIOD UINT16_MAX

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    int k, w, s, m, hpc; // k-mer length, window size, selected minimum m hash values // keep all k-mer when w == m
    int min_copy; double max_div, div_exp; // max allowed divergence
    int min_p, max_p;
    int max_range; // max range to find tandem repeat, -1 for no limit
    int only_longest; // only output the cons that spans the longest sequence
    char *splint_fn, *splint_seq, *splint_rc_seq; int splint_len;
    char *five_fn, *five_seq, *five_rc_seq; int five_len;
    char *three_fn, *three_seq, *three_rc_seq; int three_len;
    FILE *cons_out, *detail_fp; //char detail_out[1024];
    int n_thread;
} mini_tandem_para;

typedef struct {
    // TODO kstring_t *output_str;
    seq_t *cons_seq;
    int cons_n, cons_m; 
    int *cons_start, *cons_end, *cons_len; double *copy_num;
    int8_t *splint_rotated; // use cons_len to partition cons_seq when cons_n > 1
    int *cons_score;
} tandem_seq_t;

typedef struct {
    int tid;
    mini_tandem_para *mtp;
    int n_seqs; kseq_t *read_seq; tandem_seq_t *tseq;
    // auxiliary memory for computing
    int8_t **hit_array; int array_m; // for finding max hit
} thread_aux_t;


int mini_tandem(const char *read_fn, mini_tandem_para *mtp);

mini_tandem_para *mini_tandem_init_para(void);

void mini_tandem_free_para(mini_tandem_para *mtp);

#ifdef __cplusplus
}
#endif

#endif
