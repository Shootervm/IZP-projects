#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>

///---------Definicie-pre-jednotlive-mody-vyhladavania------------------
#define ZakladnyMod 0
#define Bodka 1
#define Strieska 2
#define	DvojBodka 3
#define DebugMod 10
///---------------------------------------------------------------------
#define MaxZnakov 80 /// maximalny pocet vypisovanych znakov na slovo

void Ukoncenie(int text);	/// funkcia vypisujuca hlasky z tabulky textov
int Prevod(char* c);		/// prevod string na integer
int Argumenty(int argc); 	/// kontrola poctu argumentov
int KontrolaArgumentov(int argc, char *argv[], int *p_mod, int *p_pozicia_hladaneho_pismena); /// zkontroluje argumenty
int Volitelne_Argumenty(int argc, char *argv[], int *p_mod, int *p_pozicia_hladaneho_pismena);/// zkontroluje volitelne argumenty a nastavi jednotlive priznaky
int Vstup(char *argv[], int pozicia_hladaneho_pismena, int mod); /// praca so vstupom a pocitanie slov
bool Platny_znak(int c_int); ///kontrola platneho znaku


enum msgs
{
	HELP,		/** Help*/
	ERR,		/** Program bude ukonceny.*/
	NArgs,		/** Chybaju argumenty programu.*/
	LArgs,		/** Nespravny pocet argumentov programu.*/
	ArgX,		/** Chybne zadany argument X.*/
	ArgN,		/** Chybne zadany argument N.*/
};

const char *table[]=
{
	/** HELP */
	[HELP]=
	"\nNazov:\tPocitanie slov\n"
	"Autor:\tVojtech Masek\n"
	"Email:\txmasek15@stud.fit.vutbr.cz\n\n"
	"Program analyzuje vstupny text a pocita slova, ktore obsahuju\n"
	"uzivatelom specifikovany znak.\n"
	"Uzivatel specikuje hladany znak argumentom prikazoveho riadku.\n"
	"Je mozne specifikovat aj poziciu hladaneho znaku.\n\n"
	"Syntax spustenia:\t./proj1 --help\n\t\t\t./proj1 X [N] [-d]\n"
	"Priklad pouzitia:\t./proj1 a 2\n\t\t\t./proj1 : 1 -d\n\n"
	"Popis argumentov:\n\n"
	"X\t-reprezentuje hladany znak v slove\n"
	"\t-platnym znakom sa rozumie \"a-z\", \"A-Z\", \"-\" a \"_\"\n"
	"\t-kazdy iny ako platny znak je povazovany za oddelovac slov\n"
	"\t.  -vyhladava lubovolny znak\n"
	"\t:  -vyhladava ciselne znaky  \"0-9\"\n"
	"\t^  -vyhladava lubovolne znaky velkej abecedy,"
	"\n\t   -v pripade Windowsu nutne zadat ^^\n"
	"\nVolitelne argumenty:\n\n"
	"[N]\t-reprezentuje poziciu na ktorej hladame znak\n"
	"\t-velkost je obmedzena na hodnotu INT_MAX\n"
	"[-d]\t-povolenie debug rezimu vypisujuceho jednotlive prehladavane slova\n",
	/** Stavove hlasky */
	[ERR]=	"\nProgram bude ukonceny.\n",
	[NArgs]="\nChybaju argumenty programu.\n",
	[LArgs]="\nNespravny pocet argumentov programu.\n",
	[ArgX]=	"\nChybne zadany argument X.\n",
	[ArgN]=	"\nChybne zadany argument N.\n",
};

int main(int argc, char *argv[])
{
	int mod = ZakladnyMod;
	int pozicia_hladaneho_pismena = 0;

	if (Argumenty(argc) == EXIT_FAILURE || KontrolaArgumentov(argc, &*argv, &mod, &pozicia_hladaneho_pismena) == EXIT_FAILURE)
		{
			Ukoncenie(ERR);
			return EXIT_FAILURE;
		}	
		
	Vstup(argv, pozicia_hladaneho_pismena, mod);
	return EXIT_SUCCESS;
}

int Prevod(char *c) 
{
	char chr;
	int cislo = 0;
	int cislo_pred = 0;
	while((chr = *c++) != 0) 
    {
        if(chr >= '0' && chr <= '9') /// podmienka pre platny ciselny znak 
			cislo = cislo * 10 + chr- '0';
		else
		{
			Ukoncenie(ArgN);
			return -1; /// pokial sa najde v retazci okrem cisla aj znak vracia "-1"
		}
		if(cislo >= cislo_pred) /// kontrola ci nenastalo pretecenie
			cislo_pred = cislo;
		else
		{
			Ukoncenie(ArgN);
			return -1; /// pokial je prevadzane cislo vecsie ako INT_MAX vracia "-1"
		}
    }
	return cislo_pred; 
}

int Argumenty(int argc)
	{
	if (argc == 1)
		{
		Ukoncenie(NArgs);
		return EXIT_FAILURE;
		}
	else if (argc >= 5)
		{
		Ukoncenie(LArgs);
		return EXIT_FAILURE;
		}
	return EXIT_SUCCESS;
	}

void Ukoncenie(int text) 
	{
		printf("%s", table[text]);
	}

int KontrolaArgumentov(int argc, char *argv[], int *p_mod, int *p_pozicia_hladaneho_pismena)
{	
	if (argc == 2 && strcmp("--help", argv[1]) == 0) /// pokial bol prvy a jediny argument --help, tak vypis help a skonci program
	{
		Ukoncenie(HELP);
		return EXIT_FAILURE;
	} 
	
	else if(strlen(argv[1])==1) /// nasledujuce vykonava iba ak prvy argument jeden znak
	{
		if (strncmp (".", argv[1], 1) == 0) 
			*p_mod = Bodka;
		
		else if (strncmp ("^", argv[1], 1) == 0)
			*p_mod = Strieska;
		
		else if (strncmp (":", argv[1], 1) == 0) 
			*p_mod = DvojBodka;
		else if (Platny_znak(argv[1][0]) == true)
			*p_mod = ZakladnyMod;
	}
	else
	{
		Ukoncenie(ArgX); 
		return EXIT_FAILURE;
	}
		
	if (Volitelne_Argumenty(argc, &*argv, &*p_mod, &*p_pozicia_hladaneho_pismena)== EXIT_FAILURE)
			return EXIT_FAILURE;
			
	return EXIT_SUCCESS;
}

int Volitelne_Argumenty(int argc, char *argv[], int *p_mod, int *p_pozicia_hladaneho_pismena)
{
int cislo_hladane = 0;
	if(argc >= 3 && argc <= 4) 
		{
			for(int i = 2; i < argc; i++)
			 {
				if (strcmp ("-d", argv[i]) == 0) 
					*p_mod = (*p_mod)+10;
				
				else if (cislo_hladane == 0 &&((*p_pozicia_hladaneho_pismena = Prevod(&*argv[i]))>0)){} /// iba ak je platna pozicia
				else
					return EXIT_FAILURE;
			 }		 
		}
	return EXIT_SUCCESS;
}

bool Platny_znak(int chr)
{
	if ((chr >= '0'&&chr <= '9')||(chr >= 'A'&&chr <= 'Z')||(chr >= 'a'&&chr <= 'z')||chr == '_'||chr == '-') /// znaky povazovane za platne
		return true;
	
	return false;
}

int Vstup(char *argv[], int pozicia_hladaneho_pismena, int mod)
{
	int c_int = 0;
	int najdeny = 0;
	int pocet_znakov = 0;
	bool medzera = true;
	int aktualna_pozicia = 0;
	bool znak_bol = false;		

	while ((c_int = getchar()) != EOF )
	{
		if(pozicia_hladaneho_pismena != 0)
			aktualna_pozicia++;
		
		if (Platny_znak(c_int) == true)
		{
			medzera = false;
	
			if(pozicia_hladaneho_pismena == aktualna_pozicia && znak_bol == false)
			{
				if ((mod == ZakladnyMod || mod == DebugMod )&& argv[1][0] == c_int)
					znak_bol = true;
				else if (mod == Bodka || mod == (Bodka + DebugMod)) 
					znak_bol = true;
				else if ((mod == Strieska || mod == (Strieska + DebugMod)) && c_int >= 'A' && c_int <= 'Z')
					znak_bol = true;
				else if ((mod == DvojBodka || mod == (DvojBodka + DebugMod)) && c_int>= '0' && c_int <= '9')
					znak_bol = true;
				if (znak_bol == true)
					najdeny ++;
			}
		}
		else ///kazdy iny ako platny znak je nastaveny ako oddelovac (medzera)
		{
			c_int = ' ';
			znak_bol = false;
		}

		if(c_int == ' ')
			aktualna_pozicia = 0;
		
		if(mod>=DebugMod)
		{
			if(c_int == ' '&& medzera == false)
			{	
				printf("\n");
				pocet_znakov = 0;
				medzera = true;
			}
			else if (pocet_znakov < MaxZnakov && c_int != ' ')
			{
				putchar(c_int);
				pocet_znakov++;
			}
		}
	}
	printf("%d\n", najdeny);
	return EXIT_SUCCESS;
}
