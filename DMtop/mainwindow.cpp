#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTableWidgetItem>
#include <QTimer>
#include <QDebug>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QTimer *timer = new QTimer(this);
    timer->start(2000);

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
    connect(timer, SIGNAL(timeout()), this, SLOT(timeout()));
    connect(ui->tableWidgetProcessos, SIGNAL(cellClicked(int,int)), this, SLOT(selecionarCelula(int,int)));
    connect(ui->pushButtonFiltrar, SIGNAL(released()), this, SLOT(filtrarProcessos()));
    connect(ui->pushButtonMatar, SIGNAL(released()),this,SLOT(matarProcessos()));
    connect(ui->pushButtonAlterarCPU, SIGNAL(released()), this, SLOT(alterarCPU()));


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

void MainWindow::timeout()
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
