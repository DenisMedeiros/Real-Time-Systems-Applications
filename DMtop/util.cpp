#include "util.h"
#include <QProcess>
#include <QDebug>
#include <QString>
#include <iostream>

#include <signal.h> // definição dos sinais de interrupções
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h> // system()
#include <sched.h>


int pararProcesso(int pid)
{
    return kill(pid, SIGSTOP);
}



int continuarProcesso(int pid)
{
    return kill(pid, SIGCONT);
}



int matarProcesso(int pid)
{
    /* Retornar 0 significa que deu certo. */
    return kill(pid, SIGKILL);
}


int alterarAfinidade(int pid, int cpu)
{   
    int numCPUs = obterNumCPUs();
    if(cpu <= (numCPUs-1)){
        cpu_set_t my_set;        
        CPU_ZERO(&my_set); 
        CPU_SET(cpu, &my_set); 
        return sched_setaffinity(pid, sizeof(cpu_set_t), &my_set);
    }
    return -2;
}



QList<Processo> getTodosProcessos()
{
    QList<Processo> lista;

    QProcess process;
    process.start("ps --no-headers -xao \"pid:1,uname,stat,psr,pcpu,pmem,command\"");
    process.waitForFinished(-1); // will wait forever until finished

    QString saida = process.readAllStandardOutput();

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

int obterNumCPUs()
{
    return sysconf(_SC_NPROCESSORS_ONLN);
}

Processo getInfoProcesso(int pid)
{
    Processo p;
    QProcess process;
    QString comando = "ps --no-headers -o \"pid:1,uname,stat,psr,pcpu,pmem,command\" -p " + QString::number(pid);
    process.start(comando);
    process.waitForFinished(-1); // will wait forever until finished

    QString saida = process.readAllStandardOutput();
    QString saidaS = saida.simplified();

    QStringList partes = saidaS.split(" ");

    if(partes.size() >= 7)
    {
        p.pid = partes[0].toInt();
        p.usuario = partes[1];
        p.status = partes[2];
        p.cpu = partes[3].toInt();
        p.cpuP = partes[4].toFloat();
        p.memP = partes[5].toFloat();
        p.comando = partes[6];
    }

    return p;

}
