#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QList>
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

private slots:
    void atualizarLista();
    void selecionarCelula(int l, int c);
    void filtrarProcessos();
};

#endif // MAINWINDOW_H
