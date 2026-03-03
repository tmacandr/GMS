//-----------------------------------------------------------------------------
// File : gmsDataTablesClass.h
// Date : 14-Jan-00 : initial definition
//
// Description:
//    This class defines an object that is made up of tables of data from
//    the DCW database.  Specifically, an object of this class contains
//    the following tables of information:
//
//           Database Level Data:
//           --------------------
//              1) DHT - Database Header Table
//              2) LAT - Library Attribute Table
//
//           Library Level Data:
//           -------------------
//              1) CAT - Coverage Attribute Table
//              2) DQT - Data Quality Table
//              3) GRT - Geographic Reference Table
//              4) LHT - Library Header Table
//
//    The "library" tables apply to both REGIONAL and BROWSE libraries.
//
// Copyright (c) 1999-2026, Timothy MacAndrew, all rights reserved
//-----------------------------------------------------------------------------

#ifndef GMS_TABLES_CLASS

   #define GMS_TABLES_CLASS

   class gmsDataTablesClass
      {
       public:

          //-----------------------------------------------
          // gmsDataTablesClass
          //
          // Description:
          //    Constructor.  Creates an object that
          //    represents the data tables from a DCW
          //    database.
          //-----------------------------------------------
          gmsDataTablesClass ();


          //-----------------------------------------------
          // ~gmsDataTablesClass
          //
          // Description:
          //    Destructor.  Deletes the object
          //-----------------------------------------------
          ~gmsDataTablesClass ();


          //-----------------------------------------------
          // gmsGet_DHT
          //
          // Description:
          //    This method retrieves the "database header
          //    table" from the DCW database.
          //-----------------------------------------------
          const char *gmsGet_DHT ();


          //-----------------------------------------------
          // gmsGet_LAT
          //
          // Description:
          //    This method retrieves the "library attribute
          //    table" from the DCW database.
          //-----------------------------------------------
          const char *gmsGet_LAT ();


          //-----------------------------------------------
          // gmsGet_CAT
          //
          // Description:
          //    This method retrieves the "Coverage Attribute
          //    Table" from either the current REGIONAL library
          //    or the BROWSE library.  A string that is the
          //    data is returned to the caller.
          //-----------------------------------------------
          const char *gmsGet_CAT
                         (bool isBrowse);


          //-----------------------------------------------
          // gmsGet_DQT
          //
          // Description:
          //    This method retrieves the "Data Quality
          //    Table" from either the current REGIONAL library
          //    or the BROWSE library.  A string that is the
          //    data is returned to the caller.
          //-----------------------------------------------
          const char *gmsGet_DQT
                         (bool isBrowse);


          //-----------------------------------------------
          // gmsGet_GRT
          //
          // Description:
          //    This method retrieves the "Geographic Reference
          //    Table" from either the current REGIONAL library
          //    or the BROWSE library.  A string that is the
          //    data is returned to the caller.
          //-----------------------------------------------
          const char *gmsGet_GRT
                         (bool isBrowse);


          //-----------------------------------------------
          // gmsGet_LHT
          //
          // Description:
          //    This method retrieves the "Library Header
          //    Table" from the current REGIONAL library and
          //    returns a string to the caller.
          //-----------------------------------------------
          const char *gmsGet_LHT
                         (bool isBrowse);

       private:

          void *theTables;

      };

#endif
