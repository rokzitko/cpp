#include <cassert>
#include <utility> // std::unreachable

int main() {
    assert(false && "Should never happen");
    std::unreachable();
}

