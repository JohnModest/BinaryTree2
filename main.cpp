#include <iostream>
#include <memory>

using namespace std;

////////////////////////////////////////////////
// Variant 1
////////////////////////////////////////////////
class CTree
{
public:
    struct TNode
    {
        TNode(int value) : nValue(value), pLeft(nullptr), pRight(nullptr) {}
        ~TNode() {
            //if (pLeft)
            //    delete pLeft;
            //if (pRight)
            //    delete pRight;
        }
        int nValue;
        TNode* pLeft;
        TNode* pRight;
    };

    CTree() {
        pNodeRoot_ = nullptr;
    }

    ~CTree() {
        _clear(pNodeRoot_);
        pNodeRoot_ = nullptr;
    }

    const TNode* addElement(int nValue) {
        return (pNodeRoot_ = _addElement(nValue, pNodeRoot_));
    }

    void delElement(int nValue) {
        _delElement(nValue, pNodeRoot_);
    }

    const TNode* const findElement(int nValue) {
        return _findElement(nValue, pNodeRoot_);
    }

    void printTree() {
        cout << "root" << endl;
        _printTree(pNodeRoot_);
    }


protected:
    void _printTree(TNode* pNode) {
        if (pNode == nullptr)
            return;

        cout << pNode->nValue << endl;
        if (pNode->pLeft)
        {
            cout << "left" << endl;
            _printTree(pNode->pLeft);
        }
        if (pNode->pRight)
        {
            cout << "right" << endl;
            _printTree(pNode->pRight);
        }
    }

    TNode* _addElement(int nValue, TNode* pNode)
    {
        if (pNode == nullptr)
            return (pNode = new TNode(nValue));

        if (nValue == pNode->nValue)
            return pNode;

        if (nValue < pNode->nValue)
            pNode->pLeft = _addElement(nValue, pNode->pLeft);
        else
            pNode->pRight = _addElement(nValue, pNode->pRight);
        return pNode;
    }

    TNode* _delElement(int nValue, TNode* pNode) {
        if (pNode == nullptr)
            return pNode;

        if (nValue == pNode->nValue)
        {   TNode* pNew;
            if (pNode->pRight == nullptr)
                pNew = pNode->pLeft;
            else
            {
                TNode* pTmp = pNode->pRight;
                if (pTmp->pLeft == nullptr)
                {
                    pTmp->pLeft = pNode->pLeft;
                    pNew = pTmp;
                }
                else
                {
                    TNode* pNodeMin = pTmp->pLeft;
                    while (pNodeMin->pLeft != nullptr)
                    {
                        pTmp = pNodeMin;
                        pNodeMin = pTmp->pLeft;
                    }
                    pTmp->pLeft = pNodeMin->pRight;
                    pNodeMin->pLeft = pNode->pLeft;
                    pNodeMin->pRight = pNode->pRight;
                    pNew = pNodeMin;
                }
            }

            if (pNodeRoot_ == pNode)
                pNodeRoot_ = pNew;

            delete pNode;
            return pNew;
        }
        else
        {
            if (nValue < pNode->nValue)
                pNode->pLeft = _delElement(nValue, pNode->pLeft);
            else
                pNode->pRight = _delElement(nValue, pNode->pRight);
        }
        return pNode;
    }

    const TNode* const _findElement(int nValue, TNode* pNode) {
        if (pNode == nullptr)
            return nullptr;
        if (pNode->nValue == nValue)
            return pNode;
        if (nValue < pNode->nValue)
            return _findElement(nValue, pNode->pLeft);
        return _findElement(nValue, pNode->pRight);
    }

    void _clear(TNode* pNode) {
        if (pNode != nullptr)
        {
            if (pNode->pLeft != nullptr)
                _clear(pNode->pLeft);
            if (pNode->pRight != nullptr)
                _clear(pNode->pRight);
            delete pNode;
        }
    }
    TNode* pNodeRoot_;
};
////////////////////////////////////////////////
////////////////////////////////////////////////


////////////////////////////////////////////////
// Variant 2
////////////////////////////////////////////////
template< class T >
class CTreeTempl
{
public:
    struct TNode
    {
        TNode(const T &value) : nValue(value),pLeft(nullptr), pRight(nullptr) {}
        ~TNode() {}
        T nValue;
        shared_ptr <TNode> pLeft;
        shared_ptr <TNode> pRight;
    };

    typedef int (*FuncCompare)(const T&, const T&);

    CTreeTempl(FuncCompare pFunc = nullptr) : pNodeRoot_(nullptr), pFunc_(pFunc) {
        if (pFunc == nullptr)
            pFunc_ = Compare;
    }

    ~CTreeTempl() {
        _clear(pNodeRoot_);
    }

    //
    // return : -1,0,1  -    < = >
    //
    static int Compare(const std::string& val1, const std::string& val2)
    {
        if (val1 < val2)
            return -1;
        if (val1 > val2)
            return 1;

        return !(val1 == val2);
    }


    //
    // return : -1,0,1  -    < = >
    //
    static int Compare(const int& val1, const int& val2)
    {
        if (val1 < val2)
            return -1;
        if (val1 > val2)
            return 1;

        return !(val1 == val2);
    }

    const weak_ptr <TNode> addElement(const T &nValue) {
        return _addElement(nValue, pNodeRoot_);
    }

    void delElement(const T &nValue) {
        _delElement(nValue, pNodeRoot_);
    }
    const weak_ptr <TNode> findElement(const T &nValue) {
        return _findElement(nValue, pNodeRoot_);
    }

    void printTree() {
        cout << "root" << endl;
        _printTree(pNodeRoot_);
    }

protected:
    void _printTree(const shared_ptr <TNode> &pNode) {
        if (pNode == nullptr)
            return;

        cout << pNode->nValue << endl;
        cout << "ref_count: " << pNode.use_count() << endl;
        if (pNode->pLeft)
        {
            cout << "left" << endl;
            _printTree(pNode->pLeft);
        }
        if (pNode->pRight)
        {
            cout << "right" << endl;
            _printTree(pNode->pRight);
        }
    }

    weak_ptr <TNode> _addElement(const T &nValue, shared_ptr <TNode> &pNode) {
        if (pNode == nullptr)
            return (pNode = make_shared<TNode>(nValue));

        if (pFunc_) // Extern compare
        {   int res = pFunc_(nValue, pNode->nValue);
            if (res == 0)
                return pNode;

            if (res < 0)
                //pNode->pLeft = _addElement(nValue, pNode->pLeft);
                _addElement(nValue, pNode->pLeft);
            else
                //pNode->pRight = _addElement(nValue, pNode->pRight);
                _addElement(nValue, pNode->pRight);
        }
        return pNode;
    }

    shared_ptr <TNode> _delElement(const T &nValue, shared_ptr <TNode> &pNode) {
        if (pNode == nullptr)
            return pNode;

        if (pFunc_) // Extern compare
        {
            int nRes = pFunc_(nValue, pNode->nValue);
            if (!nRes)
            {
                shared_ptr <TNode> pNew;
                if (pNode->pRight == nullptr)
                    pNew = pNode->pLeft;
                else
                {
                    shared_ptr <TNode> pTmp = pNode->pRight;
                    if (pTmp->pLeft == nullptr)
                    {
                        pTmp->pLeft = pNode->pLeft;
                        pNew = pTmp;
                    }
                    else
                    {
                        shared_ptr <TNode> pNodeMin = pTmp->pLeft;
                        while (pNodeMin->pLeft != nullptr)
                        {
                            pTmp = pNodeMin;
                            pNodeMin = pTmp->pLeft;
                        }
                        pTmp->pLeft = pNodeMin->pRight;
                        pNodeMin->pLeft = pNode->pLeft;
                        pNodeMin->pRight = pNode->pRight;
                        pNew = pNodeMin;
                    }
                }

                if (pNodeRoot_ == pNode)
                    pNodeRoot_ = pNew;

                pNode.reset();
                return pNew;
            }
            else
            {
                if (nRes < 0)
                    pNode->pLeft = _delElement(nValue, pNode->pLeft);
                else
                    pNode->pRight = _delElement(nValue, pNode->pRight);
            }
        }
        return pNode;
    }

    const weak_ptr <TNode> _findElement(const T &nValue, const shared_ptr <TNode> &pNode) {
        if (pNode == nullptr)
            return pNode;

        if (pFunc_) // Extern compare
        {
            int res = pFunc_(nValue, pNode->nValue);
            if (res == 0)
                return pNode;

            if (res < 0)
                return _findElement(nValue, pNode->pLeft);
        }
        else
        {
            if (pNode->nValue == nValue)
                return pNode;
            if (nValue < pNode->nValue)
                return _findElement(nValue, pNode->pLeft);
        }
        return _findElement(nValue, pNode->pRight);
    }

    void _clear(shared_ptr <TNode> &pNode) {
        if (pNode != nullptr)
        {
            if (pNode->pLeft != nullptr)
                _clear(pNode->pLeft);
            if (pNode->pRight != nullptr)
                _clear(pNode->pRight);
            pNode.reset();
        }
    }

    FuncCompare pFunc_;
    shared_ptr <TNode> pNodeRoot_;
};
////////////////////////////////////////////////
////////////////////////////////////////////////

int main()
{
    // Test

    cout << "Variant 1" << endl;
    ////////////////////////////////////////////////
    // Variant 1
    ////////////////////////////////////////////////
    CTree cTree;
    const CTree::TNode *pNode = cTree.addElement(6);
    cTree.addElement(17);
    cTree.addElement(10);
    cTree.addElement(5);
    cTree.addElement(7);
    cTree.addElement(15);

    const CTree::TNode* pNodeF = cTree.findElement(9);
    pNodeF = cTree.findElement(7);

    cTree.printTree();
    cout << " " << endl;
    ////////////////////////////////////////////////

    ///////////////////////////////////////////////////////////////
    // Variant 2 string
    ///////////////////////////////////////////////////////////////
    {

        auto Compare = [](const std::string& str1, const std::string& str2) {
            if (str1 < str2)
                return -1;
            if (str1 > str2)
                return 1;

            return int(!(str1 == str2));
        };

        cout << "Variant Template string" << endl;
        //CTreeTempl<std::string> cTreeTempl; // default Compare function
        //CTreeTempl<std::string> cTreeTempl(CTreeTempl<const std::string>::Compare); // delault Compare function
        CTreeTempl<std::string> cTreeTempl(Compare); // extern Compare function
        const weak_ptr <CTreeTempl<std::string>::TNode> pNode = cTreeTempl.addElement("a");
        cTreeTempl.addElement("b");
        cTreeTempl.addElement("18");
        cTreeTempl.addElement("11");
        cTreeTempl.addElement("6");
        cTreeTempl.addElement("8");
        cTreeTempl.addElement("16");

        cTreeTempl.printTree();
        cout << " " << endl;

        cTreeTempl.delElement("11");

        weak_ptr <CTreeTempl<std::string>::TNode> pNodeF = cTreeTempl.findElement("c");
        pNodeF = cTreeTempl.findElement("8");

        cTreeTempl.printTree();
        cout << " " << endl;
    }
    ////////////////////////////////////////////////

    ///////////////////////////////////////////////////////////////
    // Variant 2 int
    ///////////////////////////////////////////////////////////////
    {
        auto Compare = [](const int& val1, const int& val2) {
            if (val1 < val2)
                return -1;
            if (val1 > val2)
                return 1;

            return int(!(val1 == val2));
        };

        cout << "Variant Template int" << endl;
        //CTreeTempl<int> cTreeTempl; // default Compare function
        CTreeTempl<int> cTreeTempl(Compare); // extern Compare function
        const weak_ptr <CTreeTempl<int>::TNode> pNode = cTreeTempl.addElement(1);
        cTreeTempl.addElement(2);
        cTreeTempl.addElement(18);
        cTreeTempl.addElement(11);
        cTreeTempl.addElement(6);
        cTreeTempl.addElement(8);
        cTreeTempl.addElement(16);

        cTreeTempl.printTree();
        cout << " " << endl;

        cTreeTempl.delElement(11);

        weak_ptr <CTreeTempl<int>::TNode> pNodeF = cTreeTempl.findElement(3);
        pNodeF = cTreeTempl.findElement(8);

        cTreeTempl.printTree();
    }
    ////////////////////////////////////////////////

    cin.get();

    return 0;
}
