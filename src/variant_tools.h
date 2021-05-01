#ifndef VARIANT_TOOLS_H
#define VARIANT_TOOLS_H

#include <variant>

// From the Dacite source

template <class... Ts>
struct overloaded : Ts... {
    using Ts::operator()...;
};

///
/// To use:
/// \code
/// std::visit(
///     overloaded{
///         [](TypeA a) { return 1; },
///         [](TypeB b) { return 2; },
///     f);
/// \endcode
///
template <class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

/// QOL macros for the overload tool
#define VCASE(C) [&](C)
#define VMATCH(V, ...) std::visit(overloaded { __VA_ARGS__ }, V);
#define VMATCH_W(C, V, ...) C(overloaded { __VA_ARGS__ }, V);

/// To use macros:
/// \code
///
/// VMATCH(t,
///     VCASE(int& a) {
///         return a;
///     },
///     VCASE(std::string& s) -> int {
///         return s.size();
///     }
/// )
///
/// \endcode

#endif // VARIANT_TOOLS_H
