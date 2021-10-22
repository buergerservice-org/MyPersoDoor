//main.cpp - MyPersoDoor - GUI for persodoor
//Copyright (C) 2021 buergerservice.org e.V. <KeePerso@buergerservice.org>

#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
