#include <iostream>
#include <vector>

using namespace std;

// Tamaños de los registros
const int SIZE_A = 93;
const int SIZE_B = 84;
const int SIZE_C = 111;

// Inicialización de los registros
vector<int> A(SIZE_A, 0);
vector<int> B(SIZE_B, 0);
vector<int> C(SIZE_C, 0);

// Función para inicializar Trivium
void initializeTrivium(const vector<int>& key, const vector<int>& iv) {
    for (int i = 0; i < 80; ++i) {
        A[i] = key[i];
        B[i] = iv[i];
    }
    B[SIZE_B - 1] = 1;
    C[SIZE_C - 1] = C[SIZE_C - 2] = C[SIZE_C - 3] = 1;
}

// Genera el próximo bit del keystream
int nextKeystreamBit() {
    int t1 = A[SIZE_A - 1] ^ (A[SIZE_A - 15] & A[SIZE_A - 16]) ^ B[SIZE_B - 66];
    int t2 = B[SIZE_B - 1] ^ (B[SIZE_B - 36] & B[SIZE_B - 37]) ^ C[SIZE_C - 69];
    int t3 = C[SIZE_C - 1] ^ (C[SIZE_C - 64] & C[SIZE_C - 65]) ^ A[SIZE_A - 78];

    // Desplazamiento de registros
    rotate(A.begin(), A.end() - 1, A.end());
    rotate(B.begin(), B.end() - 1, B.end());
    rotate(C.begin(), C.end() - 1, C.end());

    A[0] = t3;
    B[0] = t1;
    C[0] = t2;

    return t1 ^ t2 ^ t3;
}

int main() {
    vector<int> key(80, 1); // Clave de 80 bits (ejemplo)
    vector<int> iv(80, 0);  // Vector de inicialización de 80 bits (ejemplo)

    initializeTrivium(key, iv); // Inicializa Trivium

    // Genera los primeros 20 bits del keystream
    cout << "Keystream generado por Trivium:" << endl;
    for (int i = 0; i < 20; ++i) {
        cout << nextKeystreamBit();
    }
    cout << endl;

    return 0;
}
