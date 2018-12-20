class ArmyStack {
public:
    typedef long long Type;

    ArmyStack(int cap=MIN_CAPACITY, int canariesVal=0xBAD7DED);
    ~ArmyStack();

    int size() const;
    Type top() const;
    Type pop();
    void push(Type x);

    int check(bool silent = false) const;

    struct Errors {
        enum ErrorsEnum {
            OK,
            BAD_HEADER,
            BAD_DATA,
            BAD_SIZE,
            POP_WHEN_EMPTY
        };

        static constexpr const char* getMessage(int code);
    };

// private:
    const static int MIN_CAPACITY = 8;
    const static int GROW_FACTOR = 2;
    const static int MIN_INVERSE_FULLNESS = 3;

    struct Element {
        int mCanary;
        Type mData;
        mutable int mHash;

        int calcHash() const;
        bool silentCheck(int canary) const;
        void updateHash();
    };

    int calcHash() const;
    bool silentCheckAt(int i) const;
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
