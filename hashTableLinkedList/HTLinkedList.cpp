#include "hashTableLinkedList.h"

SinglyLinkedList::SinglyLinkedList() : head(nullptr) {}  //konstruktor

//destruktor
SinglyLinkedList::~SinglyLinkedList() {
    Node* current = head;
    while (current != nullptr) {
        Node* nextNode = current->next;
        delete current;
        current = nextNode;
    }
}

//wstawiamy nowy węzęl na początek
void SinglyLinkedList::insert(int key, int value) {
    Node* newNode = new Node(key, value);
    newNode->next = head;
    head = newNode;
}

//usuwamy wszystkie węzły o danym klucze
bool SinglyLinkedList::remove(int key) {
    bool removed = false;//flaga za pomocą której sprawdzamy usunięcie elementu 

    //jeśli key wskazuję na head - zmieniamy head i usuwamy head
    while (head != nullptr && head->key == key) {
        Node* toDelete = head;
        head = head->next;
        delete toDelete;
        removed = true;
    }

    //szukanie elementu o wartości key w środku listy
    Node* current = head;
    while (current != nullptr && current->next != nullptr) {
        //znależliśmy element o odpowiednim klucze - zmieniamy wskażniki i usuwamy 
        if (current->next->key == key) {
            Node* toDelete = current->next;
            current->next = current->next->next;
            delete toDelete;
            removed = true;
        }
        else {
            current = current->next;
        }
    }
    return removed; //zwracamy true jeśli element został usunięty 
}

//konstruktor
HashTable::HashTable(int sz) {
    size = sz;
    table = new SinglyLinkedList[size];
}

//destruktor
HashTable::~HashTable() {
    delete[] table;
}

//wybiera odpowiedni dla elementy o kluczu key indeks w tablicy table
int HashTable::hash(int key) {
    return (key % size + size) % size;
}

//wstawienie par w table
void HashTable::insert(int key, int value) {
    //obliczmy indeks na który trzeba ustawić dany węzęl
    int index = hash(key);
    //tworzymy link do mejsca o odpowiedznim indeksie w tablice 
    SinglyLinkedList& bucket = table[index];
    //ustawiamy węzel na odpowiedni indeks za pomocą metody insert w oparciu na link do miejsca w tablice
    bucket.insert(key, value);
}

//usuwanie par o kluczu key w listach wewnątrz tablicy
bool HashTable::remove(int key) {
    //obliczmy indeks na którym trzeba usunąć dany węzęl
    int index = hash(key);
    bool erased = table[index].remove(key);
    return erased;
}
