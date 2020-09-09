#include "attack.h"


/**
 * Pearson correlation between two conso models
 * @param tableauX : first consumption model (here, Hamming weigth)
 * @param traces : second consumption model (here, power consommation)
 * @param instant : instant in the trace where catching correlation
 * @param tailleTableau : length of tableauX and traces
 * 
*/
double correlationPearson(double tableauX[],double ** traces, int instant, int tailleTableau)
{
	double sommeXY=0; //somme des Xi*yi
	double sommeX=0;//somme des Xi
	double sommeY=0;//somme des Yi
	double sommeXiCarre=0;//somme des xi carré
	double sommeYiCarre=0;
	int i=0;
	
	for( i=0; i<tailleTableau; i++)
	{
		sommeX+=tableauX[i];
		
		sommeY+=(double)traces[i][instant];
		sommeXiCarre+=pow(tableauX[i], 2.0);
		sommeYiCarre+=pow((double)traces[i][instant], 2.0);
		sommeXY=sommeXY+(tableauX[i]*(double)traces[i][instant]);
	}
	
	double r=((tailleTableau*sommeXY)-sommeX*sommeY)/sqrt((tailleTableau*sommeXiCarre-pow(sommeX,2.0))*(tailleTableau*sommeYiCarre-pow(sommeY,2.0)));
	if(r < 0){
		return r * (-1);
	}
	return r;
}

/**
 * Compute the max of the perason correlation for one of the 256 possibilities of a key byte k[ind_byte]
 * @param key_byte : the possibilty (from 0 to 255)
 * @param plaintexts : all available plaintexts
 * @param ind_byte : the index of the byte we are searching (from 0 to 15)
 * @param traces : consumption traces
 * 
*/
double correlation_max_key_byte(int key_byte,uint8_t** plaintexts, int ind_byte,double** traces){
	double distance[NB_TRACES];
	
	int tmp;
	for(int i=0;i<NB_TRACES;i++){

		tmp=key_byte ^ plaintexts[i][ind_byte];
		distance[i]=hamming_weight(sbox[tmp]);
	}
	
	double corrcoeffs[NB_SAMPLES];

	for(int i=0;i<NB_SAMPLES;i++){
		corrcoeffs[i]=correlationPearson(distance,traces, i,NB_TRACES);
	}
	return maxValue(corrcoeffs,NB_SAMPLES);

}


void cpa_attack(uint8_t ** plain_texts, double ** traces, int * key_estimation){
	double ** corrcoeffs = (double **)malloc(NB_SUBKEYS * sizeof(double *));
	for(int i=0 ; i< NB_SUBKEYS; i++){
		corrcoeffs[i] = (double *)malloc(POSSIBILITES * sizeof(double));
	}
	
	//The CPA attack
	//for all bytes (from 0 to 15)
	for(int ind_byte=0; ind_byte< NB_SUBKEYS; ind_byte++){
		printf("start subkey %d\t", ind_byte);
		fflush(stdout);
		
		//for all possibilities (from 0 to 255)
		for(int i=0;i< POSSIBILITES;i++){
			//computing the Pearson correlation between Hamming weight and true power consumption (traces)
			corrcoeffs[ind_byte][i] = correlation_max_key_byte(possible_keys[i], plain_texts, ind_byte, traces);
		}
		
		printf("fini subkey %d\n", ind_byte);
	}
	
	for(int i=0;i<NB_SUBKEYS;i++){
		key_estimation[i] = possible_keys[ArgMax(corrcoeffs[i], POSSIBILITES)];		
		free(corrcoeffs[i]);
	}
	free(corrcoeffs);
}

