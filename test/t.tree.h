#ifndef _T_TREE_H_
#define _T_TREE_H_
#include <algorithm>
#include <iostream>
#include <mimetic/tree.h>
#include "cutee.h"


namespace mimetic 
{

struct TEST_CLASS( tree )
{
    typedef TreeNode<char> Node;
    void TEST_FUNCTION( buildTree )
    {
        const char* words[] = {
              "pro", "pra", "tre", 0
        };

        // creates a tree of reversed wordlist (*words)
        for(int i = 0; words[i]; ++i)
        {
            Node::NodeList *pChilds = &m_tree.childList();
            Node::NodeList::iterator it = pChilds->begin();
            const char *w = words[i];
            int depth = 0;
            do
            {
                it = find_if(pChilds->begin(), pChilds->end(), 
                        FindNodePred<char>(*w));
                if( it == pChilds->end() )
                {
                    it = pChilds->insert(pChilds->end(),*w);
                    TEST_ASSERT(pChilds->size() > 0);
                } 
                pChilds = &it->childList();
                depth++;
            } while(*++w);
        }
    }
    void TEST_FUNCTION( search )
    {
        // boyer-moore algorithm modified to work with 
        // multiple patterns(i.e. boundaries)

        #if 0
        // tlen: text len
        // mlen: pattern len
        // text: input text
        // muster: patter
        int i, j, skip[256];

        for(i=0; i<256; ++i)  
            skip[i]=mlen;
        for(i=0; i<mlen; ++i) 
            skip[muster[i]]=mlen-1-i;

        for(i=j=mlen-1; j>=0; --i, --j)
            while(text[i] != muster[j]) 
            {
            i += max(mlen-j,skip[text[i]]);
            if(i >= tlen) 
                return -1;
            j = mlen-1;
            }
        return i;
        #endif
    }
private:
    Node m_tree;
};

}


#endif
