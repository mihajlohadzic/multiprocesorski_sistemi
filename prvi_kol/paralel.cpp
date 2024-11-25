#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>

//const int N = 100;
const int N = 560;
//const int N = 1024;

std::vector<std::vector<int>> A(N, std::vector<int>(N, 1));
std::vector<std::vector<int>> B(N, std::vector<int>(N, 2));
std::vector<std::vector<int>> C(N, std::vector<int>(N, 0));

std::vector<std::vector<bool>> ready(N, std::vector<bool>(N, false)); 
std::mutex mtx;
std::condition_variable cv;

        
void multiplyMatBlock(int startCol, int endCol) {
  for (int col = startCol; col < endCol; ++col){
    for (int i = 0; i < N; ++i) {
        if (i > 0) {
            std::unique_lock<std::mutex> lock(mtx);
            cv.wait(lock, [i, col]() { return ready[i - 1][col]; });
        }

        
        for (int k = 0; k < N; ++k) {
            C[i][col] += A[i][k] * B[k][col];
        }
        
        {
            std::lock_guard<std::mutex> lock(mtx);
            ready[i][col] = true;
        }
        cv.notify_all();
    }
    }
}

int main() {
    const int numThreads = 12;
    std::vector<std::thread> threads;

    //pocetak merenja vremena
    auto start = std::chrono::high_resolution_clock::now();

    //niti za paralelnu obradu kolona
  
    int colsPerThread = (N + numThreads - 1) / numThreads; 
    for (int t = 0; t < numThreads; ++t) {
          int startCol = t * colsPerThread;
          int endCol = std::min(startCol + colsPerThread, N);
          threads.emplace_back(multiplyMatBlock, startCol, endCol);
    }

    //sacekaj da sve niti zavrse posao
    for (size_t i = 0; i < threads.size(); ++i){
        threads[i].join();
    }

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

