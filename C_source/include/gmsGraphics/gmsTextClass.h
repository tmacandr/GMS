//-----------------------------------------------------------------------------
// File : gmsTextClass.h
// Date : 13-Jan-00 : initial definition
//
// Description:
//    This class defines a "text" object.  A text table from the DCW data-
//    base is abstracted by this object.  The text strings obtained from
//    this object are used to annotate a particular map.
//
// Copyright (c) 1999-2026, Timothy MacAndrew, all rights reserved
//-----------------------------------------------------------------------------

#ifndef GMS_TXT_CLASS

   #define GMS_TXT_CLASS

   #include <gmsTypesAndConstants.h>

   typedef struct
      {
       int                    numChars;
       char                   *theString; /* is null terminated */
       gms_2D_ScreenPointType theCoord;
      } gmsMapTextStructType;

   typedef struct
      {
       int                  numTextRecords;
       gmsMapTextStructType *textRecords;
      } gmsMapTextArrayType;


   class gmsTextClass
      {
       public:

          //-----------------------------------------------
          // gmsTextClass
          //
          // Description:
          //    Constructor.  Creates an object that
          //    represents a series of text items from
          //    a DCW text table.
          //-----------------------------------------------
          gmsTextClass
             (gmsEarthModelType whichModel,
              const char        *txtTblName);


          //-----------------------------------------------
          // ~gmsTextClass
          //
          // Description:
          //    Destructor.  Deletes the text object
          //-----------------------------------------------
          ~gmsTextClass ();


          //-----------------------------------------------
          // gmsGetTextItems
          //
          // Description:
          //    This method builds an array of text strings
          //    that annotate a map.
          //-----------------------------------------------
          gmsMapTextArrayType gmsGetTextItems ();

       private:

          void              *theTxt;

          gmsEarthModelType theModel;

      };

#endif
