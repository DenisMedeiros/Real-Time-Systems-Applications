#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QList>
#include <QString>
#include <QMainWindow>
#include <QtCharts>

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
    int processoMonitorado = -1;
    int tempoMonitorado = -1;

    Ui::MainWindow *ui;
    QList<Processo> processos;
    void atualizarLista();
    QString processarStatus(QString status);
    QTimer *timerTabela;

    // Gráfico da CPU.
    QMainWindow *janelaGrafico;

    QTimer *timerGrafico;
    QChart *chart;
    QChartView *chartView;
    QLineSeries *seriesCPU;
    QLineSeries *seriesMemoria;


private slots:
    void timeoutTabela();
    void timeoutGrafico();
    void selecionarCelula(int l, int c);
    void filtrarProcessos();
    void matarProcessos();
    void alterarCPU();
    void exibirGrafico();
};

#endif // MAINWINDOW_H
