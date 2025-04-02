#include <iostream>
#include "BinarySearchTreeClass.h"

int main() {

    BSTree<int,int> mytree;
    
    
    mytree.Insert(8);
    mytree.Insert(4);
    mytree.Insert(12);
    mytree.Insert(2);
    mytree.Insert(10);
    mytree.Insert(6);
    mytree.Insert(14);
    mytree.Insert(1);
    mytree.Insert(9);
    mytree.Insert(3);
    mytree.Insert(11);
    mytree.Insert(5);
    mytree.Insert(13);
    mytree.Insert(7);
    mytree.Insert(15);

    mytree.Print_Tree();


    cout << "\n";
    int i;
    mytree.Delete(4, i);  
    mytree.Delete(2, i);
    mytree.Delete(6, i);
    mytree.Delete(3, i);
    mytree.Delete(1, i);
    

    

    mytree.Print_Tree();
    
}
