#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/time.h>

#define swap(a, b) { int _h = a; a = b; b = _h; }
#define min(a, b) ((a) < (b) ? (a) : (b))

void insert_sort(int* left, int* right) {

    // put minimum to left position, so we can save
    // one inner loop comparsion for insert sort
    for (int* pi = left + 1; pi <= right; pi++) {
        if (*pi < *left) {
            swap(*pi, *left);
        }
    }
    for (int* pi = left + 2; pi <= right; pi++) {
        int h = *pi;
        int* pj = pi - 1;
        while (h < *pj) {
            *(pj + 1) = *pj;
            pj -= 1;
        }
        *(pj + 1) = h;
    }
}

#define sort3fast(a, b, c)              \
    if (b < a) {                        \
        if (c < a) {                    \
            if (c < b) { swap(a, c); }  \
            else { int h = a; a = b; b = c; c = h;} \
        }                               \
        else { swap(a, b); }            \
    }                                   \
    else {                              \
        if (c < b) {                    \
            if (c < a) { int h = c; c = b; b = a; a = h;} \
            else { swap(b, c); }        \
        }                               \
    }                                   \

void partition(int* left0, int* right0, int** l1, int** r1, int** l2, int** r2) {

    int* left = left0 + 1;
    int* right = right0;

    int* mid = left0 + (right0 - left0) / 2;    
    int piv = *mid;
    *mid = *left;
    sort3fast(*left0, piv, *right0);
    *left = piv;

    while (1) {
        do left += 1; while(*left < piv);
        do right -= 1; while (*right > piv);
        if (left >= right) break;
        swap(*left, *right);
    }
    *(left0 + 1) = *right;
    *right = piv;

    if (right < mid) {
        *l1 = left0; *r1 = right - 1;
        *l2 = right + 1; *r2 = right0;
    }
    else {
        *l1 = right + 1; *r1 = right0;
        *l2 = left0; *r2 = right - 1;
    }
}


void partition_block(int* left0, int* right0, int** pivot) {

    int* mid = left0 + (right0 - left0) / 2;    
    int piv = *mid;
    *mid = *(left0 + 1);
    sort3fast(*left0, piv, *right0);
    *(left0 + 1) = piv;

    int *left, *right;
    #define BSZ 256
    if (right0 - left0 > 2 * BSZ + 3) {

        left = left0 + 2;
        right = right0 - 1;
        int* offl[BSZ];
        int* offr[BSZ];
        int** ol = offl;
        int** or = offr;
        do {
            if (ol == offl) {
                int* pd = left;
                do {
                    *ol = pd;
                    ol += (piv < *pd);
                    pd += 1;
                }
                while (pd < left + BSZ);
            }
            if (or == offr) {
                int* pd = right;
                do {
                    *or = pd;
                    or += (piv > *pd);
                    pd -= 1;    
                }
                while (pd > right - BSZ);
            }
            int min = min(ol - offl, or - offr);
            ol -= min;
            or -= min;
            for (int i = 0; i < min; i++) {
                swap(**(ol + i), **(or + i));
            }
            if (ol == offl) left += BSZ;
            if (or == offr) right -= BSZ;
        }
        while (right - left > 2 * BSZ);
        left -= 1;
        right += 1;
    }
    else {
        left = left0 + 1;
        right = right0;
    }
    while (1) {
        do left += 1; while(*left < piv);
        do right -= 1; while (*right > piv);
        if (left >= right) break;
        swap(*left, *right);
    }
    *(left0 + 1) = *right;
    *right = piv;
    *pivot = right;
}

void qusort(int* left, int* right) {

    int *piv;
    if (right - left >= 100) {
        partition_block(left, right, &piv);
        qusort(left, piv - 1);
        qusort(piv + 1, right);
    } else {
        insert_sort(left, right);
    }
}


void sort(int* data, int len) {

    qusort(data, data + len - 1);
}

void init(int* data, int len) {

    for (int i = 0; i < len; i++) {
        data[i] = rand();
    }
}

double t(void) {

    static double t0;
    struct timeval tv;
    gettimeofday(&tv, NULL);
    double h = t0;
    t0 = tv.tv_sec + tv.tv_usec / 1000000.0;
    return t0 - h;
}

void test(int* data, int len) {
    for (int i = 1; i < len; i++) {
        if (data[i] < data[i - 1]) {
            printf("ERROR\n");
            break;
        }
    }
}

void print(int* data, int len) {
    for (int i = 0; i < len; i++) {
        printf("%d\n", data[i]);
    }
}

#define SIZE (50 * 1000000)
int data[SIZE];
int copy[SIZE];

static inline int cmp(const void *a, const void *b)
{
    return *(int *)a > *(int *)b;
}

int main(void) {

    init(data, SIZE);
    printf("Sorting %d million numbers with Quicksort ...\n",
        SIZE / 1000000);
    t();
    sort(data, SIZE);
    // qsort(data, SIZE, 4, cmp);
    printf("%.2fs\n", t());
    test(data, SIZE);
    return 0;
}