#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QList>
#include <QString>

#include "util.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QList<Processo> processos;
    void atualizarLista();
    QString processarStatus(QString status);


private slots:
    void timeout();
    void selecionarCelula(int l, int c);
    void filtrarProcessos();
    void matarProcessos();
    void alterarCPU();
};

#endif // MAINWINDOW_H
