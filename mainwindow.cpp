#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFrame>
#include <QLabel>
#include <QWidget>
#include <QDebug>
#include <QSqlError>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QTextEdit>
#include <QFileDialog>
#include <QString>
#include <QMessageBox>
#include <QTextStream>
#include <QSqlRecord>
#include <QSqlQueryModel>
#include <QListSpecialMethods>
#include <QComboBox>
#include <QStyleFactory>
#include <QVector>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);
    QStringList drivers = QSqlDatabase::drivers();
    qDebug() << "Available drivers: " << drivers;
    import_verif = 0;
    nbre_pro=0;
    nbre_pfl=0;
    nbre_cpt_iso=0;
    verif_retrait=false;
    verif_retour=false;
    retour_pro=false;
    retour_pfl=false;
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_pushButton_2_clicked()
{
    QApplication::quit();
}


void MainWindow::on_pushButton_6_clicked()
{
    QApplication::quit();
}


void MainWindow::on_pushButton_12_clicked()
{
    QApplication::quit();
}

int MainWindow::insert_donnees(QString chemin_fichier)
{
    QSqlDatabase db = QSqlDatabase::database();

    if (db.isOpen())
    {
       qDebug() << "Database is already open.";
    }

    else
    {

       if (!db.open())
       {
           qDebug() << "Error: connection with database failed.";
       }
       else
       {
           qDebug() << "Database is open.";
       }
     }

    QSqlQueryModel quer;
    quer.setQuery("DROP TABLE competences");
    quer.setQuery("DROP TABLE competences");
    quer.setQuery("CREATE TABLE competences(id INTEGER, liste_competences TEXT)");


     if(quer.lastError().isValid())
     {
         qDebug() << "Erreur d'exécution de la requête :" << quer.lastError();
         return 2;
     }

     quer.setQuery("DROP TABLE professionnels");

     quer.setQuery("CREATE TABLE professionnels(id INTEGER, nom TEXT, prenoms TEXT, nbre_cpt_pro INTEGER, liste_competences_pro TEXT)");

     if(quer.lastError().isValid())
     {
         qDebug() << "Erreur d'exécution de la requête :" << quer.lastError();
         return 2;
     }

     quer.setQuery("DROP TABLE profils");

     quer.setQuery("CREATE TABLE profils(id INTEGER, nom TEXT, nbre_cpt_pfl INTEGER, liste_competences_pfl TEXT)");

     if(quer.lastError().isValid())
     {
         qDebug() << "Erreur d'exécution de la requête :" << quer.lastError();
         return 2;
     }

    QFile fichier(chemin_fichier);

    if(!fichier.open(QIODevice::ReadOnly | QFile::Text))
    {

      QMessageBox::warning(this, "Attention", "Le fichier que vous essayez d'ouvrir ne peut pas être ouvert : " + fichier.errorString());
      ui->stackedWidget->setCurrentIndex(1);
      return 0;
    }

    QTextStream in(&fichier);
    QString lecture;
    QStringList inter;
    QString resultat_pro = "";
    QString resultat_pfl = "";
    QString cpt;
    int t=1, r=1, k;
    int id_cpt=0;

    while(!in.atEnd())
    {
        lecture = in.readLine();

        if (lecture=="PRO:")
        {
           nbre_pro +=1;
           int a=0, nbre_Cpt;
           QString nom;
           QString prenom;

           quint64 position = in.pos();
           fichier.seek(position);

           cpt = in.readLine();
           nom = cpt.mid(1);

           quint64 position2 = in.pos();
           fichier.seek(position2);

           cpt = in.readLine();
           prenom = cpt.mid(1);

           quint64 position3 = in.pos();
           fichier.seek(position3);

           cpt = in.readLine();
           cpt = cpt.mid(1);
           nbre_Cpt = cpt.toInt();


           quint64 position4 = in.pos();
           fichier.seek(position4);

           resultat_pro="";

           while(a<nbre_Cpt)
           {
              cpt = in.readLine();
              cpt = cpt.mid(1);
              resultat_pro +=  cpt + "\n";
              resultat_liste_cpt += cpt ;
              a+=1;
           }

           QSqlQuery cpt_pro_insert;

           cpt_pro_insert.prepare("INSERT INTO professionnels(id, nom, prenoms, nbre_cpt_pro, liste_competences_pro)" "VALUES(:id, :nom, :prenoms, :nbre_cpt_pro, :liste_competences_pro)");
           cpt_pro_insert.bindValue(":id", t);
           cpt_pro_insert.bindValue(":nom", nom);
           cpt_pro_insert.bindValue(":prenoms", prenom);
           cpt_pro_insert.bindValue(":nbre_cpt_pro", nbre_Cpt);
           cpt_pro_insert.bindValue(":liste_competences_pro", resultat_pro);
           cpt_pro_insert.exec();

           if (!cpt_pro_insert.exec())
           {
               qDebug()<<"Erreur d'exécution de la requête : "<< cpt_pro_insert.lastError();
               return 2;
           }
           t+=1;

        }

        if (lecture=="PFL:")
        {
            nbre_pfl+=1;
            int a=0,  nbre_Cpt_pfl;
            QString nom_pfl;

            quint64 position = in.pos();
            fichier.seek(position);

            cpt = in.readLine();
            nom_pfl = cpt.mid(1);

            quint64 position3 = in.pos();
            fichier.seek(position3);

            cpt = in.readLine();
            cpt = cpt.mid(1);
            nbre_Cpt_pfl = cpt.toInt();


            quint64 position4 = in.pos();
            fichier.seek(position4);

            resultat_pfl="";
            while(a<nbre_Cpt_pfl)
            {
               cpt = in.readLine();
               cpt = cpt.mid(1);
               resultat_pfl += cpt + "\n";
               resultat_liste_cpt +=  cpt  ;
               a+=1;
            }

            QSqlQuery cpt_pfl_insert;

            cpt_pfl_insert.prepare("INSERT INTO profils(id, nom, nbre_cpt_pfl, liste_competences_pfl)" "VALUES(:id, :nom, :nbre_cpt_pfl, :liste_competences_pfl)");
            cpt_pfl_insert.bindValue(":id", r);
            cpt_pfl_insert.bindValue(":nom", nom_pfl);
            cpt_pfl_insert.bindValue(":nbre_cpt_pfl", nbre_Cpt_pfl);
            cpt_pfl_insert.bindValue(":liste_competences_pfl", resultat_pfl);
            cpt_pfl_insert.exec();

            if (!cpt_pfl_insert.exec())
            {
                qDebug()<<"Erreur d'exécution de la requête : "<< cpt_pfl_insert.lastError();
                return 2;
            }
            r+=1;
        }

        if (lecture=="CPT:")
        {
            k=0;
            nbre_cpt_iso+=1;
            int a=0, nbre_Cpt;
            quint64 position = in.pos();
            fichier.seek(position);

            cpt = in.readLine().mid(1);
            nbre_Cpt = cpt.toInt();

            quint64 position2 = in.pos();
            fichier.seek(position2);

            while(a<nbre_Cpt)
            {
               cpt = in.readLine();
               cpt = cpt.mid(1);
               inter += cpt ;
               resultat_liste_cpt += cpt ;
               a+=1;
            }

            while(k<nbre_Cpt)
            {
                id_cpt+=1;
                QSqlQuery cpt_insert;

                cpt_insert.prepare("INSERT INTO competences(id, liste_competences)" "VALUES(:id, :liste_competences)");
                cpt_insert.bindValue(":id", id_cpt);
                cpt_insert.bindValue(":liste_competences", inter.at(k));
                cpt_insert.exec();

                if (!cpt_insert.exec())
                {
                    qDebug()<<"Erreur d'exécution de la requête : "<< cpt_insert.lastError();
                    return 2;
                }
                k+=1;

            }
            inter.clear();

         }
    }

    return 1;
}

void MainWindow::on_pushButton_4_clicked()
{  
  import_verif = 1;

  QString chemin_fichier = QFileDialog::getOpenFileName(this, "Importer le fichier", "", "CPT files(*.cpt)");

  if (MainWindow::insert_donnees(chemin_fichier)==1)  ui->stackedWidget->setCurrentIndex(8);

  db.close();
}


void MainWindow::on_actionMenu_principal_triggered()
{
    verif_retour=true;

    if(import_verif != 0) ui->stackedWidget->setCurrentIndex(2);

    else ui->stackedWidget->setCurrentIndex(1);
}


void MainWindow::on_pushButton_13_clicked()
{
    QApplication::quit();
}


void MainWindow::on_pushButton_10_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}


void MainWindow::on_actionExporter_la_base_de_donn_es_triggered()
{
    verif_retour=true;

    if (import_verif!=0)
    {
       ui->stackedWidget->setCurrentIndex(4);
    }

    else ui->stackedWidget->setCurrentIndex(1);
}


void MainWindow::on_pushButton_15_clicked()
{
    QApplication::quit();
}


void MainWindow::on_actionR_initialiser_la_base_de_donn_es_triggered()
{
  import_verif=0;

  if (import_verif==1)
  {
    QSqlDatabase db = QSqlDatabase::database();

    if (db.isOpen())
    {
       qDebug() << "Database is already open.";
    }

    else
    {

       if (!db.open())
       {
           qDebug() << "Error: connection with database failed.";
       }
       else
       {
           qDebug() << "Database is open.";
       }
    }

    QSqlQueryModel supression;

    supression.setQuery(" DROP TABLE professionnels ");

    if(supression.lastError().isValid())
    {
        qDebug() << "Erreur d'exécution de la requête :" << supression.lastError();
        return ;
    }

    supression.setQuery(" DROP TABLE profils ");

    if(supression.lastError().isValid())
    {
        qDebug() << "Erreur d'exécution de la requête :" << supression.lastError();
        return ;
    }

    supression.setQuery(" DROP TABLE competences");

    if(supression.lastError().isValid())
    {
        qDebug() << "Erreur d'exécution de la requête :" << supression.lastError();
        return ;
    }

    db.close();
    }

  else  ui->stackedWidget->setCurrentIndex(6);
}


void MainWindow::on_pushButton_20_clicked()
{
    QApplication::quit();
}


void MainWindow::on_actionRechercher_triggered()
{
    verif_retour=true;

    if (import_verif!=0)
    {
        ui->label_54->clear();
        ui->textEdit_3->clear();
        ui->textEdit_3->clearFocus();
        ui->stackedWidget->setCurrentIndex(5);
    }

    else ui->stackedWidget->setCurrentIndex(1);
}


void MainWindow::on_pushButton_7_clicked()
{


    QString liste_cpt;

    resultat_liste_cpt.sort();
    resultat_liste_cpt.removeDuplicates();

    int t=1;
    for(int i=0;i<resultat_liste_cpt.length();i++)
    {
        liste_cpt += QString::number(t) + "- " + resultat_liste_cpt.at(i) + "\n";
        t+=1;
    }

    ui->textBrowser_2->setText(liste_cpt);
    ui->stackedWidget->setCurrentIndex(7);

}


void MainWindow::on_pushButton_22_clicked()
{
    QApplication::quit();
}


void MainWindow::on_pushButton_24_clicked()
{
    QApplication::quit();
}


void MainWindow::on_pushButton_23_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}


void MainWindow::on_pushButton_8_clicked()
{
    QSqlDatabase db = QSqlDatabase::database();

    if (db.isOpen())
    {
       qDebug() << "Database is already open.";
    }

    else
    {

       if (!db.open())
       {
           qDebug() << "Error: connection with database failed.";
       }
       else
       {
           qDebug() << "Database is open.";
       }
     }

    QSqlQuery liste_pro;
    QString sortie_pro;

    liste_pro.exec("SELECT DISTINCT id, nom, prenoms FROM professionnels");

    if (!liste_pro.exec())
    {
        qDebug()<<"Erreur d'exécution de la requête : "<< liste_pro.lastError();
        return ;
    }


    while(liste_pro.next())
    {
        sortie_pro += liste_pro.value(0).toString() + "- " + liste_pro.value(1).toString() + "  " + liste_pro.value(2).toString() + "\n" ;
    }

    ui->textBrowser_2->setText(sortie_pro);
    ui->stackedWidget->setCurrentIndex(7);

    liste_pro.clear();
    db.close();
}


void MainWindow::on_pushButton_9_clicked()
{
    QSqlDatabase db = QSqlDatabase::database();

    if (db.isOpen())
    {
       qDebug() << "Database is already open.";
    }

    else
    {

       if (!db.open())
       {
           qDebug() << "Error: connection with database failed.";
       }
       else
       {
           qDebug() << "Database is open.";
       }
     }

    QSqlQuery liste_pfl;
    QString sortie_pfl;

    liste_pfl.exec("SELECT DISTINCT id, nom FROM profils");

    if (!liste_pfl.exec())
    {
        qDebug()<<"Erreur d'exécution de la requête : "<< liste_pfl.lastError();
        return ;
    }

    while(liste_pfl.next())
    {
        sortie_pfl += liste_pfl.value(0).toString() + "- " + liste_pfl.value(1).toString() + "\n" ;

    }

    ui->textBrowser_2->setText(sortie_pfl);
    ui->stackedWidget->setCurrentIndex(7);

    liste_pfl.clear();
    db.close();
}


void MainWindow::on_pushButton_21_clicked()
{
    import_verif = 1;

    QString chemin_fichier = QFileDialog::getOpenFileName(this, "Importer le fichier", "", "CPT files(*.cpt)");

    if (MainWindow::insert_donnees(chemin_fichier)==1)   ui->stackedWidget->setCurrentIndex(8);

    else if (MainWindow::insert_donnees(chemin_fichier)==0)      ui->stackedWidget->setCurrentIndex(6);

    db.close();
}


void MainWindow::on_pushButton_5_clicked()
{
    QSqlDatabase db = QSqlDatabase::database();

    if (db.isOpen())
    {
       qDebug() << "Database is already open.";
    }

    else
    {

       if (!db.open())
       {
           qDebug() << "Error: connection with database failed.";
       }
       else
       {
           qDebug() << "Database is open.";
       }
     }

    QSqlQuery liste_pfl;
    QString sortie_pfl;

    liste_pfl.exec("SELECT DISTINCT  id, nom FROM profils");

    if (!liste_pfl.exec())
    {
        qDebug()<<"Erreur d'exécution de la requête : "<< liste_pfl.lastError();
        return ;
    }


    while(liste_pfl.next())
    {
        sortie_pfl += liste_pfl.value(0).toString() + "- " + liste_pfl.value(1).toString() + "\n" ;

    }

    ui->textBrowser_4->setText(sortie_pfl);
    ui->stackedWidget->setCurrentIndex(9);

    intervalle_spin_pfl=0;
    for(int i=0;i<sortie_pfl.length();i++)
    {
        if(sortie_pfl.at(i)=="\n")
        {
            intervalle_spin_pfl += 1;
        }
    }

    ui->comboBox_4->clear();
    int k=0;
    while(k<intervalle_spin_pfl)
    {
        ui->comboBox_4->addItem(QString::number(k+1), QVariant(k+1));
        k+=1;
    }

    ui->comboBox_4->setEditable(false);

    liste_pfl.clear();
    db.close();
}


void MainWindow::on_pushButton_26_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}


void MainWindow::on_pushButton_27_clicked()
{
    QApplication::quit();
}


void MainWindow::on_pushButton_29_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}


void MainWindow::on_pushButton_30_clicked()
{
     QApplication::quit();
}


void MainWindow::on_pushButton_3_clicked()
{
    ui->comboBox_2->clear();
    QSqlDatabase db = QSqlDatabase::database();

    if (db.isOpen())
    {
       qDebug() << "Database is already open.";
    }

    else
    {

       if (!db.open())
       {
           qDebug() << "Error: connection with database failed.";
       }
       else
       {
           qDebug() << "Database is open.";
       }
     }

    QSqlQuery liste_pro;
    QString sortie_pro;

    liste_pro.exec("SELECT DISTINCT id, nom, prenoms FROM professionnels");

    if (!liste_pro.exec())
    {
        qDebug()<<"Erreur d'exécution de la requête : "<< liste_pro.lastError();
        return ;
    }

    while(liste_pro.next())
    {
        sortie_pro += liste_pro.value(0).toString() + "- " + liste_pro.value(1).toString() + "  " + liste_pro.value(2).toString() + "\n" ;
    }


    ui->textBrowser->setText(sortie_pro);
    ui->stackedWidget->setCurrentIndex(10);

    intervalle_spin_pro=0;
    for(int i=0;i<sortie_pro.length();i++)
    {
        if(sortie_pro.at(i)=="\n")
        {
            intervalle_spin_pro += 1;
        }
    }

    int k=0;
    while(k<intervalle_spin_pro)
    {
        ui->comboBox_2->addItem(QString::number(k+1), QVariant(k+1));
        k+=1;
    }

    ui->comboBox_2->setEditable(false);

    liste_pro.clear();
    db.close();
}

void MainWindow::on_pushButton_34_clicked()
{
    retour_pfl=false;
    ui->stackedWidget->setCurrentIndex(9);
}


void MainWindow::on_pushButton_35_clicked()
{
    QApplication::quit();
}


void MainWindow::on_pushButton_39_clicked()
{
    retour_pro=false;
    ui->stackedWidget->setCurrentIndex(10);
}


void MainWindow::on_pushButton_40_clicked()
{
    QApplication::quit();
}


void MainWindow::on_pushButton_44_clicked()
{
    verif_retour=true;
    ui->stackedWidget->setCurrentIndex(3);
}


void MainWindow::on_pushButton_43_clicked()
{
    QApplication::quit();
}


void MainWindow::on_pushButton_11_clicked()
{
  if(verif_retour==false)
  {
    QSqlDatabase db = QSqlDatabase::database();

    if (db.isOpen())
    {
       qDebug() << "Database is already open.";
    }

    else
    {

       if (!db.open())
       {
           qDebug() << "Error: connection with database failed.";
       }
       else
       {
           qDebug() << "Database is open.";
       }
     }

    QString sortie_cpt;
    QSqlQuery liste_cpt;

    liste_cpt.exec("SELECT DISTINCT id, liste_competences FROM competences");

    if (!liste_cpt.exec())
    {
        qDebug()<<"Erreur d'exécution de la requête : "<< liste_cpt.lastError();
        return ;
    }

    while(liste_cpt.next())
    {
        sortie_cpt += liste_cpt.value(0).toString() + "- " + liste_cpt.value(1).toString() + "\n";
    }

    liste_cpt_cpt = sortie_cpt;
    ui->textBrowser_5->setText(sortie_cpt);
    ui->stackedWidget->setCurrentIndex(13);

    liste_cpt.clear();
    db.close();

    }

    else
    {
      test_cpt = ui->textBrowser_5->toPlainText();
      ui->textBrowser_5->setText(test_cpt);
      ui->stackedWidget->setCurrentIndex(13);
    }

}


void MainWindow::on_pushButton_28_clicked()
{
    num_profess = ui->comboBox_2->currentData();
    QString sortie_cher_pro;
    QSqlDatabase db = QSqlDatabase::database();

    if (db.isOpen())
    {
       qDebug() << "Database is already open.";
    }

    else
    {

       if (!db.open())
       {
           qDebug() << "Error: connection with database failed.";
       }
       else
       {
           qDebug() << "Database is open.";
       }
     }

    QSqlQuery chercher_pro;

    chercher_pro.prepare("SELECT nom FROM professionnels WHERE id = :id");
    chercher_pro.bindValue(":id", num_profess.toInt());
    chercher_pro.exec();

    if (!chercher_pro.exec())
    {
        qDebug()<<"Erreur d'exécution de la requête : "<< chercher_pro.lastError();
        return ;
    }

    while(chercher_pro.next())
    {
        sortie_cher_pro = chercher_pro.value(0).toString() ;
        pros_nom =  chercher_pro.value(0).toString() + "  ";
    }

    ui->textBrowser_3->setText(sortie_cher_pro);

    chercher_pro.prepare("SELECT prenoms FROM professionnels WHERE id = :id");
    chercher_pro.bindValue(":id", num_profess.toInt());
    chercher_pro.exec();

    if (!chercher_pro.exec())
    {
        qDebug()<<"Erreur d'exécution de la requête : "<< chercher_pro.lastError();
        return ;
    }

    while(chercher_pro.next())
    {
        sortie_cher_pro = chercher_pro.value(0).toString() ;
        pros_prenom = chercher_pro.value(0).toString();
    }

    ui->textBrowser_6->setText(sortie_cher_pro);

    chercher_pro.prepare("SELECT liste_competences_pro FROM professionnels WHERE id = :id");
    chercher_pro.bindValue(":id", num_profess.toInt());
    chercher_pro.exec();

    if (!chercher_pro.exec())
    {
        qDebug()<<"Erreur d'exécution de la requête : "<< chercher_pro.lastError();
        return ;
    }

    while(chercher_pro.next())
    {
        sortie_cher_pro = chercher_pro.value(0).toString() ;
    }

    QString inspi;
    QStringList inspi_co;
    int l=1;
    for(int i=0;i<sortie_cher_pro.length();i++)
    {
        if(sortie_cher_pro.at(i)!="\n")
        {
            inspi += sortie_cher_pro.at(i);
        }
        else
        {
            inspi_co +=  inspi ;
            inspi.clear();
        }
    }

    inspi.clear();

    for(int i=0;i<inspi_co.length();i++)
    {
        inspi += QString::number(l) + "- " + inspi_co.at(i) + "\n";
        l+=1;
    }

    num_cpt_pro = sortie_cher_pro;
    ui->textBrowser_7->setText(inspi);
    ui->stackedWidget->setCurrentIndex(12);
}


void MainWindow::on_pushButton_25_clicked()
{
    num_profil = ui->comboBox_4->currentData();
    QString sortie_cher_pfl;
    QSqlDatabase db = QSqlDatabase::database();

    if (db.isOpen())
    {
       qDebug() << "Database is already open.";
    }

    else
    {

       if (!db.open())
       {
           qDebug() << "Error: connection with database failed.";
       }
       else
       {
           qDebug() << "Database is open.";
       }

    }

    QSqlQuery chercher_pfl;

    chercher_pfl.prepare("SELECT nom FROM profils WHERE id = :id");
    chercher_pfl.bindValue(":id", num_profil.toInt());
    chercher_pfl.exec();

    if (!chercher_pfl.exec())
    {
        qDebug()<<"Erreur d'exécution de la requête : "<< chercher_pfl.lastError();
        return ;
    }

    while(chercher_pfl.next())
    {
        sortie_cher_pfl = chercher_pfl.value(0).toString() ;
        pfls_nom = chercher_pfl.value(0).toString() ;
    }

    ui->textBrowser_8->setText(sortie_cher_pfl);

    chercher_pfl.prepare("SELECT liste_competences_pfl FROM profils WHERE id = :id");
    chercher_pfl.bindValue(":id", num_profil.toInt());
    chercher_pfl.exec();

    if (!chercher_pfl.exec())
    {
        qDebug()<<"Erreur d'exécution de la requête : "<< chercher_pfl.lastError();
        return ;
    }

    while(chercher_pfl.next())
    {
        sortie_cher_pfl = chercher_pfl.value(0).toString() ;
    }

    QString inspi;
    QStringList inspi_co;
    int l=1;
    for(int i=0;i<sortie_cher_pfl.length();i++)
    {
        if(sortie_cher_pfl.at(i)!="\n")
        {
            inspi += sortie_cher_pfl.at(i);
        }
        else
        {
            inspi_co +=  inspi ;
            inspi.clear();
        }
    }

    inspi.clear();

    for(int i=0;i<inspi_co.length();i++)
    {
        inspi += QString::number(l) + "- " + inspi_co.at(i) + "\n";
        l+=1;
    }


    num_cpt_pfl = sortie_cher_pfl;

    ui->textBrowser_9->setText(inspi);

    ui->stackedWidget->setCurrentIndex(11);
}


void MainWindow::on_pushButton_45_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}


void MainWindow::on_pushButton_46_clicked()
{
    QApplication::quit();
}


void MainWindow::on_pushButton_37_clicked()
{
  ui->comboBox_3->clear();
  QString memoire="";
  if(retour_pro==false)
  {
    int l=1;
    numero_ajout_pro.clear();
    for (int i=0; i<num_cpt_pro.length();i++)
    {
        if(num_cpt_pro.at(i)!="\n")
        {
            memoire += num_cpt_pro.at(i);
        }
        else
        {
            numero_ajout_pro += QString::number(l) + "- " + memoire ;
            memoire="";
            l+=1;
        }
    }
    memoire="";

    for (int i=0; i<numero_ajout_pro.length();i++)
    {
        memoire += numero_ajout_pro.at(i) + "\n";
    }
    ui->textBrowser_11->setText(pros_nom + pros_prenom);
    ui->textBrowser_10->setText(memoire);
    ui->stackedWidget->setCurrentIndex(15);
    ui->label_34->clear();

    intervalle_spin_cpt_pro = 0;
    for(int i=0; i<memoire.length();i++)
    {
        if(memoire.at(i)=="\n")
        {
            intervalle_spin_cpt_pro += 1;
        }
    }

    int k=0;
    while(k<intervalle_spin_cpt_pro)
    {
       ui->comboBox_3->addItem(QString::number(k+1), QVariant(k+1));
       k+=1;
    }

    ui->comboBox_3->setEditable(false);
  }


  else
  {
      ui->textBrowser_10->setText(ui->textBrowser_7->toPlainText());
      ui->textBrowser_11->setText(pros_nom + pros_prenom);
      ui->stackedWidget->setCurrentIndex(15);
      ui->label_34->clear();

      intervalle_spin_cpt_pro = 0;
      for(int i=0; i<ui->textBrowser_10->toPlainText().length();i++)
      {
          if(ui->textBrowser_10->toPlainText().at(i)=="\n")
          {
              intervalle_spin_cpt_pro += 1;
          }
      }

      ui->comboBox_3->clear();

      int k=0;
      while(k<intervalle_spin_cpt_pro)
      {
         ui->comboBox_3->addItem(QString::number(k+1), QVariant(k+1));
         k+=1;
      }

      ui->comboBox_3->setEditable(false);

  }

}


void MainWindow::on_pushButton_36_clicked()
{
    retour_pro=true;
    ui->textBrowser_18->setText(pros_nom + pros_prenom);
    ui->textBrowser_17->setText(ui->textBrowser_7->toPlainText());
    ui->textEdit_5->clear();
    ui->textEdit_5->clearFocus();
    ui->label_32->clear();
    ui->stackedWidget->setCurrentIndex(14);
}


void MainWindow::on_pushButton_50_clicked()
{
    retour_pro=true;
    ui->stackedWidget->setCurrentIndex(12);
}


void MainWindow::on_pushButton_51_clicked()
{
    QApplication::quit();
}


void MainWindow::on_pushButton_49_clicked()
{
    ui->stackedWidget->setCurrentIndex(12);
}


void MainWindow::on_pushButton_48_clicked()
{
    QApplication::quit();
}


void MainWindow::on_pushButton_53_clicked()
{
    retour_pfl=true;
    ui->stackedWidget->setCurrentIndex(11);
}


void MainWindow::on_pushButton_54_clicked()
{
    QApplication::quit();
}


void MainWindow::on_pushButton_32_clicked()
{
  ui->comboBox_5->clear();
  QString memoire="";
  if(retour_pfl==false)
  {
    int l=1;
    numero_ajout_pfl.clear();
    for (int i=0; i<num_cpt_pfl.length();i++)
    {
        if(num_cpt_pfl.at(i)!="\n")
        {
            memoire += num_cpt_pfl.at(i);
        }
        else
        {
            numero_ajout_pfl += QString::number(l) + "- " + memoire ;
            memoire="";
            l+=1;
        }
    }
    memoire="";

    for (int i=0; i<numero_ajout_pfl.length();i++)
    {
        memoire += numero_ajout_pfl.at(i) + "\n";
    }
    ui->textBrowser_13->setText(pfls_nom);
    ui->textBrowser_12->setText(memoire);
    ui->stackedWidget->setCurrentIndex(16);
    ui->label_36->clear();

    intervalle_spin_cpt_pfl = 0;
    for(int i=0; i<memoire.length();i++)
    {
        if(memoire.at(i)=="\n")
        {
            intervalle_spin_cpt_pfl += 1;
        }
    }

    int k=0;
    while(k<intervalle_spin_cpt_pfl)
    {
       ui->comboBox_5->addItem(QString::number(k+1), QVariant(k+1));
       k+=1;
    }

    ui->comboBox_5->setEditable(false);
  }

  else
  {
      ui->textBrowser_12->setText(ui->textBrowser_9->toPlainText());
      ui->stackedWidget->setCurrentIndex(16);
      ui->label_36->clear();

      intervalle_spin_cpt_pfl = 0;
      for(int i=0; i<ui->textBrowser_12->toPlainText().length();i++)
      {
          if(ui->textBrowser_12->toPlainText().at(i)=="\n")
          {
              intervalle_spin_cpt_pfl += 1;
          }
      }

      ui->comboBox_5->clear();

      int k=0;
      while(k<intervalle_spin_cpt_pfl)
      {
         ui->comboBox_5->addItem(QString::number(k+1), QVariant(k+1));
         k+=1;
      }

      ui->comboBox_5->setEditable(false);


   }
}

void MainWindow::on_pushButton_57_clicked()
{
    ui->stackedWidget->setCurrentIndex(11);
}

void MainWindow::on_pushButton_58_clicked()
{
    QApplication::quit();
}

void MainWindow::on_pushButton_31_clicked()
{
    retour_pfl=true;
    ui->textBrowser_20->setText(ui->textBrowser_8->toPlainText());
    ui->textBrowser_19->setText(ui->textBrowser_9->toPlainText());
    ui->textEdit_8->clear();
    ui->textEdit_8->clearFocus();
    ui->label_39->clear();
    ui->stackedWidget->setCurrentIndex(17);
}


void MainWindow::on_pushButton_59_clicked()
{
   ui->stackedWidget->setCurrentIndex(13);
}


void MainWindow::on_pushButton_60_clicked()
{
  QApplication::quit();
}


void MainWindow::on_pushButton_63_clicked()
{
    verif_retrait=true;
    ui->stackedWidget->setCurrentIndex(13);
}


void MainWindow::on_pushButton_64_clicked()
{
   QApplication::quit();
}


void MainWindow::on_pushButton_42_clicked()
{
    intervalle_spin=0;

    for(int i=0; i<ui->textBrowser_5->toPlainText().length();i++)
    {
        if(ui->textBrowser_5->toPlainText().at(i)=="\n")
        {
            intervalle_spin += 1;
        }
    }

    int a=0;
    while(a<intervalle_spin)
    {
        ui->comboBox->addItem(QString::number(a+1), QVariant(a+1));
        a+=1;
    }

    ui->comboBox->setEditable(false);

    if(verif_retrait==false) ui->textBrowser_14->setText(ui->textBrowser_5->toPlainText());

    else
    {
        test_cpt = ui->textBrowser_5->toPlainText();
        ui->textBrowser_14->setText(test_cpt);
        verif_retrait=false;
    }

    ui->label_43->clear();
    ui->stackedWidget->setCurrentIndex(19);
}


void MainWindow::on_pushButton_41_clicked()
{
    ui->textEdit_9->clear();
    ui->textEdit_9->clearFocus();
    ui->label_48->clear();
    ui->textBrowser_16->setText(ui->textBrowser_5->toPlainText());
    ui->stackedWidget->setCurrentIndex(18);
}


void MainWindow::on_pushButton_52_clicked()
{
    nouv_list_cpt.clear();
    num_spin_cpt_pro = ui->comboBox_3->currentData();
    QSqlDatabase db = QSqlDatabase::database();


    if (db.isOpen())
    {
       qDebug() << "Database is already open.";
    }

    else
    {

       if (!db.open())
       {
           qDebug() << "Error: connection with database failed.";
       }
       else
       {
           qDebug() << "Database is open.";
       }
     }

    QSqlQuery selection_cpt_pro;
    QString sortie_selection_pro;

    selection_cpt_pro.prepare("SELECT liste_competences_pro FROM professionnels WHERE id = :id");
    selection_cpt_pro.bindValue(":id", num_profess.toInt());
    selection_cpt_pro.exec();

    if (!selection_cpt_pro.exec())
    {
        qDebug()<<"Erreur d'exécution de la requête : "<< selection_cpt_pro.lastError();
        return ;
    }

    while(selection_cpt_pro.next())
    {
        sortie_selection_pro = selection_cpt_pro.value(0).toString();
    }


    QStringList pb_a_retirer;
    QString test = sortie_selection_pro;
    QString test2;

    for(int i=0; i < test.length(); i++)
    {
        if(test.at(i)!="\n")
        {
            test2 += test.at(i);
        }

        else
        {
            pb_a_retirer.append(test2);
            test2.clear();
        }
    }

    QString cpt_pro_retrait;
    cpt_pro_retrait = pb_a_retirer.at(num_spin_cpt_pro.toInt()-1);

    pb_a_retirer.removeAll(QString::fromStdString(cpt_pro_retrait.toStdString()));

    sortie_selection_pro.clear();

    for(int i=0; i<pb_a_retirer.length();i++)
    {
        sortie_selection_pro +=  pb_a_retirer.at(i) + "\n";
    }

    QSqlQuery retrait_cpt_pro;

    retrait_cpt_pro.prepare("UPDATE professionnels SET liste_competences_pro = ? WHERE id = ? ;");
    retrait_cpt_pro.addBindValue(sortie_selection_pro);
    retrait_cpt_pro.addBindValue(num_profess.toInt());
    retrait_cpt_pro.exec();

    if (!retrait_cpt_pro.exec())
    {
        qDebug()<<"Erreur d'exécution de la requête 1: "<< retrait_cpt_pro.lastError();
        return ;
    }

    QString nouv_sortie_cpt_pro;

    int k = 1;
    for(int i=0;i<pb_a_retirer.length();i++)
    {
        nouv_sortie_cpt_pro += QString::number(k) + "- " + pb_a_retirer.at(i) + "\n";
        k+=1;
    }


    ui->textBrowser_10->setText(nouv_sortie_cpt_pro);
    ui->textBrowser_7->setText(nouv_sortie_cpt_pro);
    ui->label_34->setText("         La compétence a bien été retirée !");

    resultat_liste_cpt.removeAll(QString::fromStdString(cpt_pro_retrait.toStdString()));

    resultat_liste_cpt.sort();
    resultat_liste_cpt.removeDuplicates();


    int l=1;
    for(int i=0; i<resultat_liste_cpt.length();i++)
    {
        nouv_list_cpt += QString::number(l) +"- " + resultat_liste_cpt.at(i) + "\n";
        l+=1;
    }

    ui->textBrowser_2->setText(nouv_list_cpt);

    intervalle_spin_cpt_pro=0;
    for(int i=0;i<ui->textBrowser_10->toPlainText().length();i++)
    {
          if(ui->textBrowser_10->toPlainText().at(i)=="\n")
          {
              intervalle_spin_cpt_pro += 1;
          }
    }

    ui->comboBox_3->clear();

    int j=0;
    while(j<intervalle_spin_cpt_pro)
    {
        ui->comboBox_3->addItem(QString::number(j+1), QVariant(j+1));
        j+=1;
    }
    ui->comboBox_3->setEditable(false);

    db.close();
}


void MainWindow::on_pushButton_62_clicked()
{

    nouv_list_cpt.clear();
    num_cpt_cpt = ui->comboBox->currentData();
    QStringList recup_sortie_retirer;
    QSqlDatabase db = QSqlDatabase::database();


    if (db.isOpen())
    {
       qDebug() << "Database is already open.";
    }

    else
    {

       if (!db.open())
       {
           qDebug() << "Error: connection with database failed.";
       }
       else
       {
           qDebug() << "Database is open.";
       }
     }


    QSqlQuery retirer_cpt;

    retirer_cpt.prepare("SELECT liste_competences FROM competences WHERE id = :id");
    retirer_cpt.bindValue(":id", num_cpt_cpt.toInt());
    retirer_cpt.exec();

    if (!retirer_cpt.exec())
    {
        qDebug()<<"Erreur d'exécution de la requête : "<< retirer_cpt.lastError();
        return ;
    }

    while(retirer_cpt.next())
    {
        cpt_a_retirer = retirer_cpt.value(0).toString();
    }

    QSqlQuery retrait_cpt;

    retrait_cpt.prepare("SELECT liste_competences FROM competences WHERE id = :id");
    retrait_cpt.bindValue(":id", num_cpt_cpt.toInt());
    retrait_cpt.exec();

    if (!retrait_cpt.exec())
    {
        qDebug()<<"Erreur d'exécution de la requête : "<< retrait_cpt.lastError();
        return ;
    }

    while(retrait_cpt.next())
    {
        cpt_retrait = retrait_cpt.value(0).toString();
    }

    retirer_cpt.prepare("DELETE FROM competences WHERE id = :id ");
    retirer_cpt.bindValue(":id", num_cpt_cpt.toInt());
    retirer_cpt.exec();

    if (!retirer_cpt.exec())
    {
        qDebug()<<"Erreur d'exécution de la requête : "<< retirer_cpt.lastError();
        return ;
    }

    retirer_cpt.exec("SELECT DISTINCT liste_competences FROM competences ");

    if (!retirer_cpt.exec())
    {
        qDebug()<<"Erreur d'exécution de la requête : "<< retirer_cpt.lastError();
        return ;
    }

    while(retirer_cpt.next())
    {
        recup_sortie_retirer += retirer_cpt.value(0).toString();
    }

    retirer_cpt.exec("DELETE FROM competences");

    if (!retirer_cpt.exec())
    {
        qDebug()<<"Erreur d'exécution de la requête : "<< retirer_cpt.lastError();
        return ;
    }

    int a=0;
    while(a<recup_sortie_retirer.length())
    {
       retirer_cpt.prepare("INSERT INTO competences(id, liste_competences)" "VALUES(:id, :liste_competences)");
       retirer_cpt.bindValue(":id",a+1);
       retirer_cpt.bindValue(":liste_competences", recup_sortie_retirer.at(a));

       if (!retirer_cpt.exec())
       {
           qDebug()<<"Erreur d'exécution de la requête : "<< retirer_cpt.lastError();
           return ;
       }

       a+=1;
    }


    retirer_cpt.exec("SELECT id, liste_competences FROM competences");

    sortie_nouv_liste_cpt="";
    if (!retirer_cpt.exec())
    {
        qDebug()<<"Erreur d'exécution de la requête : "<< retirer_cpt.lastError();
        return ;
    }

    sortie_nouv_liste_cpt="";
    while(retirer_cpt.next())
    {
         sortie_nouv_liste_cpt += retirer_cpt.value(0).toString() + "- " + retirer_cpt.value(1).toString() + "\n";
    }

      resultat_liste_cpt.removeAll(QString::fromStdString(cpt_retrait.toStdString()));


      resultat_liste_cpt.sort();
      resultat_liste_cpt.removeDuplicates();


      int l=1;
      for(int i=0; i<resultat_liste_cpt.length();i++)
      {
          nouv_list_cpt += QString::number(l) +"- " + resultat_liste_cpt.at(i) + "\n";
          l+=1;
      }

      ui->textBrowser_2->setText(nouv_list_cpt);
      cpt_retrait.clear();
      verif_occas=false;



    ui->comboBox->setEditable(false);

    ui->textBrowser_14->setText(sortie_nouv_liste_cpt);
    ui->textBrowser_5->setText(sortie_nouv_liste_cpt);
    ui->label_43->clear();
    ui->label_43->setText("                 La compétence a été retirée avec succès !");
    sortie_nouv_liste_cpt.clear();

    intervalle_spin=0;

      for(int i=0;i<ui->textBrowser_14->toPlainText().length();i++)
      {
            if(ui->textBrowser_14->toPlainText().at(i)=="\n")
            {
                intervalle_spin += 1;
            }
      }

      ui->comboBox->clear();

      int k=0;
      while(k<intervalle_spin)
      {
          ui->comboBox->addItem(QString::number(k+1), QVariant(k+1));
          k+=1;
      }
      ui->comboBox->setEditable(false);

      db.close();
      retirer_cpt.clear();

      verif_retrait=1;
}


void MainWindow::on_pushButton_55_clicked()
{
    nouv_list_cpt.clear();
    num_spin_cpt_pfl = ui->comboBox_5->currentData();
    QSqlDatabase db = QSqlDatabase::database();


    if (db.isOpen())
    {
       qDebug() << "Database is already open.";
    }

    else
    {

       if (!db.open())
       {
           qDebug() << "Error: connection with database failed.";
       }
       else
       {
           qDebug() << "Database is open.";
       }
     }

    QSqlQuery selection_cpt_pfl;
    QString sortie_selection_pfl;

    selection_cpt_pfl.prepare("SELECT liste_competences_pfl FROM profils WHERE id = :id");
    selection_cpt_pfl.bindValue(":id", num_profil.toInt());
    selection_cpt_pfl.exec();

    if (!selection_cpt_pfl.exec())
    {
        qDebug()<<"Erreur d'exécution de la requête 1: "<< selection_cpt_pfl.lastError();
        return ;
    }

    while(selection_cpt_pfl.next())
    {
        sortie_selection_pfl = selection_cpt_pfl.value(0).toString();
    }


    QStringList pb_a_retirer;
    QString test = sortie_selection_pfl;
    QString test2;

    for(int i=0; i < test.length(); i++)
    {
        if(test.at(i)!="\n")
        {
            test2 += test.at(i);
        }

        else
        {
            pb_a_retirer.append(test2);
            test2.clear();
        }
    }

    QString cpt_pfl_retrait;
    cpt_pfl_retrait = pb_a_retirer.at(num_spin_cpt_pfl.toInt()-1);

    pb_a_retirer.removeAll(QString::fromStdString(cpt_pfl_retrait.toStdString()));

    sortie_selection_pfl.clear();

    for(int i=0; i<pb_a_retirer.length();i++)
    {
        sortie_selection_pfl +=  pb_a_retirer.at(i) + "\n";
    }

    QSqlQuery retrait_cpt_pfl;

    retrait_cpt_pfl.prepare("UPDATE profils SET liste_competences_pfl = ? WHERE id = ? ;");
    retrait_cpt_pfl.addBindValue(sortie_selection_pfl);
    retrait_cpt_pfl.addBindValue(num_profil.toInt());
    retrait_cpt_pfl.exec();

    if (!retrait_cpt_pfl.exec())
    {
        qDebug()<<"Erreur d'exécution de la requête 2: "<< retrait_cpt_pfl.lastError();
        return ;
    }

    QString nouv_sortie_cpt_pfl;

    int k = 1;
    for(int i=0;i<pb_a_retirer.length();i++)
    {
        nouv_sortie_cpt_pfl += QString::number(k) + "- " + pb_a_retirer.at(i) + "\n";
        k+=1;
    }


    ui->textBrowser_12->setText(nouv_sortie_cpt_pfl);
    ui->textBrowser_9->setText(nouv_sortie_cpt_pfl);
    ui->label_36->setText("         La compétence a bien été retirée !");

    resultat_liste_cpt.removeAll(QString::fromStdString(cpt_pfl_retrait.toStdString()));

    resultat_liste_cpt.sort();
    resultat_liste_cpt.removeDuplicates();


    int l=1;
    for(int i=0; i<resultat_liste_cpt.length();i++)
    {
        nouv_list_cpt += QString::number(l) +"- " + resultat_liste_cpt.at(i) + "\n";
        l+=1;
    }

    ui->textBrowser_2->setText(nouv_list_cpt);

    intervalle_spin_cpt_pfl=0;
    for(int i=0;i<ui->textBrowser_12->toPlainText().length();i++)
    {
          if(ui->textBrowser_12->toPlainText().at(i)=="\n")
          {
              intervalle_spin_cpt_pfl += 1;
          }
    }

    ui->comboBox_5->clear();

    int j=0;
    while(j<intervalle_spin_cpt_pfl)
    {
        ui->comboBox_5->addItem(QString::number(j+1), QVariant(j+1));
        j+=1;
    }
    ui->comboBox_5->setEditable(false);

    db.close();
}


void MainWindow::on_pushButton_65_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}


void MainWindow::on_pushButton_61_clicked()
{

    QString cpt_a_ajouter;
    cpt_a_ajouter = ui->textEdit_9->toPlainText();

    if(cpt_a_ajouter.isEmpty())
    {
        ui->label_48->setText("              Veuillez entrer une competence !");
    }

    else
    {
        nouv_list_cpt.clear();
        cpt_a_ajouter.replace(0, 1, cpt_a_ajouter.at(0).toUpper());
        QSqlDatabase db = QSqlDatabase::database();

        if (db.isOpen())
        {
           qDebug() << "Database is already open.";
        }

        else
        {

           if (!db.open())
           {
               qDebug() << "Error: connection with database failed.";
           }
           else
           {
               qDebug() << "Database is open.";
           }
         }

        QSqlQuery ajouter_cpt;
        QString sortie_ajout_cpt;
        ajouter_cpt.exec("SELECT DISTINCT liste_competences FROM competences");

        if (!ajouter_cpt.exec())
        {
            qDebug()<<"Erreur d'exécution de la requête 6: "<< ajouter_cpt.lastError();
            return ;
        }

        int a=1;
        while(ajouter_cpt.next())
        {
            sortie_ajout_cpt += ajouter_cpt.value(0).toString() + "\n";
            a+=1;
        }


        ajouter_cpt.clear();
        ajouter_cpt.prepare("INSERT INTO competences(id, liste_competences)" "VALUES(:id, :liste_competences) ");
        ajouter_cpt.bindValue(":id", a);
        ajouter_cpt.bindValue(":liste_competences", cpt_a_ajouter);
        ajouter_cpt.exec();

        if (!ajouter_cpt.exec())
        {
            qDebug()<<"Erreur d'exécution de la requête 7: "<< ajouter_cpt.lastError();
            return ;
        }

        ajouter_cpt.prepare("SELECT DISTINCT id, liste_competences FROM competences;");

        if (!ajouter_cpt.exec())
        {
            qDebug()<<"Erreur d'exécution de la requête 8: "<< ajouter_cpt.lastError();
            return ;
        }

        sortie_ajout_cpt.clear();
        while(ajouter_cpt.next())
        {
            sortie_ajout_cpt += ajouter_cpt.value(0).toString() + "- " + ajouter_cpt.value(1).toString() + "\n";
        }

        ui->textBrowser_16->setText(sortie_ajout_cpt);
        ui->textBrowser_5->setText(sortie_ajout_cpt);

        resultat_liste_cpt.append(cpt_a_ajouter);
        resultat_liste_cpt.sort();
        resultat_liste_cpt.removeDuplicates();
        ui->label_48->setText("       La compétence a bien été ajoutée !");

        int l=1;
        for(int i=0; i<resultat_liste_cpt.length();i++)
        {
            nouv_list_cpt += QString::number(l) +"- " + resultat_liste_cpt.at(i) + "\n";
            l+=1;
        }

        ui->textBrowser_2->setText(nouv_list_cpt);
        sortie_ajout_cpt.clear();
        ui->textEdit_9->clear();
        ui->textEdit_9->clearFocus();

    }
}


void MainWindow::on_pushButton_47_clicked()
{
    QString cpt_pro_a_ajouter;
    cpt_pro_a_ajouter = ui->textEdit_5->toPlainText();

    if(cpt_pro_a_ajouter.isEmpty())
    {
        ui->label_32->setText("             Veuillez entrer une compétence !");
    }

    else
    {
        nouv_list_cpt.clear();
        cpt_pro_a_ajouter.replace(0, 1, cpt_pro_a_ajouter.at(0).toUpper());
        QSqlDatabase db = QSqlDatabase::database();

        if (db.isOpen())
        {
           qDebug() << "Database is already open.";
        }

        else
        {

           if (!db.open())
           {
               qDebug() << "Error: connection with database failed.";
           }
           else
           {
               qDebug() << "Database is open.";
           }
         }

        QSqlQuery ajouter_cpt_pro;
        QString sortie_ajout_cpt_pro;

        ajouter_cpt_pro.prepare("SELECT liste_competences_pro FROM professionnels WHERE id = :id");
        ajouter_cpt_pro.bindValue(":id", num_profess.toInt());
        ajouter_cpt_pro.exec();

        if (!ajouter_cpt_pro.exec())
        {
            qDebug()<<"Erreur d'exécution de la requête 8: "<< ajouter_cpt_pro.lastError();
            return ;
        }

        while(ajouter_cpt_pro.next())
        {
            sortie_ajout_cpt_pro = ajouter_cpt_pro.value(0).toString();
        }

        sortie_ajout_cpt_pro += cpt_pro_a_ajouter + "\n";

        ajouter_cpt_pro.clear();

        ajouter_cpt_pro.prepare("UPDATE professionnels SET liste_competences_pro = ? WHERE id = ? ;");
        ajouter_cpt_pro.addBindValue(sortie_ajout_cpt_pro);
        ajouter_cpt_pro.addBindValue(num_profess.toInt());
        ajouter_cpt_pro.exec();

        if (!ajouter_cpt_pro.exec())
        {
            qDebug()<<"Erreur d'exécution de la requête 8: "<< ajouter_cpt_pro.lastError();
            return ;
        }

        ajouter_cpt_pro.clear();
        ajouter_cpt_pro.prepare("SELECT liste_competences_pro FROM professionnels WHERE id = :id");
        ajouter_cpt_pro.bindValue(":id", num_profess.toInt());
        ajouter_cpt_pro.exec();

        if (!ajouter_cpt_pro.exec())
        {
            qDebug()<<"Erreur d'exécution de la requête 8: "<< ajouter_cpt_pro.lastError();
            return ;
        }

        sortie_ajout_cpt_pro.clear();
        while(ajouter_cpt_pro.next())
        {
            sortie_ajout_cpt_pro = ajouter_cpt_pro.value(0).toString();
        }

        QStringList pb_a_retirer;
        QString test = sortie_ajout_cpt_pro;
        QString test2;

        for(int i=0; i < test.length(); i++)
        {
            if(test.at(i)!="\n")
            {
                test2 += test.at(i);
            }

            else
            {
                pb_a_retirer.append(test2);
                test2.clear();
            }
        }

        sortie_ajout_cpt_pro.clear();

        int l=1;
        for(int i=0;i < pb_a_retirer.length();i++)
        {
            sortie_ajout_cpt_pro += QString::number(l) + "- " + pb_a_retirer.at(i) + "\n";
            l+=1;
        }

        ui->textBrowser_17->setText(sortie_ajout_cpt_pro);
        ui->textBrowser_7->setText(sortie_ajout_cpt_pro);
        ui->label_32->setText("      La compétence a bien été ajoutée !");
        ui->textEdit_5->clear();
        ui->textEdit_5->clearFocus();

        resultat_liste_cpt.append(cpt_pro_a_ajouter);
        resultat_liste_cpt.sort();
        resultat_liste_cpt.removeDuplicates();

        int k=1;
        for(int i=0; i<resultat_liste_cpt.length();i++)
        {
            nouv_list_cpt += QString::number(k) +"- " + resultat_liste_cpt.at(i) + "\n";
            k+=1;
        }

        ui->textBrowser_2->setText(nouv_list_cpt);
        sortie_ajout_cpt_pro.clear();
        ajouter_cpt_pro.clear();

    }
}


void MainWindow::on_pushButton_56_clicked()
{
    QString cpt_pfl_a_ajouter;
    cpt_pfl_a_ajouter = ui->textEdit_8->toPlainText();

    if(cpt_pfl_a_ajouter.isEmpty())
    {
        ui->label_39->setText("             Veuillez entrer une compétence !");
    }

    else
    {
        nouv_list_cpt.clear();
        cpt_pfl_a_ajouter.replace(0, 1, cpt_pfl_a_ajouter.at(0).toUpper());
        QSqlDatabase db = QSqlDatabase::database();

        if (db.isOpen())
        {
           qDebug() << "Database is already open.";
        }

        else
        {

           if (!db.open())
           {
               qDebug() << "Error: connection with database failed.";
           }
           else
           {
               qDebug() << "Database is open.";
           }
         }

        QSqlQuery ajouter_cpt_pfl;
        QString sortie_ajout_cpt_pfl;

        ajouter_cpt_pfl.prepare("SELECT liste_competences_pfl FROM profils WHERE id = :id");
        ajouter_cpt_pfl.bindValue(":id", num_profil.toInt());
        ajouter_cpt_pfl.exec();

        if (!ajouter_cpt_pfl.exec())
        {
            qDebug()<<"Erreur d'exécution de la requête 8: "<< ajouter_cpt_pfl.lastError();
            return ;
        }

        while(ajouter_cpt_pfl.next())
        {
            sortie_ajout_cpt_pfl = ajouter_cpt_pfl.value(0).toString();
        }

        sortie_ajout_cpt_pfl += cpt_pfl_a_ajouter + "\n";

        ajouter_cpt_pfl.clear();

        ajouter_cpt_pfl.prepare("UPDATE profils SET liste_competences_pfl = ? WHERE id = ? ;");
        ajouter_cpt_pfl.addBindValue(sortie_ajout_cpt_pfl);
        ajouter_cpt_pfl.addBindValue(num_profil.toInt());
        ajouter_cpt_pfl.exec();

        if (!ajouter_cpt_pfl.exec())
        {
            qDebug()<<"Erreur d'exécution de la requête 8: "<< ajouter_cpt_pfl.lastError();
            return ;
        }

        ajouter_cpt_pfl.clear();
        ajouter_cpt_pfl.prepare("SELECT liste_competences_pfl FROM profils WHERE id = :id");
        ajouter_cpt_pfl.bindValue(":id", num_profil.toInt());
        ajouter_cpt_pfl.exec();

        if (!ajouter_cpt_pfl.exec())
        {
            qDebug()<<"Erreur d'exécution de la requête 8: "<< ajouter_cpt_pfl.lastError();
            return ;
        }

        sortie_ajout_cpt_pfl.clear();
        while(ajouter_cpt_pfl.next())
        {
            sortie_ajout_cpt_pfl = ajouter_cpt_pfl.value(0).toString();
        }

        QStringList pb_a_retirer;
        QString test = sortie_ajout_cpt_pfl;
        QString test2;

        for(int i=0; i < test.length(); i++)
        {
            if(test.at(i)!="\n")
            {
                test2 += test.at(i);
            }

            else
            {
                pb_a_retirer.append(test2);
                test2.clear();
            }
        }

        sortie_ajout_cpt_pfl.clear();

        int l=1;
        for(int i=0;i < pb_a_retirer.length();i++)
        {
            sortie_ajout_cpt_pfl += QString::number(l) + "- " + pb_a_retirer.at(i) + "\n";
            l+=1;
        }

        ui->textBrowser_19->setText(sortie_ajout_cpt_pfl);
        ui->textBrowser_9->setText(sortie_ajout_cpt_pfl);
        ui->label_39->setText("      La compétence a bien été ajoutée !");
        ui->textEdit_8->clear();
        ui->textEdit_8->clearFocus();

        resultat_liste_cpt.append(cpt_pfl_a_ajouter);
        resultat_liste_cpt.sort();
        resultat_liste_cpt.removeDuplicates();

        int k=1;
        for(int i=0; i<resultat_liste_cpt.length();i++)
        {
            nouv_list_cpt += QString::number(k) +"- " + resultat_liste_cpt.at(i) + "\n";
            k+=1;
        }

        ui->textBrowser_2->setText(nouv_list_cpt);
        sortie_ajout_cpt_pfl.clear();
        ajouter_cpt_pfl.clear();

    }
}


void MainWindow::on_pushButton_66_clicked()
{
    ui->stackedWidget->setCurrentIndex(12);
}


void MainWindow::on_pushButton_67_clicked()
{
    QApplication::quit();
}


void MainWindow::on_pushButton_38_clicked()
{
    QString liste_pro_a_comparer;
    QStringList liste_cpt_pro_rapport;
    liste_pro_a_comparer = ui->textBrowser_7->toPlainText();

    QString test2;

    for(int i=0; i < liste_pro_a_comparer.length(); i++)
    {
        if(liste_pro_a_comparer.at(i)!="\n")
        {
            test2+=liste_pro_a_comparer.at(i);
        }

        else
        {
            liste_cpt_pro_rapport.append(test2.mid(3));
            test2.clear();
        }
    }

    QSqlDatabase db = QSqlDatabase::database();

    if (db.isOpen())
    {
       qDebug() << "Database is already open.";
    }

    else
    {

       if (!db.open())
       {
           qDebug() << "Error: connection with database failed.";
       }
       else
       {
           qDebug() << "Database is open.";
       }
     }

    QSqlQuery profil_comparaison;
    profil_comparaison.exec("SELECT DISTINCT id, liste_competences_pfl FROM profils");

    if (!profil_comparaison.exec())
    {
        qDebug()<<"Erreur d'exécution de la requête 8: "<< profil_comparaison.lastError();
        return ;
    }

    QStringList stock_id;
    QStringList id_cpt_stock;
    while(profil_comparaison.next())
    {
       id_cpt_stock.append(profil_comparaison.value(0).toString());
       id_cpt_stock.append(profil_comparaison.value(1).toString());
    }

    QStringList temp;
    QString test;
    QString test3;
    bool contain;
    for(int i=0;i<id_cpt_stock.size();i++)
    {
        if(i%2 != 0)
        {
            temp.clear();
            test3.clear();
            test = id_cpt_stock.at(i);
            for(int j=0; j < test.length(); j++)
            {
                if(test.at(j)!="\n")
                {
                    test3 += test.at(j);
                }

                else
                {
                    temp.append(test3);
                    test3.clear();
                }
            }

            contain=true;

            for(int t=0;t<temp.size();t++)
            {
                if(!liste_cpt_pro_rapport.contains(temp.at(t)))
                {
                    contain = false;
                    break;
                }
            }

            if(contain)
            {
                stock_id.append(id_cpt_stock.at(i-1));
            }
        }
    }

    if(stock_id.size()>0)
    {
        int p=0;
        QString liste_id;
        while(p<stock_id.size())
        {
            profil_comparaison.prepare("SELECT DISTINCT nom FROM profils WHERE id = :id");
            profil_comparaison.bindValue(":id", stock_id.at(p).toInt());
            profil_comparaison.exec();

            if (!profil_comparaison.exec())
            {
                qDebug()<<"Erreur d'exécution de la requête 8: "<< profil_comparaison.lastError();
                return ;
            }

            while(profil_comparaison.next())
            {
                liste_id += QString::number(p+1) + "- " + profil_comparaison.value(0).toString() + "\n";
            }

            p+=1;
        }

        ui->label_44->setVisible(1);
        ui->textBrowser_21->setVisible(1);
        ui->label_45->clear();
        ui->textBrowser_21->setText(liste_id);
        ui->stackedWidget->setCurrentIndex(21);
     }

     else
     {
        ui->textBrowser_21->hide();
        ui->label_44->hide();
        ui->label_45->setText("     Aucun des profils enregistrés dans la base de données ne correspond à ce professionnel !");
        ui->stackedWidget->setCurrentIndex(21);
     }

}



void MainWindow::on_pushButton_68_clicked()
{
    ui->stackedWidget->setCurrentIndex(11);
}


void MainWindow::on_pushButton_69_clicked()
{
    QApplication::quit();
}


void MainWindow::on_pushButton_33_clicked()
{
    QString liste_pfl_a_comparer;
    QStringList liste_cpt_pfl_rapport;
    liste_pfl_a_comparer = ui->textBrowser_9->toPlainText();

    QString test2;

    for(int i=0; i < liste_pfl_a_comparer.length(); i++)
    {
        if(liste_pfl_a_comparer.at(i)!="\n")
        {
            test2+=liste_pfl_a_comparer.at(i);
        }

        else
        {
            liste_cpt_pfl_rapport.append(test2.mid(3));
            test2.clear();
        }
    }

    QSqlDatabase db = QSqlDatabase::database();

    if (db.isOpen())
    {
       qDebug() << "Database is already open.";
    }

    else
    {

       if (!db.open())
       {
           qDebug() << "Error: connection with database failed.";
       }
       else
       {
           qDebug() << "Database is open.";
       }
     }

    QSqlQuery professionnel_comparaison;
    professionnel_comparaison.exec("SELECT DISTINCT id, liste_competences_pro FROM professionnels");

    if (!professionnel_comparaison.exec())
    {
        qDebug()<<"Erreur d'exécution de la requête 8: "<< professionnel_comparaison.lastError();
        return ;
    }

    QStringList stock_id;
    QStringList id_cpt_stock;
    while(professionnel_comparaison.next())
    {
       id_cpt_stock.append(professionnel_comparaison.value(0).toString());
       id_cpt_stock.append(professionnel_comparaison.value(1).toString());
    }

    QStringList temp;
    QString test;
    QString test3;
    bool contain;
    for(int i=0;i<id_cpt_stock.size();i++)
    {
        if(i%2 != 0)
        {
            temp.clear();
            test3.clear();
            test = id_cpt_stock.at(i);
            for(int j=0; j < test.length(); j++)
            {
                if(test.at(j)!="\n")
                {
                    test3 += test.at(j);
                }

                else
                {
                    temp.append(test3);
                    test3.clear();
                }
            }

            contain=true;

            for(int t=0;t<liste_cpt_pfl_rapport.size();t++)
            {
                if(!temp.contains(liste_cpt_pfl_rapport.at(t)))
                {
                    contain = false;
                    break;
                }
            }

            if(contain)
            {
                stock_id.append(id_cpt_stock.at(i-1));
            }
        }
    }

    if(stock_id.size()>0)
    {
        int p=0;
        QString liste_id;
        while(p<stock_id.size())
        {
            professionnel_comparaison.prepare("SELECT DISTINCT nom, prenoms FROM professionnels WHERE id = :id");
            professionnel_comparaison.bindValue(":id", stock_id.at(p).toInt());
            professionnel_comparaison.exec();

            if (!professionnel_comparaison.exec())
            {
                qDebug()<<"Erreur d'exécution de la requête 8: "<< professionnel_comparaison.lastError();
                return ;
            }

            while(professionnel_comparaison.next())
            {
                liste_id += QString::number(p+1) + "- " + professionnel_comparaison.value(0).toString() + " " + professionnel_comparaison.value(1).toString() + "\n";
            }

            p+=1;
        }

        ui->label_50->setVisible(1);
        ui->textBrowser_22->setVisible(1);
        ui->label_51->clear();
        ui->textBrowser_22->setText(liste_id);
        ui->stackedWidget->setCurrentIndex(22);
     }

     else
     {
        ui->textBrowser_22->hide();
        ui->label_50->hide();
        ui->label_51->setText("     Aucun des professionnels enregistrés dans la base de données ne correspond à ce profil !");
        ui->stackedWidget->setCurrentIndex(22);
     }

}


void MainWindow::on_pushButton_70_clicked()
{
    ui->textEdit_3->clear();
    ui->textEdit_3->clearFocus();
    ui->label_54->clear();
    ui->stackedWidget->setCurrentIndex(5);
}


void MainWindow::on_pushButton_71_clicked()
{
    QApplication::quit();
}


void MainWindow::on_pushButton_19_clicked()
{
    QString cpt_a_rechercher;
    cpt_a_rechercher = ui->textEdit_3->toPlainText();

    if(cpt_a_rechercher.isEmpty())
    {
        ui->label_54->setText("    Veuillez entrer le nom d'une compétence !");
    }

    else
    {
        if(!cpt_a_rechercher.at(0).isUpper())
        {
           cpt_a_rechercher.replace(0, 1, cpt_a_rechercher.at(0).toUpper());
        }

        QSqlDatabase db = QSqlDatabase::database();

        if (db.isOpen())
        {
           qDebug() << "Database is already open.";
        }

        else
        {

           if (!db.open())
           {
               qDebug() << "Error: connection with database failed.";
           }
           else
           {
               qDebug() << "Database is open.";
           }
        }

        QSqlQuery rechercher_cpt;
        QStringList sortie_recherche_cpt;
        rechercher_cpt.exec("SELECT DISTINCT id, liste_competences FROM competences");

        if (!rechercher_cpt.exec())
        {
            qDebug()<<"Erreur d'exécution de la requête 8: "<< rechercher_cpt.lastError();
            return ;
        }

        while(rechercher_cpt.next())
        {
            sortie_recherche_cpt.append(rechercher_cpt.value(0).toString());
            sortie_recherche_cpt.append(rechercher_cpt.value(1).toString());
        }

        bool contain;
        contain=true;
        if(!sortie_recherche_cpt.contains(cpt_a_rechercher))
        {
            contain=false;
        }

        if(contain)
        {
            rechercher_cpt.exec("SELECT DISTINCT id, liste_competences FROM competences");

            if (!rechercher_cpt.exec())
            {
                qDebug()<<"Erreur d'exécution de la requête 8: "<< rechercher_cpt.lastError();
                return ;
            }

            QString sortie_recherche;
            while(rechercher_cpt.next())
            {
                if(rechercher_cpt.value(1).toString()==cpt_a_rechercher)
                {
                    sortie_recherche += rechercher_cpt.value(0).toString() + "- " + rechercher_cpt.value(1).toString() + "**" + "\n";
                }
                else
                {
                   sortie_recherche += rechercher_cpt.value(0).toString() + "- " + rechercher_cpt.value(1).toString() + "\n";
                }

            }

            ui->label_52->setVisible(1);
            ui->textBrowser_23->setVisible(1);
            ui->label_53->clear();
            ui->textBrowser_23->setText(sortie_recherche);
            ui->stackedWidget->setCurrentIndex(23);
        }

        else
        {
            ui->label_55->setVisible(0);
            ui->label_52->setVisible(0);
            ui->textBrowser_23->setVisible(0);
            ui->label_53->setText("La compétence que vous recherchez ne se trouve pas dans la liste des compétences de la base de données !");
            ui->stackedWidget->setCurrentIndex(23);
        }
    }
}


void MainWindow::on_pushButton_18_clicked()
{
    QString cpt_pfl_a_rechercher;
    cpt_pfl_a_rechercher = ui->textEdit_3->toPlainText();

    if(cpt_pfl_a_rechercher.isEmpty())
    {
        ui->label_54->setText("    Veuillez entrer le nom d'une compétence !");
    }

    else
    {
        if(!cpt_pfl_a_rechercher.at(0).isUpper())
        {
           cpt_pfl_a_rechercher.replace(0, 1, cpt_pfl_a_rechercher.at(0).toUpper());
        }

        QSqlDatabase db = QSqlDatabase::database();

        if (db.isOpen())
        {
           qDebug() << "Database is already open.";
        }

        else
        {

           if (!db.open())
           {
               qDebug() << "Error: connection with database failed.";
           }
           else
           {
               qDebug() << "Database is open.";
           }
        }

        QSqlQuery rechercher_cpt_pfl;
        QStringList sortie_recherche_pfl;
        rechercher_cpt_pfl.exec("SELECT DISTINCT liste_competences_pfl FROM profils");

        if (!rechercher_cpt_pfl.exec())
        {
            qDebug()<<"Erreur d'exécution de la requête 8: "<< rechercher_cpt_pfl.lastError();
            return ;
        }

        while(rechercher_cpt_pfl.next())
        {
            sortie_recherche_pfl.append(rechercher_cpt_pfl.value(0).toString());
        }

        QStringList temp;
        QString test3;
        QString inspi;
        QStringList total_inspi;
        QStringList index;
        for(int i=0; i<sortie_recherche_pfl.size(); i++)
        {
           temp.clear();
           for(int j=0; j<sortie_recherche_pfl.at(i).length();j++)
           {
               if(sortie_recherche_pfl.at(i).at(j)!="\n")
               {
                   test3 += sortie_recherche_pfl.at(i).at(j);
               }

               else
               {
                   temp.append(test3);
                   test3.clear();
               }
           }

           if(temp.contains(cpt_pfl_a_rechercher))
           {
               index.append(QString::number(i+1));
               for(int g=0;g<temp.size();g++)
               {
                   if(temp.at(g)==cpt_pfl_a_rechercher)
                   {
                      inspi += "                   " + QString::number(g+1) + "- " +  temp.at(g) + "**" + "\n";
                   }
                   else
                   {
                       inspi += "                   " + QString::number(g+1) + "- " +  temp.at(g) + "\n";
                   }

               }

               total_inspi.append(inspi);
           }
        }

        if(index.size()>0)
        {
            QString sortie_affichage;
            for(int i=0;i<index.size();i++)
            {
                QSqlQuery affichage_recherche;
                affichage_recherche.prepare("SELECT DISTINCT nom, liste_competences_pfl FROM profils WHERE id = :id");
                affichage_recherche.bindValue(":id", index.at(i).toInt());
                affichage_recherche.exec();

                if (!affichage_recherche.exec())
                {
                    qDebug()<<"Erreur d'exécution de la requête 8: "<< affichage_recherche.lastError();
                    return ;
                }

                while(affichage_recherche.next())
                {
                    sortie_affichage += "Nom du profil : " + affichage_recherche.value(0).toString() + "\n" + "\n" + "Liste des compétences de ce profil :"  + "\n" + total_inspi.at(i) + "___________________________________________________________" + "\n" + "\n";
                }
            }

            ui->textBrowser_24->setText(sortie_affichage);
            ui->textBrowser_24->setVisible(1);
            ui->label_56->setVisible(1);
            ui->label_58->setVisible(1);
            ui->label_57->setVisible(0);
            ui->stackedWidget->setCurrentIndex(24);
        }

        else
        {
            ui->label_58->setVisible(0);
            ui->textBrowser_24->setVisible(0);
            ui->label_56->setVisible(0);
            ui->label_57->setVisible(1);
            ui->label_57->setText("Aucun des profils enregistrés dans votre base de données ne dispose de cette compétence!");
            ui->stackedWidget->setCurrentIndex(24);
        }
     }

}


void MainWindow::on_pushButton_72_clicked()
{
    ui->textEdit_3->clear();
    ui->textEdit_3->clearFocus();
    ui->label_54->clear();
    ui->stackedWidget->setCurrentIndex(5);
}


void MainWindow::on_pushButton_73_clicked()
{
    QApplication::quit();
}


void MainWindow::on_pushButton_74_clicked()
{
    ui->textEdit_3->clear();
    ui->textEdit_3->clearFocus();
    ui->label_54->clear();
    ui->stackedWidget->setCurrentIndex(5);
}


void MainWindow::on_pushButton_75_clicked()
{
    QApplication::quit();
}


void MainWindow::on_pushButton_17_clicked()
{
    QString cpt_pro_a_rechercher;
    cpt_pro_a_rechercher = ui->textEdit_3->toPlainText();

    if(cpt_pro_a_rechercher.isEmpty())
    {
        ui->label_54->setText("    Veuillez entrer le nom d'une compétence !");
    }

    else
    {
        if(!cpt_pro_a_rechercher.at(0).isUpper())
        {
           cpt_pro_a_rechercher.replace(0, 1, cpt_pro_a_rechercher.at(0).toUpper());
        }

        QSqlDatabase db = QSqlDatabase::database();

        if (db.isOpen())
        {
           qDebug() << "Database is already open.";
        }

        else
        {

           if (!db.open())
           {
               qDebug() << "Error: connection with database failed.";
           }
           else
           {
               qDebug() << "Database is open.";
           }
        }

        QSqlQuery rechercher_cpt_pro;
        QStringList sortie_recherche_pro;
        rechercher_cpt_pro.exec("SELECT DISTINCT liste_competences_pro FROM professionnels");

        if (!rechercher_cpt_pro.exec())
        {
            qDebug()<<"Erreur d'exécution de la requête 8: "<< rechercher_cpt_pro.lastError();
            return ;
        }

        while(rechercher_cpt_pro.next())
        {
            sortie_recherche_pro.append(rechercher_cpt_pro.value(0).toString());
        }

        QStringList temp;
        QString test3;
        QString inspi;
        QStringList total_inspi;
        QStringList index;
        for(int i=0; i<sortie_recherche_pro.size(); i++)
        {
           temp.clear();
           for(int j=0; j<sortie_recherche_pro.at(i).length();j++)
           {
               if(sortie_recherche_pro.at(i).at(j)!="\n")
               {
                   test3 += sortie_recherche_pro.at(i).at(j);
               }

               else
               {
                   temp.append(test3);
                   test3.clear();
               }
           }

           if(temp.contains(cpt_pro_a_rechercher))
           {
               index.append(QString::number(i+1));
               for(int g=0;g<temp.size();g++)
               {
                   if(temp.at(g)==cpt_pro_a_rechercher)
                   {
                      inspi += "                   " + QString::number(g+1) + "- " +  temp.at(g) + "**" + "\n";
                   }
                   else
                   {
                       inspi += "                   " + QString::number(g+1) + "- " +  temp.at(g) + "\n";
                   }

               }

               total_inspi.append(inspi);
               inspi.clear();
           }
        }

        if(index.size()>0)
        {
            QString sortie_affichage;
            for(int i=0;i<index.size();i++)
            {
                QSqlQuery affichage_recherche;
                affichage_recherche.prepare("SELECT DISTINCT nom, prenoms, liste_competences_pro FROM professionnels WHERE id = :id");
                affichage_recherche.bindValue(":id", index.at(i).toInt());
                affichage_recherche.exec();

                if (!affichage_recherche.exec())
                {
                    qDebug()<<"Erreur d'exécution de la requête 8: "<< affichage_recherche.lastError();
                    return ;
                }

                while(affichage_recherche.next())
                {
                    sortie_affichage += "Nom du professionnel : " + affichage_recherche.value(0).toString() + "\n" + "Prénoms du professionnel : " + affichage_recherche.value(1).toString() + "\n" + "\n" + "Liste des compétences de ce professionnel :"  + "\n" + total_inspi.at(i) + "___________________________________________________________" + "\n" + "\n";
                }
            }


            ui->textBrowser_25->setText(sortie_affichage);
            ui->textBrowser_25->setVisible(1);
            ui->label_60->setVisible(1);
            ui->label_61->setVisible(1);
            ui->label_59->setVisible(0);
            ui->stackedWidget->setCurrentIndex(25);
            sortie_affichage.clear();
        }

        else
        {
            ui->label_60->setVisible(0);
            ui->textBrowser_25->setVisible(0);
            ui->label_61->setVisible(0);
            ui->label_59->setVisible(1);
            ui->label_59->setText("Aucun des professionnels enregistrés dans votre base de données ne dispose de cette compétence!");
            ui->stackedWidget->setCurrentIndex(25);
        }
     }
}



void MainWindow::on_pushButton_76_clicked()
{
    ui->textEdit_3->clear();
    ui->textEdit_3->clearFocus();
    ui->label_54->clear();
    ui->textBrowser_26->clear();
    ui->textBrowser_27->clear();
    ui->textBrowser_28->clear();
    ui->stackedWidget->setCurrentIndex(5);
}


void MainWindow::on_pushButton_77_clicked()
{
    QApplication::quit();
}


void MainWindow::on_pushButton_16_clicked()
{
    QString cpt_a_rechercher;
    cpt_a_rechercher = ui->textEdit_3->toPlainText();

    if(cpt_a_rechercher.isEmpty())
    {
        ui->label_54->setText("    Veuillez entrer le nom d'une compétence !");
    }

    else
    {
        if(!cpt_a_rechercher.at(0).isUpper())
        {
           cpt_a_rechercher.replace(0, 1, cpt_a_rechercher.at(0).toUpper());
        }

        QSqlDatabase db = QSqlDatabase::database();

        if (db.isOpen())
        {
           qDebug() << "Database is already open.";
        }

        else
        {

           if (!db.open())
           {
               qDebug() << "Error: connection with database failed.";
           }
           else
           {
               qDebug() << "Database is open.";
           }
        }

        QSqlQuery rechercher_cpt;
        QStringList sortie_recherche_cpt;
        rechercher_cpt.exec("SELECT DISTINCT id, liste_competences FROM competences");

        if (!rechercher_cpt.exec())
        {
            qDebug()<<"Erreur d'exécution de la requête 8: "<< rechercher_cpt.lastError();
            return ;
        }

        while(rechercher_cpt.next())
        {
            sortie_recherche_cpt.append(rechercher_cpt.value(0).toString());
            sortie_recherche_cpt.append(rechercher_cpt.value(1).toString());
        }

        bool contain;
        contain=true;
        if(!sortie_recherche_cpt.contains(cpt_a_rechercher))
        {
            contain=false;
        }

        if(contain)
        {
            rechercher_cpt.exec("SELECT DISTINCT id, liste_competences FROM competences");

            if (!rechercher_cpt.exec())
            {
                qDebug()<<"Erreur d'exécution de la requête 8: "<< rechercher_cpt.lastError();
                return ;
            }

            QString sortie_recherche;
            while(rechercher_cpt.next())
            {
                if(rechercher_cpt.value(1).toString()==cpt_a_rechercher)
                {
                    sortie_recherche += rechercher_cpt.value(0).toString() + "- " + rechercher_cpt.value(1).toString() + "**" + "\n";
                }
                else
                {
                   sortie_recherche += rechercher_cpt.value(0).toString() + "- " + rechercher_cpt.value(1).toString() + "\n";
                }

            }

            ui->textBrowser_28->setVisible(1);
            ui->label_64->setText("Voici ci-dessous les résultats de la recherche!");
            ui->textBrowser_28->setText(sortie_recherche);
            ui->stackedWidget->setCurrentIndex(26);
        }

        else
        {
            ui->textBrowser_28->setVisible(0);
            ui->label_64->setText("Aucun résultat trouvé dans la base de donnée");
            ui->stackedWidget->setCurrentIndex(26);
        }
    }

    QString cpt_pro_a_rechercher;
    cpt_pro_a_rechercher = ui->textEdit_3->toPlainText();

    if(cpt_pro_a_rechercher.isEmpty())
    {
        ui->label_54->setText("    Veuillez entrer le nom d'une compétence !");
    }

    else
    {
        if(!cpt_pro_a_rechercher.at(0).isUpper())
        {
           cpt_pro_a_rechercher.replace(0, 1, cpt_pro_a_rechercher.at(0).toUpper());
        }

        QSqlDatabase db = QSqlDatabase::database();

        if (db.isOpen())
        {
           qDebug() << "Database is already open.";
        }

        else
        {

           if (!db.open())
           {
               qDebug() << "Error: connection with database failed.";
           }
           else
           {
               qDebug() << "Database is open.";
           }
        }

        QSqlQuery rechercher_cpt_pro;
        QStringList sortie_recherche_pro;
        rechercher_cpt_pro.exec("SELECT DISTINCT liste_competences_pro FROM professionnels");

        if (!rechercher_cpt_pro.exec())
        {
            qDebug()<<"Erreur d'exécution de la requête 8: "<< rechercher_cpt_pro.lastError();
            return ;
        }

        while(rechercher_cpt_pro.next())
        {
            sortie_recherche_pro.append(rechercher_cpt_pro.value(0).toString());
        }

        QStringList temp;
        QString test3;
        QString inspi;
        QStringList total_inspi;
        QStringList index;
        for(int i=0; i<sortie_recherche_pro.size(); i++)
        {
           temp.clear();
           for(int j=0; j<sortie_recherche_pro.at(i).length();j++)
           {
               if(sortie_recherche_pro.at(i).at(j)!="\n")
               {
                   test3 += sortie_recherche_pro.at(i).at(j);
               }

               else
               {
                   temp.append(test3);
                   test3.clear();
               }
           }

           if(temp.contains(cpt_pro_a_rechercher))
           {
               index.append(QString::number(i+1));
               for(int g=0;g<temp.size();g++)
               {
                   if(temp.at(g)==cpt_pro_a_rechercher)
                   {
                      inspi += "                   " + QString::number(g+1) + "- " +  temp.at(g) + "**" + "\n";
                   }
                   else
                   {
                       inspi += "                   " + QString::number(g+1) + "- " +  temp.at(g) + "\n";
                   }

               }

               total_inspi.append(inspi);
               inspi.clear();
           }
        }

        if(index.size()>0)
        {
            QString sortie_affichage;
            for(int i=0;i<index.size();i++)
            {
                QSqlQuery affichage_recherche;
                affichage_recherche.prepare("SELECT DISTINCT nom, prenoms, liste_competences_pro FROM professionnels WHERE id = :id");
                affichage_recherche.bindValue(":id", index.at(i).toInt());
                affichage_recherche.exec();

                if (!affichage_recherche.exec())
                {
                    qDebug()<<"Erreur d'exécution de la requête 8: "<< affichage_recherche.lastError();
                    return ;
                }

                while(affichage_recherche.next())
                {
                    sortie_affichage += "Nom du professionnel : " + affichage_recherche.value(0).toString() + "\n" + "Prénoms du professionnel : " + affichage_recherche.value(1).toString() + "\n" + "\n" + "Liste des compétences de ce professionnel :"  + "\n" + total_inspi.at(i) + "________________________________" + "\n" + "\n";
                }
            }

            ui->textBrowser_26->setVisible(1);
            ui->label_62->setText("Voici ci-dessous les résultats de la recherche!");
            ui->textBrowser_26->setText(sortie_affichage);
            ui->stackedWidget->setCurrentIndex(26);
            sortie_affichage.clear();
        }

        else
        {
            ui->textBrowser_26->setVisible(0);
            ui->label_62->setText("Aucun resultat trouvé dans la base de donnée");
            ui->stackedWidget->setCurrentIndex(26);
        }
     }

    QString cpt_pfl_a_rechercher;
    cpt_pfl_a_rechercher = ui->textEdit_3->toPlainText();

    if(cpt_pfl_a_rechercher.isEmpty())
    {
        ui->label_54->setText("    Veuillez entrer le nom d'une compétence !");
    }

    else
    {
        if(!cpt_pfl_a_rechercher.at(0).isUpper())
        {
           cpt_pfl_a_rechercher.replace(0, 1, cpt_pfl_a_rechercher.at(0).toUpper());
        }

        QSqlDatabase db = QSqlDatabase::database();

        if (db.isOpen())
        {
           qDebug() << "Database is already open.";
        }

        else
        {

           if (!db.open())
           {
               qDebug() << "Error: connection with database failed.";
           }
           else
           {
               qDebug() << "Database is open.";
           }
        }

        QSqlQuery rechercher_cpt_pfl;
        QStringList sortie_recherche_pfl;
        rechercher_cpt_pfl.exec("SELECT DISTINCT liste_competences_pfl FROM profils");

        if (!rechercher_cpt_pfl.exec())
        {
            qDebug()<<"Erreur d'exécution de la requête 8: "<< rechercher_cpt_pfl.lastError();
            return ;
        }

        while(rechercher_cpt_pfl.next())
        {
            sortie_recherche_pfl.append(rechercher_cpt_pfl.value(0).toString());
        }

        QStringList temp;
        QString test3;
        QString inspi;
        QStringList total_inspi;
        QStringList index;
        for(int i=0; i<sortie_recherche_pfl.size(); i++)
        {
           temp.clear();
           for(int j=0; j<sortie_recherche_pfl.at(i).length();j++)
           {
               if(sortie_recherche_pfl.at(i).at(j)!="\n")
               {
                   test3 += sortie_recherche_pfl.at(i).at(j);
               }

               else
               {
                   temp.append(test3);
                   test3.clear();
               }
           }

           if(temp.contains(cpt_pfl_a_rechercher))
           {
               index.append(QString::number(i+1));
               for(int g=0;g<temp.size();g++)
               {
                   if(temp.at(g)==cpt_pfl_a_rechercher)
                   {
                      inspi += "                   " + QString::number(g+1) + "- " +  temp.at(g) + "**" + "\n";
                   }
                   else
                   {
                       inspi += "                   " + QString::number(g+1) + "- " +  temp.at(g) + "\n";
                   }

               }

               total_inspi.append(inspi);
           }
        }

        if(index.size()>0)
        {
            QString sortie_affichage;
            for(int i=0;i<index.size();i++)
            {
                QSqlQuery affichage_recherche;
                affichage_recherche.prepare("SELECT DISTINCT nom, liste_competences_pfl FROM profils WHERE id = :id");
                affichage_recherche.bindValue(":id", index.at(i).toInt());
                affichage_recherche.exec();

                if (!affichage_recherche.exec())
                {
                    qDebug()<<"Erreur d'exécution de la requête 8: "<< affichage_recherche.lastError();
                    return ;
                }

                while(affichage_recherche.next())
                {
                    sortie_affichage += "Nom du profil : " + affichage_recherche.value(0).toString() + "\n" + "\n" + "Liste des compétences de ce profil :"  + "\n" + total_inspi.at(i) + "_________________________________" + "\n" + "\n";
                }
            }

            ui->textBrowser_27->setVisible(1);
            ui->textBrowser_27->setText(sortie_affichage);
            ui->label_63->setText("Voici ci-dessous les résultats de la recherche!");
            ui->stackedWidget->setCurrentIndex(26);
        }

        else
        {
            ui->label_63->setText("Aucun résultat trouvé dans la base de donnée");
            ui->textBrowser_27->setVisible(0);
            ui->stackedWidget->setCurrentIndex(26);
        }
     }
}


void MainWindow::on_pushButton_78_clicked()
{
    ui->stackedWidget->setCurrentIndex(27);
}


void MainWindow::on_pushButton_81_clicked()
{
    ui->textEdit->clear();
    ui->textEdit->clearFocus();
    ui->textEdit_4->clear();
    ui->textEdit_4->clearFocus();
    ui->plainTextEdit->clear();
    ui->plainTextEdit->clearFocus();
    ui->label_73->clear();
    ui->stackedWidget->setCurrentIndex(2);
}


void MainWindow::on_pushButton_82_clicked()
{
    QApplication::quit();
}


void MainWindow::on_pushButton_80_clicked()
{
    QString nom_pro_creation;
    QString prenom_pro_creation;
    QString competences_pro_creation;

    nom_pro_creation = ui->textEdit->toPlainText();
    prenom_pro_creation = ui->textEdit_4->toPlainText();
    competences_pro_creation = ui->plainTextEdit->toPlainText();

    if(nom_pro_creation.isEmpty() or prenom_pro_creation.isEmpty() or competences_pro_creation.isEmpty())
    {
        ui->label_73->setText("      Veuillez remplir tous les champs !");
    }

    else
    {
        nouv_list_cpt.clear();
        if(!nom_pro_creation.at(0).isUpper())
        {
           nom_pro_creation.replace(0, 1, nom_pro_creation.at(0).toUpper());
        }

        if(!prenom_pro_creation.at(0).isUpper())
        {
           prenom_pro_creation.replace(0, 1, prenom_pro_creation.at(0).toUpper());
        }

        QString inspi;
        QStringList inspi_co;
        for(int i=0;i<competences_pro_creation.length();i++)
        {
            if(competences_pro_creation.at(i)!='\n')
            {
                inspi += competences_pro_creation.at(i);
            }

            else
            {
                if(!inspi.at(0).isUpper())
                {
                   inspi.replace(0, 1, inspi.at(0).toUpper());
                }

                inspi_co.append(inspi);
                inspi.clear();
            }
        }

        inspi.clear();

        for(int k=0;k<inspi_co.size();k++)
        {
            inspi += inspi_co.at(k) + "\n";
        }

        QSqlDatabase db = QSqlDatabase::database();

        if (db.isOpen())
        {
           qDebug() << "Database is already open.";
        }

        else
        {

           if (!db.open())
           {
               qDebug() << "Error: connection with database failed.";
           }
           else
           {
               qDebug() << "Database is open.";
           }
        }

        QSqlQuery creer_profess;
        QString index_rec;

        creer_profess.exec("SELECT DISTINCT id FROM professionnels");

        if (!creer_profess.exec())
        {
            qDebug()<<"Erreur d'exécution de la requête 8: "<< creer_profess.lastError();
            return ;
        }

        while(creer_profess.next())
        {
            index_rec = creer_profess.value(0).toString();
        }

        creer_profess.prepare("INSERT INTO professionnels(id, nom, prenoms, nbre_cpt_pro, liste_competences_pro)" "VALUES(:id, :nom, :prenoms, :nbre_cpt_pro, :liste_competences_pro)");
        creer_profess.bindValue(":id", index_rec.toInt()+1);
        creer_profess.bindValue(":nom", nom_pro_creation);
        creer_profess.bindValue(":prenoms", prenom_pro_creation);
        creer_profess.bindValue(":nbre_cpt_pro", inspi_co.size());
        creer_profess.bindValue(":liste_competences_pro", inspi);
        creer_profess.exec();

        if (!creer_profess.exec())
        {
            qDebug()<<"Erreur d'exécution de la requête 8: "<< creer_profess.lastError();
            return ;
        }

        ui->label_73->setText("              Le professionnel a bien été créé !");
        ui->textEdit->clear();
        ui->textEdit->clearFocus();
        ui->textEdit_4->clear();
        ui->textEdit_4->clearFocus();
        ui->plainTextEdit->clear();
        ui->plainTextEdit->clearFocus();

        for(int k=0;k<inspi_co.size();k++)
        {
            resultat_liste_cpt.append(inspi_co.at(k));
        }

        resultat_liste_cpt.sort();
        resultat_liste_cpt.removeDuplicates();

        for(int l=0;l<resultat_liste_cpt.size();l++)
        {
            nouv_list_cpt += QString::number(l+1) + "- " + resultat_liste_cpt.at(l) + "\n";
        }

        ui->textBrowser_2->clear();
        ui->textBrowser_2->setText(nouv_list_cpt);
    }
}


void MainWindow::on_pushButton_83_clicked()
{
    ui->textEdit_6->clear();
    ui->textEdit_6->clearFocus();
    ui->plainTextEdit_2->clear();
    ui->plainTextEdit_2->clearFocus();
    ui->label_78->clear();
    ui->stackedWidget->setCurrentIndex(2);
}


void MainWindow::on_pushButton_84_clicked()
{
    QApplication::quit();
}


void MainWindow::on_pushButton_85_clicked()
{
    QString nom_pfl_creation;
    QString competences_pfl_creation;

    nom_pfl_creation = ui->textEdit_6->toPlainText();
    competences_pfl_creation = ui->plainTextEdit_2->toPlainText();

    if(nom_pfl_creation.isEmpty() or competences_pfl_creation.isEmpty())
    {
        ui->label_78->setText("      Veuillez remplir tous les champs !");
    }

    else
    {
        nouv_list_cpt.clear();
        if(!nom_pfl_creation.at(0).isUpper())
        {
           nom_pfl_creation.replace(0, 1, nom_pfl_creation.at(0).toUpper());
        }

        QString inspi;
        QStringList inspi_co;
        for(int i=0;i<competences_pfl_creation.length();i++)
        {
            if(competences_pfl_creation.at(i)!='\n')
            {
                inspi += competences_pfl_creation.at(i);
            }

            else
            {
                if(!inspi.at(0).isUpper())
                {
                   inspi.replace(0, 1, inspi.at(0).toUpper());
                }

                inspi_co.append(inspi);
                inspi.clear();
            }
        }

        inspi.clear();

        for(int k=0;k<inspi_co.size();k++)
        {
            inspi += inspi_co.at(k) + "\n";
        }

        QSqlDatabase db = QSqlDatabase::database();

        if (db.isOpen())
        {
           qDebug() << "Database is already open.";
        }

        else
        {

           if (!db.open())
           {
               qDebug() << "Error: connection with database failed.";
           }
           else
           {
               qDebug() << "Database is open.";
           }
        }

        QSqlQuery creer_profil;
        QString index_rec;

        creer_profil.exec("SELECT DISTINCT id FROM profils");

        if (!creer_profil.exec())
        {
            qDebug()<<"Erreur d'exécution de la requête 8: "<< creer_profil.lastError();
            return ;
        }

        while(creer_profil.next())
        {
            index_rec = creer_profil.value(0).toString();
        }

        creer_profil.prepare("INSERT INTO profils(id, nom, nbre_cpt_pfl, liste_competences_pfl)" "VALUES(:id, :nom, :nbre_cpt_pro, :liste_competences_pfl)");
        creer_profil.bindValue(":id", index_rec.toInt()+1);
        creer_profil.bindValue(":nom", nom_pfl_creation);
        creer_profil.bindValue(":nbre_cpt_pfl", inspi_co.size());
        creer_profil.bindValue(":liste_competences_pfl", inspi);
        creer_profil.exec();

        if (!creer_profil.exec())
        {
            qDebug()<<"Erreur d'exécution de la requête 8: "<< creer_profil.lastError();
            return ;
        }

        ui->label_78->setText("                    Le profil a bien été créé !");
        ui->textEdit_6->clear();
        ui->textEdit_6->clearFocus();
        ui->plainTextEdit_2->clear();
        ui->plainTextEdit_2->clearFocus();

        for(int k=0;k<inspi_co.size();k++)
        {
            resultat_liste_cpt.append(inspi_co.at(k));
        }

        resultat_liste_cpt.sort();
        resultat_liste_cpt.removeDuplicates();

        for(int l=0;l<resultat_liste_cpt.size();l++)
        {
            nouv_list_cpt += QString::number(l+1) + "- " + resultat_liste_cpt.at(l) + "\n";
        }

        ui->textBrowser_2->clear();
        ui->textBrowser_2->setText(nouv_list_cpt);
    }
}


void MainWindow::on_pushButton_79_clicked()
{
    ui->stackedWidget->setCurrentIndex(28);
}


void MainWindow::on_pushButton_89_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}


void MainWindow::on_pushButton_90_clicked()
{
    QApplication::quit();
}


void MainWindow::on_pushButton_86_clicked()
{
    nouv_list_cpt.clear();

    resultat_liste_cpt.sort();
    resultat_liste_cpt.removeDuplicates();

    for(int l=0;l<resultat_liste_cpt.size();l++)
    {
        nouv_list_cpt += QString::number(l+1) + "- " + resultat_liste_cpt.at(l) + "\n";
    }

    ui->textBrowser_29->setText(nouv_list_cpt);

    ui->comboBox_6->clear();
    int k=0;
    while(k<resultat_liste_cpt.size())
    {
        ui->comboBox_6->addItem(QString::number(k+1), QVariant(k+1));
        k+=1;
    }

    ui->comboBox_6->setEditable(false);

    ui->stackedWidget->setCurrentIndex(29);

}


void MainWindow::on_pushButton_87_clicked()
{
    ui->label_83->clear();
    num_cpt_update = ui->comboBox_6->currentData();

    ui->textBrowser_30->setText(resultat_liste_cpt.at(num_cpt_update.toInt()-1));

    ui->stackedWidget->setCurrentIndex(30);
}


void MainWindow::on_pushButton_91_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}


void MainWindow::on_pushButton_92_clicked()
{
    QApplication::quit();
}


void MainWindow::on_pushButton_88_clicked()
{
    QString cpt_update;
    QString cpt_to_update;

    cpt_to_update = ui->textBrowser_30->toPlainText();
    cpt_to_update.remove("\n");
    cpt_update = ui->textEdit_7->toPlainText();

    if(cpt_update.isEmpty())
    {
        ui->label_83->setText("Veuillez entrer un nom de compétence !");
    }

    else
    {
        if(!cpt_update.at(0).isUpper())
        {
           cpt_update.replace(0, 1, cpt_update.at(0).toUpper());
        }

        QSqlDatabase db = QSqlDatabase::database();

        if (db.isOpen())
        {
           qDebug() << "Database is already open.";
        }

        else
        {

           if (!db.open())
           {
               qDebug() << "Error: connection with database failed.";
           }
           else
           {
               qDebug() << "Database is open.";
           }
        }

        QSqlQuery update_in_cpt;
        QStringList sortie_update_cpt;
        int id_update = 0;
        update_in_cpt.exec("SELECT DISTINCT id, liste_competences FROM competences");

        if (!update_in_cpt.exec())
        {
            qDebug()<<"Erreur d'exécution de la requête 8: "<< update_in_cpt.lastError();
            return ;
        }

        while(update_in_cpt.next())
        {
            sortie_update_cpt.append(update_in_cpt.value(0).toString());
            sortie_update_cpt.append(update_in_cpt.value(1).toString());
        }

        if(sortie_update_cpt.contains(cpt_to_update))
        {
            for(int i=0;i<sortie_update_cpt.size();i++)
            {
                if(sortie_update_cpt.at(i)==cpt_to_update)
                {
                    id_update = sortie_update_cpt.at(i-1).toInt();
                }
            }

            update_in_cpt.clear();
            update_in_cpt.prepare("UPDATE competences SET liste_competences = :liste_competences WHERE id = :id");
            update_in_cpt.bindValue(":liste_competences", cpt_update);
            update_in_cpt.bindValue(":id", id_update);
            update_in_cpt.exec();

            if (!update_in_cpt.exec())
            {
                qDebug()<<"Erreur d'exécution de la requête 8: "<< update_in_cpt.lastError();
                return ;
            }
        }

        update_in_cpt.clear();
        sortie_update_cpt.clear();


        update_in_cpt.exec("SELECT DISTINCT id, liste_competences_pfl FROM profils");

        if (!update_in_cpt.exec())
        {
            qDebug()<<"Erreur d'exécution de la requête 8: "<< update_in_cpt.lastError();
            return ;
        }

        while(update_in_cpt.next())
        {
            sortie_update_cpt.append(update_in_cpt.value(0).toString());
            sortie_update_cpt.append(update_in_cpt.value(1).toString());
        }

        QVector<int> stock_id_update;
        QStringList stock_update_self;
        for(int j=0;j<sortie_update_cpt.size();j++)
        {
            if(j%2 != 0)
            {
                QStringList test_global;
                QString test = sortie_update_cpt.at(j);
                QString test4;

                for(int k=0;k<test.length();k++)
                {
                    if(test.at(k)!="\n")
                    {
                        test4 += test.at(k);
                    }

                    else
                    {
                        test_global.append(test4);
                        test4.clear();
                    }
                }

                if(test_global.contains(cpt_to_update))
                {
                    id_update = sortie_update_cpt.at(j-1).toInt();

                    test4.clear();

                    for(int p=0;p<test_global.size();p++)
                    {
                        if(test_global.at(p)==cpt_to_update)
                        {
                            test4 += cpt_update + "\n";
                        }

                        else
                        {
                            test4 += test_global.at(p) + "\n";
                        }
                    }

                    stock_update_self.append(test4);
                    stock_id_update.append(id_update);

                    test4.clear();
                }
            }
        }

        if(stock_update_self.size()>0 && stock_id_update.size()>0 && stock_update_self.size()==stock_id_update.size())
        {
            for(int g=0;g<stock_update_self.size();g++)
            {
                update_in_cpt.clear();
                update_in_cpt.prepare("UPDATE profils SET liste_competences_pfl = :liste_competences_pfl WHERE id = :id");
                update_in_cpt.bindValue(":liste_competences_pfl", stock_update_self.at(g));
                update_in_cpt.bindValue(":id", stock_id_update.at(g));

                if (!update_in_cpt.exec())
                {
                    qDebug()<<"Erreur d'exécution de la requête 8: "<< update_in_cpt.lastError();
                    return ;
                }

            }
        }

        update_in_cpt.clear();
        sortie_update_cpt.clear();


        update_in_cpt.exec("SELECT DISTINCT id, liste_competences_pro FROM professionnels");

        if (!update_in_cpt.exec())
        {
            qDebug()<<"Erreur d'exécution de la requête 8: "<< update_in_cpt.lastError();
            return ;
        }

        while(update_in_cpt.next())
        {
            sortie_update_cpt.append(update_in_cpt.value(0).toString());
            sortie_update_cpt.append(update_in_cpt.value(1).toString());
        }

        stock_id_update.clear();
        stock_update_self.clear();
        for(int j=0;j<sortie_update_cpt.size();j++)
        {
            if(j%2 != 0)
            {
                QStringList test_global;
                QString test = sortie_update_cpt.at(j);
                QString test4;

                for(int k=0;k<test.length();k++)
                {
                    if(test.at(k)!="\n")
                    {
                        test4 += test.at(k);
                    }

                    else
                    {
                        test_global.append(test4);
                        test4.clear();
                    }
                }

                if(test_global.contains(cpt_to_update))
                {
                    id_update = sortie_update_cpt.at(j-1).toInt();

                    test4.clear();

                    for(int p=0;p<test_global.size();p++)
                    {
                        if(test_global.at(p)==cpt_to_update)
                        {
                            test4 += cpt_update + "\n";
                        }

                        else
                        {
                            test4 += test_global.at(p) + "\n";
                        }
                    }

                    stock_update_self.append(test4);
                    stock_id_update.append(id_update);

                    test4.clear();
                }
            }
        }

        if(stock_update_self.size()>0 && stock_id_update.size()>0 && stock_update_self.size()==stock_id_update.size())
        {
            for(int g=0;g<stock_update_self.size();g++)
            {
                update_in_cpt.clear();
                update_in_cpt.prepare("UPDATE professionnels SET liste_competences_pro = :liste_competences_pro WHERE id = :id");
                update_in_cpt.bindValue(":liste_competences_pro", stock_update_self.at(g));
                update_in_cpt.bindValue(":id", stock_id_update.at(g));

                if (!update_in_cpt.exec())
                {
                    qDebug()<<"Erreur d'exécution de la requête 8: "<< update_in_cpt.lastError();
                    return ;
                }

            }
        }

        nouv_list_cpt.clear();

        resultat_liste_cpt.removeAll(QString::fromStdString(cpt_to_update.toStdString()));
        resultat_liste_cpt.append(cpt_update);

        resultat_liste_cpt.removeDuplicates();
        resultat_liste_cpt.sort();

        for(int l=0;l<resultat_liste_cpt.size();l++)
        {
            nouv_list_cpt += QString::number(l+1) + "- " + resultat_liste_cpt.at(l) + "\n";
        }

        ui->textBrowser_2->clear();
        ui->textBrowser_2->setText(nouv_list_cpt);

        ui->textBrowser_30->setText(cpt_update);
        ui->textEdit_7->clear();
        ui->textEdit_7->clearFocus();

        ui->label_83->setText("Mise à jour de la compétence éffectuée !");

    }
}


void MainWindow::on_pushButton_96_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
    ui->label_88->clear();
}


void MainWindow::on_pushButton_97_clicked()
{
    QApplication::quit();
}


void MainWindow::on_pushButton_93_clicked()
{
    ui->comboBox_7->clear();
    QSqlDatabase db = QSqlDatabase::database();

    if (db.isOpen())
    {
       qDebug() << "Database is already open.";
    }

    else
    {

       if (!db.open())
       {
           qDebug() << "Error: connection with database failed.";
       }
       else
       {
           qDebug() << "Database is open.";
       }
     }

    QSqlQuery delete_pro;
    QString sortie_delete_pro;

   delete_pro.exec("SELECT DISTINCT id, nom, prenoms FROM professionnels");

    if (!delete_pro.exec())
    {
        qDebug()<<"Erreur d'exécution de la requête : "<< delete_pro.lastError();
        return ;
    }

    while(delete_pro.next())
    {
        sortie_delete_pro += delete_pro.value(0).toString() + "- " + delete_pro.value(1).toString() + "  " + delete_pro.value(2).toString() + "\n" ;
    }


    ui->textBrowser_31->setText(sortie_delete_pro);
    ui->stackedWidget->setCurrentIndex(31);

    intervalle_spin_del_pro=0;
    for(int i=0;i<sortie_delete_pro.length();i++)
    {
        if(sortie_delete_pro.at(i)=="\n")
        {
            intervalle_spin_del_pro += 1;
        }
    }

    int k=0;
    while(k<intervalle_spin_del_pro)
    {
        ui->comboBox_7->addItem(QString::number(k+1), QVariant(k+1));
        k+=1;
    }

    ui->comboBox_7->setEditable(false);

    delete_pro.clear();
    db.close();
}


void MainWindow::on_pushButton_95_clicked()
{
    num_pro_del = ui->comboBox_7->currentData();

    QSqlDatabase db = QSqlDatabase::database();

    if (db.isOpen())
    {
       qDebug() << "Database is already open.";
    }

    else
    {

       if (!db.open())
       {
           qDebug() << "Error: connection with database failed.";
       }
       else
       {
           qDebug() << "Database is open.";
       }
     }

    QSqlQuery suppression;
    suppression.prepare("DELETE FROM professionnels WHERE id = :id");
    suppression.bindValue(":id", num_pro_del.toInt());
    suppression.exec();

    if (!suppression.exec())
    {
        qDebug()<<"Erreur d'exécution de la requête : "<< suppression.lastError();
        return ;
    }

    suppression.prepare("UPDATE professionnels SET id = id - 1  WHERE id > :id");
    suppression.bindValue(":id", num_pro_del.toInt());
    suppression.exec();

    if (!suppression.exec())
    {
        qDebug()<<"Erreur d'exécution de la requête : "<< suppression.lastError();
        return ;
    }

    QString sortie_suppression_pro;

   suppression.exec("SELECT DISTINCT id, nom, prenoms FROM professionnels");

    if (!suppression.exec())
    {
        qDebug()<<"Erreur d'exécution de la requête : "<< suppression.lastError();
        return ;
    }

    while(suppression.next())
    {
        sortie_suppression_pro += suppression.value(0).toString() + "- " + suppression.value(1).toString() + "  " + suppression.value(2).toString() + "\n" ;
    }


    ui->textBrowser_31->setText(sortie_suppression_pro);
    ui->label_88->setText("Le professionnel a bien été supprimé !");

    intervalle_spin_del_pro=0;
    for(int i=0;i<sortie_suppression_pro.length();i++)
    {
        if(sortie_suppression_pro.at(i)=="\n")
        {
            intervalle_spin_del_pro += 1;
        }
    }

    int k=0;
    ui->comboBox_7->clear();
    while(k<intervalle_spin_del_pro)
    {
        ui->comboBox_7->addItem(QString::number(k+1), QVariant(k+1));
        k+=1;
    }

    ui->comboBox_7->setEditable(false);

}


void MainWindow::on_pushButton_98_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
    ui->label_90->clear();
}


void MainWindow::on_pushButton_99_clicked()
{
    QApplication::quit();
}


void MainWindow::on_pushButton_94_clicked()
{
    ui->comboBox_8->clear();
    QSqlDatabase db = QSqlDatabase::database();

    if (db.isOpen())
    {
       qDebug() << "Database is already open.";
    }

    else
    {

       if (!db.open())
       {
           qDebug() << "Error: connection with database failed.";
       }
       else
       {
           qDebug() << "Database is open.";
       }
     }

    QSqlQuery delete_pfl;
    QString sortie_delete_pfl;

   delete_pfl.exec("SELECT DISTINCT id, nom FROM profils");

    if (!delete_pfl.exec())
    {
        qDebug()<<"Erreur d'exécution de la requête : "<< delete_pfl.lastError();
        return ;
    }

    while(delete_pfl.next())
    {
        sortie_delete_pfl += delete_pfl.value(0).toString() + "- " + delete_pfl.value(1).toString() + "\n" ;
    }


    ui->textBrowser_32->setText(sortie_delete_pfl);
    ui->stackedWidget->setCurrentIndex(32);

    intervalle_spin_del_pfl=0;
    for(int i=0;i<sortie_delete_pfl.length();i++)
    {
        if(sortie_delete_pfl.at(i)=="\n")
        {
            intervalle_spin_del_pfl += 1;
        }
    }

    int k=0;

    while(k<intervalle_spin_del_pfl)
    {
        ui->comboBox_8->addItem(QString::number(k+1), QVariant(k+1));
        k+=1;
    }

    ui->comboBox_8->setEditable(false);

    delete_pfl.clear();
    db.close();
}

void MainWindow::on_pushButton_100_clicked()
{
    num_pfl_del = ui->comboBox_8->currentData();

    QSqlDatabase db = QSqlDatabase::database();

    if (db.isOpen())
    {
       qDebug() << "Database is already open.";
    }

    else
    {

       if (!db.open())
       {
           qDebug() << "Error: connection with database failed.";
       }
       else
       {
           qDebug() << "Database is open.";
       }
     }

    QSqlQuery suppression;
    suppression.prepare("DELETE FROM profils WHERE id = :id");
    suppression.bindValue(":id", num_pfl_del.toInt());
    suppression.exec();

    if (!suppression.exec())
    {
        qDebug()<<"Erreur d'exécution de la requête : "<< suppression.lastError();
        return ;
    }

    suppression.prepare("UPDATE profils SET id = id - 1  WHERE id > :id");
    suppression.bindValue(":id", num_pfl_del.toInt());
    suppression.exec();

    if (!suppression.exec())
    {
        qDebug()<<"Erreur d'exécution de la requête : "<< suppression.lastError();
        return ;
    }

    QString sortie_suppression_pfl;

   suppression.exec("SELECT DISTINCT id, nom FROM profils");

    if (!suppression.exec())
    {
        qDebug()<<"Erreur d'exécution de la requête : "<< suppression.lastError();
        return ;
    }

    while(suppression.next())
    {
        sortie_suppression_pfl += suppression.value(0).toString() + "- " + suppression.value(1).toString() + "\n" ;
    }


    ui->textBrowser_32->setText(sortie_suppression_pfl);
    ui->label_90->setText("Le profil a bien été supprimé !");

    intervalle_spin_del_pfl=0;
    for(int i=0;i<sortie_suppression_pfl.length();i++)
    {
        if(sortie_suppression_pfl.at(i)=="\n")
        {
            intervalle_spin_del_pfl += 1;
        }
    }

    int k=0;
    ui->comboBox_8->clear();
    while(k<intervalle_spin_del_pfl)
    {
        ui->comboBox_8->addItem(QString::number(k+1), QVariant(k+1));
        k+=1;
    }

    ui->comboBox_8->setEditable(false);
}


void MainWindow::on_pushButton_106_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}


void MainWindow::on_pushButton_107_clicked()
{
    QApplication::quit();
}


void MainWindow::on_pushButton_101_clicked()
{
    ui->textBrowser_33->setText(ui->textBrowser_3->toPlainText());
    ui->textBrowser_34->setText(ui->textBrowser_6->toPlainText());
    ui->label_96->setVisible(0);
    ui->textEdit_10->setVisible(0);
    ui->pushButton_103->setVisible(0);
    ui->label_97->setVisible(0);
    ui->textEdit_11->setVisible(0);
    ui->pushButton_105->setVisible(0);
    ui->label_98->clear();
    ui->label_99->clear();
    ui->stackedWidget->setCurrentIndex(33);
}


void MainWindow::on_pushButton_102_clicked()
{
    ui->label_96->setVisible(1);
    ui->textEdit_10->setVisible(1);
    ui->pushButton_103->setVisible(1);
    ui->label_97->setVisible(0);
    ui->textEdit_11->setVisible(0);
    ui->pushButton_105->setVisible(0);
}


void MainWindow::on_pushButton_104_clicked()
{
    ui->label_96->setVisible(0);
    ui->textEdit_10->setVisible(0);
    ui->pushButton_103->setVisible(0);
    ui->label_97->setVisible(1);
    ui->textEdit_11->setVisible(1);
    ui->pushButton_105->setVisible(1);
}


void MainWindow::on_pushButton_103_clicked()
{
    QString new_nom_pro;
    new_nom_pro = ui->textEdit_10->toPlainText();

    if(new_nom_pro.isEmpty())
    {
        ui->label_98->setText("Veuillez entrer le nouveau nom !");
    }

    else
    {
        if(!new_nom_pro.isUpper())
        {
           new_nom_pro = new_nom_pro.toUpper();
        }

        QSqlDatabase db = QSqlDatabase::database();

        if (db.isOpen())
        {
           qDebug() << "Database is already open.";
        }

        else
        {

           if (!db.open())
           {
               qDebug() << "Error: connection with database failed.";
           }
           else
           {
               qDebug() << "Database is open.";
           }
         }

        QSqlQuery nom_update;
        QString sortie_nom_update;
        nom_update.prepare("UPDATE professionnels SET nom = ? WHERE id = ?");
        nom_update.addBindValue(new_nom_pro);
        nom_update.addBindValue(ui->comboBox_2->currentData().toInt());
        nom_update.exec();

        if (!nom_update.exec())
        {
            qDebug()<<"Erreur d'exécution de la requête : "<< nom_update.lastError();
            return ;
        }

        nom_update.clear();
        nom_update.prepare("SELECT  nom FROM professionnels WHERE id = :id");
        nom_update.bindValue(":id", ui->comboBox_2->currentData().toInt());
        nom_update.exec();

        if (!nom_update.exec())
        {
            qDebug()<<"Erreur d'exécution de la requête : "<< nom_update.lastError();
            return ;
        }

        while(nom_update.next())
        {
            sortie_nom_update = nom_update.value(0).toString();
        }

        ui->textBrowser_33->setText(sortie_nom_update);
        ui->label_98->setText("Mise à jour du nom éffectuée !");

        ui->label_96->setVisible(0);
        ui->textEdit_10->setVisible(0);
        ui->pushButton_103->setVisible(0);
        ui->textEdit_10->clearFocus();

    }
}


void MainWindow::on_pushButton_105_clicked()
{
    QString new_prenom_pro;
    new_prenom_pro = ui->textEdit_11->toPlainText();

    if(new_prenom_pro.isEmpty())
    {
        ui->label_98->setText("Veuillez entrer le nouveau nom !");
    }

    else
    {
        if(!new_prenom_pro.isUpper())
        {
           new_prenom_pro.replace(0, 1, new_prenom_pro.at(0).toUpper());
        }

        QSqlDatabase db = QSqlDatabase::database();

        if (db.isOpen())
        {
           qDebug() << "Database is already open.";
        }

        else
        {

           if (!db.open())
           {
               qDebug() << "Error: connection with database failed.";
           }
           else
           {
               qDebug() << "Database is open.";
           }
         }

        QSqlQuery prenom_update;
        QString sortie_prenom_update;
        prenom_update.prepare("UPDATE professionnels SET prenoms = ? WHERE id = ?");
        prenom_update.addBindValue(new_prenom_pro);
        prenom_update.addBindValue(ui->comboBox_2->currentData().toInt());
        prenom_update.exec();

        if (!prenom_update.exec())
        {
            qDebug()<<"Erreur d'exécution de la requête : "<< prenom_update.lastError();
            return ;
        }

        prenom_update.clear();
        prenom_update.prepare("SELECT  prenoms FROM professionnels WHERE id = :id");
        prenom_update.bindValue(":id", ui->comboBox_2->currentData().toInt());
        prenom_update.exec();

        if (!prenom_update.exec())
        {
            qDebug()<<"Erreur d'exécution de la requête : "<< prenom_update.lastError();
            return ;
        }

        while(prenom_update.next())
        {
            sortie_prenom_update = prenom_update.value(0).toString();
        }

        ui->textBrowser_34->setText(sortie_prenom_update);
        ui->label_99->setText("Mise à jour du prénom éffectuée !");

        ui->label_97->setVisible(0);
        ui->textEdit_11->setVisible(0);
        ui->pushButton_105->setVisible(0);
        ui->textEdit_11->clearFocus();

    }
}


void MainWindow::on_pushButton_109_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}


void MainWindow::on_pushButton_110_clicked()
{
    QApplication::quit();
}


void MainWindow::on_pushButton_108_clicked()
{
    ui->label_103->setVisible(0);
    ui->textEdit_12->setVisible(0);
    ui->pushButton_111->setVisible(0);
    ui->textBrowser_35->setText(ui->textBrowser_8->toPlainText());
    ui->label_104->clear();
    ui->stackedWidget->setCurrentIndex(34);
}


void MainWindow::on_pushButton_112_clicked()
{
    ui->label_103->setVisible(1);
    ui->textEdit_12->setVisible(1);
    ui->pushButton_111->setVisible(1);
}

void MainWindow::on_pushButton_111_clicked()
{
    QString new_nom_pfl;
    new_nom_pfl = ui->textEdit_12->toPlainText();

    if(new_nom_pfl.isEmpty())
    {
        ui->label_104->setText("Veuillez entrer le nouveau nom !");
    }

    else
    {
        if(!new_nom_pfl.isUpper())
        {
           new_nom_pfl.replace(0, 1, new_nom_pfl.at(0).toUpper());
        }

        QSqlDatabase db = QSqlDatabase::database();

        if (db.isOpen())
        {
           qDebug() << "Database is already open.";
        }

        else
        {

           if (!db.open())
           {
               qDebug() << "Error: connection with database failed.";
           }
           else
           {
               qDebug() << "Database is open.";
           }
         }

        QSqlQuery nom_update;
        QString sortie_nom_update;
        nom_update.prepare("UPDATE profils SET nom = ? WHERE id = ?");
        nom_update.addBindValue(new_nom_pfl);
        nom_update.addBindValue(ui->comboBox_4->currentData().toInt());
        nom_update.exec();

        if (!nom_update.exec())
        {
            qDebug()<<"Erreur d'exécution de la requête : "<< nom_update.lastError();
            return ;
        }

        nom_update.clear();
        nom_update.prepare("SELECT nom FROM profils WHERE id = :id");
        nom_update.bindValue(":id", ui->comboBox_4->currentData().toInt());
        nom_update.exec();

        if (!nom_update.exec())
        {
            qDebug()<<"Erreur d'exécution de la requête : "<< nom_update.lastError();
            return ;
        }

        while(nom_update.next())
        {
            sortie_nom_update = nom_update.value(0).toString();
        }

        ui->textBrowser_35->setText(sortie_nom_update);
        ui->label_104->setText("Mise à jour du nom éffectuée !");

        ui->label_103->setVisible(0);
        ui->textEdit_12->setVisible(0);
        ui->pushButton_111->setVisible(0);
        ui->textEdit_12->clearFocus();

    }
}


void MainWindow::on_pushButton_113_clicked()
{
    QSqlDatabase db = QSqlDatabase::database();

    if (db.isOpen())
    {
       qDebug() << "Database is already open.";
    }

    else
    {

       if (!db.open())
       {
           qDebug() << "Error: connection with database failed.";
       }
       else
       {
           qDebug() << "Database is open.";
       }
     }

    QSqlQuery export_donnees;
    QStringList contain_cpt;
    int id_max=0;

    export_donnees.exec("SELECT DISTINCT id, liste_competences FROM competences");

    if (!export_donnees.exec())
    {
        qDebug()<<"Erreur d'exécution de la requête : "<< export_donnees.lastError();
        return ;
    }

    while(export_donnees.next())
    {
        contain_cpt.append(export_donnees.value(1).toString().remove("\n"));
        id_max = export_donnees.value(0).toInt();
    }

    QString chemin_fichier_export = QFileDialog::getSaveFileName(nullptr, "Exporter uniquement les compétences isolées", ".", "Texte (*.cpt)");

    if(chemin_fichier_export.isEmpty())
    {
        ui->stackedWidget->setCurrentIndex(4);
    }

    else
    {
        QFile file(chemin_fichier_export);

        if(!file.open(QFile::WriteOnly  |  QFile::Text))
        {
            QMessageBox::warning(this, "Attention", "Le fichier que vous essayez d'ouvrir ne peut pas être ouvert : " + file.errorString());
            ui->stackedWidget->setCurrentIndex(4);
        }

        else
        {
            QTextStream out(&file);

            out << "CPT:" << endl;
            out << "\t" + QString::number(id_max) << endl;

            int f=0;
            while(f<id_max)
            {
                out << "\t" + contain_cpt.at(f) << endl;
                f+=1;
            }

            file.close();
        }

        ui->label_108->setText("Sauvegarde des données réussie ! Pour consulter vos sauvegardes, rendez vous dans le répertoire où vous les avez enregistrées.");

    }
}


void MainWindow::on_pushButton_115_clicked()
{
    QSqlDatabase db = QSqlDatabase::database();

    if (db.isOpen())
    {
       qDebug() << "Database is already open.";
    }

    else
    {

       if (!db.open())
       {
           qDebug() << "Error: connection with database failed.";
       }
       else
       {
           qDebug() << "Database is open.";
       }
     }

    QSqlQuery export_donnees_pfl;
    QStringList donnees_pfl;

    export_donnees_pfl.exec("SELECT DISTINCT nom , liste_competences_pfl FROM profils");

    if (!export_donnees_pfl.exec())
    {
        qDebug()<<"Erreur d'exécution de la requête : "<< export_donnees_pfl.lastError();
        return ;
    }

    while(export_donnees_pfl.next())
    {
        donnees_pfl.append(export_donnees_pfl.value(0).toString());
        donnees_pfl.append(export_donnees_pfl.value(1).toString());
    }

    QString chemin_fichier_export = QFileDialog::getSaveFileName(nullptr, "Exporter uniquement les compétences isolées", ".", "Texte (*.cpt)");

    if(chemin_fichier_export.isEmpty())
    {
        ui->stackedWidget->setCurrentIndex(4);
    }

    else
    {
        QFile file(chemin_fichier_export);

        if(!file.open(QFile::WriteOnly  |  QFile::Text))
        {
            QMessageBox::warning(this, "Attention", "Le fichier que vous essayez d'ouvrir ne peut pas être ouvert : " + file.errorString());
            ui->stackedWidget->setCurrentIndex(4);
        }

        else
        {
            for(int i=0;i<donnees_pfl.size();i++)
            {
                if(i%2 != 0)
                {
                    QStringList temp;
                    QString test = donnees_pfl.at(i);
                    QString test8;
                    int decompt=0;

                    for(int k=0;k<test.length();k++)
                    {
                        if(test.at(k)!="\n")
                        {
                            test8 += test.at(k);
                        }

                        else
                        {
                            decompt+=1;
                            temp.append(test8);
                            test8.clear();
                        }
                     }

                    QTextStream out(&file);

                    if(i != donnees_pfl.size()-1)
                    {
                        out << "PFL:" << endl;
                        out << "\t" + donnees_pfl.at(i-1) << endl;
                        out << "\t" + QString::number(decompt) << endl;

                        int f=0;
                        while(f<decompt)
                        {
                            out << "\t" + temp.at(f) << endl;
                            f+=1;
                        }

                        out << endl;
                    }

                    else
                    {
                        out << "PFL:" << endl;
                        out << "\t" + donnees_pfl.at(i-1) << endl;
                        out << "\t" + QString::number(decompt) << endl;

                        int f=0;
                        while(f<decompt)
                        {
                            out << "\t" + temp.at(f) << endl;
                            f+=1;
                        }

                    }

                  }
            }
        }

        file.close();
        ui->label_108->setText("Sauvegarde des données réussie ! Pour consulter vos sauvegardes, rendez vous dans le répertoire ou vous les avez enregistrées.");
    }

}


void MainWindow::on_pushButton_116_clicked()
{
    QSqlDatabase db = QSqlDatabase::database();

    if (db.isOpen())
    {
       qDebug() << "Database is already open.";
    }

    else
    {

       if (!db.open())
       {
           qDebug() << "Error: connection with database failed.";
       }
       else
       {
           qDebug() << "Database is open.";
       }
     }

    QSqlQuery export_donnees_pro;
    QStringList donnees_pro;

    export_donnees_pro.exec("SELECT DISTINCT nom, prenoms, liste_competences_pro FROM professionnels");

    if (!export_donnees_pro.exec())
    {
        qDebug()<<"Erreur d'exécution de la requête : "<< export_donnees_pro.lastError();
        return ;
    }

    while(export_donnees_pro.next())
    {
        donnees_pro.append(export_donnees_pro.value(0).toString());
        donnees_pro.append(export_donnees_pro.value(1).toString());
        donnees_pro.append(export_donnees_pro.value(2).toString());
    }

    QString chemin_fichier_export = QFileDialog::getSaveFileName(nullptr, "Exporter uniquement les compétences isolées", ".", "Texte (*.cpt)");

    if(chemin_fichier_export.isEmpty())
    {
        ui->stackedWidget->setCurrentIndex(4);
    }

    else
    {
        QFile file(chemin_fichier_export);

        if(!file.open(QFile::WriteOnly  |  QFile::Text))
        {
            QMessageBox::warning(this, "Attention", "Le fichier que vous essayez d'ouvrir ne peut pas être ouvert : " + file.errorString());
            ui->stackedWidget->setCurrentIndex(4);
        }

        else
        {
            for(int i=2;i<donnees_pro.size();i+=3)
            {
                    QStringList temp;
                    QString test = donnees_pro.at(i);
                    QString test8;
                    int decompt=0;

                    for(int k=0;k<test.length();k++)
                    {
                        if(test.at(k)!="\n")
                        {
                            test8 += test.at(k);
                        }

                        else
                        {
                            decompt+=1;
                            temp.append(test8);
                            test8.clear();
                        }
                     }

                    QTextStream out(&file);

                    if(i != donnees_pro.size()-1)
                    {
                        out << "PRO:" << endl;
                        out << "\t" + donnees_pro.at(i-2) << endl;
                        out << "\t" + donnees_pro.at(i-1) << endl;
                        out << "\t" + QString::number(decompt) << endl;

                        int f=0;
                        while(f<decompt)
                        {
                            out << "\t" + temp.at(f) << endl;
                            f+=1;
                        }

                        out << endl;
                    }

                    else
                    {
                        out << "PRO:" << endl;
                        out << "\t" + donnees_pro.at(i-2) << endl;
                        out << "\t" + donnees_pro.at(i-1) << endl;
                        out << "\t" + QString::number(decompt) << endl;

                        int f=0;
                        while(f<decompt)
                        {
                            out << "\t" + temp.at(f) << endl;
                            f+=1;
                        }
                    }

                  }
        }

        file.close();
        ui->label_108->setText("Sauvegarde des données réussie ! Pour consulter vos sauvegardes, rendez vous dans le répertoire ou vous les avez enregistrées.");
    }

}


void MainWindow::on_pushButton_114_clicked()
{
    QSqlDatabase db = QSqlDatabase::database();

    if (db.isOpen())
    {
       qDebug() << "Database is already open.";
    }

    else
    {

       if (!db.open())
       {
           qDebug() << "Error: connection with database failed.";
       }
       else
       {
           qDebug() << "Database is open.";
       }
     }

    QSqlQuery export_donnees_pro;
    QStringList donnees_pro;

    export_donnees_pro.exec("SELECT DISTINCT nom, prenoms, liste_competences_pro FROM professionnels");

    if (!export_donnees_pro.exec())
    {
        qDebug()<<"Erreur d'exécution de la requête : "<< export_donnees_pro.lastError();
        return ;
    }

    while(export_donnees_pro.next())
    {
        donnees_pro.append(export_donnees_pro.value(0).toString());
        donnees_pro.append(export_donnees_pro.value(1).toString());
        donnees_pro.append(export_donnees_pro.value(2).toString());
    }

    QSqlQuery export_donnees_pfl;
    QStringList donnees_pfl;

    export_donnees_pfl.exec("SELECT DISTINCT nom , liste_competences_pfl FROM profils");

    if (!export_donnees_pfl.exec())
    {
        qDebug()<<"Erreur d'exécution de la requête : "<< export_donnees_pfl.lastError();
        return ;
    }

    while(export_donnees_pfl.next())
    {
        donnees_pfl.append(export_donnees_pfl.value(0).toString());
        donnees_pfl.append(export_donnees_pfl.value(1).toString());
    }

    QSqlQuery export_donnees;
    QStringList contain_cpt;
    int id_max=0;

    export_donnees.exec("SELECT DISTINCT id, liste_competences FROM competences");

    if (!export_donnees.exec())
    {
        qDebug()<<"Erreur d'exécution de la requête : "<< export_donnees.lastError();
        return ;
    }

    while(export_donnees.next())
    {
        contain_cpt.append(export_donnees.value(1).toString().remove("\n"));
        id_max = export_donnees.value(0).toInt();
    }

    QString chemin_fichier_export = QFileDialog::getSaveFileName(nullptr, "Exporter uniquement les compétences isolées", ".", "Texte (*.cpt)");

    if(chemin_fichier_export.isEmpty())
    {
        ui->stackedWidget->setCurrentIndex(4);
    }

    else
    {
        QFile file(chemin_fichier_export);

        if(!file.open(QFile::WriteOnly  |  QFile::Text))
        {
            QMessageBox::warning(this, "Attention", "Le fichier que vous essayez d'ouvrir ne peut pas être ouvert : " + file.errorString());
            ui->stackedWidget->setCurrentIndex(4);
        }

        else
        {
            for(int i=2;i<donnees_pro.size();i+=3)
            {

                    QStringList temp;
                    QString test = donnees_pro.at(i);
                    QString test8;
                    int decompt=0;

                    for(int k=0;k<test.length();k++)
                    {
                        if(test.at(k)!="\n")
                        {
                            test8 += test.at(k);
                        }

                        else
                        {
                            decompt+=1;
                            temp.append(test8);
                            test8.clear();
                        }
                     }

                    QTextStream out(&file);

                    out << "PRO:" << endl;
                    out << "\t" + donnees_pro.at(i-2) << endl;
                    out << "\t" + donnees_pro.at(i-1) << endl;
                    out << "\t" + QString::number(decompt) << endl;

                    int f=0;
                    while(f<decompt)
                    {
                        out << "\t" + temp.at(f) << endl;
                        f+=1;
                    }

                    out << endl;

            }

            for(int i=0;i<donnees_pfl.size();i++)
            {
                if(i%2 != 0)
                {
                    QStringList temp;
                    QString test = donnees_pfl.at(i);
                    QString test8;
                    int decompt=0;

                    for(int k=0;k<test.length();k++)
                    {
                        if(test.at(k)!="\n")
                        {
                            test8 += test.at(k);
                        }

                        else
                        {
                            decompt+=1;
                            temp.append(test8);
                            test8.clear();
                        }
                     }

                    QTextStream out(&file);

                    out << "PFL:" << endl;
                    out << "\t" + donnees_pfl.at(i-1) << endl;
                    out << "\t" + QString::number(decompt) << endl;

                    int f=0;
                    while(f<decompt)
                    {
                        out << "\t" + temp.at(f) << endl;
                        f+=1;
                    }

                    out << endl;

                  }
            }

            QTextStream out(&file);

            out << "CPT:" << endl;
            out << "\t" + QString::number(id_max) << endl;

            int f=0;
            while(f<id_max)
            {
                out << "\t" + contain_cpt.at(f) << endl;
                f+=1;
            }

            file.close();
            ui->label_108->setText("Sauvegarde des données réussie ! Pour consulter vos sauvegardes, rendez vous dans le répertoire ou vous les avez enregistrées.");
        }
    }
}

