#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define OK 1
#define ERR 0

typedef struct
{
	int rows,cols;
	char *cells;
} Bitmap;

int find_vline(Bitmap *bitmap, int *x1, int *y1, int *x2, int *y2);
int find_hline(Bitmap *bitmap, int *x1, int *y1, int *x2, int *y2);
int find_square(Bitmap *bitmap, int *x1, int *y1, int *x2, int *y2);
int find(Bitmap *b, int r, int s, bool *f, int *a_d, int *m_d, int *s_x, int *s_y, int *x1, int *y1, int *x2, int *y2);
void *initBitmap(Bitmap *pm,int r,int c);
void fBitmap(Bitmap *pm);
char getcolor(Bitmap *pm,int r,int c);
int test(Bitmap *bitmap, FILE *f);
int hladaj(Bitmap *bitmap, int r, int s, int kolko, int *x1, int *y1, int *x2, int *y2);
bool podmienka(Bitmap *bitmap, int r, int s, int ii, int max);

const char *help =
    "Nazov:\tHladanie obrazcov\n"
    "Autor:\tVojtech Masek\n"
    "Email:\txmasek15@stud.fit.vutbr.cz\n\n"
    "Program:\tProgram hlada obrazce v monochromatickej bitmape,\n"
    "\t\tobrazcami povazujeme zvisle a vodorovne usecky a stvorce.\n\n"
    "Syntax spustenia:\t./proj3 --help\n"
    "\t\t\t./proj3 --test *.txt\n"
    "\t\t\t./proj3 --hline *.txt\n"
    "\t\t\t./proj3 --vline *.txt\n"
    "\t\t\t./proj3 --square *.txt\n"
    "priklad pouzitia:\t./proj3 --test matica.txt\n"
    "\t\t\t./proj3 --square subor_so_stvorcom.txt\n"
    "Popis parametrov:\n"
    "Program sa spusta s vyssie uvedenymi parametrami.\n"
    "\t--test overi validnost bitmapy\n"
    "\t--hline vyhlada najdlhsiu vodorovnu usecku.\n"
    "\t--vline vyhlada najdlhsiu zvislu usecku.\n"
    "\t--square vyhladava najvacsi stvorec,\n"
    "\tvracia pociatocne a koncove suradnice stvorca.\n";

int main(int argc,char *argv[])
{
	int r,c, x1=0, y1=0, x2=0, y2=0;
    FILE *f;	
    
    if (argc<3)
    { 
		if(strcmp("--help", argv[1]) == 0)
		{
			printf(help);
			return EXIT_SUCCESS;
		}
		fprintf(stderr,("Malo vstupnych argumoentov programu"));	
		return EXIT_FAILURE;
	}
	
    if((f=fopen(argv[2],"r"))==NULL) 
	{ 
		printf("Invalid\n");
		fprintf(stderr,("Nenasiel sa subor s bitmapou"));	
		return EXIT_FAILURE;
	}
				
    if(fscanf(f,"%d%d",&r,&c)<2)
    {
		fclose(f);
		printf("Invalid\n");
		return EXIT_FAILURE;
    }
	 
    Bitmap m;
    
    if (initBitmap(&m,r,c)==NULL)
    {
		fprintf(stderr,("Nepodarilo sa inicializovat Bitmapu"));
		fBitmap(&m);	
		fclose(f);
		return EXIT_FAILURE;
    }
	else if(test(&m, f)==EXIT_FAILURE)
	{
		printf("Invalid\n");
		fBitmap(&m);	
		fclose(f);	
		return EXIT_FAILURE;
	}
	else if(strcmp("--test", argv[1]) == 0)
		printf("Valid\n");
	else if(strcmp("--hline", argv[1]) == 0)
	{
		if(find_hline(&m, &x1, &y1, &x2, &y2)==OK)
			printf("%d %d %d %d\n", x1, y1, x2, y2);
		else
			printf("Nenasiel sa zadany utvar\n");
	}	
	else if(strcmp("--vline", argv[1]) == 0)
	{
		if(find_vline(&m, &x1, &y1, &x2, &y2)==OK)
			printf("%d %d %d %d\n", x1, y1, x2, y2);
		else
			printf("Nenasiel sa zadany utvar\n");
	}
	else if(strcmp("--square", argv[1]) == 0)
	{
		if(find_square(&m, &x1, &y1, &x2, &y2)==OK)
			printf("%d %d %d %d\n", x1, y1, x2, y2);
		else
			printf("Nenasiel sa zadany utvar\n");
	}
	
	fBitmap(&m);
    fclose(f);
    return EXIT_SUCCESS;
}

int hladaj(Bitmap *bitmap, int r, int s, int kolko, int *x1, int *y1, int *x2, int *y2) // kolko znaci aky dlhy bude stvorec +1
{
	int ii, pocet=kolko;

	
	for(ii=1;ii<kolko;ii++)       
	{
		if(getcolor(bitmap, r+ii, s) != 1)
			return ERR;
		pocet++;
	}
	for(ii=1;ii<kolko-1;ii++)       
	{
		if(getcolor(bitmap, r+ii, s+kolko-1) != 1)
			return ERR;
		pocet++;
	}
	for(ii=1;ii<kolko;ii++)       
	{
		if(getcolor(bitmap, r+kolko-1, s+ii) != 1)
			return ERR;
		pocet++;
	}
	
	if(pocet==4*(kolko-1))
	{
		*x1=r;
		*y1=s;
		*x2=r+kolko-1;
		*y2=s+kolko-1;
		return OK;
	}
	
	return ERR;
}

bool podmienka(Bitmap *bitmap, int r, int s, int ii, int max)
{
	if(ii<bitmap->cols&&s+max<bitmap->cols&&r+max<bitmap->rows&&(getcolor(bitmap, r, s+ii)==1))
		return true;
	return false;       
}

int find_square(Bitmap *bitmap, int *x1, int *y1, int *x2, int *y2)
{	
	int ii=0, max=1;
	
    for(int r=0; r<bitmap->rows;r++)       
	{
		for(int s=0; s<bitmap->cols;s++)
		{
			for(ii=0;podmienka(bitmap, r, s, ii, max);ii++)       
			{}
			while(ii>max)
			{
				if(hladaj(bitmap, r, s, ii, x1, y1, x2, y2)==1)
					max=ii;
				else
					ii--;
			}
		}
	}
	
	if(*x1+*x2+*y1+*y2==0)
		return ERR;
		
	return OK;
}

int test(Bitmap *bitmap, FILE *f)
{
	int x;
	
    for (int i=0;i<bitmap->cols*bitmap->rows;i++)
    {
		if(fscanf(f,"%d",&x)<1) 
			return EXIT_FAILURE;
		
		if(x==1||x==0)
			bitmap->cells[i]=x;
		else
			return EXIT_FAILURE;
			
    }
    if(fscanf(f,"%d",&x)==1)
			return EXIT_FAILURE;
	
    return EXIT_SUCCESS;
}

void *initBitmap(Bitmap *pm,int r,int c)
{
	pm->cols=c;
	pm->rows=r;
	return pm->cells=malloc(r*c*sizeof(*pm->cells));
}

void fBitmap(Bitmap *pm)
{
	free(pm->cells);
}

char getcolor(Bitmap *bitmap, int x, int y)
{
	return bitmap->cells[x*(bitmap->cols)+y];
}

int find(Bitmap *b, int r, int s, bool *f, int *a_d, int *m_d, int *s_x, int *s_y, int *x1, int *y1, int *x2, int *y2)
{
	if(getcolor(b, r, s) == 1)
	{
		if(*f == false) 
		{
			*f = true;
			*s_x = r;
			*s_y = s;
		}
		(*a_d)++;
                
		if (*a_d > *m_d) 
		{
		*x1=*s_x;
		*y1=*s_y;
		*x2=r;
		*y2=s;
		*m_d = *a_d;
		}
	}
	else
	{
		*f = false;
		*a_d = 0;
	}
	
	return EXIT_SUCCESS;
}

int find_hline(Bitmap *bitmap, int *x1, int *y1, int *x2, int *y2)
{
	bool f = false;
	int sur_x, sur_y, akt_dlzka =0, max_dlzka =0;
	
    for(int r=0; r<bitmap->rows;r++)       
	{
		for(int s=0; s<bitmap->cols;s++)
		{
			find(bitmap, r, s, &f, &akt_dlzka, &max_dlzka, &sur_x, &sur_y, x1, y1, x2, y2);
		}
		f = false;
		akt_dlzka = 0;
	}
	
	if(*x1+*x2+*y1+*y2==0)
		return ERR;
		
	return OK;
}

int find_vline(Bitmap *bitmap, int *x1, int *y1, int *x2, int *y2)
{
	bool f = false;
	int sur_x, sur_y, akt_dlzka =0, max_dlzka =0;
    for(int s=0; s<bitmap->cols;s++)      
	{
		for(int r=0; r<bitmap->rows;r++)
		{
			find(bitmap, r, s, &f, &akt_dlzka, &max_dlzka, &sur_x, &sur_y, x1, y1, x2, y2);
		}
		f = false;
		akt_dlzka = 0;
	}
	
	if(*x1+*x2+*y1+*y2==0)
		return ERR;
		
	return OK;
}
