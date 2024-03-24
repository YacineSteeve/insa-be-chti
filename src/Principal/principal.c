#include <stdio.h>
#include "DriverJeuLaser.h"
#include "ServiceJeuLaser.h"
#include "GestionSon.h"

int GestionSon_Index = 0;
int SortieSon = 0;
int bouton = 0;

int main(void) {
	// ===========================================================================
	// ============= INIT PERIPH (faites qu'une seule fois)  =====================
	// ===========================================================================

	/* Apr�s ex�cution : le coeur CPU est clock� � 72MHz ainsi que tous les timers */
	CLOCK_Configure();

	/* Configuration du son (voir ServiceJeuLaser.h) 
	Ins�rez votre code d'initialisation des parties mat�rielles g�rant le son ....*/	

	
	ServJeuLASER_Son_Init(91, 0, GestionSon_callback);
	
	//============================================================================	
		
	while (1) {
		if(bouton == 1){
			GestionSon_Start();
			bouton = 0;
		}
	}
}
