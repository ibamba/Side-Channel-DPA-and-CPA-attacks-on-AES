#include "attack.h"


int main(int argc, char ** argv){
	
	int fd;
	int i;
	
	if(argc < 5){
		printf("usage : main knownkeys_file plaintexts_file traces_file CPA/DPA [ind_bit (if attack is DPA, ind_bit in [0;7] is to partition traces)]\n");
		printf("Example (CPA): main knownkeys.raw plaintexts.raw traces.raw CPA\n");
		printf("Example (DPA): main knownkeys.raw plaintexts.raw traces.raw DPA 4\n\n");
		exit(EXIT_FAILURE);
	}
	
	//Reading Keys
	printf("Reading Key\n");
	if ((fd = open(argv[1], O_RDONLY)) == -1) {
		perror("open");
		return -1;
	}
	uint8_t * known_key = (uint8_t *)malloc(NB_SUBKEYS * sizeof(uint8_t));
	if (read(fd, known_key, sizeof(uint8_t) * NB_SUBKEYS) == -1) {
		perror("read");
		return -1;
	}
	//End Keys
	
	
	//Reading Plaintexts
	printf("Reading Plaintexts\n");
	if ((fd = open(argv[2], O_RDONLY)) == -1) {
		perror("open");
		return -1;
	}
	uint8_t ** plain_texts = (uint8_t **)malloc(NB_TRACES * sizeof(uint8_t *));
	for(i = 0; i < NB_TRACES; i++){
		plain_texts[i] = (uint8_t *)malloc(NB_SUBKEYS * sizeof(uint8_t));
		
		if (read(fd, plain_texts[i], sizeof(uint8_t) * NB_SUBKEYS) == -1) {
			perror("read");
			return -1;
		}
	}
	//End plaintexts
	
	//Reading Traces
	printf("Reading traces\n");
	if ((fd = open(argv[3], O_RDONLY)) == -1) {
		perror("open");
		return -1;
	}
	double * trace_tmp = (double *)malloc(SIZE_TRACE * sizeof(double));
	
	double ** traces = (double **)malloc(NB_TRACES * sizeof(double *));
	for(i = 0; i < NB_TRACES; i++){
		traces[i] = (double *)malloc(NB_SAMPLES* sizeof(double));
		
		if (read(fd, trace_tmp, sizeof(double) * SIZE_TRACE) == -1) {
			perror("read");
			return -1;
		}
		
		memcpy(traces[i], trace_tmp+SUBINTERVAL1, NB_SAMPLES*sizeof(double));
	}
	free(trace_tmp);
	//End Traces	
	printf("Success Reading Input Data\n\n");
	printf("NBTRACES = %d, NB_SAMPLES = %d\n", NB_TRACES, NB_SAMPLES);
	
	
	int* key_estimation=(int*)malloc(NB_SUBKEYS*sizeof(int));
	clock_t debut, fin;
	debut = clock();
	if(strcmp(argv[4], "CPA") == 0){
		cpa_attack(plain_texts, traces, key_estimation);
	}
	else if(strcmp(argv[4], "DPA") == 0){
		if(argc <6){
			printf("You have to input ind_bit to use to partition traces (value from 0 to 7)\n");
			goto err;
		}
		int ind_bit = atoi(argv[5]);
		if((ind_bit<0) || (ind_bit > 7)){
			printf("Invalid ind_bit to partition traces\n");
			goto err;
		}
		dpa_attack(plain_texts, traces, key_estimation, ind_bit);
	}
	else{
		printf("Invalid Attack, Ending Program\n");
		goto err;
	}
	fin = clock();
	printf("\n%s Time = %lf sec\n", argv[4], (double)((fin - debut)/ (double)CLOCKS_PER_SEC));
	
	//printing subkeys
	printf("predict key = [ ");
	for(int i=0;i<NB_SUBKEYS;i++){
		printf("%d, ", key_estimation[i]);
	}
	printf(" ]\n");
	
	printf("correct key = [ ");
	for(int i=0;i<NB_SUBKEYS;i++){
		printf("%d, ", known_key[i]);
	}
	printf(" ]\n");
	
	int num = 0;
	for(int i=0;i<NB_SUBKEYS;i++){
		if(known_key[i] == key_estimation[i]){
			num++;
		}
	}
	printf("\nPredicted %d Subkeys Correctly\n", num);
	
	err:
	for(i = 0; i < NB_TRACES; i++){
		free(plain_texts[i]);
		free(traces[i]);
	}
	free(known_key);
	free(key_estimation);
	
	return 0;
}
