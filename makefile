Flags=-Wall 
projet : main.c fonctionsAFN.c fonctionsAFN.h fonctionsAFNevoluees.c fonctionsAFNevoluees.h fonctionsAFD.c fonctionsAFD.h 
	gcc -o executable main.c fonctionsAFN.c fonctionsAFNevoluees.c fonctionsAFD.c $(Flags)
