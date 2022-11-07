#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <algorithm>

using namespace std;

int main()
{
    // read file
    ifstream infile("retail.csv");
    string line;
    // store the header
    getline(infile, line);
    // create a vector to store all the data
    vector<vector<string> > data;
    // read rest of the data
    while (getline(infile, line))
    {
        istringstream iss(line);
        string value;
        vector<string> row;
        while (getline(iss, value, ','))
        {
            row.push_back(value);
        }
        data.push_back(row);
    }
    // close file
    infile.close();
    
    // create a map to store customer id and purchase history
    map<string, set<string> > customer_purchase;
    // create a map to store item id and customers who bought it
    map<string, set<string> > item_customer;
    // create a map to store item id and its category
    map<string, string> item_category;
    // create a map to store customer id and its category
    map<string, string> customer_category;
    
    // go through all the data
    for (int i = 0; i < data.size(); i++)
    {
        if(**data==NULL)
            **data={""};
        // get customer id
        string customer_id = data[i][0];
        // get item id
        string item_id = data[i][1];
        // get category
        string category = data[i][4];
        
        // update the customer_purchase map
        if (customer_purchase.find(customer_id) == customer_purchase.end())
        {
            // if this is the first purchase of the customer
            set<string> purchase;
            purchase.insert(item_id);
            customer_purchase[customer_id] = purchase;
        }
        else
        {
            // if this is not the first purchase of the customer
            customer_purchase[customer_id].insert(item_id);
        }
        
        // update the item_customer map
        if (item_customer.find(item_id) == item_customer.end())
        {
            // if this is the first customer who bought the item
            set<string> customers;
            customers.insert(customer_id);
            item_customer[item_id] = customers;
        }
        else
        {
            // if this is not the first customer who bought the item
            item_customer[item_id].insert(customer_id);
        }
        
        // update the item_category map
        item_category[item_id] = category;
    }
    
    // go through all the customer_purchase map
    for (auto it = customer_purchase.begin(); it != customer_purchase.end(); it++)
    {
        // get customer id
        string customer_id = it->first;
        // get purchase history
        set<string> purchase = it->second;
        
        // create a map to store category and number of purchases
        map<string, int> category_purchase;
        
        // go through all the purchase history
        for (auto it2 = purchase.begin(); it2 != purchase.end(); it2++)
        {
            // get item id
            string item_id = *it2;
            // get category
            string category = item_category[item_id];
            
            // update the category_purchase map
            if (category_purchase.find(category) == category_purchase.end())
            {
                // if this is the first purchase of this category
                category_purchase[category] = 1;
            }
            else
            {
                // if this is not the first purchase of this category
                category_purchase[category]++;
            }
        }
        
        // find the category which has the most purchases
        int max_purchase = 0;
        string max_category;
        for (auto it2 = category_purchase.begin(); it2 != category_purchase.end(); it2++)
        {
            // get category
            string category = it2->first;
            // get number of purchases
            int num_purchase = it2->second;
            
            // update max_purchase and max_category
            if (num_purchase > max_purchase)
            {
                max_purchase = num_purchase;
                max_category = category;
            }
        }
        
        // update the customer_category map
        customer_category[customer_id] = max_category;
    }
    
    // get a customer id from input
    string customer_id;
    cout << "Enter customer id: ";
    cin >> customer_id;
    
    // get the category of the customer
    string category = customer_category[customer_id];
    
    // find all the customers who bought the same category
    set<string> customers;
    for (auto it = item_customer.begin(); it != item_customer.end(); it++)
    {
        // get item id
        string item_id = it->first;
        // get category
        string item_catg = item_category[item_id];
        
        // check if the category matches
        if (category == item_catg)
        {
            // get all the customers who bought the item
            set<string> item_customers = it->second;
            // update the customers set
            customers.insert(item_customers.begin(), item_customers.end());
        }
    }
    
    // remove the input customer from the customers set
    customers.erase(customer_id);
    
    // find the customer who is most similar to the input customer
    int max_similarity = 0;
    string similar_customer;
    for (auto it = customers.begin(); it != customers.end(); it++)
    {
        // get a customer id
        string other_customer_id = *it;
        // get the purchase history of the customer
        set<string> purchase = customer_purchase[other_customer_id];
        // get the purchase history of the input customer
        set<string> input_purchase = customer_purchase[customer_id];
        // find the number of items in the intersection of the two sets
        int num_same = 0;
        for (auto it2 = purchase.begin(); it2 != purchase.end(); it2++)
        {
            // get an item id
            string item_id = *it2;
            // check if the item is in the input customer's purchase history
            if (input_purchase.find(item_id) != input_purchase.end())
            {
                // if the item is in the input customer's purchase history
                num_same++;
            }
        }
        // update max_similarity and similar_customer
        if (num_same > max_similarity)
        {
            max_similarity = num_same;
            similar_customer = other_customer_id;
        }
    }
    
    // get the purchase history of the similar customer
    set<string> similar_purchase = customer_purchase[similar_customer];
    // get the purchase history of the input customer
    set<string> input_purchase = customer_purchase[customer_id];
    // find the items in the similar customer's purchase history but not in the input customer's purchase history
    vector<string> recommendations;
    for (auto it = similar_purchase.begin(); it != similar_purchase.end(); it++)
    {
        // get an item id
        string item_id = *it;
        // check if the item is in the input customer's purchase history
        if (input_purchase.find(item_id) == input_purchase.end())
        {
            // if the item is not in the input customer's purchase history
            recommendations.push_back(item_id);
        }
    }
    
    // print out the recommendations
    cout << "Recommendations:" << endl;
    for (int i = 0; i < recommendations.size(); i++)
    {
        cout << recommendations[i] << endl;
    }
    return 0;
}