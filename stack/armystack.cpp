#include <iostream>
#include <cassert>
#include <cstdio>
#include <cstring>
#include <algorithm>

#include "armystack.h"

int calcHashBasic(const void* obj, int size) {
    assert(!(size & 3)); // divisible by 4
    size >>= 2;

    const int* a = reinterpret_cast<const int*>(obj);
    int ans = 0;
    for (int i = 0; i < size; ++i) {
        ans ^= a[i];
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

bool ArmyStack::Element::softCheck(int canary) const{
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

bool ArmyStack::softCheck() const {
    return mHash == calcHash();
}

void ArmyStack::check() const {
    if (!softCheck()) {
        crashAndDump("header checksum mismatch");
    }
}

void ArmyStack::checkAt(int i) const {
    check();
    if (!mBuf[i].softCheck(mCanariesVal)) {
        crashAndDump("data corrupt");
    }
}

void ArmyStack::realloc(int cap) {
    assert(cap >= mSize);
    check();

    Element *newBuf = new Element[cap];

    if (mBuf) {
        for (int i = 0; i < mSize; ++i) {
            assert(mBuf[i].softCheck(mCanariesVal));
            newBuf[i] = mBuf[i];
        }
        for (int i = mCapacity; i < cap; ++i) {
            newBuf[i].mCanary = mCanariesVal;
            newBuf[i].mData = 0;
            newBuf[i].updateHash();
            assert(newBuf[i].softCheck(mCanariesVal));
        }
        delete[] mBuf;
    }

    mCapacity = cap;
    mBuf = newBuf;
    updateHash();

    check();
}

int ArmyStack::topIndex() const {
    assert(size());
    return mSize - 1;
}

int ArmyStack::size() const {
    check();
    return mSize;
}

int ArmyStack::top() const {
    check();
    assert(size());
    const Element& el = mBuf[topIndex()];
    assert(el.softCheck(mCanariesVal));
    return el.mData;
}

void ArmyStack::push(int x) {
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

    assert(el.softCheck(mCanariesVal));
    check();
}

int ArmyStack::pop() {
    check();

    int x = top();
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
    for (int i = 0; i < mSize; ++i) {
        checkAt(i);
    }
}

#define eprintf(...) fprintf (stderr, __VA_ARGS__)

void ArmyStack::crashAndDump(const char* reason) const {
    eprintf("ArmyStack is corrupt!\nMessage: %s\nDump follows:\n", reason);
    eprintf("ArmyStack@%p:\n", this);
    eprintf("  mCapacity: %d\n", mCapacity);
    eprintf("  mSize: %d\n", mSize);
    eprintf("  mCanariesVal: %d\n", mCanariesVal);
    if (softCheck()) {
        eprintf("  mHash: %d\n", mHash);
        eprintf("  mBuf: %p\n", mBuf);
        char tmp[30];
        for (int i = 0; i < mCapacity; ++i) {
            sprintf(tmp, "    [%d]", i);
            eprintf("%s mCanary: %d\n", tmp, mBuf[i].mCanary);
            std::fill(tmp, tmp + strlen(tmp), ' ');
            eprintf("%s mData: %d\n", tmp, mBuf[i].mData);
            if (mBuf[i].softCheck(mCanariesVal)) {
                eprintf("%s mHash: %d\n", tmp, mBuf[i].mHash);
            } else {
                int hash = mBuf[i].mHash;
                eprintf("%s mHash: %d [expected %d]\n", tmp, hash, mBuf[i].calcHash());
            }
        }
    } else {
        int hash = mHash;
        eprintf("  mHash: %d [expected %d]\n", hash, calcHash());
        eprintf("  mBuf: %p\n", mBuf);
    }
    eprintf("Dump end\n");
    exit(228);
}
