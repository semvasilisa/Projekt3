#ifndef HASHTABLE_OPENADDRESSING_H
#define HASHTABLE_OPENADDRESSING_H

#include <iostream>

//struktura komórki
template <typename K, typename V>
struct Entry {
    K key;
    V value;
    bool is_deleted;
    bool is_empty;

    Entry(); //konstruktor domyślny, używamy podczas tworzenia tablicy, kiedy potrzebujemy ciąg pustych komórek
    Entry(K k, V v); //konstruktor dla ustawionych wartości
};

//konstruktor domyślny
template <typename K, typename V>
Entry<K, V>::Entry() {
    is_deleted = false;
    is_empty = true;
}

//konstruktor dla ustawionych wartości
template <typename K, typename V>
Entry<K, V>::Entry(K k, V v) {
    key = k;
    value = v;
    is_deleted = false;
    is_empty = false;
}

template <typename K, typename V>
class HashTableOA {
private:
    int capacity; //iłość komurek
    int size;     //aktywny stan tablicy
    int used;     //ile było użytych komurek(aktywnych+usuniętych)
    Entry<K, V>* table; //tablica list o rozmiarze size
    const double loadFactor = 0.7; //w który moment powinnyśmy zwiększyć rozmiar tablicy

    int hash(K key); //wybiera odpowiedni dla elementy o kluczu key indeks w tablicy table
    int probeIndex(size_t hash, int i); //szuka odpowiedzni index dla elementu jeśli komórka początkowa była zajęta
    void rehash(); //metoda która zwiększa rozmiar tablicy i rehaszuje pary

public:
    HashTableOA(int setSize); //konstruktor
    ~HashTableOA(); //destruktor

    void insert(K key, V value); //metoda dodawania nowych par do komurek
    void remove(K key); //metoda usuwania wszystkich par o podanym kluczu
};

//hast table konstruktor
template <typename K, typename V>
HashTableOA<K, V>::HashTableOA(int setSize) {
    capacity = setSize;
    used = 0;
    size = 0;
    table = new Entry<K, V>[capacity];
}

//hast table destruktor
template <typename K, typename V>
HashTableOA<K, V>::~HashTableOA() {
    delete[] table;
}

//wybiera odpowiedni dla elementy o kluczu key indeks w tablicy table
template <typename K, typename V>
int HashTableOA<K, V>::hash(K key) {
    return (key % capacity + capacity) % capacity;
}

//szuka odpowiedzni index dla elementu jeśli komórka początkowa była zajęta
template <typename K, typename V>
int HashTableOA<K, V>::probeIndex(size_t hash, int i) {
    return (hash + i) % capacity; //i - numer próbki szukania wolnej komórki
}

//metoda która zwiększa rozmiar tablicy i rehaszuje pary
template <typename K, typename V>
void HashTableOA<K, V>::rehash() {
    //zapisujemy poprzedni stan tablicy
    int oldCapacity = capacity;
    Entry<K, V>* oldTable = table; //wskażnik na poprzednią tablicę

    //rozszerzamy tablicę
    capacity = 2 * oldCapacity;
    table = new Entry<K, V>[capacity]; //teraz table wskazuję na zwiększoną tablicę
    //wyzerowanie liczników
    size = 0;
    used = 0;

    //wstawiamy pary do zwiększonej tablicy
    for (int i = 0; i < oldCapacity; ++i) {
        if (!oldTable[i].is_empty && !oldTable[i].is_deleted) {
            insert(oldTable[i].key, oldTable[i].value);
        }
    }

    delete[] oldTable; //usunięcie poprzedniej tablicy
}

//metoda dodawania nowych par do komurek
template <typename K, typename V>
void HashTableOA<K, V>::insert(K key, V value) {
    //sprawdzamy wypełnienie tablicy
    if (double(used + 1) / capacity > loadFactor) rehash();

    //obliczenie indeksu odpowiedniej komórki
    int index = hash(key);
    int firstDeletedIndex = -1;

    //szukamy wolne miejsce dla ustwienia pary
    for (int i = 0; i < capacity; ++i) {
        int newIndex = probeIndex(index, i);

        //(1)jeśli komórka zajęta ale klucz pasuje -> aktualizujemy wartość
        if (!table[newIndex].is_empty && !table[newIndex].is_deleted && table[newIndex].key == key) {
            table[newIndex].value = value;
            return;
        }

        //jeśli komórka została usunięta, zapamiętujemy ją
        if (table[newIndex].is_deleted && firstDeletedIndex == -1) {
            firstDeletedIndex = newIndex;
        }

        //(2)komórka pusta lub oczyszczona -> ustawiamy parę w tą komórkę
        if (table[newIndex].is_empty) {
            if (firstDeletedIndex != -1) {
                table[firstDeletedIndex] = Entry<K, V>(key, value);
                size++;
                return;
            } else {
                table[newIndex] = Entry<K, V>(key, value);
                size++;
                used++;
                return;
            }
        }
    }

    //(3.1)nie znaleźliśmy pustej komórki,ale mogliśmy znaleźć usuniętą
    if (firstDeletedIndex != -1) {
        table[firstDeletedIndex] = Entry<K, V>(key, value);
        size++;
    } else {
        //(3.2)wszystkie komórki są zajęte, nie powinniśmy tutaj dotrzeć dzięki rehash()
        rehash();
        insert(key, value); //rekurencyjne wywołanie po rehash()
    }
}

//metoda usuwania wszystkich par o podanym kluczu
template <typename K, typename V>
void HashTableOA<K, V>::remove(K key) {
    int index = hash(key);
    for (int i = 0; i < capacity; ++i) {
        int idx = probeIndex(index, i);
        if (table[idx].is_empty) return;
        if (!table[idx].is_deleted && table[idx].key == key) {
            table[idx].is_deleted = true;
            size--;
            return;
        }
    }
}

#endif // HashTableOA_OPENADDRESSING_H
