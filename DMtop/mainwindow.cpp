#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTableWidgetItem>
#include "util.h"
#include <QTimer>

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
    ui->tableWidgetProcessos->verticalHeader()->setVisible(true);
    ui->tableWidgetProcessos->setSelectionMode(QAbstractItemView::SingleSelection);

    atualizarLista();

    /* Conectando sinais e slots. */
    connect(timer, SIGNAL(timeout()), this, SLOT(atualizarLista()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::atualizarLista()
{
    QList<Processo> processos = getTodosProcessos();
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
