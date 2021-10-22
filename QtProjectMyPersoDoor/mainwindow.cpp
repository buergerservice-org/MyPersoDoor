//mainwindow.cpp - MyPersoDoor - GUI for persodoor
//Copyright (C) 2021 buergerservice.org e.V. <KeePerso@buergerservice.org>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QInputDialog>
#include <QAction>
#include <QFile>
#include <QFileDialog>
//#include <QClipboard>
#include <QTextStream>
#include <QDataStream>
#include <QProcess>
#include <QTimer>
//#include <QApplication>

#include <string>
#include <stdint.h>
#include <sstream>


inline unsigned int stoui(const std::string& s)
{
    std::istringstream reader(s);
    unsigned int val = 0;
    reader >> val;
    return val;
}


std::string commandexec(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);

    if (!pipe)
    {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr)
    {
        result += buffer.data();
    }

    return result;
}


void MainWindow::writedata()
{
    QMessageBox msgBox;
    QFile file("database.txt");
    QFile hashfile("hashkey.txt");

    //write database.txt
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setWindowTitle("ERROR");
        msgBox.setText("ERROR - database.txt kann nicht geoeffnet werden.");
        msgBox.exec();
        return;
    }
    QTextStream out(&file);
    for(int i=0; i< datab.count(); i++)
    {
        if (datab[i][0]!="")
        {
            out << datab[i][0] << ";" << datab[i][1] << "\n";
        }
    }

    //write hashkey.txt
    if(!hashfile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setWindowTitle("ERROR");
        msgBox.setText("ERROR - hashkey.txt kann nicht geoeffnet werden.");
        msgBox.exec();
        return;
    }
    QTextStream hashout(&hashfile);
    for(int i=0; i< datab.count(); i++)
    {
        if (datab[i][1]!="")
        {
            hashout << datab[i][1] << "\n";
        }
    }
}


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    QMessageBox msgBox;
    ui->setupUi(this);

    connect(ui->startButton, &QPushButton::clicked, this, &MainWindow::on_startButton_clicked);
    connect(ui->stopButton, &QPushButton::clicked, this, &MainWindow::on_stopButton_clicked);
    connect(ui->adduButton, &QPushButton::clicked, this, &MainWindow::on_adduserButton_clicked);
    connect(ui->deluButton, &QPushButton::clicked, this, &MainWindow::on_deluserButton_clicked);

    connect(ui->actionEdit_PIN, &QAction::triggered, this, &MainWindow::on_MenuEdit_PIN_triggered);
    //connect(ui.actionSpeichern, &QAction::triggered, this, &QtWidgetsApplication1::on_MenuSpeichern_triggered);
    connect(ui->actionHilfe, &QAction::triggered, this, &MainWindow::on_MenuHilfe_triggered);
    connect(ui->actionUeber, &QAction::triggered, this, &MainWindow::on_MenuUeber_triggered);
    connect(ui->actionZertifikat, &QAction::triggered, this, &MainWindow::on_MenuZertifikat_triggered);

    //read database
    QFile file("database.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setWindowTitle("ERROR");
        msgBox.setText("ERROR - database.txt kann nicht geoeffnet werden.");
        msgBox.exec();
        return;
    }
    QTextStream in(&file);
    while(!in.atEnd())
    {
        QString line=in.readLine();
        QStringList splitline=line.split(';');
        ui->listWidget->addItem(splitline[0]);
        datab.append(splitline);
    }
    ui->listWidget->repaint();

    //start show the log every 1,5sec
    QTimer* timer = new QTimer();
    timer->setInterval(1500);
    connect(timer, &QTimer::timeout, this, &MainWindow::showlog);
    timer->start();

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::showlog()
{
    std::string outputstring;
    outputstring = commandexec("tail -n 7 persodoor.log");
    ui->textEdit->setPlainText(QString::fromStdString(outputstring));
}



void MainWindow::on_adduserButton_clicked()
{
    //ui.Label1->setText("clicked");
    QString curitem;
    QString wfoutput;
    QString PIN;
    std::string outputstring;
    std::string addstr;
    QMessageBox msgBox;
    QInputDialog diaBox;

    bool ok;

    QString itext = QInputDialog::getText(this,
        tr(""),
        tr("Benutzername:"),
        QLineEdit::Normal,
        "",
        &ok);


    outputstring = wf.getkeypad();
    msgBox.setIcon(QMessageBox::Critical);
    msgBox.setWindowTitle("ERROR");
    if (outputstring == "e1")
    {
        msgBox.setText("ERROR - please check AusweisApp2, cardreader and Personalausweis!");
        msgBox.exec();
        return;
    }
    else if (outputstring == "e2")
    {
        msgBox.setText("ERROR - please check your Personalausweis!");
        msgBox.exec();
        return;
    }
    else if (outputstring == "e3")
    {
        msgBox.setText("ERROR - please check your cardreader!");
        msgBox.exec();
        return;
    }
    else if (outputstring == "e4")
    {
        msgBox.setText("ERROR - AusweisApp2-version less than 1.22.* please update!");
        msgBox.exec();
        return;
    }
    else if (outputstring == "e5")
    {
        msgBox.setText("Warning - retryCounter of Perso <3, please start a selfauthentication direct with AusweisApp2!");
        msgBox.exec();
        return;
    }
    else if (outputstring == "e7")
    {
        msgBox.setText("ERROR - no cardreader found!");
        msgBox.exec();
        return;
    }

    if (outputstring == "false")
    {
        PIN = diaBox.getText(0, QLatin1String("PIN"), "Bitte PIN eingeben:", QLineEdit::Password, "", &ok);
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setWindowTitle("ERROR");
        if (PIN == "")
        {
            msgBox.setText("ERROR - PIN is empty.");
            msgBox.exec();
            return;
        }
        if (PIN.length() < 6)
        {
            msgBox.setText("ERROR - PIN is too short.");
            msgBox.exec();
            return;
        }
        if (PIN.length() > 6)
        {
            msgBox.setText("ERROR - PIN is too long.");
            msgBox.exec();
            return;
        }
    }
    else
    {
        PIN = "123456";
    }

    msgBox.setWindowModality(Qt::NonModal);
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setWindowTitle("Information");
    msgBox.setText("selfauthentication is running, please wait...\n(this window closes self-acting)");
    QCoreApplication::processEvents();
    msgBox.show();
    QCoreApplication::processEvents();

    //QMessageBox* msgBox1 = new QMessageBox;
    //msgBox1->setText("selfauthentication is running, please wait...\n(this window closes self-acting)");
    //msgBox1->setWindowModality(Qt::NonModal);
    //QCoreApplication::processEvents();
    //msgBox1->show();
    //QCoreApplication::processEvents();


    outputstring= wf.startworkflow(PIN.toUtf8().constData());
    PIN = "000000";
    msgBox.setIcon(QMessageBox::Critical);
    msgBox.setWindowTitle("ERROR");
    if (outputstring == "e1")
    {
        msgBox.setText("ERROR - please check AusweisApp2, cardreader and Personalausweis!");
        msgBox.exec();
        return;
    }
    else if (outputstring == "e2")
    {
        msgBox.setText("ERROR - please check your Personalausweis!");
        msgBox.exec();
        return;
    }
    else if (outputstring == "e3")
    {
        msgBox.setText("ERROR - please check your cardreader!");
        msgBox.exec();
        return;
    }
    else if (outputstring == "e4")
    {
        msgBox.setText("ERROR - AusweisApp2-version less than 1.22.* please update!");
        msgBox.exec();
        return;
    }
    else if (outputstring == "e5")
    {
        msgBox.setText("Warning - retryCounter of Perso <3, please start a selfauthentication direct with AusweisApp2!");
        msgBox.exec();
        return;
    }
    else if (outputstring == "e7")
    {
        msgBox.setText("ERROR - no cardreader found!");
        msgBox.exec();
        return;
    }


    //generate password
    //addstr = "";
    //curitem = ui.ListWidget->currentItem()->text();
    //addstr = curitem.toUtf8().constData() + outputstring;
    //outputstring = w.sha256(addstr);
    //srand(stoui(outputstring));
    //outputstring = generateUpperCase(1) + generateSpecialSign(1) + outputstring;
    wfoutput = QString::fromUtf8(outputstring.c_str());
    //ui.lineEdit->setText(wfoutput);

    //QClipboard* clipboard = QGuiApplication::clipboard();
    //clipboard->setText(wfoutput);

    outputstring = wf.GivenNames;
    outputstring = outputstring.append(", for your information the data from Personalausweis:\n\n\n");

    outputstring = outputstring.append("PersonalData read (for your information):\n\n");
    outputstring = outputstring.append("   AcademicTitle: " + wf.AcademicTitle + "\n");
    outputstring = outputstring.append("   ArtisticName: " + wf.ArtisticName + "\n");
    outputstring = outputstring.append("   BirthName: " + wf.BirthName + "\n");
    outputstring = outputstring.append("   DateOfBirth: " + wf.DateOfBirth + "\n");
    outputstring = outputstring.append("   DocumentType: " + wf.DocumentType + "\n");
    outputstring = outputstring.append("   FamilyNames: " + wf.FamilyNames + "\n");
    outputstring = outputstring.append("   GivenNames: " + wf.GivenNames + "\n");
    outputstring = outputstring.append("   IssuingState: " + wf.IssuingState + "\n");
    outputstring = outputstring.append("   Nationality: " + wf.Nationality + "\n");
    outputstring = outputstring.append("      PlaceOfBirth: " + wf.PlaceOfBirth + "\n");
    outputstring = outputstring.append("   PlaceOfResidence - StructuredPlace:\n");
    outputstring = outputstring.append("      City: " + wf.City + "\n");
    outputstring = outputstring.append("      Country: " + wf.Country + "\n");
    outputstring = outputstring.append("      Street: " + wf.Street + "\n");
    outputstring = outputstring.append("      ZipCode: " + wf.ZipCode + "\n");

    msgBox.setWindowTitle("Information");
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setText(QString::fromUtf8(outputstring.c_str()));
    msgBox.exec();


    std::string su = "000000000000000000";
    wf.personalStyledString = su;
    wf.AcademicTitle = su;
    wf.ArtisticName = su;
    wf.BirthName = su;
    wf.DateOfBirth = su;
    wf.DocumentType = su;
    wf.FamilyNames = su;
    wf.GivenNames = su;
    wf.IssuingState = su;
    wf.Nationality = su;
    wf.PlaceOfBirth = su;
    wf.City = su;
    wf.Country = su;
    wf.Street = su;
    wf.ZipCode = su;


    //set user and his hashkey
    if(ok && !itext.isEmpty() && !wfoutput.isEmpty())
    {
        //set username in ui
        ui->listWidget->addItem(itext);
        ui->listWidget->repaint();
        QStringList newuser;
        newuser[0]=itext;
        newuser[1]=wfoutput;
        datab.append(newuser);
        //qApp->processEvents();
        //write database
        //QFile file("database.txt");
    }
    else
    {
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setWindowTitle("ERROR");
        msgBox.setText("ERROR - Benutzerdaten nicht vollstaendig.");
        msgBox.exec();
        return;
    }
    writedata();
}


void MainWindow::on_deluserButton_clicked()
{
    QList<QListWidgetItem*> items=ui->listWidget->selectedItems();
    foreach(QListWidgetItem * item, items)
    {
        datab.removeAt(ui->listWidget->row(item));
        delete ui->listWidget->takeItem(ui->listWidget->row(item));
    }
    writedata();
}


void MainWindow::on_startButton_clicked()
{
    QMessageBox msgBox;
    QString Info;
    bool startedok;
    std::string outputstring="";
    outputstring = commandexec("ps -ef | grep \"./persodoor\" | grep -v grep");
    if (outputstring != "")
    {
        Info = "Programm persodoor laeuft bereits!\n";
        msgBox.setWindowTitle("ERROR");
        msgBox.setText(Info);
        msgBox.exec();
    }
    else
    {
        //outputstring = commandexec("./persodoor \&");
        startedok=QProcess::startDetached("persodoor");
        if(startedok)
        {
            Info = "Programm persodoor gestartet.\n";
            msgBox.setWindowTitle("Info");
            msgBox.setText(Info);
            msgBox.exec();
        }
    }
}


void MainWindow::on_stopButton_clicked()
{
    QMessageBox msgBox;
    QString Info;
    std::string outputstring="";

    outputstring = commandexec("pkill -e persodoor");
    if (outputstring == "")
    {
        Info = "Programm persodoor nicht gefunden!\n";
        msgBox.setWindowTitle("ERROR");
        msgBox.setText(Info);
        msgBox.exec();
    }
    else
    {
        Info = "Programm persodoor beendet:\n"+QString::fromStdString(outputstring);
        msgBox.setWindowTitle("Info");
        msgBox.setText(Info);
        msgBox.exec();
    }
}


void MainWindow::on_MenuEdit_PIN_triggered()
{
    QMessageBox msgBox;
    QString Info;
    QString PIN="";
    QInputDialog diaBox;
    bool ok;
    QString fileName="PIN.txt";
    QString line="";

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        Info = "Datei PIN.txt kann nicht geöffnet werden!\n";
        msgBox.setWindowTitle("ERROR");
        msgBox.setText(Info);
        msgBox.exec();
        return;
    }
    QTextStream out(&file);

    PIN = diaBox.getText(0, "PIN", "Bitte PIN eingeben:", QLineEdit::Password, "", &ok);

    out << PIN << "\n";

}


void MainWindow::on_MenuHilfe_triggered()
{
    QMessageBox msgBox;
    QString Info;

    Info = "MyPersoDoor\n\nHier koennen Sie mittels der Selbstauskunft des deutschen Personalausweises hashkey zu Benutzern abspeichern.\n\nBitte stellen Sie sicher, dass die AusweisApp2 gestartet ist,\nein Kartenleser angeschlossen und Ihr Personalausweis aufgelegt ist.\n\nDie gelesenen Ausweisdaten werden Ihnen zur Info angezeigt.\n";
    msgBox.setWindowTitle("Info");
    msgBox.setText(Info);
    msgBox.exec();
}



void MainWindow::on_MenuUeber_triggered()
{
    QMessageBox msgBox;
    QString Info;

    Info = "MyPersoDoor\nVersion 0.1\nCopyright 2021 buergerservice.org e.V. <KeePerso@buergerservice.org>\nlicense: GPL-3.0\n";
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setWindowTitle("Info");
    msgBox.setText(Info);
    msgBox.exec();
}



void MainWindow::on_MenuZertifikat_triggered()
{
    QMessageBox msgBox;
    QString Info;
    std::string outputstring;

    outputstring = this->wf.getcertificate();
    msgBox.setIcon(QMessageBox::Critical);
    msgBox.setWindowTitle("ERROR");
    if (outputstring == "e1")
    {
        msgBox.setText("ERROR - please check AusweisApp2, cardreader and Personalausweis!");
        msgBox.exec();
        return;
    }
    else if (outputstring == "e2")
    {
        msgBox.setText("ERROR - please check your Personalausweis!");
        msgBox.exec();
        return;
    }
    else if (outputstring == "e3")
    {
        msgBox.setText("ERROR - please check your cardreader!");
        msgBox.exec();
        return;
    }
    std::string ap = "information about the certificate you are using to identify at provider:\n\n";
    ap.append("description: \n");
    ap.append("   issuerName: " + wf.issuerName + "\n");
    ap.append("   issuerUrl: " + wf.issuerUrl + "\n");
    ap.append("   purpose: " + wf.purpose + "\n");
    ap.append("   subjectName: " + wf.subjectName + "\n");
    ap.append("   subjectUrl: " + wf.subjectUrl + "\n\n");
    ap.append("termsOfUsage:\n" + wf.termsOfUsage + "\n\n");
    ap.append("validity: \n");
    ap.append("   effectiveDate: " + wf.effectiveDate + "\n");
    ap.append("   expirationDate: " + wf.expirationDate + "\n");
    Info = QString::fromUtf8(ap.c_str());

    msgBox.setIcon(QMessageBox::Information);
    msgBox.setWindowTitle("Info");
    msgBox.setText(Info);
    msgBox.exec();
}


