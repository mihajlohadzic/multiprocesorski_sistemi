#include <iostream>
#include <vector>
#include <chrono>

//const int N = 100;
const int N = 560;
//const int N = 1024;

std::vector<std::vector<int>> A(N, std::vector<int>(N, 1));
std::vector<std::vector<int>> B(N, std::vector<int>(N, 2));
std::vector<std::vector<int>> C(N, std::vector<int>(N, 0));

void sequentialMultiply() {
    for (int j = 0; j < N; ++j) {
        C[0][j] = 0;
        
        for (int k = 0; k < N; ++k) {
            C[0][j] += A[0][k] * B[k][j];
        }
        
       
        for (int i = 1; i < N; ++i) {
            C[i][j] = 0;
            for (int k = 0; k < N; ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

int main() {
    
    auto start = std::chrono::high_resolution_clock::now();

    
    sequentialMultiply();

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    
    std::cout << "Matrica C:" << std::endl;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            std::cout << C[i][j] << " ";
        }
        std::cout << std::endl;
    }

    std::cout << "Vreme izvrsavanja: " << elapsed.count() << " sekundi" << std::endl;

    return 0;
}

