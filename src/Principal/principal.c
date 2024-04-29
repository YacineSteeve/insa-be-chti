#include <stdio.h>
#include "DriverJeuLaser.h"
#include "ServiceJeuLaser.h"
#include "GestionSon.h"
#include "DFT.h"
#include "Signal_4_12.h"

#define M 64

int GestionSon_Index = 0;
int SortieSon = 0;
int bouton = 0;
int d[M];

int main(void) {
	// ===========================================================================
	// ============= INIT PERIPH (faites qu'une seule fois)  =====================
	// ===========================================================================

	/* Après exécution : le coeur CPU est clock� � 72MHz ainsi que tous les timers */
	CLOCK_Configure();

	/* Configuration du son (voir ServiceJeuLaser.h) 
	Ins�rez votre code d'initialisation des parties mat�rielles g�rant le son ....*/	

	
	//ServJeuLASER_Son_Init(91, 0, GestionSon_callback);
	
	//============================================================================	
	for (int k_test = 0; k_test < M; k_test++) {
		d[k_test] = DFT(LeSignal, k_test);
	}
	
	while (1) {
		if(bouton == 1){
			GestionSon_Start();
			bouton = 0;
		}
	}
}
