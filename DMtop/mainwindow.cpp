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
    connect(timer, SIGNAL(timeout()), this, SLOT(atualizarLista()));
    timer->start(5000);


    /* Criando as colunas da tabela. */
    model = new QStandardItemModel();
    model->setColumnCount(7);

    model->setHorizontalHeaderItem(0, new QStandardItem(QString("PID")));
    model->setHorizontalHeaderItem(1, new QStandardItem(QString("Usuário")));
    model->setHorizontalHeaderItem(2, new QStandardItem(QString("Status")));
    model->setHorizontalHeaderItem(3, new QStandardItem(QString("Num da CPU")));
    model->setHorizontalHeaderItem(4, new QStandardItem(QString("% de CPU")));
    model->setHorizontalHeaderItem(5, new QStandardItem(QString("% de Memória")));
    model->setHorizontalHeaderItem(6, new QStandardItem(QString("Comando")));

    ui->tableViewProcessos->setModel(model);

    /* Alterando outras configurações da tabela. */

    ui->tableViewProcessos->verticalHeader()->setVisible(true);
    ui->tableViewProcessos->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //ui->tableViewProcessos->setSelectionMode(QAbstractItemView::SingleSelection);


    atualizarLista();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::atualizarLista()
{
    model->setRowCount(0);

    QList<Processo> processos = getTodosProcessos();
    foreach(Processo p, processos)
    {
        /* Adicionando uma nova linha na tabela. */
        QList<QStandardItem *> linha;
        linha.append(new QStandardItem(QString::number(p.pid)));
        linha.append(new QStandardItem(p.usuario));
        linha.append(new QStandardItem(p.status));
        linha.append(new QStandardItem(QString::number(p.cpu)));
        linha.append(new QStandardItem(QString::number(p.cpuP)));
        linha.append(new QStandardItem(QString::number(p.memP)));
        linha.append(new QStandardItem(p.comando));
        model->insertRow(model->rowCount(), linha);

    }
}
