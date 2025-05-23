#ifndef HASHTABLE_LINKEDLIST_HPP
#define HASHTABLE_LINKEDLIST_HPP

#include <iostream>

//struktura pojedynczego węzła (elementu listy)
struct Node {
    int key;
    int value;
    Node* next;

    Node(int k, int v) : key(k), value(v), next(nullptr) {}
};

// Pojedyncza lista jednokierunkowa
class SinglyLinkedList {
private:
    Node* head;
public:
    SinglyLinkedList();//konstruktor
    ~SinglyLinkedList();//destruktor

    void insert(int key, int value); //wstawiamy nowy węzęl na początek
    bool remove(int key); //usuwamy wszystkie węzły o danym klucze
};

//Hash table z użyciem listy jednokierunkowej 
class HashTable {
private:
    int size; //iłość list wewnątrz tablicy
    SinglyLinkedList* table; //tablica list o rozmiarze size

    int hash(int key); //wybiera odpowiedni dla elementy o kluczu key indeks w tablicy table
public:
    HashTable(int sz); //konstruktor
    ~HashTable(); //destruktor

    void insert(int key, int value); //wstawienie par w table
    bool remove(int key); //usuwanie par o kluczu key w listach wewnątrz tablicy
};

#endif // HASHTABLE_HPP
