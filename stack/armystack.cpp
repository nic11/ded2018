#include <iostream>
#include <cassert>
#include <cstdio>
#include <cstring>
#include <algorithm>

#include "armystack.h"

int calcHashBasic(const void* obj, int size) {
    assert(size % sizeof(int) == 0);
    size >>= 2;

    const int* a = reinterpret_cast<const int*>(obj);
    int ans = 0;
    for (int i = 0; i < size; ++i) {
        ans ^= a[i] * (i + 1);
    }
    return ans;
}

int ArmyStack::Element::calcHash() const {
    int oldHash = mHash;
    mHash = 0;
    int factHash = calcHashBasic(this, sizeof(ArmyStack::Element));
    mHash = oldHash;
    return factHash;
}

bool ArmyStack::Element::silentCheck(int canary) const{
    return mHash == calcHash() && mCanary == canary;
}

void ArmyStack::Element::updateHash() {
    mHash = calcHash();
}

int ArmyStack::calcHash() const {
    int oldHash = mHash;
    mHash = 0;
    int factHash = calcHashBasic(this, sizeof(ArmyStack));
    mHash = oldHash;
    return factHash;
}

constexpr const char* ArmyStack::Errors::getMessage(int code) {
    switch (code) {
    case OK:
        return "OK";
    case BAD_HEADER:
        return "header hash/canary mismatch";
    case BAD_DATA:
        return "data hash/canary mismatch";
    case BAD_SIZE:
        return "bad mSize, mCapacity and/or their relationship";
    case POP_WHEN_EMPTY:
        return "trying to pop or access top of an empty stack";
    default:
        return "UNKNOWN ERROR";
    }
}

#define CHECK_FAIL(code) \
    if (silent || code == Errors::OK) { \
        return code; \
    } else { \
        crashAndDump(Errors::getMessage(code)); \
    }

int ArmyStack::check(bool silent) const {
    if (mSize < 0 || mCapacity < 0 || mSize > mCapacity) {
        CHECK_FAIL(Errors::BAD_SIZE);
    }
    if (mHash != calcHash()) {
        CHECK_FAIL(Errors::BAD_HEADER);
    }
    for (int i = 0; i < mSize; ++i) {
        if (!silentCheckAt(i)) {
            CHECK_FAIL(Errors::BAD_DATA);
        }
    }
    return Errors::OK;
}

bool ArmyStack::silentCheckAt(int i) const {
    if (!mBuf[i].silentCheck(mCanariesVal)) {
        return false;
    }
    return true;
}

void ArmyStack::realloc(int cap) {
    check();

    Element *newBuf = new Element[cap];

    if (mBuf) {
        for (int i = 0; i < mSize; ++i) {
            assert(mBuf[i].silentCheck(mCanariesVal));
            newBuf[i] = mBuf[i];
        }
        for (int i = mCapacity; i < cap; ++i) {
            newBuf[i].mCanary = mCanariesVal;
            newBuf[i].mData = 0;
            newBuf[i].updateHash();
            assert(newBuf[i].silentCheck(mCanariesVal));
        }
        delete[] mBuf;
    }

    mCapacity = cap;
    mBuf = newBuf;
    updateHash();

    check();
}

int ArmyStack::topIndex() const {
    if (size() == 0) {
        crashAndDump(Errors::getMessage(Errors::POP_WHEN_EMPTY));
    }
    return mSize - 1;
}

int ArmyStack::size() const {
    return mSize;
}

ArmyStack::Type ArmyStack::top() const {
    check();
    const Element& el = mBuf[topIndex()];
    assert(el.silentCheck(mCanariesVal));
    return el.mData;
}

void ArmyStack::push(Type x) {
    check();

    if (mSize == mCapacity) {
        realloc(mCapacity * GROW_FACTOR);
    }
    
    ++mSize;
    updateHash();

    Element& el = mBuf[topIndex()];
    el.mCanary = mCanariesVal;
    el.mData = x;
    el.updateHash();

    assert(el.silentCheck(mCanariesVal));
    check();
}

ArmyStack::Type ArmyStack::pop() {
    check();

    Type x = top();
    --mSize;
    updateHash();

    if (mSize * GROW_FACTOR >= MIN_CAPACITY && mSize * MIN_INVERSE_FULLNESS < mCapacity) {
        realloc(mSize * GROW_FACTOR);
    }

    return x;
}

void ArmyStack::updateHash() {
    mHash = calcHash();
}

ArmyStack::ArmyStack(int cap, int canariesVal):
    mBuf(nullptr),
    mCapacity(0),
    mSize(0),
    mCanariesVal(canariesVal),
    mHash(0)
{
    updateHash();
    realloc(cap);
}

ArmyStack::~ArmyStack() {
    check();
}

#define eprintf(...) fprintf (stderr, __VA_ARGS__)

void ArmyStack::crashAndDump(const char* reason) const {
    eprintf("ArmyStack is corrupt or bad operation is applied!\nMessage: %s\nDump follows:\n", reason);
    eprintf("ArmyStack@%p:\n", this);
    eprintf("  mCapacity: %d\n", mCapacity);
    eprintf("  mSize: %d\n", mSize);
    eprintf("  mCanariesVal: %d\n", mCanariesVal);

    int hashStored = mHash;
    int hashReal = calcHash();
    if (hashStored == hashReal) {
        eprintf("  mHash: %d\n", hashStored);
        eprintf("  mBuf: %p\n", mBuf);

        char tmp[30] = "";
        for (int i = 0; i < mCapacity; ++i) {
            int len = sprintf(tmp, "    [%d]", i);

            eprintf("%s mCanary: %d\n", tmp, mBuf[i].mCanary);
            std::fill(tmp, tmp + len, ' ');
            eprintf("%s mData: %lld\n", tmp, mBuf[i].mData);

            if (mBuf[i].silentCheck(mCanariesVal)) {
                eprintf("%s mHash: %d\n", tmp, mBuf[i].mHash);
            } else {
                int hash = mBuf[i].mHash;
                eprintf("%s mHash: %d [expected %d]\n", tmp, hash, mBuf[i].calcHash());
            }
        }
    } else {
        eprintf("  mHash: %d [expected %d]\n", hashStored, hashReal);
        eprintf("  mBuf: %p\n", mBuf);
    }

    eprintf("Dump end\n");
    abort();
}
