#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
//#include <QWebEngineView>
#include <QProcess>
#include <QString>
#include <QDebug>
#include <QMessageBox>
#include <QDebug>

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

    void on_horizontalSlider_synergy_actionTriggered(int action);

    //extra slots
    void processOutput();
    void dataReadyOutput();

    void on_horizontalSlider_deckType_actionTriggered(int action);

    void on_pushButton_findDeck_clicked();

    void on_pushButton_useCardsFile_clicked();

    void on_pushButton_moreOptions_clicked();

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
    double getDeckTypeValue(int v);
    bool is_data_ready();

    Ui::MainWindow *ui;
};


#endif // MAINWINDOW_H
