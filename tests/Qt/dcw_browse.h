//-------------------------------------------------------------------------
// File : dcw_browse.h
// Date : 29-May-26 : initial definition
//
// Description:
//    A set of DCW browse utilities for use by the Qt version of the
//    'dcw_browse' application.
//-------------------------------------------------------------------------
#ifndef DCW_BROWSE_
#define DCW_BROWSE_

#include <QWidget>

class DCW_Browse : public QWidget
{
    Q_OBJECT

public:

   DCW_Browse(QWidget *parent = nullptr);

   ~DCW_Browse();

protected:

   void paintEvent(QPaintEvent *event) override;

private:

};

#endif

/* EOR */

