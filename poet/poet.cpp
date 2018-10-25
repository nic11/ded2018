#include <cstdio>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <cassert>
#include <string>
#include <vector>
#include <random>

struct Line {
    const char* ptr;
    int len;

    Line(): ptr(nullptr), len(0) {}

    Line(const char *ptr_, int len_): ptr(ptr_), len(len_) {}
};

int nextLetter(const char* s, int i, int lim, int step) {
    assert(step);

    for (i += step; step > 0? i < lim : i > lim; i += step) {
        if (('A' <= s[i] && s[i] <= 'Z') || ('a' <= s[i] && s[i] <= 'z')) {
            break;
        }
    }

    return i;
}

struct LineComparator {
    bool reverse;

    LineComparator(bool reverse_): reverse(reverse_) {}

    bool operator()(const Line &a, const Line &b) const {
        int i, j, step, lima, limb;
        if (!reverse) {
            i = -1;
            j = -1;
            step = 1;
            lima = a.len;
            limb = b.len;
        } else {
            i = a.len;
            j = b.len;
            step = -1;
            lima = -1;
            limb = -1;
        }

        i = nextLetter(a.ptr, i, lima, step);
        j = nextLetter(b.ptr, j, limb, step);
        while (0 <= i && i < a.len && 0 <= j && j < b.len) {
            if (a.ptr[i] != b.ptr[j]) {
                return a.ptr[i] < b.ptr[j];
            }
            i = nextLetter(a.ptr, i, lima, step);
            j = nextLetter(b.ptr, j, limb, step);
        }

        return (i < 0 || i >= a.len) && (0 <= j && j < b.len);
    }
};

char* readFile(const char* filename) {
    FILE* f = fopen(filename, "r");
    assert(f);
    fseek(f, 0, SEEK_END);
    int size = ftell(f);
    fseek(f, 0, SEEK_SET);
    char* buf = new char[size + 1];
    fread(buf, size + 1, size, f);
    fclose(f);
    return buf;
}

std::vector<Line> getLines(char* buf) {
    int num = 0;
    for (int i = 0; buf[i]; ++i) {
        num += buf[i] == '\n';
    }

    std::vector<Line> lines;
    lines.reserve(num);
    int from = 0;
    for (int i = 0; buf[i]; ++i) {
        if (buf[i] == '\n') {
            lines.emplace_back(buf + from, i - from);
            from = i + 1;
            buf[i] = 0;
        }
    }
    return lines;
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <input>\n";
        return 1;
    }

    char* buf = readFile(argv[1]);
    const std::vector<Line> lines = getLines(buf);
    // for (const Line& line : lines) {
    //     std::cout << "LINE: ";
    //     std::cout << line.ptr;
    //     // for (int i = 0; i < line.len; ++i) {
    //     //     std::cout << line.ptr[i];
    //     // }
    //     std::cout << "\n";
    // }

    std::vector<Line> lines2 = lines;

    std::sort(lines2.begin(), lines2.end(), LineComparator(true));

    int pairs = lines2.size() / 2;
    std::vector<int> seq;
    seq.reserve(pairs);
    for (int i = 0; i < pairs; ++i) {
        seq.push_back(i);
    }
    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(seq.begin(), seq.end(), gen);

    for (int i = 0; i < pairs / 2; ++i) {
        Line quatrain[4];
        for (int j1 = 0; j1 < 2; ++j1) {
            for (int j2 = 0; j2 < 2; ++j2) {
                quatrain[j1 * 2 + j2] = lines2[2 * seq[2 * i + j1] + j2];
            }
        }

        switch (rand() % 3) {
        case 0: // AABB
            break;
        case 1: // ABAB
            std::swap(quatrain[1], quatrain[2]);
            break;
        case 2: // ABBA
            std::swap(quatrain[1], quatrain[3]);
            break;
        }

        for (int j = 0; j < 4; ++j) {
            std::cout << quatrain[j].ptr << "\n";
        }
        std::cout << "\n";
    }

    delete[] buf;
    return 0;
}