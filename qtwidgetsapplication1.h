#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_mainwindow.h"

#include "workflowLibrary.h"


class QtWidgetsApplication1 : public QMainWindow
{
    Q_OBJECT

public:
    QtWidgetsApplication1(QWidget *parent = Q_NULLPTR);
    Ui::QtWidgetsApplication1Class ui;
    //void on_GenButton_clicked();
    //void on_InputButton_clicked();
    //void on_DeleteButton_clicked();

    //void on_MenuOeffnen_triggered();
    //void on_MenuSpeichern_triggered();
    //void on_MenuHilfe_triggered();
    //void on_MenuUeber_triggered();
    //void on_MenuZertifikat_triggered();

    workflowLibrary::workflow w;

private:

};
