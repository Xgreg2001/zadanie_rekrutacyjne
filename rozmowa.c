/**
 * 1. Zaprojektuj i zaimplementuj funkcje get_combinations_le_100, ktora zlicza 2 elementowe kombinacje liczb
 *      takie ze ich suma jest <= od 100
 *
 *      Funkcja przyjmuje jako argument tablice liczb unsigned. Ich wartosci moga byc dowolne.
 *      Jako kombinacje liczb uznajemy dowolne 2 liczby z 2 roznych miejsc w tablicy,
 *      Pamietaj ze kombinacja to zbior nieuporzadkowany, zatem {a, b} i {b, a} to ta sama kombinacja
 *
 *      Przyklad:
 *      unsigned t[] = {0, 2, 0, 99};
 *
 *      mamy nastepujace kombinacje (0 i 0 to dwie rozne liczby, bo ich indeksy sa rozne)
 *
 *      0 + 2, t[0] + t[1]
 *      0 + 0, t[0] + t[2]
 *      0 + 99, t[0] + t[3]
 *      2 + 0, t[1] + t[2]
 *      0 + 99, t[2] + t[3]
 *
 *      Zatem wynik to 5.
 *
 * 2. Napisz testy jednostkowe
 *
 * 3. Wyznacz zlozonosc asymptotyczna swojego rozwiazania w notacji duze O.
 *
 * Pokaz swoj styl programowania, pochwal sie znajomoscia dobrych metodyk oraz ficzerow jezyka C.
 *      Zadbaj o to aby Twoj kod byl bezpieczny, czytelny i szybki (wez pod uwage aspekty algorytmiczne jak i ficzery nowych cpu)
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>

size_t get_combinations_le_100(const unsigned t[], size_t t_len);

size_t get_combinations_le_100(const unsigned t[const], const size_t t_len)
{
    /* Zaimplementuj funkcje */

    if(t == NULL || t_len == 0){
        return 0;
    }

    unsigned count = 0;

    enum {hist_size = 101};

    unsigned hist[hist_size] = {0};

    for (size_t i = 0; i < t_len; i++){
        if (t[i] <= 100){
            hist[t[i]]++;
        }
    }

    // 0 0 0 0 0 (n - 1)!

    for(size_t i = 0; i < hist_size; i++){
        if (i <= ((hist_size - 1) / 2) && hist[i] != 0){
            count += hist[i]*(hist[i] - 1) / 2;
        }

        // 111 222 hist[1] = 3, hist[2] = 3

        for(size_t j = i + 1; j < hist_size - i; j++){
            if (i + j <= 100) {
                count += hist[i] * hist[j];
            }
        }
    }

    return count;
}

int main(void)
{
    /* Przyklad */
    const size_t no_combinations0 = get_combinations_le_100((unsigned[]){0, 2, 0, 99}, 4); /* should be 5 */

    const size_t no_combinations1 = get_combinations_le_100((unsigned[]){UINT_MAX, 2, 0, 99, 5}, 5); /* should be 4 */

    const size_t no_combinations2 = get_combinations_le_100(NULL, 0); /* should be 0 */

    const size_t no_combinations3 = get_combinations_le_100((unsigned[]){25}, 1); /* should be 0 */

    const size_t no_combinations4 = get_combinations_le_100((unsigned[]){75, 75}, 2); /* should be 0 */

    /* Dopisz testy */
    assert(no_combinations0 == 5);
    assert(no_combinations1 == 4);
    assert(no_combinations2 == 0);
    assert(no_combinations3 == 0);
    assert(no_combinations4 == 0);

    return 0;
}
