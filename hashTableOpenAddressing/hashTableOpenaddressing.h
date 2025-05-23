#ifndef HASHTABLE_OPENADDRESSING_HPP
#define HASHTABLE_OPENADDRESSING_HPP

//struktura komórki
struct Entry {
    int key;
    int value;
    bool is_deleted;
    bool is_empty;

    Entry();//konstruktor domyślny, używamy podczas tworzenia tablicy, kiedy potrzebujemy ciąg pustych komórek
    Entry(int k, int v);//konstruktor dla ustawionych wartości
};

class HashTable {
private:
    int capacity;//iłość komurek
    int size; //aktywny stan tablicy
    int used; //ile było użytych komurek(aktywnych+usuniętych)
    Entry* table; //tablica list o rozmiarze size
    const double loadFactor = 0.7; //w który moment powinnyśmy zwiększyć rozmiar tablicy

    int hash(int key); //wybiera odpowiedni dla elementy o kluczu key indeks w tablicy table
    int probeIndex(size_t hash, int i); //szuka odpowiedzni index dla elementu jeśli komórka początkowa była zajęta
    void rehash(); //metoda która zwiększa rozmiar tablicy i rehaszuje pary

public:
    HashTable(int setSize); //konstruktor
    ~HashTable(); //destruktor

    void insert(int key, int value); //metoda dodawania nowych par do komurek
    void remove(int key); //metoda usuwania wszystkich par o podanym kluczu
};

#endif 
