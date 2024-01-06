//  This is an implementation of a Skip List class, consisting of Linked Lists, which are made up of Nodes.
//  All classes are templated, such that their types may be provided at creation.
//  Nodes contain data (which doubles as key), and pointers to all directions.

#ifndef SkipList_h
#define SkipList_h

#include <iostream>
#include <cstdlib>
#include <stdlib.h>

using namespace std;

template<class T>
class Node {
public:
    Node(const T& data);                            // constructor
    T data;                                         // node data, doubles as key
    Node<T> *next;                                  // node next pointer
    Node<T> *prev;                                  // node previous pointer
    Node<T> *up;                                    // node above pointer
    Node<T> *down;                                  // node below pointer
    void printData();                               // prints data value
    void print();                                   // prints entire node object content
};

template <class T>
class LinkedList {
public:
    LinkedList(T minVal, T maxVal);                 // constructor: accepts -infinity, +infinity values,
                                                    //  creates linked list with two corresponding nodes
    ~LinkedList();                                  // destructor
    Node<T>* search(Node<T>* location, T data);     // search for data starting at location, return node <= data
    Node<T>* insert(Node<T>* location, T data);     // insert node with data at new node to be placed directly 
                                                    //  after node with povided location, return new node
    LinkedList<T> *up;      // added up pointer for LinkedLists
    LinkedList<T> *down;        // added down pointer for LinkedLists
   
    void printData();                               // prints linked list data
    void print();                                   // prints linked list with all nodes pointers
    Node<T>* head;                                  // head of list
};

template <class T>
class SkipList {
public:
    SkipList(T minVal, T maxVal);                   // constructor: accepts -infinity, +infinity values,
                                                    //  creates skip list with top-level only
    ~SkipList();                                    // destructor
    Node<T>* search(T data);                        // search for data, return node <= data (existing node, or location
                                                    //  where new node should be inserted in the bottom-most list)

    Node<T>* insert(T data);                            // insert node with data, return pointer if inserted, NULL if error
    void printData();                               // prints skip list data
    void print();                                   // prints skip list with all nodes pointers
    LinkedList<T> *topList;                         // pointer to the top-most list
    int randSeed = 330;                             // to be used as seed for the getRand() function
};

//returns 0 or 1 with a 50% chance 
//When it returns 1, insert the node to next level of the skiplist
int getRand(){
    return rand()%2;
}

/********************** From here down is the content of the LinkedList.cpp file: ***********************/

/****** Implementation of Node ******/

// Constructor
template<class T>
Node<T>::Node(const T& data)
{
    this->data = data;
    this->next = nullptr;
    this->prev = nullptr;
    this->up = nullptr;
    this->down = nullptr;
}

// Print node's data value
template <class T>
void Node<T>::printData()
{
    cout << data << " ";
}

// Print entire node object content (data value, object's address, and all pointers)
template <class T>
void Node<T>::print()
{
    cout << " * " << data << ": [addr: " << this << " next: " << this->next << " prev: " << this->prev << " up: " << this->up << " down: " << this->down << "]  ";
}


/****** Implementation of linked list ******/

//Constructor
template <class T>
LinkedList<T>::LinkedList(T minVal, T maxVal){
    
    head = new Node<T>(minVal);
    Node<T> *tail = new Node<T>(maxVal);
    head -> next = tail;
    tail -> prev = head;
    this->up = nullptr;
    this->down = nullptr;
}

//Destructor
template <class T>
LinkedList<T>::~LinkedList(){
    
    Node<T> *curr = head;

    while(curr != nullptr){
        Node<T> *nextNode = curr -> next;
        delete curr;
        curr = nextNode;
    }

    head = nullptr;
}

//search - looking for next->data <= data to maintain order. So when conditions of while loop not satisfied
//         either left with node containing data (if exists), or node before where new data should be inserted to maintain order
template <class T>
Node<T>* LinkedList<T>::search(Node<T>* location, T data){
    
    while(location && location -> next && (location -> next -> data <= data)){
        location = location -> next;
    }

    return location;
}

//insert - inserts new node with data 'data', at specified location 'location' if location exists, location->next exists,
//data at location is less than the new data, and the node after the new node's data should be greater than the new data.
template <class T>
Node<T>* LinkedList<T>::insert(Node<T>* location, T data){
    
    if((location) && (location->next) && (location->data < data) && (location->next->data >= data)){
        Node<T> *newNode = new Node<T>(data);
        newNode -> next = location -> next;
        newNode -> prev = location;
        location -> next -> prev = newNode;
        location -> next = newNode;
        return newNode;
    }else{
        return nullptr;
    }
}

//printData - prints data of all nodes in linked list seperated by a space.
template <class T>
void LinkedList<T>::printData(){
    Node<T> *curr = head; //from beginning

    while(curr){ //printing then moving, all the way to INT_MAX from INT_MIN
        curr -> printData(); //includes the space

        curr = curr -> next;
    }
    cout << endl;
}

//print - prints entire contents of linked list including node addresses and pointers
template <class T>
void LinkedList<T>::print(){
    Node<T> *curr = head; //from beginning

    while(curr){
        curr -> print();
        curr = curr -> next;
    }

    cout << endl;
}



/****** Skip List Implementation ******/

// Constructor
template <class T>
SkipList<T>::SkipList(T minVal, T maxVal){
    topList = new LinkedList<T>(minVal, maxVal);
    topList -> down = nullptr;
    topList -> up = nullptr;
    srand(this -> randSeed); // seed the random number generator using the seed initialized in the SkipList class


}

// Destructor
template <class T>
SkipList<T>::~SkipList(){
    LinkedList<T>* currentList = this->topList;
    while(currentList != nullptr){
        LinkedList<T>* temp = currentList -> down;
        currentList -> ~LinkedList();
        currentList = temp;
    }
    cout << "pass" << endl;
}


// Search - returns a pointer to the node if x exists, or a pointer to its predecessor
// in bottom-most list if x doesn't exist
template <class T>
Node<T>* SkipList<T>::search(T data){
    Node<T> *curr = topList -> head; //node to head of the list
    while(true){
        while(curr -> next && curr -> next -> data <= data){ //move right if value is less than data
            curr = curr -> next;
        }
        if(curr -> down == nullptr){ //
            break;
        }
        curr = curr -> down; //catch all case, move down if not at bottom of the list
    }
    return curr;
}


// Insert function
template <class T>
Node<T>* SkipList<T>::insert(T data){

    Node<T>* curr = search(data); //make a pointer equal to the value of the search function
    
    if(curr -> data == data){
        return nullptr;  //if the data is already in the skiplist, return a nullptr

    }

    Node<T>* insertedElement;
    int i = 1;
    //finding heads and tails, treating this as 0/1
    while (true) {
        int result = rand() % 2;
        if (result == 1) {
            i++;
        } else { 
            break;
        }
    }

    //finding height of the tree to figure out how many new lists to add
    int height = 1;
    LinkedList<T>* bottomList = topList; //make a list to 
    while(bottomList -> down != nullptr){
        bottomList = bottomList -> down;
        height++;
    }

    //adding levels dependent upon the counted height of the tree in the previous step
    for (int h = 1; h <= i; h++) {
        if(i > height){
            LinkedList<T>* newList = new LinkedList<T>(INT_MIN,INT_MAX);
            newList -> down = topList;
            topList -> up = newList;
            newList -> up = nullptr;
            topList = newList;
        }
        Node<T>* findPtr = bottomList -> search(bottomList -> head,data); //search for the data value in the list
        bottomList -> insert(findPtr,data); //insert the 
        bottomList = bottomList -> up;
    }
    return insertedElement;
}

//prints data using Linked List print methods
template <class T>
void SkipList<T>::printData(){
    LinkedList<T> *currentLists = topList;
    while(currentLists != nullptr){  //while its not a null pointer, print list data and traverse down
        currentLists -> printData();
        currentLists = currentLists -> down;
    }
}

template <class T>
void SkipList<T>::print(){
    LinkedList<T> *currentLists = topList;
    while(currentLists != nullptr){ //while not a null pointer, print list addresses and traverse down
        currentLists -> print(); 
        currentLists = currentLists -> down;
    }
}

#endif /* SkipList_h */

