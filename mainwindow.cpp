#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow){
    ui->setupUi(this);

    //prepares parameters
    deckFinderPath = DECK_FINDER_PATH.c_str();
    maxNumberOfCards = QString::number(MAX_NUMBER_OF_CARDS);
    statsResource = STATS_RESOURCE.c_str();
    minMatchValidRatio = QString::number(MIN_MATCH_VALID_RATIO);
    tupleNumber = ui -> horizontalSlider_synergy -> sliderPosition() + 2;
    //p = getDeckTypeValue(ui -> horizontalSlider_deckType ->sliderPosition());
    numOfGenerations = QString::number(NUM_OF_GENERATIONS);
    limOfGenerationsWithoutImprovment = QString::number(LIMIT_OF_GENERATIONS_WITHOUT_IMPROVMENT);
    amountOfPopulation = QString::number(AMOUNT_OF_POPULATION);
    probabilityOfMutation = QString::number(PROBABILITY_OF_MUTATION);
    randomElementsInPopulation = QString::number(RANDOM_ELEMENTS_IN_POPULATION);

    //prepares GUI parameters
    for(string s: SYNERGY_LEVELS)
        synergyLevels.push_back(s.c_str());
    for(string s: DECK_TYPES)
        deckTypes.push_back(s.c_str());

    //process = new QProcess(this);

    prepare_GUI( );
}

MainWindow::~MainWindow(){
    delete ui;
}

bool MainWindow::is_data_ready(){
    bool result = true;
    for( string s : CARD_CLASSES ){
        if(!FileManagment::exists("data/"+s+"_Info.json")){
            qInfo() << ("data/"+s+"_Info.json").c_str();
            result = false;
            break;
        }
    }
    return result;
}

void MainWindow::prepare_GUI(){
    for(std::string s: CARD_CLASSES)
        ui->comboBox_cardClass -> addItem(QString::fromStdString(s));

    ui->checkBox_prepareAll->setEnabled(false);

    if(is_data_ready())
        ui->checkBox_prepareAll->setChecked(true);
}


void MainWindow::on_pushButton_PrepareAll_clicked()
{
    search_buttons_enabled(false);

    ui -> progressBar_findDeck -> setValue(1);

    for(std::string s: CARD_CLASSES){

        ui->textBrowser_results->append("Preparing " + QString(s.c_str()) + "...");
        //Prepare cards
        QString programNamePrepareCards( deckFinderPath + QString("bin/PrepareCards.exe") );
        QStringList argsPrepareCards;
            argsPrepareCards.push_back( QString::number(1) );
            argsPrepareCards.push_back( QString(s.c_str()) );
            if( (ui->checkBox_useCardsFile->isChecked()) )
                argsPrepareCards.push_back( QString::number( 1 ) );
            else
                argsPrepareCards.push_back( QString::number( 0 ) );
        QProcess::execute(programNamePrepareCards,argsPrepareCards);

        //Match Collector
        QString programNameMatchCollector( deckFinderPath + QString("bin/MatchCollector.exe") );
        QStringList argsMatchCollector;
            argsMatchCollector.push_back(QString::number(1));
            argsMatchCollector.push_back( QString(s.c_str()) );
            argsMatchCollector.push_back(maxNumberOfCards);
            argsMatchCollector.push_back(statsResource);
        QProcess::execute(programNameMatchCollector,argsMatchCollector);

        ui -> progressBar_findDeck -> setValue( ui -> progressBar_findDeck->value() + 8 );
        QCoreApplication::processEvents();

        //Prepares parameters to the iterative process
        argsPrepareCards.removeLast();
        argsMatchCollector[2] = minMatchValidRatio;

        for(int c1=2; c1<=MAX_TUPLE_NUMBER; ++c1){
            argsPrepareCards[0] = QString::number(c1);
            argsMatchCollector[0] = QString::number(c1);
            QProcess::execute(programNamePrepareCards,argsPrepareCards);
            QProcess::execute(programNameMatchCollector,argsMatchCollector);
        }
        ui -> progressBar_findDeck -> setValue( ui -> progressBar_findDeck->value() + 3 );
        QCoreApplication::processEvents();
    }

    ui -> progressBar_findDeck -> setValue(100);
    ui->checkBox_prepareAll->setChecked(true);
    search_buttons_enabled(true);
}


void MainWindow::on_horizontalSlider_synergy_actionTriggered()
{
    tupleNumber = ui -> horizontalSlider_synergy -> sliderPosition() + 2;
    ui -> label_synergy -> setText( synergyLevels[ui -> horizontalSlider_synergy->sliderPosition()] );
}

void MainWindow::dataReadyOutput()
{
    int lastV = ui -> progressBar_findDeck -> value();
    ui -> progressBar_findDeck -> setValue(
                lastV + 100/MAX_TUPLE_NUMBER >= 99 ? 100 : lastV +  100 / MAX_TUPLE_NUMBER
                );
}

void MainWindow::on_pushButton_findDeck_clicked()
{
    search_buttons_enabled(false);

    if( ui -> checkBox_prepareAll ->isChecked() ){
        ui->progressBar_findDeck->setValue(2);
        //ui->textBrowser_results->clear();
        p = random_p();
        ui->textBrowser_results->append(ui->label_synergy->text() + " synergy " + ui->comboBox_cardClass->currentText() + " deck with p=" + QString::number(p) + ".");

        process = new QProcess(this);
        QString programName(deckFinderPath + "bin/DeckFinder");
        QStringList args;
            args.push_back(QString::number(tupleNumber));
            args.push_back(ui->comboBox_cardClass->currentText());
            args.push_back(numOfGenerations);
            args.push_back(limOfGenerationsWithoutImprovment);
            args.push_back(amountOfPopulation);
            args.push_back(probabilityOfMutation);
            args.push_back(randomElementsInPopulation);
            args.push_back(QString::number(p));
        connect (process, SIGNAL(readyReadStandardOutput()), this, SLOT(processOutput()));
        process->start(programName,args);
        connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            [=](int, QProcess::ExitStatus){ search_buttons_enabled(true); });
        connect(process, SIGNAL(finished(int,QProcess::ExitStatus)), process, SLOT(deleteLater()));
    }
    else {
        QMessageBox m;
        m.setStyleSheet("background-color: #EEEEEE;");
        m.setText("First prepare the information files.");
        m.exec();
        search_buttons_enabled(true);
    }
}

void MainWindow::processOutput()
{
    QString output(process->readAllStandardOutput());
    if( output.at(0)<='9' && output.at(0)>='0' ){
        int percentage,numgen = output.toInt();
        if(numgen < 400){
            percentage = numgen / 4;
        }
        else {
            percentage = 99;
        }
        ui->progressBar_findDeck->setValue( percentage );
    }
    else{
        ui->textBrowser_results->append(output);
        ui->progressBar_findDeck->setValue(100);
        QStringList fullDeck = output.split('\n');
        deckString = fullDeck[fullDeck.size()-2];
        deckString.chop(1);
    }
}

void MainWindow::on_pushButton_updateMyCollection_clicked()
{
    search_buttons_enabled(false);
    ui->progressBar_findDeck->setValue(50);
    process = new QProcess(this);
    QString programName(deckFinderPath + "bin/My-Hearthstone-Collection");
    connect (process, SIGNAL(readyReadStandardOutput()), this, SLOT(processImportOutput()));
    process->start(programName);
    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
        [=](int, QProcess::ExitStatus){ search_buttons_enabled(true); ui->progressBar_findDeck->setValue(100); });
    connect(process, SIGNAL(finished(int,QProcess::ExitStatus)), process, SLOT(deleteLater()));
}

void MainWindow::processImportOutput()
{
    QString output(process->readAllStandardOutput());
    QMessageBox m;
    m.setWindowTitle("Error!");
    m.setStyleSheet("background-color: #EEEEEE;");
    m.setText(output);
    m.exec();
}

double MainWindow::random_p()
{
    int c = QRandomGenerator::global()->bounded(3);
    switch(c){
        case 0 : return 0.05 + QRandomGenerator::global()->bounded(0.95);
        case 1 : return 1.0 + QRandomGenerator::global()->bounded(1.0);
        case 2 : return 2.0 + QRandomGenerator::global()->bounded(5.0);
        default : return 1.0;
    }
}

void MainWindow::on_pushButton_link_clicked()
{
    QDesktopServices::openUrl(QUrl("https://hs.protolambda.com/" + deckString));
}

void MainWindow::on_pushButton_clipboard_clicked()
{
    QApplication::clipboard()->setText(deckString);
}

void MainWindow::on_pushButton_paypal_clicked()
{
    QDesktopServices::openUrl(QUrl("https://www.paypal.me/edyrol"));
}

void MainWindow::on_actionInstructions_triggered()
{
    QMessageBox m;
    m.setWindowTitle("Instructions");
    m.setStyleSheet("background-color: #EEEEEE;");
    m.setText("This app tries to find good decks which are somewhat similar to the decks played in the current meta. "
              "In order to use this program, follow these steps.\n\n"
              "First decide if you want to limit the results to your collection or not and check the box accordingly.\n\n"
              "In case you want to use your collection, open Hearthstone and press the \"Update My Collection\" button.\n\n"
              "Then push the \"Prepare All\" button to process the data. After this the \"Use My Collection\" box makes no difference.\n\n"
              "Choose a Class and a Synergy level and press the \"Find me a good deck!\" button to start.\n\n"
              "You can copy the deck code to the clipboard to export it to Hearthstone or view the deck online.\n\n"
              "Warning: We expect High Synergy is kind of slow but should give better decks. We expect Low Synergy to produce terrible decks. "
              "Sometimes the deck produced will be garbage, please use your brain before playing.");
    m.exec();
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox m;
    m.setWindowTitle("About");
    m.setStyleSheet("background-color: #EEEEEE;");
    m.setText("This software was developed by a mathematics student and his advisor as part of a MSc project in UNAM. "
              "It looks at the current meta and tries to construct decent decks to play with. "
              "It can restrict the results to the users collection so it may be useful for people who have a limited collection, although it probably won't work if they have few new cards.");
    m.exec();
}

void MainWindow::search_buttons_enabled(bool b)
{
    ui->pushButton_findDeck->setEnabled(b);
    ui->pushButton_updateMyCollection->setEnabled(b);
    ui->pushButton_PrepareAll->setEnabled(b);
}
