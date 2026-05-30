#ifndef DCW_BROWSE_MAIN_WINDOW_H
#define DCW_BROWSE_MAIN_WINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class dcw_browse_Main_Window; }
QT_END_NAMESPACE

class dcw_browse_Main_Window : public QMainWindow
{
    Q_OBJECT

public:
    dcw_browse_Main_Window(QWidget *parent = nullptr);
    ~dcw_browse_Main_Window();

public slots:

   void exit_action_slot();

private:
    Ui::dcw_browse_Main_Window *ui = nullptr;
};
#endif // DCW_BROWSE_MAIN_WINDOW_H

/* EOF */

