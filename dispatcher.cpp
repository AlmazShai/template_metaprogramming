#include <iostream>
#include <utility>
#include <type_traits>

// Example handler functions
void handleSymbol()   { std::cout << "Handling Symbol (tag 55)\n"; }
void handleOrderQty() { std::cout << "Handling OrderQty (tag 38)\n"; }
void handlePrice()    { std::cout << "Handling Price (tag 44)\n"; }

// =====================
// Dispatcher definition
// =====================

// Generic template for dispatcher
template <typename... Pairs>
struct Dispatcher {
    // - checks if tag matches one of the Pairs
    // - calls the corresponding handler
    // - does nothing (or static_assert) if tag is not found
    void operator()(int tag) const {
        ((tag == Pairs::tag ? (Pairs::func(), true) : false) || ...);
    }
};

// Helper alias to create <tag, handler> pairs easily
template <int Tag, auto Func>
struct Handler {
    static constexpr int tag = Tag;
    static constexpr auto func = Func;
};

// Helper function to build dispatcher
template <typename... Pairs>
constexpr auto make_dispatcher() {
    return Dispatcher<Pairs...>{};
}

// =====================
// Example usage
// =====================
int main() {
    // Create dispatcher with known handlers
    constexpr auto dispatcher = Dispatcher<
        Handler<55, handleSymbol>,
        Handler<38, handleOrderQty>,
        Handler<44, handlePrice>
    >{};

    dispatcher(55); // should call handleSymbol
    dispatcher(38); // should call handleOrderQty
    dispatcher(44); // should call handlePrice
    dispatcher(99); // TODO: what should happen here?
}
