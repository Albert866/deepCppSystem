/**
 * @file 14_shared_node.cpp
 * @brief shared_ptr<TreeNode> 实现树形结构；shared_ptr<TreeNode> 作为树节点，通过 weak_ptr<TreeNode> parent 记录父节点；
 * @author Albert
 * @version 1.0.0
 * @date 2024-12-06
 */
#include <iostream>
#include <memory>
#include <string>
#include <vector>
using namespace std;

class TreeNode : public enable_shared_from_this<TreeNode>
{
public:
    string name;
    weak_ptr<TreeNode> parent;
    vector<shared_ptr<TreeNode>> subNodes;

    TreeNode(const string & _name) : name(_name) { cout << "ctor " << name << endl; }

    void add_child(shared_ptr<TreeNode> & child)
    {
        subNodes.push_back(child);
        child->parent = shared_from_this();
    }

    ~TreeNode() { cout << "delete " << name << endl; }
};

int main()
{
    shared_ptr<TreeNode> n1 = make_shared<TreeNode>("A");
    shared_ptr<TreeNode> n2 = make_shared<TreeNode>("B");
    shared_ptr<TreeNode> n3 = make_shared<TreeNode>("C");
    shared_ptr<TreeNode> n4 = make_shared<TreeNode>("D");
    shared_ptr<TreeNode> n5 = make_shared<TreeNode>("E");

    n1->add_child(n2);
    n1->add_child(n3);
    n2->add_child(n4);
    n2->add_child(n5);
}
