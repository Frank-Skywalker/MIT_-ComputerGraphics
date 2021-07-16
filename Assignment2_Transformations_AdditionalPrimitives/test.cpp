#include <iostream>

using namespace std;


class Node{
    public:
    Node()
    {
        x=1;
    }
    int& getX()
    {
        return x;
    }
    int x;
};

int main()
{
    Node node;
    int a=node.getX();
    cout<<a<<endl;
    a=2;
    cout<<a<<endl;
    cout<<node.x<<endl;
    node.x=3;
    cout<<node.x<<endl;
    cout<<a<<endl;
}