#include "attack.h"


/**
 * Give the ind_bith-th bit of an integer
 * @param p the integer to get bit
 * @param ind_bit index of the bit to get
 * @return the specific bit
*/
int get_bit(uint8_t p, int ind_bit){
	return (p & ( 1 << ind_bit )) >> ind_bit;
}

/**
 * Add element of t at the the index i to element of g at index i
 * @param g set of elements where to add
 * @param t set element to add
*/
void add(double * g, double * t){
	for(int i=0; i< NB_SAMPLES; i++){
		g[i] += t[i];
	}
}

/**
 * Compute the average of all elements of a set g
 * @param g set of integer
 * @param nb number of element of g
*/
void average(double * g, int nb){
	for(int i=0; i<NB_SAMPLES; i++){
		g[i] = (double)(g[i] / (double)(nb));
	}
}

/**
 * Compute the difference between two set of integers
 * @param g1 first set of integers
 * @param g2 second set of integers
 * @param res adress to stock result
 * 
*/
void diff(double * g1, double * g2, double * res){
	for(int i=0; i< NB_SAMPLES; i++){
		res[i] = fabs(g1[i] - g2[i]);
	}
}


/**
 * Implementation of the DPA attack
 * @param plain_texts set of the plaintexts
 * @param traces set of the traces
 * @param key_estimation estimation of the key
 * @param ind_bit the bit to attack
*/
void dpa_attack(uint8_t ** plain_texts, double ** traces, int * key_estimation, int ind_bit){
	// Memory allocations
	double * g1 = (double *)malloc(NB_SAMPLES * sizeof(double));
	double * g2 = (double *)malloc(NB_SAMPLES * sizeof(double));
	double * res = (double *)malloc(NB_SAMPLES * sizeof(double));
	double * diffs = (double *)malloc(POSSIBILITES * sizeof(double));

	int nb1=0, nb2=0;

	// Initialisation of sets to zero
	memset(g1, 0, NB_SAMPLES * sizeof(double));
	memset(g2, 0, NB_SAMPLES * sizeof(double));
	memset(res, 0, NB_SAMPLES * sizeof(double));
	memset(diffs, 0, POSSIBILITES * sizeof(double));
	int out;
	
	for(int ind_byte=0; ind_byte< NB_SUBKEYS; ind_byte++){
		printf("start subkey %d\t", ind_byte);
		fflush(stdout);
		
		for(int possible=0; possible<POSSIBILITES; possible++){
			
			for(int i=0; i<NB_TRACES; i++){
				
				out = sbox[possible_keys[possible] ^ plain_texts[i][ind_byte]];
				
				if(get_bit(out, ind_bit) == 1){
					add(g1, traces[i]);
					nb1++;
				}
				else{
					add(g2, traces[i]);
					nb2++;
				}
			}
			
			if(nb1 != 0){
				average(g1, nb1);
			}
			if(nb2 != 0){
				average(g2, nb2);
			}
			
			diff(g1, g2, res);

			diffs[possible] = maxValue(res, NB_SAMPLES);

			
			memset(g1, 0, NB_SAMPLES * sizeof(double));
			memset(g2, 0, NB_SAMPLES * sizeof(double));
			memset(res, 0, NB_SAMPLES * sizeof(double));
			nb1 = 0; nb2 = 0;
		}
		
		key_estimation[ind_byte] = possible_keys[ArgMax(diffs, POSSIBILITES)];
		memset(diffs, 0, POSSIBILITES * sizeof(double));
		printf("fini subkey %d\n", ind_byte);
	}
	
	free(g1); free(g2); free(diffs); free(res);
	
}
