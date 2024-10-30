#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDriver>
#include <QSqlDatabase>
#include <QDebug>
#include <QSqlQuery>
#include <QFileSystemModel>
#include <QTabWidget>
#include <QPlainTextEdit>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QSqlDatabase db;


private slots:

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_12_clicked();

    void on_pushButton_4_clicked();

    void on_actionMenu_principal_triggered();

    void on_pushButton_13_clicked();

    void on_pushButton_10_clicked();

    void on_actionExporter_la_base_de_donn_es_triggered();

    void on_pushButton_15_clicked();

    void on_actionR_initialiser_la_base_de_donn_es_triggered();

    void on_pushButton_20_clicked();

    void on_actionRechercher_triggered();

    void on_pushButton_7_clicked();

    void on_pushButton_22_clicked();

    void on_pushButton_24_clicked();

    void on_pushButton_23_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_21_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_26_clicked();

    void on_pushButton_27_clicked();

    void on_pushButton_29_clicked();

    void on_pushButton_30_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_34_clicked();

    void on_pushButton_35_clicked();

    void on_pushButton_39_clicked();

    void on_pushButton_40_clicked();

    void on_pushButton_44_clicked();

    void on_pushButton_43_clicked();

    void on_pushButton_11_clicked();

    void on_pushButton_28_clicked();

    void on_pushButton_25_clicked();

    int insert_donnees(QString chemin_fichier);

    void on_pushButton_45_clicked();

    void on_pushButton_46_clicked();

    void on_pushButton_37_clicked();

    void on_pushButton_36_clicked();

    void on_pushButton_50_clicked();

    void on_pushButton_51_clicked();

    void on_pushButton_49_clicked();

    void on_pushButton_48_clicked();

    void on_pushButton_53_clicked();

    void on_pushButton_54_clicked();

    void on_pushButton_32_clicked();

    void on_pushButton_57_clicked();

    void on_pushButton_58_clicked();

    void on_pushButton_31_clicked();

    void on_pushButton_59_clicked();

    void on_pushButton_60_clicked();

    void on_pushButton_63_clicked();

    void on_pushButton_64_clicked();

    void on_pushButton_42_clicked();

    void on_pushButton_41_clicked();

    void on_pushButton_52_clicked();

    void on_pushButton_62_clicked();

    void on_pushButton_55_clicked();

    void on_pushButton_65_clicked();

    void on_pushButton_61_clicked();

    void on_pushButton_47_clicked();

    void on_pushButton_56_clicked();

    void on_pushButton_66_clicked();

    void on_pushButton_67_clicked();

    void on_pushButton_38_clicked();

    void on_pushButton_68_clicked();

    void on_pushButton_69_clicked();

    void on_pushButton_33_clicked();

    void on_pushButton_70_clicked();

    void on_pushButton_71_clicked();

    void on_pushButton_19_clicked();

    void on_pushButton_18_clicked();

    void on_pushButton_72_clicked();

    void on_pushButton_73_clicked();

    void on_pushButton_74_clicked();

    void on_pushButton_75_clicked();

    void on_pushButton_17_clicked();

    void on_pushButton_76_clicked();

    void on_pushButton_77_clicked();

    void on_pushButton_16_clicked();

    void on_pushButton_78_clicked();

    void on_pushButton_81_clicked();

    void on_pushButton_82_clicked();

    void on_pushButton_80_clicked();

    void on_pushButton_83_clicked();

    void on_pushButton_84_clicked();

    void on_pushButton_85_clicked();

    void on_pushButton_79_clicked();

    void on_pushButton_89_clicked();

    void on_pushButton_90_clicked();

    void on_pushButton_86_clicked();

    void on_pushButton_87_clicked();

    void on_pushButton_91_clicked();

    void on_pushButton_92_clicked();

    void on_pushButton_88_clicked();

    void on_pushButton_96_clicked();

    void on_pushButton_97_clicked();

    void on_pushButton_93_clicked();

    void on_pushButton_95_clicked();

    void on_pushButton_98_clicked();

    void on_pushButton_99_clicked();

    void on_pushButton_94_clicked();

    void on_pushButton_100_clicked();

    void on_pushButton_106_clicked();

    void on_pushButton_107_clicked();

    void on_pushButton_101_clicked();

    void on_pushButton_102_clicked();

    void on_pushButton_104_clicked();

    void on_pushButton_103_clicked();

    void on_pushButton_105_clicked();

    void on_pushButton_109_clicked();

    void on_pushButton_110_clicked();

    void on_pushButton_108_clicked();

    void on_pushButton_112_clicked();

    void on_pushButton_111_clicked();

    void on_pushButton_113_clicked();

    void on_pushButton_115_clicked();

    void on_pushButton_116_clicked();

    void on_pushButton_114_clicked();

private:
    Ui::MainWindow *ui;

    QFileSystemModel *fichier_import = new QFileSystemModel(this);

    QStringList resultat_liste_cpt;

    int import_verif;

    int nbre_pro;

    int nbre_pfl;

    int nbre_cpt_iso;

    bool verif_retrait;

    bool verif_retour;

    bool retour_pfl;

    bool verif_occas;

    bool retour_pro;

    int deku;

    QString index_a_retirer;

    QString sortie_nouv_liste_cpt;

    QString pros_nom;

    QString pros_prenom;

    QString num_cpt_pro;

    QStringList numero_ajout_pro;

    QString pfls_nom;

    QString num_cpt_pfl;

    QStringList numero_ajout_pfl;

    QStringList global_pro;  

    QString liste_cpt_cpt;

    QString nouv_list_cpt;

    QString cpt_a_retirer;

    QString test_cpt;

    QVariant num_cpt_cpt;

    QVariant num_profess;

    QVariant num_profil;

    QVariant num_spin_cpt_pro;

    QVariant num_spin_cpt_pfl;

    QVariant num_cpt_update;

    QVariant num_pro_del;

    QVariant num_pfl_del;

    QStringList bled;

    QString cpt_retrait;

    int intervalle_spin;

    int intervalle_spin_pro;

    int intervalle_spin_pfl;

    int intervalle_spin_cpt_pro;

    int intervalle_spin_cpt_pfl;

    int intervalle_spin_del_pro;

    int intervalle_spin_del_pfl;

};
#endif // MAINWINDOW_H
