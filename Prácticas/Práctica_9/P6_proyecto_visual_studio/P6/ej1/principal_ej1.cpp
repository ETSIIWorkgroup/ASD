//----------------------------------------------------
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Dpto ATC. www.atc.us.es 
//  ASD subject 3º GTI
//  - Understanding basic concepts for OpenMP
//-----------------------------------------------------

using namespace std;

#include <iostream>
#include <omp.h>

int main() {
    
    // Crear el cerrojo:
    omp_lock_t cerrojo;
    omp_init_lock(&cerrojo);

    // Especificar el nº de hilos:
    omp_set_num_threads(4);

    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;

    #pragma omp parallel
    {
        cout << "Hello world!. Soy el hilo " << omp_get_thread_num() << " de un total de " << omp_get_num_threads() << endl;
    }

    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;

    #pragma omp parallel
    {
        omp_set_lock(&cerrojo);
        cout << "Hello world!. Soy el hilo " << omp_get_thread_num() << " de un total de " << omp_get_num_threads() << endl;
        omp_unset_lock(&cerrojo);
    }
    omp_destroy_lock(&cerrojo);

    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;

    #pragma omp parallel
    {
        #pragma omp critical
        {
            cout << "Hello world!. Soy el hilo " << omp_get_thread_num() << " de un total de " << omp_get_num_threads() << endl;
        }
    }

    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;


}
