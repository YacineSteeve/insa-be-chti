	PRESERVE8
	THUMB 
	
	EXPORT GestionSon_callback
	EXPORT GestionSon_Start
	
	IMPORT GestionSon_Index
	IMPORT LongueurSon 
	IMPORT Son
	IMPORT SortieSon
	IMPORT ServJeuLASER_WritePWMSoundVal
		
	
	
; ====================== zone de r�servation de donn�es,  ======================================
;Section RAM (read only) :
	AREA    mesdata,data,readonly


;Section RAM (read write):
	AREA    maram,data,readwrite


; ===============================================================================================



	
;Section ROM code (read only) :		
	AREA    moncode,code,readonly
; �crire le code ici	

GestionSon_Start
    ldr r0, =GestionSon_Index
	mov r1, #0
	str r1, [r0]
	bx lr

GestionSon_callback
	push {lr}
	push {r4-r8}
	
	; Mise � jour index
	ldr r8, =GestionSon_Index ; Met dans r0 l'adresse repr�sent�e par GestionSon_Index
	ldr r1, [r8]              ; Met dans r0 la valeur situ�e � cette adresse
	
	; R�cup�rationde la longueur de Son[]
	ldr r2, =LongueurSon
	ldr r3, [r2]
	; Quitter la fonction si la longueur est atteinte
	cmp r1, r3
	beq exit 
	
	; Sinon, faire la mise � l'�chelle
	ldr r4, =Son
	ldrsh r4, [r4, r1, lsl #1] ; R�cup�rer l'indice GestionSon_Index de Son[]
	mov r5, #32767
	mov r6, #32768
	mov r7, #719
	add r4, r4, r6
	mul r4, r4, r7
	add r5, r5, r6
	udiv r4, r4, r5
	ldr r5, =SortieSon
	str r4, [r5]
	
	; Mettre � jour index
	add r1, #1                ; L'incr�mente de 1
	str r1, [r8]              ; R��crit la nouvelle valeur � l'adresse
	
	mov r0, r4
	
	pop {r4-r8}
	bl ServJeuLASER_WritePWMSoundVal
	pop {pc}
exit
	pop {r4-r8}
	pop {pc}
	bx lr
	

		
	END
