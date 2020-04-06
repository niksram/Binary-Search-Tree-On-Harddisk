STORING BINARY-SEARCH-TREE IN HARD-DISK

    The following program helps in preserving a Binary Search Tree in a file which can be easily ported to any device and be read back.
    The following program incorporates the concept of Dynamically Allocated Memory (Linked Lists) to Linearly Allocated Memory (array), hence insertion and deletion of nodes doesn't affect the relative position of other nodes physically stored in the file. Similarly free spaced in-between the fle are reused when new nodes are inserted.
    