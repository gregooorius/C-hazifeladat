#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct szolo_fajta
{
  char name[24];
  double exp_avg;           //elvárt átlag
  struct szolo_fajta *next; // kovetkezo fajta szollo
} szolo_fajta;

typedef struct szolo_terulet
{
  char name[24];
  double exp_avg; //elvárt átlag
  double hektar;  //txt-ből
  struct szolo_terulet *next;
  struct szolo_ev *start;
} szolo_terulet;

typedef struct szolo_ev
{
  int ev;
  double termes; //kg txt-ből
  double avg;
  double eves_avg;
  struct szolo_ev *next;
} szolo_ev;

szolo_fajta *push_szolo_fajta(szolo_fajta *head)
{
  szolo_fajta *current = head;
  while (current->next != NULL)
  {
    current = current->next;
  }

  current->next = malloc(sizeof(szolo_fajta));
  current->next->next = NULL;

  return current->next;
}

szolo_terulet *push_szolo_terulet(szolo_terulet *head)
{
  szolo_terulet *current = head;
  while (current->next != NULL)
  {
    current = current->next;
  }

  current->next = malloc(sizeof(szolo_terulet));
  current->next->next = NULL;

  return current->next;
}

szolo_ev *push_szolo_ev(szolo_ev *head)
{
  szolo_ev *current = head;
  while (current->next != NULL)
  {
    current = current->next;
  }

  current->next = malloc(sizeof(szolo_ev));
  current->next->next = NULL;

  return current->next;
}

szolo_ev *szolo_ev_beolvas(char *evek_str)
{
  szolo_ev *szolo_ev_head = malloc(sizeof(szolo_ev));
  szolo_ev_head->next = NULL;

  szolo_ev *szolo_ev_tail = NULL;

  int ev = 2016;
  char *pt;
  pt = strtok(evek_str, ",");
  while (pt != NULL)
  {
    if (szolo_ev_tail == NULL)
    {
      szolo_ev_tail = szolo_ev_head;
    }
    else
    {
      szolo_ev_tail = push_szolo_ev(szolo_ev_tail);
    }

    szolo_ev_tail->ev = ev++;
    szolo_ev_tail->termes = atof(pt);
    pt = strtok(NULL, ",");
  }

  return szolo_ev_head;
}

szolo_terulet *szolo_terulet_beolvas(char *fajlnev)
{
  szolo_terulet *szolo_terulet_head = malloc(sizeof(szolo_terulet));
  szolo_terulet_head->next = NULL;

  szolo_terulet *szolo_terulet_tail = NULL;

  FILE *fajl;

  char name[24];
  double hektar; //txt-ből
  char evek_str[255];

  fajl = fopen(fajlnev, "r");
  while (fscanf(fajl, "%s %lf %s", name, &hektar, evek_str) == 3)
  {
    if (szolo_terulet_tail == NULL)
    {
      szolo_terulet_tail = szolo_terulet_head;
    }
    else
    {
      szolo_terulet_tail = push_szolo_terulet(szolo_terulet_tail);
    }

    strcpy(szolo_terulet_tail->name, name);
    szolo_terulet_tail->hektar = hektar;
    szolo_terulet_tail->start = szolo_ev_beolvas(evek_str);
  }

  fclose(fajl);

  return szolo_terulet_head;
}

szolo_fajta *szolo_fajta_beolvasas(char *fajlnev)
{
  szolo_fajta *szolo_fajta_head = malloc(sizeof(szolo_terulet));
  szolo_fajta_head->next = NULL;

  szolo_fajta *szolo_fajta_tail = NULL;

  FILE *fajl;

  char name[24];
  double exp_avg;

  fajl = fopen(fajlnev, "r");
  while (fscanf(fajl, "%s %lf", name, &exp_avg) == 2)
  {
    if (szolo_fajta_tail == NULL)
    {
      szolo_fajta_tail = szolo_fajta_head;
    }
    else
    {
      szolo_fajta_tail = push_szolo_fajta(szolo_fajta_tail);
    }

    strcpy(szolo_fajta_tail->name, name);
    szolo_fajta_tail->exp_avg = exp_avg;
  }

  fclose(fajl);

  return szolo_fajta_head;
}

void szolo_fajta_paroztatas(szolo_fajta *fajta_lista, szolo_terulet *terulet_lista)
{
  while (terulet_lista != NULL)
  {
    szolo_fajta *fajta_lista_head = fajta_lista;
    while (fajta_lista_head != NULL)
    {
      // printf(" - Ellenorzes: %s es %s \n", terulet_lista->name, fajta_lista_head->name);
      if (strcmp(terulet_lista->name, fajta_lista_head->name) == 0)
      {
        // printf(" +++ Egyezes: %s es %s \n", terulet_lista->name, fajta_lista_head->name);
        terulet_lista->exp_avg = fajta_lista_head->exp_avg;
        fajta_lista_head = fajta_lista_head->next;
        break;
      }

      fajta_lista_head = fajta_lista_head->next;
    }

    terulet_lista = terulet_lista->next;
  }
}

void szolo_szamolas(szolo_terulet *terulet_lista)
{
  while (terulet_lista != NULL)
  {
    szolo_ev *szolo_ev_head = terulet_lista->start;
    while (szolo_ev_head != NULL)
    {
      szolo_ev_head->avg = szolo_ev_head->termes / terulet_lista->hektar;
      szolo_ev_head = szolo_ev_head->next;
    }

    terulet_lista = terulet_lista->next;
  }
}

double eves_szolo_atlag(int ev, szolo_terulet *terulet_lista)
{
  double terulet_osszes = 0;
  double szollok_osszes = 0;

  while (terulet_lista != NULL)
  {
    szolo_ev *szolo_ev_head = terulet_lista->start;
    while (szolo_ev_head != NULL)
    {
      if (szolo_ev_head->ev == ev)
      {

        terulet_osszes = terulet_osszes + terulet_lista->hektar;
        szollok_osszes = szollok_osszes + szolo_ev_head->termes;
        // printf(" ++++ %d\n", szolo_ev_head->ev);

        szolo_ev_head = szolo_ev_head->next;
        break;
      }

      szolo_ev_head = szolo_ev_head->next;
    }

    terulet_lista = terulet_lista->next;
  }

  // printf(" ||| Osszes szolo: %f , Osszes terulet: %f\n", szollok_osszes, terulet_osszes);
  return szollok_osszes / terulet_osszes;
}

void minden_eves_szolo_atlag(szolo_terulet *terulet_lista)
{
  szolo_terulet *terulet_lista_head = terulet_lista;
  while (terulet_lista_head != NULL)
  {
    szolo_ev *szolo_ev_head = terulet_lista_head->start;
    while (szolo_ev_head != NULL)
    {
      szolo_ev_head->eves_avg = eves_szolo_atlag(szolo_ev_head->ev, terulet_lista);
      szolo_ev_head = szolo_ev_head->next;
    }

    terulet_lista_head = terulet_lista_head->next;
  }
}

int main(void)
{
  printf("start\n");

  szolo_terulet *lista = szolo_terulet_beolvas("tabla.txt");
  szolo_fajta *fajta_lista = szolo_fajta_beolvasas("fajtak.dat");
  szolo_fajta_paroztatas(fajta_lista, lista);
  szolo_szamolas(lista);
  minden_eves_szolo_atlag(lista);

  while (lista != NULL)
  {
    printf("Szolo: %s, Elvart: %f, Hektar: %f\n", lista->name, lista->exp_avg, lista->hektar);

    szolo_ev *szolo_ev_head = lista->start;

    while (szolo_ev_head != NULL)
    {
      printf("Ev: %d, Termes: %f, Atlag: %f kg/hektar, Eves termes avg: %f kg/hektar\n", szolo_ev_head->ev, szolo_ev_head->termes, szolo_ev_head->avg, szolo_ev_head->eves_avg);

      if (szolo_ev_head->avg < lista->exp_avg && szolo_ev_head->eves_avg > szolo_ev_head->avg)
      {
        printf(" - Ki kell vagni a picsaba!\n");
      }

      szolo_ev *szolo_ev_next = szolo_ev_head->next;
      free(szolo_ev_head);
      szolo_ev_head = szolo_ev_next;
    }

    szolo_terulet *t = lista->next;
    free(lista);
    lista = t;
    printf("-----------\n\n");
  }

  printf("end\n");
  return 0;
}