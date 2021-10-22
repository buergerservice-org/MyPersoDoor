//mainwindow.h - MyPersoDoor - GUI for persodoor
//Copyright (C) 2021 buergerservice.org e.V. <KeePerso@buergerservice.org>
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStringList>
#include "workflowLibrary.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void on_startButton_clicked();
    void on_stopButton_clicked();
    void on_adduserButton_clicked();
    void on_deluserButton_clicked();

    void on_MenuEdit_PIN_triggered();
    void on_MenuHilfe_triggered();
    void on_MenuUeber_triggered();
    void on_MenuZertifikat_triggered();
    void showlog();
    void writedata();

    QList<QStringList> datab;
    workflowLibrary::workflow wf;

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
