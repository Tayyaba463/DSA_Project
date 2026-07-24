#include <iostream>
#include <fstream>
#include <string>

using namespace std;

// File paths (same naming style as main backend.cpp)
const string ACTIVE_FILE = "data/active_orders.txt";
const string HISTORY_FILE = "data/order_history.txt";

// ===== NODE =====
struct Order
{
    int id;
    string customer;
    string food;
    string status;
    Order* next;
};

// ===== QUEUE =====
struct Queue
{
    Order* front;
    Order* rear;
};

int nextId = 1001;   // simple auto-increment id, same style as main backend

// ===== INIT =====
void initQueue(Queue &q)
{
    q.front = q.rear = NULL;
}

// ===== ENQUEUE =====
void enqueue(Queue &q, int id, string name, string food)
{
    Order* newNode = new Order{id, name, food, "pending", NULL};

    if (q.rear == NULL)
    {
        q.front = q.rear = newNode;
    }
    else
    {
        q.rear->next = newNode;
        q.rear = newNode;
    }
}

// ===== SAVE FILE (rewrites full active queue, same idea as saveActiveOrders in main backend) =====
void saveActiveOrders(Queue &q)
{
    ofstream file(ACTIVE_FILE.c_str());
    if (!file.is_open()) return;

    Order* temp = q.front;
    while (temp != NULL)
    {
        file << temp->id << "," << temp->customer << "," << temp->food << "," << temp->status << "\n";
        temp = temp->next;
    }
    file.close();
}

// ===== SAVE TO HISTORY (append, same idea as addToHistory in main backend) =====
void addToHistory(Order* order)
{
    ofstream file(HISTORY_FILE.c_str(), ios::app);
    if (!file.is_open()) return;

    file << order->id << "," << order->customer << "," << order->food << ",completed\n";
    file.close();
}

// ===== PEEK =====
void peek(Queue &q)
{
    if (q.front == NULL)
    {
        cout << "Queue Empty." << endl;
        return;
    }

    cout << "Next Order -> #" << q.front->id << " | "
         << q.front->customer << " | "
         << q.front->food << endl;
}

// ===== SEARCH =====
void searchOrder(Order* head, string name)
{
    Order* temp = head;
    bool found = false;

    while (temp != NULL)
    {
        if (temp->customer == name)
        {
            cout << "Found: #" << temp->id << " " << temp->customer
                 << " -> " << temp->food << " [" << temp->status << "]" << endl;
            found = true;
        }
        temp = temp->next;
    }

    if (!found)
        cout << "Order not found." << endl;
}

// ===== COUNT =====
int countOrders(Order* head)
{
    int count = 0;
    while (head != NULL)
    {
        count++;
        head = head->next;
    }
    return count;
}

// ===== DELETE (cancel order, mirrors cancelOrder in main backend) =====
void deleteOrder(Queue &q, string name)
{
    Order* temp = q.front;
    Order* prev = NULL;

    while (temp != NULL)
    {
        if (temp->customer == name)
        {
            if (prev == NULL)
                q.front = temp->next;
            else
                prev->next = temp->next;

            if (temp == q.rear)
                q.rear = prev;

            delete temp;
            cout << "Order deleted." << endl;
            saveActiveOrders(q);
            return;
        }

        prev = temp;
        temp = temp->next;
    }

    cout << "Order not found." << endl;
}

// ===== DISPLAY QUEUE =====
void showQueue(Queue &q)
{
    Order* temp = q.front;

    cout << "\n--- Active Queue ---\n";
    if (temp == NULL)
    {
        cout << "(empty)\n";
        return;
    }
    while (temp != NULL)
    {
        cout << "#" << temp->id << " | " << temp->customer
             << " -> " << temp->food << " [" << temp->status << "]" << endl;
        temp = temp->next;
    }
}

// ===== LINKED LIST DISPLAY =====
void showLinkedList(Order* head)
{
    if (head == NULL)
    {
        cout << "Linked List is empty." << endl;
        return;
    }

    cout << "\n--- Linked List ---\n";

    Order* temp = head;
    while (temp != NULL)
    {
        cout << "ID: " << temp->id
             << " | Name: " << temp->customer
             << " | Food: " << temp->food
             << " | Status: " << temp->status << endl;
        temp = temp->next;
    }
}

// ===== MAIN =====
int main()
{
    system("mkdir data 2>nul || mkdir -p data");

    Queue q;
    initQueue(q);

    string name, food, searchName, deleteName;
    int choice;

    while (true)
    {
        cout << endl;
        cout << "===== Restaurant Order Queue =====" << endl;
        cout << "1. Add Order (Name + Food)" << endl;
        cout << "2. Show Queue" << endl;
        cout << "3. Serve Order (Dequeue)" << endl;
        cout << "4. Peek Front Order" << endl;
        cout << "5. Search Order" << endl;
        cout << "6. Total Orders" << endl;
        cout << "7. Delete/Cancel Order" << endl;
        cout << "8. Display Linked List" << endl;
        cout << "9. Exit" << endl;
        cout << "Choice: ";
        cin >> choice;

        if (choice == 1)
        {
            cout << "Enter Name: ";
            cin >> name;
            cout << "Enter Food: ";
            cin.ignore();
            getline(cin, food);

            int id = nextId++;
            enqueue(q, id, name, food);
            saveActiveOrders(q);

            cout << "Order Added -> #" << id << endl;
        }

        else if (choice == 2)
        {
            showQueue(q);
        }

        else if (choice == 3)
        {
            if (q.front == NULL)
            {
                cout << "Queue Empty." << endl;
            }
            else
            {
                Order* temp = q.front;
                cout << "Served: #" << temp->id << " " << temp->customer
                     << " -> " << temp->food << endl;

                addToHistory(temp);

                q.front = q.front->next;
                if (q.front == NULL)
                    q.rear = NULL;

                delete temp;
                saveActiveOrders(q);
            }
        }

        else if (choice == 4)
        {
            peek(q);
        }

        else if (choice == 5)
        {
            cout << "Enter name to search: ";
            cin >> searchName;
            searchOrder(q.front, searchName);
        }

        else if (choice == 6)
        {
            cout << "Total Orders: " << countOrders(q.front) << endl;
        }

        else if (choice == 7)
        {
            cout << "Enter name to delete: ";
            cin >> deleteName;
            deleteOrder(q, deleteName);
        }

        else if (choice == 8)
        {
            showLinkedList(q.front);
        }

        else if (choice == 9)
        {
            break;
        }

        else
        {
            cout << "Invalid Option." << endl;
        }
    }

    return 0;
}