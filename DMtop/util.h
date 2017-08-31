#ifndef UTIL_H
#define UTIL_H

#include <QList>

/* Lista das funções para interagir com o SO. */

struct Processo {
    int pid;
    QString usuario;
    QString status;
    int cpu;
    float cpuP; // Entre 0.0 e 100.0
    float memP; // Entre 0.0 e 100.0
    QString comando;
};


int pararProcesso(int pid);
int continuarProcesso(int pid);
int matarProcesso(int pid);
int alterarAfinidade(int pid, int cpu);
int obterNumCPUs();
QList<Processo> getTodosProcessos(void);

#endif // UTIL_H

