#ifndef HASHTABLE_LINKEDLIST_HPP
#define HASHTABLE_LINKEDLIST_HPP

#include <iostream>
#include <functional>

//struktura pojedynczego węzła (elementu listy)
template <typename K, typename V>
struct Node {
    K key;
    V value;
    Node* next;

    Node(K k, V v) : key(k), value(v), next(nullptr) {}
};

// Pojedyncza lista jednokierunkowa
template <typename K, typename V>
class SinglyLinkedList {
private:
    Node<K, V>* head;
public:
    SinglyLinkedList(); //konstruktor
    ~SinglyLinkedList(); //destruktor

    void insert(K key, V value); //wstawiamy nowy węzęl na początek
    bool remove(K key); //usuwamy wszystkie węzły o danym klucze
};

//konstruktor
template <typename K, typename V>
SinglyLinkedList<K, V>::SinglyLinkedList() : head(nullptr) {}

//destruktor
template <typename K, typename V>
SinglyLinkedList<K, V>::~SinglyLinkedList() {
    Node<K, V>* current = head;
    while (current != nullptr) {
        Node<K, V>* nextNode = current->next;
        delete current;
        current = nextNode;
    }
}

//wstawiamy nowy węzęl na początek
template <typename K, typename V>
void SinglyLinkedList<K, V>::insert(K key, V value) {
    Node<K, V>* newNode = new Node<K, V>(key, value);
    newNode->next = head;
    head = newNode;
}

//usuwamy wszystkie węzły o danym klucze
template <typename K, typename V>
bool SinglyLinkedList<K, V>::remove(K key) {
    bool removed = false; //flaga za pomocą której sprawdzamy usunięcie elementu 

    //jeśli key wskazuję na head - zmieniamy head i usuwamy head
    while (head != nullptr && head->key == key) {
        Node<K, V>* toDelete = head;
        head = head->next;
        delete toDelete;
        removed = true;
    }

    //szukanie elementu o wartości key w środku listy
    Node<K, V>* current = head;
    while (current != nullptr && current->next != nullptr) {
        //znależliśmy element o odpowiednim klucze - zmieniamy wskażniki i usuwamy 
        if (current->next->key == key) {
            Node<K, V>* toDelete = current->next;
            current->next = current->next->next;
            delete toDelete;
            removed = true;
        } else {
            current = current->next;
        }
    }

    return removed; //zwracamy true jeśli element został usunięty 
}

//Hash table z użyciem listy jednokierunkowej 
template <typename K, typename V>
class HashTable {
private:
    int size; //ilość list wewnątrz tablicy
    SinglyLinkedList<K, V>* table; //tablica list o rozmiarze size

    int hash(K key); //wybiera odpowiedni dla elementy o kluczu key indeks w tablicy table
public:
    HashTable(int sz); //konstruktor
    ~HashTable(); //destruktor

    void insert(K key, V value); //wstawienie par w table
    bool remove(K key); //usuwanie par o kluczu key w listach wewnątrz tablicy
};

//konstruktor
template <typename K, typename V>
HashTable<K, V>::HashTable(int sz) {
    size = sz;
    table = new SinglyLinkedList<K, V>[size];
}

//destruktor
template <typename K, typename V>
HashTable<K, V>::~HashTable() {
    delete[] table;
}

//wybiera odpowiedni dla elementy o kluczu key indeks w tablicy table
template <typename K, typename V>
int HashTable<K, V>::hash(K key) {
    return (key % size + size) % size;
}

//wstawienie par w table
template <typename K, typename V>
void HashTable<K, V>::insert(K key, V value) {
    //obliczmy indeks na który trzeba ustawić dany węzęl
    int index = hash(key);
    //tworzymy link do mejsca o odpowiedznim indeksie w tablice 
    SinglyLinkedList<K, V>& bucket = table[index];
    //ustawiamy węzel na odpowiedni indeks za pomocą metody insert w oparciu na link do miejsca w tablice
    bucket.insert(key, value);
}

//usuwanie par o kluczu key w listach wewnątrz tablicy
template <typename K, typename V>
bool HashTable<K, V>::remove(K key) {
    //obliczmy indeks na którym trzeba usunąć dany węzęl
    int index = hash(key);
    bool erased = table[index].remove(key);
    return erased;
}

#endif // HASHTABLE_LINKEDLIST_HPP
