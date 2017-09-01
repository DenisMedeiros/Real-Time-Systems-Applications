#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTableWidgetItem>
#include <QTimer>
#include <QDebug>
#include <QMessageBox>
#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    timerTabela = new QTimer(this);
    timerGrafico = new QTimer(this);

    timerTabela->start(2000);

    /* Criando as colunas da tabela. */

    QStringList titulos;
    titulos << "PID" << "Usuário" << "Status" << "Num da CPU" << "% de CPU" << "% de Memória" << "Comando";
    ui->tableWidgetProcessos->setHorizontalHeaderLabels(titulos);

    /* Alterando outras configurações da tabela. */
    ui->tableWidgetProcessos->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidgetProcessos->verticalHeader()->setVisible(false);
    //ui->tableWidgetProcessos->horizontalHeader()->setDisabled(true);
    //ui->tableWidgetProcessos->horizontalHeader()->setSelectionMode(QAbstractItemView::NoSelection);
    //ui->tableWidgetProcessos->setSortingEnabled(true);

    //ui->tableWidgetProcessos->setSelectionMode(QAbstractItemView::SingleSelection);
    //ui->tableWidgetProcessos->setSelectionMode(QAbstractItemView::MultiSelection);

    /* Preenchendo o combobox com as CPUs. */
    int numCPUs = obterNumCPUs();

    for(int i = 0; i < numCPUs; i++)
    {
        ui->comboBoxCPU->addItem("CPU " + QString::number(i));
    }

    /* Line edit dos PIDs para receber apenas número. */
    ui->lineEditAcaoPID->setValidator(new QIntValidator(1, 65535, this));
    ui->lineEditAlterarCPUPID->setValidator(new QIntValidator(1, 65535, this));

    atualizarLista();

    /* Conectando sinais e slots. */
    connect(timerTabela, SIGNAL(timeout()), this, SLOT(timeoutTabela()));
    connect(ui->tableWidgetProcessos, SIGNAL(cellClicked(int,int)), this, SLOT(selecionarCelula(int,int)));
    connect(ui->pushButtonFiltrar, SIGNAL(released()), this, SLOT(filtrarProcessos()));
    connect(ui->pushButtonMatar, SIGNAL(released()),this,SLOT(matarProcessos()));
    connect(ui->pushButtonAlterarCPU, SIGNAL(released()), this, SLOT(alterarCPU()));
    connect(ui->lineEditNomeProcesso, SIGNAL(textChanged(QString)), this, SLOT(filtrarProcessos())); // Filtra em tempo real
    connect(ui->pushButtonExibir, SIGNAL(released()), this, SLOT(exibirGrafico()));
    connect(timerGrafico, SIGNAL(timeout()), this, SLOT(timeoutGrafico()));


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::atualizarLista()
{
    processos.clear();
    processos = getTodosProcessos();
    ui->tableWidgetProcessos->setRowCount(0);
    QString nomeProcesso = ui->lineEditNomeProcesso->text();
    int count = 0;

    foreach(Processo p, processos)
    {
        if(p.comando.contains(nomeProcesso))
        {
            ui->tableWidgetProcessos->insertRow(ui->tableWidgetProcessos->rowCount());

            /* Adicionando uma nova linha; */
            ui->tableWidgetProcessos->setItem(count, 0, new QTableWidgetItem(QString::number(p.pid)));
            ui->tableWidgetProcessos->setItem(count, 1, new QTableWidgetItem(p.usuario));
            ui->tableWidgetProcessos->setItem(count, 2, new QTableWidgetItem(processarStatus(p.status)));
            ui->tableWidgetProcessos->setItem(count, 3, new QTableWidgetItem(QString::number(p.cpu)));
            ui->tableWidgetProcessos->setItem(count, 4, new QTableWidgetItem(QString::number(p.cpuP)));
            ui->tableWidgetProcessos->setItem(count, 5, new QTableWidgetItem(QString::number(p.memP)));
            ui->tableWidgetProcessos->setItem(count, 6, new QTableWidgetItem(p.comando));


            count++;
        }
    }

    ui->statusBar->showMessage("Monitorando " + QString::number(count) + " processos.");

}

QString MainWindow::processarStatus(QString status)
{
    QString statusSaida;

    if(status.contains("D"))
    {
        statusSaida = "Dormindo (no int)";
    }
    else if (status.contains("R"))
    {
        statusSaida = "Em execução";
    }
    else if (status.contains("S"))
    {
        statusSaida = "Dormindo (int)";
    }
    else if (status.contains("T"))
    {
        statusSaida = "Parado";
    }
    else if (status.contains("W"))
    {
        statusSaida = "Em Paginação";
    }
    else if (status.contains("X"))
    {
        statusSaida = "Morto";
    }
    else if (status.contains("Z"))
    {
        statusSaida = "Zumbi";
    }

    return statusSaida;

}

/* Fica atualizando o gráfico com novos pontos. */
void MainWindow::timeoutGrafico()
{
    Processo p = getInfoProcesso(processoMonitorado);
    tempoMonitorado++;
    chart->axisX()->setRange(0, tempoMonitorado+1);

    seriesCPU->append(tempoMonitorado, p.cpuP);
    seriesMemoria->append(tempoMonitorado, p.memP);

    std::cout << "cpu = " << (float) p.cpuP << ", memoria = " << p.memP << std::endl;
}

void MainWindow::timeoutTabela()
{
    atualizarLista();
}

void MainWindow::selecionarCelula(int l, int c)
{
    c = 0;
    QTableWidgetItem *item = ui->tableWidgetProcessos->item(l, c);
    ui->tableWidgetProcessos->selectRow(l);
    QString pidText = item->text();
    ui->lineEditAcaoPID->setText(pidText);
    ui->lineEditAlterarCPUPID->setText(pidText);
    ui->lineEditGrafico->setText(pidText);
}

void MainWindow::filtrarProcessos()
{
    atualizarLista();
}

void MainWindow::matarProcessos()
{
    QString pidText = ui->lineEditAcaoPID->text();

    if(pidText.trimmed().isEmpty())
    {
      QMessageBox::about(this, "Erro", "Digite ou selecione um PID.");
      return;
    }

    int pid = pidText.toInt();
    if (matarProcesso(pid) == 0)
    {
        atualizarLista();
        return;
    }

    QMessageBox::about(this, "Erro", "O processo " + pidText  + " não pôde ser morto.");
}

void MainWindow::alterarCPU()
{
    QString pidText = ui->lineEditAlterarCPUPID->text();

    if(pidText.trimmed().isEmpty())
    {
      QMessageBox::about(this, "Erro", "Digite ou selecione um PID.");
      return;
    }

    int pid = pidText.toInt();

    QString cpuText = ui->comboBoxCPU->currentText();
    cpuText = cpuText.remove(0,4); // Remove a palavra CPU
    int cpu = cpuText.toInt();

    if(alterarAfinidade(pid, cpu) == 0)
    {
        atualizarLista();
        return;
    }

    QMessageBox::about(this, "Erro", "O processo " + pidText  + " não pôde ter a CPU alterada.");
}

void MainWindow::exibirGrafico()
{
     QString pidText = ui->lineEditAcaoPID->text();
     if(pidText.trimmed().isEmpty())
     {
       QMessageBox::about(this, "Erro", "Digite ou selecione um PID.");
       return;
     }

     processoMonitorado = pidText.toInt();
     tempoMonitorado = 0;

     seriesCPU = new QLineSeries();
     seriesMemoria = new QLineSeries();

     chart = new QChart();
     chart->setTitle("Monitorando processo " + pidText);
     chart->addSeries(seriesCPU);
     chart->addSeries(seriesMemoria);
     chart->createDefaultAxes();
     chart->legend()->hide();

     chart->axisY()->setRange(0, 110);

     chart->axisX()->setGridLineVisible(true);
     chart->axisY()->setGridLineVisible(true);

     chartView = new QChartView(chart);
     chartView->setRenderHint(QPainter::Antialiasing);

     janelaGrafico = new QMainWindow();
     janelaGrafico->resize(400, 400);
     janelaGrafico->setCentralWidget(chartView);
     janelaGrafico->show();

     timerGrafico->start(2000);
}
