#include "util.h"
#include <QProcess>
#include <QDebug>
#include <QString>

void pararProcesso(int pid)
{
    // TODO
}



void continuarProcesso(int pid)
{
    // TODO
}



void matarProcesso(int pid)
{
    // TODO
}


void alterarCPU(int pid, int cpu)
{
    // TODO
}

QList<Processo> getTodosProcessos()
{
    QList<Processo> lista;

    QProcess process;
    process.start("ps --no-headers -xao \"pid:1,uname,stat,psr,pcpu,pmem,command\"");
    process.waitForFinished(-1); // will wait forever until finished

    QString saida = process.readAllStandardOutput();
    QString erro = process.readAllStandardError();

    QStringList linhas = saida.split("\n");
    foreach(QString linha, linhas)
    {
        QString linhaS = linha.simplified();
        QStringList partes = linhaS.split(" "); // Remove excesso de espaços em branco.

        if(partes.size() >= 7)
        {

            Processo p;
            p.pid = partes[0].toInt();
            p.usuario = partes[1];
            p.status = partes[2];
            p.cpu = partes[3].toInt();
            p.cpuP = partes[4].toFloat();
            p.memP = partes[5].toFloat();
            p.comando = partes[6];

            lista.append(p);
        }

    }

    return lista;
}
