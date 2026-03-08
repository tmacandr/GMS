//----------------------------------------------------------------------------*/
// File : util_test.c
// Date : 21-Dec-99 : initial defintion
//        22-Dec-99 : more subtests
//        11-Feb-00 : expand lists of dirs to test for existence
//
// Description:
//    Console application to exercise the "gmsUtilities" component.
//
// Copyright (c) 1999-2026, Timothy MacAndrew, all rights reserved
//----------------------------------------------------------------------------*/

#include <stdio.h>
#include <gmsUtilities.h>

static void existenceSubtest ();

static void dcwPathSubtest ();

static void dirExistsSubtest ();



//-------------------------------------*/
// main
//
// Description:
//    Main test driver.
//-------------------------------------*/
int main (int  argc,
          char **argv)

{
   printf("_______________________________\n");
   printf("Begin : GMS Utilities Test\n");

   existenceSubtest();

   dcwPathSubtest ();

   dirExistsSubtest ();

   printf("End : GMS Utilities Test\n");
   printf("_______________________________\n");

   return 0;
}


//---------------------------------------------*/
// existenceSubtest
//
// Description:
//    Exercises the 'gmsFileExists' utility.
//---------------------------------------------*/
static void existenceSubtest ()

{
         const char *theFile = { "./test_file.txt" };
         bool exists;
         FILE *fd;

   printf("---> Does '%s' exist?\n", theFile);

   exists = gmsFileExists (theFile);

   if (exists)
      {
       printf("*****************\n");
       printf("FAIL : file %s already exists.  Test FAILS!\n",
              theFile);
       printf("*****************\n");

       return;
      }

   printf("---> OK, file does NOT exist, create it ...\n");

   fd = fopen(theFile, "w");

   if (fd == NULL)
      {
       printf("****_______****_______****_______\n");
       printf("FAILED to create file %s\n", theFile);
       printf("****_______****_______****_______\n");

       return;
      }

   fclose(fd);

   exists = gmsFileExists (theFile);

   if ( ! exists)
      {
       printf("*****************\n");
       printf("FAIL : file %s does NOT exists.  Test FAILS!\n",
              theFile);
       printf("*****************\n");

       return;
      }

   printf("---> OK, good-to-go.\n");
}


//---------------------------------------------*/
// dcwPathSubtest
//
// Description:
//    Exercises the 'gmsGetDcwFullPath', et.al.
//    utilities.
//---------------------------------------------*/
static void dcwPathSubtest ()

{
      char *dcwPath;

   dcwPath = getDhtFile();

   printf("---> DHT file path : %s\n", dcwPath);

   dcwPath = getLatFile ();

   printf("---> LAT file path : %s\n", dcwPath);

   dcwPath = gmsGetDcwFullPath("GRF");

   printf("---> GRF file path : %s\n", dcwPath);

   dcwPath = gmsGetBrowseFullPath ("DQT");

   printf("---> Browse DQT file path : %s\n", dcwPath);
}


//---------------------------------------------*/
// dirExistsSubtest
//
// Description:
//    Exercises the utility 'gmsDirectoryExists'.
//---------------------------------------------*/
static void dirExistsSubtest ()

{
      #define Num_Dirs 7
      char    *theDirs[Num_Dirs] = { (char *) "/temp",
                                     (char *) "/tmp",
                                     (char *) "/MyFiles",
                                     (char *) "/",
                                     (char *) "/shit",
                                     (char *) "/bull_shit",
                                     (char *) "/way_too_much_shit" };
      bool    dirExists;
      int     i;

   for (i = 0; i < Num_Dirs; i++)
      {
       dirExists = gmsDirectoryExists (theDirs[i]);

       printf("---> The directory >>%s<< exists : \t %d\n",
              theDirs[i],
              dirExists);
      }
}

