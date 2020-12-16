#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vaalit.h"

int main(int argc, char const *argv[])
{
   int i;
   int osia = 0;
   int jatketaan = 1;
   int tulosLaskettu = 0;
   char** komento = NULL;
   /* Luodaan taulut */
   EhdokasTaulu *et = luoEhdokasTaulu();
   VaalipiiriTaulu *vt = luoVaalipiiriTaulu();
   PuolueTaulu *pt = luoPuolueTaulu();
   /* Pääsilmukka */
   while(jatketaan)
   {
      /* Luetaan komento */
      komento = lueKomento(&osia);
      /* Toimitaan saadun komennon mukaan */
      if (strcmp(komento[0],"lisaaPuolue") == 0)
      {
         lisaaPuolue(pt, komento[1]);
         tulosLaskettu = 0;
      }
      else if (strcmp(komento[0],"lisaaVaalipiiri") == 0)
      {
         lisaaVaalipiiri(vt, komento[1], atoi(komento[2]));
         tulosLaskettu = 0;
      }
      else if (strcmp(komento[0],"lisaaEhdokas") == 0)
      {
         lisaaEhdokas(et, komento[1], komento[2], komento[3], atoi(komento[4]));
         tulosLaskettu = 0;
      }
      else if (strcmp(komento[0],"laskeTulos") == 0)
      {
         laskeTulos(pt, vt, et);
         tulosLaskettu = 1;
      }
      else if (strcmp(komento[0],"puolueet") == 0)
      {
         printf("Puolueet:\n");
         tulostaPuolueet(pt, et, tulosLaskettu);
      }
      else if (strcmp(komento[0],"vaalipiirit") == 0)
      {
         printf("Vaalipiirit:\n");
         tulostaVaalipiirit(vt, et, tulosLaskettu);
      }
      else if (strcmp(komento[0],"ehdokkaat") == 0)
      {
         printf("Ehdokkaat:\n");
         tulostaEhdokkaat(et, tulosLaskettu);
      }
      else if (strcmp(komento[0],"lopeta") == 0)
      {
         jatketaan = 0;
      }
      /* Vapautetaan komento lopuksi */
      for (i = 0; i < osia; i++)
      {
         free(komento[i]);
      }
      free(komento);
   }
   return 0;
}