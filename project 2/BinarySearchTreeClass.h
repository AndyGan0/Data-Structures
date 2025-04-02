#pragma once
#include <iostream>
#include <queue>
using namespace std;








//Decraring classes
template <class T>
class BinaryTree;

template <class E, class K>
class BSTree;








//Defining Class BinaryTreeNode
template <typename T>
class BinaryTreeNode {
public:

    BinaryTreeNode() { deleted = false; LeftChild = RightChild = 0; }
    BinaryTreeNode(const T& e) { deleted = false; data = e; LeftChild = RightChild = 0; }
    BinaryTreeNode(const T& e, BinaryTreeNode * l, BinaryTreeNode *r) {
        deleted = false; 
        data = e;
        LeftChild = l;
        RightChild = r;
    }


    friend BinaryTree<T>;
    friend BSTree<T,T>;

private:

    bool deleted;
    T data;
    BinaryTreeNode<T> *LeftChild,   //  Left subtree
                      *RightChild;  //  Right subtree

};








template <class T>
class BinaryTree {
private:

    BinaryTreeNode<T>* root;    //  pointer to the root

    BinaryTreeNode<T>* makeEmpty(BinaryTreeNode<T>* t) {

        if (t == NULL)
            return NULL;
        {
            makeEmpty(t->LeftChild);
            makeEmpty(t->RightChild);
            delete t;
        }
        return NULL;
    }
    
    

public:

    
    BinaryTree() { root = 0; };

    ~BinaryTree() { root = makeEmpty(root); 
    };

    bool IsEmpty() const { return ((root) ? false : true); }


    friend BSTree<T, T>;


};








template <class E, class K>
class BSTree : private BinaryTree<E>{
private:

    //  methods used by reconstruct
    BSTree<E, K>& reconstruct(BinaryTreeNode<E>* t);
    int countNonDeletedNodes(BinaryTreeNode<E>* t);
    void makeArray(int* array, BinaryTreeNode<E>* t, int &i);
    BinaryTreeNode<E>& reconstruct2(int* array, int start, int end);

    //  methods used to check if we need reconstruct *AFTER DELETING*
    BinaryTreeNode<E>* CallCheck4EachNode( BinaryTreeNode<E>* k);
    int countDeletedNodes_Helper(BinaryTreeNode<E>* t, int& AllNodes, int& NodesMarkedDeleted) const;

    //  Methods Used by Find_Between
    void Find_Between_Helper(BinaryTreeNode<E>* root, int k1, int k2);


    //  methods used to check if we need reconstruct *AFTER INSERTING*
    int numberOfNodes(BinaryTreeNode<E>* node);
    bool isBalanced(BinaryTreeNode<E>* node);
    void check4Balance(BinaryTreeNode<E>* k);
    int Height(BinaryTreeNode<E>* node);


    //  Methods Used by Print
    int countDigitsLeft(BinaryTreeNode<E>* root, int x);
    int countAllDigits(BinaryTreeNode<E>* root);

public:
    
    bool Search(const K& k, E& e) const;
    BSTree<E, K>& Insert(const E& e);
    BSTree<E, K>& Delete(const K& k, E& e);
    void Find_Between(int k1, int k2) { Find_Between_Helper(this->root, k1, k2); }
    void Print_Tree();
    int FindMin();
    int FindMax();


};


















/*
 * ------------------------------------------------------------------------------------------------------------------------
 *   METHODS
 * ------------------------------------------------------------------------------------------------------------------------
 */



template<typename E, typename K>
bool BSTree<E,K>::Search(const K& k, E &e) const {
    //  Search for element that matches k.
    //  Pointer p starts at the root and moves through
    //  the tree looking for an element with key k

    BinaryTreeNode<E>* p = this->root;
    while (p) {   
        //examine p->data
        if (k < p->data) p = p->LeftChild;
        else if (k > p->data) p = p->RightChild;
        else {
            
            if (!(p->deleted)) {
                //  found element and its not deleted
                //  item with data p exists only if variable deleted is false
                e = p->data;
                return true;
            }
            else return false;

            
        }

    }
    return false;
}





template<typename E, typename K>
BSTree<E,K>& BSTree<E,K>::Insert(const E& e) {
    //  Insert e if not duplicate.
    BinaryTreeNode<E>* p = this->root,    //  search pointer
                     * pp = 0;      //  parent of p
    //  find place to insert
    while (p)
    {
        pp = p;
        //  move p to a child
        if (e < p->data) p = p->LeftChild;
        else if (e > p->data) p = p->RightChild;
        else { 
            //   if the item exists already, we make sure to make variable deleted false
            p->deleted = false; 
            return *this; 
        }  
    }

    //  get a node for e and attach to pp
    BinaryTreeNode<E>* r = new BinaryTreeNode<E>(e);
    if (this->root) {
        //  tree not empty
        if (e < pp->data) pp->LeftChild = r;
        else pp->RightChild = r;

        check4Balance(r);

    }
    else // insertion into empty tree
        this->root = r;

    return *this;
    
}





template<typename E, typename K>
BSTree<E,K>& BSTree<E,K>::Delete(const K& k, E& e) {
    //  Delete element with key k and put it in e
    
    //  Set p to point to node with key k
    BinaryTreeNode<E>* p = this->root,    //  search pointer
                     * pp = 0;      //  parent of p
    while (p && p->data != k) {
        //  Move to a child of p
        pp = p;
        if (k < p->data) p = p->LeftChild;
        else p = p->RightChild;
    }

    if (!p) throw "Bad Input";   //  no element with key k
    e = p->data;    //  save element to delete




    //  restructure tree
    //  handle case when p has two children 
    if (p->LeftChild && p->RightChild) {
        //  two children 
        //  p will be marked as deleted
        p->deleted = true;  

        //BinaryTreeNode<E>* temp = this->CallCheck4EachNode(this->root, p);
        BinaryTreeNode<E>* temp = this->CallCheck4EachNode(p);
        if (temp) {
            check4Balance(temp);
        }
        

        return *this;
    }
    
    //  p has at most one child
    //  save child pointer in c
    BinaryTreeNode<E>* c;
    if (p->LeftChild) c = p->LeftChild;
    else c = p->RightChild;


    //  delete p
    if (p == this->root) this->root = c;
    else {
        //  is p Left or Right child of pp?
        if (p == pp->LeftChild)
            pp->LeftChild = c;
        else pp->RightChild = c;
    }

    delete p;



    //BinaryTreeNode<E>* temp = this->CallCheck4EachNode(this->root, pp);
    BinaryTreeNode<E>* temp = this->CallCheck4EachNode(pp);
    if (temp) {
        check4Balance(temp);
    }

    return *this;

}





template<typename E, typename K>
void BSTree<E, K>::Find_Between_Helper(BinaryTreeNode<E>* root, int k1, int k2) {
    //  Prints all items in [k1,k2]

    
    if (root == NULL)
        return;

    
    if (root->data > k1)
        Find_Between_Helper(root->LeftChild, k1, k2);

    
    if (k1 <= root->data && k2 >= root->data)
        if (!(root->deleted)) {
            cout << root->data << " ";
        }
        

    
    if (k2 > root->data)
        Find_Between_Helper(root->RightChild, k1, k2);

}





template<typename E, typename K>
int BSTree<E, K>::FindMin() {

    BinaryTreeNode<E>* t = this->root;
    while (t->LeftChild) {
        t = t->LeftChild;
    }
    return t->data;

}





template<typename E, typename K>
int BSTree<E, K>::FindMax() {

    BinaryTreeNode<E>* t = this->root;
    while (t->RightChild) {
        t = t->RightChild;
    }
    return t->data;

}















/*
 * ------------------------------------------------------------------------------------------------------------------------
 *   CHECK IF MORE THAN 50% OF NODES ARE DELETED IN A SUBTREE
 *   USED AFTER NODE IS DELETED
 *   Checks if any subtree needs reconstruction
 *   If needed, it calls reconstruct method
 * ------------------------------------------------------------------------------------------------------------------------
 */



template<typename E, typename K>
BinaryTreeNode<E>* BSTree<E, K>::CallCheck4EachNode( BinaryTreeNode<E>* t) {
    //  This method is used to check if any node needs reconstruction and calls the reconstruction method if needed (After deleting)
    //  node t is the node marked as deleted or the parent of the deleted node
    //  Returns the last node checked for reconstruction (whether reconstruction happened or not)
    //-------------------------------------------------------------------------

    //  Note:
    // 
    //  Once a node is marked as deleted, only the subtrees that include this node have a chance
    //  to have more than 50% of the nodes marked as deleted!!!
    // 
    //  Those subtrees, are all the subtrees, which have as a root any nodes along the path root-deleted node
    //  So, we check only the subtrees along the path of root-deleted node
    //-------------------------------------------------------------------------



    //  starting from the root
    BinaryTreeNode<E>* p = this->root;

    while (p) {

        
        //  Considering the subtree with p as root
        //  Counting all nodes inside of it and nodes marked as deleted
        int AllNodes = 0;
        int NodesMarkedDeleted = 0;
        int height = countDeletedNodes_Helper(p, AllNodes, NodesMarkedDeleted);  //  this function counts all nodes and nodes marked deleted
        //  countDeletedNodes_Helper also returns the height of the subtree


        if (height < 3) {
            //  if height of subtree is less than 3 returns the node
            //  height in the next subtrees will be even lower, no need to keep checking
            //  reconstruction wont happen
            return p;
        }

         



        //  Checking the current node
        if (NodesMarkedDeleted > 0.5 * AllNodes) {

            //  If deleted nodes inside its subtree are more than 50%, then reconstruct and return
            reconstruct(p);
            return p;

        }




        //  We need to go to the next node of the path root-deleted node
        if (p == t) {
            //  If node p is the deleted node then return, no need to continue
            //  All nodes along the path root-deleted node have been checked
            return p;
        }
        else if (p->data > t->data) {
            //  If data inside p is bigger than data inside t, then t is on the left of p bcs we are in a binary search tree
            //  Moving to the next node of the path root-deleted node

            p = p->LeftChild;
        }
        else if (p->data < t->data) {

            p = p->RightChild;

        }

    }



    return p;


}



template<typename E, typename K>
int BSTree<E, K>::countDeletedNodes_Helper(BinaryTreeNode<E>* t, int& AllNodes, int& NodesMarkedDeleted) const {
    //  Counts All Nodes and Deleted Nodes of a subtree with root the node t
    //  The number of all nodes will be put in AllNodes 
    //  number of deleted Nodes that still exist inside the subtree will be put in as NodesMarkedDeleted
    //  returns height of the subtree
    //  BEFORE CALLING THIS METHOD, AllNodes AND NodesMarkedDeleted MUST BE 0

    if (t) {

        //  Counting nodes in right and left child
        int heightUnder = countDeletedNodes_Helper(t->LeftChild, AllNodes, NodesMarkedDeleted);
        int temp = countDeletedNodes_Helper(t->RightChild, AllNodes, NodesMarkedDeleted);
        
        //  finding the height of the highest subtree between the left and right subtree
        if (temp > heightUnder) heightUnder = temp;


        //  Counting the current node
        AllNodes++;
        if (t->deleted) NodesMarkedDeleted++;

        return heightUnder + 1; // returning the height of the subtree with t as root

    }
    else {
        return 0;
    }

}














/*
 * ------------------------------------------------------------------------------------------------------------------------
 *  BALANCE
 *  USED AFTER NODE IS INSERTED
 *  Checks if any subtree needs reconstruction
 *  If needed, it calls reconstruct method
 * ------------------------------------------------------------------------------------------------------------------------
 */



template <typename E, typename K>
void BSTree<E,K>::check4Balance(BinaryTreeNode<E>* k) {
    //  When a node is inserted, This method checks all nodes that are needed to be checked for balance
    //  The nodes that are needed to be checked are those along the path root-inserted node 
    //  Note: node k isnt checked for balance because it was just inserted and doesn't have children
    

    BinaryTreeNode<E>* p = this->root;
    while (p && p != k) {

        if (Height(p) < 3) {
            return;
        }


        if (isBalanced(p) == false) {
            reconstruct(p);
            return;
        }

        if (k->data < p->data) p = p->LeftChild;
        else if (k->data > p->data) p = p->RightChild;

    }

}



template <typename E, typename K>
bool BSTree<E,K>::isBalanced(BinaryTreeNode<E>* node) {
    //  Checks if a subtree of a node is balanced
    //  Returns true if it is balanced. Returns false otherwise.

    int l = this->numberOfNodes(node->LeftChild);
    int r = this->numberOfNodes(node->RightChild);

    if (r >= 2 * l || l >= 2 * r) {
        return false;
    }

    return true;
}



template <typename E, typename K>
int BSTree<E,K>::numberOfNodes(BinaryTreeNode<E>* node) {
    //  Gets a node and inside the subtree with this node as root, 
    //  it counts the number of all nodes (deleted and non-deleted)
    //  Returns the number of all nodes

    if (node == NULL) {
        return 0;
    }

    return numberOfNodes(node->LeftChild) + numberOfNodes(node->RightChild) + 1;

}



template <typename E, typename K>
int BSTree<E, K>::Height(BinaryTreeNode<E>* node) {

    if (node == NULL) return 0;

    return max(Height(node->LeftChild), Height(node->RightChild)) + 1;

}














/*
 * ------------------------------------------------------------------------------------------------------------------------
 *   PRINT METHOD + methods used by print method
 * ------------------------------------------------------------------------------------------------------------------------
 */



int print_digit(int x) {
    //  prints all digits of a number and returns number of digits
    //  This function is used by the method Print_Tree


    int count = 1;

    if (x >= 10)
        count += print_digit(x / 10);

    int digit = x % 10;

    std::cout << ' ' << digit << ' ';

    return count;
}



template<typename E, typename K>
void BSTree<E, K>::Print_Tree() {
    //  Prints Tree

    cout << "\n";

    if (this->root == NULL) {
        cout << "The Tree is empty \n";
        return;
    }



    // Q holds all items in current level
    queue<BinaryTreeNode<E>*> Q;
    Q.push(this->root);

    //  CurrentLevelItemsLeft holds the number items in the current level that are left for printing
    //  Used to check when we go to next level
    int CurrentLevelItemsLeft = Q.size();

    //  Used to count how many digits have been printed from current level
    int PrintedItemsinLevel = 0;


    //  LevelLength is the length of each horizontal line that we print
    int LevelLength = countAllDigits(this->root);
    


    //  Level order traversal of the tree
    while (!Q.empty()) {

        //  Putting the current node in t
        BinaryTreeNode<E>* t = Q.front();



        //  Count how many digits all the node nodes of the left child have
        int l = countDigitsLeft(this->root, t->data);



        for (; PrintedItemsinLevel < l; ++PrintedItemsinLevel) {
            cout << " X ";
        }



        //Printing the current item
        if (t->data == 0) {

            if (t->deleted) {
                cout << ' x ';
            }
            else {
                cout << ' ' << 0 << ' ';
            }            
            PrintedItemsinLevel++;
        }
        else {

            if (t->deleted) {

                int temp = t->data;
                int numberOfDigits = 0;
                while (temp != 0) {
                    numberOfDigits++;
                    temp = temp / 10;
                }
                for (int i = 0; i < temp; i++) {
                    cout << ' X ';
                }
                PrintedItemsinLevel = PrintedItemsinLevel + temp;

            }
            else {
                PrintedItemsinLevel += print_digit(t->data);
            }
        }







        //  put t's children on Q
        if (t->LeftChild) Q.push(t->LeftChild);
        if (t->RightChild) Q.push(t->RightChild);

        //  Removing the printed items from
        try {
            Q.pop();
        }
        catch (...) { return; }




        /*
         *   Checking if we need to move to next level (new line)
         */

         //  We printed 1 item, so we have 1 less item to print in the current level
        CurrentLevelItemsLeft--;

        if (CurrentLevelItemsLeft == 0) {

            //  In this case all items of the level have been printed

            //  Filling the remaining with "X "
            for (; PrintedItemsinLevel < LevelLength; ++PrintedItemsinLevel) {
                cout << " X ";
            }

            //  After printing all items for this level, print the next level in new line
            cout << "\n";


            PrintedItemsinLevel = 0;            


            //  CurrentLevelItemsLeft should be reset to reflect the items of the current level
            CurrentLevelItemsLeft = Q.size();

        }

    }

    cout << "\n";

}



template<typename E, typename K>
int BSTree<E, K>::countDigitsLeft(BinaryTreeNode<E>* root, int x) {
    //  Gets a tree node and a value x
    //  Counts the number of digits of all nodes smaller than x
    //  Returns this number
    

    if (root == NULL)
        return 0;

    int count;

    //  Then we don't count the number of digits in right subtree or in the node root
    if (root->data >= x) {
        count = this->countDigitsLeft(root->LeftChild, x);
        return count;
    }

    //  Else we count the number of digits of all nodes in both subtrees
    count = this->countDigitsLeft(root->LeftChild, x) + this->countDigitsLeft(root->RightChild, x);

    //  Then we count number of digits in node root
    if (root->data == 0) return count + 1;

    int currData = root->data;

    while (currData != 0) {

        count++;
        currData = currData / 10;
    }

    return count;

}



template<typename E, typename K>
int BSTree<E, K>::countAllDigits(BinaryTreeNode<E>* root) {
    //  Gets a tree node and a value x
    //  Counts the number of digits of all nodes in a tree
    //  Returns this number

    if (!root) {
        return 0;
    }

    int count = 0;
    int t = root->data;

    //  counting number of digits in t
    if (t == 0) {
        count = 1;
    }
    else {
        while (t != 0) {
            count++;
            t = t / 10;
        }
    }
        

    //  return number of digits in left subtree, right subtree and the current node
    return countAllDigits(root->LeftChild) + countAllDigits(root->RightChild) + count;

}














/*
 * ------------------------------------------------------------------------------------------------------------------------
 *   RECONSTRUCTION METHODS
 * ------------------------------------------------------------------------------------------------------------------------
 */



template<typename E, typename K>
BSTree<E, K>& BSTree<E, K>::reconstruct(BinaryTreeNode<E>* t) {
    //  Gets a node and performs reconstruct

    // count how many nodes in n
    int n = countNonDeletedNodes(t);

    int* nodes = new int[n];
    int i = 0;
    makeArray(nodes,t, i);
    //-------------------------




    if (t == this->root) {
        //  if reconstructing the whole tree


        this->makeEmpty(t);

        this->root = &(reconstruct2(nodes, 0, n - 1));

        delete[] nodes;

        return *this;

    }






    //  if reconstructing subtree
    
    // finding the parent t
    BinaryTreeNode<E>* p = this->root;
    while (p->LeftChild != t && p->RightChild != t) {
        
        if (t->data > p->data)
            p = p->RightChild;
        else
            p = p->LeftChild;
    }
    //  p points to parent of p

    // tIspChild is l if t is LeftChild of p and r if t is RightChild of p
    string tIspChild;
    if (t == p->LeftChild)
        tIspChild = "l";
    else
        tIspChild = "r";


    this->makeEmpty(t);




    if (tIspChild == "l") {
        p->LeftChild = &(reconstruct2(nodes, 0, n - 1));
        t = p->LeftChild;
    }        
    else {
        p->RightChild = &(reconstruct2(nodes, 0, n - 1));
        t = p->RightChild;
    }
        



    delete[] nodes;

    return *this;
}



template<typename E, typename K>
int BSTree<E, K>::countNonDeletedNodes(BinaryTreeNode<E>* t) {
    //  Gets a node t and counts the number of non deleted nodes inside the subtree with t as root

    if (t == NULL)
        return 0;
    else if (t->deleted)
        return countNonDeletedNodes(t->LeftChild) + countNonDeletedNodes(t->RightChild);
    else
        return countNonDeletedNodes(t->LeftChild) + countNonDeletedNodes(t->RightChild) + 1;

}



template<typename E, typename K>
void BSTree<E, K>::makeArray(int* array, BinaryTreeNode<E>* t, int &i) {
    //  Gets an array of integers and a node t. Then it performs in order traversal of 
    //  subtree with node t as root and when it finds an item that isnt deleted this item is added to the array
    //  variable i is used only to add items in the correct spot in the array


    //  InOrder traversal of *t
    if(t) {
                
        makeArray(array,t->LeftChild,i);

        //  if t isnt deleted add its data in array
        if (t->deleted == false) {

            array[i] = t->data;
            i++;

        }
        
        makeArray(array,t->RightChild,i);
    }
        
}



template<typename E, typename K>
BinaryTreeNode<E>& BSTree<E, K>::reconstruct2(int* array,int start, int end) {
    //  making tree for elements in the array from start to end

    BinaryTreeNode<E>* y = new BinaryTreeNode<E>;

    
    if (end - start + 1 >= 3) {
        //  if start to end is 3 or more elements
        //  y will be the node in the middle

        int middle = (end + start) / 2;
        y->data = array[middle];
        y->LeftChild = NULL;
        y->RightChild = NULL;
        y->deleted = false;

        //  calling reconstruct2 for nodes before the one in the middle
        y->LeftChild = &(reconstruct2(array, start, middle - 1));
        //  calling reconstruct2 for nodes after the one in the middle
        y->RightChild = &(reconstruct2(array, middle + 1, end));

    }
    else if (end - start + 1 == 2) {
        //  if start to end is 2 elements
        //  making tree with those 2 nodes

        y->data = array[end];
        y->RightChild = NULL;
        y->deleted = false;
        BinaryTreeNode<E>* x = new BinaryTreeNode<E>;
        y->LeftChild = x;

        x->data = array[start];
        x->deleted = false;
        x->LeftChild = NULL;
        x->RightChild = NULL;
        

    }
    else if (end - start + 1 == 1) {
        //  if start to end is 1 element
        //  making tree with 1 node

        y->data = array[start];
        y->LeftChild = NULL;
        y->RightChild = NULL;
        y->deleted = false;


    }    

    return (*y);

}