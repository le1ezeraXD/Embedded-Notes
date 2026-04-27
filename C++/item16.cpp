#include<iostream>
#include<vector>
#include<algorithm>
#include<mutex>
#include<atomic>
#include<math.h>

class Polynomial {
public:
    using RootsType = std::vector<double>;
    RootsType roots() const {
        std::lock_guard<std::mutex> g(m);
        if (!rootsAreValid) {
            rootsAreValid = true;
        }
        return rootVals;
    }
private:
    mutable std::mutex m;
    mutable bool rootsAreValid {false};
    mutable RootsType   rootVals{};
};

class Point {
private:
    mutable std::atomic<unsigned> callCount{0};
    double x, y;
public:
    double distanceFromOrigin() const noexcept {
        ++callCount;

        return std::sqrt((x*x)+(y*y));
    }
};

int main(void) {
    Polynomial p;

}