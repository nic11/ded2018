class ArmyStack {
public:
    ArmyStack(int cap=MIN_CAPACITY, int canariesVal=0xBAD7DED);
    ~ArmyStack();

    int size() const;
    int top() const;
    int pop();
    void push(int x);

private:
    const static int MIN_CAPACITY = 8;
    const static int GROW_FACTOR = 2;
    const static int MIN_INVERSE_FULLNESS = 3;

    struct Element {
        int mCanary;
        int mData;
        mutable int mHash;

        int calcHash() const;
        bool softCheck(int canary) const;
        void updateHash();
    };

    int calcHash() const;
    bool softCheck() const;
    void check() const;
    void checkAt(int i) const;
    void realloc(int cap);
    int topIndex() const;
    void updateHash();
    void crashAndDump(const char* reason) const;

    Element *mBuf;
    int mCapacity;
    int mSize;
    int mCanariesVal;
    mutable int mHash;
};
