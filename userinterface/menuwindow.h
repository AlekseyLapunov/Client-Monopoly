#ifndef MENUWINDOW_H
#define MENUWINDOW_H

#include <QMainWindow>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QMessageBox>

#include "helpers/basewin.h"

namespace Ui {
class MenuWindow;
}

class MenuWindow : public QMainWindow, public BaseWin
{
    Q_OBJECT

public:
    explicit MenuWindow(QWidget *parent = nullptr);
    ~MenuWindow();

private slots:
    void quitApp();

private:
    Ui::MenuWindow *ui;
};

#endif // MENUWINDOW_H
