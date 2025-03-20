#ifndef BSTSET_H
#define BSTSET_H

#include <functional>

template <typename T>
class BSTSet {
private:
    struct Node {
        T value;
        Node* left;
        Node* right;
        
        Node(const T& val) : value(val), left(nullptr), right(nullptr) {}
    };
    
    Node* m_root;
    int m_size;
    
    // Helper function to recursively delete nodes
    void clear(Node* node) {
        if (node == nullptr) {
            return;
        }
        clear(node->left);
        clear(node->right);
        delete node;
    }
    
    // Helper function to copy a BST
    Node* copyTree(Node* src) {
        if (src == nullptr) {
            return nullptr;
        }
        
        Node* newNode = new Node(src->value);
        newNode->left = copyTree(src->left);
        newNode->right = copyTree(src->right);
        return newNode;
    }
    
    // Helper function to insert a value
    bool insert(Node*& node, const T& value) {
        if (node == nullptr) {
            node = new Node(value);
            m_size++;
            return true;
        }
        
        if (value < node->value) {
            return insert(node->left, value);
        } else if (node->value < value) {
            return insert(node->right, value);
        } else {
            // Value already exists
            return false;
        }
    }
    
    // Helper function to check if a value exists
    bool contains(Node* node, const T& value) const {
        if (node == nullptr) {
            return false;
        }
        
        if (value < node->value) {
            return contains(node->left, value);
        } else if (node->value < value) {
            return contains(node->right, value);
        } else {
            return true;
        }
    }

public:
    class SetIterator {
    private:
        // Using a stack to keep track of nodes during traversal
        std::vector<Node*> m_stack;
        
        // Helper function to push all left descendants onto stack
        void pushLeftDescendants(Node* node) {
            while (node != nullptr) {
                m_stack.push_back(node);
                node = node->left;
            }
        }
        
    public:
        SetIterator(Node* root) {
            // Initialize stack with leftmost path from root
            pushLeftDescendants(root);
        }
        
        // Get current element and advance to next
        const T* get_and_advance() {
            if (m_stack.empty()) {
                return nullptr;  // End of iteration
            }
            
            // Get current node and pop from stack
            Node* current = m_stack.back();
            m_stack.pop_back();
            
            // Process right subtree
            pushLeftDescendants(current->right);
            
            return &(current->value);
        }
    };
    
    // Constructor
    BSTSet() : m_root(nullptr), m_size(0) {}
    
    // Copy constructor
    BSTSet(const BSTSet& other) : m_root(nullptr), m_size(other.m_size) {
        m_root = copyTree(other.m_root);
    }
    
    // Assignment operator
    BSTSet& operator=(const BSTSet& other) {
        if (this != &other) {
            clear(m_root);
            m_root = copyTree(other.m_root);
            m_size = other.m_size;
        }
        return *this;
    }
    
    // Destructor
    ~BSTSet() {
        clear(m_root);
    }
    
    // Return number of items in the set
    int size() const {
        return m_size;
    }
    
    // Insert a value into the set
    bool insert(const T& value) {
        return insert(m_root, value);
    }
    
    // Check if a value exists in the set
    bool contains(const T& value) const {
        return contains(m_root, value);
    }
    
    // Create an iterator at the beginning of the set
    SetIterator begin() const {
        return SetIterator(m_root);
    }
};

#endif // BSTSET_H