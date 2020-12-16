#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "vaalit.h"

/* Harjoitustehtävänä tehty funktio, joka lukee käyttäjältä syötteen */
char * lueRivi(FILE *syote)
{
   size_t koko = 16;
   char* rivi = malloc(koko*sizeof(char));
   size_t luettu = 0;
   int jatketaan = 1;
   while(jatketaan)
   {
      rivi[koko-2] = '\0';
      if (fgets(&rivi[luettu], koko - luettu, syote) != NULL)
      {
         if (rivi[koko-2] != '\0' && rivi[koko-2] != '\n')
         {
            luettu = koko-1;
            koko *= 2;
            rivi = realloc(rivi, koko*sizeof(char));
         }
         else
         {
            luettu = strlen(rivi);
            if (luettu > 0 && rivi[luettu-1] == '\n')
            {
               rivi[luettu-1] = '\0';
               luettu--;
            }
            rivi = realloc(rivi, luettu+1*sizeof(char));
            jatketaan = 0;
         }
      }
      else
      {
         rivi = realloc(rivi, luettu+1*sizeof(char));
         jatketaan = 0;
      }
   }
   return rivi;
}

/* Lukee käyttäjältä syötteen lueRivi-funktion avulla, pilkkoo syötteen palasiin,
   poistaa turhat välilyönnit osien ympäriltä ja palauttaa valmiit osat taulukkona. */
char** lueKomento(int *osia)
{
   int komentojenlkm = 1;
   int osapit;
   char **komennot = malloc(komentojenlkm*sizeof(char *));
   char *osa;
   char *rivi = NULL;
   rivi = lueRivi(stdin);
   /* Komennon ensimmäinen osa eroteltu välilyönnillä*/
   osa = strtok(rivi, " ");
   osapit = strlen(osa);
   komennot[komentojenlkm-1] = malloc(osapit*sizeof(char)+1);
   strcpy(komennot[komentojenlkm-1], osa);

   /* Komennon loput osat eroteltu /-merkillä */
   for (osa = strtok(NULL, "/"); osa != NULL; osa = strtok(NULL, "/"))
   {
      /* Poistetaan välilyönnit alusta */
      while(*osa == ' ')
      {
         osa += 1;
      }
      osapit = strlen(osa);
      /* Poistetaan välilyönnit lopusta */
      while(osapit > 0 && osa[osapit-1] == ' ')
      {
         osa[osapit-1] = '\0';
         osapit--;
      }
      komentojenlkm++;
      komennot = realloc(komennot, komentojenlkm*sizeof(char *));
      komennot[komentojenlkm-1] = malloc(osapit*sizeof(char)+1);
      strcpy(komennot[komentojenlkm-1], osa);
   }
   /* Kerrotaan kutsuneelle funktiolle, montako osaa taulukko sisältää. */
   *osia = komentojenlkm;
   free(rivi);
   return komennot;
}

/* Luo EhdokasTaulu-olion */
EhdokasTaulu * luoEhdokasTaulu(void)
{
   EhdokasTaulu *uusi = malloc(sizeof(EhdokasTaulu));
   uusi->taulu = malloc(2*sizeof(Ehdokas));
   uusi->kapasiteetti = 2;
   uusi->n = 0;
   return uusi;
}

/* Luo VaalipiiriTaulu-olion */
VaalipiiriTaulu * luoVaalipiiriTaulu(void)
{
   VaalipiiriTaulu *uusi = malloc(sizeof(VaalipiiriTaulu));
   uusi->taulu = malloc(2*sizeof(Vaalipiiri));
   uusi->kapasiteetti = 2;
   uusi->n = 0;
   return uusi;
}

/* Luo PuolueTaulu-olion */
PuolueTaulu * luoPuolueTaulu(void)
{
   PuolueTaulu *uusi = malloc(sizeof(PuolueTaulu));
   uusi->taulu = malloc(2*sizeof(Puolue));
   uusi->kapasiteetti = 2;
   uusi->n = 0;
   return uusi;
}

/* Lisää EhdokasTauluun uuden Ehdokas-olion saamiensa parametrien pohjalta */
void lisaaEhdokas(EhdokasTaulu *et, const char* nimi, const char *puolue, const char *vaalipiiri, int aanimaara)
{
   char *nimi2;
   char *puolue2;
   char *vaalipiiri2;
   /* Kasvatetaan taulukon kokoa tarvittaessa */
   if (et->n >= et->kapasiteetti)
   {
      et->kapasiteetti *= 2;
      et->taulu = realloc(et->taulu, et->kapasiteetti*sizeof(Ehdokas));
   }
   nimi2 = malloc(strlen(nimi)+1);
   strcpy(nimi2, nimi);
   puolue2 = malloc(strlen(puolue)+1);
   strcpy(puolue2, puolue);
   vaalipiiri2 = malloc(strlen(vaalipiiri)+1);
   strcpy(vaalipiiri2, vaalipiiri);
   et->taulu[et->n].nimi = nimi2;
   et->taulu[et->n].puolue = puolue2;
   et->taulu[et->n].vaalipiiri = vaalipiiri2;
   et->taulu[et->n].aanimaara = aanimaara;
   et->taulu[et->n].vertausluku = 0;
   et->taulu[et->n].valittiinko = 0;
   et->n += 1;
}

/* Lisää VaalipiiriTauluun uuden Vaalipiiri-olion saamiensa parametrien pohjalta */
void lisaaVaalipiiri(VaalipiiriTaulu *vt, const char* nimi, int lkm)
{
   char *nimi2;
   /* Kasvatetaan taulukon kokoa tarvittaessa */
   if (vt->n >= vt->kapasiteetti)
   {
      vt->kapasiteetti *= 2;
      vt->taulu = realloc(vt->taulu, vt->kapasiteetti*sizeof(Vaalipiiri));
   }
   nimi2 = malloc(strlen(nimi)+1);
   strcpy(nimi2, nimi);
   vt->taulu[vt->n].nimi = nimi2;
   vt->taulu[vt->n].lkm = lkm;
   vt->n += 1;
}

/* Lisää PuolueTauluun uuden Puolue-olion saamiensa parametrien pohjalta */
void lisaaPuolue(PuolueTaulu *pt, const char* nimi)
{
   char *nimi2;
   /* Kasvatetaan taulukon kokoa tarvittaessa */
   if (pt->n >= pt->kapasiteetti)
   {
      pt->kapasiteetti *= 2;
      pt->taulu = realloc(pt->taulu, pt->kapasiteetti*sizeof(Puolue));
   }
   nimi2 = malloc(strlen(nimi)+1);
   strcpy(nimi2, nimi);
   pt->taulu[pt->n].nimi = nimi2;
   pt->taulu[pt->n].kansanedustajia = 0;
   pt->taulu[pt->n].vertausluku = 0;
   pt->taulu[pt->n].ehdokkaita = 0;
   pt->n += 1;
}

/* Järjestää qsortin avulla taulukon oliot nimiensä mukaiseen aakkosjärjestykseen */
int nimiVrt(const void *a, const void *b)
{
   Ehdokas *eka = (Ehdokas *) a;
   Ehdokas *toka = (Ehdokas *) b;
   int vrt = strcmp(eka->nimi, toka->nimi);
   return vrt;
}

/* Järjestää qsortin avulla taulukon oliot ensisijaisesti puolueen,
   toissijaisesti nimen mukaiseen aakkosjärjestykseen */
int puolueNimiVrt(const void *a, const void *b)
{
   Ehdokas *eka = (Ehdokas *) a;
   Ehdokas *toka = (Ehdokas *) b;
   int vrt = strcmp(eka->puolue, toka->puolue);
   if (vrt == 0)
   {
      vrt = strcmp(eka->nimi, toka->nimi);
   }
   return vrt;
}

/* Järjestää qsortin avulla taulukon oliot ensisijaisesti vaalipiirin,
   toissijaisesti äänimäärän mukaiseen järjestykseen */
int vaalipiiriAanimaaraVrt(const void *a, const void *b)
{
   Ehdokas *eka = (Ehdokas *) a;
   Ehdokas *toka = (Ehdokas *) b;
   int vrt = strcmp(eka->vaalipiiri, toka->vaalipiiri);
   if (vrt == 0)
   {
      vrt = toka->aanimaara - eka->aanimaara;
   }
   return vrt;
}

/* Järjestää qsortin avulla taulukon oliot ensisijaisesti vaalipiirin,
   toissijaisesti vertausluvun mukaiseen järjestykseen */
int vaalipiiriVertauslukuVrt(const void *a, const void *b)
{
   Ehdokas *eka = (Ehdokas *) a;
   Ehdokas *toka = (Ehdokas *) b;
   int vrt = strcmp(eka->vaalipiiri, toka->vaalipiiri);
   if (vrt == 0)
   {
      vrt = toka->vertausluku - eka->vertausluku;
   }
   return vrt;
}

/* Järjestää qsortin avulla taulukon oliot puolueen nimen mukaiseen järjestykseen */
int puolueVrt(const void *a, const void *b)
{
   Puolue *eka = (Puolue *) a;
   Puolue *toka = (Puolue *) b;
   int vrt = strcmp(eka->nimi, toka->nimi);
   return vrt;
}

/* Järjestää qsortin avulla taulukon oliot vaalipiirin nimen mukaiseen järjestykseen */
int vaalipiiriVrt(const void *a, const void *b)
{
   Vaalipiiri *eka = (Vaalipiiri *) a;
   Vaalipiiri *toka = (Vaalipiiri *) b;
   int vrt = strcmp(eka->nimi, toka->nimi);
   return vrt;
}

/* Järjestää qsortin avulla taulukon oliot ensisijaisesti vaalipiirin,
   toissijaisesti puolueen ja kolmanneksi vielä ehdokkaan nimen mukaiseen järjestykseen */
int ehdokasVrt(const void *a, const void *b)
{
   Ehdokas *eka = (Ehdokas *) a;
   Ehdokas *toka = (Ehdokas *) b;
   int vrt = strcmp(eka->vaalipiiri, toka->vaalipiiri);
   if (vrt == 0)
   {
      vrt = strcmp(eka->puolue, toka->puolue);
      if (vrt == 0)
      {
         vrt = strcmp(eka->nimi, toka->nimi);
      }
   }
   return vrt;
}

/* Tulostaa puolueet ja jos tulos on laskettu, tulostetaan myös puolueesta valitut kansanedustajat */
void tulostaPuolueet(PuolueTaulu *pt, EhdokasTaulu *et, int tulosLaskettu)
{
   int i;
   int j;
   qsort(pt->taulu, pt->n, sizeof(Puolue), puolueVrt);
   if (tulosLaskettu)
   {
      qsort(et->taulu, et->n, sizeof(Ehdokas), nimiVrt);
      for(i = 0; i < pt->n; i++)
      {
         printf("%s (%d)\n", pt->taulu[i].nimi, pt->taulu[i].kansanedustajia);
         for (j = 0; j < et->n; j++)
         {
            if (et->taulu[j].valittiinko && strcmp(et->taulu[j].puolue, pt->taulu[i].nimi) == 0)
            {
               printf("  %s %s %d\n", et->taulu[j].nimi, et->taulu[j].vaalipiiri, et->taulu[j].aanimaara);
            }
         }
      }
   }
   else
   {
      for(i = 0; i < pt->n; i++)
      {
         printf("%s\n", pt->taulu[i].nimi);
      }
   }
   
}

/* Tulostaa vaalipiirit ja jos tulos on laskettu, tulostetaan myös vaalipiiristä valitut kansanedustajat */
void tulostaVaalipiirit(VaalipiiriTaulu *vt, EhdokasTaulu *et, int tulosLaskettu)
{
   int i;
   int j;
   qsort(vt->taulu, vt->n, sizeof(Vaalipiiri), vaalipiiriVrt);
   if (tulosLaskettu)
   {
      qsort(et->taulu, et->n, sizeof(Ehdokas), puolueNimiVrt);
      for(i = 0; i < vt->n; i++)
      {
         if (vt->taulu[i].lkm == 1)
         {
            printf("%s (%d edustaja)\n", vt->taulu[i].nimi, vt->taulu[i].lkm);
         }
         else
         {
            printf("%s (%d edustajaa)\n", vt->taulu[i].nimi, vt->taulu[i].lkm);
         }
         for (j = 0; j < et->n; j++)
         {
            if (et->taulu[j].valittiinko && strcmp(et->taulu[j].vaalipiiri, vt->taulu[i].nimi) == 0)
            {
               printf("  %s %s %d\n", et->taulu[j].nimi, et->taulu[j].puolue, et->taulu[j].aanimaara);
            }
         }
      }
   }
   else
   {
      for(i = 0; i < vt->n; i++)
      {
         if (vt->taulu[i].lkm == 1)
         {
            printf("%s (%d edustaja)\n", vt->taulu[i].nimi, vt->taulu[i].lkm);
         }
         else
         {
            printf("%s (%d edustajaa)\n", vt->taulu[i].nimi, vt->taulu[i].lkm);
         }
      }
   }
}

/* Tulostaa ehdokkaat ja jos tulos on laskettu, tulostetaan eduskuntaan valittujen kohdalle myös tieto valinnasta */
void tulostaEhdokkaat(EhdokasTaulu *et, int tulosLaskettu)
{
   int i;
   qsort(et->taulu, et->n, sizeof(Ehdokas), ehdokasVrt);
   for(i = 0; i < et->n; i++)
   {
      if (et->taulu[i].valittiinko == 1)
      {
         printf("%s %s %s %d (valittu)\n", et->taulu[i].nimi, et->taulu[i].puolue, et->taulu[i].vaalipiiri, et->taulu[i].aanimaara);
      }
      else
      {
         printf("%s %s %s %d\n", et->taulu[i].nimi, et->taulu[i].puolue, et->taulu[i].vaalipiiri, et->taulu[i].aanimaara);
      }
   }
}

/* Laskee vaalien tuloksen */
void laskeTulos(PuolueTaulu *pt, VaalipiiriTaulu *vt, EhdokasTaulu *et)
{
   int i = 0;
   int j = 0;
   int k = 0;
   int lkm = 0;
   /* Järjestetään ehdokkaat vaalipiirin ja äänimäärän mukaiseen järjestykseen */
   qsort(et->taulu, et->n, sizeof(Ehdokas), vaalipiiriAanimaaraVrt);
   /* Käydään läpi kaikki ehdokkaat */
   for (i = 0; i < et->n; i++)
   {
      /* Käydään läpi vaalipiiri */
      while(j < et->n && strcmp(et->taulu[i].vaalipiiri, et->taulu[j].vaalipiiri) == 0)
      {
         /* Kasvatetaan ehdokkaan puolueen vertauslukua käsiteltävässä vaalipiirissä */
         for (k = 0; k < pt->n; k++)
         {
            if (strcmp(et->taulu[j].puolue, pt->taulu[k].nimi) == 0)
            {
               pt->taulu[k].vertausluku = pt->taulu[k].vertausluku + et->taulu[j].aanimaara;
            }
         }
         j++;
      }
      /* Siirrytään takaisin käsiteltävän vaalipiirin alkuun */
      j = i;
      /* Käydään vaalipiiri uudelleen läpi ja lasketaan ehdokkaiden vertausluvut */
      while(j < et->n && strcmp(et->taulu[i].vaalipiiri, et->taulu[j].vaalipiiri) == 0)
      {
         for (k = 0; k < pt->n; k++)
         {
            if (strcmp(et->taulu[j].puolue, pt->taulu[k].nimi) == 0)
            {
               /* Vertausluku on puolueen vertausluku jaettuna ehdokkaan sijoituksella puolueessa äänimäärän mukaan laskettuna */
               pt->taulu[k].ehdokkaita++;
               et->taulu[j].vertausluku = (pt->taulu[k].vertausluku / pt->taulu[k].ehdokkaita);
            }
         }
         j++;
      }
      /* Kasvatetaan i:tä niin, että seuraavalla kierroksella käydään läpi seuraava vaalipiiri */
      i = j-1;
      /* Nollataan puolueiden vertausluvut ja tieto siitä monesko ehdokas on menossa */
      for (k = 0; k < pt->n; k++)
      {
         pt->taulu[k].vertausluku = 0;
         pt->taulu[k].ehdokkaita = 0;
      }
   }

   /* Lajitellaan ehdokkaat vaalipiirin ja vertausluvun mukaan, jotta voidaan laskea kenet on valittu eduskuntaan */
   qsort(et->taulu, et->n, sizeof(Ehdokas), vaalipiiriVertauslukuVrt);
   j = 0;
   /* Käydään läpi kaikki vaalipiirit */
   for (i = 0; i < vt->n; i++)
   {
      lkm = vt->taulu[i].lkm;
      /* Käydään läpi kaikki ehdokkaat */
      for (j = 0; j < et->n; j++)
      {
         /* Jos ehdokas on tutkittavasta vaalipiiristä ja vaalipiiristä mahtuu vielä ehdokkaita
            eduskuntaan, merkataan hänet valituksi */
         if (lkm && strcmp(vt->taulu[i].nimi, et->taulu[j].vaalipiiri) == 0)
         {
            et->taulu[j].valittiinko = 1;
            /* Kasvatetaan valitun edustajan puolueen kansanedustaja-laskuria */
            for (k = 0; k < pt->n; k++)
            {
               if (strcmp(et->taulu[j].puolue, pt->taulu[k].nimi) == 0)
               {
                  pt->taulu[k].kansanedustajia++;
               }
            }
            lkm--;
         }
      }
   }
}