#include <iostream>
#include <random>
#include <functional>
#include <cmath>

using namespace std;


#include "Header1.h"






int main()
{


    /*
     *  Random number generator
    */
    std::default_random_engine generator;

    std::uniform_int_distribution<int> list_size_distribution(100, 200);
    std::uniform_int_distribution<int> data_element_distribution (0, 50);

    auto random_list_size = std::bind(list_size_distribution, generator);
    auto random_element = std::bind(data_element_distribution, generator);








    /*
     *  Asking the user for number of lists
    */
    
    int k;
    cout << "Please give the number of lists: ";
    cin >> k;


    //  ensuring user enters non negative integer number of lists (higher than 1)
    while (cin.fail() || k < 2) {
        cout << "\nPlease give a non negative integer number higher than 1: ";
        cin.clear();
        cin.ignore(256, '\n');
        cin >> k;
    }












    Chain<int> OutputList;  // Output list
    

    

    
    //  List_of_lists is a list with pointers
    //  Each of the pointers points to one of the k lists

    Chain< Chain<int>* > List_of_lists;

    

    //  templist is a pointer used to CREATE lists
    //  templist will also be used later to ACCESS the lists inside List_of_Lists

    Chain<int>* templist;




    for (int i = 0; i < k; i++) {

        templist  = new Chain<int>; //  Making new Chain with templist
        //  All chains created that way must be deleted later        
        

        
        //  Calling the generator to choose a random size for the list
        int list_size = random_list_size();
        

        for (int j = 0; j < list_size; j++) {
            //  Calling the generator to choose a random data and inserting it in the list
            templist->Insert(j, random_element());
        }

        //  Adding the pointer in List_of_lists
        List_of_lists.Insert(i, templist);

    }

    
        












    /*   
     * 
     *   Starting the process of creating the Output List
     * 
     *   Chooses first number of the first list, but using the method InsertSorted the number is added in Output list sorted (if it should be added)
     *   After that, the number is deleted from all lists and the process is repeated
     * 
     *   InsertSorted adds an int number in a list in a way that if the list is already sorted with ascending order then the number should be added in the correct place to keep the list sorted
     *   Thus inserting numbers in Output list using only InsertSorted method will make Output list sorted
     * 
    */



    //  LeftLists variable represents the number of lists remaining in List_of_Lists
    //  When a list is emtpy, it is deleted
    int LeftLists = k;



    //  ListsNeeded4Output variable represents the number of lists in which a number must appears in order to be added in Output List
    int ListsNeeded4Output = ceil(k / 2.0);

    

    if (k > 0) {


        /*
         *  The process of making the Output list continues WHILE LeftLists >= ListsNeeded4Output
         *  A number needs to appear at least in ceil(k / 2.0) lists to be added in Output
         *  If less than ceil(k / 2.0) lists exist then the rest of the numbers in any of the remaining list wont be added in Output anyway
        */
        while (LeftLists >= ListsNeeded4Output) {



            //  Starting with the first number in the first list
            List_of_lists.Find(1, templist);
            int PossibleOutputItem;
            templist->Find(1, PossibleOutputItem);
            int count = 1;




            //checking how many times it exists in all lists
            for (int i = 2; i <= LeftLists && count < ListsNeeded4Output; i++) {
                // Once count becomes equal to ListsNeeded4Output, the number will be added to Output so we dont need to check if it exists in the rest lists

                List_of_lists.Find(i, templist);
                if (templist->Search(PossibleOutputItem) != 0) {
                    //  if it exists in list that templist shows then count will be increased
                    count++;

                }

            }




            //if it exists in at least ListsNeeded4Output lists then it will be added in Output using method InsertSorted
            //InsertSorted is special method created to add an int in an ascending sorted list in the correct place to keep it sorted
            if (count >= ListsNeeded4Output) {
                OutputList.InsertSorted(PossibleOutputItem);
            }




            //Deleting the number from all lists
            for (int i = 1; i <= LeftLists; i++) {

                List_of_lists.Find(i, templist);
                templist->DeleteAllOccurrence(PossibleOutputItem);

                if (templist->IsEmpty()) {

                    //  If the list is empty then it will be deleted from List_of_lists and from the memory
                    List_of_lists.Delete(i, templist);
                    templist->~Chain();
                    delete templist;
                    LeftLists--;
                    i--;

                }

            }

        }
    }


    //  Printing Output list
    cout << "\n The output list is: \n";
    cout << OutputList;


}