#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QStandardItemModel>
#include <QTableWidgetItem>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QStandardItemModel *model = new QStandardItemModel();
    model->setColumnCount(5);


    model->setHorizontalHeaderItem(0, new QStandardItem(QString("PID")));
    model->setHorizontalHeaderItem(1, new QStandardItem(QString("Usuário")));
    model->setHorizontalHeaderItem(2, new QStandardItem(QString("Num da CPU")));
    model->setHorizontalHeaderItem(3, new QStandardItem(QString("% de CPU")));
    model->setHorizontalHeaderItem(4, new QStandardItem(QString("% de Memória")));

    ui->tableViewProcessos->setModel(model);
    ui->tableViewProcessos->verticalHeader()->setVisible(true);
    ui->tableViewProcessos->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //ui->tableViewProcessos->setSelectionMode(QAbstractItemView::SingleSelection);

    /* Adicionando linha */
    QList<QStandardItem *> linha;

    linha.append(new QStandardItem(QString("12234")));
    linha.append(new QStandardItem(QString("denis")));
    linha.append(new QStandardItem(QString("0")));
    linha.append(new QStandardItem(QString("5")));
    linha.append(new QStandardItem(QString("6")));

    model->insertRow(model->rowCount(), linha);





}

MainWindow::~MainWindow()
{
    delete ui;
}
