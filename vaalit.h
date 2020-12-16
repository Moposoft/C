struct Ehdokas
{
   char *nimi;
   char *puolue;
   char *vaalipiiri;
   int aanimaara;
   int vertausluku;
   int valittiinko;
};

typedef struct Ehdokas Ehdokas;

struct EhdokasTaulu
{
   Ehdokas *taulu;
   size_t kapasiteetti;
   size_t n;
};

typedef struct EhdokasTaulu EhdokasTaulu;

struct Vaalipiiri
{
   char *nimi;
   int lkm;
};

typedef struct Vaalipiiri Vaalipiiri;

struct VaalipiiriTaulu
{
   Vaalipiiri *taulu;
   size_t kapasiteetti;
   size_t n;
};

typedef struct VaalipiiriTaulu VaalipiiriTaulu;

struct Puolue
{
   char *nimi;
   int kansanedustajia;
   /* Puolueen vertausluku kullakin hetkellä tutkittavassa vaalipiirissä */
   int vertausluku;
   /* Puolueen ehdokkaiden lukumäärä kullakin hetkellä tutkittavassa vaalipiirissä*/
   int ehdokkaita;
};

typedef struct Puolue Puolue;

struct PuolueTaulu
{
   Puolue *taulu;
   size_t kapasiteetti;
   size_t n;
};

typedef struct PuolueTaulu PuolueTaulu;

char** lueKomento(int *osia);
EhdokasTaulu * luoEhdokasTaulu(void);
VaalipiiriTaulu * luoVaalipiiriTaulu(void);
PuolueTaulu * luoPuolueTaulu(void);
void lisaaPuolue(PuolueTaulu *pt, const char* nimi);
void lisaaVaalipiiri(VaalipiiriTaulu *vt, const char* nimi, int lkm);
void lisaaEhdokas(EhdokasTaulu *et, const char* nimi, const char *puolue, const char *vaalipiiri, int aanimaara);
void tulostaPuolueet(PuolueTaulu *pt, EhdokasTaulu *et, int tulosLaskettu);
void tulostaVaalipiirit(VaalipiiriTaulu *vt, EhdokasTaulu *et, int tulosLaskettu);
void tulostaEhdokkaat(EhdokasTaulu *et, int tulosLaskettu);
void laskeTulos(PuolueTaulu *pt, VaalipiiriTaulu *vt, EhdokasTaulu *et);