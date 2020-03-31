#include "assignment_4.h"
#include <stdlib.h>

void preorder_rec(FILE*,int);

void inorder_rec(FILE*,int);

int insert_at_free(FILE *, node_t, tree_t);

node_t create_node(int);

node_t read_node(FILE *, int);

tree_t read_tree(FILE *);

void write_tree(FILE *, tree_t);

void write_node(FILE *, node_t, int);

void write_node(FILE *fp, node_t new, int offset)
{
    fseek(fp, sizeof(tree_t) + offset * sizeof(node_t), SEEK_SET);
    fwrite(&new, sizeof(node_t), 1, fp);
}

void write_tree(FILE *fp, tree_t tree)
{
    fseek(fp, 0, SEEK_SET);
    fwrite(&tree, sizeof(tree), 1, fp);
}

int insert_at_free(FILE *fp, node_t new, tree_t tree)
{
    if (tree.free_head >= 0)
    {
        node_t node = read_node(fp, tree.free_head);
        write_node(fp, new, tree.free_head);
        tree.free_head = node.left_offset;
        write_tree(fp, tree);
    }
    else
    {
        fseek(fp, 0, SEEK_END);
        fwrite(&new, sizeof(new), 1, fp);
    }
    return ((ftell(fp) - sizeof(tree_t) - sizeof(node_t)) / sizeof(node_t));
}

node_t create_node(int key)
{
    node_t new;
    new.key = key;
    new.left_offset = -1;
    new.right_offset = -1;
    return new;
}

tree_t read_tree(FILE *fp)
{
    tree_t tree;
    fseek(fp, 0, SEEK_SET);
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
    fp = fopen(filename, "rb+");
    if (!fp)
    {
        fp = fopen(filename, "wb+");
        tree_t tree;
        tree.free_head = -1;
        tree.root = -1;
        fwrite(&tree, sizeof(tree_t), 1, fp);
        fclose(fp);
        fp = fopen(filename, "rb+");
    }
    return fp;
}

void insert_key(int key, FILE *fp)
{
    node_t new = create_node(key);
    tree_t tree = read_tree(fp);
    int flag = 1;
    if (tree.root == -1)
    {
        int root = insert_at_free(fp, new, tree);
        tree = read_tree(fp);
        tree.root = root;
        write_tree(fp, tree);
    }
    else
    {
        node_t node = read_node(fp, tree.root);
        int parent_offset=tree.root;
        int flag = 1;
        while (flag)
        {
            if (new.key < node.key)
            {
                if (node.left_offset >= 0)
                {
                    parent_offset = node.left_offset;
                    node = read_node(fp, node.left_offset);
                }
                else
                {
                    node.left_offset = insert_at_free(fp, new, tree);
                    write_node(fp, node, parent_offset);
                    flag--;
                }
            }
            else if (new.key > node.key)
            {
                if (node.right_offset >= 0)
                {
                    parent_offset = node.right_offset;
                    node = read_node(fp, node.right_offset);
                }
                else
                {
                    node.right_offset = insert_at_free(fp, new, tree);
                    write_node(fp, node, parent_offset);
                    flag--;
                }
            }
            else
                flag--;
        }
    }
}

void display_preorder(FILE *fp)
{
    tree_t tree=read_tree(fp);
    preorder_rec(fp,tree.root);
    printf("\n");
}

void preorder_rec(FILE* fp,int offset)
{
    if(offset>=0)
    {
        node_t node=read_node(fp,offset);
        printf("%d ",node.key);
        preorder_rec(fp,node.left_offset);
        preorder_rec(fp,node.right_offset);
    }
}

void display_inorder(FILE *fp)
{
    tree_t tree=read_tree(fp);
    inorder_rec(fp,tree.root);
    printf("\n");
}

void inorder_rec(FILE* fp,int offset)
{
    if(offset>=0)
    {
        node_t node=read_node(fp,offset);
        inorder_rec(fp,node.left_offset);
        printf("%d ",node.key);
        inorder_rec(fp,node.right_offset);
    }
}

void close_tree(FILE *fp)
{
    if (fp != NULL)
        fclose(fp);
}