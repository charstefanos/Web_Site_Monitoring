/* **************************************************************************************
   *                                                                                    *
   *   Stefanos Charalamobus, 2315792c, SP Exercise 1a                                  *
   *   This is my own work as defined in the Academic Ethics agreement I have signed.   *
   *                                                                                    *
   ************************************************************************************** */

#include "tldlist.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>

void rebalance(TLDNode *node, TLDList *tld);
TLDNode *rotate_left(TLDNode *node);
TLDNode *rotate_right(TLDNode *node);
TLDNode *rotate_left_then_right(TLDNode *node);
TLDNode *rotate_right_then_left(TLDNode *node);
void set_balance(TLDNode *node);
int height(TLDNode *node);
void re_height(TLDNode *node);
int max(int num1, int num2);
char *tldstrip(char *hostname);
TLDNode *next_node(TLDNode *node);
TLDNode *create_tldnode(char *tldname, TLDNode *parent);
void node_destroy(TLDNode *node);
int tld_insert(TLDNode *node, char *hostname, TLDList *tld);

struct tldlist
{
    Date *begin;
    Date *end;
    TLDNode *root;
    int countList;
};

struct tldnode
{
    char *content;
    int countNode;
    int height;
    int balance;
    TLDNode *left;
    TLDNode *right;
    TLDNode *parent;
};

struct tlditerator
{
    TLDNode *node;
};

/* AVL Functions */
void rebalance(TLDNode *node, TLDList *tld)
{
    set_balance(node);

    if (node->balance == -2)
    {
        if (height(node->left->left) >= height(node->left->right))
            node = rotate_right(node);
        else
            node = rotate_left_then_right(node);
    }

    else if (node->balance == 2)
    {
        if (height(node->right->right) >= height(node->right->left))
            node = rotate_left(node);
        else
            node = rotate_right_then_left(node);
    }

    if (node->parent != NULL)
        rebalance(node->parent, tld);
    else
        tld->root = node;
}

TLDNode *rotate_left(TLDNode *node)
{
    TLDNode *right = node->right;

    right->parent = node->parent;
    node->right = right->left;

    if (node->right != NULL)
        node->right->parent = node;

    right->left = node;
    node->parent = right;

    if (right->parent != NULL)
    {
        if (right->parent->right == node)
            right->parent->right = right;
        else
            right->parent->left = right;
    }

    set_balance(node);
    set_balance(right);

    return right;
}

TLDNode *rotate_right(TLDNode *node)
{
    TLDNode *left = node->left;

    left->parent = node->parent;
    node->left = left->right;

    if (node->left != NULL)
        node->left->parent = node;

    left->right = node;
    node->parent = left;

    if (left->parent != NULL)
    {
        if (left->parent->right == node)
            left->parent->right = left;
        else
            left->parent->left = left;
    }

    set_balance(node);
    set_balance(left);

    return left;
}

TLDNode *rotate_left_then_right(TLDNode *node)
{
    node->left = rotate_left(node->left);
    return rotate_right(node);
}

TLDNode *rotate_right_then_left(TLDNode *node)
{
    node->right = rotate_right(node->right);
    return rotate_left(node);
}

void set_balance(TLDNode *node)
{
    re_height(node);
    node->balance = height(node->right) - height(node->right);
}

int height(TLDNode *node)
{
    if (node != NULL)
        return node->height;

    return -1;
}

void re_height(TLDNode *node)
{
    if (node != NULL)
        node->height = 1 + max(height(node->left), height(node->right));
}

int max(int num1, int num2)
{
    return ((num1 < num2) ? num2 : num1);
}

//returns the tld from a given hostname
char *tldstrip(char *hostname)
{
    char *host;
    host = (char *)malloc((strlen(hostname) + 1) * sizeof(char));

    if (host != NULL)
        for (int i = 0; i < strlen(hostname); i++)
            host[i] = tolower(hostname[i]);

    return host;
}

/*
 * tldlist_create generates a list structure for storing counts against
 * top level domains (TLDs)
 *
 * creates a TLDList that is constrained to the `begin' and `end' Date's
 * returns a pointer to the list if successful, NULL if not
 */
TLDList *tldlist_create(Date *begin, Date *end)
{
    //initialises new tld list and creates space for it on heap
    TLDList *tld = (TLDList *)malloc(sizeof(TLDList));
    if (tld != NULL)
    {
        //if malloc was successful initialise the dates
        tld->begin = begin;
        tld->end = end;
    }

    return tld;
};

/*
 * tldlist_iter_create creates an iterator over the TLDList; returns a pointer
 * to the iterator if successful, NULL if not
 */
TLDIterator *tldlist_iter_create(TLDList *tld)
{
    if (tld->root != NULL)
    {
        //creates a new TLDIterator and assigns space for it on the heap
        TLDIterator *iterator = (TLDIterator *)malloc(sizeof(TLDIterator));

        if (iterator != NULL)
        {
            TLDNode *prev = tld->root;
            TLDNode *cur = prev->left;

            while (cur != NULL)
            {
                prev = cur;
                cur = cur->left;
            }

            iterator->node = prev;
        }
        return iterator;
    }
    return NULL;
};

/*
 * tldlist_iter_next returns the next element in the list; returns a pointer
 * to the TLDNode if successful, NULL if no more elements to return
 */
TLDNode *tldlist_iter_next(TLDIterator *iter)
{
    if (iter != NULL)
    {
        TLDNode *cur = iter->node;
        TLDNode *next = next_node(cur);
        iter->node = next;
        return cur;
    }
    return NULL;
};

//finds the next node to be added
TLDNode *next_node(TLDNode *node)
{
    //checks if given node was passed as null
    if (node)
    {
        //traverse down the tree from the right if not null
        if (node->right)
        {
            TLDNode *cur = node->right;
            TLDNode *parentN = node;
            while (cur)
            {
                parentN = cur;
                cur = cur->left;
            }
            return parentN;
        }

        else if (node->parent != NULL)
        {
            if (node->parent->left == node)
                return node->parent;

            else
            {
                TLDNode *parentN = node->parent;
                TLDNode *cur = node;
                while (parentN && (parentN->right == cur))
                {
                    cur = parentN;
                    parentN = parentN->parent;
                }
                //
                return parentN;
            }
        }
    }
    //Tree with root but no children
    return NULL;
}

/*
 * tldlist_iter_destroy destroys the iterator specified by `iter'
 */
void tldlist_iter_destroy(TLDIterator *iter)
{
    free(iter);
}

//function to create a TLDNode and return a pointer to it
TLDNode *create_tldnode(char *tldname, TLDNode *parent)
{
    //creates a new TLDNode and assigns memory to it on the heap
    TLDNode *node = (TLDNode *)malloc(sizeof(TLDNode));

    if (node != NULL)
    {
        //if the malloc was successful initialise all the node attributes
        node->content = tldname;
        node->countNode = 1;
        node->height = 0;
        node->balance = 0;
        node->left = NULL;
        node->right = NULL;
        node->parent = parent;
    }

    return node;
}

//frees all nodes
void node_destroy(TLDNode *node)
{
    if (node != NULL)
    {
        node_destroy(node->left);
        node_destroy(node->right);

        //gets rid of data leaks
        free(node->content);
        free(node);
    }
    return;
}

/*
 * tldlist_destroy destroys the list structure in `tld'
 *
 * all heap allocated storage associated with the list is returned to the heap
 */

void tldlist_destroy(TLDList *tld)
{
    node_destroy(tld->root);
    free(tld);
}

/*
 * tldlist_count returns the number of successful tldlist_add() calls since
 * the creation of the TLDList
 */
long tldlist_count(TLDList *tld)
{
    return tld->countList;
};

/*
 * tldnode_tldname returns the tld associated with the TLDNode
 */
char *tldnode_tldname(TLDNode *node)
{
    return node->content;
};

/*
 * tldnode_count returns the number of times that a log entry for the
 * corresponding tld was added to the list
 */
long tldnode_count(TLDNode *node)
{
    return node->countNode;
};

/*
 * tldlist_add adds the TLD contained in `hostname' to the tldlist if
 * `d' falls in the begin and end dates associated with the list;
 * returns 1 if the entry was counted, 0 if not
 */
int tldlist_add(TLDList *tld, char *hostname, Date *d)
{
    //compares the dates of input to determine if the node will be added
    if ((date_compare(d, tld->begin) >= 0) && (date_compare(tld->end, d) >= 0))
    {
        //strip the hostname
        char *host = tldstrip(strrchr(hostname, '.') + 1); //+1 to remove dot

        if (host == NULL)
        {
            free(host);
            return 0;
        }
        //List does not have a root, create a node and make it the root
        if (tld->root == NULL)
        {
            TLDNode *node = create_tldnode(host, NULL);

            if (node != NULL)
            {
                tld->root = node;
                tld->countList = 1;
                return 1;
            }
            return 0;
        }
        //If the list has a root, search the tree for the hostname and add it
        if (tld_insert(tld->root, host, tld))
        {
            tld->countList += 1;
            return 1;
        }
    }
    return 0;
};

//recursive method to insert hostname into binary tree tld
int tld_insert(TLDNode *node, char *hostname, TLDList *tld)
{

    //compares the hostname and the hostname on the current node
    int value = strcmp(hostname, node->content);

    //if the hostname is smaller than the hostname on the current node go left
    if (value < 0)
    {
        //If the leaf is empty create a new node
        if (node->left == NULL)
        {
            TLDNode *add_node = create_tldnode(hostname, node);

            if (add_node != NULL)
            {
                node->left = add_node;
                rebalance(node, tld);
                return 1;
            }
            return 0;
        }
        //to prevent segv error
        else
        {
            tld_insert(node->left, hostname, tld);
            return 1;
        }
    }
    //if the hostname is bigger than the hostname on the current node go right

    if (value > 0)
    {

        //If the leaf is empty create a new node
        if (node->right == NULL)
        {
            TLDNode *add_node = create_tldnode(hostname, node);

            if (add_node != NULL)
            {
                node->right = add_node;
                rebalance(node, tld);
                return 1;
            }
            return 0;
        }
        //to prevent seg error
        else
        {
            tld_insert(node->right, hostname, tld);
            return 1;
        }
    }
    //when the tld node already exists
    else
    {
        node->countNode += 1;
        free(hostname);
        return 1;
    }

    return 0;
}