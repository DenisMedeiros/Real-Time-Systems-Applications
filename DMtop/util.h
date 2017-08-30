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


void pararProcesso(int pid);
void continuarProcesso(int pid);
void matarProcesso(int pid);
void alterarCPU(int pid, int cpu);
QList<Processo> getTodosProcessos(void);

#endif // UTIL_H

