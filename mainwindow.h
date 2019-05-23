#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>
#include <QString>
#include <QDebug>
#include <QMessageBox>
#include <QDebug>
#include <QRandomGenerator>

#include "defaultparameters.h"
#include "filemanagment.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_PrepareAll_clicked();

    void on_horizontalSlider_synergy_actionTriggered();

    //extra slots
    void processOutput();
    void dataReadyOutput();

    void on_pushButton_findDeck_clicked();

    void on_pushButton_updateMyCollection_clicked();
    void processImportOutput();

private:
    //variables
    QString deckFinderPath;
    //parameters
    QString maxNumberOfCards;
    QString statsResource;
    QString minMatchValidRatio;
    int tupleNumber;
    QString numOfGenerations;
    QString limOfGenerationsWithoutImprovment;
    QString amountOfPopulation;
    QString probabilityOfMutation;
    QString randomElementsInPopulation;
    double p;
    //GUI
    QStringList synergyLevels;
    QStringList deckTypes;
    //process
    QProcess *process;

    //methods
    void prepare_GUI();
    bool is_data_ready();
    double random_p();
    void PrepareAll(std::string, int, int);

    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
