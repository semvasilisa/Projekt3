#include "hashTableOpenAddressing.h"
#include <iostream>

//struktura komórki - konstruktor domyślny
Entry::Entry() {
    is_deleted = false;
    is_empty = true;
}

//konstruktor dla ustawionych wartości
Entry::Entry(int k, int v) {
    key = k;
    value = v;
    is_deleted = false;
    is_empty = false;
}

//hast table konstruktor
HashTable::HashTable(int setSize) {
    capacity = setSize;
    used = 0;
    size = 0;
    table = new Entry[capacity];
}

//hast table destruktor
HashTable::~HashTable() {
    delete[] table;
}

//wybiera odpowiedni dla elementy o kluczu key indeks w tablicy table
int HashTable::hash(int key) {
    return (key % capacity + capacity) % capacity;
}

//szuka odpowiedzni index dla elementu jeśli komórka początkowa była zajęta
int HashTable::probeIndex(size_t hash, int i) {
    return (hash + i) % capacity; //i - numer próbki szukania wolnej komórki
}

//metoda która zwiększa rozmiar tablicy i rehaszuje pary
void HashTable::rehash() {
    //zapisujemy poprzedni stan tablicy
    int oldCapacity = capacity;
    Entry* oldTable = table;//wskażnik na poprzednią tablicę

    //rozszerzamy tablicę
    capacity = 2 * oldCapacity;
    table = new Entry[capacity];//teraz table wskazuję na zwiększoną tablicę
    //wyzerowanie liczników
    size = 0;
    used = 0;

    //wstawiamy pary do zwiększonej tablicy
    for (int i = 0;i < oldCapacity;++i) {
        //metoda insert działa z tablicą table, wywołując tą metodę dodajemy elementy z OldTable do zwiększonej tablicy
        if (!oldTable[i].is_empty && !oldTable[i].is_deleted) {
            insert(oldTable[i].key, oldTable[i].value);
        }
    }
    delete[] oldTable; //usunięcie poprzedniej tablicy
}

//metoda dodawania nowych par do komurek
void HashTable::insert(int key, int value) {
    //sprawdzamy wypełnienie tablicy
    if (double(used + 1) / capacity > loadFactor) rehash();

    //obliczenie indeksu odpowiedniej komórki
    int index = hash(key);
    int firstDeletedIndex = -1;

    //szukamy wolne miejsce dla ustwienia pary
    for (int i = 0; i < capacity; ++i) {
        //liniejne próbkowanie???
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
            //(2.1)jeśli wcześniej znaleźliśmy usuniętą komórkę —> używamy ją zamiast pustej
            if (firstDeletedIndex != -1) {
                table[firstDeletedIndex] = Entry(key, value);
                size++;
                return;
            }
            else {
                //(2.2)nie znaleźliśmy usuniętej komórki, używamy pustej
                table[newIndex] = Entry(key, value);
                size++;
                used++;
                return;
            }
        }

    }

    //(3.1)nie znaleźliśmy pustej komórki,ale mogliśmy znaleźć usuniętą
    if (firstDeletedIndex != -1) {
        table[firstDeletedIndex] = Entry(key, value);
        size++;
    }
    else {
        //(3.2)wszystkie komórki są zajęte, nie powinniśmy tutaj dotrzeć dzięki rehash()
        //na wszelki wypadek możemy rzucić wyjątek lub wykonać rehash
        rehash();
        insert(key, value); //rekurencyjne wywołanie po rehash()
    }
}

//metoda usuwania wszystkich par o podanym kluczu
void HashTable::remove(int key) {
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
