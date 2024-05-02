#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
int a,b,c,d,k = 0, bodovanje = 80


struct Asocijacije{
	char kolonaA[5][16];
	char kolonaB[5][16];
	char kolonaC[5][16];
	char kolonaD[5][16];
	char konacnoResenje[16];
};

//A structure for memorizing field content
struct Asocijacije bazaIgre[5] = {
	{ 
		{"BRAVA","EMISIJA","MATRICA","PRIVEZAK","A:KLJUC"},
		{"VOCE","KUPUS","CEZAR","RUSKA","B:SALATA"},
		{"LJUBAV","VATRA","OBRAZ","STRAST","C:POLJUBAC"},
		{"KAUC","BOLNICA","KREVET","KUGLA","D:LEZAJ"},
		"K:FRANCUSKI"
	},
	{ 
		{"RADILICA","TAKT","KLIP","GLAVA","A:MOTOR"},
		{"PLAZA","TERASA","LETO","KREMA","B:SUNCANJE"},
		{"KVRGE","OLIVA","CRNA","ZELENA","C:MASLINA"},
		{"MORE","UDICA","SOM","MREZA","D:RIBA"},
		"K:ULJE"
	
	},
	{	
		{"HEMIJA","BIOSKOP","DOSTAVA","RED","A:KUCA"},
		{"KRALJICA","ZRNASTI","BELO","MECAVA","B:SNEG"},
		{"POLULOPTA","STENA","CRKVA","TAMBUR","C:KUPOLA"},
		{"POLJUBAC","KAJAK","GRENLAND","PAS","D:ESKIM"},
		"K:IGLO"
	},
	{	
		{"SKOLA","PAUZA","LETO","ZIMA","A:RASPUST"},
		{"MESEC","PRVI","SKUPSTINA","PREVRAT","B:MAJ"},
		{"VRATA","KUCA","TOLERANCIJA","CESKA","C:PRAG"},
		{"TRIVALIC","IME","BOGINJA","ZMIJANAC","D:VESNA"},
		"K:PROLECE"
	},
	{	
		{"PROSTOR","PRLJAV","RUCAK","PONUDA","A:POSAO"},
		{"TENIS","LAMPA","FUDBAL","OKRUGLI","B:STO"},
		{"PRIPRAVNIK","PUNOMOC","KOMORA","DJAVO","C:ADVOKAT"},
		{"POSLANIK","PESMA","BANKA","POSLOVICA","D:NAROD"},
		"K:PROLECE"
	}
	   
};
void prikaziAsocijaciju(int *otvorenaPolja, struct Asocijacije igra, int duzinaNiza);
void prikaziPolje(char *vrednost);
int daLiSadrzi(int *niz, int vrednost, int size);
int validnoPolje(char *izborPolja);
int brojOdPolja(char *niz);
int validnoResenjeZaKolonu(char *niz);
int cekirajResenje(int kolona, struct Asocijacije igra, char *odgovor);
char pokreniIgru();
void prikaziResenje(char *konacno);
void kapitalizacija(char *unos);
void prikaziUputstva();
void ispisiBodoveIzDatoteke();

int main(){
	char pocetak; // variable for entering the character for the start of the game
	char izborPolja[20]; // string entered by the user
	struct Asocijacije igra = bazaIgre[0]; // initializing a game variable of type struct association and assigning a value to it
	int *nizOtvorenihPolja, i; // a dynamic array that counts the fields the user has opened
	int kraj = 0; // variable to mark end (association hit)
	int brojPartija = 0; // counting the played games in order to move on to the next one
	char c, ispis[] = "Osvojeni bodovi su: ";
	FILE *datotekaBodova = fopen("bodovi.txt", "a+");
   
	if(datotekaBodova == NULL) {
		printf("\nOtvaranje datoteke za upis nije uspelo.\n");
		exit(1);
    }

	//INTRODUCTION
	printf("Zelite li da pokrenete igru (D-da, N-ne)? \n");
	pocetak = pokreniIgru();
	if(toupper(pocetak) == 'N')
		exit(1);
	prikaziUputstva();

	//START OF THE GAME
	i=1; 

	nizOtvorenihPolja = (int *)malloc(i*sizeof(int)); 
	if(nizOtvorenihPolja == NULL){
		printf("\nMemorija nije uspesno dodeljena\n");
		exit(1);
	}
	printf("\n");

	prikaziAsocijaciju(nizOtvorenihPolja, igra, sizeof(nizOtvorenihPolja));

	while(1){
		if(i == 1) 
			nizOtvorenihPolja = (int *)malloc(i*sizeof(int)); // array allocation to count open fields
		else 
			nizOtvorenihPolja = (int *)realloc(nizOtvorenihPolja, i*sizeof(int)); // array reallocation after each new user input
		printf("\nUnesite vas izbor: \n");
		fflush(stdin);		
		gets(izborPolja);
		kapitalizacija(izborPolja);

		//the program works by checking whether the user entered a valid format
		if(validnoPolje(izborPolja) == 1){
			//if this is the case, it is important to check whether it has already been entered,
			//ie whether that value already exists in the string, so that the length of the string does not 
			//enlarge unnecessarily
			if(daLiSadrzi(nizOtvorenihPolja, brojOdPolja(izborPolja), i) == 0){
					//the array of open fields gets index from the entered field,
					nizOtvorenihPolja[i-1] = brojOdPolja(izborPolja);
					i++;
					bodovanje -= 5;
			}
		//if a valid field opening format is not entered, we check if it is
		//entered a valid format for entering the final solution of the column
		}else if(validnoResenjeZaKolonu(izborPolja)){
			//if this is the case, we check again whether the user has already entered the same solution
			//as well as whether that solution is correct
			if(daLiSadrzi(nizOtvorenihPolja, brojOdPolja(izborPolja), i) == 0
				&& proveriResenje(brojOdPolja(izborPolja), igra, izborPolja)){
					//here we check whether the user has entered the final solution
					//so we know when to restart the program
					if(proveriResenje(brojOdPolja(izborPolja), igra, izborPolja) == 2){
						kraj = 1;
					}

				nizOtvorenihPolja[i-1] = brojOdPolja(izborPolja);
				i++;
			}
		}

		//for a visually nicer appearance, the entire association is displayed after each selection
		prikaziAsocijaciju(nizOtvorenihPolja, igra, i);

		//if the user has entered a solution
		if(kraj == 1){
			rewind(datotekaBodova);
			fprintf(datotekaBodova, "%d. %d\n", brojPartija+1, bodovanje);
			i=1;
			//we move on to the next game
			brojPartija++;
			printf("\nPogodili ste konacno resenje!\nOsvojeni bodovi: %d/100\n", bodovanje);
			printf("\nDa li zelite da zapocnete novu igru (D-da, N-ne)?\n");
			pocetak = pokreniIgru();
			if(toupper(pocetak) == 'D'){
				//we reset the values ​​so that the user can play a new game from the beginning
				bodovanje = 80;
				a=0;
				b=0;
				c=0;
				d=0;
				k=0;
				kraj = 0;
				igra = bazaIgre[brojPartija];
				nizOtvorenihPolja = (int *)malloc(i*sizeof(int)); 
				if(nizOtvorenihPolja == NULL){
					printf("\nMemorija nije uspesno dodeljena\n");
					exit(1);
				}
				printf("\n");
				prikaziAsocijaciju(nizOtvorenihPolja, igra, sizeof(nizOtvorenihPolja));
			}else if(toupper(pocetak) == 'N')
				 kraj = 2;
		}
		if(kraj == 2){
			printf("\nOsvojeni bodovi po partijama: \n");
			rewind(datotekaBodova);
			while((c=getc(datotekaBodova)) != EOF)
				putchar(c);
			printf("\nHvala na igri! \n");
			exit(1);
		}
	}
	
	return 0;
}

//introducing the user to the instructions and the format they need to fill out
void prikaziUputstva(){
	printf("\nPRAVILA IGRE\n");

	printf("\nIgra asocijacije se sastoji od 4 kolone po 4 polja. \n");
	printf("Da biste uneli resenje za odredjenu kolonu morate otvoriti bar jedno polje iste.\n");
	printf("Takodje, da biste uneli konacno resenje asocijacije, morate pogoditi resenje bar jedne kolone.\n");

	printf("\nUPUTSTVO\n");
	printf("\nFormat za otvaranje jednog polja - 'A1, B2, C4...'\n");
	printf("Format za pogadjanje resenja kolone - 'A:resenje'\n");
	printf("Format za pogadjanje konacnog resenja - 'K:resenje'\n");
	printf("Pisite 'celavom' latinicom i onako kako je gore navedeno!");

} 

// return an error to the user if he does not enter the correct answer
char pokreniIgru(){
	char karakter;

	do{
		fflush(stdin);
		karakter = getchar();
		if(toupper(karakter) != 'N' && toupper(karakter) != 'D') 
			printf("\nNiste uneli validnu vrednost.\n");
	}while(toupper(karakter) != 'N'&& toupper(karakter) != 'D');

	return karakter;
} 

//function that prints the fields, checks if the index of the field is entered in the array, if so it prints the field
//otherwise, prints the field name
//variables a,b,c,d are entered to open all fields after the hit column
void prikaziAsocijaciju(int *otvorenaPolja, struct Asocijacije igra, int duzinaNiza){
		int i;
		
		for(i=0; i< 60; i++) {
			printf("-");
		}
		printf("\n");

		if(daLiSadrzi(otvorenaPolja, 0, duzinaNiza)== 1 || a==1 || k==1)
			prikaziPolje(igra.kolonaA[0]);
		else 
			prikaziPolje("A1");

		if(daLiSadrzi(otvorenaPolja, 1, duzinaNiza)== 1 || b==1 || k==1) 
			prikaziPolje(igra.kolonaB[0]);
		else 
			prikaziPolje("B1");

		if(daLiSadrzi(otvorenaPolja, 2, duzinaNiza)== 1|| c==1 || k==1) 
			prikaziPolje(igra.kolonaC[0]);
		else 
			prikaziPolje("C1");

		if(daLiSadrzi(otvorenaPolja, 3, duzinaNiza)== 1 || d==1 || k==1) 
			prikaziPolje(igra.kolonaD[0]);
		else 
			prikaziPolje("D1");
	
		printf("\n");
		for(i=0; i< 60; i++) {
			printf("-");
		}
		printf("\n");

		if(daLiSadrzi(otvorenaPolja, 4, duzinaNiza)== 1 || a==1 || k==1) 
			prikaziPolje(igra.kolonaA[1]);
		else 
			prikaziPolje("A2");

		if(daLiSadrzi(otvorenaPolja, 5, duzinaNiza)== 1 || b==1 || k==1) 
			prikaziPolje(igra.kolonaB[1]);
		else 
			prikaziPolje("B2");

		if(daLiSadrzi(otvorenaPolja, 6, duzinaNiza)== 1 || c==1 || k==1) 
			prikaziPolje(igra.kolonaC[1]);
		else 
			prikaziPolje("C2");

		if(daLiSadrzi(otvorenaPolja, 7, duzinaNiza)== 1 || d==1 || k==1) 
			prikaziPolje(igra.kolonaD[1]);
		else 
			prikaziPolje("D2");
	
		printf("\n");
		for(i=0; i< 60; i++) {
			printf("-");
		}
		printf("\n");


		if(daLiSadrzi(otvorenaPolja, 8, duzinaNiza)== 1 || a==1 || k==1) 
			prikaziPolje(igra.kolonaA[2]);
		else 
			prikaziPolje("A3");

		if(daLiSadrzi(otvorenaPolja, 9, duzinaNiza)== 1 || b==1 || k==1) 
			prikaziPolje(igra.kolonaB[2]);
		else 
			prikaziPolje("B3");

		if(daLiSadrzi(otvorenaPolja, 10, duzinaNiza)== 1 || c==1 || k==1) 
			prikaziPolje(igra.kolonaC[2]);
		else 
			prikaziPolje("C3");

		if(daLiSadrzi(otvorenaPolja, 11, duzinaNiza)== 1 || d==1 || k==1) 
			prikaziPolje(igra.kolonaD[2]);
		else 
			prikaziPolje("D3");
	
		printf("\n");
		for(i=0; i< 60; i++) {
			printf("-");
		}
		printf("\n");


		if(daLiSadrzi(otvorenaPolja, 12, duzinaNiza)== 1 || a==1 || k==1) 
			prikaziPolje(igra.kolonaA[3]);
		else 
			prikaziPolje("A4");

		if(daLiSadrzi(otvorenaPolja, 13, duzinaNiza)== 1 || b==1 || k==1) 
			prikaziPolje(igra.kolonaB[3]);
		else 
			prikaziPolje("B4");

		if(daLiSadrzi(otvorenaPolja, 14, duzinaNiza)== 1 || c==1 || k==1) 
			prikaziPolje(igra.kolonaC[3]);
		else 
			prikaziPolje("C4");

		if(daLiSadrzi(otvorenaPolja, 15, duzinaNiza)== 1 || d==1 || k==1) 
			prikaziPolje(igra.kolonaD[3]);
		else 
			prikaziPolje("D4");

		printf("\n");
		for(i=0; i< 60; i++) {
			printf("-");
		}
		printf("\n");
	
		if(daLiSadrzi(otvorenaPolja, 16, duzinaNiza)== 1 || k==1)
			prikaziPolje(igra.kolonaA[4]);
		else 
			prikaziPolje("A");

		if(daLiSadrzi(otvorenaPolja, 17, duzinaNiza)== 1 || k==1) 
			prikaziPolje(igra.kolonaB[4]);
		else 
			prikaziPolje("B");

		if(daLiSadrzi(otvorenaPolja, 18, duzinaNiza)== 1 || k==1) 
			prikaziPolje(igra.kolonaC[4]);
		else 
			prikaziPolje("C");

		if(daLiSadrzi(otvorenaPolja, 19, duzinaNiza)== 1 || k==1) 
			prikaziPolje(igra.kolonaD[4]);
		else 
			prikaziPolje("D");

		printf("\n");
		for(i=0; i< 60; i++) {
			printf("-");
		}
		printf("\n");

		if(daLiSadrzi(otvorenaPolja, 20, duzinaNiza)) 
			prikaziResenje(igra.konacnoResenje);
		else 
			prikaziResenje("Konacno");

		printf("\n");
		for(i=0; i< 60; i++) {
			printf("-");
		}
		printf("\n");

};

//function to display the fields in the given frame
//display by using the length of the string to print the string
//an empty field is printed in the suptor
//i == 14 represents the end of the field length
void prikaziPolje(char *polje) {
	int i;
	for(i = 0; i < 15; i++) { // 15 - odredjena duzina jednog polja 
		if(i < strlen(polje)) { 
			printf("%c", polje[i]); 
		}else if(i == 14){
			printf("|");
		}else{
			printf(" "); 
		}
	}
}

// passing through all members of the array, we check whether the array contains the entered value
int daLiSadrzi(int *nizOtvorenaPolja, int indeksPolja, int duzinaNiza) {
	int i;

	for (i=0; i < duzinaNiza; i++) { // proveravanje da li se u nizu otvorenih polja vec nalazi uneta vrednost
		if(nizOtvorenaPolja[i] == indeksPolja) 
			return 1;
	}
	return 0;
}

//function that checks if the input format is valid
int validnoPolje(char *izborPolja) {
	char *c = izborPolja;

	if(	strcmp(c, "A1") == 0 ||
		strcmp(c, "B1") == 0 ||
		strcmp(c, "C1") == 0 || 
		strcmp(c, "D1") == 0 || 
		strcmp(c, "A2") == 0 || 
		strcmp(c, "B2") == 0 || 
		strcmp(c, "C2") == 0 || 
		strcmp(c, "D2") == 0 ||
		strcmp(c, "A3") == 0 ||
		strcmp(c, "B3") == 0 || 
		strcmp(c, "C3") == 0 ||
		strcmp(c, "D3") == 0 || 
		strcmp(c, "A4") == 0 || 
		strcmp(c, "B4") == 0 || 
		strcmp(c, "C4") == 0 || 
		strcmp(c, "D4") == 0 )
		return 1;
	else{
		return 0;
	}
}

//after checking whether the format is valid, this function is used to write the number from the field into the string
//for easier work with integers, the function is done this way
int brojOdPolja(char *niz){

	if(strcmp(niz, "A1") == 0){
		return 0;
	}else if(strcmp(niz, "B1") == 0){
		return 1;
	}else if(strcmp(niz, "C1") == 0){
		return 2;
	}else if(strcmp(niz, "D1") == 0){
		return 3;
	}else if(strcmp(niz, "A2") == 0){
		return 4;
	}else if(strcmp(niz, "B2") == 0){
		return 5;
	}else if(strcmp(niz, "C2") == 0){
		return 6;
	}else if(strcmp(niz, "D2") == 0){
		return 7;
	}else if(strcmp(niz, "A3") == 0){
		return 8;
	}else if(strcmp(niz, "B3") == 0){
		return 9;
	}else if(strcmp(niz, "C3") == 0){
		return 10;
	}else if(strcmp(niz, "D3") == 0){
		return 11;
	}else if(strcmp(niz, "A4") == 0){
		return 12;
	}else if(strcmp(niz, "B4") == 0){
		return 13;
	}else if(strcmp(niz, "C4") == 0){
		return 14;
	}else if(strcmp(niz, "D4") == 0){
		return 15;
	}else if(toupper(niz[0]) == 'A'){
		return 16;
	}else if(toupper(niz[0]) == 'B'){
		return 17;
	} else if(toupper(niz[0]) == 'C'){
		return 18;
	} else if(toupper(niz[0]) == 'D'){
		return 19;
	} else if(toupper(niz[0]) == 'K'){
		return 20;
	}
}


//a function that checks if the column solution input format is entered
//if it is and if it is valid, we go to the next function that serves to check the result
int validnoResenjeZaKolonu(char *niz) {

	if(	(toupper(niz[0]) == 'A' && niz[1] == ':') ||
		(toupper(niz[0]) == 'B' && niz[1] == ':') ||
		(toupper(niz[0]) == 'C' && niz[1] == ':') ||
		(toupper(niz[0]) == 'D' && niz[1] == ':') ||
		(toupper(niz[0]) == 'K' && niz[1] == ':') ) 
		return 1;
	else{
		printf("\nNije unet validan format za konacno resenje kolone.\n");
		return 0;
	}
}


//if the user tried to enter the final solution of the column,
//(we check this by checking whether the number of the given field is in the string)
//we check if the entered solution is correct
int proveriResenje(int kolona, struct Asocijacije igra, char *odgovor){

	if (kolona == 16) {
		if (strcmp(igra.kolonaA[4], odgovor) == 0){
			a=1;
			bodovanje += 4;
			return 1;
		}
	}else if (kolona == 17) {
		if (strcmp(igra.kolonaB[4], odgovor) == 0){
			bodovanje += 4;
			b=1;
			return 1;
		}
	}else if (kolona == 18) {
		if (strcmp(igra.kolonaC[4], odgovor) == 0){
			bodovanje += 4;
			c=1;
			return 1;
		}
	}else if (kolona == 19){
		if (strcmp(igra.kolonaD[4], odgovor) == 0){
			bodovanje += 4;
			d=1;
			return 1;
		}
	}else if (kolona == 20){
		if (strcmp(igra.konacnoResenje, odgovor) == 0){
			bodovanje += 4;
			k=1;
			return 2;
		}
	}
	return 0;
} 

//ova funkcija je bila potrebna, jer se format stampanja konacnog resenje 
//ralikuje od formata stampanja svakog polja 
void prikaziResenje(char *konacno){
	int i;
	int duzina = strlen(konacno);

	for(i=0;i<25;i++){
		printf(" ");
	}
	for(i=25;i<25+duzina;i++){
		printf("%c", konacno[i-25]);
	}
	for(i=25+duzina;i<60;i++){
		if(i==59) printf("|");
		else printf(" ");
	}	
	
}

//in order to avoid constant capitalization of letters throughout the code
//or write double code for certain functions
//all user input is capitalized immediately after input
void kapitalizacija(char *unos){
	int i; 

	for(i=0; i<strlen(unos); i++)
		unos[i] = toupper(unos[i]);
		
}





