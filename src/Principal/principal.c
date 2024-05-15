#include <stdio.h>
#include "DriverJeuLaser.h"
#include "ServiceJeuLaser.h"
#include "GestionSon.h"
#include "DFT.h"
// #include "Affichage_Valise.h"

#define M 64
#define FREQ_NUM 6

int GestionSon_Index = 0;
int SortieSon = 0;
int bouton = 1;
int base_freq = 5000; 
int freq_indexes[FREQ_NUM] = {17, 18, 19, 20, 23, 24};
int scores[FREQ_NUM] = {0};
int touched = 0;
int timer = 0;

int interruption_period = 5000; // µs
short int signal[M];
int d[M];
int last_freq_captured = 0;
int last_freq_apparition_count = 0;

int high_index(int * dft) {
	int curr_val, max_val, max_index;
		
	for (int i = 1; i < M - 1; i++) {
		curr_val = dft[i]; 
		
		if (curr_val > max_val) {
			max_val = curr_val;
			max_index = i;
		}
	}
	
	return max_index;
}

int get_index_of_freq(int freq) {
	int index = -1;
	
	for (int i= 0; i< FREQ_NUM; i++) {
		if (freq == freq_indexes[i]) {
			index = i;
		}
	}
	
	return index;
}

void mesures_and_dft(void) {
	// Mesure du signal
	ServJeuLASER_StartDMA();
	
	if (touched == 0) {
		// Calcul de la DFT
		for (int k = 0; k < M; k++) {
			d[k] = DFT((unsigned short int *)signal, k);
		}
		
		int freq_captured = high_index(d);
		int freq_captured_index;
		
		if (freq_captured == last_freq_captured) {
			last_freq_apparition_count++;
			
			if (last_freq_apparition_count == 4) {
				freq_captured_index = get_index_of_freq(freq_captured);
				
				if (freq_captured_index != -1) {
					scores[freq_captured_index] += 1;
					touched = 1;
					GestionSon_Start();
				}
				
				last_freq_apparition_count=0;				
			}
		} else {
			last_freq_apparition_count = 0;
			last_freq_captured=freq_captured;
		}
	}
	
	if (timer < 500) {
		timer++;
	} else {
		timer = 0;
		touched = 0;
	}
}

int main(void) {
	// ===========================================================================
	// ============= INIT PERIPH (faites qu'une seule fois)  =====================
	// ===========================================================================

	/* Après exécution : le coeur CPU est clock� � 72MHz ainsi que tous les timers */
	CLOCK_Configure();

	/* Configuration du son (voir ServiceJeuLaser.h) 
	Ins�rez votre code d'initialisation des parties mat�rielles g�rant le son ....*/	

	// Init_Affichage();
	
	ServJeuLASER_Son_Init(91, 1, GestionSon_callback);
	
	ServJeuLASER_ADC_DMA(signal);
	
	ServJeuLASER_Systick_IT_Init(interruption_period, 0, mesures_and_dft);
	
	//============================================================================	
	
	
	while (1) {
	}
}
