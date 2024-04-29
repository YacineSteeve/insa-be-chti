#include <stdio.h>
#include "CosSin_Fract_1_15.h"

#define M 64

int DFT(unsigned short int * Sig, int k) {
	int X_re_k = 0x0;  // format 5.11
	int X_im_k = 0x0;  // format 5.11
	int X_re_k_2, X_im_k_2;
	int l;
	
	for (int n = 0; n < M; n++) {
		l = (k * n) % M;
		/*
		Sig[n] -> format 4.12
		TabCos[l] -> format 1.15
		Sig[n] * TabCos[l] -> format 5.27
		*/
		X_re_k += (Sig[n] * TabCos[l]) >> 16;
		/*
		Sig[n] -> format 4.12
		TabSin[l] -> format 1.15
		Sig[n] * TabSin[l] -> format 5.27
		*/
		X_im_k += (Sig[n] * TabSin[l]) >> 16;
	}
	
	/*
	A cause des retenues, après 64 (2^6) itérations, les formats sont plutôt
	X_re_k -> format 11.11 (6.0 + 5.11)
	X_im_k -> format 11.11 (6.0 + 5.11)
	*/
	X_re_k_2 = X_re_k >> 6; // format 11.5
	X_im_k_2 = X_im_k >> 6; // format 11.5
	
	/*
	 X_re_k_2 * X_re_k_2 -> format 22.10
	 X_im_k_2 * X_im_k_2 -> format 22.10
	*/
	return X_re_k_2*X_re_k_2 + X_im_k_2*X_im_k_2;
}
