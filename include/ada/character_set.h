// copied from boost-url
// https://github.com/boostorg/url/blob/develop/include/boost/url/grammar/charset.hpp

#include <cstdint>
#include <type_traits>

// clang-format off

namespace detail {
template<class T, class = void>
struct is_pred : std::false_type {};

template<class T>
struct is_pred<T, std::void_t<
    decltype(
    std::declval<bool&>() =
        std::declval<T const&>().operator()(
            std::declval<char>())
            ) > > : std::true_type
{
};
} // detail

/** A set of characters

    The characters defined by instances of
    this set are provided upon construction.
    The `constexpr` implementation allows
    these to become compile-time constants.

    @par Example
    Character sets are used with rules and the
    functions @ref find_if and @ref find_if_not.
    @code
    constexpr charset vowel_chars = "AEIOU" "aeiou";

    result< string_view > rv = parse( "Aiea", token_rule( vowel_chars ) );
    @endcode

    @see
        @ref find_if,
        @ref find_if_not,
        @ref parse,
        @ref token_rule.
*/
class charset
{
    std::uint64_t mask_[4] = {};

    constexpr
    static
    std::uint64_t
    lo(char c) noexcept
    {
        return static_cast<
            unsigned char>(c) & 3;
    }

    constexpr
    static
    std::uint64_t
    hi(char c) noexcept
    {
        return 1ULL << (static_cast<
            unsigned char>(c) >> 2);
    }

    constexpr
    static
    charset
    construct(
        char const* s) noexcept
    {
        return *s
            ? charset(*s) +
              construct(s+1)
            : charset();
    }

    constexpr
    static
    charset
    construct(
        unsigned char ch,
        bool b) noexcept
    {
        return b
            ? charset(ch)
            : charset();
    }

    template<class Pred>
    constexpr
    static
    charset
    construct(
        Pred const& pred,
        unsigned char ch) noexcept
    {
        return ch == 255
            ? construct(ch, pred(ch))
            : construct(ch, pred(ch)) +
                construct(pred, ch + 1);
    }

    constexpr
    charset() = default;

    constexpr
    charset(
        std::uint64_t m0,
        std::uint64_t m1,
        std::uint64_t m2,
        std::uint64_t m3) noexcept
        : mask_{ m0, m1, m2, m3 }
    {
    }

public:
    /** Constructor

        This function constructs a character
        set which has as a single member,
        the character `ch`.

        @par Example
        @code
        constexpr charset asterisk( '*' );
        @endcode

        @par Complexity
        Constant.

        @par Exception Safety
        Throws nothing.

        @param ch A character.
    */
    constexpr
    charset(char ch) noexcept
        : mask_ {
            lo(ch) == 0 ? hi(ch) : 0,
            lo(ch) == 1 ? hi(ch) : 0,
            lo(ch) == 2 ? hi(ch) : 0,
            lo(ch) == 3 ? hi(ch) : 0 }
    {
    }

    /** Constructor

        This function constructs a character
        set which has as members, all of the
        characters present in the null-terminated
        string `s`.

        @par Example
        @code
        constexpr charset digits = "0123456789";
        @endcode

        @par Complexity
        Linear in `::strlen(s)`, or constant
        if `s` is a constant expression.

        @par Exception Safety
        Throws nothing.

        @param s A null-terminated string.
    */
    constexpr
    charset(
        char const* s) noexcept
        : charset(construct(s))
    {
    }

    /** Constructor.

        This function constructs a character
        set which has as members, every value
        of `char ch` for which the expression
        `pred(ch)` returns `true`.

        @par Example
        @code
        struct is_digit
        {
            constexpr bool
            operator()(char c ) const noexcept
            {
                return c >= '0' && c <= '9';
            }
        };

        constexpr charset digits( is_digit{} );
        @endcode

        @par Complexity
        Linear in `pred`, or constant if
        `pred(ch)` is a constant expression.

        @par Exception Safety
        Throws nothing.

        @param pred The function object to
        use for determining membership in
        the character set.
    */
    template<class Pred
        ,class = typename std::enable_if<
            detail::is_pred<Pred>::value &&
        ! std::is_base_of<
            charset, Pred>::value>::type
    >
    constexpr
    charset(Pred const& pred) noexcept
        : charset(
            construct(pred, 0))
    {
    }

    /** Return true if ch is in the character set.

        This function returns true if the
        character `ch` is in the set, otherwise
        it returns false.

        @par Complexity
        Constant.

        @par Exception Safety
        Throws nothing.

        @param ch The character to test.
    */
    constexpr
    bool
    operator()(
        unsigned char ch) const noexcept
    {
        return mask_[lo(ch)] & hi(ch);
    }

    /** Return the union of two character sets.

        This function returns a new character
        set which contains all of the characters
        in `cs0` as well as all of the characters
        in `cs`.

        @par Example
        This creates a character set which
        includes all letters and numbers
        @code
        constexpr charset alpha_chars(
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz");

        constexpr charset alnum_chars = alpha_chars + "0123456789";
        @endcode

        @par Complexity
        Constant.

        @return The new character set.

        @param cs0 A character to join

        @param cs1 A character to join
    */
    friend
    constexpr
    charset
    operator+(
            charset const& cs0,
            charset const& cs1) noexcept
    {
        return charset(
            cs0.mask_[0] | cs1.mask_[0],
            cs0.mask_[1] | cs1.mask_[1],
            cs0.mask_[2] | cs1.mask_[2],
            cs0.mask_[3] | cs1.mask_[3]);
    }

    /** Return a new character set by subtracting

        This function returns a new character
        set which is formed from all of the
        characters in `cs0` which are not in `cs`.

        @par Example
        This statement declares a character set
        containing all the lowercase letters
        which are not vowels:
        @code
        constexpr charset consonants = charset("abcdefghijklmnopqrstuvwxyz") - "aeiou";
        @endcode

        @par Complexity
        Constant.

        @return The new character set.

        @param cs0 A character set to join.

        @param cs1 A character set to join.
    */
    friend
    constexpr
    charset
    operator-(
            charset const& cs0,
            charset const& cs1) noexcept
    {
        return charset(
            cs0.mask_[0] & ~cs1.mask_[0],
            cs0.mask_[1] & ~cs1.mask_[1],
            cs0.mask_[2] & ~cs1.mask_[2],
            cs0.mask_[3] & ~cs1.mask_[3]);
    }

    /** Return a new character set which is the complement of another character set.

        This function returns a new character
        set which contains all of the characters
        that are not in `*this`.

        @par Example
        This statement declares a character set
        containing everything but vowels:
        @code
        constexpr charset not_vowels = ~charset( "AEIOU" "aeiou" );
        @endcode

        @par Complexity
        Constant.

        @par Exception Safety
        Throws nothing.

        @return The new character set.
    */
    constexpr
    charset
    operator~() const noexcept
    {
        return charset(
            ~mask_[0],
            ~mask_[1],
            ~mask_[2],
            ~mask_[3]
        );
    }
};

// clang-format on