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
    timer->start(5000);

    /* Criando as colunas da tabela. */

    QStringList titulos;
    titulos << "PID" << "Usuário" << "Status" << "Num da CPU" << "% de CPU" << "% de Memória" << "Comando";
    ui->tableWidgetProcessos->setHorizontalHeaderLabels(titulos);

    /* Alterando outras configurações da tabela. */
    ui->tableWidgetProcessos->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidgetProcessos->verticalHeader()->setVisible(false);
    //ui->tableWidgetProcessos->horizontalHeader()->setDisabled(true);
    //ui->tableWidgetProcessos->horizontalHeader()->setSelectionMode(QAbstractItemView::NoSelection);
    ui->tableWidgetProcessos->setSortingEnabled(true);

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
    connect(timer, SIGNAL(timeout()), this, SLOT(atualizarLista()));
    connect(ui->tableWidgetProcessos, SIGNAL(cellClicked(int,int)), this, SLOT(selecionarCelula(int,int)));
    connect(ui->pushButtonFiltrar, SIGNAL(released()), this, SLOT(filtrarProcessos()));
    connect(ui->pushButtonSair, SIGNAL(released()),this,SLOT(matarProcessos()));
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
    ui->tableWidgetProcessos->setRowCount(processos.size());

    for(int i = 0; i < processos.size(); i++)
    {
        Processo p = processos[i];
        /* Adicionando uma nova linha; */
        ui->tableWidgetProcessos->setItem(i, 0, new QTableWidgetItem(QString::number(p.pid)));
        ui->tableWidgetProcessos->setItem(i, 1, new QTableWidgetItem(p.usuario));
        ui->tableWidgetProcessos->setItem(i, 2, new QTableWidgetItem(p.status));
        ui->tableWidgetProcessos->setItem(i, 3, new QTableWidgetItem(QString::number(p.cpu)));
        ui->tableWidgetProcessos->setItem(i, 4, new QTableWidgetItem(QString::number(p.cpuP)));
        ui->tableWidgetProcessos->setItem(i, 5, new QTableWidgetItem(QString::number(p.memP)));
        ui->tableWidgetProcessos->setItem(i, 6, new QTableWidgetItem(p.comando));
    }

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
    QString nomeProcesso = ui->lineEditNomeProcesso->text();

    /* Busca processo pelo nome na lista de processos. */
    for(int i = 0; i < processos.size(); i++)
    {
        if(processos[i].comando.contains(nomeProcesso))
        {
            QTableWidgetItem *item = ui->tableWidgetProcessos->item(i, 0);
            QString pidText = item->text();
            QString saida = QString("Processo com pid " + pidText);

        }

    }
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
