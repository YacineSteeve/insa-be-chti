#include <stdio.h>
#include <stdlib.h>
#include "DriverJeuLaser.h"
#include "ServiceJeuLaser.h"
#include "GestionSon.h"
#include "DFT.h"
#include "Affichage_Valise.h"

#define M 64
#define FREQ_NUM 4

int GestionSon_Index = 0;
int SortieSon = 0;
int bouton = 1;
int base_freq = 5000; 
int freq_indexes[FREQ_NUM] = {17, 18, 19, 20}; //, 23, 24};
int scores[FREQ_NUM] = {0};
char afficheurs[FREQ_NUM] = {'1', '2', '3', '4'};
int touched = 0;
int timer = 0;

int interruption_period = 5000; // µs
short int signal[M];
int d[M];
int last_freq_captured = 0;
int last_freq_apparition_count = 0;
int random = 0;

void get_random_target() {
	Prepare_Clear_LED(random);
	random = rand()%4;
	Choix_Capteur(random+1);
	Prepare_Set_LED(random);
}

int high_index(int * dft) {
	int curr_val, max_val, max_index;
		
	for (int i = 1; i < M - 1; i++) {
		curr_val = dft[i]; 
		
		if (curr_val > 50 && curr_val > max_val) {
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
		} else {
			last_freq_apparition_count = 0;
			last_freq_captured=freq_captured;
		}
			
		if (last_freq_apparition_count == 4) {
			freq_captured_index = get_index_of_freq(freq_captured);
			
			if (freq_captured_index != -1) {
				scores[freq_captured_index] += 1;
				touched = 1;
				Prepare_Afficheur(freq_captured_index+1, scores[freq_captured_index]);
				get_random_target();
				Mise_A_Jour_Afficheurs_LED();
				GestionSon_Start();
			}
			
			last_freq_apparition_count=0;				
		}
	}else {
		if (timer < 250) {
			timer++;
		} else {
			timer = 0;
			touched = 0;
		}
	}
} 

int main(void) {
	// ===========================================================================
	// ============= INIT PERIPH (faites qu'une seule fois)  =====================
	// ===========================================================================
	srand(rand());
	
	/* Après exécution : le coeur CPU est clock� � 72MHz ainsi que tous les timers */
	CLOCK_Configure();

	/* Configuration de l'affichage */

	Init_Affichage();
	
	for(int i=0; i<FREQ_NUM;  i++){
		Prepare_Afficheur(i+1, 0);
		Prepare_Clear_Point_Unite(i+1);
		Prepare_Clear_LED(i);
	}
	
	get_random_target();
	
	Mise_A_Jour_Afficheurs_LED();
	
	/* Configuration du son  */
	
	ServJeuLASER_Son_Init(91, 1, GestionSon_callback);
	
	/* Configuration de la DFT  */
	
	ServJeuLASER_ADC_DMA(signal);
	
	ServJeuLASER_Systick_IT_Init(interruption_period, 9, mesures_and_dft);
	
	//============================================================================	
	
	
	while (1) {
	}
}
