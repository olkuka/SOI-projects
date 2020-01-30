#include<stdlib.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>

#define NUM_OF_FILES 64 

char* vFileName;
unsigned long vFileSize = 0, vFileStart = 0, vFileFree = 0;	/* wielkosc calego systemu plikow, rozmiar pamieci zajetej na starcie (rozmiar(inode)*maksymalna liczba plikow) i ilosc wolnego miejsca */
int vFile_ID;
int inodeFree = NUM_OF_FILES;		/* ilosc aktualnych wolnych inode'ów */

typedef struct vfsinode		/* struktura jednego inode'a */
{
  char fileName[128];		/* nazwa pliku */
  unsigned long fileSize;		/* jego rozmiar */
  unsigned long fileBegin;		/* polozenie poczatku bloku */
} inode;

typedef struct node		/* struktura elementu listy */
{
  inode data;			/* dane (czyli jeden inode to jeden element listy) */
  struct node* next;		/* wskaznik na nastepny element */ 
} listNode;

listNode* listHead = NULL;	/* glowa listy */
listNode* actINode = NULL;	/* wskaznik aktualnego inode gotowego do uzycia */
unsigned long actPos = 0;	/* aktualna pozycja w pamieci */

/* "czyszczenie" */
void vfile_clear(void)
{
  unsigned int i;
  int buf[2048];
  int count;
  
  for(i = 0;i < 2048;i++) 
    buf[i] = '\0';	/* zapisywanie calego bufora znakami konca linii */
  i = vFileSize;
  count = 2048;			/* zakladam, ze kazdy plik ma na poczatku rozmiar 2048 bajty */
  while(i > 0)
  {
    if(i < count) 
      count = i;	/* jesli juz sie konczy miejsce na dysku ( czyli jest mniej niz 2048 )*/
    write(vFile_ID,buf,count);		/* zapisywanie z bufora (czyli znaku konca linii) do kazdego wirtualnego pliku */
    i -= count;
  }
}

/* funkcja, ktora tworzy dysk wirtualny */
int create_vfile(void)
{
  if((sizeof(inode)*NUM_OF_FILES) > vFileSize)	/* jesli nie ma wystarczajacej ilosci miejsca na dysku - stosowny komunikat*/
  {
    fprintf(stderr, "Rozmiar dysku wirtualnego jest za maly!");
    exit(0);
  }
  
  vFile_ID = open(vFileName, O_RDWR | O_CREAT);	
 /*close(vFileName);*/
  vfile_clear();
  vFileStart = sizeof(inode)*NUM_OF_FILES; /* obliczam poczatek calego systemu (tam gdzie zaczna sie pliki) */
  vFileFree = vFileSize - vFileStart;	/* ilosc wolnego miejsca */
  printf("Utworzono wirtualny dysk.\n");
  return 1;
  
}

/* funkcja pomocnicza do wczytywania tekstu, aby nie musiec uzywac scanf, po ktorym trzeba czyscic bufor */
int get_line (char *linia, int max_dl)  
{
  char c;
  int i;
  
  for (i=0; (c=getchar())!=EOF && c!='\n' &&  i<=max_dl; i++) 
    *linia++=c;
  *linia='\0';
  return i;
}

/* funkcja pomocnicza sprawdzajaca czy na dysku istnieje juz plik o podanej nazwie */ 
int is_unique(char* name) 
{
  listNode* temp1;
  printf("Sprawdzono, czy plik o nazwie %s jest juz na dysku.\n", name);
  for(temp1 = listHead; temp1 != NULL; temp1 = temp1->next) 
    if (strcmp(temp1->data.fileName, name) == 0) 
      return 0;
  return 1;
}

/* funkcja sprawdzajaca, czy na vfs jest wystarczajaco miejsca, aby dodac nowy plik */
long is_enough_space(long size)
{
  listNode* temp1;
  if(listHead == NULL)		/* przypadek, jesli plik ma byc pierwszym plikiem */
  {
    if((vFileSize - vFileStart) > size) 
      return vFileStart;	
    else return -1;
  }
  /* gdy juz istnieja zapisane pliki, ale pomiedzy nimi jest luka pamieci o wystarczajacej wielkosci*/
  /* na poczayku blokow */
  if((listHead->data.fileBegin - vFileStart) >= size) 
    return vFileStart;
  /* gdzies w srodku blokow */
  for(temp1 = listHead; temp1->next != NULL; temp1 = temp1->next)
    if((temp1->next->data.fileBegin - (temp1->data.fileBegin + temp1->data.fileSize)) >= size) 
      return (temp1->data.fileBegin + temp1->data.fileSize);
  /* gdy juz istnieja zapisane pliki, ale nie ma pomiedzy nimi luki o wystarczajacej wielkosci */
  if((vFileSize - temp1->data.fileBegin + temp1->data.fileSize) >= size) 
    return (temp1->data.fileBegin + temp1->data.fileSize);
  printf("Sprawdzono, czy jest wystarczajaca ilosc miejsca na dysku.\n");
  return -1;
}

/* funkcja zwracajaca ostatni inode */
listNode* prev_inode(long end)
{
  listNode* temp1;
  
  if((listHead != NULL)&&(listHead->next == NULL)) 
    return listHead;
  for (temp1 = listHead; temp1 != NULL; temp1 = temp1->next) 
    if((temp1->data.fileBegin + temp1->data.fileSize) == end) 
      return temp1;
  return NULL;
}

/* zapisywanie pliku na dysk wirtualny */ 
int write_to_vfile(int src, char *dest, long size)
{
  char buf[4096 * 8];		/* maksymalny rozmiar bufora */
  long begin;
  listNode *new, *prev;
  int count;
  
  begin = is_enough_space(size);
  if(begin < 0)
  {
    printf("\nBrak wolnego miejsca na dysku wirtualnym !\n");
    return 0;
  }
  new =(listNode*)malloc(sizeof(listNode));		/* tworze nowy element listy */
  strncpy(new->data.fileName, dest, 128-1);	
  new->data.fileSize  = size;
  new->data.fileBegin = begin;
  prev = prev_inode(begin);			/* ustawiam wszystkie parametry nowego elementu listy */ 
  if(prev == NULL)
  {
    if(listHead == NULL)
    {
      listHead = new;
      new->next = NULL;
    }
    else
    {
      new->next = listHead;
      listHead = new;
    }
  }
  else 
  {
    new->next = prev->next;
    prev->next = new;
  }
  lseek(vFile_ID, new->data.fileBegin, 0);
  count = 4096 * 8;
  while (size > 0)			/* zajmowanie pamieci az caly plik zostanie zapisany */
  {
    if (size < count) 
      count = size;	
    read(src, buf, count);		/* czytanie z pliku do bufora */
    write(vFile_ID, buf, count);	/* zapisywanie z bufora do pliku na dysku wirtualnym */
    size -= count;
  }
  inodeFree--;					/* zmiana ilosci wolnych inode'ow */ 
  vFileFree -= new->data.fileSize;		/* zmiana wolnej przestrzeni w systemie plikow */
  printf("Dodano nowy plik do dysku wirtualnego.\n");
  return 1;
}

/* funkcja kopiujaca plik z Minixa na dysk wirtualny */
void copy_file(void) 
{
    char remote[128];		/* nazwa tymczasowego pliku */
  unsigned long size;			/* rozmiar */
  int fileID;				/* id pliku */
  struct stat stbuf;			/* stat to struktura, ktora przechowuje dane na temat pliku */
  
  /*printf("Podaj nazwe pliku do skopiowania: ");*/
  get_line(remote, 128);

  if(!is_unique(remote))
  {
    fprintf(stderr, "Taki plik juz jest na dysku!\n");
    return;
  }
  stat(remote, &stbuf);
  if((fileID = open(remote, O_RDONLY)) < 0 || (stbuf.st_mode & S_IFDIR)) /* jesli plik nie otworzyl sie prawidlowo lub pliku zwyczajnie nie ma w katalogu */
  {
    fprintf(stderr, "Nie ma pliku %s !", remote);
    return;
  }
  size = stbuf.st_size;		/* w przeciwnym wypadku ustawiamy rozmiar tego pliku na rozmiar pliku ktory chcemy skopiowac */
  if(write_to_vfile(fileID, remote, size) != 0)
  printf("Skopiowano plik %s z Minixa na dysk wirtualny.\n", remote);
}

listNode* get_listnode(char* name)
{
  listNode* temp1;
  for(temp1 = listHead; temp1 != NULL; temp1 = temp1->next) 
    if(strcmp(name, temp1->data.fileName) == 0) 
      break;
  printf("Pobrano pozycje pliku o nazwie %s z listy.\n", name);
  return temp1;
}

/* funkcja kopiujaca z dysku wirtualnego na Minixa */ 
int read_from_vfile(void)
{
  listNode* temp1;
  int fileID;
  int count;
  long size;
  char buf[4096 * 8];
  char name[128];
  char remote[128];
  
  /*printf("Podaj nazwe pliku, ktory skopiowac : ");*/
  get_line(name, 128 );

  if((temp1 = get_listnode(name)) == NULL)
  {
    fprintf(stderr, "Nie ma takiego pliku %s.\n", name);
    return;
  }

  /*printf("Podaj nazwe pliku, do ktorego skopiowac : ");*/
  get_line(remote, 128);

  lseek(vFile_ID, temp1->data.fileBegin, 0); 	
  fileID = open(remote, O_WRONLY | O_CREAT);		/* utworzenie nowego pliku do pisania */
  count = 4096 * 8;
  size = temp1->data.fileSize;
  while(size > 0) 		/* zajmowanie pamieci dopoki caly plik zostanie zapisany */
  {
    if(size < count) 
      count = size;
    read(vFile_ID, buf, count); 	/* czytanie z pliku do bufora */
    write(fileID, buf, count);		/* zapisywanie z bufora do pliku */
    size -= count;			/* */
  }
  printf("Skopiowano plik %s z wirtualnego dysku do Minixa.\n", temp1);
  close(fileID);
}

/* funkcja wyswietlajaca zawartosc wirtualnego dysku */
void dir_vfile(void)
{
  listNode* temp1;
  if(listHead == NULL) 
    printf("\nNa dysku wirtualnym nie ma plikow.\n");
  temp1 = listHead; 
  while(temp1 != NULL)
  { 
    printf("\n%s: \trozmiar %d poczatek pliku %d koniec pliku %d ", temp1->data.fileName, temp1->data.fileSize, temp1->data.fileBegin, (temp1->data.fileBegin + temp1->data.fileSize));
    temp1 = temp1->next;
  }
  printf("\n");
}

/* usuwanie pliku - zwykle usuwanie elementu z listy */
void del_inode(char* del)
{
  listNode* temp1, *temp2;
  if(listHead == NULL) return;
  for(temp1 = listHead; temp1 != NULL; temp2 = temp1, temp1 = temp1->next) 
    if(strcmp(temp1->data.fileName, del) == 0) 
      break;
  if(listHead == temp1)
    listHead = temp1->next;
  else 
    temp2->next = temp1->next;
  vFileFree += temp1->data.fileSize;
  inodeFree++;
  free(temp1);
  printf("Usunieto plik z listy plikow.\n");
}

/* funkcja usuwajaca plik z vfs */
int del_file(void)		
{
  char name[128];
  listNode* temp1;
  
  /*printf("Podaj nazwe pliku do usuniecia: ");*/
  get_line(name, 128);
  if((temp1 = get_listnode(name)) == NULL) 
  {
    printf("\nPlik o nazwie %s nie istnieje!\n", name);
    return 0; 
  }
  del_inode(name);
  printf("Usunieto plik o nazwie: %s\n", name);
  return 1;
}

/* usuniecie calej listy */
void del_list(void) 
{
  listNode *temp = listHead;
  listNode *del;
  while (temp != NULL) {
    del = temp;
    temp = temp->next;
    free(del);
   }
   listHead = NULL;
   printf("Usunieto cala liste plikow.\n");
}

/* usuniecie calego dysku */
void remove_vfile(void) 
{
   close(vFile_ID);
   del_list();
   remove(vFileName);
   printf("Usunieto dysk.\n");
}

/* wyswietlanie mapy zajetosci */
void mapvfile(void) 
{
   printf("MAPA ZAJETOSCI DYSKU\n");
   printf("Rozmiar dysku: %lu\n", vFileSize);
   printf("Liczba plikow na dysku: %d\n", NUM_OF_FILES - inodeFree);
   printf("Ilosc danych systemowych: begin = 0, size = %lu\n", vFileStart);
   printf("Pozostala ilosc wolnego miejsca: %lu\n", vFileFree);
}

void test_dodawanie_plikow(void)
{
   int i;
   for (i = 0; i < 5; i++) copy_file();
   dir_vfile();
   mapvfile();
}

void test_usuniecie_pliku(void) 
{
   int i;
   for (i = 0; i < 4; i++) copy_file();
   dir_vfile();
   del_file();
   del_file();
   dir_vfile();
   mapvfile();
}

void test_kopiowanie_pliku_na_Minix(void)
{
   copy_file();
   read_from_vfile();
}

void test_usuniecie_dysku(void) 
{
   int i;
   for (i = 0; i < 4; i++) copy_file();
   dir_vfile();
   remove_vfile();
}


int main(int argc, char** argv)
{
  char c,i,d;
  if (argc != 3) {
    printf("Brak argumentow (nazwa + rozmiar systemu).\n");
   return 0;
  }
  vFileName = argv[1];
  vFileSize = atoi(argv[2]);
  create_vfile();
  
 /* test_dodawanie_plikow();*/	/* skrypt1.sh */
 /*  test_usuniecie_pliku(); */	/* skrypt2.sh */
 /*  test_kopiowanie_pliku_na_Minix(); */	/* skrypt3.sh */
   test_usuniecie_dysku(); 	/* skrypt4.sh */
  
  return 0;
}
