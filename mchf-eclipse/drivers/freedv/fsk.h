/*---------------------------------------------------------------------------*\

  FILE........: fsk.h
  AUTHOR......: Brady O'Brien
  DATE CREATED: 6 January 2016

  C Implementation of 2FSK/4FSK modulator/demodulator, based on octave/fsk_horus.m

\*---------------------------------------------------------------------------*/

/*
  Copyright (C) 2016 David Rowe

  All rights reserved.

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License version 2.1, as
  published by the Free Software Foundation.  This program is
  distributed in the hope that it will be useful, but WITHOUT ANY
  WARRANTY; without even the implied warranty of MERCHANTABILITY or
  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
  License for more details.

  You should have received a copy of the GNU Lesser General Public License
  along with this program; if not, see <http://www.gnu.org/licenses/>.
*/


#ifndef __C2FSK_H
#define __C2FSK_H
#include <stdint.h>
#include "comp.h"
#include "kiss_fftr.h"
#include "modem_stats.h"

#define MODE_2FSK 2
#define MODE_4FSK 4

#define FSK_SCALE 16383

struct FSK {
    /*  Static parameters set up by fsk_init */
    int Ndft;               /* buffer size for freq offset est fft */
    int Fs;                 /* sample freq */
    int N;                  /* processing buffer size */
    int Rs;                 /* symbol rate */
    int Ts;                 /* samples per symbol */
    int Nmem;               /* size of extra mem for timing adj */
    int P;                  /* oversample rate for timing est/adj */
    int Nsym;               /* Number of symbols spat out in a processing frame */
    int Nbits;              /* Number of bits spat out in a processing frame */
    int f1_tx;              /* f1 for modulator */
    int fs_tx;              /* Space between TX freqs for modulatosr */
    int mode;               /* 2FSK or 4FSK */
    int est_min;            /* Minimum frequency for freq. estimator */
    int est_max;            /* Maximum frequency for freq. estimaotr */
    int est_space;          /* Minimum frequency spacing for freq. estimator */
    
    /*  Parameters used by demod */
    COMP phi1_c;
    COMP phi2_c;
    COMP phi3_c;
    COMP phi4_c;
    kiss_fftr_cfg fft_cfg;  /* Config for KISS FFT, used in freq est */
    float norm_rx_timing;   /* Normalized RX timing */
    
    float *samp_old;        /* Tail end of last batch of samples */
    int nstash;             /* How many elements are in there */
    
    float *fft_est;			/* Freq est FFT magnitude */
    
    /* Memory used by demod but not important between demod frames */
    
    /*  Parameters used by mod */
    COMP tx_phase_c;        /* TX phase, but complex */ 
    
    /*  Statistics generated by demod */
    float EbNodB;           /* Estimated EbNo in dB */
    float f1_est;           /* Estimated f1 freq. */
    float f2_est;           /* Estimated f2 freq. */
    float f3_est;			/* Estimated f3 freq. */
    float f4_est;			/* Estimated f4 freq. */
    float ppm;              /* Estimated PPM clock offset */
    
    /*  Parameters used by mod/demod and driving code */
    int nin;                /* Number of samples to feed the next demod cycle */
    
    /*  Pointer to modem statistic struct */
    struct MODEM_STATS *stats;
};

/*
 * Create an FSK config/state struct from a set of config parameters
 * 
 * int Fs - Sample frequency
 * int Rs - Symbol rate
 * int tx_f1 - '0' frequency
 * int tx_fs - frequency spacing
 */
struct FSK * fsk_create(int Fs, int Rs, int M, int tx_f1, int tx_fs);

/*
 * Create an FSK config/state struct from a set of config parameters
 * 
 * int Fs - Sample frequency
 * int Rs - Symbol rate
 * int tx_f1 - '0' frequency
 * int tx_fs - frequency spacing
 */
struct FSK * fsk_create_hbr(int Fs, int Rs, int P, int M, int tx_f1, int tx_fs);

/* 
 * Set a new number of symbols per processing frame
 */
void fsk_set_nsym(struct FSK *fsk,int nsym);

/*
 * Set the minimum and maximum frequencies at which the freq. estimator can find tones
 */
void fsk_set_est_limits(struct FSK *fsk,int fmin, int fmax);

/* 
 * Clear the estimator states
 */
void fsk_clear_estimators(struct FSK *fsk);

/*
 * Set a MODEM_STATS struct in which to deposit demod statistics
 */
void fsk_setup_modem_stats(struct FSK *fsk,struct MODEM_STATS *stats);

/*
 * Destroy an FSK state struct and free it's memory
 * 
 * struct FSK *fsk - FSK config/state struct to be destroyed
 */
void fsk_destroy(struct FSK *fsk);

/*
 * Modulates Nsym bits into N samples
 * 
 * struct FSK *fsk - FSK config/state struct, set up by fsk_create
 * float fsk_out[] - Buffer for N samples of modulated FSK
 * uint8_t tx_bits[] - Buffer containing Nbits unpacked bits
 */
void fsk_mod(struct FSK *fsk, float fsk_out[], uint8_t tx_bits[]);


/*
 * Returns the number of samples needed for the next fsk_demod() cycle
 *
 * struct FSK *fsk - FSK config/state struct, set up by fsk_create
 * returns - number of samples to be fed into fsk_demod next cycle 
 */
uint32_t fsk_nin(struct FSK *fsk);


/*
 * Demodulate some number of FSK samples. The number of samples to be 
 *  demodulated can be found by calling fsk_nin().
 * 
 * struct FSK *fsk - FSK config/state struct, set up by fsk_create
 * uint8_t rx_bits[] - Buffer for Nbits unpacked bits to be written
 * float fsk_in[] - nin samples of modualted FSK
 */
void fsk_demod(struct FSK *fsk, uint8_t rx_bits[],float fsk_in[]);



#endif
