#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {
    TreeMap* map = (TreeMap*) malloc(sizeof(TreeMap)); 
    if (map == NULL) return NULL;                      

    map->root = NULL;                                 
    map->current = NULL;                             
    map->lower_than = lower_than;                     

    return map;                                       
}


void insertTreeMap(TreeMap * tree, void* key, void * value) {
    if (tree == NULL) return;

    if (tree->root == NULL) {
        TreeNode* new_node = createTreeNode(key, value);
        tree->root = new_node;
        tree->current = new_node;
        return;
    }

    TreeNode* node = tree->root;
    TreeNode* parent = NULL;

    while (node != NULL) {
        parent = node;
        if (is_equal(tree, key, node->pair->key)) return;

        if (tree->lower_than(key, node->pair->key)) {
            node = node->left;
        } else {
            node = node->right;
        }
    }

    TreeNode* new_node = createTreeNode(key, value);
    new_node->parent = parent;

    if (tree->lower_than(key, parent->pair->key)) {
        parent->left = new_node;
    } else {
        parent->right = new_node;
    }
    tree->current = new_node;


}

TreeNode * minimum(TreeNode * x) {
    if (x == NULL) return NULL;
    while (x->left != NULL) {
        x = x->left;
    }
    return x;
}


void removeNode(TreeMap * tree, TreeNode* node) {
    if (node->left == NULL && node->right == NULL) {
        if (node->parent == NULL) {
            tree->root = NULL;
        } else {
            if (node->parent->left == node) node->parent->left = NULL;
            else node->parent->right = NULL;
        }
        free(node->pair);
        free(node);
        return;
    }

    if (node->left == NULL || node->right == NULL) {
        TreeNode* child = (node->left != NULL) ? node->left : node->right;

        if (node->parent == NULL) {
            tree->root = child;
        } else {
            if (node->parent->left == node) node->parent->left = child;
            else node->parent->right = child;
        }
        child->parent = node->parent;

        free(node->pair);
        free(node);
        return;
    }

    
    TreeNode* minNode = minimum(node->right);
    node->pair = minNode->pair;
    removeNode(tree, minNode);
}


void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}


Pair * searchTreeMap(TreeMap * tree, void* key) {
    TreeNode* current = tree->root;

    while (current != NULL) {
        if (is_equal(tree, key, current->pair->key)) {
            tree->current = current;
            return current->pair;
        }
        if (tree->lower_than(key, current->pair->key)) {
            current = current->left;
        } else {
            current = current->right;
        }
    }

    return NULL;
}


Pair * upperBound(TreeMap * tree, void* key) {
    TreeNode* current = tree->root;
    TreeNode* candidate = NULL;

    while (current != NULL) {
        if (is_equal(tree, key, current->pair->key)) {
            tree->current = current;
            return current->pair;
        }
        if (tree->lower_than(key, current->pair->key)) {
            candidate = current;
            current = current->left;
        } else {
            current = current->right;
        }
    }    

    if (candidate != NULL) {
        tree->current = candidate;
        return candidate->pair;
    }

    return NULL;

}

Pair * firstTreeMap(TreeMap * tree) {
    if (tree == NULL || tree->root == NULL) return NULL;

    TreeNode* minNode = minimum(tree->root);
    if (minNode != NULL) {
        tree->current = minNode;
        return minNode->pair;
    }

    return NULL;
}

Pair * nextTreeMap(TreeMap * tree) {
    if (tree == NULL || tree->current == NULL) return NULL;

    TreeNode * current = tree->current;

    if (current->right != NULL) {
        current = current->right;
        while (current->left != NULL) {
            current = current->left;
        }

        tree->current = current;
        return current->pair;
    }
    TreeNode * parent = current->parent;
    while (parent != NULL && current == parent->right) {
        current = parent;
     
     
        parent = parent->parent;
    }  

    if (parent != NULL) {
        tree->current = parent;
        return parent->pair;
    }
    return NULL;


}
