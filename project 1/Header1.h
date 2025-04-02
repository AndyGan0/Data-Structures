#pragma once
#include <iostream>
using namespace std;




//Decraring class Chain to make class ChainNode aware that there will be definition of Chain later 
template <typename T>
class Chain;





//Defining Class ChainNode
template <typename T>
class ChainNode {
    friend Chain<T>;
private:
    T data; // node data
    ChainNode<T>* link; //pointer to the next node
};







template <typename T>
class Chain {
private:
    ChainNode<T>* first;    //  pointer to the first node

public:

    Chain() { first = 0; }
    ~Chain();
    bool IsEmpty() const { return (first == 0); }
    int Length() const;
    bool Find(int k, T& x) const;
    int Search(const T& x) const;
    Chain<T>& Delete(int k, T& x);
    Chain<T>& DeleteAllOccurrence(T x);
    Chain<T>& Insert(int k, const T& x);
    Chain<T>& InsertSorted(const T& x);
    void Output(ostream& out) const;

};




/*
 *   Methods
 */




template<typename T>
Chain<T>::~Chain() {

    //  Chain destructor. Delete all nodes in chain
    ChainNode<T>* next; // next node
    while (first) {
        next = first->link;
        delete first;
        first = next;
    }
}




template<typename T>
int Chain<T>::Length() const {
    // Return the number of elements in the chain
    ChainNode<T>* current = first;
    int len = 0;
    while (current) {
        len++;
        current = current->link;
    }
    return len;
}




template<typename T>
bool Chain<T>::Find(int k, T& x) const {
    // Set x to the k'th element in the chain
    // Return false if no k'th; return true otherwise
    if (k < 1) return false;
    ChainNode<T>* current = first;
    int index = 1;  //  index of current
    while (index < k && current) {
        current = current->link;
        index++;
    }
    if (current) {
        x = current->data; return true;
    }
    return false; // no k'th element
}




template<typename T>
int Chain<T>::Search(const T& x) const {
    // Locate x. Return position of x if found
    // Return 0 if x not in the chain
    ChainNode<T>* current = first;
    int index = 1;  // index of current
    while (current && current->data != x) {
        current = current->link;
        index++;
    }
    if (current) return index;
    return 0;
}




template<typename T>
Chain<T>& Chain<T>::Delete(int k, T& x) {
    //  Set x to the k'th element and delete it
    //  Throw OutOfBounds exception if no k'th element
    if (k < 1 || !first) throw out_of_range("List Out of Bounds"); // no k'th
    //  p will eventually point to k'th node
    ChainNode<T>* p = first;
    //  move p to k'th & remove from chain
    if (k == 1)     //p already at k'th
        first = first->link;    // remove
    else {  //  Use q to get to k-1'th
        ChainNode<T>* q = first;
        for (int index = 1; index < k - 1 && q; index++) {
            q = q->link;
        }
        if (!q || !q->link) throw out_of_range("List Out of Bounds");  // no k'th
        p = q->link; q->link = p->link; // remove k'th
    }
    //  Save k'th element and free node p 
    x = p->data; delete p; return *this;  
}




template<typename T>
Chain<T>& Chain<T>::DeleteAllOccurrence(T x) {
    //  Deletes every node where data is equal with x

    //  While first node is x, first node will be deleted
    ChainNode<T>* p;
    while (first) {
        if (first->data == x) {
            p = first;
            first = first->link;
            delete p;
        }
        else { break; }       
    }

    
    if (first){ //  if list isnt empty and first node isnt x

        //  p will show every node after the first, one after the other
        p = first;
        ChainNode<T>* current;
        while (p->link != NULL) {
            if (p->link->data == x) {
                //  If data of p is equal to x, the node will be deleted
                current = p->link;
                p->link = current->link;
                delete current; 
            }
            else {
                //  If data of p isnt x, we will move p to next node
                p = p->link;
            }
        }
    }

    return *this;
}




template<typename T>
Chain<T>& Chain<T>::Insert(int k, const T& x) {
    // Insert x after the k'th element
    // Throw OutOfBounds exception if no k'th element
    // Pass NoMem exception if inadequate space

    if (k < 0) throw out_of_range("List Out of Bounds");
    //  p will eventually point to k'th node
    ChainNode<T>* p = first;
    for (int index = 1; index < k && p; index++)
        p = p->link; // move p to k'th element
    if (k > 0 && !p) throw out_of_range("List Out of Bounds"); //  no k'th
    //  Insert
    ChainNode<T>* y = new ChainNode<T>; y->data = x;
    if (k) {
        y->link = p->link; p->link = y;
    }
    else {
        y->link = first; first = y;
    }
    return *this;
}




template<typename T>
Chain<T>& Chain<T>::InsertSorted(const T& x) {
    //  If a list is sorted with ascending order then InsertSorted will add a node with data x to the list in a way that the list will still be sorted
    //  SHOULD ONLY BE USED BY ALREADY SORTED LISTS WITH ASCENDING ORDER, USING THIS FOR NON-SORTED LISTS IS POINTLESS


    if (!first || first->data > x) {

        //  In case there is no first node, or if x should be added before first node
        //  New node is created as first node with data x
        ChainNode<T>* y = new ChainNode<T>;
        y->data = x;
        y->link = first;
        first = y;

    }
    else {

        //  Finds the node whose next node is >=x. This node will be in the pointer p
        //  If the end of the list is reached then p will be last node of the list
        ChainNode<T>* p = first;
        while (p->link && p->link->data < x) {
            p = p->link;
        }


        //  If next node from p is x then x already exists in list. No need to add node with data x
        if (p->link && p->link->data == x) {

            return *this;

        }


        //  Adding node with data x after node p
        ChainNode<T>* y = new ChainNode<T>;
        y->data = x;
        y->link = p->link;
        p->link = y;

    }

    return *this;

}




template<typename T>
void Chain<T>::Output(ostream& out) const {
    //  Insert the Chain elements into the stream out
    ChainNode<T>* current;
    out << "[ ";
    for (current = first; current; current = current->link) {
        out << current->data << " ";
    }
    out << "]\n";
}

// overload <<
template<class T>
ostream& operator<< (ostream& out, const Chain<T>& x)
{
    x.Output(out);
    return out;
}