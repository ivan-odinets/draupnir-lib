#ifndef OPERATOR_CONCEPTS_H
#define OPERATOR_CONCEPTS_H

#include <concepts>
#include <compare>

namespace draupnir::utils
{

/*! @ingroup Utils
 *  @brief Checks whether two types can be compared for equality in both directions.
 *  @tparam First The first compared type.
 *  @tparam Second The second compared type.
 *
 *  @details This concept requires that expressions `first == second` and `second == first` are both valid and return
 *           exactly `bool`. */

template<class First, class Second>
concept equal_comparable =
    requires(const First& first, const Second& second) {
        { first == second } -> std::same_as<bool>;
        { second == first } -> std::same_as<bool>;
        { first != second } -> std::same_as<bool>;
        { second != first } -> std::same_as<bool>;
    };

/*! @ingroup Utils
 *  @brief Checks whether two types support strong three-way comparison in both directions.
 *  @tparam First The first compared type.
 *  @tparam Second The second compared type.
 *
 *  @details This concept requires that expressions `first <=> second` and `second <=> first` are both valid and return
 *           exactly `std::strong_ordering`. */

template<class First, class Second>
concept strong_three_way_comparable =
    requires(const First& first, const Second& second) {
        { first <=> second } -> std::same_as<std::strong_ordering>;
        { second <=> first } -> std::same_as<std::strong_ordering>;
    };

/*! @ingroup Utils
 *  @brief Checks whether two types support weak three-way comparison in both directions.
 *  @tparam First The first compared type.
 *  @tparam Second The second compared type.
 *
 *  @details This concept requires that expressions `first <=> second` and `second <=> first` are both valid and return
 *           exactly `std::weak_ordering`. */

template<class First, class Second>
concept weak_three_way_comparable =
    requires(const First& first, const Second& second) {
        { first <=> second } -> std::same_as<std::weak_ordering>;
        { second <=> first } -> std::same_as<std::weak_ordering>;
    };

/*! @ingroup Utils
 *  @brief Checks whether two types support any three-way comparison in both directions.
 *  @tparam First The first compared type.
 *  @tparam Second The second compared type.
 *
 *  @details This concept requires that expressions `first <=> second` and `second <=> first` are both valid and return
 *           either `std::weak_ordering` or `std::strong_ordering`. */

template<class First, class Second>
concept three_way_comparable =
    strong_three_way_comparable<First,Second> || weak_three_way_comparable<First,Second>;


/*! @ingroup Utils
 *  @brief Checks whether a type supports bitwise AND assignment with another type.
 *  @tparam First The left-hand operand type.
 *  @tparam Second The right-hand operand type.
 *
 *  @details This concept requires that expression `first &= second` is valid and returns a reference to the left-hand operand. */

template<class First, class Second>
concept bitwise_and_assignable =
    requires(First& first, const Second& second) {
        { first &= second } -> std::same_as<First&>;
    };

/*! @ingroup Utils
 *  @brief Checks whether a type supports bitwise OR assignment with another type.
 *  @tparam First The left-hand operand type.
 *  @tparam Second The right-hand operand type.
 *
 *  @details This concept requires that expression `first |= second` is valid and returns a reference to the left-hand operand. */

template<class First, class Second>
concept bitwise_or_assignable =
    requires(First& first, const Second& second) {
        { first |= second } -> std::same_as<First&>;
    };

/*! @ingroup Utils
 *  @brief Checks whether a type supports bitwise XOR assignment with another type.
 *  @tparam First The left-hand operand type.
 *  @tparam Second The right-hand operand type.
 *
 *  @details This concept requires that expression `first ^= second` is valid and returns a reference to the left-hand operand. */

template<class First, class Second>
concept bitwise_xor_assignable =
    requires(First& first, const Second& second) {
        { first ^= second } -> std::same_as<First&>;
    };

/*! @ingroup Utils
 *  @brief Checks whether bitwise AND can be applied to `First` and `Second`.
 *  @tparam First Left-hand side operand type.
 *  @tparam Second Right-hand side operand type.
 *  @tparam Result Expected result type. Defaults to `First`.
 *
 *  @details This concept is satisfied if an expression of the form `first & second` is well-formed and its result type is
 *           exactly `Result`. */

template<class First, class Second, class Result = First>
concept bitwise_and_appliable = requires(First& first, Second& second) {
    { first & second } -> std::same_as<Result>;
    { second & first } -> std::same_as<Result>;
};

/*! @ingroup Utils
 *  @brief Checks whether bitwise OR can be applied to `First` and `Second`.
 *  @tparam First Left-hand side operand type.
 *  @tparam Second Right-hand side operand type.
 *  @tparam Result Expected result type. Defaults to `First`.
 *
 *  @details This concept is satisfied if an expression of the form `first | second` is well-formed and its result type is
 *           exactly `Result`. */

template<class First, class Second, class Result = First>
concept bitwise_or_appliable = requires(First& first, Second& second) {
    { first | second } -> std::same_as<Result>;
    { second | first } -> std::same_as<Result>;
};

/*! @ingroup Utils
 *  @brief Checks whether bitwise XOR can be applied to `First` and `Second`.
 *  @tparam First Left-hand side operand type.
 *  @tparam Second Right-hand side operand type.
 *  @tparam Result Expected result type. Defaults to `First`.
 *
 *  @details This concept is satisfied if an expression of the form `first ^ second` is well-formed and its result type is
 *           exactly `Result`. */

template<class First, class Second, class Result = First>
concept bitwise_xor_appliable = requires(First& first, Second& second) {
    { first ^ second } -> std::same_as<Result>;
    { second ^ first } -> std::same_as<Result>;
};

}; // namespace draupnir::utils

#endif // OPERATOR_CONCEPTS_H
