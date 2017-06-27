/*****************************************************************************************
***                                                                                    ***
***             php-wbootstrap is a tool able to execute the good version of php       ***
***             according wampserver configuration                                     ***
***  --------------------------------------------------------------------------------  ***
***                                                                                    ***
***  DESCRIPTION:                                                                      ***
***  This code execute the good PHP binary version according the wamp configuration.   ***
***  Place this php.exe in C:\wamp\ (installation directory) and make sure that        ***
***  C:\wamp is in the PATH system.                                                    ***
***                                                                                    ***
***  --------------------------------------------------------------------------------  ***
***  Date         | Author     | Version   | Description                               ***
***  --------------------------------------------------------------------------------  ***
***  2017-05-16   | Clément G. | 1.0.0     | Creation                                  ***
***                                                                                    ***
*****************************************************************************************/


/***********************************
****         INCLUDES           ****
***********************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/***********************************
****          DEFINES           ****
***********************************/

/* Messages */
#define MSG_ERR_001 ("ERR-001: Unable to open the ini file.\n")

/* Configuration */
#define NB_CHAR_STR    255   /* Number of maximum characters managed by string */


/***********************************
****          TYPEDEF           ****
***********************************/

/* type */
typedef int                bool;

/***********************************
****        DECLARATION         ****
***********************************/

/* Variables */

/* Function prototype */
int   ini_get(char *file, char *section, char *key, char *value);
char *strrep(char *string, char chr_dst, char chr_src);
char *freadline(char *p_output, FILE *p_file);


/***********************************
****        FUNCTIONS         ****
***********************************/
/*!
  \Function      main

  \brief         Entry point of application
                    - Manage option in arguments

  \param[in]     int argc:      Number of options in argument
                  char *argv[]:  List of options in argument

  \param[out]    None

  \retval[]      int
                    - EXIT_SUCCESS: No error occured
                    - EXIT_FAILURE: An error occured
!*/
int main (int argc, char *argv[])
{
  int      Lc_ReturnCode = EXIT_SUCCESS;
  int      i;
  char     Ls_PhpValueTemp[NB_CHAR_STR];
  char     Ls_Arguments[NB_CHAR_STR];
  char     Ls_CmdSys[NB_CHAR_STR];
  char    *Ls_PhpVersion;

  /* Initialization */
  memset(Ls_PhpValueTemp, 0, NB_CHAR_STR);
  memset(Ls_Arguments,    0, NB_CHAR_STR);
  memset(Ls_CmdSys,       0, NB_CHAR_STR);

  /* Build arguments path */
  for(i=1; i<argc; i++)
  {
    strcat(Ls_Arguments, " ");
    strcat(Ls_Arguments, argv[i]);
  }
  
  if(ini_get("C:\\wamp\\wampmanager.conf", "php", "phpVersion ", Ls_PhpValueTemp) == EXIT_SUCCESS)
  {
    Ls_PhpValueTemp[strlen(Ls_PhpValueTemp)-1] = 0;
    Ls_PhpVersion = Ls_PhpValueTemp + 2;
  }
  else
  {
    Lc_ReturnCode = EXIT_FAILURE;
  }
  
  if(Lc_ReturnCode == EXIT_SUCCESS)
  {
    strcat(Ls_CmdSys, "call C:\\wamp\\bin\\php\\php");
    strcat(Ls_CmdSys, Ls_PhpVersion);
    strcat(Ls_CmdSys, "\\php.exe");
    strcat(Ls_CmdSys, Ls_Arguments);
    
    printf("PHP Version: %s\n", Ls_PhpVersion);
  //  printf("Command: %s\n\", Ls_CmdSys);
    printf("\n");
    
    system(Ls_CmdSys);
  }

  return Lc_ReturnCode;
}

/*!
  \Function      ini_get

  \brief         Return the key value from a ini file

  \param[in]     char *file: Path of the ini file
                 char *section: Section related to the key to find
                 char *key: Key to find

  \param[out]    char *value: String of the value

  \retval[]      int
                    - EXIT_SUCCESS: No error occured
                    - EXIT_FAILURE: Ini file problem or SW integration problem
!*/
int   ini_get(char *file, char *section, char *key, char *value)
{
  int      Lc_ReturnCode = EXIT_SUCCESS;
  FILE    *Lp_InFile = NULL;
  char     Ls_Line[NB_CHAR_STR];
  char    *Lpc_Result = NULL;
  char     Ls_SectionTemp[NB_CHAR_STR];
  bool     Lb_InSection = 0;
  char    *Lpc_KeySep = NULL;
  char     Ls_KeyRead[NB_CHAR_STR];
  bool     Lb_ExitLoop = 0;

  if((file != NULL) && (section != NULL) && (key != NULL) && (value != NULL))
  {
    Lp_InFile = fopen(file, "r");

    /* Build Temp Section */
    memset(Ls_SectionTemp, 0, NB_CHAR_STR);
    strcat(Ls_SectionTemp, "[");
    strcat(Ls_SectionTemp, section);
    strcat(Ls_SectionTemp, "]");

    if(Lp_InFile != NULL)
    {
      do
      {
        memset(Ls_Line, 0, NB_CHAR_STR);
        Lpc_Result = freadline(Ls_Line, Lp_InFile);

        if(strcmp(Ls_Line, Ls_SectionTemp) == 0)
        {
          Lb_InSection = 1;
        }
        else
        {
          if(Ls_Line[0] == '[')
          {
            Lb_InSection = 0;
          }
          else
          {
            /* Section found */
            if(Lb_InSection == 1)
            {
              Lpc_KeySep = strchr(Ls_Line, '=');
              if(Lpc_KeySep != NULL)
              {
                memset(Ls_KeyRead, 0, NB_CHAR_STR);
                memcpy(Ls_KeyRead, Ls_Line, Lpc_KeySep-Ls_Line);
                /* Key found */
                if(strcmp(Ls_KeyRead, key) == 0)
                {
                  memcpy(value, Lpc_KeySep+1, strlen(Ls_Line)-(Lpc_KeySep-Ls_Line+1));
                  Lb_ExitLoop = 1;
                }
              }
            }
          }
        }
      }
      while(((*Lpc_Result) != EOF) && (Lb_ExitLoop == 0));

      fclose(Lp_InFile);
    }
    else
    {
      printf(MSG_ERR_001);
      Lc_ReturnCode = EXIT_FAILURE;
    }
  }
  else
  {
    Lc_ReturnCode = EXIT_FAILURE;
  }

  return Lc_ReturnCode;
}

/*!
  \Function      freadline

  \brief         Read a line in a FILE system. The file shall be formatted with \n endline.

  \param[in]     FILE *p_file: Pointer to the FILE object

  \param[out]    char *p_output: Line read

  \retval[]      char *: p_output or EOF
!*/
char * freadline(char *p_output, FILE *p_file)
{
  char *Lp_Return = NULL;
  char  Lc_Char = 0;
  int   Li_pos = 0;

  if((p_file != NULL) && (p_output != NULL))
  {
    fread(&Lc_Char, 1, 1, p_file);
    while((Lc_Char != EOF) && (Lc_Char!='\n') && (Lc_Char != '\r'))
    {
      memcpy(p_output+Li_pos, &Lc_Char, 1);
      Li_pos++;
      fread(&Lc_Char, 1, 1, p_file);
    }
    if(Lc_Char != EOF)
    {
      p_output[Li_pos] = 0;
    }
    else
    {
      p_output[0] = EOF;
    }
    Lp_Return = p_output;
  }

  return Lp_Return;
}





