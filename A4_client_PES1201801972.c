#include"assignment_4.h"
#include<stdlib.h>

int main()
{
    char c[100];
    scanf("%s",c);
    FILE* fp=init_tree(c);
    insert_key(5,fp);
    insert_key(10,fp);
    insert_key(1,fp);
    insert_key(3,fp);
    insert_key(9,fp);
    insert_key(7,fp);
    display_preorder(fp);
    display_inorder(fp);
    close_tree(fp);
}