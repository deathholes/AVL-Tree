#include<iostream>
#include<stack>
using namespace std;
class node
{
private:
    int data;
    int height;
public:
    node *left;
    node *right;
    node(int i=0):data(i),height(1),left(nullptr),right(nullptr){};
    ~node();
    static int heightn(node *);
    void setheight();
    int getdata();
    void putdata(int);
    int getheight();
    void putheight(int );
    bool isleaf();
};
node::~node()
{}
int node::getdata()
{
    return data;
}
int node::getheight()
{
    return height;
}
void node::putdata(int i)
{
    this->data=i;
}
void node::putheight(int h)
{
    this->height=h;
}
bool node::isleaf()
{
    return (!left && !right);
}
void node::setheight()
{
    this->putheight(max(heightn(this->left),heightn(this->right))+1);
}
int node::heightn(node *n)
{
    if(!n)
        return 0;
    return n->getheight();
}

class avl_tree
{
private:
    node *root;
    node * __insert(node *,int );
    node * __delete(node *,int );
    node *clockrotate(node *);
    node *anticlockrotate(node *);
public:
    avl_tree():root(nullptr){};
    ~avl_tree();
    int getbalance(node *)const;
    int height(node *)const;
    void setheight(node *);
    bool isempty();
    node *search(int);
    void insert(int );
    void remove(int );
    void preorder(node *)const;
    friend ostream & operator<<(ostream &,const avl_tree &);
};
avl_tree::~avl_tree()
{}
bool avl_tree::isempty()
{
    return(root==nullptr);
}
node * avl_tree::search(int key)
{
    node *temp=root;
    while(temp)
    {
        if(temp->getdata()==key)
        {
            return temp;
        }
        else
            if(temp->getdata() > key)
                temp=temp->left;
            else
                temp=temp->right;
    }
    return nullptr;
}
ostream & operator<<(ostream &out,const avl_tree &avl)
{
    avl.preorder(avl.root);
    /*stack<node *> st;
    node *temp=avl.root;
    if(temp==nullptr)
    {
        cerr<<"tree is empty";
        return;
    }
    while(true)
    {
        while(temp)
        {
            st.push(temp);
            temp=temp->left;
        }
        if(st.empty())
            return;
        else
        {
            temp=st.top();
            out<<temp->getdata()<<"->";
            st.pop();
            temp=temp->right;
        }
    }*/
    return out;
}
void avl_tree::preorder(node* root)const
{
    if(!root)
        return;
    else
    {
        cout<<root->getdata()<<"->";
        preorder(root->left);
        preorder(root->right);
    }
}
int avl_tree::getbalance(node*a) const
{
    return (height(a->left)-height(a->right));
}
int avl_tree::height(node *n)const
{
    if(!n)
        return 0;
    return n->getheight();
}
void avl_tree::insert(int key)
{
    if(!search(key))
        root=__insert(root,key);
    else
    {
        cerr<<"key already present";
        return;
    }
}

node * avl_tree::__insert(node* root, int key)
{
    if(root==nullptr)
        return new node(key);
    else
        if(root->getdata() > key)
            root->left=__insert(root->left,key);
        else
            root->right=__insert(root->right,key);
    root->setheight();
    int bal=getbalance(root);
    //left left rotate
    if(bal > 1 && root->left->getdata() > key)
        return clockrotate(root);
    //right right rotate
    if(bal < -1 && root->right->getdata() < key)
        return anticlockrotate(root);
    //left right rotate
    if(bal > 1 && root->left->getdata() < key)
    {
        root->left=anticlockrotate(root->left);
        return clockrotate(root);
    }
    //right left rotate
    if(bal < -1 && root->right->getdata() > key)
    {
        root->right=clockrotate(root->right);
        return anticlockrotate(root);
    }
    return root;
}

node *avl_tree::clockrotate(node* root)
{
    node *child = root->left;
    root->left=child->right;
    child->right=root;
    root->setheight();
    child->setheight();
    return child;
}
node *avl_tree::anticlockrotate(node* root)
{
    node *child = root->right;
    root->right=child->left;
    child->left=root;
    root->setheight();
    child->setheight();
    return child;
}

void avl_tree::remove(int key)
{
    if(!search(key))
    {
        cerr<<"key not present";
        return;
    }
    root=__delete(root,key);
}

node * avl_tree::__delete(node*root, int key)
{
    if(!root)
        return nullptr;
    if(root->getdata() > key)
    {
        root->left=__delete(root->left,key);
    }
    else if(root->getdata() < key)
    {
        root->right=__delete(root->right,key);
    }
    else
    {
        //case 1: leaf
        if(root->isleaf())
        {
            delete root;
            return nullptr;
        }
        //case 2: only one child
        if(!root->right)
        {
            node *temp=root->left;
            delete root;
            return temp;
        }
        else if (!root->left)
        {
            node *temp=root->right;
            delete root;
            return temp;
        }
        //case 3: both child
        {
            node *temp=root->left;
            while(temp->right)
                temp=temp->right;
            root->putdata(temp->getdata());
            root->left= __delete(root->left,temp->getdata());
            return root;
        }
    }
    root->setheight();
    int bal=getbalance(root);
    if(bal>1 && getbalance(root->left)>=0)
        return clockrotate(root);
    else if (bal <-1 && getbalance(root->right)<=0)
        return anticlockrotate(root);
    else if(bal >1 && getbalance(root->left)<0)
    {
        root->left=anticlockrotate(root->left);
        return clockrotate(root);
    }
    else if(bal < -1 && getbalance(root->right)>0)
    {
        root->right=clockrotate(root->right);
        return anticlockrotate(root);
    }
    return root;
}
int main()
{
    avl_tree avl;
    avl.insert(8);
    avl.insert(5);
    avl.insert(2);
    avl.insert(1);
    avl.insert(3);
    avl.insert(4);
    avl.insert(0);
    avl.insert(16);
    avl.insert(19);
    cout<<avl<<endl;
    avl.remove(3);
    cout<<avl<<endl;
    cout.flush();
}
