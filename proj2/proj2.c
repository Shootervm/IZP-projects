#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


#define eps 		1e-12								/// relativna presnost eps
#define pi			3.141592653589793
#define pi_pol 		1.5707963267948965 					/// pi/2
#define mag_cislo 	0.70710678118654752440084436210485 	/// sqrt(2)/2 /// asin(sqrt(2)/2) je 45° /// pouzite koli arcsin aproximacii

const double INF=1.0/0.0;
const double NAN=0.0/0.0;

void Ukoncenie(int text);											/** Funkcia pre vypis stavovych hlasok programu*/
double ab_h(double x);												/** Vracia absolutnu hodnotu vstupneho parametra*/
int Argumenty(int argc);											/** Kontrola poctu argumentov programu*/
double my_sqrt(double x);											/** Vypocet druhej odmocniny vstupneho parametra*/
double my_asin(double x);											/** Vypocet arcsin na zaklade vstupneho parametra*/
int Triangle(char *argv[] , double *pole);							/** Vypocet a vypis vnutornych uhlov trojuholnika*/
int Body_stran(char *argv[], double pole[]);						/** Prevod vstupnych argumentov na body stran ako double*/
double Vypocet_asin(double x, double x2);							/** Funkcia pre asin, obsahuje vypocet na zaklade Taylorovej rady*/
bool Zostrojitelnost(double a, double b, double c);					/** Kontrola zostrojitelnosti trojuholnika*/
double Strana(double xx, double xy, double yx, double yy);			/** Vypocet dlzky strany zo vstupnych bodov*/

enum msgs
{
	HELP,		/** Help*/
	NArgs,		/** Chybaju argumenty programu.*/
	Args,		/** Nespravny syntax argumentov programu.*/
};

const char *table[]=
{
	/** HELP */
	[HELP]=
	"\nNazov:\tIteracne vypocty\n"
	"Autor:\tVojtech Masek\n"
	"Email:\txmasek15@stud.fit.vutbr.cz\n\n"
	"Program vypocita uhly obecneho trojuholnika, daneho tromi vrcholmi.\n"
	"Vypise ich v radianoch v poradi uhol pri vrchole A, B a C.\n"
	"Dalsimi dvoma funkciami su vypocet druhej odmocniny a arcsin.\n"
	"Detaily funkcnosti programu su podrobne popisane v dokumentacii.\n\n"
	"Syntax spustenia:\t./proj1 --help\n\t\t\t./proj2 --sqrt X\n"
	"\t\t\t./proj2 --asin X\n\t\t\t./proj2 --triangle AX AY BX BY CX CY\n"
	"Priklad pouzitia:\t./proj2 --asin 1\n\t\t\t./proj2 --triangle 0 0 1 0 0 2\n"
	"Popis argumentov:\n"
	"X reprezentuje cislo pre ktore chceme pocitat danu matematicku operaciu\n"
	"AX AY BX BY CX CY predstavuju body trojuholnika v tvare [0,0],[1,0],[0,2]\n",
	/** Stavove hlasky */
	[NArgs]="\nChybaju argumenty programu.\n",
	[Args]="\nNespravny syntax argumentov programu.\nPouzite parameter --help.\n",
};

int main(int argc, char *argv[])
{
	double pole[6];
	char* pEnd;
	
	if (Argumenty(argc) == EXIT_FAILURE)
		return EXIT_FAILURE;


	if (argc == 2 && strcmp("--help", argv[1]) == 0)
	{
		Ukoncenie(HELP);
		return EXIT_SUCCESS;
	}
	
	if(strcmp(argv[1], "--triangle")== 0 && argc==8)
	{
		if(Triangle(argv, pole)==EXIT_FAILURE)
		{
			printf("%.10e\n%.10e\n%.10e\n", NAN, NAN, NAN);
			return EXIT_FAILURE;
		}
		else
			return EXIT_SUCCESS;
	}
		
	if(strcmp(argv[1], "--sqrt")== 0 && argc==3)
	{
		double x= strtod(argv[2], &pEnd);
		if(*pEnd == 0)
		{
			if(x>=0)
			{
				printf("%.10e\n", my_sqrt(x));
				return EXIT_SUCCESS;
			}
			else
				printf("%.10e\n", NAN);
		}
		else if(strcmp(argv[2], "inf")== 0)
		{
			printf("%.10e\n", INF);
			return EXIT_SUCCESS;	
		}
		else
			printf("%.10e\n", NAN);
			
		return EXIT_FAILURE;
	}
	
	if(strcmp(argv[1], "--asin")== 0&& argc==3)
	{
		double x= strtod(argv[2], &pEnd);
		if(*pEnd == 0)
		{
			if (ab_h(x)<=1.0) 		///	iba interval x= <-1.0&&x>=-1.0)
			{
				printf("%.10e\n", my_asin(x));
				return EXIT_SUCCESS;	
			}
		}
		printf("%.10e\n", NAN);
		return EXIT_FAILURE;
	}
	
	Ukoncenie(Args);
	return EXIT_FAILURE;
}	

void Ukoncenie(int text) 
{
	printf("%s", table[text]);
}

double ab_h(double x)
{
	 return (x < 0.0) ? -x : x;
}

int Argumenty(int argc)
{
	if (argc == 1)
	{
		Ukoncenie(NArgs);
		return EXIT_FAILURE;
	}
	
	else if (argc < 3)
	{
		Ukoncenie(Args);
		return EXIT_FAILURE;
	}
		
	return EXIT_SUCCESS;
}

int Body_stran(char *argv[], double pole[])
{
	char* pEnd;
	double x;
	for (int i=0; i<6; i++)
	{
		x= strtod(argv[2+i], &pEnd);
		if(*pEnd == 0)
			pole[i]=x;
		else
			return NAN;
	}
	return EXIT_SUCCESS;
}

bool Zostrojitelnost(double a, double b, double c)
{
	return ((a+b>c)&&(b+c>a)&&(c + a>b)) ? true : false;
}

double Strana(double xx, double xy, double yx, double yy)
{
	double dx = xx - yx;
	double dy = yy - xy;
	return (dx*dx + dy*dy);
}

int Triangle(char *argv[] , double *pole)
{
	int i=0;	
	if(Body_stran(argv, pole)==NAN)
		return EXIT_FAILURE;
		
	double a2=Strana(pole[i+0], pole[i+1], pole[i+2], pole[i+3]);
	double b2=Strana(pole[i+0], pole[i+1], pole[i+4], pole[i+5]);
	double c2=Strana(pole[i+2], pole[i+3], pole[i+4], pole[i+5]);
	double a=my_sqrt(a2);
	double b=my_sqrt(b2);
	double c=my_sqrt(c2);
	
	if ((Zostrojitelnost(a, b, c)==false))
		return EXIT_FAILURE;
	
	double A= pi_pol-my_asin((a2+b2-c2)/(2.0*a*b)); 	
	double C= pi_pol-my_asin((b2+c2-a2)/(2.0*b*c));
	double B= pi-A-C;
	
	if(B>=pi)
		return EXIT_FAILURE;
	
	printf("%.10e\n%.10e\n%.10e\n", A, B, C);
	
	return EXIT_SUCCESS;		
}

double my_asin(double x) 
{
	double x2=x*x;
	if(x>mag_cislo)	
		x=pi_pol -  my_asin(my_sqrt(1-x2));	
	else if(x<(-mag_cislo))
		x=-(pi_pol -  my_asin(my_sqrt(1-x2)));	
	else
		x=Vypocet_asin(x, x2);	
	return x;
}

double Vypocet_asin(double x, double x2)
{
	double min = x;
	double xs;
	unsigned long int i = 0;
	do 
	{
		xs = x;
		i++;
		min *=((x2)*(2*i-1)*(2*i-1))/((2*i)*(2*i+1));
		x +=min; 
	}while(ab_h(xs-x)>(eps*min));
	return x;
}

double my_sqrt(double x)
{
	if(x==0)
		return 0;
	double Yn = 0.0;
	double Yi = 2.0;
	do 
	{	
		Yn=Yi;
		Yi=((x/Yi)+Yi)/2;
	}while(ab_h(Yn - Yi)>(eps*Yi));
	return Yi;
}
