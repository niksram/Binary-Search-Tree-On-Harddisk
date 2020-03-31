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
    insert_key(15,fp);
    insert_key(12,fp);
    insert_key(13,fp);
    delete_key(10,fp);
    display_preorder(fp);
    display_inorder(fp);
    close_tree(fp);
}