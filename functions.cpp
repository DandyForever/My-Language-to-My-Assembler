#include "Tree_t.h"

FILE* LOG_txt = nullptr;
#define DEBUG_

#define FUNC_START fprintf (LOG_txt, "%s started\n", __func__);
#define FUNC_END fprintf (LOG_txt, "%s ended successfully\n\n", __func__);
#define CHECK_NODE(node) if (!NodeOK (node))\
    {\
        fprintf (LOG_txt, "BAD POINTER [%p]\n", node);\
        assert (0);\
    }
#define CHECK_SIZE if (size >= MaxTree)\
    {\
        fprintf (LOG_txt, "NOT ENOUGH MEMORY TO PUSH LEFT\n");\
        assert (0);\
    }

#define assert(condition)                                                   \
        if (!condition)                                                     \
        {                                                                   \
        printf("ASSERTION %s in %s: function %s (%d)\n", #condition, __FILE__, __func__, __LINE__);\
        fclose (LOG_txt);                                                   \
        abort();                                                            \
        }

#ifdef DEBUG_
    #define DEBUG
#else
    #define DEBUG if (0)
#endif // DEBUG

#ifdef DEBUG_
    #define ASSERT_TREE(); \
            DEBUG fprintf (LOG_txt, "asserting...\n");\
            if (!TreeOK (first_node))\
            {                 \
            Dump();    \
            assert(0);        \
            }
#else
    #define ASSERT_TREE();
#endif

Tree_t::Tree_t ():
    first_node (nullptr),
    size (0)

    {
        LOG_txt = fopen (LOGf, "w");

        ASSERT_TREE()

        DEBUG fprintf (LOG_txt, "constructed successfully\n\n");
    }

Tree_t::~Tree_t ()
{
    DEBUG fprintf (LOG_txt, "destructing...\n");

    ASSERT_TREE()

    if (size > 0)
        DeleteNode (first_node);

    DEBUG fprintf (LOG_txt, "destructed successfully\n\n");

    fclose (LOG_txt);
}

void Tree_t::DeleteNode (Node* current)
{
    DEBUG FUNC_START

    ASSERT_TREE()

    CHECK_NODE(current)

    if (current->left)
    {
        Node* cur_cop = current->left;

        current->left = nullptr;

        DeleteNode (cur_cop);
    }

    if (current->right)
    {
        Node* cur_cop = current->right;

        current->right = nullptr;

        DeleteNode (cur_cop);
    }

    while (current)
    {
        delete current;
        current = nullptr;
    }

    size--;

    ASSERT_TREE()

    DEBUG FUNC_END
}

void Tree_t::PushLeft (Node* node, const TreeKey_t key, const TreeVal_t value)
{
    DEBUG FUNC_START

    ASSERT_TREE()

    if (size == 0)
    {
        PushFirst (key, value);
        return;
    }

    CHECK_NODE(node)

    CHECK_SIZE

    if (node->left)
    {
        Node* copynode = node->left;
        assert(copynode);
        node->left = nullptr;

        DeleteNode (copynode);
    }

    Node* node_left = new Node;

    node_left->value = value;
    node_left->key   = key;
    node_left->first = first_node;

    node_left->left = nullptr;
    node_left->right = nullptr;

    node->left = node_left;
    size++;

    ASSERT_TREE()

    DEBUG FUNC_END
}

void Tree_t::PushLeft (Node* node, Node* node_left)
{
    DEBUG FUNC_START
    ASSERT_TREE()

    if (!node)
    {
        PushFirst (node_left);
        return;
    }

    CHECK_NODE(node)

    CHECK_SIZE

    if (node->left)
    {
        Node* copynode = node->left;
        assert(copynode);
        node->left = nullptr;

        DeleteNode (copynode);
    }

    node->left = node_left;

    size += NodeSize (node_left);

    ASSERT_TREE()
    DEBUG FUNC_END
}

void Tree_t::PushRight (Node* node, const TreeKey_t key, const TreeVal_t value)
{
    DEBUG FUNC_START

    ASSERT_TREE()

    if (size == 0)
    {
        PushFirst (key, value);
        return;
    }

    CHECK_NODE(node)

    CHECK_SIZE

    if (node->right)
    {
        Node* copynode = node->right;
        assert(copynode);
        node->right = nullptr;

        DeleteNode (copynode);
    }

    Node* node_right = new Node;

    node_right->value = value;
    node_right->key   = key;
    node_right->first = first_node;

    node_right->left = nullptr;
    node_right->right = nullptr;

    node->right = node_right;
    size++;

    ASSERT_TREE()

    DEBUG FUNC_END
}

void Tree_t::PushRight (Node* node, Node* node_right)
{
    DEBUG FUNC_START
    ASSERT_TREE()

    if (!node)
    {
        PushFirst (node_right);
        return;
    }

    CHECK_NODE(node)
    //CHECK_NODE(node_right)

    CHECK_SIZE

    if (node->right)
    {
        Node* copynode = node->right;
        assert(copynode);
        node->right = nullptr;

        DeleteNode (copynode);
    }

    node->right = node_right;

    size += NodeSize (node_right);

    ASSERT_TREE()
    DEBUG FUNC_END
}

bool Tree_t::NodeOK (const Node* node)
{
    DEBUG fprintf (LOG_txt, "CHECKING NODE\n");

    if (size == 0)
        return 1;

    //node->first = first_node;

    /*if (node->key == VAR && value != POISON_VAR)
        return 0;

    if (node->key == NUM && (value == POISON_VAR || value == POISON_OPERATOR))
        return 0;

    if (node->key == OPERATOR && value != POISON_OPERATOR)
        return 0;*/

    return node && node->first == first_node && node->canaryleft == Crashcan1 && node->canaryright == Crashcan2;
}

bool Tree_t::TreeOK (const Node* current)
{
    if (size == 0)
        return 1;

    if (current == first_node)
        return this && first_node && size >= 0 && canaryleft == Crashcan1 && canaryright == Crashcan2 &&
                NodeOK (current) && TreeOK (current->left) && TreeOK (current->right);

    else
        return NodeOK (current) && TreeOK (current->left) && TreeOK (current->right) || !current;
}

void Tree_t::Dump ()
{
    DEBUG FUNC_START

    FILE* output_file = fopen ("Dump.dot", "w");
    assert (output_file);

    fprintf (output_file, "digraph Tree\n{\nrankdir=TB\n");
    fprintf (output_file, "\t\tnode [ shape = \"box\", color = \"black\" ]\n");
    fprintf (output_file, "\t\tedge [ color = \"black\" ]\n\n");

    if (canaryleft != Crashcan1)
        fprintf (output_file, "\tCanaryLEFT [ label = \"CanaryLEFT = %d\\nBUT EXPECTED %d\","
                 "color = \"red\" ]\n", canaryleft, Crashcan1);
    else
        fprintf (output_file, "\tCanaryLEFT [ label = \"CanaryLEFT = %d\" ]\n", canaryleft);

    if (canaryright != Crashcan2)
        fprintf (output_file, "\tCanaryRIGHT [ label = \"CanaryRIGHT = %d\\nBUT EXPECTED %d\","
                 "color = \"red\" ]\n", canaryright, Crashcan2);
    else
        fprintf (output_file, "\tCanaryRIGHT [ label = \"CanaryRIGHT = %d\" ]\n", canaryright);

    if (size >= 0)
        fprintf (output_file, "\tsize [ label = \"size = %d\" ]\n\n", size);
    else
        fprintf (output_file, "\tsize [ label = \"size = %d\\n<0\", color = \"red\"]\n\n", size);

    int number = 1;

    if (size > 0)
        NodeDumps (output_file, first_node, 0, &number, "none");

    if (number != size)
        fprintf (output_file, "\tsize [color = \"red\" ]");

    fprintf (output_file, "}");
    fclose (output_file);


    system ("dot -Tpng Dump.dot -o Dump.png");

    DEBUG FUNC_END
}

void Tree_t::NodeDumps (FILE* output_file, const Node* cur, const int parent, int* number, const char* side)
{
    const int number_copy = *number;
    fprintf (output_file, "subgraph clust%d{\nrankdir = HR;\n", *number);

    fprintf (output_file, "\telem%d [ shape = \"record\", label = \"{ <pointer> %d\\n%p|"
             "{ canaryLEFT\\n%d | canaryRIGHT\\n%d } |  { key\\n%d | value\\n "
             , *number, *number, cur, cur->canaryleft, cur->canaryright, cur->key);

    bool isnormal = dump (cur->value, output_file) && NodeOK (cur);

    fprintf (output_file, " | <first> first\\n%p } | { <left> left\\n%p | <right> right\\n%p } }\", color = "
             , cur->first, cur->left, cur->right);

    if (isnormal)
        fprintf (output_file, "\"blue\" ]\n");
    else
        fprintf (output_file, "\"red\" ]\n");

    fprintf (output_file, "}\n\n");

    if (!strcmp (side, "left"))
    {
        fprintf (output_file, "\t\telem%d:<left> -> elem%d\n", parent, *number);
    }
    else if (!strcmp (side, "right"))
    {
        fprintf (output_file, "\t\telem%d:<right> -> elem%d\n", parent, *number);
    }

    //fprintf (output_file, "\t\telem%d:<first> -> elem1:<pointer>\n", *number);

    if (cur->left)
    {
        (*number)++;
        NodeDumps (output_file, cur->left, number_copy, number, "left");
    }

    if (cur->right)
    {
        (*number)++;
        NodeDumps (output_file, cur->right, number_copy, number, "right");
    }
}

void Tree_t::PushFirst (const TreeKey_t key, const TreeVal_t value)
{
    DEBUG FUNC_START

    ASSERT_TREE()

    if (size)
    {
        Node* copynode = first_node;
        assert (copynode);
        first_node = 0;

        DeleteNode (copynode);
    }

    Node* first = new Node;

    first->first = first;

    first->left = nullptr;
    first->right = nullptr;

    first->key = key;
    first->value = value;

    first_node = first;

    size++;

    ASSERT_TREE()

    DEBUG FUNC_END
}

void Tree_t::PushFirst (Node* node)
{
    DEBUG FUNC_START
    ASSERT_TREE()

    if (size)
    {
        DeleteNode (first_node);
    }

    first_node = node;

    size = NodeSize (node);

    ASSERT_TREE()
    DEBUG FUNC_END
}

Node* Tree_t::First ()
{
    DEBUG FUNC_START

    ASSERT_TREE()

    if (!NodeOK (first_node))
    {
        fprintf (LOG_txt, "EMPTY TREE\n");
        assert (0);
    }

    DEBUG FUNC_END

    return first_node;
}


int Tree_t::Size ()
{
    DEBUG FUNC_START
    ASSERT_TREE()
    DEBUG FUNC_END

    return size;
}

bool Tree_t::dump (const int value, FILE* output_file)
{
    fprintf (output_file, "%d ", value);

    return 1;
}

bool Tree_t::dump (const double value, FILE* output_file)
{
    if (isnan (value))
    {
        fprintf (output_file, "%f\\nnot a number ", value);

        return 0;
    }

    else
    {
        fprintf (output_file, " %f ", value);

        return 1;
    }
}

bool Tree_t::dump (const void* value, FILE* output_file)
{
    TreeVal_t* val = (TreeVal_t*) value;

    if (val == nullptr)
    {
        fprintf (output_file, " %p\\nzero pointer ", val);

        return 0;
    }

    else
    {
        fprintf (output_file, " %p ", val);

        return 1;
    }
}

Node* Tree_t::CopyNode (const Node* current)
{
    DEBUG FUNC_START
    ASSERT_TREE()

    Node* copynode = new Node;

    copynode->first = first_node;

    copynode->key = current->key;
    copynode->value = current->value;

    if (current->left != nullptr)
        copynode->left = CopyNode (current->left);

    else
        copynode->left = nullptr;

    if (current->right != nullptr)
        copynode->right = CopyNode (current->right);

    else
        copynode->right = nullptr;

    DEBUG FUNC_END

    return copynode;
}

int Tree_t::NodeSize (Node* current)
{
    DEBUG FUNC_START
    DEBUG FUNC_END

    if (!current)
        return 0;

    else
    {
        current->first = first_node;
        return 1 + NodeSize (current->left) + NodeSize (current->right);
    }
}
