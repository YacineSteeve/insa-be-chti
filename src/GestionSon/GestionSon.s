	PRESERVE8
	THUMB 
	
	EXPORT GestionSon_callback
	EXPORT GestionSon_Start
	
	IMPORT GestionSon_Index
	IMPORT LongueurSon 
	IMPORT Son
	IMPORT SortieSon
	IMPORT ServJeuLASER_WritePWMSoundVal
		
	
	
; ====================== zone de réservation de données,  ======================================
;Section RAM (read only) :
	AREA    mesdata,data,readonly


;Section RAM (read write):
	AREA    maram,data,readwrite


; ===============================================================================================



	
;Section ROM code (read only) :		
	AREA    moncode,code,readonly
; écrire le code ici	

GestionSon_Start
    ldr r0, =GestionSon_Index
	mov r1, #0
	str r1, [r0]
	bx lr

GestionSon_callback
	push {lr}
	push {r4-r8}
	
	; Mise à jour index
	ldr r8, =GestionSon_Index ; Met dans r0 l'adresse représentée par GestionSon_Index
	ldr r1, [r8]              ; Met dans r0 la valeur située à cette adresse
	
	; Récupérationde la longueur de Son[]
	ldr r2, =LongueurSon
	ldr r3, [r2]
	; Quitter la fonction si la longueur est atteinte
	cmp r1, r3
	beq exit 
	
	; Sinon, faire la mise à l'échelle
	ldr r4, =Son
	ldrsh r4, [r4, r1, lsl #1] ; Récupérer l'indice GestionSon_Index de Son[]
	mov r5, #32767
	mov r6, #32768
	mov r7, #719
	add r4, r4, r6
	mul r4, r4, r7
	add r5, r5, r6
	udiv r4, r4, r5
	ldr r5, =SortieSon
	str r4, [r5]
	
	; Mettre à jour index
	add r1, #1                ; L'incrémente de 1
	str r1, [r8]              ; Réécrit la nouvelle valeur à l'adresse
	
	mov r0, r4
	
	pop {r4-r8}
	bl ServJeuLASER_WritePWMSoundVal
	pop {pc}
exit
	pop {r4-r8}
	pop {pc}
	bx lr
	

		
	END
