#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Proceso {
    char nombre;
    int llegada;
    int cpu;
    int instante;
    int t_fin;
    int t_e;
    int t_r;
    double penalizacion;
};

// Función de comparación para qsort
int compararProcesos(const void *a, const void *b) {
    struct Proceso *procesoA = (struct Proceso *)a;
    struct Proceso *procesoB = (struct Proceso *)b;

    if (procesoA->nombre < procesoB->nombre) {
        return -1;
    } else if (procesoA->nombre > procesoB->nombre) {
        return 1;
    } else {
        return 0;
    }
}

int main() {
    printf("+------------------------------------------------------+\n");
    printf("| 		El proceso mas corto		       |\n");
    printf("+------------------------------------------------------+\n");

    int num_procesos;
    printf("Ingrese el numero de procesos: ");
    scanf("%d", &num_procesos);

    struct Proceso procesos[num_procesos];

    for (int i = 0; i < num_procesos; i++) {
        printf("Ingrese la hora de llegada del proceso %d: ", i + 1);
        scanf("%d", &procesos[i].llegada);
        printf("Ingrese la duracion del proceso %d: ", i + 1);
        scanf("%d", &procesos[i].cpu);
        procesos[i].nombre = 'A' + i;
    }

    int tiempo_actual = 0;
    double penalizacion_total = 0;

    printf("+---------+---------+-----+----------+-------+------+-----------------------+\n");
    printf("| proceso | llegada | cpu | instante | t.fin | t.e  | t.r | penalizacion |\n");
    printf("+---------+---------+-----+----------+-------+------+-----------------------+\n");

    // Crear un array de cadenas de caracteres para almacenar las filas de valores
    char filas_por_nombre[num_procesos][100];  // Suponemos un máximo de 100 caracteres por fila

    while (1) {
        int proceso_ejecutar = -1;
        int menor_tiempo_cpu = -1;

        for (int i = 0; i < num_procesos; i++) {
            if (procesos[i].llegada <= tiempo_actual && procesos[i].cpu > 0) {
                if (proceso_ejecutar == -1 || procesos[i].cpu < menor_tiempo_cpu) {
                    proceso_ejecutar = i;
                    menor_tiempo_cpu = procesos[i].cpu;
                }
            }
        }

        if (proceso_ejecutar == -1) {
            break;
        }

        struct Proceso *p = &procesos[proceso_ejecutar];
        p->instante = tiempo_actual;
        p->t_fin = p->instante + p->cpu;
        p->t_e = p->instante - p->llegada;
        p->t_r = p->t_e + p->cpu;

        if (p->t_e == 0) {
            p->penalizacion = 0;
        } else {
            p->penalizacion = (double)p->t_r / p->t_e;
        }

        penalizacion_total += p->penalizacion;
        tiempo_actual = p->t_fin;

        // Crear la fila de valores para este proceso
        snprintf(filas_por_nombre[proceso_ejecutar], sizeof(filas_por_nombre[proceso_ejecutar]),
                 "|    %c    |    %2d   |  %2d |    %2d    |  %3d  |  %2d  |  %2d  |    %.8f   |",
                 p->nombre, p->llegada, p->cpu, p->instante, p->t_fin, p->t_e, p->t_r, p->penalizacion);

        p->cpu = 0;
    }

    // Ordenar las filas de valores por nombre de proceso
    for (int i = 0; i < num_procesos; i++) {
        for (int j = i + 1; j < num_procesos; j++) {
            if (filas_por_nombre[i][4] > filas_por_nombre[j][4]) {
                char temp[100];
                strcpy(temp, filas_por_nombre[i]);
                strcpy(filas_por_nombre[i], filas_por_nombre[j]);
                strcpy(filas_por_nombre[j], temp);
            }
        }
    }

    // Imprimir las filas de valores ordenadas
    for (int i = 0; i < num_procesos; i++) {
        printf("%s\n", filas_por_nombre[i]);
    }

    printf("+---------+---------+-----+----------+-------+------+-----------------------+\n");

    double promedio_penalizacion = penalizacion_total / num_procesos;
    printf("Promedio de penalizacion: %.8f\n", promedio_penalizacion);

    return 0;
}

//IG:rodrigoruiz_11
