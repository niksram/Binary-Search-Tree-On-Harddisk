#include "assignment_4.h"
#include <stdlib.h>

void display_with_index(FILE *);

int main()
{
    printf("enter complete file name\n");
    char c[100];
    scanf("%s", c);
    FILE *fp = init_tree(c);
    int val;
    int inp;
    int flag = 1;
    printf("1 for inserting, 2 for deleting, 3 for printing inorder, 4 for printing preorder, 0 for printing file contents as it is\n");
    printf("enter some other value to exit safely\n");
    while (flag)
    {
        scanf("%d", &inp);
        switch (inp)
        {
        case 0:
        {
            display_with_index(fp);
            break;
        }
        case 1:
        {
            scanf("%d", &val);
            insert_key(val, fp);
            break;
        }
        case 2:
        {
            scanf("%d", &val);
            delete_key(val, fp);
            break;
        }
        case 3:
        {
            display_inorder(fp);
            break;
        }
        case 4:
        {
            display_preorder(fp);
            break;
        }
        default:
            flag--;
        }
    }
    fclose(fp);
    printf("TERMINATED\n");
}

void display_with_index(FILE *fp)
{
    fseek(fp, 0, SEEK_SET);
    tree_t tree;
    node_t node,*node2=malloc(sizeof(node_t));
    fread(&tree, sizeof(tree_t), 1, fp);
    printf("%d %d\n", tree.root, tree.free_head);
    int c=0;
    while (!feof(fp))
    {
        fread(node2, sizeof(node_t), 1, fp);
        if(c)
            printf("%d key: %d left:%d right: %d\n", c-1, node.key, node.left_offset, node.right_offset);
        node=*node2;
        c++;
    }
    printf("\n");
    free(node2);
}

// void display_with_index(FILE *fp)
// {
//     fseek(fp, 0, SEEK_SET);
//     tree_t tree;
//     node_t node,*node2=malloc(sizeof(node_t));
//     fread(&tree, sizeof(tree_t), 1, fp);
//     printf("%d %d\n", tree.root, tree.free_head);
//     int c=1;
//     while (!feof(fp))
//     {
//         fread(&node, sizeof(node_t), 1, fp);
//         if(c)
//             printf("%d key-%d left-%d right-%d\n", c-1, node.key, node.left_offset, node.right_offset);
//         //node=*node2;
//         c++;
//     }
//     printf("\n");
//     free(node2);
// }