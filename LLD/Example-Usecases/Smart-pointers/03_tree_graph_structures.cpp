/*
====================================================================
DATA STRUCTURES WITH UNIQUE_PTR
====================================================================

PERFECT FOR CODING ROUNDS:
- Binary Trees
- N-ary Trees
- Graphs with ownership
- Linked Lists

WHY UNIQUE_PTR:
- Parent owns children
- Automatic cleanup of entire tree
- No manual delete needed
- Exception-safe
*/

#include <iostream>
#include <memory>
#include <vector>
#include <queue>
using namespace std;

// ========== Binary Tree with unique_ptr ==========
struct TreeNode {
    int val;
    unique_ptr<TreeNode> left;
    unique_ptr<TreeNode> right;
    
    TreeNode(int v) : val(v), left(nullptr), right(nullptr) {
        cout << "TreeNode " << val << " created\n";
    }
    
    ~TreeNode() {
        cout << "TreeNode " << val << " destroyed\n";
    }
};

class BinaryTree {
    unique_ptr<TreeNode> root;
    
public:
    // Insert node
    void insert(int val) {
        root = insertHelper(move(root), val);
    }
    
    unique_ptr<TreeNode> insertHelper(unique_ptr<TreeNode> node, int val) {
        if(!node) {
            return make_unique<TreeNode>(val);
        }
        
        if(val < node->val) {
            node->left = insertHelper(move(node->left), val);
        } else {
            node->right = insertHelper(move(node->right), val);
        }
        
        return node;
    }
    
    // Inorder traversal
    void inorder() {
        cout << "Inorder: ";
        inorderHelper(root.get());
        cout << "\n";
    }
    
    void inorderHelper(TreeNode* node) {
        if(!node) return;
        inorderHelper(node->left.get());
        cout << node->val << " ";
        inorderHelper(node->right.get());
    }
    
    // Tree will be automatically cleaned up when BinaryTree is destroyed!
};

// ========== N-ary Tree (File System Example) ==========
struct FileNode {
    string name;
    bool isDirectory;
    vector<unique_ptr<FileNode>> children; // Directory owns its files
    
    FileNode(const string& n, bool isDir) : name(n), isDirectory(isDir) {
        cout << (isDirectory ? "Dir" : "File") << " '" << name << "' created\n";
    }
    
    ~FileNode() {
        cout << (isDirectory ? "Dir" : "File") << " '" << name << "' destroyed\n";
    }
    
    void addChild(unique_ptr<FileNode> child) {
        children.push_back(move(child));
    }
    
    void display(int depth = 0) {
        for(int i = 0; i < depth; i++) cout << "  ";
        cout << (isDirectory ? "[D] " : "[F] ") << name << "\n";
        
        for(auto& child : children) {
            child->display(depth + 1);
        }
    }
};

class FileSystem {
    unique_ptr<FileNode> root;
    
public:
    FileSystem() {
        root = make_unique<FileNode>("root", true);
    }
    
    void createStructure() {
        // Create /root/home
        auto home = make_unique<FileNode>("home", true);
        home->addChild(make_unique<FileNode>("file1.txt", false));
        home->addChild(make_unique<FileNode>("file2.txt", false));
        
        // Create /root/var
        auto var = make_unique<FileNode>("var", true);
        var->addChild(make_unique<FileNode>("log.txt", false));
        
        root->addChild(move(home));
        root->addChild(move(var));
    }
    
    void display() {
        cout << "\n=== File System Structure ===\n";
        root->display();
    }
};

// ========== Graph Node (for adjacency list) ==========
struct GraphNode {
    int id;
    // If we want to own the adjacent nodes, use unique_ptr
    // If we just reference them, use raw pointers or indices
    
    GraphNode(int i) : id(i) {}
};

class Graph {
    vector<unique_ptr<GraphNode>> nodes;
    
public:
    void addNode(int id) {
        nodes.push_back(make_unique<GraphNode>(id));
    }
    
    int size() {
        return nodes.size();
    }
    
    // Entire graph automatically cleaned up!
};

int main() {
    cout << "=== BINARY TREE DEMO ===\n";
    {
        BinaryTree tree;
        tree.insert(5);
        tree.insert(3);
        tree.insert(7);
        tree.insert(1);
        tree.insert(9);
        tree.inorder();
        
        cout << "\n--- Tree going out of scope ---\n";
    }
    // Entire tree automatically destroyed in correct order!
    
    cout << "\n=== FILE SYSTEM DEMO ===\n";
    {
        FileSystem fs;
        fs.createStructure();
        fs.display();
        
        cout << "\n--- FileSystem going out of scope ---\n";
    }
    // Entire file system automatically cleaned up!
    
    cout << "\n=== GRAPH DEMO ===\n";
    {
        Graph g;
        g.addNode(1);
        g.addNode(2);
        g.addNode(3);
        cout << "Graph with " << g.size() << " nodes created\n";
        
        cout << "\n--- Graph going out of scope ---\n";
    }
    
    cout << "\n=== PROGRAM ENDED ===\n";
    return 0;
}
