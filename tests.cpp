#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <fstream>
#include <string>
#include <unordered_set>
#include <sstream>

#include "HashTableAVLTree.h"
#include "hashTableLinkedList.h"
#include "hashTableOpenaddressing.h"

using namespace std;
using namespace std::chrono;

const vector<int> sizes = {5000, 8000, 10000, 16000, 20000, 40000, 60000, 100000};
const int trials = 100;

std::mt19937 rng(std::random_device{}());
std::uniform_int_distribution<int> dist(0, 1000);

void benchmarkHashTableAVLTree(ofstream& out) {
    out << "HashTableAVLTree insert:\nRozmiar,Czas\n";
    cout << "HashTableAVLTree insert:\nRozmiar,Czas\n";
    for (int size : sizes) {
        double avgInsertTime = 0;

        for (int t = 0; t < trials; ++t) {
            HashTableAVLTree<int, int> table(size);
            
            for (int i = 0; i < size; ++i) {
                table.insert(i, i * 10);
            }

            auto start = high_resolution_clock::now();
            table.insert(dist(rng), dist(rng));
            auto end = high_resolution_clock::now();
            avgInsertTime += duration_cast<nanoseconds>(end - start).count();
        }

        double avg = avgInsertTime / trials;
        out << size << "," << avg << "\n";
        cout << size << "," << avg << "\n";
    }

    out << "\nHashTableAVLTree remove:\nRozmiar,Czas\n";
    cout << "\nHashTableAVLTree remove:\nRozmiar,Czas\n";
    for (int size : sizes) {
        double avgRemoveTime = 0;

        for (int t = 0; t < trials; ++t) {
            HashTableAVLTree<int, int> table(size);

            for (int i = 0; i < size; ++i) {
                table.insert(i, i * 10);
            }

            auto start = high_resolution_clock::now();
            table.remove(dist(rng) % size);
            auto end = high_resolution_clock::now();
            avgRemoveTime += duration_cast<nanoseconds>(end - start).count();
        }

        double avg = avgRemoveTime / trials;
        out << size << "," << avg << "\n";
        cout << size << "," << avg << "\n";
    }
}

void benchmarkOpenAddressingHashTable(ofstream& out) {
    out << "\nHashTable OpenAddressing insert:\nRozmiar,Czas\n";
    cout << "\nHashTable OpenAddressing insert:\nRozmiar,Czas\n";
    for (int size : sizes) {
        double avgInsertTime = 0;

        for (int t = 0; t < trials; ++t) {
            HashTableOA<int, int> table(size);
            
            for (int i = 0; i < size; ++i) {
                table.insert(i, i * 10);
            }

            auto start = high_resolution_clock::now();
            table.insert(dist(rng), dist(rng));
            auto end = high_resolution_clock::now();
            avgInsertTime += duration_cast<nanoseconds>(end - start).count();
        }

        double avg = avgInsertTime / trials;
        out << size << "," << avg << "\n";
        cout << size << "," << avg << "\n";
    }

    out << "\nHashTable OpenAddressing remove:\nRozmiar,Czas\n";
    cout << "\nHashTable OpenAddressing remove:\nRozmiar,Czas\n";
    for (int size : sizes) {
        double avgRemoveTime = 0;

        for (int t = 0; t < trials; ++t) {
            HashTableOA<int, int> table(size);
            
            for (int i = 0; i < size; ++i) {
                table.insert(i, i * 10);
            }

            auto start = high_resolution_clock::now();
            table.remove(dist(rng) % size);
            auto end = high_resolution_clock::now();
            avgRemoveTime += duration_cast<nanoseconds>(end - start).count();
        }

        double avg = avgRemoveTime / trials;
        out << size << "," << avg << "\n";
        cout << size << "," << avg << "\n";
    }
}

void benchmarkLinkedListHashTable(ofstream& out) {
    out << "\nHashTable LinkedList insert:\nRozmiar,Czas\n";
    cout << "\nHashTable LinkedList insert:\nRozmiar,Czas\n";
    for (int size : sizes) {
        double avgInsertTime = 0;

        for (int t = 0; t < trials; ++t) {
            HashTableLL<int, int> table(size);

            for (int i = 0; i < size; ++i) {
                table.insert(i, i * 10);
            }

            auto start = high_resolution_clock::now();
            table.insert(dist(rng), dist(rng));
            auto end = high_resolution_clock::now();
            avgInsertTime += duration_cast<nanoseconds>(end - start).count();
        }

        double avg = avgInsertTime / trials;
        out << size << "," << avg << "\n";
        cout << size << "," << avg << "\n";
    }

    out << "\nHashTable LinkedList remove:\nRozmiar,Czas\n";
    cout << "\nHashTable LinkedList remove:\nRozmiar,Czas\n";
    for (int size : sizes) {
        double avgRemoveTime = 0;

        for (int t = 0; t < trials; ++t) {
            HashTableLL<int, int> table(size);
            
            for (int i = 0; i < size; ++i) {
                table.insert(i, i * 10);
            }

            auto start = high_resolution_clock::now();
            table.remove(dist(rng) % size);
            auto end = high_resolution_clock::now();
            avgRemoveTime += duration_cast<nanoseconds>(end - start).count();
        }

        double avg = avgRemoveTime / trials;
        out << size << "," << avg << "\n";
        cout << size << "," << avg << "\n";
    }
}


int main() {
    ofstream file("results.csv");
    if (!file.is_open()) {
        cerr << "Nie można otworzyć pliku.\n";
        return 1;
    }

    benchmarkOpenAddressingHashTable(file);
    benchmarkLinkedListHashTable(file);
    benchmarkHashTableAVLTree(file);

    file.close();
    cout << "Wyniki zapisane w results.csv\n";
    return 0;
}
