#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define C0 299792458            // Speed of light in m/s
#define MAX_FREQUENCY_LENGTH 50 //

int main(int argc, char *argv[])
{
  char sfreq[MAX_FREQUENCY_LENGTH] = {'\0'};
  char pfreq[MAX_FREQUENCY_LENGTH] = {'\0'};

  /* lower case all */
  for (int i = 1; i < argc; ++i)
    for (int j = 0; argv[i][j]; j++)
      argv[i][j] = tolower(argv[i][j]);

  /* check for postfix without white space */
  if (argc == 2)
  {
    int m = 0, n = 0;
    for (int i = 0; argv[1][i]; ++i)
    {
      if (isalpha(argv[1][i]))
        pfreq[n++] = argv[1][i];
      else
        sfreq[m++] = argv[1][i];
    }
    sfreq[m] = '\0';
    pfreq[n] = '\0';
  }

  /* check for postfix after white space */
  if (argc == 3)
  {
    snprintf(sfreq, MAX_FREQUENCY_LENGTH, "%s", argv[1]);
    snprintf(pfreq, MAX_FREQUENCY_LENGTH, "%s", argv[2]);
  }
  printf("Frequency number: %f %s\n", atof(sfreq), pfreq);

  /* calculate wavelength */
  double wlen = 0;
  if (pfreq[0] == '\0')
    wlen = (float)(C0) / atof(sfreq);
  else if (strcmp(pfreq, "hz") == 0)
    wlen = (float)(C0) / (atof(sfreq) * 1E0);
  else if (strcmp(pfreq, "khz") == 0)
    wlen = (float)(C0) / (atof(sfreq) * 1E3);
  else if (strcmp(pfreq, "mhz") == 0)
    wlen = (float)(C0) / (atof(sfreq) * 1E6);
  else if (strcmp(pfreq, "ghz") == 0)
    wlen = (float)(C0) / (atof(sfreq) * 1E9);
  else if (strcmp(pfreq, "thz") == 0)
    wlen = (float)(C0) / (atof(sfreq) * 1E12);
  else
  {
    printf("Unrecognized freqeuncy postfix: %s\n", pfreq);
    return 1;
  }

  printf("Wavelength %f [m]\n", wlen);
  // char ans[MAX_FREQUENCY_LENGTH];
  // sprintf(ans, "ANS=%f", wlen);
  // _putenv("AND=Deneeme");

  printf("Speed of light: %f\n", (float)C0);
  // printf("Speed of light: %f\n", (float)C0);
  return 0;
}