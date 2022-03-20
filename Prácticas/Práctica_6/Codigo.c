int a[] = {1,2,3,4,5,6,7,8,9};

double test1(int niter) {
    
    int i;
    double acc = 1.0;

    for(i = 0; i < niter; i++) {

        acc = acc * a[i];

    }

    return acc;
}

int main() {
    test1(100);
}