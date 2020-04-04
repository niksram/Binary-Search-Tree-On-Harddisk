#include "assignment_4.h"
#include <stdlib.h>

void preorder_rec(FILE *, int);             // recursive functions to find preorder
void inorder_rec(FILE *, int);              // recursive functions to find inorder
int insert_at_free(FILE *, node_t, tree_t); //inserts the given node at the free_head location or EOF
node_t create_node(int);                    // it initialises a leaf node with the given key values
node_t read_node(FILE *, int);              // reads the node from the file at a given offset
tree_t read_tree(FILE *);                   // reads the tree structure at the start of the file
void write_tree(FILE *, tree_t);            //writes the given tree to the start of the file
void write_node(FILE *, node_t, int);       //writes the given node at the input integer offset
void free_node(FILE *, int);                //called to free the node from tree and link it to the list of free nodes

void write_node(FILE *fp, node_t new, int offset)
{
    fseek(fp, sizeof(tree_t) + offset * sizeof(node_t), SEEK_SET); //seeking the offset
    fwrite(&new, sizeof(node_t), 1, fp);
}

void free_node(FILE *fp, int offset)
{
    tree_t tree = read_tree(fp);
    node_t node = read_node(fp, offset);
    node.left_offset = tree.free_head; //nodes left points to the previous fee_head
    tree.free_head = offset;           // free_head of the tree type pints to current offset
    write_node(fp, node, offset);
    write_tree(fp, tree);
}

void write_tree(FILE *fp, tree_t tree)
{
    fseek(fp, 0, SEEK_SET);
    fwrite(&tree, sizeof(tree), 1, fp);
}

int insert_at_free(FILE *fp, node_t new, tree_t tree)
{
    if (tree.free_head >= 0) //if free_head has valid location, node is written there
    {
        int old_free_head = tree.free_head;
        node_t node = read_node(fp, tree.free_head);
        write_node(fp, new, tree.free_head);
        tree.free_head = node.left_offset; //free_head points to new free location
        write_tree(fp, tree);
        return (old_free_head); //returns current nodes offset after writing in file.
    }
    else //if free_head is -1, then node needs to be written at the end of file
    {
        fseek(fp, 0, SEEK_END);
        fwrite(&new, sizeof(new), 1, fp);
        return ((ftell(fp) - sizeof(tree_t) - sizeof(node_t)) / sizeof(node_t)); // current nodes offset is determined here
    }
}

node_t create_node(int key)
{
    node_t new;
    new.key = key;
    new.left_offset = -1; //initialise left and right offset as a leaf node
    new.right_offset = -1;
    return new;
}

tree_t read_tree(FILE *fp)
{
    tree_t tree;
    fseek(fp, 0, SEEK_SET); //read the tree header at the start of the file
    fread(&tree, sizeof(tree), 1, fp);
    return tree;
}

node_t read_node(FILE *fp, int offset)
{
    node_t node;
    fseek(fp, sizeof(tree_t) + offset * sizeof(node_t), SEEK_SET);
    fread(&node, sizeof(node_t), 1, fp);
    return node;
}

FILE *init_tree(const char *filename)
{
    FILE *fp;
    fp = fopen(filename, "rb+"); // reads initially in rb+ mode... returns NULL if file doesn't exist
    if (!fp)                     // if file doesn't exist, we need to initialise the tree data-structure at the start of the file.
    {
        fp = fopen(filename, "wb+"); //opened in write mode to create the file
        tree_t tree;
        tree.free_head = -1;
        tree.root = -1;
        fwrite(&tree, sizeof(tree_t), 1, fp);
        fclose(fp);
        fp = fopen(filename, "rb+"); //for safety, file in writemode is closed and opened in rb+ mode
    }
    return fp;
}

void insert_key(int key, FILE *fp)
{
    node_t new = create_node(key); //node is created
    tree_t tree = read_tree(fp);
    int flag = 1;
    if (tree.root == -1) //if tree is empty.
    {
        int root = insert_at_free(fp, new, tree); //node is written to the file at free location
        tree = read_tree(fp);
        tree.root = root; //root in the tree structure at the file beginning is made to point to its offset
        write_tree(fp, tree);
    }
    else
    {
        node_t node = read_node(fp, tree.root);
        int parent_offset = tree.root;
        int flag = 1;
        while (flag) // traverse through the binary search tree to find the node under which this new node is to be inserted
        {
            if (new.key < node.key) //move left
            {
                if (node.left_offset >= 0) // if there exists a node to its left
                {
                    parent_offset = node.left_offset;
                    node = read_node(fp, node.left_offset);
                }
                else // else node can be inserted there
                {
                    node.left_offset = insert_at_free(fp, new, tree);
                    write_node(fp, node, parent_offset);
                    flag--;
                }
            }
            else if (new.key > node.key) // move right
            {
                if (node.right_offset >= 0) // if there exists a node to its right
                {
                    parent_offset = node.right_offset;
                    node = read_node(fp, node.right_offset);
                }
                else // else node can be inserted there
                {
                    node.right_offset = insert_at_free(fp, new, tree);
                    write_node(fp, node, parent_offset);
                    flag--;
                }
            }
            else // if node with same key exists, then exit loop
                flag--;
        }
    }
}

void delete_key(int key, FILE *fp)
{
    tree_t tree = read_tree(fp);
    if (tree.root != -1)
    {
        node_t node = read_node(fp, tree.root);
        int parent_offset = -1;
        int curr_offset = tree.root;
        int flag = 1;
        int left = -1;  // -1 if node is root, 1 if node is on parent's right, 0 if node is on parent's left
        while (flag) // loop similar to that in 'insert_key', to determine the node to be deleted by traversion the BST
        {
            if (key < node.key)
            {
                if (node.left_offset >= 0)
                {
                    left = 1;
                    parent_offset = curr_offset;
                    curr_offset = node.left_offset;
                    node = read_node(fp, node.left_offset);
                }
                else
                    return;
            }
            else if (key > node.key)
            {
                if (node.right_offset >= 0)
                {
                    left = 0;
                    parent_offset = curr_offset;
                    curr_offset = node.right_offset;
                    node = read_node(fp, node.right_offset);
                }
                else
                    return;
            }
            else
                flag--;
        }

        if (node.right_offset == -1 || node.left_offset==-1) // if only right or only left or no sub tree exists
        {
            int child=node.right_offset==-1 ? node.left_offset:node.right_offset; //child holds the offset of the only subtree that exists, else -1
            if (parent_offset == -1)    //if node is root
            {
                tree_t tree = read_tree(fp);
                tree.root = child;  // tree root is modified to point to child, else -1
                write_tree(fp, tree);
            }
            else    // else parent's (left/right) offset is made to point to child sub-tree
            {
                node_t parent = read_node(fp, parent_offset);
                if (left)   // decides either to parents left or right
                    parent.left_offset = child;
                else
                    parent.right_offset = child;
                write_node(fp, parent, parent_offset);
            }
        }
        else // if both sub tree exist
        {
            
            node_t new = read_node(fp, node.right_offset);
            int new_offset = node.right_offset;
            int new_parent_offset = -1;
            while (new.left_offset != -1)   // inorder successor is found
            {
                new_parent_offset = new_offset;
                new_offset = new.left_offset;
                new = read_node(fp, new_offset);
            }
            if (new_parent_offset != -1)    // if inorder successor's parent is not the node to be deleted
            {
                node_t new_par = read_node(fp, new_parent_offset);  
                new_par.left_offset = new.right_offset; // the parent of the inorder successor's left shall have the right subtree of the successor
                write_node(fp, new_par, new_parent_offset);
                node.key = new.key; // key of the successor is overwritten on node.key
                write_node(fp, node, curr_offset);
                curr_offset = new_offset;   // the node to be freed is the duplicate which exists
            }
            else    // if inorder successor's parent is the node to be deleted itself
            {
                new.left_offset = node.left_offset; //inorder successor left shall point to the left sub-tree of the node
                write_node(fp, new, new_offset);
                if (parent_offset == -1)    // if the node is root, then tree header structure of the file is modified
                {
                    tree_t tree = read_tree(fp);
                    tree.root = node.right_offset;
                    write_tree(fp, tree);
                }
                else    // else node's parent's offset is modified accordingly
                {
                    node_t parent = read_node(fp, parent_offset);
                    if (left)
                        parent.left_offset = node.right_offset;
                    else
                        parent.right_offset = node.right_offset;
                    write_node(fp, parent, parent_offset);
                }
            }
        }
        free_node(fp, curr_offset); //node at curr_offset is freed
    }
}

void display_preorder(FILE *fp)
{
    tree_t tree = read_tree(fp);
    preorder_rec(fp, tree.root);
    printf("\n");
}

void preorder_rec(FILE *fp, int offset) // recursive function for preorder
{
    if (offset >= 0)
    {
        node_t node = read_node(fp, offset);
        printf("%d ", node.key);
        preorder_rec(fp, node.left_offset);
        preorder_rec(fp, node.right_offset);
    }
}

void display_inorder(FILE *fp)
{
    tree_t tree = read_tree(fp);
    inorder_rec(fp, tree.root);
    printf("\n");
}

void inorder_rec(FILE *fp, int offset) // recursive function for inorder
{
    if (offset >= 0)
    {
        node_t node = read_node(fp, offset);
        inorder_rec(fp, node.left_offset);
        printf("%d ", node.key);
        inorder_rec(fp, node.right_offset);
    }
}

void close_tree(FILE *fp)
{
    if (fp != NULL)
        fclose(fp);
}