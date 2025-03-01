/* The MIT License (MIT)

Copyright (c) 2019 jonathan poelen

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#pragma once

#ifndef JLN_MP_HPP
#define JLN_MP_HPP

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <type_traits>
#include <utility>

namespace jln::mp
{
    /// \ingroup functional

    /// \treturn \value
    struct identity
    {
        template<class x>
        using f = x;
    };

    namespace emp
    {
        template<class x>
        using identity = x;
    }
} // namespace jln::mp
namespace jln::mp
{
    /// \cond
    namespace detail
    {
        template <class L> struct _unpack {};
        template <class L> struct _unpack_append {};
    }
    /// \endcond

    /// \ingroup utility

    /// Turns a \typelist into a \sequence of those types.
    /// \semantics
    ///   \code
    ///   unpack<F>::f<typelist<xs...>, ys...> == F::f<ys..., xs...>
    ///   \endcode
    /// \treturn \sequence
    template<class C>
    struct unpack
    {
        template<class seq, class... xs>
        using f = typename detail::_unpack<seq>::template f<C, xs...>;
    };

    /// Turns a \typelist into a \sequence of those types.
    /// \semantics
    ///   \code
    ///   reverse_unpack<F>::f<typelist<xs...>, ys...> == F::f<xs..., ys...>
    ///   \endcode
    /// \treturn \sequence
    template<class C>
    struct unpack_append
    {
        template<class seq, class... xs>
        using f = typename detail::_unpack_append<seq>::template f<C, xs...>;
    };

    namespace emp
    {
        template<class L, class C, class... xs>
        using unpack = typename detail::_unpack<L>::template f<C, xs...>;

        template<class L, class C, class... xs>
        using unpack_append = typename detail::_unpack_append<L>::template f<C, xs...>;
    }
} // namespace jln::mp


namespace jln::mp
{
    /// \ingroup list

    template<class... xs>
    class list {};
} // namespace jln::mp
#ifndef JLN_MP_ENABLE_DEBUG
#  define JLN_MP_ENABLE_DEBUG 0
#endif

#if JLN_MP_ENABLE_DEBUG
#  define JLN_MP_DEBUG_A(...) , __VA_ARGS__
#  define JLN_MP_DEBUG_B(...) __VA_ARGS__,
#  define JLN_MP_DEBUG(...) __VA_ARGS__
#else
#  define JLN_MP_DEBUG_A(...)
#  define JLN_MP_DEBUG_B(...)
#  define JLN_MP_DEBUG(...)
#endif
#ifndef JLN_MP_MAX_CALL_ELEMENT
# define JLN_MP_MAX_CALL_ELEMENT 10000
#endif

/// \defgroup list List
/// \defgroup algorithm Algorithm
/// \defgroup number Number
/// \defgroup value Value
/// \defgroup functional Functional
/// \defgroup utility Utility
/// \defgroup trait Trait
/// \defgroup search Search
/// \defgroup group Group
namespace jln::mp
{
    /// \ingroup number

    using int_ = std::intmax_t;
    using uint_ = std::intmax_t;

    template<int_ v>
    struct number { static const int_ value = v; };

    using true_ = number<1>;
    using false_ = number<0>;
}
/// \cond
#ifdef _MSC_VER
# ifdef JLN_MP_ENABLE_DEBUG
#  ifndef JLN_MP_ENABLE_DEBUG_FORCE
#   undef JLN_MP_ENABLE_DEBUG
#   define JLN_MP_ENABLE_DEBUG 1
#  endif
# else
#  define JLN_MP_ENABLE_DEBUG 1
# endif
#endif
/// \endcond

namespace jln::mp
{
    /// \cond
    namespace detail
    {
        template<bool> struct dcall;
        template<bool> struct dcallf;
        template<bool> struct dcall_c;
        template<bool> struct dcallf_c;

        template<class C, class L, class = void>
        struct _memoizer_impl
        {};

        template<class C, class... xs>
        using _memoizer = _memoizer_impl<C, list<xs...>>;

        template<template<class...> class F, class L, class = void>
        struct _memoizerf_impl
        {};

        template<template<class...> class F, class... xs>
        using _memoizerf = _memoizerf_impl<F, list<xs...>>;
    }
    /// \endcond

    /// \ingroup functional

    /// Memoization version of \link call.
    template<class C, class... xs>
    using memoize_call = typename detail::_memoizer<C, xs...>::type;

#define JLN_MP_DCALL_XS(xs, ...) JLN_MP_DCALL(sizeof...(xs) < JLN_MP_MAX_CALL_ELEMENT, __VA_ARGS__)
#define JLN_MP_DCALLF_XS(xs, ...) JLN_MP_DCALLF(sizeof...(xs) < JLN_MP_MAX_CALL_ELEMENT, __VA_ARGS__)
#define JLN_MP_DCALLF_C_XS(xs, ...) JLN_MP_DCALLF_C(sizeof...(xs) < JLN_MP_MAX_CALL_ELEMENT, __VA_ARGS__)

#if JLN_MP_ENABLE_DEBUG || defined(JLN_MP_DOXYGENATING)

#ifdef JLN_MP_DOXYGENATING
    template<class C, class... xs>
    using call = C::f<xs...>;
#define JLN_MP_DCALL(cond, ...) call<__VA_ARGS__>
#define JLN_MP_DCALLF(cond, F, ...) F<__VA_ARGS__>
#define JLN_MP_DCALL_C(cond, F, ...) F<__VA_ARGS__>
#define JLN_MP_DCALLF_C(cond, F, ...) F<__VA_ARGS__>
#else
    template<class C, class... xs>
    using call = typename detail::_memoizer<C, xs...>::type;
#define JLN_MP_DCALL(cond, ...) typename detail::_memoizer<__VA_ARGS__>::type
#define JLN_MP_DCALLF(cond, ...) typename detail::dcallf<(cond)>::template f<__VA_ARGS__>
#define JLN_MP_DCALL_C(cond, ...) typename detail::dcall_c<(cond)>::template f<__VA_ARGS__>
#define JLN_MP_DCALLF_C(cond, ...) typename detail::dcallf_c<(cond)>::template f<__VA_ARGS__>
#endif

#if __cplusplus >= 201703L
    template<class C, auto... xs>
#else
    template<class C, int_... xs>
#endif
    using call_c = typename C::template f<xs...>;

    template<class C, class F, class... xs>
    using dispatch = call<C, call<F, xs>...>;

    template<class FC, class F, class... xs>
    using indirect_call = call<call<FC, xs...>, F, xs...>;

    template<class C, class F, class... xs>
    using unary_compose_call = call<C, call<F, xs...>>;

    template<class C, class F0, class F1, class... xs>
    using binary_compose_call = call<C, call<F0, xs...>, call<F1, xs...>>;

    template<class C, class F0, class F1, class F2, class... xs>
    using ternary_compose_call = call<C, call<F0, xs...>, call<F1, xs...>, call<F2, xs...>>;

#else
    /// \cond
    namespace detail
    {
        template<template<class...> class f, class FC, class F, class... xs>
        using _indirect_call = f<f<FC, xs...>, F, xs...>;

        template<template<class...> class f, class C, class F, class... xs>
        using _dispatch = f<C, f<F, xs>...>;

        template<template<class...> class f, class C, class F, class... xs>
        using _unary_compose_call = f<C, f<F, xs...>>;

        template<template<class...> class f, class C, class F0, class F1, class... xs>
        using _binary_compose_call = f<C, f<F0, xs...>, f<F1, xs...>>;

        template<template<class...> class f, class C, class F0, class F1, class F2, class... xs>
        using _ternary_compose_call = f<C, f<F0, xs...>, f<F1, xs...>, f<F2, xs...>>;
    }
    /// \endcond

# define JLN_MP_DCALL(cond, ...) typename detail::dcall<(cond)>::template f<__VA_ARGS__>
# define JLN_MP_DCALLF(cond, ...) typename detail::dcallf<(cond)>::template f<__VA_ARGS__>
# define JLN_MP_DCALL_C(cond, ...) typename detail::dcall_c<(cond)>::template f<__VA_ARGS__>
# define JLN_MP_DCALLF_C(cond, ...) typename detail::dcallf_c<(cond)>::template f<__VA_ARGS__>

    template<class C, class... xs>
    using call = typename detail::dcall<(sizeof...(xs) < JLN_MP_MAX_CALL_ELEMENT)>
        ::template f<C, xs...>;

#if __cplusplus >= 201703L
    template<class C, auto... xs>
#else
    template<class C, int_... xs>
#endif
    using call_c = typename detail::dcall_c<(sizeof...(xs) < JLN_MP_MAX_CALL_ELEMENT)>
        ::template f<C, xs...>;

    template<class FC, class F, class... xs>
    using indirect_call = detail::_indirect_call<
        detail::dcall<(sizeof...(xs) < JLN_MP_MAX_CALL_ELEMENT)>::template f,
        FC, F, xs...>;

    template<class C, class F, class... xs>
    using dispatch = detail::_dispatch<
        detail::dcall<(sizeof...(xs) < JLN_MP_MAX_CALL_ELEMENT)>::template f,
        C, F, xs...>;

    template<class C, class F, class... xs>
    using unary_compose_call = detail::_unary_compose_call<
        detail::dcall<(sizeof...(xs) < JLN_MP_MAX_CALL_ELEMENT)>::template f,
        C, F, xs...>;

    template<class C, class F0, class F1, class... xs>
    using binary_compose_call = detail::_binary_compose_call<
        detail::dcall<(sizeof...(xs) < JLN_MP_MAX_CALL_ELEMENT)>::template f,
        C, F0, F1, xs...>;

    template<class C, class F0, class F1, class F2, class... xs>
    using ternary_compose_call = detail::_ternary_compose_call<
        detail::dcall<(sizeof...(xs) < JLN_MP_MAX_CALL_ELEMENT)>::template f,
        C, F0, F1, F2, xs...>;
#endif

    template<class C, class... xs>
    using call_t = typename call<C, xs...>::type;

    template<class FC, class F, class... xs>
    using indirect_call_t = typename indirect_call<FC, F, xs...>::type;

    template<class C, class F, class... xs>
    using dispatch_t = typename dispatch<C, F, xs...>::type;

    template<class C, class F, class... xs>
    using unary_compose_call_t = typename unary_compose_call<C, F, xs...>::type;

    template<class C, class F0, class F1, class... xs>
    using binary_compose_call_t = typename binary_compose_call<C, F0, F1, xs...>::type;

    template<class C, class F0, class F1, class F2, class... xs>
    using ternary_compose_call_t = typename ternary_compose_call<C, F0, F1, F2, xs...>::type;
}

/// \cond
namespace jln::mp::detail
{
    template<class x, class...>
    using _first = x;

    template<class C, class... xs>
    struct _memoizer_impl<C, list<xs...>,
        _first<void, typename C::template f<xs...>>>
    {
        using type = typename C::template f<xs...>;
    };

    template<template<class...> class F, class... xs>
    struct _memoizerf_impl<F, list<xs...>, _first<void, F<xs...>>>
    {
        using type = F<xs...>;
    };

    template<>
    struct dcall<true>
    {
        template<class C, class... xs>
        using f = typename C::template f<xs...>;
    };

    template<>
    struct dcall_c<true>
    {
#if __cplusplus >= 201703L
        template<class C, auto...xs>
#else
        template<class C, int_...xs>
#endif
        using f = typename C::template f<xs...>;
    };

    template<>
    struct dcallf<true>
    {
        template<template<class...> class F, class... xs>
        using f = F<xs...>;
    };

    template<>
    struct dcallf_c<true>
    {
#if __cplusplus >= 201703L
        template<template<auto...> class F, auto...xs>
#else
        template<template<int_...> class F, int_...xs>
#endif
        using f = F<xs...>;
    };
}
/// \endcond
namespace jln::mp
{
    /// \ingroup functional

    /// Makes a \function from a \lazymetafunction.
    /// \treturn \value
    /// \see lift
    template<template<class...> class F, class C = identity>
    struct lift_t
    {
        template<class... xs>
        using f = typename C::template f<JLN_MP_DCALLF_XS(xs, F, xs...)::type>;
    };

    /// \cond
    template<template<class...> class F>
    struct lift_t<F, identity>
    {
        template<class... xs>
        using f = JLN_MP_DCALLF_XS(xs, F, xs...)::type;
    };
    /// \endcond

    /// Makes a \function from a \metafunction.
    /// \treturn \value
    /// \see lift_t
    template<template<class...> class F, class C = identity>
    struct lift
    {
        template<class... xs>
        using f = typename C::template f<JLN_MP_DCALLF_XS(xs, F, xs...)>;
    };

    /// \cond
    template<template<class...> class F>
    struct lift<F, identity>
    {
        template<class... xs>
        using f = JLN_MP_DCALLF_XS(xs, F, xs...);
    };
    /// \endcond

#if __cplusplus >= 201703L
    template<template<auto...> class F>
    struct lift_c
    {
        template<auto... xs>
        using f = JLN_MP_DCALLF_C_XS(xs, F, xs...);
    };

    template<template<auto...> class F>
    struct lift_v
#else
    template<template<int_...> class F>
    struct lift_c
    {
        template<int_... xs>
        using f = JLN_MP_DCALLF_C_XS(xs, F, xs...);
    };

    template<template<int_...> class F>
    struct lift_v
#endif
    {
        template<class... xs>
        using f = JLN_MP_DCALLF_C_XS(xs, F, xs::value...);
    };
} // namespace jln::mp
namespace jln::mp
{
    /// \ingroup list

    /// \treturn \link list
    using listify = lift<list>;

    /// \cond
    template<>
    struct lift<list, identity>
    {
        template<class... xs>
        using f = list<xs...>;
    };
    /// \endcond
} // namespace jln::mp
/// \cond
namespace jln::mp::detail
{
    template<template<class...> class Seq, class... ys>
    struct _unpack<Seq<ys...>>
    {
        template<class C, class... xs>
        using f = typename C::template f<xs..., ys...>;
    };

    template<template<class...> class Seq, class... xs>
    struct _unpack_append<Seq<xs...>>
    {
        template<class C, class... ys>
        using f = typename C::template f<xs..., ys...>;
    };

    template<class C>
    struct optimize_useless_unpack
    {
        using type = C;
    };

    template<>
    struct optimize_useless_unpack<unpack<listify>>
    {
        using type = identity;
    };

    template<>
    struct optimize_useless_unpack<unpack_append<listify>>
    {
        using type = identity;
    };

    template<class C>
    using optimize_useless_unpack_t = typename optimize_useless_unpack<C>::type;
}
/// \endcond
namespace jln::mp
{
    /// \ingroup list

    /// Appends \c x to the beginning of the sequence.
    /// \treturn \sequence
    template<class x, class C = listify>
    struct push_front
    {
        template<class... xs>
        using f = call<C, x, xs...>;
    };

    namespace emp
    {
        template<class L, class T, class C = mp::listify>
        using push_front = unpack<L, mp::push_front<T, C>>;
    }
}

namespace jln::mp::detail
{
    constexpr unsigned min(unsigned a, unsigned b)
    {
        return a < b ? a : b;
    }

    constexpr unsigned max(unsigned a, unsigned b)
    {
        return a < b ? b : a;
    }

    constexpr unsigned n_8_or_less_16_64_256(unsigned n)
    {
        return
            n <= 8 ? n
            : n < 16 ? 8
            : n < 64 ? 16
            : n < 256 ? 64
            : 256
            ;
    }

    constexpr unsigned n_8_or_less_16_64(unsigned n)
    {
        return
            n <= 8 ? n
            : n < 16 ? 8
            : n < 64 ? 16
            : 64
            ;
    }

    constexpr int n_4_or_less_8_16_64_256(int n)
    {
        return
            n <= 4 ? n
            : n < 8 ? 4
            : n < 16 ? 8
            : n < 64 ? 16
            : n < 256 ? 64
            : 256
            ;
    }

    constexpr int sub_1_n_4_or_less_8_16_64_256(int n)
    {
        --n;
        return
            n <= 4 ? n
            : n < 8 ? 4
            : n < 16 ? 8
            : n < 64 ? 16
            : n < 256 ? 64
            : 256
            ;
    }

    constexpr unsigned n_16_64_256_1024(unsigned n)
    {
        return
            n <= 16 ? 16
            : n <= 64 ? 64
            : n <= 256 ? 256
            : 1024
            ;
    }

    constexpr unsigned n_8_or_more_16_32_64_128_256(unsigned n)
    {
        return n <= 8 ? n
            : n <= 16 ? 16
            : n <= 32 ? 32
            : n <= 64 ? 64
            : n <= 128 ? 128
            : 256;
    }
}

#define JLN_MP_NIL
#define JLN_MP_COMMA ,
#define JLN_MP_PAREN_OPEN (
#define JLN_MP_PAREN_CLOSE )

#define JLN_MP_GEN_XS_16_64_256_1024(func)                       \
  func(16,   JLN_MP_XS_16,   JLN_MP_RXS_16,   JLN_MP_REPEAT_16)  \
  func(64,   JLN_MP_XS_64,   JLN_MP_RXS_64,   JLN_MP_REPEAT_64)  \
  func(256,  JLN_MP_XS_256,  JLN_MP_RXS_256,  JLN_MP_REPEAT_256) \
  func(1024, JLN_MP_XS_1024, JLN_MP_RXS_1024, JLN_MP_REPEAT_1024)

#define JLN_MP_GEN_XS_8(func)                               \
  func(8,   JLN_MP_XS_8,   JLN_MP_RXS_8,   JLN_MP_REPEAT_8) \

#define JLN_MP_GEN_XS_8_args(func, ...) \
  func(8,   JLN_MP_XS_8,   JLN_MP_RXS_8,   JLN_MP_REPEAT_8, __VA_ARGS__)

#define JLN_MP_GEN_XS_16_64_256(func)                        \
  func(16,  JLN_MP_XS_16,  JLN_MP_RXS_16,  JLN_MP_REPEAT_16) \
  func(64,  JLN_MP_XS_64,  JLN_MP_RXS_64,  JLN_MP_REPEAT_64) \
  func(256, JLN_MP_XS_256, JLN_MP_RXS_256, JLN_MP_REPEAT_256)

#define JLN_MP_GEN_XS_16_32_64_128_256(func)                  \
  func(16,  JLN_MP_XS_16,  JLN_MP_RXS_16,  JLN_MP_REPEAT_16)  \
  func(32,  JLN_MP_XS_32,  JLN_MP_RXS_32,  JLN_MP_REPEAT_32)  \
  func(64,  JLN_MP_XS_64,  JLN_MP_RXS_64,  JLN_MP_REPEAT_64)  \
  func(128, JLN_MP_XS_128, JLN_MP_RXS_128, JLN_MP_REPEAT_128) \
  func(256, JLN_MP_XS_256, JLN_MP_RXS_256, JLN_MP_REPEAT_256)

#define JLN_MP_GEN_XS_8_16_64_256(func)                      \
  func(8,   JLN_MP_XS_8,   JLN_MP_RXS_8,   JLN_MP_REPEAT_8)  \
  func(16,  JLN_MP_XS_16,  JLN_MP_RXS_16,  JLN_MP_REPEAT_16) \
  func(64,  JLN_MP_XS_64,  JLN_MP_RXS_64,  JLN_MP_REPEAT_64) \
  func(256, JLN_MP_XS_256, JLN_MP_RXS_256, JLN_MP_REPEAT_256)

#define JLN_MP_GEN_XS_4_8_16_64_256(func)                    \
  func(4,   JLN_MP_XS_4,   JLN_MP_RXS_4,   JLN_MP_REPEAT_4)  \
  func(8,   JLN_MP_XS_8,   JLN_MP_RXS_8,   JLN_MP_REPEAT_8)  \
  func(16,  JLN_MP_XS_16,  JLN_MP_RXS_16,  JLN_MP_REPEAT_16) \
  func(64,  JLN_MP_XS_64,  JLN_MP_RXS_64,  JLN_MP_REPEAT_64) \
  func(256, JLN_MP_XS_256, JLN_MP_RXS_256, JLN_MP_REPEAT_256)

#define JLN_MP_GEN_XS_2_4_8_16_32_64_128_256(func)            \
  func(2,   JLN_MP_XS_2,   JLN_MP_RXS_2,   JLN_MP_REPEAT_2)   \
  func(4,   JLN_MP_XS_4,   JLN_MP_RXS_4,   JLN_MP_REPEAT_4)   \
  func(8,   JLN_MP_XS_8,   JLN_MP_RXS_8,   JLN_MP_REPEAT_8)   \
  func(16,  JLN_MP_XS_16,  JLN_MP_RXS_16,  JLN_MP_REPEAT_16)  \
  func(32,  JLN_MP_XS_32,  JLN_MP_RXS_32,  JLN_MP_REPEAT_32)  \
  func(64,  JLN_MP_XS_64,  JLN_MP_RXS_64,  JLN_MP_REPEAT_64)  \
  func(128, JLN_MP_XS_128, JLN_MP_RXS_128, JLN_MP_REPEAT_128) \
  func(256, JLN_MP_XS_256, JLN_MP_RXS_256, JLN_MP_REPEAT_256)

#define JLN_MP_GEN_XS_0_TO_8(func)                    \
  func(0, JLN_MP_XS_0, JLN_MP_RXS_0, JLN_MP_REPEAT_0) \
  func(1, JLN_MP_XS_1, JLN_MP_RXS_1, JLN_MP_REPEAT_1) \
  func(2, JLN_MP_XS_2, JLN_MP_RXS_2, JLN_MP_REPEAT_2) \
  func(3, JLN_MP_XS_3, JLN_MP_RXS_3, JLN_MP_REPEAT_3) \
  func(4, JLN_MP_XS_4, JLN_MP_RXS_4, JLN_MP_REPEAT_4) \
  func(5, JLN_MP_XS_5, JLN_MP_RXS_5, JLN_MP_REPEAT_5) \
  func(6, JLN_MP_XS_6, JLN_MP_RXS_6, JLN_MP_REPEAT_6) \
  func(7, JLN_MP_XS_7, JLN_MP_RXS_7, JLN_MP_REPEAT_7)

#define JLN_MP_GEN_XS_1_TO_8(func)                    \
  func(1, JLN_MP_XS_1, JLN_MP_RXS_1, JLN_MP_REPEAT_1) \
  func(2, JLN_MP_XS_2, JLN_MP_RXS_2, JLN_MP_REPEAT_2) \
  func(3, JLN_MP_XS_3, JLN_MP_RXS_3, JLN_MP_REPEAT_3) \
  func(4, JLN_MP_XS_4, JLN_MP_RXS_4, JLN_MP_REPEAT_4) \
  func(5, JLN_MP_XS_5, JLN_MP_RXS_5, JLN_MP_REPEAT_5) \
  func(6, JLN_MP_XS_6, JLN_MP_RXS_6, JLN_MP_REPEAT_6) \
  func(7, JLN_MP_XS_7, JLN_MP_RXS_7, JLN_MP_REPEAT_7)

#define JLN_MP_GEN_XS_2_TO_8(func)                    \
  func(2, JLN_MP_XS_2, JLN_MP_RXS_2, JLN_MP_REPEAT_2) \
  func(3, JLN_MP_XS_3, JLN_MP_RXS_3, JLN_MP_REPEAT_3) \
  func(4, JLN_MP_XS_4, JLN_MP_RXS_4, JLN_MP_REPEAT_4) \
  func(5, JLN_MP_XS_5, JLN_MP_RXS_5, JLN_MP_REPEAT_5) \
  func(6, JLN_MP_XS_6, JLN_MP_RXS_6, JLN_MP_REPEAT_6) \
  func(7, JLN_MP_XS_7, JLN_MP_RXS_7, JLN_MP_REPEAT_7)

#define JLN_MP_GEN_XS_0_TO_4(func)                    \
  func(0, JLN_MP_XS_0, JLN_MP_RXS_0, JLN_MP_REPEAT_0) \
  func(1, JLN_MP_XS_1, JLN_MP_RXS_1, JLN_MP_REPEAT_1) \
  func(2, JLN_MP_XS_2, JLN_MP_RXS_2, JLN_MP_REPEAT_2) \
  func(3, JLN_MP_XS_3, JLN_MP_RXS_3, JLN_MP_REPEAT_3)

#define JLN_MP_GEN_XS_1_TO_4(func)                    \
  func(1, JLN_MP_XS_1, JLN_MP_RXS_1, JLN_MP_REPEAT_1) \
  func(2, JLN_MP_XS_2, JLN_MP_RXS_2, JLN_MP_REPEAT_2) \
  func(3, JLN_MP_XS_3, JLN_MP_RXS_3, JLN_MP_REPEAT_3)


#define JLN_MP_REPEAT_0(x, sep)
#define JLN_MP_REPEAT_1(x, B) x
#define JLN_MP_REPEAT_2(x, B) x B x
#define JLN_MP_REPEAT_3(x, B) x B x B x
#define JLN_MP_REPEAT_4(x, B) x B x B x B x
#define JLN_MP_REPEAT_5(x, B) x B x B x B x B x
#define JLN_MP_REPEAT_6(x, B) x B x B x B x B x B x
#define JLN_MP_REPEAT_7(x, B) x B x B x B x B x B x B x
#define JLN_MP_REPEAT_8(x, B) x B x B x B x B x B x B x B x

#define JLN_MP_REPEAT_16(x, B) \
  x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B x

#define JLN_MP_REPEAT_32(x, B)                                    \
  x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B \
  x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B x

#define JLN_MP_REPEAT_64(x, B)                                    \
  x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B \
  x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B \
  x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B \
  x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B x

#define JLN_MP_REPEAT_128(x, B)                                   \
  x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B \
  x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B \
  x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B \
  x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B \
  x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B \
  x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B \
  x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B \
  x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B x

#define JLN_MP_REPEAT_256(x, B)                                   \
  x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B \
  x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B \
  x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B \
  x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B \
  x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B \
  x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B \
  x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B \
  x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B \
  x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B \
  x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B \
  x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B \
  x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B \
  x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B \
  x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B \
  x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B \
  x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B x

#define JLN_MP_REPEAT_512(x, B)                                   \
  x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B \
  x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B \
  x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B \
  x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B \
  x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B \
  x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B \
  x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B \
  x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B \
  x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B \
  x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B \
  x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B \
  x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B \
  x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B \
  x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B \
  x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B \
  x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B \
  x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B \
  x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B \
  x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B \
  x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B \
  x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B \
  x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B \
  x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B \
  x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B \
  x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B \
  x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B \
  x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B \
  x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B \
  x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B \
  x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B \
  x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B \
  x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B x

#define JLN_MP_REPEAT_1024(x, B)                                  \
  x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B \
  x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B \
  x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B \
  x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B \
  x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B \
  x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B \
  x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B \
  x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B \
  x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B \
  x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B \
  x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B \
  x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B \
  x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B \
  x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B \
  x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B \
  x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B \
  x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B \
  x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B \
  x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B \
  x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B \
  x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B \
  x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B \
  x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B \
  x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B \
  x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B \
  x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B \
  x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B \
  x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B \
  x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B \
  x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B \
  x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B \
  x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B \
  x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B \
  x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B \
  x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B \
  x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B \
  x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B \
  x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B \
  x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B \
  x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B \
  x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B \
  x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B \
  x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B \
  x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B \
  x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B \
  x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B \
  x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B \
  x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B \
  x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B \
  x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B \
  x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B \
  x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B \
  x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B \
  x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B \
  x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B \
  x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B \
  x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B \
  x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B \
  x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B \
  x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B \
  x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B \
  x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B \
  x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B \
  x B x B x B x B x B x B x B x B x B x B x B x B x B x B x B x


#define JLN_MP_XS_0(before, after, sep)

#define JLN_MP_XS_1(before, after, sep) \
  before _1 after

#define JLN_MP_XS_2(before, after, sep) \
  before _1 after sep before _2 after

#define JLN_MP_XS_3(before, after, sep) \
  before _1 after sep before _2 after sep before _3 after

#define JLN_MP_XS_4(before, after, sep) \
  before _1 after sep before _2 after sep before _3 after sep before _4 after

#define JLN_MP_XS_5(before, after, sep)                                           \
  before _1 after sep before _2 after sep before _3 after sep before _4 after sep \
  before _5 after

#define JLN_MP_XS_6(before, after, sep)                                           \
  before _1 after sep before _2 after sep before _3 after sep before _4 after sep \
  before _5 after sep before _6 after

#define JLN_MP_XS_7(before, after, sep)                                           \
  before _1 after sep before _2 after sep before _3 after sep before _4 after sep \
  before _5 after sep before _6 after sep before _7 after

#define JLN_MP_XS_8(before, after, sep)                                           \
  before _1 after sep before _2 after sep before _3 after sep before _4 after sep \
  before _5 after sep before _6 after sep before _7 after sep before _8 after

#define JLN_MP_XS_2_TO_8(before, after, sep)                                      \
                      before _2 after sep before _3 after sep before _4 after sep \
  before _5 after sep before _6 after sep before _7 after sep before _8 after

#define JLN_MP_XS_16(before, after, sep)                                             \
  before _1 after sep before _2 after sep before _3 after sep before _4 after sep    \
  before _5 after sep before _6 after sep before _7 after sep before _8 after sep    \
  before _9 after sep before _10 after sep before _11 after sep before _12 after sep \
  before _13 after sep before _14 after sep before _15 after sep before _16 after

#define JLN_MP_XS_2_TO_16(before, after, sep)                                        \
                      before _2 after sep before _3 after sep before _4 after sep    \
  before _5 after sep before _6 after sep before _7 after sep before _8 after sep    \
  before _9 after sep before _10 after sep before _11 after sep before _12 after sep \
  before _13 after sep before _14 after sep before _15 after sep before _16 after

#define JLN_MP_XS_32(before, after, sep)                                              \
  before _1 after sep before _2 after sep before _3 after sep before _4 after sep     \
  before _5 after sep before _6 after sep before _7 after sep before _8 after sep     \
  before _9 after sep before _10 after sep before _11 after sep before _12 after sep  \
  before _13 after sep before _14 after sep before _15 after sep before _16 after sep \
  before _17 after sep before _18 after sep before _19 after sep before _20 after sep \
  before _21 after sep before _22 after sep before _23 after sep before _24 after sep \
  before _25 after sep before _26 after sep before _27 after sep before _28 after sep \
  before _29 after sep before _30 after sep before _31 after sep before _32 after

#define JLN_MP_XS_2_TO_32(before, after, sep)                                         \
                      before _2 after sep before _3 after sep before _4 after sep     \
  before _5 after sep before _6 after sep before _7 after sep before _8 after sep     \
  before _9 after sep before _10 after sep before _11 after sep before _12 after sep  \
  before _13 after sep before _14 after sep before _15 after sep before _16 after sep \
  before _17 after sep before _18 after sep before _19 after sep before _20 after sep \
  before _21 after sep before _22 after sep before _23 after sep before _24 after sep \
  before _25 after sep before _26 after sep before _27 after sep before _28 after sep \
  before _29 after sep before _30 after sep before _31 after sep before _32 after

#define JLN_MP_XS_64(before, after, sep)                                              \
  before _1 after sep before _2 after sep before _3 after sep before _4 after sep     \
  before _5 after sep before _6 after sep before _7 after sep before _8 after sep     \
  before _9 after sep before _10 after sep before _11 after sep before _12 after sep  \
  before _13 after sep before _14 after sep before _15 after sep before _16 after sep \
  before _17 after sep before _18 after sep before _19 after sep before _20 after sep \
  before _21 after sep before _22 after sep before _23 after sep before _24 after sep \
  before _25 after sep before _26 after sep before _27 after sep before _28 after sep \
  before _29 after sep before _30 after sep before _31 after sep before _32 after sep \
  before _33 after sep before _34 after sep before _35 after sep before _36 after sep \
  before _37 after sep before _38 after sep before _39 after sep before _40 after sep \
  before _41 after sep before _42 after sep before _43 after sep before _44 after sep \
  before _45 after sep before _46 after sep before _47 after sep before _48 after sep \
  before _49 after sep before _50 after sep before _51 after sep before _52 after sep \
  before _53 after sep before _54 after sep before _55 after sep before _56 after sep \
  before _57 after sep before _58 after sep before _59 after sep before _60 after sep \
  before _61 after sep before _62 after sep before _63 after sep before _64 after

#define JLN_MP_XS_2_TO_64(before, after, sep)                                         \
                      before _2 after sep before _3 after sep before _4 after sep     \
  before _5 after sep before _6 after sep before _7 after sep before _8 after sep     \
  before _9 after sep before _10 after sep before _11 after sep before _12 after sep  \
  before _13 after sep before _14 after sep before _15 after sep before _16 after sep \
  before _17 after sep before _18 after sep before _19 after sep before _20 after sep \
  before _21 after sep before _22 after sep before _23 after sep before _24 after sep \
  before _25 after sep before _26 after sep before _27 after sep before _28 after sep \
  before _29 after sep before _30 after sep before _31 after sep before _32 after sep \
  before _33 after sep before _34 after sep before _35 after sep before _36 after sep \
  before _37 after sep before _38 after sep before _39 after sep before _40 after sep \
  before _41 after sep before _42 after sep before _43 after sep before _44 after sep \
  before _45 after sep before _46 after sep before _47 after sep before _48 after sep \
  before _49 after sep before _50 after sep before _51 after sep before _52 after sep \
  before _53 after sep before _54 after sep before _55 after sep before _56 after sep \
  before _57 after sep before _58 after sep before _59 after sep before _60 after sep \
  before _61 after sep before _62 after sep before _63 after sep before _64 after

#define JLN_MP_XS_128(before, after, sep)                                                 \
  before _1 after sep before _2 after sep before _3 after sep before _4 after sep         \
  before _5 after sep before _6 after sep before _7 after sep before _8 after sep         \
  before _9 after sep before _10 after sep before _11 after sep before _12 after sep      \
  before _13 after sep before _14 after sep before _15 after sep before _16 after sep     \
  before _17 after sep before _18 after sep before _19 after sep before _20 after sep     \
  before _21 after sep before _22 after sep before _23 after sep before _24 after sep     \
  before _25 after sep before _26 after sep before _27 after sep before _28 after sep     \
  before _29 after sep before _30 after sep before _31 after sep before _32 after sep     \
  before _33 after sep before _34 after sep before _35 after sep before _36 after sep     \
  before _37 after sep before _38 after sep before _39 after sep before _40 after sep     \
  before _41 after sep before _42 after sep before _43 after sep before _44 after sep     \
  before _45 after sep before _46 after sep before _47 after sep before _48 after sep     \
  before _49 after sep before _50 after sep before _51 after sep before _52 after sep     \
  before _53 after sep before _54 after sep before _55 after sep before _56 after sep     \
  before _57 after sep before _58 after sep before _59 after sep before _60 after sep     \
  before _61 after sep before _62 after sep before _63 after sep before _64 after sep     \
  before _65 after sep before _66 after sep before _67 after sep before _68 after sep     \
  before _69 after sep before _70 after sep before _71 after sep before _72 after sep     \
  before _73 after sep before _74 after sep before _75 after sep before _76 after sep     \
  before _77 after sep before _78 after sep before _79 after sep before _80 after sep     \
  before _81 after sep before _82 after sep before _83 after sep before _84 after sep     \
  before _85 after sep before _86 after sep before _87 after sep before _88 after sep     \
  before _89 after sep before _90 after sep before _91 after sep before _92 after sep     \
  before _93 after sep before _94 after sep before _95 after sep before _96 after sep     \
  before _97 after sep before _98 after sep before _99 after sep before _100 after sep    \
  before _101 after sep before _102 after sep before _103 after sep before _104 after sep \
  before _105 after sep before _106 after sep before _107 after sep before _108 after sep \
  before _109 after sep before _110 after sep before _111 after sep before _112 after sep \
  before _113 after sep before _114 after sep before _115 after sep before _116 after sep \
  before _117 after sep before _118 after sep before _119 after sep before _120 after sep \
  before _121 after sep before _122 after sep before _123 after sep before _124 after sep \
  before _125 after sep before _126 after sep before _127 after sep before _128 after

#define JLN_MP_XS_1_TO_128(before, after, sep)                                            \
                      before _2 after sep before _3 after sep before _4 after sep         \
  before _5 after sep before _6 after sep before _7 after sep before _8 after sep         \
  before _9 after sep before _10 after sep before _11 after sep before _12 after sep      \
  before _13 after sep before _14 after sep before _15 after sep before _16 after sep     \
  before _17 after sep before _18 after sep before _19 after sep before _20 after sep     \
  before _21 after sep before _22 after sep before _23 after sep before _24 after sep     \
  before _25 after sep before _26 after sep before _27 after sep before _28 after sep     \
  before _29 after sep before _30 after sep before _31 after sep before _32 after sep     \
  before _33 after sep before _34 after sep before _35 after sep before _36 after sep     \
  before _37 after sep before _38 after sep before _39 after sep before _40 after sep     \
  before _41 after sep before _42 after sep before _43 after sep before _44 after sep     \
  before _45 after sep before _46 after sep before _47 after sep before _48 after sep     \
  before _49 after sep before _50 after sep before _51 after sep before _52 after sep     \
  before _53 after sep before _54 after sep before _55 after sep before _56 after sep     \
  before _57 after sep before _58 after sep before _59 after sep before _60 after sep     \
  before _61 after sep before _62 after sep before _63 after sep before _64 after sep     \
  before _65 after sep before _66 after sep before _67 after sep before _68 after sep     \
  before _69 after sep before _70 after sep before _71 after sep before _72 after sep     \
  before _73 after sep before _74 after sep before _75 after sep before _76 after sep     \
  before _77 after sep before _78 after sep before _79 after sep before _80 after sep     \
  before _81 after sep before _82 after sep before _83 after sep before _84 after sep     \
  before _85 after sep before _86 after sep before _87 after sep before _88 after sep     \
  before _89 after sep before _90 after sep before _91 after sep before _92 after sep     \
  before _93 after sep before _94 after sep before _95 after sep before _96 after sep     \
  before _97 after sep before _98 after sep before _99 after sep before _100 after sep    \
  before _101 after sep before _102 after sep before _103 after sep before _104 after sep \
  before _105 after sep before _106 after sep before _107 after sep before _108 after sep \
  before _109 after sep before _110 after sep before _111 after sep before _112 after sep \
  before _113 after sep before _114 after sep before _115 after sep before _116 after sep \
  before _117 after sep before _118 after sep before _119 after sep before _120 after sep \
  before _121 after sep before _122 after sep before _123 after sep before _124 after sep \
  before _125 after sep before _126 after sep before _127 after sep before _128 after

#define JLN_MP_XS_256(before, after, sep)                                                 \
  before _1 after sep before _2 after sep before _3 after sep before _4 after sep         \
  before _5 after sep before _6 after sep before _7 after sep before _8 after sep         \
  before _9 after sep before _10 after sep before _11 after sep before _12 after sep      \
  before _13 after sep before _14 after sep before _15 after sep before _16 after sep     \
  before _17 after sep before _18 after sep before _19 after sep before _20 after sep     \
  before _21 after sep before _22 after sep before _23 after sep before _24 after sep     \
  before _25 after sep before _26 after sep before _27 after sep before _28 after sep     \
  before _29 after sep before _30 after sep before _31 after sep before _32 after sep     \
  before _33 after sep before _34 after sep before _35 after sep before _36 after sep     \
  before _37 after sep before _38 after sep before _39 after sep before _40 after sep     \
  before _41 after sep before _42 after sep before _43 after sep before _44 after sep     \
  before _45 after sep before _46 after sep before _47 after sep before _48 after sep     \
  before _49 after sep before _50 after sep before _51 after sep before _52 after sep     \
  before _53 after sep before _54 after sep before _55 after sep before _56 after sep     \
  before _57 after sep before _58 after sep before _59 after sep before _60 after sep     \
  before _61 after sep before _62 after sep before _63 after sep before _64 after sep     \
  before _65 after sep before _66 after sep before _67 after sep before _68 after sep     \
  before _69 after sep before _70 after sep before _71 after sep before _72 after sep     \
  before _73 after sep before _74 after sep before _75 after sep before _76 after sep     \
  before _77 after sep before _78 after sep before _79 after sep before _80 after sep     \
  before _81 after sep before _82 after sep before _83 after sep before _84 after sep     \
  before _85 after sep before _86 after sep before _87 after sep before _88 after sep     \
  before _89 after sep before _90 after sep before _91 after sep before _92 after sep     \
  before _93 after sep before _94 after sep before _95 after sep before _96 after sep     \
  before _97 after sep before _98 after sep before _99 after sep before _100 after sep    \
  before _101 after sep before _102 after sep before _103 after sep before _104 after sep \
  before _105 after sep before _106 after sep before _107 after sep before _108 after sep \
  before _109 after sep before _110 after sep before _111 after sep before _112 after sep \
  before _113 after sep before _114 after sep before _115 after sep before _116 after sep \
  before _117 after sep before _118 after sep before _119 after sep before _120 after sep \
  before _121 after sep before _122 after sep before _123 after sep before _124 after sep \
  before _125 after sep before _126 after sep before _127 after sep before _128 after sep \
  before _129 after sep before _130 after sep before _131 after sep before _132 after sep \
  before _133 after sep before _134 after sep before _135 after sep before _136 after sep \
  before _137 after sep before _138 after sep before _139 after sep before _140 after sep \
  before _141 after sep before _142 after sep before _143 after sep before _144 after sep \
  before _145 after sep before _146 after sep before _147 after sep before _148 after sep \
  before _149 after sep before _150 after sep before _151 after sep before _152 after sep \
  before _153 after sep before _154 after sep before _155 after sep before _156 after sep \
  before _157 after sep before _158 after sep before _159 after sep before _160 after sep \
  before _161 after sep before _162 after sep before _163 after sep before _164 after sep \
  before _165 after sep before _166 after sep before _167 after sep before _168 after sep \
  before _169 after sep before _170 after sep before _171 after sep before _172 after sep \
  before _173 after sep before _174 after sep before _175 after sep before _176 after sep \
  before _177 after sep before _178 after sep before _179 after sep before _180 after sep \
  before _181 after sep before _182 after sep before _183 after sep before _184 after sep \
  before _185 after sep before _186 after sep before _187 after sep before _188 after sep \
  before _189 after sep before _190 after sep before _191 after sep before _192 after sep \
  before _193 after sep before _194 after sep before _195 after sep before _196 after sep \
  before _197 after sep before _198 after sep before _199 after sep before _200 after sep \
  before _201 after sep before _202 after sep before _203 after sep before _204 after sep \
  before _205 after sep before _206 after sep before _207 after sep before _208 after sep \
  before _209 after sep before _210 after sep before _211 after sep before _212 after sep \
  before _213 after sep before _214 after sep before _215 after sep before _216 after sep \
  before _217 after sep before _218 after sep before _219 after sep before _220 after sep \
  before _221 after sep before _222 after sep before _223 after sep before _224 after sep \
  before _225 after sep before _226 after sep before _227 after sep before _228 after sep \
  before _229 after sep before _230 after sep before _231 after sep before _232 after sep \
  before _233 after sep before _234 after sep before _235 after sep before _236 after sep \
  before _237 after sep before _238 after sep before _239 after sep before _240 after sep \
  before _241 after sep before _242 after sep before _243 after sep before _244 after sep \
  before _245 after sep before _246 after sep before _247 after sep before _248 after sep \
  before _249 after sep before _250 after sep before _251 after sep before _252 after sep \
  before _253 after sep before _254 after sep before _255 after sep before _256 after

#define JLN_MP_XS_2_TO_256(before, after, sep)                                            \
                      before _2 after sep before _3 after sep before _4 after sep         \
  before _5 after sep before _6 after sep before _7 after sep before _8 after sep         \
  before _9 after sep before _10 after sep before _11 after sep before _12 after sep      \
  before _13 after sep before _14 after sep before _15 after sep before _16 after sep     \
  before _17 after sep before _18 after sep before _19 after sep before _20 after sep     \
  before _21 after sep before _22 after sep before _23 after sep before _24 after sep     \
  before _25 after sep before _26 after sep before _27 after sep before _28 after sep     \
  before _29 after sep before _30 after sep before _31 after sep before _32 after sep     \
  before _33 after sep before _34 after sep before _35 after sep before _36 after sep     \
  before _37 after sep before _38 after sep before _39 after sep before _40 after sep     \
  before _41 after sep before _42 after sep before _43 after sep before _44 after sep     \
  before _45 after sep before _46 after sep before _47 after sep before _48 after sep     \
  before _49 after sep before _50 after sep before _51 after sep before _52 after sep     \
  before _53 after sep before _54 after sep before _55 after sep before _56 after sep     \
  before _57 after sep before _58 after sep before _59 after sep before _60 after sep     \
  before _61 after sep before _62 after sep before _63 after sep before _64 after sep     \
  before _65 after sep before _66 after sep before _67 after sep before _68 after sep     \
  before _69 after sep before _70 after sep before _71 after sep before _72 after sep     \
  before _73 after sep before _74 after sep before _75 after sep before _76 after sep     \
  before _77 after sep before _78 after sep before _79 after sep before _80 after sep     \
  before _81 after sep before _82 after sep before _83 after sep before _84 after sep     \
  before _85 after sep before _86 after sep before _87 after sep before _88 after sep     \
  before _89 after sep before _90 after sep before _91 after sep before _92 after sep     \
  before _93 after sep before _94 after sep before _95 after sep before _96 after sep     \
  before _97 after sep before _98 after sep before _99 after sep before _100 after sep    \
  before _101 after sep before _102 after sep before _103 after sep before _104 after sep \
  before _105 after sep before _106 after sep before _107 after sep before _108 after sep \
  before _109 after sep before _110 after sep before _111 after sep before _112 after sep \
  before _113 after sep before _114 after sep before _115 after sep before _116 after sep \
  before _117 after sep before _118 after sep before _119 after sep before _120 after sep \
  before _121 after sep before _122 after sep before _123 after sep before _124 after sep \
  before _125 after sep before _126 after sep before _127 after sep before _128 after sep \
  before _129 after sep before _130 after sep before _131 after sep before _132 after sep \
  before _133 after sep before _134 after sep before _135 after sep before _136 after sep \
  before _137 after sep before _138 after sep before _139 after sep before _140 after sep \
  before _141 after sep before _142 after sep before _143 after sep before _144 after sep \
  before _145 after sep before _146 after sep before _147 after sep before _148 after sep \
  before _149 after sep before _150 after sep before _151 after sep before _152 after sep \
  before _153 after sep before _154 after sep before _155 after sep before _156 after sep \
  before _157 after sep before _158 after sep before _159 after sep before _160 after sep \
  before _161 after sep before _162 after sep before _163 after sep before _164 after sep \
  before _165 after sep before _166 after sep before _167 after sep before _168 after sep \
  before _169 after sep before _170 after sep before _171 after sep before _172 after sep \
  before _173 after sep before _174 after sep before _175 after sep before _176 after sep \
  before _177 after sep before _178 after sep before _179 after sep before _180 after sep \
  before _181 after sep before _182 after sep before _183 after sep before _184 after sep \
  before _185 after sep before _186 after sep before _187 after sep before _188 after sep \
  before _189 after sep before _190 after sep before _191 after sep before _192 after sep \
  before _193 after sep before _194 after sep before _195 after sep before _196 after sep \
  before _197 after sep before _198 after sep before _199 after sep before _200 after sep \
  before _201 after sep before _202 after sep before _203 after sep before _204 after sep \
  before _205 after sep before _206 after sep before _207 after sep before _208 after sep \
  before _209 after sep before _210 after sep before _211 after sep before _212 after sep \
  before _213 after sep before _214 after sep before _215 after sep before _216 after sep \
  before _217 after sep before _218 after sep before _219 after sep before _220 after sep \
  before _221 after sep before _222 after sep before _223 after sep before _224 after sep \
  before _225 after sep before _226 after sep before _227 after sep before _228 after sep \
  before _229 after sep before _230 after sep before _231 after sep before _232 after sep \
  before _233 after sep before _234 after sep before _235 after sep before _236 after sep \
  before _237 after sep before _238 after sep before _239 after sep before _240 after sep \
  before _241 after sep before _242 after sep before _243 after sep before _244 after sep \
  before _245 after sep before _246 after sep before _247 after sep before _248 after sep \
  before _249 after sep before _250 after sep before _251 after sep before _252 after sep \
  before _253 after sep before _254 after sep before _255 after sep before _256 after

#define JLN_MP_XS_512(before, after, sep)                                                 \
  before _1 after sep before _2 after sep before _3 after sep before _4 after sep         \
  before _5 after sep before _6 after sep before _7 after sep before _8 after sep         \
  before _9 after sep before _10 after sep before _11 after sep before _12 after sep      \
  before _13 after sep before _14 after sep before _15 after sep before _16 after sep     \
  before _17 after sep before _18 after sep before _19 after sep before _20 after sep     \
  before _21 after sep before _22 after sep before _23 after sep before _24 after sep     \
  before _25 after sep before _26 after sep before _27 after sep before _28 after sep     \
  before _29 after sep before _30 after sep before _31 after sep before _32 after sep     \
  before _33 after sep before _34 after sep before _35 after sep before _36 after sep     \
  before _37 after sep before _38 after sep before _39 after sep before _40 after sep     \
  before _41 after sep before _42 after sep before _43 after sep before _44 after sep     \
  before _45 after sep before _46 after sep before _47 after sep before _48 after sep     \
  before _49 after sep before _50 after sep before _51 after sep before _52 after sep     \
  before _53 after sep before _54 after sep before _55 after sep before _56 after sep     \
  before _57 after sep before _58 after sep before _59 after sep before _60 after sep     \
  before _61 after sep before _62 after sep before _63 after sep before _64 after sep     \
  before _65 after sep before _66 after sep before _67 after sep before _68 after sep     \
  before _69 after sep before _70 after sep before _71 after sep before _72 after sep     \
  before _73 after sep before _74 after sep before _75 after sep before _76 after sep     \
  before _77 after sep before _78 after sep before _79 after sep before _80 after sep     \
  before _81 after sep before _82 after sep before _83 after sep before _84 after sep     \
  before _85 after sep before _86 after sep before _87 after sep before _88 after sep     \
  before _89 after sep before _90 after sep before _91 after sep before _92 after sep     \
  before _93 after sep before _94 after sep before _95 after sep before _96 after sep     \
  before _97 after sep before _98 after sep before _99 after sep before _100 after sep    \
  before _101 after sep before _102 after sep before _103 after sep before _104 after sep \
  before _105 after sep before _106 after sep before _107 after sep before _108 after sep \
  before _109 after sep before _110 after sep before _111 after sep before _112 after sep \
  before _113 after sep before _114 after sep before _115 after sep before _116 after sep \
  before _117 after sep before _118 after sep before _119 after sep before _120 after sep \
  before _121 after sep before _122 after sep before _123 after sep before _124 after sep \
  before _125 after sep before _126 after sep before _127 after sep before _128 after sep \
  before _129 after sep before _130 after sep before _131 after sep before _132 after sep \
  before _133 after sep before _134 after sep before _135 after sep before _136 after sep \
  before _137 after sep before _138 after sep before _139 after sep before _140 after sep \
  before _141 after sep before _142 after sep before _143 after sep before _144 after sep \
  before _145 after sep before _146 after sep before _147 after sep before _148 after sep \
  before _149 after sep before _150 after sep before _151 after sep before _152 after sep \
  before _153 after sep before _154 after sep before _155 after sep before _156 after sep \
  before _157 after sep before _158 after sep before _159 after sep before _160 after sep \
  before _161 after sep before _162 after sep before _163 after sep before _164 after sep \
  before _165 after sep before _166 after sep before _167 after sep before _168 after sep \
  before _169 after sep before _170 after sep before _171 after sep before _172 after sep \
  before _173 after sep before _174 after sep before _175 after sep before _176 after sep \
  before _177 after sep before _178 after sep before _179 after sep before _180 after sep \
  before _181 after sep before _182 after sep before _183 after sep before _184 after sep \
  before _185 after sep before _186 after sep before _187 after sep before _188 after sep \
  before _189 after sep before _190 after sep before _191 after sep before _192 after sep \
  before _193 after sep before _194 after sep before _195 after sep before _196 after sep \
  before _197 after sep before _198 after sep before _199 after sep before _200 after sep \
  before _201 after sep before _202 after sep before _203 after sep before _204 after sep \
  before _205 after sep before _206 after sep before _207 after sep before _208 after sep \
  before _209 after sep before _210 after sep before _211 after sep before _212 after sep \
  before _213 after sep before _214 after sep before _215 after sep before _216 after sep \
  before _217 after sep before _218 after sep before _219 after sep before _220 after sep \
  before _221 after sep before _222 after sep before _223 after sep before _224 after sep \
  before _225 after sep before _226 after sep before _227 after sep before _228 after sep \
  before _229 after sep before _230 after sep before _231 after sep before _232 after sep \
  before _233 after sep before _234 after sep before _235 after sep before _236 after sep \
  before _237 after sep before _238 after sep before _239 after sep before _240 after sep \
  before _241 after sep before _242 after sep before _243 after sep before _244 after sep \
  before _245 after sep before _246 after sep before _247 after sep before _248 after sep \
  before _249 after sep before _250 after sep before _251 after sep before _252 after sep \
  before _253 after sep before _254 after sep before _255 after sep before _256 after sep \
  before _257 after sep before _258 after sep before _259 after sep before _260 after sep \
  before _261 after sep before _262 after sep before _263 after sep before _264 after sep \
  before _265 after sep before _266 after sep before _267 after sep before _268 after sep \
  before _269 after sep before _270 after sep before _271 after sep before _272 after sep \
  before _273 after sep before _274 after sep before _275 after sep before _276 after sep \
  before _277 after sep before _278 after sep before _279 after sep before _280 after sep \
  before _281 after sep before _282 after sep before _283 after sep before _284 after sep \
  before _285 after sep before _286 after sep before _287 after sep before _288 after sep \
  before _289 after sep before _290 after sep before _291 after sep before _292 after sep \
  before _293 after sep before _294 after sep before _295 after sep before _296 after sep \
  before _297 after sep before _298 after sep before _299 after sep before _300 after sep \
  before _301 after sep before _302 after sep before _303 after sep before _304 after sep \
  before _305 after sep before _306 after sep before _307 after sep before _308 after sep \
  before _309 after sep before _310 after sep before _311 after sep before _312 after sep \
  before _313 after sep before _314 after sep before _315 after sep before _316 after sep \
  before _317 after sep before _318 after sep before _319 after sep before _320 after sep \
  before _321 after sep before _322 after sep before _323 after sep before _324 after sep \
  before _325 after sep before _326 after sep before _327 after sep before _328 after sep \
  before _329 after sep before _330 after sep before _331 after sep before _332 after sep \
  before _333 after sep before _334 after sep before _335 after sep before _336 after sep \
  before _337 after sep before _338 after sep before _339 after sep before _340 after sep \
  before _341 after sep before _342 after sep before _343 after sep before _344 after sep \
  before _345 after sep before _346 after sep before _347 after sep before _348 after sep \
  before _349 after sep before _350 after sep before _351 after sep before _352 after sep \
  before _353 after sep before _354 after sep before _355 after sep before _356 after sep \
  before _357 after sep before _358 after sep before _359 after sep before _360 after sep \
  before _361 after sep before _362 after sep before _363 after sep before _364 after sep \
  before _365 after sep before _366 after sep before _367 after sep before _368 after sep \
  before _369 after sep before _370 after sep before _371 after sep before _372 after sep \
  before _373 after sep before _374 after sep before _375 after sep before _376 after sep \
  before _377 after sep before _378 after sep before _379 after sep before _380 after sep \
  before _381 after sep before _382 after sep before _383 after sep before _384 after sep \
  before _385 after sep before _386 after sep before _387 after sep before _388 after sep \
  before _389 after sep before _390 after sep before _391 after sep before _392 after sep \
  before _393 after sep before _394 after sep before _395 after sep before _396 after sep \
  before _397 after sep before _398 after sep before _399 after sep before _400 after sep \
  before _401 after sep before _402 after sep before _403 after sep before _404 after sep \
  before _405 after sep before _406 after sep before _407 after sep before _408 after sep \
  before _409 after sep before _410 after sep before _411 after sep before _412 after sep \
  before _413 after sep before _414 after sep before _415 after sep before _416 after sep \
  before _417 after sep before _418 after sep before _419 after sep before _420 after sep \
  before _421 after sep before _422 after sep before _423 after sep before _424 after sep \
  before _425 after sep before _426 after sep before _427 after sep before _428 after sep \
  before _429 after sep before _430 after sep before _431 after sep before _432 after sep \
  before _433 after sep before _434 after sep before _435 after sep before _436 after sep \
  before _437 after sep before _438 after sep before _439 after sep before _440 after sep \
  before _441 after sep before _442 after sep before _443 after sep before _444 after sep \
  before _445 after sep before _446 after sep before _447 after sep before _448 after sep \
  before _449 after sep before _450 after sep before _451 after sep before _452 after sep \
  before _453 after sep before _454 after sep before _455 after sep before _456 after sep \
  before _457 after sep before _458 after sep before _459 after sep before _460 after sep \
  before _461 after sep before _462 after sep before _463 after sep before _464 after sep \
  before _465 after sep before _466 after sep before _467 after sep before _468 after sep \
  before _469 after sep before _470 after sep before _471 after sep before _472 after sep \
  before _473 after sep before _474 after sep before _475 after sep before _476 after sep \
  before _477 after sep before _478 after sep before _479 after sep before _480 after sep \
  before _481 after sep before _482 after sep before _483 after sep before _484 after sep \
  before _485 after sep before _486 after sep before _487 after sep before _488 after sep \
  before _489 after sep before _490 after sep before _491 after sep before _492 after sep \
  before _493 after sep before _494 after sep before _495 after sep before _496 after sep \
  before _497 after sep before _498 after sep before _499 after sep before _500 after sep \
  before _501 after sep before _502 after sep before _503 after sep before _504 after sep \
  before _505 after sep before _506 after sep before _507 after sep before _508 after sep \
  before _509 after sep before _510 after sep before _511 after sep before _512 after

#define JLN_MP_XS_2_TO_512(before, after, sep)                                            \
                      before _2 after sep before _3 after sep before _4 after sep         \
  before _5 after sep before _6 after sep before _7 after sep before _8 after sep         \
  before _9 after sep before _10 after sep before _11 after sep before _12 after sep      \
  before _13 after sep before _14 after sep before _15 after sep before _16 after sep     \
  before _17 after sep before _18 after sep before _19 after sep before _20 after sep     \
  before _21 after sep before _22 after sep before _23 after sep before _24 after sep     \
  before _25 after sep before _26 after sep before _27 after sep before _28 after sep     \
  before _29 after sep before _30 after sep before _31 after sep before _32 after sep     \
  before _33 after sep before _34 after sep before _35 after sep before _36 after sep     \
  before _37 after sep before _38 after sep before _39 after sep before _40 after sep     \
  before _41 after sep before _42 after sep before _43 after sep before _44 after sep     \
  before _45 after sep before _46 after sep before _47 after sep before _48 after sep     \
  before _49 after sep before _50 after sep before _51 after sep before _52 after sep     \
  before _53 after sep before _54 after sep before _55 after sep before _56 after sep     \
  before _57 after sep before _58 after sep before _59 after sep before _60 after sep     \
  before _61 after sep before _62 after sep before _63 after sep before _64 after sep     \
  before _65 after sep before _66 after sep before _67 after sep before _68 after sep     \
  before _69 after sep before _70 after sep before _71 after sep before _72 after sep     \
  before _73 after sep before _74 after sep before _75 after sep before _76 after sep     \
  before _77 after sep before _78 after sep before _79 after sep before _80 after sep     \
  before _81 after sep before _82 after sep before _83 after sep before _84 after sep     \
  before _85 after sep before _86 after sep before _87 after sep before _88 after sep     \
  before _89 after sep before _90 after sep before _91 after sep before _92 after sep     \
  before _93 after sep before _94 after sep before _95 after sep before _96 after sep     \
  before _97 after sep before _98 after sep before _99 after sep before _100 after sep    \
  before _101 after sep before _102 after sep before _103 after sep before _104 after sep \
  before _105 after sep before _106 after sep before _107 after sep before _108 after sep \
  before _109 after sep before _110 after sep before _111 after sep before _112 after sep \
  before _113 after sep before _114 after sep before _115 after sep before _116 after sep \
  before _117 after sep before _118 after sep before _119 after sep before _120 after sep \
  before _121 after sep before _122 after sep before _123 after sep before _124 after sep \
  before _125 after sep before _126 after sep before _127 after sep before _128 after sep \
  before _129 after sep before _130 after sep before _131 after sep before _132 after sep \
  before _133 after sep before _134 after sep before _135 after sep before _136 after sep \
  before _137 after sep before _138 after sep before _139 after sep before _140 after sep \
  before _141 after sep before _142 after sep before _143 after sep before _144 after sep \
  before _145 after sep before _146 after sep before _147 after sep before _148 after sep \
  before _149 after sep before _150 after sep before _151 after sep before _152 after sep \
  before _153 after sep before _154 after sep before _155 after sep before _156 after sep \
  before _157 after sep before _158 after sep before _159 after sep before _160 after sep \
  before _161 after sep before _162 after sep before _163 after sep before _164 after sep \
  before _165 after sep before _166 after sep before _167 after sep before _168 after sep \
  before _169 after sep before _170 after sep before _171 after sep before _172 after sep \
  before _173 after sep before _174 after sep before _175 after sep before _176 after sep \
  before _177 after sep before _178 after sep before _179 after sep before _180 after sep \
  before _181 after sep before _182 after sep before _183 after sep before _184 after sep \
  before _185 after sep before _186 after sep before _187 after sep before _188 after sep \
  before _189 after sep before _190 after sep before _191 after sep before _192 after sep \
  before _193 after sep before _194 after sep before _195 after sep before _196 after sep \
  before _197 after sep before _198 after sep before _199 after sep before _200 after sep \
  before _201 after sep before _202 after sep before _203 after sep before _204 after sep \
  before _205 after sep before _206 after sep before _207 after sep before _208 after sep \
  before _209 after sep before _210 after sep before _211 after sep before _212 after sep \
  before _213 after sep before _214 after sep before _215 after sep before _216 after sep \
  before _217 after sep before _218 after sep before _219 after sep before _220 after sep \
  before _221 after sep before _222 after sep before _223 after sep before _224 after sep \
  before _225 after sep before _226 after sep before _227 after sep before _228 after sep \
  before _229 after sep before _230 after sep before _231 after sep before _232 after sep \
  before _233 after sep before _234 after sep before _235 after sep before _236 after sep \
  before _237 after sep before _238 after sep before _239 after sep before _240 after sep \
  before _241 after sep before _242 after sep before _243 after sep before _244 after sep \
  before _245 after sep before _246 after sep before _247 after sep before _248 after sep \
  before _249 after sep before _250 after sep before _251 after sep before _252 after sep \
  before _253 after sep before _254 after sep before _255 after sep before _256 after sep \
  before _257 after sep before _258 after sep before _259 after sep before _260 after sep \
  before _261 after sep before _262 after sep before _263 after sep before _264 after sep \
  before _265 after sep before _266 after sep before _267 after sep before _268 after sep \
  before _269 after sep before _270 after sep before _271 after sep before _272 after sep \
  before _273 after sep before _274 after sep before _275 after sep before _276 after sep \
  before _277 after sep before _278 after sep before _279 after sep before _280 after sep \
  before _281 after sep before _282 after sep before _283 after sep before _284 after sep \
  before _285 after sep before _286 after sep before _287 after sep before _288 after sep \
  before _289 after sep before _290 after sep before _291 after sep before _292 after sep \
  before _293 after sep before _294 after sep before _295 after sep before _296 after sep \
  before _297 after sep before _298 after sep before _299 after sep before _300 after sep \
  before _301 after sep before _302 after sep before _303 after sep before _304 after sep \
  before _305 after sep before _306 after sep before _307 after sep before _308 after sep \
  before _309 after sep before _310 after sep before _311 after sep before _312 after sep \
  before _313 after sep before _314 after sep before _315 after sep before _316 after sep \
  before _317 after sep before _318 after sep before _319 after sep before _320 after sep \
  before _321 after sep before _322 after sep before _323 after sep before _324 after sep \
  before _325 after sep before _326 after sep before _327 after sep before _328 after sep \
  before _329 after sep before _330 after sep before _331 after sep before _332 after sep \
  before _333 after sep before _334 after sep before _335 after sep before _336 after sep \
  before _337 after sep before _338 after sep before _339 after sep before _340 after sep \
  before _341 after sep before _342 after sep before _343 after sep before _344 after sep \
  before _345 after sep before _346 after sep before _347 after sep before _348 after sep \
  before _349 after sep before _350 after sep before _351 after sep before _352 after sep \
  before _353 after sep before _354 after sep before _355 after sep before _356 after sep \
  before _357 after sep before _358 after sep before _359 after sep before _360 after sep \
  before _361 after sep before _362 after sep before _363 after sep before _364 after sep \
  before _365 after sep before _366 after sep before _367 after sep before _368 after sep \
  before _369 after sep before _370 after sep before _371 after sep before _372 after sep \
  before _373 after sep before _374 after sep before _375 after sep before _376 after sep \
  before _377 after sep before _378 after sep before _379 after sep before _380 after sep \
  before _381 after sep before _382 after sep before _383 after sep before _384 after sep \
  before _385 after sep before _386 after sep before _387 after sep before _388 after sep \
  before _389 after sep before _390 after sep before _391 after sep before _392 after sep \
  before _393 after sep before _394 after sep before _395 after sep before _396 after sep \
  before _397 after sep before _398 after sep before _399 after sep before _400 after sep \
  before _401 after sep before _402 after sep before _403 after sep before _404 after sep \
  before _405 after sep before _406 after sep before _407 after sep before _408 after sep \
  before _409 after sep before _410 after sep before _411 after sep before _412 after sep \
  before _413 after sep before _414 after sep before _415 after sep before _416 after sep \
  before _417 after sep before _418 after sep before _419 after sep before _420 after sep \
  before _421 after sep before _422 after sep before _423 after sep before _424 after sep \
  before _425 after sep before _426 after sep before _427 after sep before _428 after sep \
  before _429 after sep before _430 after sep before _431 after sep before _432 after sep \
  before _433 after sep before _434 after sep before _435 after sep before _436 after sep \
  before _437 after sep before _438 after sep before _439 after sep before _440 after sep \
  before _441 after sep before _442 after sep before _443 after sep before _444 after sep \
  before _445 after sep before _446 after sep before _447 after sep before _448 after sep \
  before _449 after sep before _450 after sep before _451 after sep before _452 after sep \
  before _453 after sep before _454 after sep before _455 after sep before _456 after sep \
  before _457 after sep before _458 after sep before _459 after sep before _460 after sep \
  before _461 after sep before _462 after sep before _463 after sep before _464 after sep \
  before _465 after sep before _466 after sep before _467 after sep before _468 after sep \
  before _469 after sep before _470 after sep before _471 after sep before _472 after sep \
  before _473 after sep before _474 after sep before _475 after sep before _476 after sep \
  before _477 after sep before _478 after sep before _479 after sep before _480 after sep \
  before _481 after sep before _482 after sep before _483 after sep before _484 after sep \
  before _485 after sep before _486 after sep before _487 after sep before _488 after sep \
  before _489 after sep before _490 after sep before _491 after sep before _492 after sep \
  before _493 after sep before _494 after sep before _495 after sep before _496 after sep \
  before _497 after sep before _498 after sep before _499 after sep before _500 after sep \
  before _501 after sep before _502 after sep before _503 after sep before _504 after sep \
  before _505 after sep before _506 after sep before _507 after sep before _508 after sep \
  before _509 after sep before _510 after sep before _511 after sep before _512 after

#define JLN_MP_XS_1024(before, after, sep)                                                    \
  before _1 after sep before _2 after sep before _3 after sep before _4 after sep             \
  before _5 after sep before _6 after sep before _7 after sep before _8 after sep             \
  before _9 after sep before _10 after sep before _11 after sep before _12 after sep          \
  before _13 after sep before _14 after sep before _15 after sep before _16 after sep         \
  before _17 after sep before _18 after sep before _19 after sep before _20 after sep         \
  before _21 after sep before _22 after sep before _23 after sep before _24 after sep         \
  before _25 after sep before _26 after sep before _27 after sep before _28 after sep         \
  before _29 after sep before _30 after sep before _31 after sep before _32 after sep         \
  before _33 after sep before _34 after sep before _35 after sep before _36 after sep         \
  before _37 after sep before _38 after sep before _39 after sep before _40 after sep         \
  before _41 after sep before _42 after sep before _43 after sep before _44 after sep         \
  before _45 after sep before _46 after sep before _47 after sep before _48 after sep         \
  before _49 after sep before _50 after sep before _51 after sep before _52 after sep         \
  before _53 after sep before _54 after sep before _55 after sep before _56 after sep         \
  before _57 after sep before _58 after sep before _59 after sep before _60 after sep         \
  before _61 after sep before _62 after sep before _63 after sep before _64 after sep         \
  before _65 after sep before _66 after sep before _67 after sep before _68 after sep         \
  before _69 after sep before _70 after sep before _71 after sep before _72 after sep         \
  before _73 after sep before _74 after sep before _75 after sep before _76 after sep         \
  before _77 after sep before _78 after sep before _79 after sep before _80 after sep         \
  before _81 after sep before _82 after sep before _83 after sep before _84 after sep         \
  before _85 after sep before _86 after sep before _87 after sep before _88 after sep         \
  before _89 after sep before _90 after sep before _91 after sep before _92 after sep         \
  before _93 after sep before _94 after sep before _95 after sep before _96 after sep         \
  before _97 after sep before _98 after sep before _99 after sep before _100 after sep        \
  before _101 after sep before _102 after sep before _103 after sep before _104 after sep     \
  before _105 after sep before _106 after sep before _107 after sep before _108 after sep     \
  before _109 after sep before _110 after sep before _111 after sep before _112 after sep     \
  before _113 after sep before _114 after sep before _115 after sep before _116 after sep     \
  before _117 after sep before _118 after sep before _119 after sep before _120 after sep     \
  before _121 after sep before _122 after sep before _123 after sep before _124 after sep     \
  before _125 after sep before _126 after sep before _127 after sep before _128 after sep     \
  before _129 after sep before _130 after sep before _131 after sep before _132 after sep     \
  before _133 after sep before _134 after sep before _135 after sep before _136 after sep     \
  before _137 after sep before _138 after sep before _139 after sep before _140 after sep     \
  before _141 after sep before _142 after sep before _143 after sep before _144 after sep     \
  before _145 after sep before _146 after sep before _147 after sep before _148 after sep     \
  before _149 after sep before _150 after sep before _151 after sep before _152 after sep     \
  before _153 after sep before _154 after sep before _155 after sep before _156 after sep     \
  before _157 after sep before _158 after sep before _159 after sep before _160 after sep     \
  before _161 after sep before _162 after sep before _163 after sep before _164 after sep     \
  before _165 after sep before _166 after sep before _167 after sep before _168 after sep     \
  before _169 after sep before _170 after sep before _171 after sep before _172 after sep     \
  before _173 after sep before _174 after sep before _175 after sep before _176 after sep     \
  before _177 after sep before _178 after sep before _179 after sep before _180 after sep     \
  before _181 after sep before _182 after sep before _183 after sep before _184 after sep     \
  before _185 after sep before _186 after sep before _187 after sep before _188 after sep     \
  before _189 after sep before _190 after sep before _191 after sep before _192 after sep     \
  before _193 after sep before _194 after sep before _195 after sep before _196 after sep     \
  before _197 after sep before _198 after sep before _199 after sep before _200 after sep     \
  before _201 after sep before _202 after sep before _203 after sep before _204 after sep     \
  before _205 after sep before _206 after sep before _207 after sep before _208 after sep     \
  before _209 after sep before _210 after sep before _211 after sep before _212 after sep     \
  before _213 after sep before _214 after sep before _215 after sep before _216 after sep     \
  before _217 after sep before _218 after sep before _219 after sep before _220 after sep     \
  before _221 after sep before _222 after sep before _223 after sep before _224 after sep     \
  before _225 after sep before _226 after sep before _227 after sep before _228 after sep     \
  before _229 after sep before _230 after sep before _231 after sep before _232 after sep     \
  before _233 after sep before _234 after sep before _235 after sep before _236 after sep     \
  before _237 after sep before _238 after sep before _239 after sep before _240 after sep     \
  before _241 after sep before _242 after sep before _243 after sep before _244 after sep     \
  before _245 after sep before _246 after sep before _247 after sep before _248 after sep     \
  before _249 after sep before _250 after sep before _251 after sep before _252 after sep     \
  before _253 after sep before _254 after sep before _255 after sep before _256 after sep     \
  before _257 after sep before _258 after sep before _259 after sep before _260 after sep     \
  before _261 after sep before _262 after sep before _263 after sep before _264 after sep     \
  before _265 after sep before _266 after sep before _267 after sep before _268 after sep     \
  before _269 after sep before _270 after sep before _271 after sep before _272 after sep     \
  before _273 after sep before _274 after sep before _275 after sep before _276 after sep     \
  before _277 after sep before _278 after sep before _279 after sep before _280 after sep     \
  before _281 after sep before _282 after sep before _283 after sep before _284 after sep     \
  before _285 after sep before _286 after sep before _287 after sep before _288 after sep     \
  before _289 after sep before _290 after sep before _291 after sep before _292 after sep     \
  before _293 after sep before _294 after sep before _295 after sep before _296 after sep     \
  before _297 after sep before _298 after sep before _299 after sep before _300 after sep     \
  before _301 after sep before _302 after sep before _303 after sep before _304 after sep     \
  before _305 after sep before _306 after sep before _307 after sep before _308 after sep     \
  before _309 after sep before _310 after sep before _311 after sep before _312 after sep     \
  before _313 after sep before _314 after sep before _315 after sep before _316 after sep     \
  before _317 after sep before _318 after sep before _319 after sep before _320 after sep     \
  before _321 after sep before _322 after sep before _323 after sep before _324 after sep     \
  before _325 after sep before _326 after sep before _327 after sep before _328 after sep     \
  before _329 after sep before _330 after sep before _331 after sep before _332 after sep     \
  before _333 after sep before _334 after sep before _335 after sep before _336 after sep     \
  before _337 after sep before _338 after sep before _339 after sep before _340 after sep     \
  before _341 after sep before _342 after sep before _343 after sep before _344 after sep     \
  before _345 after sep before _346 after sep before _347 after sep before _348 after sep     \
  before _349 after sep before _350 after sep before _351 after sep before _352 after sep     \
  before _353 after sep before _354 after sep before _355 after sep before _356 after sep     \
  before _357 after sep before _358 after sep before _359 after sep before _360 after sep     \
  before _361 after sep before _362 after sep before _363 after sep before _364 after sep     \
  before _365 after sep before _366 after sep before _367 after sep before _368 after sep     \
  before _369 after sep before _370 after sep before _371 after sep before _372 after sep     \
  before _373 after sep before _374 after sep before _375 after sep before _376 after sep     \
  before _377 after sep before _378 after sep before _379 after sep before _380 after sep     \
  before _381 after sep before _382 after sep before _383 after sep before _384 after sep     \
  before _385 after sep before _386 after sep before _387 after sep before _388 after sep     \
  before _389 after sep before _390 after sep before _391 after sep before _392 after sep     \
  before _393 after sep before _394 after sep before _395 after sep before _396 after sep     \
  before _397 after sep before _398 after sep before _399 after sep before _400 after sep     \
  before _401 after sep before _402 after sep before _403 after sep before _404 after sep     \
  before _405 after sep before _406 after sep before _407 after sep before _408 after sep     \
  before _409 after sep before _410 after sep before _411 after sep before _412 after sep     \
  before _413 after sep before _414 after sep before _415 after sep before _416 after sep     \
  before _417 after sep before _418 after sep before _419 after sep before _420 after sep     \
  before _421 after sep before _422 after sep before _423 after sep before _424 after sep     \
  before _425 after sep before _426 after sep before _427 after sep before _428 after sep     \
  before _429 after sep before _430 after sep before _431 after sep before _432 after sep     \
  before _433 after sep before _434 after sep before _435 after sep before _436 after sep     \
  before _437 after sep before _438 after sep before _439 after sep before _440 after sep     \
  before _441 after sep before _442 after sep before _443 after sep before _444 after sep     \
  before _445 after sep before _446 after sep before _447 after sep before _448 after sep     \
  before _449 after sep before _450 after sep before _451 after sep before _452 after sep     \
  before _453 after sep before _454 after sep before _455 after sep before _456 after sep     \
  before _457 after sep before _458 after sep before _459 after sep before _460 after sep     \
  before _461 after sep before _462 after sep before _463 after sep before _464 after sep     \
  before _465 after sep before _466 after sep before _467 after sep before _468 after sep     \
  before _469 after sep before _470 after sep before _471 after sep before _472 after sep     \
  before _473 after sep before _474 after sep before _475 after sep before _476 after sep     \
  before _477 after sep before _478 after sep before _479 after sep before _480 after sep     \
  before _481 after sep before _482 after sep before _483 after sep before _484 after sep     \
  before _485 after sep before _486 after sep before _487 after sep before _488 after sep     \
  before _489 after sep before _490 after sep before _491 after sep before _492 after sep     \
  before _493 after sep before _494 after sep before _495 after sep before _496 after sep     \
  before _497 after sep before _498 after sep before _499 after sep before _500 after sep     \
  before _501 after sep before _502 after sep before _503 after sep before _504 after sep     \
  before _505 after sep before _506 after sep before _507 after sep before _508 after sep     \
  before _509 after sep before _510 after sep before _511 after sep before _512 after sep     \
  before _513 after sep before _514 after sep before _515 after sep before _516 after sep     \
  before _517 after sep before _518 after sep before _519 after sep before _520 after sep     \
  before _521 after sep before _522 after sep before _523 after sep before _524 after sep     \
  before _525 after sep before _526 after sep before _527 after sep before _528 after sep     \
  before _529 after sep before _530 after sep before _531 after sep before _532 after sep     \
  before _533 after sep before _534 after sep before _535 after sep before _536 after sep     \
  before _537 after sep before _538 after sep before _539 after sep before _540 after sep     \
  before _541 after sep before _542 after sep before _543 after sep before _544 after sep     \
  before _545 after sep before _546 after sep before _547 after sep before _548 after sep     \
  before _549 after sep before _550 after sep before _551 after sep before _552 after sep     \
  before _553 after sep before _554 after sep before _555 after sep before _556 after sep     \
  before _557 after sep before _558 after sep before _559 after sep before _560 after sep     \
  before _561 after sep before _562 after sep before _563 after sep before _564 after sep     \
  before _565 after sep before _566 after sep before _567 after sep before _568 after sep     \
  before _569 after sep before _570 after sep before _571 after sep before _572 after sep     \
  before _573 after sep before _574 after sep before _575 after sep before _576 after sep     \
  before _577 after sep before _578 after sep before _579 after sep before _580 after sep     \
  before _581 after sep before _582 after sep before _583 after sep before _584 after sep     \
  before _585 after sep before _586 after sep before _587 after sep before _588 after sep     \
  before _589 after sep before _590 after sep before _591 after sep before _592 after sep     \
  before _593 after sep before _594 after sep before _595 after sep before _596 after sep     \
  before _597 after sep before _598 after sep before _599 after sep before _600 after sep     \
  before _601 after sep before _602 after sep before _603 after sep before _604 after sep     \
  before _605 after sep before _606 after sep before _607 after sep before _608 after sep     \
  before _609 after sep before _610 after sep before _611 after sep before _612 after sep     \
  before _613 after sep before _614 after sep before _615 after sep before _616 after sep     \
  before _617 after sep before _618 after sep before _619 after sep before _620 after sep     \
  before _621 after sep before _622 after sep before _623 after sep before _624 after sep     \
  before _625 after sep before _626 after sep before _627 after sep before _628 after sep     \
  before _629 after sep before _630 after sep before _631 after sep before _632 after sep     \
  before _633 after sep before _634 after sep before _635 after sep before _636 after sep     \
  before _637 after sep before _638 after sep before _639 after sep before _640 after sep     \
  before _641 after sep before _642 after sep before _643 after sep before _644 after sep     \
  before _645 after sep before _646 after sep before _647 after sep before _648 after sep     \
  before _649 after sep before _650 after sep before _651 after sep before _652 after sep     \
  before _653 after sep before _654 after sep before _655 after sep before _656 after sep     \
  before _657 after sep before _658 after sep before _659 after sep before _660 after sep     \
  before _661 after sep before _662 after sep before _663 after sep before _664 after sep     \
  before _665 after sep before _666 after sep before _667 after sep before _668 after sep     \
  before _669 after sep before _670 after sep before _671 after sep before _672 after sep     \
  before _673 after sep before _674 after sep before _675 after sep before _676 after sep     \
  before _677 after sep before _678 after sep before _679 after sep before _680 after sep     \
  before _681 after sep before _682 after sep before _683 after sep before _684 after sep     \
  before _685 after sep before _686 after sep before _687 after sep before _688 after sep     \
  before _689 after sep before _690 after sep before _691 after sep before _692 after sep     \
  before _693 after sep before _694 after sep before _695 after sep before _696 after sep     \
  before _697 after sep before _698 after sep before _699 after sep before _700 after sep     \
  before _701 after sep before _702 after sep before _703 after sep before _704 after sep     \
  before _705 after sep before _706 after sep before _707 after sep before _708 after sep     \
  before _709 after sep before _710 after sep before _711 after sep before _712 after sep     \
  before _713 after sep before _714 after sep before _715 after sep before _716 after sep     \
  before _717 after sep before _718 after sep before _719 after sep before _720 after sep     \
  before _721 after sep before _722 after sep before _723 after sep before _724 after sep     \
  before _725 after sep before _726 after sep before _727 after sep before _728 after sep     \
  before _729 after sep before _730 after sep before _731 after sep before _732 after sep     \
  before _733 after sep before _734 after sep before _735 after sep before _736 after sep     \
  before _737 after sep before _738 after sep before _739 after sep before _740 after sep     \
  before _741 after sep before _742 after sep before _743 after sep before _744 after sep     \
  before _745 after sep before _746 after sep before _747 after sep before _748 after sep     \
  before _749 after sep before _750 after sep before _751 after sep before _752 after sep     \
  before _753 after sep before _754 after sep before _755 after sep before _756 after sep     \
  before _757 after sep before _758 after sep before _759 after sep before _760 after sep     \
  before _761 after sep before _762 after sep before _763 after sep before _764 after sep     \
  before _765 after sep before _766 after sep before _767 after sep before _768 after sep     \
  before _769 after sep before _770 after sep before _771 after sep before _772 after sep     \
  before _773 after sep before _774 after sep before _775 after sep before _776 after sep     \
  before _777 after sep before _778 after sep before _779 after sep before _780 after sep     \
  before _781 after sep before _782 after sep before _783 after sep before _784 after sep     \
  before _785 after sep before _786 after sep before _787 after sep before _788 after sep     \
  before _789 after sep before _790 after sep before _791 after sep before _792 after sep     \
  before _793 after sep before _794 after sep before _795 after sep before _796 after sep     \
  before _797 after sep before _798 after sep before _799 after sep before _800 after sep     \
  before _801 after sep before _802 after sep before _803 after sep before _804 after sep     \
  before _805 after sep before _806 after sep before _807 after sep before _808 after sep     \
  before _809 after sep before _810 after sep before _811 after sep before _812 after sep     \
  before _813 after sep before _814 after sep before _815 after sep before _816 after sep     \
  before _817 after sep before _818 after sep before _819 after sep before _820 after sep     \
  before _821 after sep before _822 after sep before _823 after sep before _824 after sep     \
  before _825 after sep before _826 after sep before _827 after sep before _828 after sep     \
  before _829 after sep before _830 after sep before _831 after sep before _832 after sep     \
  before _833 after sep before _834 after sep before _835 after sep before _836 after sep     \
  before _837 after sep before _838 after sep before _839 after sep before _840 after sep     \
  before _841 after sep before _842 after sep before _843 after sep before _844 after sep     \
  before _845 after sep before _846 after sep before _847 after sep before _848 after sep     \
  before _849 after sep before _850 after sep before _851 after sep before _852 after sep     \
  before _853 after sep before _854 after sep before _855 after sep before _856 after sep     \
  before _857 after sep before _858 after sep before _859 after sep before _860 after sep     \
  before _861 after sep before _862 after sep before _863 after sep before _864 after sep     \
  before _865 after sep before _866 after sep before _867 after sep before _868 after sep     \
  before _869 after sep before _870 after sep before _871 after sep before _872 after sep     \
  before _873 after sep before _874 after sep before _875 after sep before _876 after sep     \
  before _877 after sep before _878 after sep before _879 after sep before _880 after sep     \
  before _881 after sep before _882 after sep before _883 after sep before _884 after sep     \
  before _885 after sep before _886 after sep before _887 after sep before _888 after sep     \
  before _889 after sep before _890 after sep before _891 after sep before _892 after sep     \
  before _893 after sep before _894 after sep before _895 after sep before _896 after sep     \
  before _897 after sep before _898 after sep before _899 after sep before _900 after sep     \
  before _901 after sep before _902 after sep before _903 after sep before _904 after sep     \
  before _905 after sep before _906 after sep before _907 after sep before _908 after sep     \
  before _909 after sep before _910 after sep before _911 after sep before _912 after sep     \
  before _913 after sep before _914 after sep before _915 after sep before _916 after sep     \
  before _917 after sep before _918 after sep before _919 after sep before _920 after sep     \
  before _921 after sep before _922 after sep before _923 after sep before _924 after sep     \
  before _925 after sep before _926 after sep before _927 after sep before _928 after sep     \
  before _929 after sep before _930 after sep before _931 after sep before _932 after sep     \
  before _933 after sep before _934 after sep before _935 after sep before _936 after sep     \
  before _937 after sep before _938 after sep before _939 after sep before _940 after sep     \
  before _941 after sep before _942 after sep before _943 after sep before _944 after sep     \
  before _945 after sep before _946 after sep before _947 after sep before _948 after sep     \
  before _949 after sep before _950 after sep before _951 after sep before _952 after sep     \
  before _953 after sep before _954 after sep before _955 after sep before _956 after sep     \
  before _957 after sep before _958 after sep before _959 after sep before _960 after sep     \
  before _961 after sep before _962 after sep before _963 after sep before _964 after sep     \
  before _965 after sep before _966 after sep before _967 after sep before _968 after sep     \
  before _969 after sep before _970 after sep before _971 after sep before _972 after sep     \
  before _973 after sep before _974 after sep before _975 after sep before _976 after sep     \
  before _977 after sep before _978 after sep before _979 after sep before _980 after sep     \
  before _981 after sep before _982 after sep before _983 after sep before _984 after sep     \
  before _985 after sep before _986 after sep before _987 after sep before _988 after sep     \
  before _989 after sep before _990 after sep before _991 after sep before _992 after sep     \
  before _993 after sep before _994 after sep before _995 after sep before _996 after sep     \
  before _997 after sep before _998 after sep before _999 after sep before _1000 after sep    \
  before _1001 after sep before _1002 after sep before _1003 after sep before _1004 after sep \
  before _1005 after sep before _1006 after sep before _1007 after sep before _1008 after sep \
  before _1009 after sep before _1010 after sep before _1011 after sep before _1012 after sep \
  before _1013 after sep before _1014 after sep before _1015 after sep before _1016 after sep \
  before _1017 after sep before _1018 after sep before _1019 after sep before _1020 after sep \
  before _1021 after sep before _1022 after sep before _1023 after sep before _1024 after

#define JLN_MP_XS_2_TO_1024(before, after, sep)                                               \
                      before _2 after sep before _3 after sep before _4 after sep             \
  before _5 after sep before _6 after sep before _7 after sep before _8 after sep             \
  before _9 after sep before _10 after sep before _11 after sep before _12 after sep          \
  before _13 after sep before _14 after sep before _15 after sep before _16 after sep         \
  before _17 after sep before _18 after sep before _19 after sep before _20 after sep         \
  before _21 after sep before _22 after sep before _23 after sep before _24 after sep         \
  before _25 after sep before _26 after sep before _27 after sep before _28 after sep         \
  before _29 after sep before _30 after sep before _31 after sep before _32 after sep         \
  before _33 after sep before _34 after sep before _35 after sep before _36 after sep         \
  before _37 after sep before _38 after sep before _39 after sep before _40 after sep         \
  before _41 after sep before _42 after sep before _43 after sep before _44 after sep         \
  before _45 after sep before _46 after sep before _47 after sep before _48 after sep         \
  before _49 after sep before _50 after sep before _51 after sep before _52 after sep         \
  before _53 after sep before _54 after sep before _55 after sep before _56 after sep         \
  before _57 after sep before _58 after sep before _59 after sep before _60 after sep         \
  before _61 after sep before _62 after sep before _63 after sep before _64 after sep         \
  before _65 after sep before _66 after sep before _67 after sep before _68 after sep         \
  before _69 after sep before _70 after sep before _71 after sep before _72 after sep         \
  before _73 after sep before _74 after sep before _75 after sep before _76 after sep         \
  before _77 after sep before _78 after sep before _79 after sep before _80 after sep         \
  before _81 after sep before _82 after sep before _83 after sep before _84 after sep         \
  before _85 after sep before _86 after sep before _87 after sep before _88 after sep         \
  before _89 after sep before _90 after sep before _91 after sep before _92 after sep         \
  before _93 after sep before _94 after sep before _95 after sep before _96 after sep         \
  before _97 after sep before _98 after sep before _99 after sep before _100 after sep        \
  before _101 after sep before _102 after sep before _103 after sep before _104 after sep     \
  before _105 after sep before _106 after sep before _107 after sep before _108 after sep     \
  before _109 after sep before _110 after sep before _111 after sep before _112 after sep     \
  before _113 after sep before _114 after sep before _115 after sep before _116 after sep     \
  before _117 after sep before _118 after sep before _119 after sep before _120 after sep     \
  before _121 after sep before _122 after sep before _123 after sep before _124 after sep     \
  before _125 after sep before _126 after sep before _127 after sep before _128 after sep     \
  before _129 after sep before _130 after sep before _131 after sep before _132 after sep     \
  before _133 after sep before _134 after sep before _135 after sep before _136 after sep     \
  before _137 after sep before _138 after sep before _139 after sep before _140 after sep     \
  before _141 after sep before _142 after sep before _143 after sep before _144 after sep     \
  before _145 after sep before _146 after sep before _147 after sep before _148 after sep     \
  before _149 after sep before _150 after sep before _151 after sep before _152 after sep     \
  before _153 after sep before _154 after sep before _155 after sep before _156 after sep     \
  before _157 after sep before _158 after sep before _159 after sep before _160 after sep     \
  before _161 after sep before _162 after sep before _163 after sep before _164 after sep     \
  before _165 after sep before _166 after sep before _167 after sep before _168 after sep     \
  before _169 after sep before _170 after sep before _171 after sep before _172 after sep     \
  before _173 after sep before _174 after sep before _175 after sep before _176 after sep     \
  before _177 after sep before _178 after sep before _179 after sep before _180 after sep     \
  before _181 after sep before _182 after sep before _183 after sep before _184 after sep     \
  before _185 after sep before _186 after sep before _187 after sep before _188 after sep     \
  before _189 after sep before _190 after sep before _191 after sep before _192 after sep     \
  before _193 after sep before _194 after sep before _195 after sep before _196 after sep     \
  before _197 after sep before _198 after sep before _199 after sep before _200 after sep     \
  before _201 after sep before _202 after sep before _203 after sep before _204 after sep     \
  before _205 after sep before _206 after sep before _207 after sep before _208 after sep     \
  before _209 after sep before _210 after sep before _211 after sep before _212 after sep     \
  before _213 after sep before _214 after sep before _215 after sep before _216 after sep     \
  before _217 after sep before _218 after sep before _219 after sep before _220 after sep     \
  before _221 after sep before _222 after sep before _223 after sep before _224 after sep     \
  before _225 after sep before _226 after sep before _227 after sep before _228 after sep     \
  before _229 after sep before _230 after sep before _231 after sep before _232 after sep     \
  before _233 after sep before _234 after sep before _235 after sep before _236 after sep     \
  before _237 after sep before _238 after sep before _239 after sep before _240 after sep     \
  before _241 after sep before _242 after sep before _243 after sep before _244 after sep     \
  before _245 after sep before _246 after sep before _247 after sep before _248 after sep     \
  before _249 after sep before _250 after sep before _251 after sep before _252 after sep     \
  before _253 after sep before _254 after sep before _255 after sep before _256 after sep     \
  before _257 after sep before _258 after sep before _259 after sep before _260 after sep     \
  before _261 after sep before _262 after sep before _263 after sep before _264 after sep     \
  before _265 after sep before _266 after sep before _267 after sep before _268 after sep     \
  before _269 after sep before _270 after sep before _271 after sep before _272 after sep     \
  before _273 after sep before _274 after sep before _275 after sep before _276 after sep     \
  before _277 after sep before _278 after sep before _279 after sep before _280 after sep     \
  before _281 after sep before _282 after sep before _283 after sep before _284 after sep     \
  before _285 after sep before _286 after sep before _287 after sep before _288 after sep     \
  before _289 after sep before _290 after sep before _291 after sep before _292 after sep     \
  before _293 after sep before _294 after sep before _295 after sep before _296 after sep     \
  before _297 after sep before _298 after sep before _299 after sep before _300 after sep     \
  before _301 after sep before _302 after sep before _303 after sep before _304 after sep     \
  before _305 after sep before _306 after sep before _307 after sep before _308 after sep     \
  before _309 after sep before _310 after sep before _311 after sep before _312 after sep     \
  before _313 after sep before _314 after sep before _315 after sep before _316 after sep     \
  before _317 after sep before _318 after sep before _319 after sep before _320 after sep     \
  before _321 after sep before _322 after sep before _323 after sep before _324 after sep     \
  before _325 after sep before _326 after sep before _327 after sep before _328 after sep     \
  before _329 after sep before _330 after sep before _331 after sep before _332 after sep     \
  before _333 after sep before _334 after sep before _335 after sep before _336 after sep     \
  before _337 after sep before _338 after sep before _339 after sep before _340 after sep     \
  before _341 after sep before _342 after sep before _343 after sep before _344 after sep     \
  before _345 after sep before _346 after sep before _347 after sep before _348 after sep     \
  before _349 after sep before _350 after sep before _351 after sep before _352 after sep     \
  before _353 after sep before _354 after sep before _355 after sep before _356 after sep     \
  before _357 after sep before _358 after sep before _359 after sep before _360 after sep     \
  before _361 after sep before _362 after sep before _363 after sep before _364 after sep     \
  before _365 after sep before _366 after sep before _367 after sep before _368 after sep     \
  before _369 after sep before _370 after sep before _371 after sep before _372 after sep     \
  before _373 after sep before _374 after sep before _375 after sep before _376 after sep     \
  before _377 after sep before _378 after sep before _379 after sep before _380 after sep     \
  before _381 after sep before _382 after sep before _383 after sep before _384 after sep     \
  before _385 after sep before _386 after sep before _387 after sep before _388 after sep     \
  before _389 after sep before _390 after sep before _391 after sep before _392 after sep     \
  before _393 after sep before _394 after sep before _395 after sep before _396 after sep     \
  before _397 after sep before _398 after sep before _399 after sep before _400 after sep     \
  before _401 after sep before _402 after sep before _403 after sep before _404 after sep     \
  before _405 after sep before _406 after sep before _407 after sep before _408 after sep     \
  before _409 after sep before _410 after sep before _411 after sep before _412 after sep     \
  before _413 after sep before _414 after sep before _415 after sep before _416 after sep     \
  before _417 after sep before _418 after sep before _419 after sep before _420 after sep     \
  before _421 after sep before _422 after sep before _423 after sep before _424 after sep     \
  before _425 after sep before _426 after sep before _427 after sep before _428 after sep     \
  before _429 after sep before _430 after sep before _431 after sep before _432 after sep     \
  before _433 after sep before _434 after sep before _435 after sep before _436 after sep     \
  before _437 after sep before _438 after sep before _439 after sep before _440 after sep     \
  before _441 after sep before _442 after sep before _443 after sep before _444 after sep     \
  before _445 after sep before _446 after sep before _447 after sep before _448 after sep     \
  before _449 after sep before _450 after sep before _451 after sep before _452 after sep     \
  before _453 after sep before _454 after sep before _455 after sep before _456 after sep     \
  before _457 after sep before _458 after sep before _459 after sep before _460 after sep     \
  before _461 after sep before _462 after sep before _463 after sep before _464 after sep     \
  before _465 after sep before _466 after sep before _467 after sep before _468 after sep     \
  before _469 after sep before _470 after sep before _471 after sep before _472 after sep     \
  before _473 after sep before _474 after sep before _475 after sep before _476 after sep     \
  before _477 after sep before _478 after sep before _479 after sep before _480 after sep     \
  before _481 after sep before _482 after sep before _483 after sep before _484 after sep     \
  before _485 after sep before _486 after sep before _487 after sep before _488 after sep     \
  before _489 after sep before _490 after sep before _491 after sep before _492 after sep     \
  before _493 after sep before _494 after sep before _495 after sep before _496 after sep     \
  before _497 after sep before _498 after sep before _499 after sep before _500 after sep     \
  before _501 after sep before _502 after sep before _503 after sep before _504 after sep     \
  before _505 after sep before _506 after sep before _507 after sep before _508 after sep     \
  before _509 after sep before _510 after sep before _511 after sep before _512 after sep     \
  before _513 after sep before _514 after sep before _515 after sep before _516 after sep     \
  before _517 after sep before _518 after sep before _519 after sep before _520 after sep     \
  before _521 after sep before _522 after sep before _523 after sep before _524 after sep     \
  before _525 after sep before _526 after sep before _527 after sep before _528 after sep     \
  before _529 after sep before _530 after sep before _531 after sep before _532 after sep     \
  before _533 after sep before _534 after sep before _535 after sep before _536 after sep     \
  before _537 after sep before _538 after sep before _539 after sep before _540 after sep     \
  before _541 after sep before _542 after sep before _543 after sep before _544 after sep     \
  before _545 after sep before _546 after sep before _547 after sep before _548 after sep     \
  before _549 after sep before _550 after sep before _551 after sep before _552 after sep     \
  before _553 after sep before _554 after sep before _555 after sep before _556 after sep     \
  before _557 after sep before _558 after sep before _559 after sep before _560 after sep     \
  before _561 after sep before _562 after sep before _563 after sep before _564 after sep     \
  before _565 after sep before _566 after sep before _567 after sep before _568 after sep     \
  before _569 after sep before _570 after sep before _571 after sep before _572 after sep     \
  before _573 after sep before _574 after sep before _575 after sep before _576 after sep     \
  before _577 after sep before _578 after sep before _579 after sep before _580 after sep     \
  before _581 after sep before _582 after sep before _583 after sep before _584 after sep     \
  before _585 after sep before _586 after sep before _587 after sep before _588 after sep     \
  before _589 after sep before _590 after sep before _591 after sep before _592 after sep     \
  before _593 after sep before _594 after sep before _595 after sep before _596 after sep     \
  before _597 after sep before _598 after sep before _599 after sep before _600 after sep     \
  before _601 after sep before _602 after sep before _603 after sep before _604 after sep     \
  before _605 after sep before _606 after sep before _607 after sep before _608 after sep     \
  before _609 after sep before _610 after sep before _611 after sep before _612 after sep     \
  before _613 after sep before _614 after sep before _615 after sep before _616 after sep     \
  before _617 after sep before _618 after sep before _619 after sep before _620 after sep     \
  before _621 after sep before _622 after sep before _623 after sep before _624 after sep     \
  before _625 after sep before _626 after sep before _627 after sep before _628 after sep     \
  before _629 after sep before _630 after sep before _631 after sep before _632 after sep     \
  before _633 after sep before _634 after sep before _635 after sep before _636 after sep     \
  before _637 after sep before _638 after sep before _639 after sep before _640 after sep     \
  before _641 after sep before _642 after sep before _643 after sep before _644 after sep     \
  before _645 after sep before _646 after sep before _647 after sep before _648 after sep     \
  before _649 after sep before _650 after sep before _651 after sep before _652 after sep     \
  before _653 after sep before _654 after sep before _655 after sep before _656 after sep     \
  before _657 after sep before _658 after sep before _659 after sep before _660 after sep     \
  before _661 after sep before _662 after sep before _663 after sep before _664 after sep     \
  before _665 after sep before _666 after sep before _667 after sep before _668 after sep     \
  before _669 after sep before _670 after sep before _671 after sep before _672 after sep     \
  before _673 after sep before _674 after sep before _675 after sep before _676 after sep     \
  before _677 after sep before _678 after sep before _679 after sep before _680 after sep     \
  before _681 after sep before _682 after sep before _683 after sep before _684 after sep     \
  before _685 after sep before _686 after sep before _687 after sep before _688 after sep     \
  before _689 after sep before _690 after sep before _691 after sep before _692 after sep     \
  before _693 after sep before _694 after sep before _695 after sep before _696 after sep     \
  before _697 after sep before _698 after sep before _699 after sep before _700 after sep     \
  before _701 after sep before _702 after sep before _703 after sep before _704 after sep     \
  before _705 after sep before _706 after sep before _707 after sep before _708 after sep     \
  before _709 after sep before _710 after sep before _711 after sep before _712 after sep     \
  before _713 after sep before _714 after sep before _715 after sep before _716 after sep     \
  before _717 after sep before _718 after sep before _719 after sep before _720 after sep     \
  before _721 after sep before _722 after sep before _723 after sep before _724 after sep     \
  before _725 after sep before _726 after sep before _727 after sep before _728 after sep     \
  before _729 after sep before _730 after sep before _731 after sep before _732 after sep     \
  before _733 after sep before _734 after sep before _735 after sep before _736 after sep     \
  before _737 after sep before _738 after sep before _739 after sep before _740 after sep     \
  before _741 after sep before _742 after sep before _743 after sep before _744 after sep     \
  before _745 after sep before _746 after sep before _747 after sep before _748 after sep     \
  before _749 after sep before _750 after sep before _751 after sep before _752 after sep     \
  before _753 after sep before _754 after sep before _755 after sep before _756 after sep     \
  before _757 after sep before _758 after sep before _759 after sep before _760 after sep     \
  before _761 after sep before _762 after sep before _763 after sep before _764 after sep     \
  before _765 after sep before _766 after sep before _767 after sep before _768 after sep     \
  before _769 after sep before _770 after sep before _771 after sep before _772 after sep     \
  before _773 after sep before _774 after sep before _775 after sep before _776 after sep     \
  before _777 after sep before _778 after sep before _779 after sep before _780 after sep     \
  before _781 after sep before _782 after sep before _783 after sep before _784 after sep     \
  before _785 after sep before _786 after sep before _787 after sep before _788 after sep     \
  before _789 after sep before _790 after sep before _791 after sep before _792 after sep     \
  before _793 after sep before _794 after sep before _795 after sep before _796 after sep     \
  before _797 after sep before _798 after sep before _799 after sep before _800 after sep     \
  before _801 after sep before _802 after sep before _803 after sep before _804 after sep     \
  before _805 after sep before _806 after sep before _807 after sep before _808 after sep     \
  before _809 after sep before _810 after sep before _811 after sep before _812 after sep     \
  before _813 after sep before _814 after sep before _815 after sep before _816 after sep     \
  before _817 after sep before _818 after sep before _819 after sep before _820 after sep     \
  before _821 after sep before _822 after sep before _823 after sep before _824 after sep     \
  before _825 after sep before _826 after sep before _827 after sep before _828 after sep     \
  before _829 after sep before _830 after sep before _831 after sep before _832 after sep     \
  before _833 after sep before _834 after sep before _835 after sep before _836 after sep     \
  before _837 after sep before _838 after sep before _839 after sep before _840 after sep     \
  before _841 after sep before _842 after sep before _843 after sep before _844 after sep     \
  before _845 after sep before _846 after sep before _847 after sep before _848 after sep     \
  before _849 after sep before _850 after sep before _851 after sep before _852 after sep     \
  before _853 after sep before _854 after sep before _855 after sep before _856 after sep     \
  before _857 after sep before _858 after sep before _859 after sep before _860 after sep     \
  before _861 after sep before _862 after sep before _863 after sep before _864 after sep     \
  before _865 after sep before _866 after sep before _867 after sep before _868 after sep     \
  before _869 after sep before _870 after sep before _871 after sep before _872 after sep     \
  before _873 after sep before _874 after sep before _875 after sep before _876 after sep     \
  before _877 after sep before _878 after sep before _879 after sep before _880 after sep     \
  before _881 after sep before _882 after sep before _883 after sep before _884 after sep     \
  before _885 after sep before _886 after sep before _887 after sep before _888 after sep     \
  before _889 after sep before _890 after sep before _891 after sep before _892 after sep     \
  before _893 after sep before _894 after sep before _895 after sep before _896 after sep     \
  before _897 after sep before _898 after sep before _899 after sep before _900 after sep     \
  before _901 after sep before _902 after sep before _903 after sep before _904 after sep     \
  before _905 after sep before _906 after sep before _907 after sep before _908 after sep     \
  before _909 after sep before _910 after sep before _911 after sep before _912 after sep     \
  before _913 after sep before _914 after sep before _915 after sep before _916 after sep     \
  before _917 after sep before _918 after sep before _919 after sep before _920 after sep     \
  before _921 after sep before _922 after sep before _923 after sep before _924 after sep     \
  before _925 after sep before _926 after sep before _927 after sep before _928 after sep     \
  before _929 after sep before _930 after sep before _931 after sep before _932 after sep     \
  before _933 after sep before _934 after sep before _935 after sep before _936 after sep     \
  before _937 after sep before _938 after sep before _939 after sep before _940 after sep     \
  before _941 after sep before _942 after sep before _943 after sep before _944 after sep     \
  before _945 after sep before _946 after sep before _947 after sep before _948 after sep     \
  before _949 after sep before _950 after sep before _951 after sep before _952 after sep     \
  before _953 after sep before _954 after sep before _955 after sep before _956 after sep     \
  before _957 after sep before _958 after sep before _959 after sep before _960 after sep     \
  before _961 after sep before _962 after sep before _963 after sep before _964 after sep     \
  before _965 after sep before _966 after sep before _967 after sep before _968 after sep     \
  before _969 after sep before _970 after sep before _971 after sep before _972 after sep     \
  before _973 after sep before _974 after sep before _975 after sep before _976 after sep     \
  before _977 after sep before _978 after sep before _979 after sep before _980 after sep     \
  before _981 after sep before _982 after sep before _983 after sep before _984 after sep     \
  before _985 after sep before _986 after sep before _987 after sep before _988 after sep     \
  before _989 after sep before _990 after sep before _991 after sep before _992 after sep     \
  before _993 after sep before _994 after sep before _995 after sep before _996 after sep     \
  before _997 after sep before _998 after sep before _999 after sep before _1000 after sep    \
  before _1001 after sep before _1002 after sep before _1003 after sep before _1004 after sep \
  before _1005 after sep before _1006 after sep before _1007 after sep before _1008 after sep \
  before _1009 after sep before _1010 after sep before _1011 after sep before _1012 after sep \
  before _1013 after sep before _1014 after sep before _1015 after sep before _1016 after sep \
  before _1017 after sep before _1018 after sep before _1019 after sep before _1020 after sep \
  before _1021 after sep before _1022 after sep before _1023 after sep before _1024 after

#define JLN_MP_RXS_0(before, after, sep)

#define JLN_MP_RXS_1(before, after, sep) \
  before _1 after

#define JLN_MP_RXS_2(before, after, sep) \
  before _2 after sep before _1 after

#define JLN_MP_RXS_3(before, after, sep) \
  before _3 after sep before _2 after sep before _1 after

#define JLN_MP_RXS_4(before, after, sep) \
  before _4 after sep before _3 after sep before _2 after sep before _1 after

#define JLN_MP_RXS_5(before, after, sep) \
  before _5 after sep before _4 after sep before _3 after sep before _2 after sep \
  before _1 after

#define JLN_MP_RXS_6(before, after, sep)                                          \
  before _6 after sep before _5 after sep before _4 after sep before _3 after sep \
  before _2 after sep before _1 after

#define JLN_MP_RXS_7(before, after, sep)                                          \
  before _7 after sep before _6 after sep before _5 after sep before _4 after sep \
  before _3 after sep before _2 after sep before _1 after

#define JLN_MP_RXS_8(before, after, sep)                                          \
  before _8 after sep before _7 after sep before _6 after sep before _5 after sep \
  before _4 after sep before _3 after sep before _2 after sep before _1 after

#define JLN_MP_RXS_16(before, after, sep)                                             \
  before _16 after sep before _15 after sep before _14 after sep before _13 after sep \
  before _12 after sep before _11 after sep before _10 after sep before _9 after sep  \
  before _8 after sep before _7 after sep before _6 after sep before _5 after sep     \
  before _4 after sep before _3 after sep before _2 after sep before _1 after

#define JLN_MP_RXS_32(before, after, sep)                                             \
  before _32 after sep before _31 after sep before _30 after sep before _29 after sep \
  before _28 after sep before _27 after sep before _26 after sep before _25 after sep \
  before _24 after sep before _23 after sep before _22 after sep before _21 after sep \
  before _20 after sep before _19 after sep before _18 after sep before _17 after sep \
  before _16 after sep before _15 after sep before _14 after sep before _13 after sep \
  before _12 after sep before _11 after sep before _10 after sep before _9 after sep  \
  before _8 after sep before _7 after sep before _6 after sep before _5 after sep     \
  before _4 after sep before _3 after sep before _2 after sep before _1 after

#define JLN_MP_RXS_64(before, after, sep)                                             \
  before _64 after sep before _63 after sep before _62 after sep before _61 after sep \
  before _60 after sep before _59 after sep before _58 after sep before _57 after sep \
  before _56 after sep before _55 after sep before _54 after sep before _53 after sep \
  before _52 after sep before _51 after sep before _50 after sep before _49 after sep \
  before _48 after sep before _47 after sep before _46 after sep before _45 after sep \
  before _44 after sep before _43 after sep before _42 after sep before _41 after sep \
  before _40 after sep before _39 after sep before _38 after sep before _37 after sep \
  before _36 after sep before _35 after sep before _34 after sep before _33 after sep \
  before _32 after sep before _31 after sep before _30 after sep before _29 after sep \
  before _28 after sep before _27 after sep before _26 after sep before _25 after sep \
  before _24 after sep before _23 after sep before _22 after sep before _21 after sep \
  before _20 after sep before _19 after sep before _18 after sep before _17 after sep \
  before _16 after sep before _15 after sep before _14 after sep before _13 after sep \
  before _12 after sep before _11 after sep before _10 after sep before _9 after sep  \
  before _8 after sep before _7 after sep before _6 after sep before _5 after sep     \
  before _4 after sep before _3 after sep before _2 after sep before _1 after

#define JLN_MP_RXS_128(before, after, sep)                                                \
  before _128 after sep before _127 after sep before _126 after sep before _125 after sep \
  before _124 after sep before _123 after sep before _122 after sep before _121 after sep \
  before _120 after sep before _119 after sep before _118 after sep before _117 after sep \
  before _116 after sep before _115 after sep before _114 after sep before _113 after sep \
  before _112 after sep before _111 after sep before _110 after sep before _109 after sep \
  before _108 after sep before _107 after sep before _106 after sep before _105 after sep \
  before _104 after sep before _103 after sep before _102 after sep before _101 after sep \
  before _100 after sep before _99 after sep before _98 after sep before _97 after sep    \
  before _96 after sep before _95 after sep before _94 after sep before _93 after sep     \
  before _92 after sep before _91 after sep before _90 after sep before _89 after sep     \
  before _88 after sep before _87 after sep before _86 after sep before _85 after sep     \
  before _84 after sep before _83 after sep before _82 after sep before _81 after sep     \
  before _80 after sep before _79 after sep before _78 after sep before _77 after sep     \
  before _76 after sep before _75 after sep before _74 after sep before _73 after sep     \
  before _72 after sep before _71 after sep before _70 after sep before _69 after sep     \
  before _68 after sep before _67 after sep before _66 after sep before _65 after sep     \
  before _64 after sep before _63 after sep before _62 after sep before _61 after sep     \
  before _60 after sep before _59 after sep before _58 after sep before _57 after sep     \
  before _56 after sep before _55 after sep before _54 after sep before _53 after sep     \
  before _52 after sep before _51 after sep before _50 after sep before _49 after sep     \
  before _48 after sep before _47 after sep before _46 after sep before _45 after sep     \
  before _44 after sep before _43 after sep before _42 after sep before _41 after sep     \
  before _40 after sep before _39 after sep before _38 after sep before _37 after sep     \
  before _36 after sep before _35 after sep before _34 after sep before _33 after sep     \
  before _32 after sep before _31 after sep before _30 after sep before _29 after sep     \
  before _28 after sep before _27 after sep before _26 after sep before _25 after sep     \
  before _24 after sep before _23 after sep before _22 after sep before _21 after sep     \
  before _20 after sep before _19 after sep before _18 after sep before _17 after sep     \
  before _16 after sep before _15 after sep before _14 after sep before _13 after sep     \
  before _12 after sep before _11 after sep before _10 after sep before _9 after sep      \
  before _8 after sep before _7 after sep before _6 after sep before _5 after sep         \
  before _4 after sep before _3 after sep before _2 after sep before _1 after

#define JLN_MP_RXS_256(before, after, sep)                                                \
  before _256 after sep before _255 after sep before _254 after sep before _253 after sep \
  before _252 after sep before _251 after sep before _250 after sep before _249 after sep \
  before _248 after sep before _247 after sep before _246 after sep before _245 after sep \
  before _244 after sep before _243 after sep before _242 after sep before _241 after sep \
  before _240 after sep before _239 after sep before _238 after sep before _237 after sep \
  before _236 after sep before _235 after sep before _234 after sep before _233 after sep \
  before _232 after sep before _231 after sep before _230 after sep before _229 after sep \
  before _228 after sep before _227 after sep before _226 after sep before _225 after sep \
  before _224 after sep before _223 after sep before _222 after sep before _221 after sep \
  before _220 after sep before _219 after sep before _218 after sep before _217 after sep \
  before _216 after sep before _215 after sep before _214 after sep before _213 after sep \
  before _212 after sep before _211 after sep before _210 after sep before _209 after sep \
  before _208 after sep before _207 after sep before _206 after sep before _205 after sep \
  before _204 after sep before _203 after sep before _202 after sep before _201 after sep \
  before _200 after sep before _199 after sep before _198 after sep before _197 after sep \
  before _196 after sep before _195 after sep before _194 after sep before _193 after sep \
  before _192 after sep before _191 after sep before _190 after sep before _189 after sep \
  before _188 after sep before _187 after sep before _186 after sep before _185 after sep \
  before _184 after sep before _183 after sep before _182 after sep before _181 after sep \
  before _180 after sep before _179 after sep before _178 after sep before _177 after sep \
  before _176 after sep before _175 after sep before _174 after sep before _173 after sep \
  before _172 after sep before _171 after sep before _170 after sep before _169 after sep \
  before _168 after sep before _167 after sep before _166 after sep before _165 after sep \
  before _164 after sep before _163 after sep before _162 after sep before _161 after sep \
  before _160 after sep before _159 after sep before _158 after sep before _157 after sep \
  before _156 after sep before _155 after sep before _154 after sep before _153 after sep \
  before _152 after sep before _151 after sep before _150 after sep before _149 after sep \
  before _148 after sep before _147 after sep before _146 after sep before _145 after sep \
  before _144 after sep before _143 after sep before _142 after sep before _141 after sep \
  before _140 after sep before _139 after sep before _138 after sep before _137 after sep \
  before _136 after sep before _135 after sep before _134 after sep before _133 after sep \
  before _132 after sep before _131 after sep before _130 after sep before _129 after sep \
  before _128 after sep before _127 after sep before _126 after sep before _125 after sep \
  before _124 after sep before _123 after sep before _122 after sep before _121 after sep \
  before _120 after sep before _119 after sep before _118 after sep before _117 after sep \
  before _116 after sep before _115 after sep before _114 after sep before _113 after sep \
  before _112 after sep before _111 after sep before _110 after sep before _109 after sep \
  before _108 after sep before _107 after sep before _106 after sep before _105 after sep \
  before _104 after sep before _103 after sep before _102 after sep before _101 after sep \
  before _100 after sep before _99 after sep before _98 after sep before _97 after sep    \
  before _96 after sep before _95 after sep before _94 after sep before _93 after sep     \
  before _92 after sep before _91 after sep before _90 after sep before _89 after sep     \
  before _88 after sep before _87 after sep before _86 after sep before _85 after sep     \
  before _84 after sep before _83 after sep before _82 after sep before _81 after sep     \
  before _80 after sep before _79 after sep before _78 after sep before _77 after sep     \
  before _76 after sep before _75 after sep before _74 after sep before _73 after sep     \
  before _72 after sep before _71 after sep before _70 after sep before _69 after sep     \
  before _68 after sep before _67 after sep before _66 after sep before _65 after sep     \
  before _64 after sep before _63 after sep before _62 after sep before _61 after sep     \
  before _60 after sep before _59 after sep before _58 after sep before _57 after sep     \
  before _56 after sep before _55 after sep before _54 after sep before _53 after sep     \
  before _52 after sep before _51 after sep before _50 after sep before _49 after sep     \
  before _48 after sep before _47 after sep before _46 after sep before _45 after sep     \
  before _44 after sep before _43 after sep before _42 after sep before _41 after sep     \
  before _40 after sep before _39 after sep before _38 after sep before _37 after sep     \
  before _36 after sep before _35 after sep before _34 after sep before _33 after sep     \
  before _32 after sep before _31 after sep before _30 after sep before _29 after sep     \
  before _28 after sep before _27 after sep before _26 after sep before _25 after sep     \
  before _24 after sep before _23 after sep before _22 after sep before _21 after sep     \
  before _20 after sep before _19 after sep before _18 after sep before _17 after sep     \
  before _16 after sep before _15 after sep before _14 after sep before _13 after sep     \
  before _12 after sep before _11 after sep before _10 after sep before _9 after sep      \
  before _8 after sep before _7 after sep before _6 after sep before _5 after sep         \
  before _4 after sep before _3 after sep before _2 after sep before _1 after

#define JLN_MP_RXS_512(before, after, sep)                                                \
  before _512 after sep before _511 after sep before _510 after sep before _509 after sep \
  before _508 after sep before _507 after sep before _506 after sep before _505 after sep \
  before _504 after sep before _503 after sep before _502 after sep before _501 after sep \
  before _500 after sep before _499 after sep before _498 after sep before _497 after sep \
  before _496 after sep before _495 after sep before _494 after sep before _493 after sep \
  before _492 after sep before _491 after sep before _490 after sep before _489 after sep \
  before _488 after sep before _487 after sep before _486 after sep before _485 after sep \
  before _484 after sep before _483 after sep before _482 after sep before _481 after sep \
  before _480 after sep before _479 after sep before _478 after sep before _477 after sep \
  before _476 after sep before _475 after sep before _474 after sep before _473 after sep \
  before _472 after sep before _471 after sep before _470 after sep before _469 after sep \
  before _468 after sep before _467 after sep before _466 after sep before _465 after sep \
  before _464 after sep before _463 after sep before _462 after sep before _461 after sep \
  before _460 after sep before _459 after sep before _458 after sep before _457 after sep \
  before _456 after sep before _455 after sep before _454 after sep before _453 after sep \
  before _452 after sep before _451 after sep before _450 after sep before _449 after sep \
  before _448 after sep before _447 after sep before _446 after sep before _445 after sep \
  before _444 after sep before _443 after sep before _442 after sep before _441 after sep \
  before _440 after sep before _439 after sep before _438 after sep before _437 after sep \
  before _436 after sep before _435 after sep before _434 after sep before _433 after sep \
  before _432 after sep before _431 after sep before _430 after sep before _429 after sep \
  before _428 after sep before _427 after sep before _426 after sep before _425 after sep \
  before _424 after sep before _423 after sep before _422 after sep before _421 after sep \
  before _420 after sep before _419 after sep before _418 after sep before _417 after sep \
  before _416 after sep before _415 after sep before _414 after sep before _413 after sep \
  before _412 after sep before _411 after sep before _410 after sep before _409 after sep \
  before _408 after sep before _407 after sep before _406 after sep before _405 after sep \
  before _404 after sep before _403 after sep before _402 after sep before _401 after sep \
  before _400 after sep before _399 after sep before _398 after sep before _397 after sep \
  before _396 after sep before _395 after sep before _394 after sep before _393 after sep \
  before _392 after sep before _391 after sep before _390 after sep before _389 after sep \
  before _388 after sep before _387 after sep before _386 after sep before _385 after sep \
  before _384 after sep before _383 after sep before _382 after sep before _381 after sep \
  before _380 after sep before _379 after sep before _378 after sep before _377 after sep \
  before _376 after sep before _375 after sep before _374 after sep before _373 after sep \
  before _372 after sep before _371 after sep before _370 after sep before _369 after sep \
  before _368 after sep before _367 after sep before _366 after sep before _365 after sep \
  before _364 after sep before _363 after sep before _362 after sep before _361 after sep \
  before _360 after sep before _359 after sep before _358 after sep before _357 after sep \
  before _356 after sep before _355 after sep before _354 after sep before _353 after sep \
  before _352 after sep before _351 after sep before _350 after sep before _349 after sep \
  before _348 after sep before _347 after sep before _346 after sep before _345 after sep \
  before _344 after sep before _343 after sep before _342 after sep before _341 after sep \
  before _340 after sep before _339 after sep before _338 after sep before _337 after sep \
  before _336 after sep before _335 after sep before _334 after sep before _333 after sep \
  before _332 after sep before _331 after sep before _330 after sep before _329 after sep \
  before _328 after sep before _327 after sep before _326 after sep before _325 after sep \
  before _324 after sep before _323 after sep before _322 after sep before _321 after sep \
  before _320 after sep before _319 after sep before _318 after sep before _317 after sep \
  before _316 after sep before _315 after sep before _314 after sep before _313 after sep \
  before _312 after sep before _311 after sep before _310 after sep before _309 after sep \
  before _308 after sep before _307 after sep before _306 after sep before _305 after sep \
  before _304 after sep before _303 after sep before _302 after sep before _301 after sep \
  before _300 after sep before _299 after sep before _298 after sep before _297 after sep \
  before _296 after sep before _295 after sep before _294 after sep before _293 after sep \
  before _292 after sep before _291 after sep before _290 after sep before _289 after sep \
  before _288 after sep before _287 after sep before _286 after sep before _285 after sep \
  before _284 after sep before _283 after sep before _282 after sep before _281 after sep \
  before _280 after sep before _279 after sep before _278 after sep before _277 after sep \
  before _276 after sep before _275 after sep before _274 after sep before _273 after sep \
  before _272 after sep before _271 after sep before _270 after sep before _269 after sep \
  before _268 after sep before _267 after sep before _266 after sep before _265 after sep \
  before _264 after sep before _263 after sep before _262 after sep before _261 after sep \
  before _260 after sep before _259 after sep before _258 after sep before _257 after sep \
  before _256 after sep before _255 after sep before _254 after sep before _253 after sep \
  before _252 after sep before _251 after sep before _250 after sep before _249 after sep \
  before _248 after sep before _247 after sep before _246 after sep before _245 after sep \
  before _244 after sep before _243 after sep before _242 after sep before _241 after sep \
  before _240 after sep before _239 after sep before _238 after sep before _237 after sep \
  before _236 after sep before _235 after sep before _234 after sep before _233 after sep \
  before _232 after sep before _231 after sep before _230 after sep before _229 after sep \
  before _228 after sep before _227 after sep before _226 after sep before _225 after sep \
  before _224 after sep before _223 after sep before _222 after sep before _221 after sep \
  before _220 after sep before _219 after sep before _218 after sep before _217 after sep \
  before _216 after sep before _215 after sep before _214 after sep before _213 after sep \
  before _212 after sep before _211 after sep before _210 after sep before _209 after sep \
  before _208 after sep before _207 after sep before _206 after sep before _205 after sep \
  before _204 after sep before _203 after sep before _202 after sep before _201 after sep \
  before _200 after sep before _199 after sep before _198 after sep before _197 after sep \
  before _196 after sep before _195 after sep before _194 after sep before _193 after sep \
  before _192 after sep before _191 after sep before _190 after sep before _189 after sep \
  before _188 after sep before _187 after sep before _186 after sep before _185 after sep \
  before _184 after sep before _183 after sep before _182 after sep before _181 after sep \
  before _180 after sep before _179 after sep before _178 after sep before _177 after sep \
  before _176 after sep before _175 after sep before _174 after sep before _173 after sep \
  before _172 after sep before _171 after sep before _170 after sep before _169 after sep \
  before _168 after sep before _167 after sep before _166 after sep before _165 after sep \
  before _164 after sep before _163 after sep before _162 after sep before _161 after sep \
  before _160 after sep before _159 after sep before _158 after sep before _157 after sep \
  before _156 after sep before _155 after sep before _154 after sep before _153 after sep \
  before _152 after sep before _151 after sep before _150 after sep before _149 after sep \
  before _148 after sep before _147 after sep before _146 after sep before _145 after sep \
  before _144 after sep before _143 after sep before _142 after sep before _141 after sep \
  before _140 after sep before _139 after sep before _138 after sep before _137 after sep \
  before _136 after sep before _135 after sep before _134 after sep before _133 after sep \
  before _132 after sep before _131 after sep before _130 after sep before _129 after sep \
  before _128 after sep before _127 after sep before _126 after sep before _125 after sep \
  before _124 after sep before _123 after sep before _122 after sep before _121 after sep \
  before _120 after sep before _119 after sep before _118 after sep before _117 after sep \
  before _116 after sep before _115 after sep before _114 after sep before _113 after sep \
  before _112 after sep before _111 after sep before _110 after sep before _109 after sep \
  before _108 after sep before _107 after sep before _106 after sep before _105 after sep \
  before _104 after sep before _103 after sep before _102 after sep before _101 after sep \
  before _100 after sep before _99 after sep before _98 after sep before _97 after sep    \
  before _96 after sep before _95 after sep before _94 after sep before _93 after sep     \
  before _92 after sep before _91 after sep before _90 after sep before _89 after sep     \
  before _88 after sep before _87 after sep before _86 after sep before _85 after sep     \
  before _84 after sep before _83 after sep before _82 after sep before _81 after sep     \
  before _80 after sep before _79 after sep before _78 after sep before _77 after sep     \
  before _76 after sep before _75 after sep before _74 after sep before _73 after sep     \
  before _72 after sep before _71 after sep before _70 after sep before _69 after sep     \
  before _68 after sep before _67 after sep before _66 after sep before _65 after sep     \
  before _64 after sep before _63 after sep before _62 after sep before _61 after sep     \
  before _60 after sep before _59 after sep before _58 after sep before _57 after sep     \
  before _56 after sep before _55 after sep before _54 after sep before _53 after sep     \
  before _52 after sep before _51 after sep before _50 after sep before _49 after sep     \
  before _48 after sep before _47 after sep before _46 after sep before _45 after sep     \
  before _44 after sep before _43 after sep before _42 after sep before _41 after sep     \
  before _40 after sep before _39 after sep before _38 after sep before _37 after sep     \
  before _36 after sep before _35 after sep before _34 after sep before _33 after sep     \
  before _32 after sep before _31 after sep before _30 after sep before _29 after sep     \
  before _28 after sep before _27 after sep before _26 after sep before _25 after sep     \
  before _24 after sep before _23 after sep before _22 after sep before _21 after sep     \
  before _20 after sep before _19 after sep before _18 after sep before _17 after sep     \
  before _16 after sep before _15 after sep before _14 after sep before _13 after sep     \
  before _12 after sep before _11 after sep before _10 after sep before _9 after sep      \
  before _8 after sep before _7 after sep before _6 after sep before _5 after sep         \
  before _4 after sep before _3 after sep before _2 after sep before _1 after

#define JLN_MP_RXS_1024(before, after, sep)                                                   \
  before _1024 after sep before _1023 after sep before _1022 after sep before _1021 after sep \
  before _1020 after sep before _1019 after sep before _1018 after sep before _1017 after sep \
  before _1016 after sep before _1015 after sep before _1014 after sep before _1013 after sep \
  before _1012 after sep before _1011 after sep before _1010 after sep before _1009 after sep \
  before _1008 after sep before _1007 after sep before _1006 after sep before _1005 after sep \
  before _1004 after sep before _1003 after sep before _1002 after sep before _1001 after sep \
  before _1000 after sep before _999 after sep before _998 after sep before _997 after sep    \
  before _996 after sep before _995 after sep before _994 after sep before _993 after sep     \
  before _992 after sep before _991 after sep before _990 after sep before _989 after sep     \
  before _988 after sep before _987 after sep before _986 after sep before _985 after sep     \
  before _984 after sep before _983 after sep before _982 after sep before _981 after sep     \
  before _980 after sep before _979 after sep before _978 after sep before _977 after sep     \
  before _976 after sep before _975 after sep before _974 after sep before _973 after sep     \
  before _972 after sep before _971 after sep before _970 after sep before _969 after sep     \
  before _968 after sep before _967 after sep before _966 after sep before _965 after sep     \
  before _964 after sep before _963 after sep before _962 after sep before _961 after sep     \
  before _960 after sep before _959 after sep before _958 after sep before _957 after sep     \
  before _956 after sep before _955 after sep before _954 after sep before _953 after sep     \
  before _952 after sep before _951 after sep before _950 after sep before _949 after sep     \
  before _948 after sep before _947 after sep before _946 after sep before _945 after sep     \
  before _944 after sep before _943 after sep before _942 after sep before _941 after sep     \
  before _940 after sep before _939 after sep before _938 after sep before _937 after sep     \
  before _936 after sep before _935 after sep before _934 after sep before _933 after sep     \
  before _932 after sep before _931 after sep before _930 after sep before _929 after sep     \
  before _928 after sep before _927 after sep before _926 after sep before _925 after sep     \
  before _924 after sep before _923 after sep before _922 after sep before _921 after sep     \
  before _920 after sep before _919 after sep before _918 after sep before _917 after sep     \
  before _916 after sep before _915 after sep before _914 after sep before _913 after sep     \
  before _912 after sep before _911 after sep before _910 after sep before _909 after sep     \
  before _908 after sep before _907 after sep before _906 after sep before _905 after sep     \
  before _904 after sep before _903 after sep before _902 after sep before _901 after sep     \
  before _900 after sep before _899 after sep before _898 after sep before _897 after sep     \
  before _896 after sep before _895 after sep before _894 after sep before _893 after sep     \
  before _892 after sep before _891 after sep before _890 after sep before _889 after sep     \
  before _888 after sep before _887 after sep before _886 after sep before _885 after sep     \
  before _884 after sep before _883 after sep before _882 after sep before _881 after sep     \
  before _880 after sep before _879 after sep before _878 after sep before _877 after sep     \
  before _876 after sep before _875 after sep before _874 after sep before _873 after sep     \
  before _872 after sep before _871 after sep before _870 after sep before _869 after sep     \
  before _868 after sep before _867 after sep before _866 after sep before _865 after sep     \
  before _864 after sep before _863 after sep before _862 after sep before _861 after sep     \
  before _860 after sep before _859 after sep before _858 after sep before _857 after sep     \
  before _856 after sep before _855 after sep before _854 after sep before _853 after sep     \
  before _852 after sep before _851 after sep before _850 after sep before _849 after sep     \
  before _848 after sep before _847 after sep before _846 after sep before _845 after sep     \
  before _844 after sep before _843 after sep before _842 after sep before _841 after sep     \
  before _840 after sep before _839 after sep before _838 after sep before _837 after sep     \
  before _836 after sep before _835 after sep before _834 after sep before _833 after sep     \
  before _832 after sep before _831 after sep before _830 after sep before _829 after sep     \
  before _828 after sep before _827 after sep before _826 after sep before _825 after sep     \
  before _824 after sep before _823 after sep before _822 after sep before _821 after sep     \
  before _820 after sep before _819 after sep before _818 after sep before _817 after sep     \
  before _816 after sep before _815 after sep before _814 after sep before _813 after sep     \
  before _812 after sep before _811 after sep before _810 after sep before _809 after sep     \
  before _808 after sep before _807 after sep before _806 after sep before _805 after sep     \
  before _804 after sep before _803 after sep before _802 after sep before _801 after sep     \
  before _800 after sep before _799 after sep before _798 after sep before _797 after sep     \
  before _796 after sep before _795 after sep before _794 after sep before _793 after sep     \
  before _792 after sep before _791 after sep before _790 after sep before _789 after sep     \
  before _788 after sep before _787 after sep before _786 after sep before _785 after sep     \
  before _784 after sep before _783 after sep before _782 after sep before _781 after sep     \
  before _780 after sep before _779 after sep before _778 after sep before _777 after sep     \
  before _776 after sep before _775 after sep before _774 after sep before _773 after sep     \
  before _772 after sep before _771 after sep before _770 after sep before _769 after sep     \
  before _768 after sep before _767 after sep before _766 after sep before _765 after sep     \
  before _764 after sep before _763 after sep before _762 after sep before _761 after sep     \
  before _760 after sep before _759 after sep before _758 after sep before _757 after sep     \
  before _756 after sep before _755 after sep before _754 after sep before _753 after sep     \
  before _752 after sep before _751 after sep before _750 after sep before _749 after sep     \
  before _748 after sep before _747 after sep before _746 after sep before _745 after sep     \
  before _744 after sep before _743 after sep before _742 after sep before _741 after sep     \
  before _740 after sep before _739 after sep before _738 after sep before _737 after sep     \
  before _736 after sep before _735 after sep before _734 after sep before _733 after sep     \
  before _732 after sep before _731 after sep before _730 after sep before _729 after sep     \
  before _728 after sep before _727 after sep before _726 after sep before _725 after sep     \
  before _724 after sep before _723 after sep before _722 after sep before _721 after sep     \
  before _720 after sep before _719 after sep before _718 after sep before _717 after sep     \
  before _716 after sep before _715 after sep before _714 after sep before _713 after sep     \
  before _712 after sep before _711 after sep before _710 after sep before _709 after sep     \
  before _708 after sep before _707 after sep before _706 after sep before _705 after sep     \
  before _704 after sep before _703 after sep before _702 after sep before _701 after sep     \
  before _700 after sep before _699 after sep before _698 after sep before _697 after sep     \
  before _696 after sep before _695 after sep before _694 after sep before _693 after sep     \
  before _692 after sep before _691 after sep before _690 after sep before _689 after sep     \
  before _688 after sep before _687 after sep before _686 after sep before _685 after sep     \
  before _684 after sep before _683 after sep before _682 after sep before _681 after sep     \
  before _680 after sep before _679 after sep before _678 after sep before _677 after sep     \
  before _676 after sep before _675 after sep before _674 after sep before _673 after sep     \
  before _672 after sep before _671 after sep before _670 after sep before _669 after sep     \
  before _668 after sep before _667 after sep before _666 after sep before _665 after sep     \
  before _664 after sep before _663 after sep before _662 after sep before _661 after sep     \
  before _660 after sep before _659 after sep before _658 after sep before _657 after sep     \
  before _656 after sep before _655 after sep before _654 after sep before _653 after sep     \
  before _652 after sep before _651 after sep before _650 after sep before _649 after sep     \
  before _648 after sep before _647 after sep before _646 after sep before _645 after sep     \
  before _644 after sep before _643 after sep before _642 after sep before _641 after sep     \
  before _640 after sep before _639 after sep before _638 after sep before _637 after sep     \
  before _636 after sep before _635 after sep before _634 after sep before _633 after sep     \
  before _632 after sep before _631 after sep before _630 after sep before _629 after sep     \
  before _628 after sep before _627 after sep before _626 after sep before _625 after sep     \
  before _624 after sep before _623 after sep before _622 after sep before _621 after sep     \
  before _620 after sep before _619 after sep before _618 after sep before _617 after sep     \
  before _616 after sep before _615 after sep before _614 after sep before _613 after sep     \
  before _612 after sep before _611 after sep before _610 after sep before _609 after sep     \
  before _608 after sep before _607 after sep before _606 after sep before _605 after sep     \
  before _604 after sep before _603 after sep before _602 after sep before _601 after sep     \
  before _600 after sep before _599 after sep before _598 after sep before _597 after sep     \
  before _596 after sep before _595 after sep before _594 after sep before _593 after sep     \
  before _592 after sep before _591 after sep before _590 after sep before _589 after sep     \
  before _588 after sep before _587 after sep before _586 after sep before _585 after sep     \
  before _584 after sep before _583 after sep before _582 after sep before _581 after sep     \
  before _580 after sep before _579 after sep before _578 after sep before _577 after sep     \
  before _576 after sep before _575 after sep before _574 after sep before _573 after sep     \
  before _572 after sep before _571 after sep before _570 after sep before _569 after sep     \
  before _568 after sep before _567 after sep before _566 after sep before _565 after sep     \
  before _564 after sep before _563 after sep before _562 after sep before _561 after sep     \
  before _560 after sep before _559 after sep before _558 after sep before _557 after sep     \
  before _556 after sep before _555 after sep before _554 after sep before _553 after sep     \
  before _552 after sep before _551 after sep before _550 after sep before _549 after sep     \
  before _548 after sep before _547 after sep before _546 after sep before _545 after sep     \
  before _544 after sep before _543 after sep before _542 after sep before _541 after sep     \
  before _540 after sep before _539 after sep before _538 after sep before _537 after sep     \
  before _536 after sep before _535 after sep before _534 after sep before _533 after sep     \
  before _532 after sep before _531 after sep before _530 after sep before _529 after sep     \
  before _528 after sep before _527 after sep before _526 after sep before _525 after sep     \
  before _524 after sep before _523 after sep before _522 after sep before _521 after sep     \
  before _520 after sep before _519 after sep before _518 after sep before _517 after sep     \
  before _516 after sep before _515 after sep before _514 after sep before _513 after sep     \
  before _512 after sep before _511 after sep before _510 after sep before _509 after sep     \
  before _508 after sep before _507 after sep before _506 after sep before _505 after sep     \
  before _504 after sep before _503 after sep before _502 after sep before _501 after sep     \
  before _500 after sep before _499 after sep before _498 after sep before _497 after sep     \
  before _496 after sep before _495 after sep before _494 after sep before _493 after sep     \
  before _492 after sep before _491 after sep before _490 after sep before _489 after sep     \
  before _488 after sep before _487 after sep before _486 after sep before _485 after sep     \
  before _484 after sep before _483 after sep before _482 after sep before _481 after sep     \
  before _480 after sep before _479 after sep before _478 after sep before _477 after sep     \
  before _476 after sep before _475 after sep before _474 after sep before _473 after sep     \
  before _472 after sep before _471 after sep before _470 after sep before _469 after sep     \
  before _468 after sep before _467 after sep before _466 after sep before _465 after sep     \
  before _464 after sep before _463 after sep before _462 after sep before _461 after sep     \
  before _460 after sep before _459 after sep before _458 after sep before _457 after sep     \
  before _456 after sep before _455 after sep before _454 after sep before _453 after sep     \
  before _452 after sep before _451 after sep before _450 after sep before _449 after sep     \
  before _448 after sep before _447 after sep before _446 after sep before _445 after sep     \
  before _444 after sep before _443 after sep before _442 after sep before _441 after sep     \
  before _440 after sep before _439 after sep before _438 after sep before _437 after sep     \
  before _436 after sep before _435 after sep before _434 after sep before _433 after sep     \
  before _432 after sep before _431 after sep before _430 after sep before _429 after sep     \
  before _428 after sep before _427 after sep before _426 after sep before _425 after sep     \
  before _424 after sep before _423 after sep before _422 after sep before _421 after sep     \
  before _420 after sep before _419 after sep before _418 after sep before _417 after sep     \
  before _416 after sep before _415 after sep before _414 after sep before _413 after sep     \
  before _412 after sep before _411 after sep before _410 after sep before _409 after sep     \
  before _408 after sep before _407 after sep before _406 after sep before _405 after sep     \
  before _404 after sep before _403 after sep before _402 after sep before _401 after sep     \
  before _400 after sep before _399 after sep before _398 after sep before _397 after sep     \
  before _396 after sep before _395 after sep before _394 after sep before _393 after sep     \
  before _392 after sep before _391 after sep before _390 after sep before _389 after sep     \
  before _388 after sep before _387 after sep before _386 after sep before _385 after sep     \
  before _384 after sep before _383 after sep before _382 after sep before _381 after sep     \
  before _380 after sep before _379 after sep before _378 after sep before _377 after sep     \
  before _376 after sep before _375 after sep before _374 after sep before _373 after sep     \
  before _372 after sep before _371 after sep before _370 after sep before _369 after sep     \
  before _368 after sep before _367 after sep before _366 after sep before _365 after sep     \
  before _364 after sep before _363 after sep before _362 after sep before _361 after sep     \
  before _360 after sep before _359 after sep before _358 after sep before _357 after sep     \
  before _356 after sep before _355 after sep before _354 after sep before _353 after sep     \
  before _352 after sep before _351 after sep before _350 after sep before _349 after sep     \
  before _348 after sep before _347 after sep before _346 after sep before _345 after sep     \
  before _344 after sep before _343 after sep before _342 after sep before _341 after sep     \
  before _340 after sep before _339 after sep before _338 after sep before _337 after sep     \
  before _336 after sep before _335 after sep before _334 after sep before _333 after sep     \
  before _332 after sep before _331 after sep before _330 after sep before _329 after sep     \
  before _328 after sep before _327 after sep before _326 after sep before _325 after sep     \
  before _324 after sep before _323 after sep before _322 after sep before _321 after sep     \
  before _320 after sep before _319 after sep before _318 after sep before _317 after sep     \
  before _316 after sep before _315 after sep before _314 after sep before _313 after sep     \
  before _312 after sep before _311 after sep before _310 after sep before _309 after sep     \
  before _308 after sep before _307 after sep before _306 after sep before _305 after sep     \
  before _304 after sep before _303 after sep before _302 after sep before _301 after sep     \
  before _300 after sep before _299 after sep before _298 after sep before _297 after sep     \
  before _296 after sep before _295 after sep before _294 after sep before _293 after sep     \
  before _292 after sep before _291 after sep before _290 after sep before _289 after sep     \
  before _288 after sep before _287 after sep before _286 after sep before _285 after sep     \
  before _284 after sep before _283 after sep before _282 after sep before _281 after sep     \
  before _280 after sep before _279 after sep before _278 after sep before _277 after sep     \
  before _276 after sep before _275 after sep before _274 after sep before _273 after sep     \
  before _272 after sep before _271 after sep before _270 after sep before _269 after sep     \
  before _268 after sep before _267 after sep before _266 after sep before _265 after sep     \
  before _264 after sep before _263 after sep before _262 after sep before _261 after sep     \
  before _260 after sep before _259 after sep before _258 after sep before _257 after sep     \
  before _256 after sep before _255 after sep before _254 after sep before _253 after sep     \
  before _252 after sep before _251 after sep before _250 after sep before _249 after sep     \
  before _248 after sep before _247 after sep before _246 after sep before _245 after sep     \
  before _244 after sep before _243 after sep before _242 after sep before _241 after sep     \
  before _240 after sep before _239 after sep before _238 after sep before _237 after sep     \
  before _236 after sep before _235 after sep before _234 after sep before _233 after sep     \
  before _232 after sep before _231 after sep before _230 after sep before _229 after sep     \
  before _228 after sep before _227 after sep before _226 after sep before _225 after sep     \
  before _224 after sep before _223 after sep before _222 after sep before _221 after sep     \
  before _220 after sep before _219 after sep before _218 after sep before _217 after sep     \
  before _216 after sep before _215 after sep before _214 after sep before _213 after sep     \
  before _212 after sep before _211 after sep before _210 after sep before _209 after sep     \
  before _208 after sep before _207 after sep before _206 after sep before _205 after sep     \
  before _204 after sep before _203 after sep before _202 after sep before _201 after sep     \
  before _200 after sep before _199 after sep before _198 after sep before _197 after sep     \
  before _196 after sep before _195 after sep before _194 after sep before _193 after sep     \
  before _192 after sep before _191 after sep before _190 after sep before _189 after sep     \
  before _188 after sep before _187 after sep before _186 after sep before _185 after sep     \
  before _184 after sep before _183 after sep before _182 after sep before _181 after sep     \
  before _180 after sep before _179 after sep before _178 after sep before _177 after sep     \
  before _176 after sep before _175 after sep before _174 after sep before _173 after sep     \
  before _172 after sep before _171 after sep before _170 after sep before _169 after sep     \
  before _168 after sep before _167 after sep before _166 after sep before _165 after sep     \
  before _164 after sep before _163 after sep before _162 after sep before _161 after sep     \
  before _160 after sep before _159 after sep before _158 after sep before _157 after sep     \
  before _156 after sep before _155 after sep before _154 after sep before _153 after sep     \
  before _152 after sep before _151 after sep before _150 after sep before _149 after sep     \
  before _148 after sep before _147 after sep before _146 after sep before _145 after sep     \
  before _144 after sep before _143 after sep before _142 after sep before _141 after sep     \
  before _140 after sep before _139 after sep before _138 after sep before _137 after sep     \
  before _136 after sep before _135 after sep before _134 after sep before _133 after sep     \
  before _132 after sep before _131 after sep before _130 after sep before _129 after sep     \
  before _128 after sep before _127 after sep before _126 after sep before _125 after sep     \
  before _124 after sep before _123 after sep before _122 after sep before _121 after sep     \
  before _120 after sep before _119 after sep before _118 after sep before _117 after sep     \
  before _116 after sep before _115 after sep before _114 after sep before _113 after sep     \
  before _112 after sep before _111 after sep before _110 after sep before _109 after sep     \
  before _108 after sep before _107 after sep before _106 after sep before _105 after sep     \
  before _104 after sep before _103 after sep before _102 after sep before _101 after sep     \
  before _100 after sep before _99 after sep before _98 after sep before _97 after sep        \
  before _96 after sep before _95 after sep before _94 after sep before _93 after sep         \
  before _92 after sep before _91 after sep before _90 after sep before _89 after sep         \
  before _88 after sep before _87 after sep before _86 after sep before _85 after sep         \
  before _84 after sep before _83 after sep before _82 after sep before _81 after sep         \
  before _80 after sep before _79 after sep before _78 after sep before _77 after sep         \
  before _76 after sep before _75 after sep before _74 after sep before _73 after sep         \
  before _72 after sep before _71 after sep before _70 after sep before _69 after sep         \
  before _68 after sep before _67 after sep before _66 after sep before _65 after sep         \
  before _64 after sep before _63 after sep before _62 after sep before _61 after sep         \
  before _60 after sep before _59 after sep before _58 after sep before _57 after sep         \
  before _56 after sep before _55 after sep before _54 after sep before _53 after sep         \
  before _52 after sep before _51 after sep before _50 after sep before _49 after sep         \
  before _48 after sep before _47 after sep before _46 after sep before _45 after sep         \
  before _44 after sep before _43 after sep before _42 after sep before _41 after sep         \
  before _40 after sep before _39 after sep before _38 after sep before _37 after sep         \
  before _36 after sep before _35 after sep before _34 after sep before _33 after sep         \
  before _32 after sep before _31 after sep before _30 after sep before _29 after sep         \
  before _28 after sep before _27 after sep before _26 after sep before _25 after sep         \
  before _24 after sep before _23 after sep before _22 after sep before _21 after sep         \
  before _20 after sep before _19 after sep before _18 after sep before _17 after sep         \
  before _16 after sep before _15 after sep before _14 after sep before _13 after sep         \
  before _12 after sep before _11 after sep before _10 after sep before _9 after sep          \
  before _8 after sep before _7 after sep before _6 after sep before _5 after sep             \
  before _4 after sep before _3 after sep before _2 after sep before _1 after
namespace jln::mp
{
    /// \cond
    namespace detail
    {
        template<int>
        struct _fold_left;
    }
    /// \endcond

    /// \ingroup algorithm

    /// Folds left over a list using a binary predicate.
    /// fold_left consideres the first element in the input pack as the state,
    /// use \c push_front<> to add state if needed.
    /// \semantics
    ///   Equivalent to
    ///   \code
    ///   F::f<... F::f<xs[0], xs[1]>, xs[2]>, ..., xs[n-1]>, ...>
    ///   \endcode
    /// \treturn \value
    /// \see fold_right, fold_tree, reverse_fold, fold_balanced_tree
    template<class F, class C = identity>
    struct fold_left
    {
        template<class... xs>
        using f = typename C::template f<
            typename detail::_fold_left<
            detail::sub_1_n_4_or_less_8_16_64_256(sizeof...(xs))
      >::template f<F::template f, xs...>
        >;
    };

    namespace emp
    {
        template<class L, class state, class F, class C = mp::identity>
        using fold_left = unpack<L,
            mp::push_front<state, mp::fold_left<F, C>>>;
    }
}


/// \cond
namespace jln::mp::detail
{
#define JLN_MP_FOLD_LEFT_SELECT(n, mp_xs, _, mp_dup)  \
  template<>                                          \
  struct _fold_left<n>                                \
  {                                                   \
    template<template<class...> class F, class state, \
      mp_xs(class, JLN_MP_NIL, JLN_MP_COMMA),         \
      class... xs>                                    \
    using f = typename _fold_left<                    \
      detail::n_4_or_less_8_16_64_256(sizeof...(xs))  \
    >::template f<F,                                  \
      mp_dup(F<, JLN_MP_NIL) state,                   \
      mp_xs(JLN_MP_NIL, >, JLN_MP_COMMA), xs...       \
    >;                                                \
  };

    JLN_MP_GEN_XS_4_8_16_64_256(JLN_MP_FOLD_LEFT_SELECT)

#undef JLN_MP_FOLD_LEFT_SELECT

#define JLN_MP_FOLD_LEFT_SELECT(n, mp_xs, _, mp_dup)  \
  template<>                                          \
  struct _fold_left<n>                                \
  {                                                   \
    template<template<class...> class F, class state, \
      mp_xs(class, JLN_MP_NIL, JLN_MP_COMMA)>         \
    using f = mp_dup(F<, JLN_MP_NIL) state,           \
      mp_xs(JLN_MP_NIL, >, JLN_MP_COMMA);             \
  };

        JLN_MP_GEN_XS_1_TO_4(JLN_MP_FOLD_LEFT_SELECT)

#undef JLN_MP_FOLD_LEFT_SELECT

        template<>
    struct _fold_left<0>
    {
        template<template<class...> class, class state>
        using f = state;
    };

    template<>
    struct _fold_left<-1>
    {};
}
/// \endcond
namespace jln::mp
{
    /// \ingroup algorithm

    /// Executes \c F on every element of a \sequence.
    /// \treturn \sequence
    template<class F, class C = listify>
    struct transform
    {
        template<class... xs>
        using f = dispatch<C, F, xs...>;
    };

    namespace emp
    {
        template<class L, class C = mp::listify>
        using transform = unpack<L, mp::transform<C>>;
    }
}


/// \cond
namespace jln::mp::detail
{
    template<class T>
    struct optimize_useless_transform_unpack
    {
        using type = T;
    };

    template<class T>
    using optimize_useless_transform_unpack_t
        = typename optimize_useless_transform_unpack<T>::type;

    template<class F>
    struct optimize_useless_transform_unpack_impl
    {
        template<class C>
        using f = transform<F, C>;
    };

    template<>
    struct optimize_useless_transform_unpack_impl<identity>
    {
        template<class C>
        using f = optimize_useless_transform_unpack_t<C>;
    };

    template<class F, class C>
    struct optimize_useless_transform_unpack<transform<F, C>>
    {
        using type = typename optimize_useless_transform_unpack_impl<optimize_useless_unpack_t<F>>
            ::template f<C>;
    };
}
/// \endcond
namespace jln::mp
{
    /// \cond
    namespace detail
    {
        template<class C = listify>
        struct _zip;
    }
    /// \endcond

    /// \ingroup group

    /// n-ary version of transform.
    /// \pre all parameters must be \lists
    /// \pre all lists must be the same size
    /// \semantics
    ///   \code
    ///   call<zip<listify>,
    ///     list<_1, _2, _3>,
    ///     list<_a, _b, _c>
    ///   > = list<
    ///     list<_1, _a>,
    ///     list<_2, _b>,
    ///     list<_3, _c>
    ///   >
    ///   \endcode
    /// \treturn \sequence of \list
    template<class C = listify>
    using zip = detail::_zip<detail::optimize_useless_transform_unpack_t<C>>;

    template<class F = listify, class C = listify>
    using zip_with = zip<transform<unpack<F>, C>>;

    namespace emp
    {
        template<class L, class C = mp::listify>
        using zip = unpack<L, mp::zip<C>>;

        template<class L, class F = mp::listify, class C = mp::listify>
        using zip_with = unpack<L, mp::zip_with<F, C>>;
    }
}

namespace jln::mp
{
    template<template<class...> class source, class error>
    class error_occurred
    {
        template<class new_error>
        using next = error_occurred<source, new_error>;
    };

    namespace err
    {
        class all_parameters_must_be_of_type_mp_list;

        class not_a_number;

        class you_need_to_add_a_specialization;
    }
}
namespace jln::mp
{
    /// \cond
    namespace detail
    {
        template<std::size_t n JLN_MP_DEBUG_A(class = err::all_parameters_must_be_of_type_mp_list)>
        struct _join_select;
    }
    /// \endcond

    /// \ingroup list

    /// Concatenates \lists.
    /// \pre \c seqs must be a \sequence of \list
    /// \treturn \sequence
    template<class C = listify>
    struct join
    {
        template<class... seqs>
        using f = typename detail::_join_select<sizeof...(seqs)>::template f<C, seqs...>::type;
    };

    namespace emp
    {
        template<class... seqs>
        using join = typename mp::join<>::template f<seqs...>;
    }
}

/**
* \author    Jonathan Poelen <jonathan.poelen+jln@gmail.com>
* \version   0.1
* \brief     C++17 utility macros
*/

#define JLN_PRAGMA(x) JLN_PRAGMA_I(x)

#ifdef _MSC_VER
#  define JLN_PRAGMA_I(x) __pragma(x)
#else
#  define JLN_PRAGMA_I(x) _Pragma(#x)
#endif


// Diagnostic
//@{
#if defined(__GNUC__) || defined(__clang__)

#  define JLN_DIAGNOSTIC_PUSH JLN_PRAGMA(GCC diagnostic push)
#  define JLN_DIAGNOSTIC_POP JLN_PRAGMA(GCC diagnostic pop)

#  define JLN_DIAGNOSTIC_MSVC_IGNORE(X)
#  define JLN_DIAGNOSTIC_GCC_IGNORE(X) JLN_PRAGMA(GCC diagnostic ignored X)
#  define JLN_DIAGNOSTIC_MSVC_WARNING(X)
#  define JLN_DIAGNOSTIC_GCC_WARNING(X) JLN_PRAGMA(GCC diagnostic warning X)
#  define JLN_DIAGNOSTIC_MSVC_ERROR(X)
#  define JLN_DIAGNOSTIC_GCC_ERROR(X) JLN_PRAGMA(GCC diagnostic error X)
#  ifdef __clang__
#    define JLN_DIAGNOSTIC_GCC_ONLY_IGNORE(X)
#    define JLN_DIAGNOSTIC_CLANG_IGNORE JLN_DIAGNOSTIC_GCC_IGNORE
#    define JLN_DIAGNOSTIC_GCC_ONLY_WARNING(X)
#    define JLN_DIAGNOSTIC_CLANG_WARNING JLN_DIAGNOSTIC_GCC_WARNING
#    define JLN_DIAGNOSTIC_GCC_ONLY_ERROR(X)
#    define JLN_DIAGNOSTIC_CLANG_ERROR JLN_DIAGNOSTIC_GCC_ERROR
#  else
#    define JLN_DIAGNOSTIC_GCC_ONLY_IGNORE JLN_DIAGNOSTIC_GCC_IGNORE
#    define JLN_DIAGNOSTIC_CLANG_IGNORE(X)
#    define JLN_DIAGNOSTIC_GCC_ONLY_WARNING JLN_DIAGNOSTIC_GCC_WARNING
#    define JLN_DIAGNOSTIC_CLANG_WARNING(X)
#    define JLN_DIAGNOSTIC_GCC_ONLY_ERROR JLN_DIAGNOSTIC_GCC_ERROR
#    define JLN_DIAGNOSTIC_CLANG_ERROR(X)
#  endif

#elif defined(_MSC_VER)

#  define JLN_DIAGNOSTIC_PUSH __pragma(warning(push))
#  define JLN_DIAGNOSTIC_POP __pragma(warning(pop))

#  define JLN_DIAGNOSTIC_MSVC_IGNORE(X) __pragma(warning(disable:X))
#  define JLN_DIAGNOSTIC_GCC_ONLY_IGNORE(X)
#  define JLN_DIAGNOSTIC_GCC_IGNORE(X)
#  define JLN_DIAGNOSTIC_CLANG_IGNORE(X)

#  define JLN_DIAGNOSTIC_MSVC_WARNING(X) __pragma(warning(4:X))
#  define JLN_DIAGNOSTIC_GCC_ONLY_WARNING(X)
#  define JLN_DIAGNOSTIC_GCC_WARNING(X)
#  define JLN_DIAGNOSTIC_CLANG_WARNING(X)

#  define JLN_DIAGNOSTIC_MSVC_ERROR(X) __pragma(error(X))
#  define JLN_DIAGNOSTIC_GCC_ONLY_ERROR(X)
#  define JLN_DIAGNOSTIC_GCC_ERROR(X)
#  define JLN_DIAGNOSTIC_CLANG_ERROR(X)

#else

#  define JLN_DIAGNOSTIC_PUSH
#  define JLN_DIAGNOSTIC_POP

#  define JLN_DIAGNOSTIC_MSVC_IGNORE(X)
#  define JLN_DIAGNOSTIC_GCC_IGNORE(X)
#  define JLN_DIAGNOSTIC_GCC_ONLY_IGNORE(X)
#  define JLN_DIAGNOSTIC_CLANG_IGNORE(X)

#  define JLN_DIAGNOSTIC_MSVC_WARNING(X)
#  define JLN_DIAGNOSTIC_GCC_WARNING(X)
#  define JLN_DIAGNOSTIC_GCC_ONLY_WARNING(X)
#  define JLN_DIAGNOSTIC_CLANG_WARNING(X)

#  define JLN_DIAGNOSTIC_MSVC_ERROR(X)
#  define JLN_DIAGNOSTIC_GCC_ERROR(X)
#  define JLN_DIAGNOSTIC_GCC_ONLY_ERROR(X)
#  define JLN_DIAGNOSTIC_CLANG_ERROR(X)

#endif

//@}
/// \cond
namespace jln::mp::detail
{
    template<std::size_t n JLN_MP_DEBUG_A(class error)>
    struct _join_select : _join_select<n_16_64_256_1024(n)>
    {};

    JLN_DIAGNOSTIC_PUSH
        JLN_DIAGNOSTIC_MSVC_IGNORE(4348)

#define JLN_MP_JOIN_SELECT(n, mp_xs, mp_rxs, _)   \
  template<JLN_MP_DEBUG(class error)>             \
  struct _join_select<n JLN_MP_DEBUG_A(error)>    \
  {                                               \
    template<class F,                             \
      mp_xs(class, = list<>, JLN_MP_COMMA)>       \
    struct f;                                     \
                                                  \
    template<class F,                             \
      mp_xs(class..., JLN_MP_NIL, JLN_MP_COMMA)>  \
    struct f<F, mp_xs(list<, ...>, JLN_MP_COMMA)> \
    {                                             \
      using type = typename F::template f<        \
        mp_xs(JLN_MP_NIL, ..., JLN_MP_COMMA)>;    \
    };                                            \
  };

        JLN_MP_GEN_XS_16_64_256(JLN_MP_JOIN_SELECT)

#undef JLN_MP_JOIN_SELECT

        template<JLN_MP_DEBUG(class error)>
    struct _join_select<1024 JLN_MP_DEBUG_A(error)>
    {
        template<class F,
            JLN_MP_XS_1024(class, = list<>, JLN_MP_COMMA),
            class... tail>
            struct f;

        template<class F,
            JLN_MP_XS_1024(class..., JLN_MP_NIL, JLN_MP_COMMA),
            class... tail>
            struct f<F, JLN_MP_XS_1024(list<, ...>, JLN_MP_COMMA), tail...>
            : _join_select<sizeof...(tail) + 1>
            ::template f<F, list<JLN_MP_XS_1024(JLN_MP_NIL, ..., JLN_MP_COMMA)>, tail...>
        {};
    };

    JLN_DIAGNOSTIC_POP

} // namespace jln::mp::detail
/// \endcond
/// \cond
namespace jln::mp::detail
{
    template<class...>
    struct _zip_impl;

#define JLN_MP_TRANSPOSE_IMPL(n, mp_xs, mp_rxs, mp_rep)         \
  template<class C, mp_xs(class..., JLN_MP_NIL, JLN_MP_COMMA)>  \
  struct _zip_impl<C, mp_xs(list<, ...>, JLN_MP_COMMA)>         \
  {                                                             \
    using type = typename C::template f<list<                   \
      mp_xs(JLN_MP_NIL, JLN_MP_NIL, JLN_MP_COMMA)               \
    >...>;                                                      \
  };                                                            \
                                                                \
  template<class F, class C,                                    \
    mp_xs(class..., JLN_MP_NIL, JLN_MP_COMMA)>                  \
  struct _zip_impl<                                             \
    transform<unpack<F>, C>, mp_xs(list<, ...>, JLN_MP_COMMA)>  \
  {                                                             \
    using type = typename C::template f<typename F::template f< \
      mp_xs(JLN_MP_NIL, JLN_MP_NIL, JLN_MP_COMMA)               \
    >...>;                                                      \
  };

    JLN_MP_GEN_XS_1_TO_8(JLN_MP_TRANSPOSE_IMPL)
        JLN_MP_TRANSPOSE_IMPL(8, JLN_MP_XS_8, _, _)

#undef JLN_MP_TRANSPOSE_IMPL

        template<int>
    struct _zip_dispatch;

    template<class C>
    struct _zip
    {
        template<class... seqs>
        using f = typename _zip_dispatch<
            sizeof...(seqs) ? (sizeof...(seqs) > 8 ? 2 : 1) : 0
            >::template f<C, seqs...>;
    };

    template<>
    struct _zip_dispatch<0>
    {
        template<class C>
        using f = typename C::template f<>;
    };

    template<>
    struct _zip_dispatch<1>
    {
        template<class C, class... seqs>
        using f = typename _zip_impl<C, seqs...>::type;
    };

    template<int>
    struct _recursive_zip
    {
        template<int n, class C,
            class _0, class _1, class _2, class _3, class _4,
            class _5, class _6, class _7, class... seqs>
            using f = typename _recursive_zip<(n >= 8 ? 8 : n)>
            ::template f<
            n - 8, C, seqs..., typename _zip_impl<listify,
            _0, _1, _2, _3, _4, _5, _6, _7>::type
            >;
    };

    template<>
    struct _recursive_zip<0>
    {
        template<int, class C, class... seqs>
        using f = typename zip<transform<unpack<join<>>, C>>
            ::template f<seqs...>;
    };

#define JLN_MP_TRANSPOSE_IMPL(n, mp_xs, mp_rxs, mp_rep)      \
  template<>                                                 \
  struct _recursive_zip<n>                                   \
  {                                                          \
    template<int, class C                                    \
      mp_xs(JLN_MP_COMMA class, JLN_MP_NIL, JLN_MP_NIL),     \
      class... seqs>                                         \
    using f = typename zip<transform<unpack<join<>>, C>>     \
      ::template f<seqs..., typename _zip_impl<listify,      \
        mp_xs(JLN_MP_NIL, JLN_MP_NIL, JLN_MP_COMMA)>::type>; \
  };

    JLN_MP_GEN_XS_1_TO_8(JLN_MP_TRANSPOSE_IMPL)

#undef JLN_MP_TRANSPOSE_IMPL

        template<>
    struct _zip_dispatch<2>
    {
        template<class C, class... seqs>
        using f = typename _recursive_zip<
            (sizeof...(seqs) >= 8 ? 8 : sizeof...(seqs))
        >::template f<sizeof...(seqs) - 8, C, seqs...>;
    };
}
/// \endcond
namespace jln::mp
{
    /// \ingroup functional

    /// Invokes a \function with its two first arguments reversed.
    /// \treturn \sequence
    template<class C = listify>
    struct flip
    {
        template<class x0, class x1, class... xs>
        using f = call<C, x1, x0, xs...>;
    };

    namespace emp
    {
        template<class L, class C = mp::listify>
        using flip = unpack<L, mp::flip<C>>;
    }
}
namespace jln::mp
{
    /// \ingroup algorithm

    /// Computes the recursive invocation of \c F with the result of the previous
    /// invocation and each element of one or more \lists traversed in parallel
    /// from the beginning to the end.
    /// \pre all \c seqs must be the same size
    /// \semantics
    ///   Equivalent to
    ///   \code
    ///   C::f<fold_left<F>::f<
    ///     ...
    ///     fold_left<F>::f<fold_left<F>::f<state, ...seqs[:][0]>, ...seqs[:][1]>
    ///     ...,
    ///     ...seqs[:][n-1]>
    ///   >>
    ///   \endcode
    /// \treturn \value
    template<class F, class C = identity>
    struct accumulate
    {
        template<class state, class... seqs>
        using f = typename zip<push_front<state, fold_left<flip<unpack<F>>, C>>>
            ::template f<seqs...>;
    };

    namespace emp
    {
        template<class L, class state, class F, class C = mp::identity>
        using accumulate = unpack<L,
            mp::zip<mp::push_front<state, mp::fold_left<mp::flip<mp::unpack<F>>, C>>>>;
    }
}
namespace jln::mp
{
    /// \ingroup utility

    template<bool>
    struct conditional_c;

    template<>
    struct conditional_c<true>
    {
        template<class x, class y>
        using f = x;
    };

    template<>
    struct conditional_c<false>
    {
        template<class x, class y>
        using f = y;
    };

    template<class v>
    using conditional = conditional_c<bool(v::value)>;

    namespace emp
    {
        template<class v, class x, class y>
        using conditional = typename mp::conditional_c<bool(v::value)>
            ::template f<x, y>;

        template<bool cond, class x, class y>
        using conditional_c = typename mp::conditional_c<cond>
            ::template f<x, y>;
    }
} // namespace jln::mp
namespace jln::mp
{
    /// \cond
    namespace detail
    {
        template <unsigned n>
        struct _rotate;

        constexpr unsigned _rotate_size(int_ n, unsigned size)
        {
            return n >= size
                ? (size ? n % size : 0)
                : (n < 0 ? (size && n % size ? size + n % size : 0) : n);
        }
    }
    /// \endcond

    /// \ingroup algorithm

    /// Rotates the elements of a \sequence around a pivot.
    /// \tparam N  a negative value start to end of sequence. The final offset is a modulo of `sizeof...(xs)`.
    /// \semantics
    ///   Equivalent to
    ///   \code
    ///     n = 0
    ///     len = sizeof...(xs)
    ///     if (len) {
    ///       n = N::value
    ///       if (n < 0) n = len + (n % len)
    ///       n = n % len
    ///     }
    ///     C::f<...xs[n:], ...xs[:n]>
    ///   \endcode
    /// \treturn \sequence
    template <class N, class C = listify>
    struct rotate
    {
        template<class... xs>
        using f = typename detail::_rotate<detail::n_8_or_less_16_64_256(
            detail::_rotate_size(N::value, sizeof...(xs))
        )>
            ::template f<
            detail::_rotate_size(N::value, sizeof...(xs)),
            C, xs...
            >;
    };

    template <int_ n, class C = listify>
    using rotate_c = rotate<number<n>, C>;

    namespace emp
    {
        template <class L, class n, class C = mp::listify>
        using rotate = unpack<L, mp::rotate<n, C>>;

        template <class L, int_ n, class C = mp::listify>
        using rotate_c = unpack<L, mp::rotate<number<n>, C>>;
    }
}


/// \cond
namespace jln::mp::detail
{
#define JLN_MP_ROTATE_IMPL(n, mp_xs, mp_rxs, _)           \
  template<>                                              \
  struct _rotate<n>                                       \
  {                                                       \
    template<unsigned size, class C,                      \
      mp_xs(class, JLN_MP_COMMA, JLN_MP_NIL)              \
      class... xs>                                        \
    using f = typename C::template f<                     \
      xs... mp_xs(JLN_MP_COMMA, JLN_MP_NIL, JLN_MP_NIL)>; \
  };

    JLN_MP_GEN_XS_0_TO_8(JLN_MP_ROTATE_IMPL)

#undef JLN_MP_ROTATE_IMPL

#define JLN_MP_ROTATE_IMPL(n, mp_xs, mp_rxs, mp_rep) \
  JLN_MP_ROTATE_IMPL2(n, mp_xs, mp_rep, mp_rxs,      \
    n_8_or_less_16_64_256)

#define JLN_MP_ROTATE_IMPL2(n, mp_xs, mp_rxs, _, next_int) \
  template<>                                               \
  struct _rotate<n>                                        \
  {                                                        \
    template<unsigned size, class C,                       \
      mp_xs(class, JLN_MP_COMMA, JLN_MP_NIL)               \
      class... xs>                                         \
    using f = typename _rotate<next_int (size-n)>          \
      ::template f<size-n, C,                              \
        xs... mp_xs(JLN_MP_COMMA, JLN_MP_NIL, JLN_MP_NIL)  \
      >;                                                   \
  };

        JLN_MP_GEN_XS_8_args(JLN_MP_ROTATE_IMPL2, JLN_MP_NIL)
        JLN_MP_GEN_XS_16_64_256(JLN_MP_ROTATE_IMPL)

#undef JLN_MP_ROTATE_IMPL2
#undef JLN_MP_ROTATE_IMPL
} // namespace jln::mp
/// \endcond
namespace jln::mp
{
    /// \cond
    namespace detail
    {
        template<class>
        struct _adjacent_difference;
    }
    /// \endcond

    /// \ingroup algorithm

    /// Computes the differences between adjacent_difference pair of elements.
    /// \semantics
    ///   \code
    ///   adjacent_difference_difference<C>::f<a, b, c> = C::f<a, sub::f<a, b>, sub::f<b, c>>
    ///   \endcode
    /// \treturn \sequence
    template<class C = listify>
    struct adjacent_difference
    {
        template<class... xs>
        using f = typename detail::_adjacent_difference<
            rotate_c<-1>::template f<xs...>
        >::template f<C, xs...>;
    };

    namespace emp
    {
        template<class L, class C = mp::listify>
        using adjacent_difference = unpack<L, mp::adjacent_difference<C>>;
    }
}


/// \cond
namespace jln::mp::detail
{
#ifdef _MSC_VER
    template<class C, class x, int_... xs>
    using _adjacent_difference_msvc = JLN_MP_DCALL_XS(xs, C, x, number<xs>...);
#endif

    template<class y, class... ys>
    struct _adjacent_difference<list<y, ys...>>
    {
#ifdef _MSC_VER
        template<class C, class x, class... xs>
        using f = _adjacent_difference_msvc<C, x, (xs::value - ys::value)...>;
#else
        template<class C, class x, class... xs>
        using f = JLN_MP_DCALL_XS(xs, C, x, number<xs::value - ys::value>...);
#endif
    };

    template<>
    struct _adjacent_difference<list<>>
    {
        template<class C, class...>
        using f = typename C::template f<>;
    };
}
/// \endcond
namespace jln::mp
{
    /// \cond
    namespace detail
    {
        template<int>
        struct _same;
    }
    /// \endcond

    /// \ingroup algorithm

    /// Checks whether all \values are identical.
    /// \treturn \bool
    template<class C = identity>
    struct same
    {
        template<class... xs>
        using f = typename C::template f<
            typename detail::_same<detail::min(sizeof...(xs), 3)>::template f<xs...>>;
    };

    namespace emp
    {
        template<class L, class C = mp::identity>
        using same = unpack<L, mp::same<C>>;
    }
}


namespace jln::mp
{
    template<>
    struct same<identity>
    {
        template<class... xs>
        using f = typename detail::_same<detail::min(sizeof...(xs), 3)>::template f<xs...>;
    };
}

/// \cond
namespace jln::mp::detail
{
    template<>
    struct _same<0>
    {
        template<class...>
        using f = mp::true_;
    };

    template<>
    struct _same<1>
    {
        template<class>
        using f = mp::true_;
    };

    template<>
    struct _same<2>
    {
        template<class x, class y>
        using f = mp::number<std::is_same<x, y>::value>;
    };

    template<>
    struct _same<3>
    {
        template<class x, class... xs>
        using f = mp::number<std::is_same<list<x, xs...>, list<xs..., x>>::value>;
    };
}
/// \endcond
namespace jln::mp
{
    /// \cond
    namespace detail
    {
        template<class>
        struct _adjacent_remove;
    }
    /// \endcond

    /// \ingroup algorithm

    /// Removes each element in a \sequence which respect a predicate with privious element.
    /// \treturn \sequence
    template<class BinaryPred, class C = listify>
    struct adjacent_remove_if
    {
        template<class... xs>
        using f = typename detail::_adjacent_remove<
            rotate_c<-1>::template f<xs...>
        >::template f<C, BinaryPred, xs...>;
    };

    /// Removes each element in a \sequence which is the same type as the privious element.
    /// \treturn \sequence
    template<class C = listify>
    using adjacent_remove = adjacent_remove_if<same<>, C>;

    namespace emp
    {
        template<class L, class BinaryPred, class C = mp::listify>
        using adjacent_remove_if = unpack<L, mp::adjacent_remove_if<BinaryPred, C>>;

        template<class L, class C = mp::listify>
        using adjacent_remove = unpack<L, mp::adjacent_remove<C>>;
    }
}


namespace jln::mp
{
    /// \cond
    namespace detail
    {
        template <class C, class... Fs>
        struct _tee;
    }
    /// \endcond

    /// \ingroup functional

    /// Invokes multiple functions passing all parameters to each.
    /// \treturn \value
    /// \see each, partial
#ifdef JLN_MP_DOXYGENATING
    template <class... Fs, class C>
    struct tee
    {
        template<class... xs>
        using f = C::f<Fs::f<xs...>...>;
    };
#else
    template <class... Fs>
    struct tee
        : rotate<number<-1>, lift<detail::_tee>>
        ::template f<Fs...>
    {};
#endif

    /// \cond
    template <class F, class C>
    struct tee<F, C>
    {
        template<class... xs>
        using f = unary_compose_call<C, F, xs...>;
    };

    template <class F0, class F1, class C>
    struct tee<F0, F1, C>
    {
        template<class... xs>
        using f = binary_compose_call<C, F0, F1, xs...>;
    };

    template <class F0, class F1, class F2, class C>
    struct tee<F0, F1, F2, C>
    {
        template<class... xs>
        using f = ternary_compose_call<C, F0, F1, F2, xs...>;
    };
    /// \endcond
}

/// \cond
namespace jln::mp::detail
{
    template <class C, class... Fs>
    struct _tee
    {
        template <class... xs>
        using f = typename C::template f<call<Fs, xs...>...>;
    };
} // namespace jln::mp
/// \endcond
namespace jln::mp
{
    /// \ingroup utility

    /// Always evaluate at an arbitrary value.
    /// \post result = x
    /// \treturn value
    template<class x, class C = identity>
    struct always
    {
        template<class... xs>
        using f = typename C::template f<x>;
    };

    template<class x>
    struct always<x, identity>
    {
        template<class... xs>
        using f = x;
    };
}
namespace jln::mp
{
    /// \ingroup functional

    /// A conditional expression.
    /// \treturn \value
    template<class Pred, class TC, class FC = always<false_>>
    struct if_
    {
        template<class... xs>
        using f = typename mp::conditional_c<bool(call<Pred, xs...>::value)>
            ::template f<TC, FC>
            ::template f<xs...>;
    };

    namespace emp
    {
        template<class Pred, class TC, class FC, class... xs>
        using if_ = typename conditional<call<Pred, xs...>, TC, FC>
            ::template f<xs...>;
    }
}
namespace jln::mp
{
    /// \ingroup list

    /// Appends \c x to the end of the sequence.
    /// \treturn \sequence
    template<class x, class C = listify>
    struct push_back
    {
        template<class... xs>
        using f = call<C, xs..., x>;
    };

    namespace emp
    {
        template<class L, class T, class C = mp::listify>
        using push_back = unpack<L, mp::push_back<T, C>>;
    }
}
namespace jln::mp
{
    /// \ingroup list

    /// Returns the number of elements in a \c xs.
    /// \treturn \number
    template<class C = identity>
    struct size
    {
        template<class... xs>
        using f = typename C::template f<number<sizeof...(xs)>>;
    };

    namespace emp
    {
        template<class L, class C = mp::identity>
        using size = unpack<L, mp::size<C>>;
    }
}
JLN_DIAGNOSTIC_PUSH
JLN_DIAGNOSTIC_GCC_ONLY_IGNORE("-Wlogical-op")

namespace jln::mp
{
    /// \ingroup number

    template<class C = identity>
    struct or_
    {
        template<class... xs>
        using f = typename C::template f<number<(false || ... || xs::value)>>;
    };

    template<class C = identity>
    struct and_
    {
        template<class... xs>
        using f = typename C::template f<number<(true && ... && xs::value)>>;
    };

    template<class C = identity>
    struct add
    {
        template<class... xs>
        using f = typename C::template f<number<(... + xs::value)>>;
    };

    template<class C = identity>
    using add0 = if_<size<>, add<C>, always<number<0>, C>>;

    template<class C = identity>
    struct sub
    {
        template<class... xs>
        using f = typename C::template f<number<(... - xs::value)>>;
    };

    template<class C = identity>
    using sub0 = if_<size<>, sub<C>, always<number<0>, C>>;

    template<class C = identity>
    struct lshift
    {
        template<class... xs>
        using f = typename C::template f<number<(... << xs::value)>>;
    };

    template<class C = identity>
    using lshift0 = if_<size<>, lshift<C>, always<number<0>, C>>;

    template<class C = identity>
    struct rshift
    {
        template<class... xs>
        using f = typename C::template f<number<(... >> xs::value)>>;
    };

    template<class C = identity>
    using rshift0 = if_<size<>, rshift<C>, always<number<0>, C>>;

    template<class C = identity>
    struct mul
    {
        template<class... xs>
        using f = typename C::template f<number<(... * xs::value)>>;
    };

    template<class C = identity>
    using mul0 = if_<size<>, mul<C>, always<number<0>, C>>;

    template<class C = identity>
    using mul1 = if_<size<>, mul<C>, always<number<1>, C>>;

    template<class C = identity>
    struct div
    {
        template<class... xs>
        using f = typename C::template f<number<(... / xs::value)>>;
    };

    template<class C = identity>
    using div0 = if_<size<>, div<C>, always<number<0>, C>>;

    template<class C = identity>
    using div1 = if_<size<>, div<C>, always<number<1>, C>>;

    template<class C = identity>
    struct mod
    {
        template<class... xs>
        using f = typename C::template f<number<(... % xs::value)>>;
    };

    template<class C = identity>
    using mod0 = if_<size<>, mod<C>, always<number<0>, C>>;

    template<class C = identity>
    using mod1 = if_<size<>, mod<C>, always<number<1>, C>>;

    template<class C = identity>
    struct xor_
    {
        template<class... xs>
        using f = typename C::template f<number<(... ^ xs::value)>>;
    };

    template<class C = identity>
    using xor0 = if_<size<>, xor_<C>, always<number<0>, C>>;

    template<class C = identity>
    struct bit_and
    {
        template<class... xs>
        using f = typename C::template f<number<(... & xs::value)>>;
    };

    template<class C = identity>
    using bit_and0 = if_<size<>, bit_and<C>, always<number<0>, C>>;

    template<class C = identity>
    struct bit_or
    {
        template<class... xs>
        using f = typename C::template f<number<(... | xs::value)>>;
    };

    template<class C = identity>
    using bit_or0 = if_<size<>, bit_or<C>, always<number<0>, C>>;

    template<class C = identity>
    struct neg
    {
        template<class x>
        using f = typename C::template f<number<(-x::value)>>;
    };

    template<class C = identity>
    struct unary_plus
    {
        template<class x>
        using f = typename C::template f<number<(+x::value)>>;
    };

    template<class C = identity>
    struct not_
    {
        template<class x>
        using f = typename C::template f<number<(!x::value)>>;
    };

    template<class C>
    struct not_<not_<C>>
    {
        template<class x>
        using f = typename C::template f < number < (!int_{ !x::value }) >> ;
    };

    template<class C = identity>
    struct bit_not
    {
        template<class x>
        using f = typename C::template f<number<(~x::value)>>;
    };

    template<class C = identity>
    struct inc
    {
        template<class x>
        using f = typename C::template f<number<(x::value + 1)>>;
    };

    template<class C = identity>
    struct dec
    {
        template<class x>
        using f = typename C::template f<number<(x::value - 1)>>;
    };

    template<class C = identity>
    struct equal
    {
        template<class x, class y>
        using f = typename C::template f<number<(x::value == y::value)>>;
    };

    template<class C = identity>
    struct not_equal
    {
        template<class x, class y>
        using f = typename C::template f<number<(x::value != y::value)>>;
    };

    template<class C = identity>
    struct less
    {
        template<class x, class y>
        using f = typename C::template f<number<(x::value < y::value)>>;
    };

    template<class C = identity>
    struct less_equal
    {
        template<class x, class y>
        using f = typename C::template f<number<(x::value <= y::value)>>;
    };

    template<class C = identity>
    struct greater
    {
        template<class x, class y>
        using f = typename C::template f<number<(x::value > y::value)>>;
    };

    template<class C = identity>
    struct greater_equal
    {
        template<class x, class y>
        using f = typename C::template f<number<(x::value >= y::value)>>;
    };


    template<class N, class C = identity>
    using add_to = push_front<N, add<C>>;

    template<class N, class C = identity>
    using add_by = push_back<N, add<C>>;

    template<class N, class C = identity>
    using sub_to = push_front<N, sub<C>>;

    template<class N, class C = identity>
    using sub_by = push_back<N, sub<C>>;

    template<class N, class C = identity>
    using lshift_to = push_front<N, lshift<C>>;

    template<class N, class C = identity>
    using lshift_by = push_back<N, lshift<C>>;

    template<class N, class C = identity>
    using rshift_to = push_front<N, rshift<C>>;

    template<class N, class C = identity>
    using rshift_by = push_back<N, rshift<C>>;

    template<class N, class C = identity>
    using mul_to = push_front<N, mul<C>>;

    template<class N, class C = identity>
    using mul_by = push_back<N, mul<C>>;

    template<class N, class C = identity>
    using div_to = push_front<N, div<C>>;

    template<class N, class C = identity>
    using div_by = push_back<N, div<C>>;

    template<class N, class C = identity>
    using mod_to = push_front<N, mod<C>>;

    template<class N, class C = identity>
    using mod_by = push_back<N, mod<C>>;

    template<class N, class C = identity>
    using xor_to = push_front<N, xor_<C>>;

    template<class N, class C = identity>
    using xor_by = push_back<N, xor_<C>>;

    template<class N, class C = identity>
    using bit_and_to = push_front<N, bit_and<C>>;

    template<class N, class C = identity>
    using bit_and_by = push_back<N, bit_and<C>>;

    template<class N, class C = identity>
    using bit_or_to = push_front<N, bit_or<C>>;

    template<class N, class C = identity>
    using bit_or_by = push_back<N, bit_or<C>>;


    template<class N, class C = identity>
    using equal_to = push_front<N, equal<C>>;

    template<class N, class C = identity>
    using not_equal_to = push_front<N, not_equal<C>>;

    template<class N, class C = identity>
    using less_than = push_back<N, less<C>>;

    template<class N, class C = identity>
    using less_equal_than = push_back<N, less_equal<C>>;

    template<class N, class C = identity>
    using greater_than = push_back<N, greater<C>>;

    template<class N, class C = identity>
    using greater_equal_than = push_back<N, greater_equal<C>>;


    template<int_ n, class C = identity>
    using add_to_c = add_to<number<n>, C>;

    template<int_ n, class C = identity>
    using add_by_c = add_by<number<n>, C>;

    template<int_ n, class C = identity>
    using sub_to_c = sub_to<number<n>, C>;

    template<int_ n, class C = identity>
    using sub_by_c = sub_by<number<n>, C>;

    template<int_ n, class C = identity>
    using lshift_to_c = lshift_to<number<n>, C>;

    template<int_ n, class C = identity>
    using lshift_by_c = lshift_by<number<n>, C>;

    template<int_ n, class C = identity>
    using rshift_to_c = rshift_to<number<n>, C>;

    template<int_ n, class C = identity>
    using rshift_by_c = rshift_by<number<n>, C>;

    template<int_ n, class C = identity>
    using mul_to_c = mul_to<number<n>, C>;

    template<int_ n, class C = identity>
    using mul_by_c = mul_by<number<n>, C>;

    template<int_ n, class C = identity>
    using div_to_c = div_to<number<n>, C>;

    template<int_ n, class C = identity>
    using div_by_c = div_by<number<n>, C>;

    template<int_ n, class C = identity>
    using mod_to_c = mod_to<number<n>, C>;

    template<int_ n, class C = identity>
    using mod_by_c = mod_by<number<n>, C>;

    template<int_ n, class C = identity>
    using xor_to_c = xor_to<number<n>, C>;

    template<int_ n, class C = identity>
    using xor_by_c = xor_by<number<n>, C>;

    template<int_ n, class C = identity>
    using bit_and_to_c = bit_and_to<number<n>, C>;

    template<int_ n, class C = identity>
    using bit_and_by_c = bit_and_by<number<n>, C>;

    template<int_ n, class C = identity>
    using bit_or_to_c = bit_or_to<number<n>, C>;

    template<int_ n, class C = identity>
    using bit_or_by_c = bit_or_by<number<n>, C>;


    template<int_ n, class C = identity>
    using equal_to_c = equal_to<number<n>, C>;

    template<int_ n, class C = identity>
    using not_equal_to_c = not_equal_to<number<n>, C>;

    template<int_ n, class C = identity>
    using less_than_c = less_than<number<n>, C>;

    template<int_ n, class C = identity>
    using less_equal_than_c = less_equal_than<number<n>, C>;

    template<int_ n, class C = identity>
    using greater_than_c = greater_than<number<n>, C>;

    template<int_ n, class C = identity>
    using greater_equal_than_c = greater_equal_than<number<n>, C>;
}

namespace jln::mp::emp
{
    template<class L, class C = mp::identity>
    using or_seq = unpack<L, mp::or_<C>>;

    template<class L, class C = mp::identity>
    using and_seq = unpack<L, mp::and_<C>>;

    template<class L, class C = mp::identity>
    using add_seq = unpack<L, mp::add<C>>;

    template<class L, class C = mp::identity>
    using add0_seq = unpack<L, mp::add0<C>>;

    template<class L, class C = mp::identity>
    using sub_seq = unpack<L, mp::sub<C>>;

    template<class L, class C = mp::identity>
    using sub0_seq = unpack<L, mp::sub0<C>>;

    template<class L, class C = mp::identity>
    using lshift_seq = unpack<L, mp::lshift<C>>;

    template<class L, class C = mp::identity>
    using lshift0_seq = unpack<L, mp::lshift0<C>>;

    template<class L, class C = mp::identity>
    using rshift_seq = unpack<L, mp::rshift<C>>;

    template<class L, class C = mp::identity>
    using rshift0_seq = unpack<L, mp::rshift0<C>>;

    template<class L, class C = mp::identity>
    using mul_seq = unpack<L, mp::mul<C>>;

    template<class L, class C = mp::identity>
    using mul0_seq = unpack<L, mp::mul0<C>>;

    template<class L, class C = mp::identity>
    using mul1_seq = unpack<L, mp::mul1<C>>;

    template<class L, class C = mp::identity>
    using div_seq = unpack<L, mp::div<C>>;

    template<class L, class C = mp::identity>
    using div0_seq = unpack<L, mp::div0<C>>;

    template<class L, class C = mp::identity>
    using div1_seq = unpack<L, mp::div1<C>>;

    template<class L, class C = mp::identity>
    using mod_seq = unpack<L, mp::mod<C>>;

    template<class L, class C = mp::identity>
    using mod0_seq = unpack<L, mp::mod0<C>>;

    template<class L, class C = mp::identity>
    using mod1_seq = unpack<L, mp::mod1<C>>;

    template<class L, class C = mp::identity>
    using xor_seq = unpack<L, mp::xor_<C>>;

    template<class L, class C = mp::identity>
    using xor0_seq = unpack<L, mp::xor0<C>>;

    template<class L, class C = mp::identity>
    using bit_and_seq = unpack<L, mp::bit_and<C>>;

    template<class L, class C = mp::identity>
    using bit_and0_seq = unpack<L, mp::bit_and0<C>>;

    template<class L, class C = mp::identity>
    using bit_or_seq = unpack<L, mp::bit_or<C>>;

    template<class L, class C = mp::identity>
    using bit_or0_seq = unpack<L, mp::bit_or0<C>>;


    template<int_... xs>
    using or_c = number<(false || ... || xs)>;

    template<int_... xs>
    using and_c = number<(true && ... && xs)>;

    template<int_... xs>
    using add_c = number<(... + xs)>;

    template<int_... xs>
    using add0_c = add_c<xs..., 0>;

    template<int_... xs>
    using sub_c = number<(... - xs)>;

    template<int_... xs>
    using sub0_c = sub_c<xs..., 0>;

    template<int_... xs>
    using lshift_c = number<(... << xs)>;

    template<int_... xs>
    using lshift0_c = lshift_c<xs..., 0>;

    template<int_... xs>
    using rshift_c = number<(... >> xs)>;

    template<int_... xs>
    using rshift0_c = rshift_c<xs..., 0>;

    template<int_... xs>
    using mul_c = number<(... * xs)>;

    template<int_... xs>
    using mul0_c = mul_c<xs..., (sizeof...(xs) ? 1 : 0)>;

    template<int_... xs>
    using mul1_c = mul_c<xs..., 1>;

    template<int_... xs>
    using div_c = number<(... / xs)>;

    template<int_... xs>
    using div0_c = div_c<xs..., (sizeof...(xs) ? 1 : 0)>;

    template<int_... xs>
    using div1_c = div_c<xs..., 1>;

    template<int_... xs>
    using mod_c = number<(... % xs)>;

    template<int_... xs>
    using mod0_c = mod_c<xs...,
        (sizeof...(xs) ? std::numeric_limits<int_>::min() : 0)>;

    template<int_... xs>
    using mod1_c = mod_c<xs...,
        (sizeof...(xs) ? std::numeric_limits<int_>::min() : 1)>;

    template<int_... xs>
    using xor_c = number<(... ^ xs)>;

    template<int_... xs>
    using xor0_c = xor_c<xs..., 0, 0>;

    template<int_... xs>
    using bit_and_c = number<(... & xs)>;

    template<int_... xs>
    using bit_and0_c = bit_and_c<xs...,
        (sizeof...(xs) ? std::numeric_limits<int_>::max() : 0)>;

    template<int_... xs>
    using bit_or_c = number<(... | xs)>;

    template<int_... xs>
    using bit_or0_c = bit_or_c<xs...,
        (sizeof...(xs) ? std::numeric_limits<int_>::max() : 0)>;


    template<class... xs>
    using or_ = number<(false || ... || xs::value)>;

    template<class... xs>
    using and_ = number<(true && ... && xs::value)>;

    template<class... xs>
    using add = number<(... + xs::value)>;

    template<class... xs>
    using add0 = call<mp::add0<>, xs...>;

    template<class... xs>
    using sub = number<(... - xs::value)>;

    template<class... xs>
    using sub0 = call<mp::sub0<>, xs...>;

    template<class... xs>
    using lshift = number<(... << xs::value)>;

    template<class... xs>
    using lshift0 = call<mp::lshift0<>, xs...>;

    template<class... xs>
    using rshift = number<(... >> xs::value)>;

    template<class... xs>
    using rshift0 = call<mp::rshift0<>, xs...>;

    template<class... xs>
    using mul = number<(... * xs::value)>;

    template<class... xs>
    using mul0 = call<mp::mul0<>, xs...>;

    template<class... xs>
    using mul1 = call<mp::mul1<>, xs...>;

    template<class... xs>
    using div = number<(... / xs::value)>;

    template<class... xs>
    using div0 = call<mp::div0<>, xs...>;

    template<class... xs>
    using div1 = call<mp::div1<>, xs...>;

    template<class... xs>
    using mod = number<(... % xs::value)>;

    template<class... xs>
    using mod0 = call<mp::mod0<>, xs...>;

    template<class... xs>
    using mod1 = call<mp::mod1<>, xs...>;

    template<class... xs>
    using xor_ = number<(... ^ xs::value)>;

    template<class... xs>
    using xor0 = call<mp::xor0<>, xs...>;

    template<class... xs>
    using bit_and = number<(... & xs::value)>;

    template<class... xs>
    using bit_and0 = call<mp::bit_and0<>, xs...>;

    template<class... xs>
    using bit_or = number<(... | xs::value)>;

    template<class... xs>
    using bit_or0 = call<mp::bit_or0<>, xs...>;


    template<class x, class C = mp::identity>
    using neg = call<mp::neg<C>, x>;

    template<class x, class C = mp::identity>
    using unary_plus = call<mp::unary_plus<C>, x>;

    template<class x, class C = mp::identity>
    using not_ = call<mp::not_<C>, x>;

    template<class x, class C = mp::identity>
    using bit_not = call<mp::bit_not<C>, x>;

    template<class x, class C = mp::identity>
    using inc = call<mp::inc<C>, x>;

    template<class x, class C = mp::identity>
    using dec = call<mp::dec<C>, x>;

    template<class x, class y, class C = mp::identity>
    using equal = call<mp::equal<C>, x, y>;

    template<class x, class y, class C = mp::identity>
    using not_equal = call<mp::not_equal<C>, x, y>;

    template<class x, class y, class C = mp::identity>
    using less = call<mp::less<C>, x, y>;

    template<class x, class y, class C = mp::identity>
    using less_equal = call<mp::less_equal<C>, x, y>;

    template<class x, class y, class C = mp::identity>
    using greater = call<mp::greater<C>, x, y>;

    template<class x, class y, class C = mp::identity>
    using greater_equal = call<mp::greater_equal<C>, x, y>;
}

JLN_DIAGNOSTIC_POP
namespace jln::mp
{
    /// \cond
    namespace detail
    {
        template<class F, class C>
        struct not_fn_impl;
    }
    /// \endcond

    /// \ingroup functional

    /// Returns the negation of \c F.
    /// \treturn \bool
    template<class F, class C = identity>
    using not_fn = typename detail::not_fn_impl<F, C>::type;

    namespace emp
    {
        template<class F, class... xs>
        using not_of = mp::number<!detail::dcall<(sizeof...(xs) < JLN_MP_MAX_CALL_ELEMENT)>
            ::template f<F, xs...>::value>;

        template<class L, class F, class C = mp::identity>
        using not_fn = unpack<L, mp::not_fn<F, C>>;
    }
}


namespace jln::mp
{
    /// \ingroup utility

    /// \treturn \bool
    template <class T, class C = identity>
    struct is
    {
        template<class x>
        using f = typename C::template f<number<std::is_same<T, x>::value>>;
    };
} // namespace jln::mp
namespace jln::mp
{
    /// \ingroup number

    /// Converts a \value to a \bool.
    /// \treturn \bool
    template<class C = identity>
    struct to_bool
    {
        template<class x>
        using f = call<C, number<bool(x::value)>>;
    };

    namespace emp
    {
        template<class x>
        using to_bool = number<bool(x::value)>;
    }
}
/// \cond
namespace jln::mp::detail
{
    template<class F>
    struct to_not_fn;

    template<class F>
    using to_not_fn_t = typename to_not_fn<F>::type;

    template<class Pred>
    struct to_not_fn
    {
        using type = tee<Pred, not_<>>;
    };

    template<>
    struct to_not_fn<not_<>>
    {
        using type = not_<not_<>>;
    };

    template<>
    struct to_not_fn<not_<not_<>>>
    {
        using type = not_<>;
    };

    template<class C>
    struct to_not_fn<not_<to_bool<C>>>
        : to_not_fn<not_<C>>
    {};

    template<class C>
    struct to_not_fn<not_<not_<not_<C>>>>
        : to_not_fn<not_<C>>
    {};

    template<>
    struct to_not_fn<identity>
    {
        using type = not_<>;
    };

    template<class PredNot>
    struct optimise_not_not;

    template<class PredNot>
    struct optimise_not_not<not_<not_<PredNot>>>
    {
        using type = PredNot;
    };

    template<class PredNot>
    struct optimise_not_not
    {
        using type = PredNot;
    };

    template<class C>
    struct to_not_fn<to_bool<C>>
    {
        using type = to_bool<typename optimise_not_not<to_not_fn_t<C>>::type>;
    };

    template<class x, class C>
    struct to_not_fn<is<x, C>>
    {
        using type = is<x, typename optimise_not_not<to_not_fn_t<C>>::type>;
    };

    template<class C>
    struct to_not_fn<same<C>>
    {
        using type = same<typename optimise_not_not<to_not_fn_t<C>>::type>;
    };

    template<class PredNot>
    struct to_not_fn_lift;

    template<template<class...> class F, class C, class Not>
    struct to_not_fn_lift<lift<F, tee<C, Not>>>
    {
        using type = tee<lift<F, C>, Not>;
    };

    template<class PredNot>
    struct to_not_fn_lift
    {
        using type = PredNot;
    };

    template<template<class...> class F, class C>
    struct to_not_fn<lift<F, C>>
        : to_not_fn_lift<lift<F, to_not_fn_t<C>>>
    {};

    template<class x, class C>
    struct to_not_fn<push_front<x, C>>
    {
        using type = push_front<x, to_not_fn_t<C>>;
    };

    template<class x, class C>
    struct to_not_fn<push_back<x, C>>
    {
        using type = push_back<x, to_not_fn_t<C>>;
    };

    template<class Pred>
    struct to_not_fn_tee;

    template<class Pred>
    struct to_not_fn_tee<tee<Pred, not_<>>>
    {
        using type = tee<Pred, not_<not_<>>>;
    };

    template<class Pred>
    struct to_not_fn_tee
        : to_not_fn<Pred>
    {};

    template<class Pred>
    struct to_not_fn<tee<Pred, not_<>>>
        : to_not_fn_tee<to_not_fn_t<Pred>>
    {};

    template<class PredNot>
    struct to_not_fn_tee2;

    template<class PredNot>
    struct to_not_fn_tee2<tee<PredNot, not_<>>>
        : to_not_fn<PredNot>
    {};

    template<class PredNot>
    struct to_not_fn_tee2<tee<PredNot, not_<not_<>>>>
        : to_not_fn<PredNot>
    {};

    template<class PredNot>
    struct to_not_fn_tee2
    {
        using type = PredNot;
    };

    template<class Pred, class C>
    struct to_not_fn<tee<Pred, C>>
        : to_not_fn_tee2<tee<Pred, to_not_fn_t<C>>>
    {};

    template<class F, class C>
    struct not_fn_impl
        : to_not_fn<tee<F, C>>
    {};

    template<class F>
    struct not_fn_impl<F, identity>
        : to_not_fn<F>
    {};
}
/// \endcond
namespace jln::mp
{
    /// \cond
    namespace detail
    {
        template<class Pred>
        struct mk_wrap_in_list_if;

        template<class Pred>
        struct mk_wrap_in_list_if_not;
    }
    /// \endcond

    /// \ingroup list

    /// Returns a \list with the first element if the predicate is checked, otherwise returns a empty list.
    /// \pre \c Pred<xs...>::value must be narrowing convertible to bool
    /// \treturn \list
    template<class Pred>
    using wrap_in_list_if = typename detail::mk_wrap_in_list_if_not<
        detail::to_not_fn_t<Pred>
    >::type;

    /// Returns a \list with the first element if the predicate is not checked, otherwise returns a empty list.
    /// \pre \c Pred<xs...>::value must be narrowing convertible to bool
    /// \treturn \list
    template<class Pred>
    using wrap_in_list_if_not = typename detail::mk_wrap_in_list_if<
        detail::to_not_fn_t<Pred>
    >::type;

    template<bool b>
    struct wrap_in_list_c;

    template<>
    struct wrap_in_list_c<true>
    {
        template<class... xs>
        using f = list<xs...>;
    };

    template<>
    struct wrap_in_list_c<false>
    {
        template<class>
        using f = list<>;
    };

    template<class b>
    using wrap_in_list = wrap_in_list_c<b::value>;

    namespace emp
    {
        template<class Pred, class... xs>
        using wrap_in_list_if = typename mp::wrap_in_list_if<Pred>::template f<xs...>;

        template<class Pred, class... xs>
        using wrap_in_list_if_not = typename mp::wrap_in_list_if_not<Pred>::template f<xs...>;

        template<class b, class... xs>
        using wrap_in_list = typename mp::wrap_in_list_c<b::value>::template f<xs...>;

        template<bool b, class... xs>
        using wrap_in_list_c = typename mp::wrap_in_list_c<b>::template f<xs...>;
    }
}

/// \cond
namespace jln::mp::detail
{
    template<class Pred>
    struct _wrap_in_list_if
    {
        template<class x, class... xs>
        using f = typename wrap_in_list_c<call<Pred, x, xs...>::value>::template f<x>;
    };

    template<class Pred>
    struct _wrap_in_list_if_not
    {
        template<class x, class... xs>
        using f = typename wrap_in_list_c<!call<Pred, x, xs...>::value>::template f<x>;
    };

    template<class Pred>
    struct mk_wrap_in_list_if
    {
        using type = _wrap_in_list_if<Pred>;
    };

    template<class Pred>
    struct mk_wrap_in_list_if_not
    {
        using type = _wrap_in_list_if_not<Pred>;
    };


    template<class x>
    struct mk_wrap_in_list_if<is<x, not_<>>>
    {
        using type = _wrap_in_list_if_not<is<x>>;
    };

    template<>
    struct mk_wrap_in_list_if<same<not_<>>>
    {
        using type = _wrap_in_list_if_not<same<>>;
    };

    template<template<class...> class F>
    struct mk_wrap_in_list_if<lift<F, not_<>>>
    {
        using type = _wrap_in_list_if_not<lift<F>>;
    };

    template<class Pred>
    struct mk_wrap_in_list_if<tee<Pred, not_<>>>
    {
        using type = _wrap_in_list_if_not<Pred>;
    };


    template<class x>
    struct mk_wrap_in_list_if_not<is<x, not_<>>>
    {
        using type = _wrap_in_list_if<is<x>>;
    };

    template<>
    struct mk_wrap_in_list_if_not<same<not_<>>>
    {
        using type = _wrap_in_list_if<same<>>;
    };

    template<template<class...> class F>
    struct mk_wrap_in_list_if_not<lift<F, not_<>>>
    {
        using type = _wrap_in_list_if<lift<F>>;
    };

    template<class Pred>
    struct mk_wrap_in_list_if_not<tee<Pred, not_<>>>
    {
        using type = _wrap_in_list_if<Pred>;
    };
}
/// \endcond
/// \cond
namespace jln::mp::detail
{
    template<class y, class... ys>
    struct _adjacent_remove<list<y, ys...>>
    {
        template<class C, class BinaryPred, class x, class... xs>
        using f = typename join<C>::template f<
            list<x>,
            typename wrap_in_list_c<!BinaryPred::template f<xs, ys>::value>::template f<xs>...>;
    };

    template<class>
    struct _adjacent_remove
    {
        template<class C, class...>
        using f = typename C::template f<>;
    };
}
/// \endcond
namespace jln::mp
{
    /// \cond
    namespace detail
    {
        template<int, bool not_found>
        struct _drop_while;

        template<class>
        struct drop_while_impl;
    }
    /// \endcond

    /// \ingroup search

    /// Remove the first elements of a \sequence that satisfy a \predicate.
    /// \treturn \sequence
    /// \see drop_front, drop_back, take_while
    template<class Pred, class C = listify>
    struct drop_while
    {
        template<class... xs>
        using f = typename detail::drop_while_impl<
            typename detail::_drop_while<
            detail::n_8_or_more_16_32_64_128_256(sizeof...(xs)), true
      >::template f<0, Pred, xs...>
        >::template f<C, xs...>;
    };

    namespace emp
    {
        template<class L, class Pred, class C = mp::listify>
        using drop_while = unpack<L, mp::drop_while<Pred, C>>;
    }
}


#if defined(_MSC_VER) || defined(__clang__)
#endif

namespace jln::mp
{
    /// \cond
    namespace detail
    {
        template<unsigned>
        struct _drop_front;

#if defined(_MSC_VER) || defined(__clang__)
        template<int_ i, std::size_t n, class = void>
        struct validate_index
        {};

        template<int_ i, std::size_t n>
        struct validate_index<i, n, std::enable_if_t<(int_(n) - i >= 0)>>
        {
            static constexpr int_ value = i;
        };
#else
        template<int_ i, std::size_t n>
        using validate_index = number < (0 * std::size_t{ int_(n) - i }) + i > ;
#endif
    }
    /// \endcond

    /// \ingroup list

    /// Removes \c N elements from the beginning of a \sequence.
    /// \pre `0 <= N <= sizeof...(xs)`
    /// \treturn \sequence
    /// \see drop_front_max, drop_back, take_front, take_back, drop_while
    template<class N, class C = listify>
    struct drop_front
    {
        template<class... xs>
        using f = typename detail::_drop_front<
            detail::n_8_or_less_16_64_256(
                detail::validate_index<N::value, sizeof...(xs)>::value
            )
        >::template f<N::value, C, xs...>;
    };

    /// Removes at most \c N elements from the beginning of a \sequence.
    /// \pre `0 <= N`
    /// \treturn \sequence
    /// \see drop_front, drop_back, drop_while
    template<class N, class C = listify>
    struct drop_front_max
    {
        template<class... xs>
        using f = typename detail::_drop_front <
            detail::n_8_or_less_16_64_256(
                detail::min(std::size_t{ N::value }, sizeof...(xs))
            )
        > ::template f < detail::min(std::size_t{ N::value }, sizeof...(xs)), C, xs... > ;
    };

    template<int_ n, class C = listify>
    using drop_front_c = drop_front<number<n>, C>;

    template<int_ n, class C = listify>
    using drop_front_max_c = drop_front_max<number<n>, C>;

    namespace emp
    {
        template<class L, class N, class C = mp::listify>
        using drop_front = unpack<L, mp::drop_front<N, C>>;

        template<class L, int_ n, class C = mp::listify>
        using drop_front_c = unpack<L, mp::drop_front<number<n>, C>>;

        template<class L, class N, class C = mp::listify>
        using drop_front_max = unpack<L, mp::drop_front_max<N, C>>;

        template<class L, int_ n, class C = mp::listify>
        using drop_front_max_c = unpack<L, mp::drop_front_max<number<n>, C>>;
    }

    /// \cond
    template<class C>
    struct drop_front<number<0>, C>
    {
        template<class... xs>
        using f = JLN_MP_DCALL(sizeof...(xs) >= 0, C, xs...);
    };


    template<class C>
    struct drop_front_max<number<0>, C>
    {
        template<class... xs>
        using f = JLN_MP_DCALL(sizeof...(xs) >= 0, C, xs...);
    };
    /// \endcond
}

/// \cond
namespace jln::mp::detail
{
#define JLN_MP_DROP_IMPL(n, _, mp_rxs, mp_rep) \
  template<>                                   \
  struct _drop_front<n>                        \
  {                                            \
    template<unsigned size, class C,           \
      mp_rep(class JLN_MP_COMMA, JLN_MP_NIL)   \
      class... xs>                             \
    using f = typename C::template f<xs...>;   \
  };

    JLN_MP_GEN_XS_0_TO_8(JLN_MP_DROP_IMPL)

#undef JLN_MP_DROP_IMPL

#define JLN_MP_DROP_IMPL(n, mp_xs, mp_rxs, mp_rep) \
  JLN_MP_DROP_IMPL2(n, mp_xs, mp_rxs, mp_rep,      \
    n_8_or_less_16_64_256)

#define JLN_MP_DROP_IMPL2(n, _, mp_rxs, mp_rep, next_int) \
  template<>                                              \
  struct _drop_front<n>                                   \
  {                                                       \
    template<unsigned size, class C,                      \
      mp_rep(class JLN_MP_COMMA, JLN_MP_NIL)              \
      class... xs>                                        \
    using f = typename _drop_front<next_int (size-n)>     \
      ::template f<(size-n), C, xs...>;                   \
  };

        JLN_MP_GEN_XS_8_args(JLN_MP_DROP_IMPL2, JLN_MP_NIL)
        JLN_MP_GEN_XS_16_64_256(JLN_MP_DROP_IMPL)

#undef JLN_MP_DROP_IMPL2
#undef JLN_MP_DROP_IMPL
}
/// \endcond
/// \cond
namespace jln::mp::detail
{
    template<std::size_t n>
    struct _drop_while_result
    {
        template<class C, std::size_t consumed, class Pred, class... xs>
        using f = _drop_while_result;
    };

    struct _drop_while_continue
    {
        template<class C, std::size_t consumed, class Pred, class... xs>
        using f = typename C::template f<consumed, Pred, xs...>;
    };

    template<>
    struct drop_while_impl<_drop_while_continue>
    {
        template<class C, class...>
        using f = typename C::template f<>;
    };

    template<std::size_t n>
    struct drop_while_impl<_drop_while_result<n>>
    {
        template<class C, class... xs>
        using f = typename drop_front<number<sizeof...(xs) - n - 1>, C>::template f<xs...>;
    };

#define JLN_DROP_WHILE_IMPL(n, m)                                    \
  template<>                                                         \
  struct _drop_while<n, true>                                        \
  {                                                                  \
    template<std::size_t consumed, class Pred, class x, class... xs> \
    using f = typename _drop_while<m, Pred::template f<x>::value>    \
            ::template f<consumed, Pred, xs...>;                     \
  };                                                                 \
                                                                     \
  template<>                                                         \
  struct _drop_while<n, false>                                       \
  {                                                                  \
    template<std::size_t consumed, class Pred, class... xs>          \
    using f = _drop_while_result<consumed+sizeof...(xs)>;            \
  }

    JLN_DROP_WHILE_IMPL(7, 6);
    JLN_DROP_WHILE_IMPL(6, 5);
    JLN_DROP_WHILE_IMPL(5, 4);
    JLN_DROP_WHILE_IMPL(4, 3);
    JLN_DROP_WHILE_IMPL(3, 2);
    JLN_DROP_WHILE_IMPL(2, 1);
    JLN_DROP_WHILE_IMPL(1, 0);

#undef JLN_DROP_WHILE_IMPL

    template<>
    struct _drop_while<0, true>
    {
        template<std::size_t consumed, class Pred, class... xs>
        using f = _drop_while_continue;
    };

    template<>
    struct _drop_while<0, false>
    {
        template<std::size_t consumed, class Pred, class... xs>
        using f = _drop_while_result<consumed + sizeof...(xs)>;
    };

    template<>
    struct _drop_while<8, true>
    {
        template<
            std::size_t consumed,
            class Pred,
            JLN_MP_XS_8(class, JLN_MP_NIL, JLN_MP_COMMA),
            class... xs>
            using f = typename _drop_while<7, Pred::template f<_1>::value>
            ::template f<
            consumed + sizeof...(xs), Pred,
            JLN_MP_XS_2_TO_8(JLN_MP_NIL, JLN_MP_NIL, JLN_MP_COMMA)>;
    };

    template<>
    struct _drop_while<16, true>
    {
        template<
            std::size_t consumed,
            class Pred,
            JLN_MP_XS_8(class, JLN_MP_NIL, JLN_MP_COMMA),
            class... xs>
            using f = typename _drop_while<7, Pred::template f<_1>::value>
            ::template f<
            consumed + sizeof...(xs), Pred,
            JLN_MP_XS_2_TO_8(JLN_MP_NIL, JLN_MP_NIL, JLN_MP_COMMA)>
            ::template f<
            _drop_while<n_8_or_more_16_32_64_128_256(sizeof...(xs)), true>,
            consumed, Pred, xs...>;
    };

#define JLN_DROP_WHILE_IMPL(n, m, xs)                                     \
  template<>                                                              \
  struct _drop_while<n, true>                                             \
  {                                                                       \
    template<                                                             \
      std::size_t consumed,                                               \
      class Pred,                                                         \
      xs(class, JLN_MP_NIL, JLN_MP_COMMA),                                \
      class... xs>                                                        \
    using f = typename _drop_while<m, true>                               \
      ::template f<                                                       \
          consumed+sizeof...(xs), Pred,                                   \
          xs(JLN_MP_NIL, JLN_MP_NIL, JLN_MP_COMMA)>                       \
      ::template f<                                                       \
          _drop_while<n_8_or_more_16_32_64_128_256(sizeof...(xs)), true>, \
          consumed, Pred, xs...>;                                         \
  }

    JLN_DROP_WHILE_IMPL(32, 16, JLN_MP_XS_16);
    JLN_DROP_WHILE_IMPL(64, 32, JLN_MP_XS_32);
    JLN_DROP_WHILE_IMPL(128, 64, JLN_MP_XS_64);
    JLN_DROP_WHILE_IMPL(256, 128, JLN_MP_XS_128);

#undef JLN_DROP_WHILE_IMPL
}
/// \endcond
namespace jln::mp
{
    /// \ingroup list

    /// Removes all elements from the \sequence.
    /// \treturn \number
    template<class C = listify>
    struct clear
    {
        template<class... xs>
        using f = JLN_MP_DCALL(sizeof...(xs) >= 0, C);
    };
}
namespace jln::mp
{
    /// \cond
    namespace detail
    {
        template<class>
        struct find_if_impl;
    }
    /// \endcond

    /// \ingroup search

    /// Finds the first element that satisfy a \predicate.
    /// Calls \c TC with all the elements since the one found at the end.
    /// If no element is found, \c FC is used with the whole \sequence.
    /// \treturn \sequence
    /// \see find_if_not, find, find_last_if, find_last
    template<class Pred, class TC = listify, class FC = clear<TC>>
    struct find_if
    {
        template<class... xs>
        using f = typename detail::find_if_impl<
            typename detail::_drop_while<
            detail::n_8_or_more_16_32_64_128_256(sizeof...(xs)), true
      >::template f<0, detail::to_not_fn_t<Pred>, xs...>
        >::template f<TC, FC, xs...>;
    };

    /// \see find_if, find, find_last_if, find_last
    template<class Pred, class TC = listify, class FC = clear<TC>>
    using find_if_not = find_if<not_fn<Pred>, TC, FC>;

    /// \see find_if, find_if_not, find_last_if, find_last
    template<class T, class TC = listify, class FC = clear<TC>>
    using find = find_if<is<T>, TC, FC>;

    namespace emp
    {
        template<class L, class Pred, class TC = mp::listify, class FC = clear<TC>>
        using find_if = unpack<L, mp::find_if<Pred, TC, FC>>;

        template<class L, class Pred, class TC = mp::listify, class FC = clear<TC>>
        using find_if_not = unpack<L, mp::find_if_not<Pred, TC, FC>>;

        template<class L, class T, class TC = mp::listify, class FC = clear<TC>>
        using find = unpack<L, mp::find_if<mp::is<T>, TC, FC>>;
    }
}


/// \cond
namespace jln::mp::detail
{
    template<>
    struct find_if_impl<_drop_while_continue>
    {
        template<class TC, class FC, class... xs>
        using f = typename FC::template f<xs...>;
    };

    template<std::size_t n>
    struct find_if_impl<_drop_while_result<n>>
    {
        template<class TC, class FC, class... xs>
        using f = typename drop_front<number<sizeof...(xs) - n - 1>, TC>::template f<xs...>;
    };
}
/// \endcond
namespace jln::mp
{
    /// \cond
    namespace detail
    {
        template<class>
        struct index_if_impl;
    }
    /// \endcond

    /// \ingroup search

    /// Finds the index of the first element of \sequence
    /// that satisfies the \predicate \c Pred.
    /// Calls \c TC with the index found or \c FC with the whole \sequence.
    /// \treturn \number
    template<class Pred, class TC = identity, class FC = size<>>
    struct index_if
    {
        template<class... xs>
        using f = typename detail::index_if_impl<
            typename detail::_drop_while<
            detail::n_8_or_more_16_32_64_128_256(sizeof...(xs)), true
      >::template f<0, detail::to_not_fn_t<Pred>, xs...>
        >::template f<TC, FC, xs...>;
    };

    /// Finds the index of the first element of \sequence that is a type \c T.
    /// Calls \c TC with the index found or \c FC with the whole \sequence.
    /// \treturn \number
    template<class T, class TC = identity, class FC = size<>>
    using index_of = index_if<is<T>, TC, FC>;

    namespace emp
    {
        template<class L, class Pred, class TC = mp::identity, class FC = mp::size<>>
        using index_if = unpack<L, mp::index_if<Pred, TC, FC>>;

        template<class L, class T, class TC = mp::identity, class FC = mp::size<>>
        using index_of = unpack<L, mp::index_of<T, TC, FC>>;
    }
}

/// \cond
namespace jln::mp::detail
{
    template<>
    struct index_if_impl<_drop_while_continue>
    {
        template<class TC, class FC, class... xs>
        using f = typename FC::template f<xs...>;
    };

    template<std::size_t n>
    struct index_if_impl<_drop_while_result<n>>
    {
        template<class TC, class FC, class... xs>
        using f = typename TC::template f<number<sizeof...(xs) - n - 1>>;
    };
}
/// \endcond
namespace jln::mp
{
    /// \cond
    namespace detail
    {
        template<int, bool found>
        struct _search;

        template<class>
        struct search_before_impl;

        template<class>
        struct search_before_extended_by_n_impl;
    }
    /// \endcond

    /// \ingroup search

    /// Search the first sub-\sequence that satisfy a \predicate.
    /// Calls \c TC with all the elements from sub-\sequence found at the end.
    /// If no element is found, \c FC is used with the whole \sequence.
    /// \treturn \sequence
    /// \see search_before, partial_search, after, search_index
    /// \see search_before_extended_by_n
    /// \see partial_search_before, partial_search_before_extended_by_n
    template<class Pred, class TC = listify, class FC = clear<TC>>
    struct search
    {
        template<class... xs>
        using f = typename detail::find_if_impl<
            typename detail::_search<
            detail::n_8_or_more_16_32_64_128_256(sizeof...(xs)), false
      >::template f<sizeof...(xs), Pred, xs...>
        >::template f<TC, FC, xs...>;
    };

    /// Search elements before sub-\sequence that satisfy a \predicate.
    /// Calls \c TC with the elements from the beginning to sub-\sequence found.
    /// If no element is found, \c FC is used with the whole \sequence.
    /// \treturn \sequence
    /// \see search, partial_search, after, search_index
    /// \see search_before_extended_by_n
    /// \see partial_search_before, partial_search_before_extended_by_n
    template<class Pred, class TC = listify, class FC = clear<TC>>
    struct search_before
    {
        template<class... xs>
        using f = typename detail::search_before_impl<
            typename detail::_search<
            detail::n_8_or_more_16_32_64_128_256(sizeof...(xs)), false
      >::template f<sizeof...(xs), Pred, xs...>
        >::template f<TC, FC, xs...>;
    };

    /// Search elements before sub-\sequence that satisfy a \predicate.
    /// Calls \c TC with the elements from the beginning to sub-\sequence found + \c ExtendedByN.
    /// If no element is found, \c FC is used with the whole \sequence.
    /// \treturn \sequence
    /// \see search, search_before, partial_search, after, search_index
    /// \see partial_search_before, partial_search_before_extended_by_n
    template<class Pred, class ExtendedByN, class TC = listify, class FC = clear<TC>>
    struct search_before_extended_by_n
    {
        template<class... xs>
        using f = typename detail::search_before_extended_by_n_impl<
            typename detail::_search<
            detail::n_8_or_more_16_32_64_128_256(sizeof...(xs)), false
      >::template f<sizeof...(xs), Pred, xs...>
        >::template f<TC, FC, ExtendedByN, xs...>;
    };

    /// Same \c search, but it stops when there is StopWhenAtLeast::value element or less.
    /// \treturn \sequence
    /// \see search, search_before, after, search_index
    /// \see search_before_extended_by_n
    /// \see partial_search_before, partial_search_before_extended_by_n
    template<class StopWhenAtLeast, class Pred, class TC = listify, class FC = clear<TC>>
    struct partial_search
    {
        template<class... xs>
        using f = typename detail::find_if_impl<
            typename detail::_search<
            detail::n_8_or_more_16_32_64_128_256(
                sizeof...(xs) > StopWhenAtLeast::value ? sizeof...(xs) - StopWhenAtLeast::value : 0
            ), false
      >::template f<size_t(sizeof...(xs) - StopWhenAtLeast::value), Pred, xs...>
        >::template f<TC, FC, xs...>;
    };

    template<int_ StopWhenAtLeast, class Pred, class TC = listify, class FC = clear<TC>>
    using partial_search_c = partial_search<number<StopWhenAtLeast>, Pred, TC, FC>;

    /// Same \c search_before, but it stops when there is StopWhenAtLeast::value element or less.
    /// \treturn \sequence
    /// \see search, search_before, partial_search, after, search_index
    /// \see search_before_extended_by_n, partial_search_before_extended_by_n
    template<class StopWhenAtLeast, class Pred, class TC = listify, class FC = clear<TC>>
    struct partial_search_before
    {
        template<class... xs>
        using f = typename detail::search_before_impl<
            typename detail::_search<
            detail::n_8_or_more_16_32_64_128_256(
                sizeof...(xs) > StopWhenAtLeast::value ? sizeof...(xs) - StopWhenAtLeast::value : 0
            ), false
      >::template f<size_t(sizeof...(xs) - StopWhenAtLeast::value), Pred, xs...>
        >::template f<TC, FC, xs...>;
    };

    template<int_ StopWhenAtLeast, class Pred, class TC = listify, class FC = clear<TC>>
    using partial_search_before_c = partial_search_before<number<StopWhenAtLeast>, Pred, TC, FC>;

    /// Same \c search_before, but it stops when there is StopWhenAtLeast::value element or less.
    /// \treturn \sequence
    /// \see search, search_before, partial_search, after, search_index
    /// \see search_before_extended_by_n, partial_search_before
    template<class StopWhenAtLeast, class Pred, class ExtendedByN,
        class TC = listify, class FC = clear<TC>>
        struct partial_search_before_extended_by_n
    {
        template<class... xs>
        using f = typename detail::search_before_extended_by_n_impl<
            typename detail::_search<
            detail::n_8_or_more_16_32_64_128_256(
                sizeof...(xs) > StopWhenAtLeast::value ? sizeof...(xs) - StopWhenAtLeast::value : 0
            ), false
      >::template f<size_t(sizeof...(xs) - StopWhenAtLeast::value), Pred, xs...>
        >::template f<TC, FC, ExtendedByN, xs...>;
    };

    template<int_ StopWhenAtLeast, class Pred, class ExtendedByN, class TC = listify, class FC = clear<TC>>
    using partial_search_before_extended_by_n_c
        = partial_search_before_extended_by_n<number<StopWhenAtLeast>, Pred, ExtendedByN, TC, FC>;

    /// Search the index of first sub-\sequence that satisfy a \predicate.
    /// \treturn \sequence
    /// \see search, search_before, after
    template<class Pred, class TC = identity, class FC = size<>>
    struct search_index
    {
        template<class... xs>
        using f = typename detail::index_if_impl<
            typename detail::_search<
            detail::n_8_or_more_16_32_64_128_256(sizeof...(xs)), false
      >::template f<sizeof...(xs), Pred, xs...>
        >::template f<TC, FC, xs...>;
    };

    namespace emp
    {
        template<class L, class Pred, class TC = mp::listify, class FC = mp::clear<TC>>
        using search = unpack<L, mp::search<Pred, TC, FC>>;

        template<class L, class Pred, class TC = mp::listify, class FC = mp::clear<TC>>
        using search_before = unpack<L, mp::search_before<Pred, TC, FC>>;

        template<class L, class Pred, class ExtendedByN,
            class TC = mp::listify, class FC = mp::clear<TC>>
            using search_before_extended_by_n = unpack<L,
            mp::search_before_extended_by_n<Pred, ExtendedByN, TC, FC>>;


        template<class L, class StopWhenAtLeast, class Pred,
            class TC = mp::listify, class FC = mp::clear<TC>>
            using partial_search = unpack<L, mp::partial_search<Pred, StopWhenAtLeast, TC, FC>>;

        template<class L, class StopWhenAtLeast, class Pred,
            class TC = mp::listify, class FC = mp::clear<TC>>
            using partial_search_before = unpack<L,
            mp::partial_search_before<Pred, StopWhenAtLeast, TC, FC>>;

        template<class L, class StopWhenAtLeast, class Pred, class ExtendedByN,
            class TC = mp::listify, class FC = mp::clear<TC>>
            using partial_search_before_extended_by_n = unpack<L,
            mp::partial_search_before_extended_by_n<Pred, StopWhenAtLeast, TC, FC>>;


        template<class L, int_ StopWhenAtLeast, class Pred,
            class TC = mp::listify, class FC = mp::clear<TC>>
            using partial_search_c = unpack<L, mp::partial_search<Pred, number<StopWhenAtLeast>, TC, FC>>;

        template<class L, int_ StopWhenAtLeast, class Pred,
            class TC = mp::listify, class FC = mp::clear<TC>>
            using partial_search_before_c = unpack<L,
            mp::partial_search_before<Pred, number<StopWhenAtLeast>, TC, FC>>;

        template<class L, int_ StopWhenAtLeast, class Pred, class ExtendedByN,
            class TC = mp::listify, class FC = mp::clear<TC>>
            using partial_search_before_extended_by_n_c = unpack<L,
            mp::partial_search_before_extended_by_n<Pred, mp::number<StopWhenAtLeast>, TC, FC>>;


        template<class L, class Pred, class TC = mp::identity, class FC = mp::size<>>
        using search_index = unpack<L, mp::search<Pred, TC, FC>>;
    }
}

namespace jln::mp
{
    /// \ingroup list

    /// Retrieves the first element of a sequence.
    /// \treturn \value
    template<class C = identity>
    struct front
    {
        template<class x, class... xs>
        using f = typename C::template f<x>;
    };

    namespace emp
    {
        template<class L, class C = mp::identity>
        using front = unpack<L, front<C>>;
    }
}

namespace jln::mp
{
    /// \cond
    template<>
    struct front<identity>
    {
        template<class x, class...>
        using f = x;
    };
    /// \endcond
}
namespace jln::mp
{
    /// \ingroup list

    /// Extracts \c N elements from the beginning of a \sequence.
    /// \pre `0 <= N <= sizeof...(xs)`
    /// \treturn \sequence
    /// \see take_front_max, take_back, drop_front, drop_back, take_while
    template<class N, class C = listify>
    struct take_front
    {
        template<class... xs>
        using f = typename rotate<N, drop_front<number<sizeof...(xs) - N::value>, C>>
            ::template f<xs...>;
    };

    /// Extracts at most \c N elements from the beginning of a \sequence.
    /// \pre `0 <= N`
    /// \treturn \sequence
    /// \see take_front, take_back, take_while
    template<class N, class C = listify>
    struct take_front_max
    {
        template<class... xs>
        using f = typename rotate < N, drop_front_max < number < sizeof...(xs)
            - detail::min(sizeof...(xs), std::size_t{ N::value }) > , C >>
            ::template f<xs...>;
    };

    template<int_ n, class C = listify>
    using take_front_c = take_front<number<n>, C>;

    template<int_ n, class C = listify>
    using take_front_max_c = take_front_max<number<n>, C>;

    namespace emp
    {
        template<class L, class N, class C = mp::listify>
        using take_front = unpack<L, mp::take_front<N, C>>;

        template<class L, int_ n, class C = mp::listify>
        using take_front_c = unpack<L, mp::take_front<number<n>, C>>;

        template<class L, class N, class C = mp::listify>
        using take_front_max = unpack<L, mp::take_front_max<N, C>>;

        template<class L, int_ n, class C = mp::listify>
        using take_front_max_c = unpack<L, mp::take_front_max<number<n>, C>>;
    }

    /// \cond
    template<class C>
    struct take_front<number<0>, C>
    {
        template<class... xs>
        using f = JLN_MP_DCALL(sizeof...(xs) >= 0, C);
    };

    template<class C>
    struct take_front_max<number<0>, C>
    {
        template<class... xs>
        using f = JLN_MP_DCALL(sizeof...(xs) >= 0, C);
    };
    /// \endcond
}

namespace jln::mp
{
    /// \ingroup algorithm

    /// Checks if the \sequence begins with the given prefix.
    /// \treturn \bool
    /// \see ends_with
    template<class Seq, class C = identity>
    struct starts_with;

    namespace emp
    {
        template<class L, class Seq, class C = mp::identity>
        using starts_with = unpack<starts_with<Seq, C>, L>;
    }

    template<class... Ts, class C>
    struct starts_with<list<Ts...>, C>
    {
        template<class... xs>
        using f = typename C::template f<
            typename conditional_c<sizeof...(Ts) <= sizeof...(xs)>
            ::template f<take_front_c<sizeof...(Ts), lift<list, is<list<Ts...>>>>,
            always<false_>>
            ::template f<xs...>
            >;
    };

    /// \cond
    template<class T, class C>
    struct starts_with<list<T>, C>
    {
        template<class... xs>
        using f = typename C::template f<
            typename conditional_c<1 <= sizeof...(xs)>
            ::template f<front<is<T>>, always<false_>>
            ::template f<xs...>
        >;
    };

    template<class C>
    struct starts_with<list<>, C>
    {
        template<class... xs>
        using f = typename C::template f<true_>;
    };
    /// \endcond
}
namespace jln::mp
{
    /// \ingroup list

    /// Extracts \c N elements from the end of a \sequence.
    /// \pre `0 <= N <= sizeof...(xs)`
    /// \treturn \sequence
    /// \see take_back_max, take_front, drop_front, drop_back, take_while
    template<class N, class C = listify>
    struct take_back
    {
        template<class... xs>
        using f = typename detail::_drop_front<
            detail::n_8_or_less_16_64_256(
                sizeof...(xs) - detail::validate_index<N::value, sizeof...(xs)>::value
            )
        >::template f<sizeof...(xs) - N::value, C, xs...>;
    };

    /// Extracts at most \c N elements from the end of a \sequence.
    /// \pre `0 <= N`
    /// \treturn \sequence
    /// \see take_front, take_back, take_while
    template<class N, class C = listify>
    struct take_back_max
    {
        template<class... xs>
        using f = typename detail::_drop_front <
            detail::n_8_or_less_16_64_256(
                sizeof...(xs) - detail::min(std::size_t{ N::value }, sizeof...(xs))
            )
        > ::template f < sizeof...(xs) - detail::min(std::size_t{ N::value }, sizeof...(xs)), C, xs... > ;
    };

    template<int_ n, class C = listify>
    using take_back_c = take_back<number<n>, C>;

    template<int_ n, class C = listify>
    using take_back_max_c = take_back_max<number<n>, C>;

    namespace emp
    {
        template<class L, class N, class C = mp::listify>
        using take_back = unpack<L, mp::take_back<N, C>>;

        template<class L, int_ n, class C = mp::listify>
        using take_back_c = unpack<L, mp::take_back<number<n>, C>>;

        template<class L, class N, class C = mp::listify>
        using take_back_max = unpack<L, mp::take_back_max<N, C>>;

        template<class L, int_ n, class C = mp::listify>
        using take_back_max_c = unpack<L, mp::take_back_max<number<n>, C>>;
    }

    /// \cond
    template<class C>
    struct take_back<number<0>, C>
    {
        template<class... xs>
        using f = JLN_MP_DCALL(sizeof...(xs) >= 0, C);
    };

    template<class C>
    struct take_back_max<number<0>, C>
    {
        template<class... xs>
        using f = JLN_MP_DCALL(sizeof...(xs) >= 0, C);
    };
    /// \endcond
}
namespace jln::mp
{
    /// \ingroup algorithm

    /// Checks if the \sequence ends with the given prefix.
    /// \treturn \bool
    /// \see starts_with
    template<class Seq, class C = identity>
    struct ends_with;

    namespace emp
    {
        template<class L, class Seq, class C = mp::identity>
        using ends_with = unpack<ends_with<Seq, C>, L>;
    }

    template<class... Ts, class C>
    struct ends_with<list<Ts...>, C>
    {
        template<class... xs>
        using f = typename C::template f<
            typename conditional_c<sizeof...(Ts) <= sizeof...(xs)>
            ::template f<take_back_c<sizeof...(Ts), lift<list, is<list<Ts...>>>>,
            always<false_>>
            ::template f<xs...>
            >;
    };

    /// \cond
    template<class T, class C>
    struct ends_with<list<T>, C>
    {
        template<class... xs>
        using f = typename C::template f<
            typename conditional_c<1 <= sizeof...(xs)>
            ::template f<take_back_c<1, is<T>>, always<false_>>
            ::template f<xs...>
        >;
    };

    template<class C>
    struct ends_with<list<>, C>
    {
        template<class... xs>
        using f = typename C::template f<true_>;
    };
    /// \endcond
}
/// \cond
namespace jln::mp
{
    // optimize search with starts_with
    template<class T, class U, class... Ts, class C, class TC, class FC>
    struct search<starts_with<list<T, U, Ts...>, C>, TC, FC>
        : partial_search<number<sizeof...(Ts) + 1>,
        starts_with<list<T, U, Ts...>, C>, TC, FC>
    {};

    // optimize search_before with starts_with
    template<class T, class U, class... Ts, class C, class TC, class FC>
    struct search_before<starts_with<list<T, U, Ts...>, C>, TC, FC>
        : partial_search_before<number<sizeof...(Ts) + 1>,
        starts_with<list<T, U, Ts...>, C>, TC, FC>
    {};

    // optimize search_before_extended_by_n with starts_with
    template<class T, class U, class... Ts, class C, class ExtendedByN, class TC, class FC>
    struct search_before_extended_by_n<starts_with<list<T, U, Ts...>, C>, ExtendedByN, TC, FC>
        : partial_search_before_extended_by_n<number<sizeof...(Ts) + 1>,
        starts_with<list<T, U, Ts...>, C>, ExtendedByN, TC, FC>
    {};

    // optimize search_index with starts_with
    template<class T, class U, class... Ts, class C, class TC, class FC>
    struct search_index<starts_with<list<T, U, Ts...>, C>, TC, FC>
    {
        template<class... xs>
        using f = typename detail::index_if_impl<
            typename detail::_search<
            detail::n_8_or_more_16_32_64_128_256(
                sizeof...(xs) > sizeof...(Ts) + 1 ? sizeof...(xs) - sizeof...(Ts) - 1 : 0
            ), false
      >::template f<sizeof...(xs) - sizeof...(Ts) - 1u, starts_with<list<T, U, Ts...>, C>, xs...>
            >::template f<TC, FC, xs...>;
    };

    // optimize search with ends_with
    template<class T, class U, class... Ts, class C, class TC, class FC>
    struct search<ends_with<list<T, U, Ts...>, C>, TC, FC>
        : partial_search<number<sizeof...(Ts) + 1>,
        ends_with<list<T, U, Ts...>, C>, TC, FC>
    {};

    // optimize search_before with ends_with
    template<class T, class U, class... Ts, class C, class TC, class FC>
    struct search_before<ends_with<list<T, U, Ts...>, C>, TC, FC>
        : partial_search_before<number<sizeof...(Ts) + 1>,
        ends_with<list<T, U, Ts...>, C>, TC, FC>
    {};

    // optimize search_before_extended_by_n with ends_with
    template<class T, class U, class... Ts, class C, class ExtendedByN, class TC, class FC>
    struct search_before_extended_by_n<ends_with<list<T, U, Ts...>, C>, ExtendedByN, TC, FC>
        : partial_search_before_extended_by_n<number<sizeof...(Ts) + 1>,
        ends_with<list<T, U, Ts...>, C>, ExtendedByN, TC, FC>
    {};

    // optimize search_index with ends_with
    template<class T, class U, class... Ts, class C, class TC, class FC>
    struct search_index<ends_with<list<T, U, Ts...>, C>, TC, FC>
    {
        template<class... xs>
        using f = typename detail::index_if_impl<
            typename detail::_search<
            detail::n_8_or_more_16_32_64_128_256(
                sizeof...(xs) > sizeof...(Ts) + 1 ? sizeof...(xs) - sizeof...(Ts) - 1 : 0
            ), false
      >::template f<sizeof...(xs) - sizeof...(Ts) - 1u, ends_with<list<T, U, Ts...>, C>, xs...>
        >::template f<TC, FC, xs...>;
    };
}
/// \endcond

/// \cond
namespace jln::mp::detail
{
    template<>
    struct search_before_impl<_drop_while_continue>
    {
        template<class TC, class FC, class... xs>
        using f = typename FC::template f<xs...>;
    };

    template<std::size_t n>
    struct search_before_impl<_drop_while_result<n>>
    {
        template<class TC, class FC, class... xs>
        using f = typename take_front<number<sizeof...(xs) - n - 1>, TC>::template f<xs...>;
    };


    template<>
    struct search_before_extended_by_n_impl<_drop_while_continue>
    {
        template<class TC, class FC, class ExtendedByN, class... xs>
        using f = typename FC::template f<xs...>;
    };

    constexpr int_ extended_by_n(int_ seq_len, int_ n, int_ extended)
    {
        auto i = seq_len - n;
        auto take = i + extended;
        return (take < 0) ? 0
            : (take > seq_len) ? seq_len
            : take;
    }

    template<std::size_t n>
    struct search_before_extended_by_n_impl<_drop_while_result<n>>
    {
        template<class TC, class FC, class ExtendedByN, class... xs>
        using f = typename take_front<number<
            extended_by_n(sizeof...(xs), n + 1, ExtendedByN::value)
        >, TC>::template f<xs...>;
    };


#define JLN_DROP_WHILE_IMPL(n, m)                                     \
  template<>                                                          \
  struct _search<n, false>                                            \
  {                                                                   \
    template<std::size_t remaining, class Pred, class x, class... xs> \
    using f = typename _search<m, Pred::template f<x, xs...>::value>  \
            ::template f<remaining-1, Pred, xs...>;                   \
  };                                                                  \
                                                                      \
  template<>                                                          \
  struct _search<n, true>                                             \
  {                                                                   \
    template<std::size_t remaining, class Pred, class... xs>          \
    using f = _drop_while_result<sizeof...(xs)>;                      \
  }

    JLN_DROP_WHILE_IMPL(7, 6);
    JLN_DROP_WHILE_IMPL(6, 5);
    JLN_DROP_WHILE_IMPL(5, 4);
    JLN_DROP_WHILE_IMPL(4, 3);
    JLN_DROP_WHILE_IMPL(3, 2);
    JLN_DROP_WHILE_IMPL(2, 1);
    JLN_DROP_WHILE_IMPL(1, 0);

#undef JLN_DROP_WHILE_IMPL

    template<>
    struct _search<0, false>
    {
        template<std::size_t remaining, class Pred, class... xs>
        using f = _drop_while_continue;
    };

    template<>
    struct _search<0, true>
    {
        template<std::size_t remaining, class Pred, class... xs>
        using f = _drop_while_result<sizeof...(xs)>;
    };

    template<>
    struct _search<8, false>
    {
        template<std::size_t remaining, class Pred, class x, class... xs>
        using f = typename _search<7, Pred::template f<x, xs...>::value>
            ::template f<remaining - 8, Pred, xs...>;
    };

    template<>
    struct _search<16, false>
    {
        template<
            std::size_t remaining,
            class Pred,
            JLN_MP_XS_8(class, JLN_MP_NIL, JLN_MP_COMMA),
            class... xs>
            using f = typename _search<7, Pred::template f<
            JLN_MP_XS_8(JLN_MP_NIL, JLN_MP_NIL, JLN_MP_COMMA), xs...
            >::value>
            ::template f<7, Pred,
            JLN_MP_XS_2_TO_8(JLN_MP_NIL, JLN_MP_NIL, JLN_MP_COMMA),
            xs...>
            ::template f<_search<n_8_or_more_16_32_64_128_256(remaining - 8), false>,
            remaining - 8, Pred, xs...>;
    };

#define JLN_DROP_WHILE_IMPL(n, m, xs)                                         \
  template<>                                                                  \
  struct _search<n, false>                                                    \
  {                                                                           \
    template<                                                                 \
      std::size_t remaining,                                                  \
      class Pred,                                                             \
      xs(class, JLN_MP_NIL, JLN_MP_COMMA),                                    \
      class... xs>                                                            \
    using f = typename _search<m, false>                                      \
      ::template f<m, Pred, xs(JLN_MP_NIL, JLN_MP_NIL, JLN_MP_COMMA), xs...>  \
      ::template f<_search<n_8_or_more_16_32_64_128_256(remaining-m), false>, \
                   remaining-m, Pred, xs...>;                                 \
  }

    JLN_DROP_WHILE_IMPL(32, 16, JLN_MP_XS_16);
    JLN_DROP_WHILE_IMPL(64, 32, JLN_MP_XS_32);
    JLN_DROP_WHILE_IMPL(128, 64, JLN_MP_XS_64);
    JLN_DROP_WHILE_IMPL(256, 128, JLN_MP_XS_128);

#undef JLN_DROP_WHILE_IMPL
}
/// \endcond
namespace jln::mp
{
    /// \ingroup search

    /// Find the \sequence after a sub-\sequence.
    /// Calls \c TC with all the elements after the sub-\sequence found.
    /// If no element is found, \c FC is used with the whole \sequence.
    /// \treturn \sequence
    /// \see search, search_before
    template<class Seq, class TC = listify, class FC = clear<TC>>
    struct after;

    template<class... Ts, class TC, class FC>
    struct after<list<Ts...>, TC, FC>
        : search<starts_with<list<Ts...>>, drop_front_c<sizeof...(Ts), TC>, FC>
    {};

    namespace emp
    {
        template<class L, class Seq, class TC = mp::listify, class FC = mp::clear<TC>>
        using after = unpack<L, mp::after<Seq, TC, FC>>;
    }

    /// \cond
    template<class TC, class FC>
    struct after<list<>, TC, FC>
    {
        template<class... xs>
        using f = JLN_MP_DCALL(0 <= sizeof...(xs), TC, xs...);
    };
    /// \endcond
}
namespace jln::mp
{
    /// \ingroup list

    /// Checks whether a sequence has no elements.
    /// \treturn \number
    template<class C = identity>
    using is_empty = size<not_<C>>;

    namespace emp
    {
        template<class L, class C = mp::identity>
        using is_empty = unpack<L, mp::is_empty<C>>;
    }
}
namespace jln::mp
{
    /// \ingroup algorithm

    /// Checks whether a predicate holds for all elements of a \sequence.
    /// \treturn \bool
    template<class Pred, class C = identity>
    using all_of = drop_while<not_fn<not_fn<Pred>>, is_empty<C>>;

    namespace emp
    {
        template<class L, class Pred, class C = mp::identity>
        using all_of = unpack<L, mp::all_of<Pred, C>>;
    }
} // namespace jln::mp
namespace jln::mp
{
    /// \ingroup algorithm

    /// Checks whether a predicate holds for at least some element of a \sequence.
    /// \treturn \bool
    template<class Pred, class C = identity>
    using any_of = drop_while<not_fn<Pred>, is_empty<not_<C>>>;

    namespace emp
    {
        template<class L, class Pred, class C = mp::identity>
        using any_of = unpack<L, mp::any_of<Pred, C>>;
    }
} // namespace jln::mp
namespace jln::mp
{
    /// \cond
    namespace detail
    {
        template<class, unsigned>
        struct _cartesian;
    }
    /// \endcond

    /// \ingroup algorithm

    /// Computes the cartesian product of \lists.
    /// \pre `emp::is_list<seqs> && ...`
    /// \post `sizeof...(result) == (emp::size<seqs> * ...)` if `sizeof...(xs) != 0` else `0`
    /// \semantics
    ///   \code
    ///   call<cartesian<listify>,
    ///     list<_0, _1, _2>,
    ///     list<_3, _4>,
    ///     list<_5>
    ///   > = list<
    ///     list<_0, _3, _5>, list<_0, _4, _5>,
    ///     list<_1, _3, _5>, list<_1, _4, _5>,
    ///     list<_2, _3, _5>, list<_2, _4, _5>
    ///   >
    ///   \endcode
    /// \treturn \sequence
    template<class C = listify>
    struct cartesian
    {
        template<class... seqs>
        using f = typename detail::_cartesian<C, detail::min(2, sizeof...(seqs))>
            ::template f<seqs...>;
    };

    namespace emp
    {
        template<class L, class C = mp::listify>
        using cartesian = unpack<L, cartesian<C>>;
    }
}


/// \cond
namespace jln::mp::detail
{
    template<class, class>
    struct _product;

    template<class... xs, class... ys>
    struct _product<list<xs...>, list<ys...>>
    {
        using type = list<list<xs..., ys>...>;
    };

    template<class... ys>
    struct _product<list<>, list<ys...>>
    {
        using type = list<>;
    };

    template<class>
    struct _product1;

    template<class... xs>
    struct _product1<list<xs...>>
    {
        using type = list<list<xs>...>;
    };

    template<class C>
    struct _cartesian<C, 0>
    {
        template<class...>
        using f = typename C::template f<>;
    };

    template<>
    struct _cartesian<listify, 0>
    {
        template<class...>
        using f = list<>;
    };

    template<class C>
    struct _cartesian<C, 1>
    {
        template<class seq>
        using f = typename C::template f<
            typename _product1<seq>::type>;
    };

    template<>
    struct _cartesian<listify, 1>
    {
        template<class seq>
        using f = typename _product1<seq>::type;
    };

    template<class, class>
    struct _cartesian_impl;

    template<class... seqs, class seq>
    struct _cartesian_impl<list<seqs...>, seq>
    {
        using type = typename join<>
            ::f<typename _product<seqs, seq>::type...>;
    };

    template<class C, unsigned n>
    struct _cartesian
    {
        template<class seq, class... seqs>
        using f = typename fold_left<lift_t<detail::_cartesian_impl>, unpack<C>>
            ::template f<typename _product1<seq>::type, seqs...>;
    };

    template<unsigned n>
    struct _cartesian<listify, n>
    {
        template<class seq, class... seqs>
        using f = typename fold_left<lift_t<detail::_cartesian_impl>>
            ::template f<typename _product1<seq>::type, seqs...>;
    };
}
/// \endcond
namespace jln::mp
{
    /// \ingroup number

    template<class C = listify>
    struct numbers
    {
        template<int_... ns>
        using f = call<C, number<ns>...>;
    };

    /// \cond
    template<>
    struct numbers<listify>
    {
        template<int_... ns>
        using f = list<number<ns>...>;
    };
    /// \endcond

    namespace emp
    {
        template<int_... vs>
        using numbers = list<number<vs>...>;
    }
}
#if defined(__has_builtin)
#  if __has_builtin(__make_integer_seq)
#    define JLN_MP_USE_INTEGER_PACK 0
#    define JLN_MP_USE_MAKE_INTEGER_SEQ 1
#  elif __has_builtin(__integer_pack)
#    define JLN_MP_USE_INTEGER_PACK 1
#    define JLN_MP_USE_MAKE_INTEGER_SEQ 0
#  endif
#elif defined(__GNUC__)
#  if __GNUC__ >= 8
#    define JLN_MP_USE_INTEGER_PACK 1
#    define JLN_MP_USE_MAKE_INTEGER_SEQ 0
#  endif
#elif defined(_MSC_VER)
#  define JLN_MP_USE_INTEGER_PACK 0
#  define JLN_MP_USE_MAKE_INTEGER_SEQ 1
#endif

#ifndef JLN_MP_USE_INTEGER_PACK
#  define JLN_MP_USE_INTEGER_PACK 0
#  define JLN_MP_USE_MAKE_INTEGER_SEQ 0
#endif


namespace jln::mp
{
    /// \cond
    namespace detail
    {
#if JLN_MP_USE_MAKE_INTEGER_SEQ || JLN_MP_USE_INTEGER_PACK
        template<std::size_t>
#else
        template<class>
#endif
        struct _make_int_sequence;
    }
    /// \endcond


    /// \ingroup number

    /// Generates an incremental sequence of \c n \c int_.
    /// \treturn \sequence
    /// \see make_int_sequence, iota
    template<class C = numbers<>>
    struct make_int_sequence_v
    {
        template<class n>
#if JLN_MP_USE_MAKE_INTEGER_SEQ || JLN_MP_USE_INTEGER_PACK
        using f = typename detail::_make_int_sequence<n::value>
            ::type::template f<C>;
#else
        using f = typename detail::_make_int_sequence<
            std::make_index_sequence<n::value>>::template f<C>;
#endif
    };

    template<class C = mp::listify>
    using make_int_sequence = make_int_sequence_v<mp::numbers<C>>;

    namespace emp
    {
        template<class n, class C = mp::numbers<>>
        using make_int_sequence_v = typename mp::make_int_sequence_v<C>::template f<n>;

        template<int_ n, class C = mp::numbers<>>
        using make_int_sequence_v_c = make_int_sequence_v<mp::number<n>, C>;

        template<class n, class C = mp::listify>
        using make_int_sequence = typename mp::make_int_sequence<C>::template f<n>;

        template<int_ n, class C = mp::listify>
        using make_int_sequence_c = make_int_sequence<mp::number<n>, C>;
    }
} // namespace jln::mp

/// \cond
namespace jln::mp::detail
{
#if JLN_MP_USE_MAKE_INTEGER_SEQ
    template<class, int_... ns>
    struct _make_int_sequence_impl
    {
        template<class C>
        using f = typename C::template f<ns...>;
    };

    template<std::size_t n>
    struct _make_int_sequence
    {
        using type = __make_integer_seq<_make_int_sequence_impl, int_, n>;
    };
#elif JLN_MP_USE_INTEGER_PACK
    template<int_... ns>
    struct _make_int_sequence_impl
    {
        template<class C>
        using f = typename C::template f<ns...>;
    };

    template<std::size_t n>
    struct _make_int_sequence
    {
        using type = _make_int_sequence_impl<__integer_pack(n)...>;
    };
#else
    template<std::size_t... ns>
    struct _make_int_sequence<std::integer_sequence<std::size_t, ns...>>
    {
        template<class C>
        using f = typename C::template f<int_(ns)...>;
    };
#endif
}
/// \endcond

#undef JLN_MP_USE_INTEGER_PACK
#undef JLN_MP_USE_MAKE_INTEGER_SEQ

#if 0
namespace jln::mp::emp
{
    template<int_ n, class C = listify>
    using make_int_sequence_c = typename _make_int_sequence<
        (n == 0 ? 0 : n == 1 ? -1 : n - 1)
    >::template f<jln::mp::numbers<C>>;
}

namespace jln::mp::detail
{
    template<int_...>
    class enumeration;

    template<bool, int_ n, class ns>
    struct _make_int_sequence_impl;

    template<int_ n, int_... ns>
    struct _make_int_sequence_impl<false, n, enumeration<ns...>>
    {
        using type = enumeration<ns..., (n + ns)...>;

        template<class C>
        using f = typename C::template f<0, ns..., (n + ns)...>;
    };

    template<int_ n, int_... ns>
    struct _make_int_sequence_impl<true, n, enumeration<ns...>>
    {
        using type = enumeration<ns..., (n + ns)..., n * 2 + 1>;

        template<class C>
        using f = typename C::template f<0, ns..., (n + ns)..., n * 2 + 1>;
    };

    template<int_ n>
    struct _make_int_sequence
        : _make_int_sequence_impl<bool(n & 1), n / 2, typename _make_int_sequence<n / 2>::type>
    {};

    template<>
    struct _make_int_sequence<0>
    {
        using type = enumeration<>;

        template<class C>
        using f = typename C::template f<>;
    };

    template<>
    struct _make_int_sequence<-1>
    {
        using type = enumeration<>;

        template<class C>
        using f = typename C::template f<0>;
    };
}
#endif
namespace jln::mp
{
    /// \cond
    namespace detail
    {
        template<int_ n>
        struct _repeat;
    }
    /// \endcond

    /// \ingroup algorithm

    /// Returns a \sequence that contains a \number of copies of the same \sequence.
    /// \treturn \sequence
    template<class N, class C = listify>
    struct repeat
    {
        template<class... xs>
        using f = emp::make_int_sequence<N,
            typename detail::_repeat<detail::min(sizeof...(xs), 2)>
            ::template f<C, xs...>>;
    };

    template<int_ n, class C = listify>
    using repeat_c = repeat<number<n>, C>;

    namespace emp
    {
        template<class L, class n, class C = mp::listify>
        using repeat = unpack<L, mp::repeat<n, C>>;

        template<class L, int_ n, class C = mp::listify>
        using repeat_c = unpack<L, mp::repeat<number<n>, C>>;
    }
}


/// \cond
namespace jln::mp::detail
{
    template<>
    struct _repeat<0>
    {
        template<class C>
        using f = _cartesian<C, 0>; // for C::f<>
    };

    template<>
    struct _repeat<1>
    {
        template<class C, class x>
        using f = transform<always<x>, C>;
    };

    template<>
    struct _repeat<2>
    {
        template<class C, class... xs>
        using f = transform<always<list<xs...>>, join<C>>;
    };
}
/// \endcond
namespace jln::mp
{
    /// \ingroup algorithm

    /// Computes all possible combinations (with repetition) from the elements in a \sequence.
    /// \treturn \sequence of \list
    template<class C = listify>
    struct combine
    {
        template<class... xs>
        using f = typename repeat_c<sizeof...(xs), cartesian<C>>
            ::template f<list<xs...>>;
    };

    namespace emp
    {
        template<class L, class C = mp::listify>
        using combine = unpack<L, mp::combine<C>>;
    }
}
namespace jln::mp
{
    /// \cond
    namespace detail
    {
        template <class C, class... Fs>
        struct _each;
    }
    /// \endcond

    /// \ingroup functional

    /// Invokes multiple functions each taking the parameter corresponding to its position.
    /// \treturn \value
    /// \see tee, partial
#ifdef JLN_MP_DOXYGENATING
    template <class... Fs, class C>
    struct each
    {
        template<class... xs>
        using f = C::f<Fs::f<xs>...>;
    };
#else
    template <class... Fs>
    struct each
        : rotate<number<-1>, lift<detail::_each>>
        ::template f<Fs...>
    {};
#endif

    /// \cond
    template <class C>
    struct each<C> : detail::_each<C>
    {};

    template <class F, class C>
    struct each<F, C> : detail::_each<C, F>
    {};

    template <class F0, class F1, class C>
    struct each<F0, F1, C> : detail::_each<C, F0, F1>
    {};

    template <class F0, class F1, class F2, class C>
    struct each<F0, F1, F2, C> : detail::_each<C, F0, F1, F2>
    {};
    /// \endcond
}

/// \cond
namespace jln::mp::detail
{
    template <class C, class... Fs>
    struct _each
    {
        template <class... xs>
        using f = call<C, call<Fs, xs>...>;
    };
} // namespace jln::mp
/// \endcond
namespace jln::mp
{
    /// \ingroup algorithm

    /// comparison on the result of a function.
    /// \treturn \bool
    template<class F, class Cmp = less<>>
    using compare_with = each<F, F, Cmp>;

    namespace emp
    {
        template<class F, class x, class y>
        using compare_with = less<call<F, x>, call<F, y>>;
    }
}
namespace jln::mp
{
    /// \ingroup algorithm

    /// Checks whether a \value is contained in a \list.
    /// \treturn \bool
    template<class x, class C = identity>
    using contains = any_of<is<x>, C>;

    namespace emp
    {
        template<class L, class x, class C = mp::identity>
        using contains = unpack<L, mp::contains<x, C>>;
    }
}
namespace jln::mp
{
    /// \ingroup algorithm

    /// Removes all elements that satisfy a predicate.
    /// \treturn \sequence
    template<class Pred, class C = listify>
    using remove_if = transform<wrap_in_list_if_not<Pred>, join<C>>;

    /// Removes all occurence of a \value.
    /// \treturn \sequence
    template<class T, class C = listify>
    using remove = remove_if<is<T>, C>;

    namespace emp
    {
        template<class L, class Pred, class C = mp::listify>
        using remove_if = unpack<L, mp::remove_if<Pred, C>>;

        template<class L, class T, class C = mp::listify>
        using remove = unpack<L, mp::remove<T, C>>;
    }
}
namespace jln::mp
{
    /// \ingroup algorithm

    /// Copies all elements that satisfy a predicate.
    /// \treturn \sequence
    template<class Pred, class C = listify>
    using copy_if = remove_if<tee<Pred, not_<>>, C>;

    /// Copies all occurence of a \value.
    /// \treturn \sequence
    template<class x, class C = listify>
    using copy = remove_if<is<x, not_<>>, C>;

    namespace emp
    {
        template<class L, class Pred, class C = mp::listify>
        using copy_if = unpack<L, mp::copy_if<Pred, C>>;

        template<class L, class x, class C = mp::listify>
        using copy = unpack<L, mp::copy<x, C>>;
    }
}
namespace jln::mp
{
    /// \ingroup algorithm

    /// Counts all elements that satisfy a predicate.
    template<class Pred, class C = identity>
    using count_if = transform<Pred, add0<C>>;

    /// Counts all elements identical to a \value.
    template<class x, class C = identity>
    using count = transform<is<x>, add0<C>>;

    namespace emp
    {
        template<class L, class Pred, class C = mp::identity>
        using count_if = unpack<L, mp::count_if<Pred, C>>;

        template<class L, class x, class C = mp::identity>
        using count = unpack<L, mp::count<x, C>>;
    }
}
namespace jln::mp
{
    /// \ingroup list

    /// Remove the first element of sequence
    /// \pre `sizeof...(xs) > 0`
    /// \treturn \sequence
    template<class C = listify>
    using pop_front = drop_front<number<1>, C>;

    namespace emp
    {
        template<class L, class C = mp::listify>
        using pop_front = drop_front<L, mp::number<1>, C>;
    }
}
namespace jln::mp
{
    /// \cond
    namespace detail
    {
        template<class>
        struct _recursively;

        template<class>
        struct _recursively_as_much_as_possible;

        template<class x>
        struct _recursively_next;

        template<class x>
        struct _recursively_as_much_as_possible_next;

        template<class...>
        struct _recursion_result;

        template<class...>
        struct _next_recursion;
    }
    /// \endcond

    /// Stop the recursion, the input values will be used as result.
    struct stop_recursion
    {
        template<class...>
        using f = stop_recursion;
    };

    /// Specify result values and stop recursion.
    struct recursion_result
    {
        template<class... xs>
        using f = detail::_recursion_result<xs...>;
    };

    /// Specify values for the next iteration.
    struct next_recursion
    {
        template<class... xs>
        using f = detail::_next_recursion<xs...>;
    };

    /// \ingroup functional

    /// Recursively invokes \c F until \c stop_recursion or \c recursion_result.
    /// \treturn \value
    /// \see recursively_xs, recursively_as_much_as_possible
    /// \see stop_recursion, recursion_result, next_recursion
    template<class F, class C = identity>
    struct recursively
    {
        template<class... xs>
        using f = typename detail::_recursively<
            typename detail::_recursively_next<call<F, xs...>>
            ::template g<xs...>
            ::template f<F>
            ::template f<F>
            ::template f<F>
        >::template f<F, C>;
    };

    /// Same than \c recursively, but with listify as continuation.
    /// \treturn \sequence
    /// \see recursively
    template<class F, class C = listify>
    using recursively_xs = recursively<F, C>;

    /// Recursively invokes \c F until the result is \c stop_recursion, \c recursion_result or no longer changes.
    /// \treturn \value
    /// \see recursively_as_much_as_possible_xs
    /// \see recursively
    /// \see stop_recursion, recursion_result, next_recursion
    template<class F, class C = identity>
    struct recursively_as_much_as_possible
    {
        template<class... xs>
        using f = typename detail::_recursively_as_much_as_possible<
            typename detail::_recursively_as_much_as_possible_next<call<F, xs...>>
            ::template g<xs...>
            ::template f<F>
            ::template f<F>
            ::template f<F>
        >::template f<F, C>;
    };

    /// Same than \c recursively_as_much_as_possible, but with listify as continuation.
    /// \treturn \value
    /// \see recursively_as_much_as_possible
    template<class F, class C = listify>
    using recursively_as_much_as_possible_xs = recursively_as_much_as_possible<F, C>;

    namespace emp
    {
        template<class L, class F, class C = mp::identity>
        using recursively = unpack<L, mp::recursively<F, C>>;

        template<class L, class F, class C = mp::identity>
        using recursively_as_much_as_possible = unpack<L, mp::recursively_as_much_as_possible<F, C>>;

        template<class L, class F, class C = mp::listify>
        using recursively_xs = unpack<L, mp::recursively<F, C>>;

        template<class L, class F, class C = mp::listify>
        using recursively_as_much_as_possible_xs = unpack<L, mp::recursively_as_much_as_possible<F, C>>;
    }
}


/// \cond
namespace jln::mp::detail
{
    template<class x>
    struct _recursively_next
    {
        template<class>
        using f = _recursively_next;

        template<class...>
        using g = _recursively_next<_next_recursion<x>>;
    };

    template<>
    struct _recursively_next<stop_recursion>
    {
        template<class>
        using f = _recursively_next;

        template<class... xs>
        using g = _recursively_next<_recursion_result<xs...>>;
    };

    template<class... xs>
    struct _recursively_next<_recursion_result<xs...>>
    {
        template<class>
        using f = _recursively_next;

        template<class...>
        using g = _recursively_next;
    };

    template<class... xs>
    struct _recursively_next<_next_recursion<xs...>>
    {
        template<class F>
        using f = typename _recursively_next<typename F::template f<xs...>>
            ::template g<xs...>;

        template<class...>
        using g = _recursively_next;
    };

    template<class... xs>
    struct _recursively<_recursively_next<_next_recursion<xs...>>>
    {
        template<class F, class C>
        using f = typename _recursively<
            typename _recursively_next<typename F::template f<xs...>>
            ::template g<xs...>
            ::template f<F>
            ::template f<F>
            ::template f<F>
            ::template f<F>
            ::template f<F>
            ::template f<F>
            ::template f<F>
        >::template f<F, C>;
    };

    template<class... xs>
    struct _recursively<_recursively_next<_recursion_result<xs...>>>
    {
        template<class F, class C>
        using f = typename C::template f<xs...>;
    };

    template<bool>
    struct recursively_next_step;

    template<>
    struct recursively_next_step<true>
    {
        template<class x>
        using f = _recursively_as_much_as_possible_next<_recursion_result<x>>;
    };

    template<>
    struct recursively_next_step<false>
    {
        template<class x>
        using f = _recursively_as_much_as_possible_next<_next_recursion<x>>;
    };

    template<class x>
    struct _recursively_as_much_as_possible_next
    {
        template<class>
        using f = _recursively_as_much_as_possible_next;

        template<class... xs>
        using g = typename recursively_next_step<std::is_same<list<x>, list<xs...>>::value>
            ::template f<x>;
    };

    template<>
    struct _recursively_as_much_as_possible_next<stop_recursion>
    {
        template<class>
        using f = _recursively_as_much_as_possible_next;

        template<class... xs>
        using g = _recursively_as_much_as_possible_next<_recursion_result<xs...>>;
    };

    template<class... xs>
    struct _recursively_as_much_as_possible_next<_recursion_result<xs...>>
    {
        template<class>
        using f = _recursively_as_much_as_possible_next;

        template<class...>
        using g = _recursively_as_much_as_possible_next;
    };

    template<class... xs>
    struct _recursively_as_much_as_possible_next<_next_recursion<xs...>>
    {
        template<class F>
        using f = typename _recursively_as_much_as_possible_next<typename F::template f<xs...>>
            ::template g<xs...>;

        template<class...>
        using g = _recursively_as_much_as_possible_next;
    };

    template<class... xs>
    struct _recursively_as_much_as_possible<_recursively_as_much_as_possible_next<_next_recursion<xs...>>>
    {
        template<class F, class C>
        using f = typename _recursively_as_much_as_possible<
            typename _recursively_as_much_as_possible_next<typename F::template f<xs...>>
            ::template g<xs...>
            ::template f<F>
            ::template f<F>
            ::template f<F>
            ::template f<F>
            ::template f<F>
            ::template f<F>
            ::template f<F>
        >::template f<F, C>;
    };

    template<class... xs>
    struct _recursively_as_much_as_possible<_recursively_as_much_as_possible_next<_recursion_result<xs...>>>
    {
        template<class F, class C>
        using f = typename C::template f<xs...>;
    };
}
/// \endcond
namespace jln::mp
{
    /// \ingroup search

    /// Finds the last element that satisfy a \predicate.
    /// Calls \c TC with all the elements since the last found at the end.
    /// If no element is found, \c FC is used with the whole \sequence.
    /// \treturn \sequence
    /// \see find_last_if_not, find_last, find
    template<class Pred, class TC = listify, class FC = clear<TC>>
    using find_last_if = find_if<Pred, recursively<
        pop_front<find_if<Pred, next_recursion, stop_recursion>>,
        TC
    >, FC>;

    /// \see find_last_if, find_last, find
    template<class Pred, class TC = listify, class FC = clear<TC>>
    using find_last_if_not = find_last_if<not_fn<Pred>, TC, FC>;

    /// \see find_last_if, find_last_if_not, find_last, find
    template<class T, class TC = listify, class FC = clear<TC>>
    using find_last = find_last_if<is<T>, TC, FC>;

    namespace emp
    {
        template<class L, class Pred, class TC = mp::listify, class FC = clear<TC>>
        using find_last_if = unpack<L, mp::find_last_if<Pred, TC, FC>>;

        template<class L, class Pred, class TC = mp::listify, class FC = clear<TC>>
        using find_last_if_not = unpack<L, mp::find_last_if_not<Pred, TC, FC>>;

        template<class L, class T, class TC = mp::listify, class FC = clear<TC>>
        using find_last = unpack<L, mp::find_last_if<mp::is<T>, TC, FC>>;
    }
}
namespace jln::mp
{
    /// \cond
    namespace detail
    {
        template<template<class...> class, class>
        struct _flatten;

        template<class S>
        struct wrapper;
    }
    /// \endcond

    /// \ingroup algorithm

    /// Converts a tree or list of lists into one list containing the contents of all children.
    /// \treturn \sequence
    template<class S = lift<list>, class C = listify>
    struct flatten
    {};

    template<template<class...> class S, class C>
    struct flatten<lift<S, identity>, C>
    {
        template<class... seqs>
        using f = typename detail::_join_select<sizeof...(seqs)>
            ::template f<C, typename detail::_flatten<S, seqs>::type...>
            ::type;
    };

    /// converts a \typelist to a \c lift<S>
    template<class L>
    using wrapper = typename detail::wrapper<L>::type;

    namespace emp
    {
        template<class L, class S = wrapper<L>, class C = mp::listify>
        using flatten = unpack<L, mp::flatten<S, C>>;

        template<class L, class... xs>
        using rewrap = typename wrapper<L>::template f<xs...>;
    }
}

/// \cond
namespace jln::mp::detail
{
    template<template<class...> class, class T>
    struct _flatten
    {
        using type = list<T>;
    };

    template<template<class...> class S, class... xs>
    struct _flatten<S, S<xs...>>
        : _join_select<sizeof...(xs)>
        ::template f<listify, typename _flatten<S, xs>::type...>
    {};

    template<template<class...> class S, class... xs>
    struct wrapper<S<xs...>>
    {
        using type = lift<S>;
    };
}
/// \endcond
namespace jln::mp
{
    /// \cond
    namespace detail
    {
        template<int>
        struct _fold_right;
    }
    /// \endcond

    /// \ingroup algorithm

    /// Folds right over a list using a binary predicate.
    /// fold_right consideres the first element in the input pack as the state,
    /// use \c push_front<> to add state if needed.
    /// \semantics
    ///   Equivalent to
    ///   \code
    ///   F::f<x[0], ..., F::f<x[n-2], F::f<xs[n-1], state>>>
    ///   \endcode
    /// \treturn \value
    /// \see fold_left, fold_tree, reverse_fold, fold_balanced_tree
    template<class F, class C = identity>
    struct fold_right
    {
        template<class... xs>
        using f = typename C::template f<
            typename detail::_fold_right<
            detail::sub_1_n_4_or_less_8_16_64_256(sizeof...(xs))
      >::template f<F::template f, xs...>
        >;
    };

    namespace emp
    {
        template<class L, class state, class F, class C = mp::identity>
        using fold_right = unpack<L,
            mp::push_front<state, mp::fold_right<F, C>>>;
    }
}


/// \cond
namespace jln::mp::detail
{
#define JLN_MP_FOLD_RIGHT_SELECT(n, mp_xs, mp_rxs, mp_dup) \
  template<>                                               \
  struct _fold_right<n>                                    \
  {                                                        \
    template<template<class...> class F, class state,      \
      mp_xs(class, JLN_MP_NIL, JLN_MP_COMMA),              \
      class... xs>                                         \
    using f = mp_xs(F<, JLN_MP_COMMA, JLN_MP_NIL)          \
      typename _fold_right<                                \
        detail::n_4_or_less_8_16_64_256(sizeof...(xs))     \
      >::template f<F, state, xs...>                       \
    mp_dup(>, JLN_MP_NIL);                                 \
  };

    JLN_MP_GEN_XS_4_8_16_64_256(JLN_MP_FOLD_RIGHT_SELECT)

#undef JLN_MP_FOLD_RIGHT_SELECT

#define JLN_MP_FOLD_RIGHT_SELECT(n, mp_xs, mp_rxs, mp_dup) \
  template<>                                               \
  struct _fold_right<n>                                    \
  {                                                        \
    template<template<class...> class F, class state,      \
      mp_xs(class, JLN_MP_NIL, JLN_MP_COMMA)>              \
    using f = mp_xs(F<, JLN_MP_COMMA, JLN_MP_NIL)          \
      state mp_dup(>, JLN_MP_NIL);                         \
  };

        JLN_MP_GEN_XS_1_TO_4(JLN_MP_FOLD_RIGHT_SELECT)

#undef JLN_MP_FOLD_RIGHT_SELECT

        template<>
    struct _fold_right<0>
    {
        template<template<class...> class, class state>
        using f = state;
    };

    template<>
    struct _fold_right<-1>
    {};
}
/// \endcond
namespace jln::mp
{
    /// \cond
    namespace detail
    {
        template<uint_>
        struct _fold_tree;

        template<uint_>
        struct _fold_balanced_tree;
    }
    /// \endcond

    /// \ingroup algorithm

    /// Folds tree over a list using a binary predicate.
    /// recursively fold n/2 value to the left and the rest to the right.
    /// \semantics
    ///   Equivalent to
    ///   \code
    ///   F::f<fold_tree::f<...xs[0..n/2]>, fold_tree::f<...xs[n/2..n]>>
    ///   \endcode
    /// \treturn \value
    /// \see fold_left, fold_right, fold_balanced_tree
    template<class F, class C = identity>
    struct fold_tree
    {
        template<class... xs>
        using f = typename detail::_fold_tree<
            detail::min(3, sizeof...(xs))
        >::template f<C, F, xs...>;
    };

    /// Folds tree over a list using a binary predicate.
    /// recursively fold (n+1)/2 value to the left and the rest to the right.
    /// \semantics
    ///   Equivalent to
    ///   \code
    ///   F::f<fold_tree::f<...xs[0..(n+1)/2]>, fold_tree::f<...xs[n-(n+1)/2..n]>>
    ///   \endcode
    /// \treturn \value
    /// \see fold_left, fold_right, fold_tree
    template<class F, class C = identity>
    struct fold_balanced_tree
    {
        template<class... xs>
        using f = typename detail::_fold_balanced_tree<
            detail::min(3, sizeof...(xs))
        >::template f<C, F, xs...>;
    };

    namespace emp
    {
        template<class L, class F, class C = mp::identity>
        using fold_tree = unpack<L, mp::fold_tree<F, C>>;

        template<class L, class F, class C = mp::identity>
        using fold_balanced_tree = unpack<L, mp::fold_balanced_tree<F, C>>;
    }
}


/// \cond
namespace jln::mp::detail
{
    // std::bit_ceil (c++20)
    constexpr uint_ bit_ceil(uint_ n)
    {
        n--;
        n |= n >> 1;
        n |= n >> 2;
        n |= n >> 4;
        n |= n >> 8;
        n |= n >> 16;
        n++;
        return n;
    }

    template<>
    struct _fold_tree<0>
    {
        template<class C, class F>
        using f = typename C::template f<>;
    };

    template<>
    struct _fold_tree<1>
    {
        template<class C, class F, class x>
        using f = typename C::template f<x>;
    };

    template<>
    struct _fold_tree<2>
    {
        template<class C, class F, class x, class y>
        using f = typename C::template f<typename F::template f<x, y>>;
    };

    template<class F, uint_>
    struct fold_tree_impl;

    template<class F>
    struct fold_tree_impl<F, 0>
    {
        template<class x>
        using f = x;
    };

    template<class F>
    struct fold_tree_impl<F, 1>
    {
        template<class x, class y>
        using f = typename F::template f<x, y>;
    };

    template<class F, uint_ n>
    struct fold_tree_impl
    {
        template<class... xs>
        using f = typename F::template f<
            typename take_front_c<n, fold_tree_impl<F, n / 2>>::template f<xs...>,
            typename drop_front_c<n, fold_tree_impl<F, bit_ceil(sizeof...(xs) - n) / 2>>::template f<xs...>
        >;
    };

    template<>
    struct _fold_tree<3>
    {
        template<class C, class F, class... xs>
        using f = typename C::template f<
            typename fold_tree_impl<F, bit_ceil(sizeof...(xs)) / 2>::template f<xs...>
        >;
    };


    template<>
    struct _fold_balanced_tree<0>
        : _fold_tree<0>
    {};

    template<>
    struct _fold_balanced_tree<1>
        : _fold_tree<1>
    {};

    template<>
    struct _fold_balanced_tree<2>
        : _fold_tree<2>
    {};

    template<class F, uint_>
    struct fold_balanced_tree_impl;

    template<class F>
    struct fold_balanced_tree_impl<F, 1>
    {
        template<class x>
        using f = x;
    };

    template<class F>
    struct fold_balanced_tree_impl<F, 2>
    {
        template<class x, class y>
        using f = typename F::template f<x, y>;
    };

    template<class F, uint_ n>
    struct fold_balanced_tree_impl
    {
        template<class... xs>
        using f = typename F::template f<
            typename take_front_c<(n + 1) / 2, fold_balanced_tree_impl<F, (n + 1) / 2>>::template f<xs...>,
            typename drop_front_c<(n + 1) / 2, fold_balanced_tree_impl<F, n - (n + 1) / 2>>::template f<xs...>
        >;
    };

    template<>
    struct _fold_balanced_tree<3>
    {
        template<class C, class F, class... xs>
        using f = typename C::template f<
            typename fold_balanced_tree_impl<F, sizeof...(xs)>::template f<xs...>
        >;
    };
}
/// \endcond
namespace jln::mp
{
    struct na {};

    using is_na = is<na>;
    using violation = always<na>;

    /// \cond
    namespace detail
    {
        template<class, class, class = void>
        struct _try_impl;

        template<class x>
        struct _try_dispatch;
    }
    /// \endcond

    /// \ingroup functional

    /// Invokes \c TC::f<result> whetehr \c F::f<xs...> is a valid expression
    /// other than \c na, otherwhise invokes \c FC::f<xs...>.
    /// \pre \c F::f<xs...> must be a SFINAE compatible expression
    /// \treturn \value
    template<class F, class TC = identity, class FC = violation>
    struct try_;

    template<class F, class FC>
    using try_or = try_<F, identity, FC>;

    template<class F, class TC, class FC>
    struct try_
    {
        template<class... xs>
        using f = typename detail::_try_dispatch<
            typename detail::_try_impl<F, list<xs...>>::type
        >::template f<TC, FC, xs...>;
    };

    namespace emp
    {
        template<class F, class TC, class FC, class... xs>
        using try_ = typename try_<F, TC, FC>::template f<xs...>;

        template<class F, class FC, class... xs>
        using try_or = typename try_<F, mp::identity, FC>::template f<xs...>;
    }
}


namespace jln::mp
{
    /// \cond
    template<class F>
    struct try_<F, always<true_>, always<false_>>
    {
        template<class... xs>
        using f = number<!std::is_same<na,
            typename detail::_try_impl<F, list<xs...>>::type
        >::value>;
    };

    template<class F>
    struct try_<F, identity, violation>
    {
        template<class... xs>
        using f = typename detail::_try_impl<F, list<xs...>>::type;
    };
    /// \endcond
}

/// \cond
namespace jln::mp::detail
{
    template<class, class, class>
    struct _try_impl
    {
        using type = na;
    };

    template<class F, class... xs>
    struct _try_impl<F, list<xs...>, std::void_t<typename F::template f<xs...>>>
    {
        using type = typename F::template f<xs...>;
    };

    template<class x>
    struct _try_dispatch
    {
        template<class TC, class FC, class...>
        using f = typename TC::template f<x>;
    };

    template<>
    struct _try_dispatch<na>
    {
        template<class TC, class FC, class... xs>
        using f = typename FC::template f<xs...>;
    };
}
/// \endcond
namespace jln::mp
{
    /// \cond
    namespace detail
    {
        template<bool>
        struct _group;
    }
    /// \endcond

    /// \ingroup group

    /// Groups adjacent elements that respect a predicate.
    /// \semantics
    ///   \code
    ///   call<group<same<>>,
    ///     void, void, int, void
    ///   > = list<
    ///     list<void, void>,
    ///     list<int>,
    ///     list<void>
    ///   >
    ///   \endcode
    /// \treturn \sequence
    template<class Cmp, class C = listify>
    struct group_by
    {
        template<class... xs>
        using f = typename detail::_group<sizeof...(xs) != 0>
            ::template f<C, Cmp, xs...>;
    };

    template<class C = listify>
    using group = group_by<same<>, C>;

    namespace emp
    {
        template<class L, class Cmp, class C = mp::listify>
        using group_by = unpack<L, mp::group_by<Cmp, C>>;

        template<class L, class C = mp::listify>
        using group = unpack<L, mp::group<C>>;
    }
}


namespace jln::mp
{
    /// \cond
    namespace detail
    {
        template <bool>
        struct _split;

        inline constexpr int_ split_keep = 0;
        inline constexpr int_ split_after = 1;
        inline constexpr int_ split_before = 2;
        inline constexpr int_ split_skip = 3;
        // inline constexpr int_ split_na = 4;
    }
    /// \endcond

    /// \ingroup group

    /// Splits a \sequence into multiple \lists at every point that satisfy a predicate.
    /// \semantics
    ///   \code
    ///   call<split_if<is<void>, _0, _1, _2, _0, _3> == list<
    ///     list<>,
    ///     list<_1, _2>,
    ///     list<_3>
    ///   >
    ///   \endcode
    /// \treturn \sequence of \list
    /// \see split_before_if, split_after_if
    template<class Pred = identity, class C = listify>
    struct split_if
    {
        template <class... xs>
        using f = typename detail::_split<sizeof...(xs) != 0>
            ::template f<detail::split_skip, C, Pred, xs...>;
    };

    template<class x, class C = listify>
    using split = split_if<is<x>, C>;

    namespace emp
    {
        template<class L, class Pred = mp::identity, class C = mp::listify>
        using split_if = unpack<L, mp::split_if<Pred, C>>;

        template<class L, class x, class C = mp::listify>
        using split = unpack<L, mp::split<x, C>>;
    }
}

/// \cond
namespace jln::mp::detail
{
    template <class x, class state>
    struct split_state;

    template<class x, class... Ls, class... xs>
    struct split_state<list<number<split_keep>, x>, list<list<xs...>, Ls...>>
    {
        using type = list<list<x, xs...>, Ls...>;
    };

    template<class x, class... Ls, class... xs>
    struct split_state<list<number<split_after>, x>, list<list<xs...>, Ls...>>
    {
        using type = list<list<x>, list<xs...>, Ls...>;
    };

    template<class x, class... Ls, class... xs>
    struct split_state<list<number<split_before>, x>, list<list<xs...>, Ls...>>
    {
        using type = list<list<>, list<x, xs...>, Ls...>;
    };

    template<class x, class... Ls, class... xs>
    struct split_state<list<number<split_skip>, x>, list<list<xs...>, Ls...>>
    {
        using type = list<list<>, list<xs...>, Ls...>;
    };

    template<>
    struct _split<true>
    {
        template<int_ policy, class C, class Pred, class... xs>
        using f = typename fold_right<lift_t<split_state>, optimize_useless_unpack_t<unpack<C>>>
            ::template f < list<list<>>,
            list < number<bool{ Pred::template f<xs>::value }
            ? policy : split_keep > , xs > ...
            > ;
    };

    template<>
    struct _split<false>
    {
        template<int_, class C, class>
        using f = typename C::template f<>;
    };
}
/// \endcond
namespace jln::mp
{
    /// \cond
    namespace detail
    {
        template <class, class...>
        struct _partial;
    }
    /// \endcond

    /// \ingroup functional

    /// Invokes multiple functions each taking the parameter corresponding to its position
    /// then calls \c C with the results and the rest of the parameters.
    /// \pre `sizeof...(xs) >= sizeof...(Fs)`
    /// \semantics
    ///   \code
    ///   partial<F,G,C>::f<a,b,c,d> == C::f<F::f<a>, G::f<b>, c, d>
    ///   \endcode
    /// \treturn \value
    /// \see each, tee, on
#ifdef JLN_MP_DOXYGENATING
    template <class... Fs, class C>
    struct partial
    {
        template<class... xs>
        using f = /* unspecified */;
    };
#else
    template <class... Fs>
    struct partial
        : rotate<number<-1>, lift<detail::_partial>>
        ::template f<Fs...>
    {};
#endif

    /// \cond
    template <class C>
    struct partial<C>
    {
        template <class... xs>
        using f = call<C, xs...>;
    };

    template <class F, class C>
    struct partial<F, C>
    {
        template <class x, class... xs>
        using f = call<C, call<F, x>, xs...>;
    };

    template <class F0, class F1, class C>
    struct partial<F0, F1, C>
    {
        template <class x0, class x1, class... xs>
        using f = call<C, call<F0, x0>, call<F1, x1>, xs...>;
    };

    template <class F0, class F1, class F2, class C>
    struct partial<F0, F1, F2, C>
    {
        template <class x0, class x1, class x2, class... xs>
        using f = call<C, call<F0, x0>, call<F1, x1>, call<F2, x2>, xs...>;
    };
    /// \endcond
}

/// \cond
namespace jln::mp::detail
{
    template <class C, class... Fs>
    struct _partial
    {
        template<class... xs>
        using f = typename _join_select<2>::f<
            C,
            typename take_front_c<sizeof...(Fs), _each<listify, Fs...>>::template f<xs...>,
            typename drop_front_c<sizeof...(Fs)>::template f<xs...>
        >::type;
    };
} // namespace jln::mp
/// \endcond
/// \cond
namespace jln::mp::detail
{
    template<class x, class C>
    using _group_insert_x = partial<unpack<push_front<x>>, C>;

    template<class...>
    struct _group_impl;

    template<class C, class Cmp, class x, class... xs, class... ys>
    struct _group_impl<C, Cmp, x, list<ys...>, xs...>
    {
        using type = typename fold_right<lift_t<split_state>, unpack<_group_insert_x<x, C>>>
            ::template f<list<list<>>,
            list<number<Cmp::template f<ys, xs>::value
            ? split_keep : split_before>, xs>...
            >;
    };

    template<class... ys>
    struct _smp_group_impl
    {
        template<class C, class Cmp, class x, class... xs>
        using f = typename fold_right<lift_t<split_state>, unpack<_group_insert_x<x, C>>>
            ::template f<list<list<>>,
            list<number<Cmp::template f<ys, xs>::value
            ? split_keep : split_before>, xs>...
            >;
    };

    template<class C, class Cmp, class TC, class FC, class x, class... xs, class... ys>
    struct _group_impl<C, try_<Cmp, TC, FC>, x, list<ys...>, xs...>
    {
        using type = typename try_<_smp_group_impl<ys...>>
            ::template f<C, try_<Cmp, TC, FC>, x, xs...>;
    };

    template<>
    struct _group<true>
    {
        template<class C, class Cmp, class x, class... xs>
        using f = typename _group_impl<
            C, Cmp, x,
            typename take_front_c<sizeof...(xs)>::template f<x, xs...>,
            xs...
        >::type;
    };

    template<>
    struct _group<false>
    {
        template<class C, class>
        using f = typename C::template f<>;
    };
}
/// \endcond
namespace jln::mp
{
    /// \cond
    namespace detail
    {
        template<bool>
        struct _group_n;
    }
    /// \endcond

    /// \ingroup group

    /// Splits a sequence by arbitrary size group.
    /// \post If `n <= 0`, then the result sequence is empty
    /// \semantics
    ///   \code
    ///   call<group_n<number<2>>,
    ///     void, void, int, void, void
    ///   > = list<
    ///     list<void, void>,
    ///     list<int, void>,
    ///     list<void>
    ///   >
    ///   \endcode
    /// \treturn \sequence
    template<class n, class C = listify>
    struct group_n
    {
        template<class... xs>
        using f = typename detail::_group_n<sizeof...(xs) != 0 && (n::value > 0)>
            ::template f<C, n::value, xs...>;
    };

    template<int_ n, class C = listify>
    using group_n_c = group_n<number<n>, C>;

    namespace emp
    {
        template<class L, class n, class C = mp::listify>
        using group_n = unpack<L, mp::group_n<n, C>>;

        template<class L, int_ n, class C = mp::listify>
        using group_n_c = unpack<L, mp::group_n_c<n, C>>;
    }
}


/// \cond
namespace jln::mp::detail
{
    template<int_... i>
    struct _group_n_impl
    {
        template<class C, unsigned long long n, class... xs>
        using f = typename fold_right<lift_t<split_state>, unpack<pop_front<C>>>
            ::template f<
            list<list<>>,
            list<number<(i% n ? split_keep : split_before)>, xs>...
            >;
    };

    template<>
    struct _group_n<true>
    {
        template<class C, unsigned long long n, class... xs>
        using f = typename emp::make_int_sequence_v_c<
            sizeof...(xs), lift_c<_group_n_impl>
        >::template f<C, n, xs...>;
    };

    template<>
    struct _group_n<false>
    {
        template<class C, unsigned long long, class...>
        using f = typename C::template f<>;
    };
}
/// \endcond
namespace jln::mp
{
    /// \ingroup number

    /// Replaces each element of a \sequence by its corresponding index.
    /// \treturn \sequence
    template<class C = listify>
    using indices = size<make_int_sequence<C>>;

    namespace emp
    {
        template<class L, class C = mp::listify>
        using indices = unpack<L, mp::indices<C>>;
    }
} // namespace jln::mp
namespace jln::mp
{
    /// \cond
    namespace detail
    {
        template<int>
        struct _intersperse;
    }
    /// \endcond

    /// \ingroup algorithm

    /// Inserts a \value between each element of a \sequence.
    /// \treturn \list
    /// \see prefix, suffix
    template<class x, class C = listify>
    struct intersperse
    {
        template<class... xs>
        using f = typename detail::_intersperse<sizeof...(xs) ? 1 : 0>
            ::template f<C, x, xs...>;
    };

    namespace emp
    {
        template<class L, class x, class C = mp::listify>
        using intersperse = unpack<L, mp::intersperse<x, C>>;
    }
}


/// \cond
namespace jln::mp::detail
{
    template<>
    struct _intersperse<0>
    {
        template<class C, class x>
        using f = typename C::template f<>;
    };

    template<>
    struct _intersperse<1>
    {
        template<class C, class x, class... xs>
        using f = typename detail::_join_select<sizeof...(xs)>
            ::template f<pop_front<C>, list<x, xs>...>::type;
    };
}
/// \endcond
namespace jln::mp
{
    /// \cond
    namespace detail
    {
        template<int_ start, int_ stride, bool inc>
        struct _iota_v_c;
    }
    /// \endcond

    /// \ingroup number

    /// Generates a sequence of \int_.
    /// \treturn \sequence of \int_
    /// \see iota
    template<class C = numbers<>>
    struct iota_v
    {
        template<class start, class count, class stride = number<1>>
        using f = typename detail::_iota_v_c<start::value, stride::value, (count::value >= 0)>
            ::template f<C, count::value>;
    };

    /// \ingroup number

    /// Generates a sequence of \number.
    /// \treturn \sequence of number
    template<class C = listify>
    using iota = iota_v<numbers<C>>;

    namespace emp
    {
        template<int_ start, int_ count, int_ stride = 1, class C = mp::numbers<>>
        using iota_v_c = typename detail::_iota_v_c<start, stride, (count >= 0)>
            ::template f<C, count>;

        template<class start, class count, class stride = number<1>, class C = mp::numbers<>>
        using iota_v = iota_v_c<start::value, count::value, stride::value, C>;

        template<int_ start, int_ count, int_ stride = 1, class C = mp::listify>
        using iota_c = iota_v_c<start, count, stride, mp::numbers<C>>;

        template<class start, class count, class stride = number<1>, class C = mp::listify>
        using iota = iota_v_c<start::value, count::value, stride::value, mp::numbers<C>>;
    }
}

/// \cond
namespace jln::mp::detail
{
    template<int_ start, int_ stride, class C>
    struct _iota
    {
        template<int_... ns>
        using f = typename C::template f<(start + ns * stride)...>;
    };

    template<int_ start, int_ stride>
    struct _iota<start, stride, numbers<>>
    {
        template<int_... ns>
        using f = list<number<(start + ns * stride)>...>;
    };

    template<>
    struct _iota_v_c<0, 1, true>
    {
        template<class C, int_ count>
        using f = emp::make_int_sequence_v_c<count, C>;
    };

    template<int_ start, int_ stride, bool>
    struct _iota_v_c
    {
        template<class C, int_ count>
        using f = emp::make_int_sequence_v_c<
            (count < 0 ? -count : count),
            detail::_iota<start, (count < 0 ? -stride : stride), C>>;
    };
}
/// \endcond
namespace jln::mp
{
    /// \cond
    namespace detail
    {
        template<uint_>
        struct _is_disjoint;
    }
    /// \endcond

    /// \ingroup algorithm

    /// Checks whether \value in `seqs[0]` are disjoint from the \value in `seqs[1:]`.
    /// Returns `mp::true_` when `sizeof...(seqs) < 2`
    /// \treturn \number
    /// \see is_disjoint
    template<class Equal = lift<std::is_same>, class C = identity>
    struct is_disjoint_with
    {
        template<class... seqs>
        using f = typename C::template f<number<
            detail::_is_disjoint<detail::min(3, sizeof...(seqs))>
            ::template f<detail::to_not_fn_t<Equal>, seqs...>
            ::value
            >>;
    };

    /// Checks whether \value in `seqs[0]` are disjoint from the \value in `seqs[1:]`.
    /// Returns `mp::true_` when `sizeof...(seqs) < 2`
    /// \treturn \number
    /// \see is_disjoint_with
    template<class C = identity>
    using is_disjoint = is_disjoint_with<lift<std::is_same>, C>;

    namespace emp
    {
#ifdef JLN_MP_DOXYGENATING
        template<class L1, class L2, class C = mp::identity>
        using is_disjoint = typename is_disjoint<C>::template f<L1, L2>;

        template<class L1, class L2, class Equal = lift<std::is_same>, class C = mp::identity>
        using is_disjoint_with = typename is_disjoint_with<Equal, C>::template f<L1, L2>;
#endif
    }
}


/// \cond
namespace jln::mp::detail
{
    template<class Pred>
    struct is_disjoint_impl
    {
        template<class... xs>
        using f = std::is_same<_drop_while_continue, typename _drop_while<
            n_8_or_more_16_32_64_128_256(sizeof...(xs)), true
        >::template f<0, Pred, xs...>>;
    };

    template<class NotEqual, class... xs>
    struct _is_disjoint_of
    {
        template<class x>
        using f = std::is_same<_drop_while_continue, typename _drop_while<
            n_8_or_more_16_32_64_128_256(sizeof...(xs)), true
        >::template f<0, push_back<x, NotEqual>, xs...>>;
    };

    template<>
    struct _is_disjoint<0>
    {
        template<class NotEqual, class... xs>
        using f = true_;
    };

    template<>
    struct _is_disjoint<1> : _is_disjoint<0>
    {};

    template<class NotEqual, class seq0>
    using to_is_disjoint_impl = is_disjoint_impl<
        typename _unpack<seq0>::template f<lift<_is_disjoint_of>, NotEqual>
    >;

    template<>
    struct _is_disjoint<2>
    {
        template<class NotEqual, class seq0, class seq1>
        using f = typename _unpack<seq1>
            ::template f<to_is_disjoint_impl<NotEqual, seq0>>;
    };

    template<>
    struct _is_disjoint<3>
    {
        template<class NotEqual, class seq0, class... seqs>
        using f = std::is_same<_drop_while_continue, typename _drop_while<
            n_8_or_more_16_32_64_128_256(sizeof...(seqs)), true
        >::template f<
            0,
            unpack<to_is_disjoint_impl<NotEqual, seq0>>,
            seqs...
            >>;
    };
}

#ifndef JLN_MP_DOXYGENATING
namespace jln::mp::emp
{
    template<class L1, class L2, class C = mp::identity>
    using is_disjoint = typename C::template f<number<
        detail::_is_disjoint<2>::f<mp::lift<std::is_same, mp::not_<>>, L1, L2>::value
        >>;

    template<class L1, class L2, class Equal = lift<std::is_same>, class C = mp::identity>
    using is_disjoint_with = typename C::template f<number<
        detail::_is_disjoint<2>::f<detail::to_not_fn_t<Equal>, L1, L2>::value
        >>;
}
#endif
/// \endcond
namespace jln::mp
{
    /// \cond
    namespace detail
    {
        template<unsigned>
        struct _is_sorted;
    }
    /// \endcond

    /// \ingroup algorithm

    /// Checks wheteher a \sequence is sorted.
    /// \treturn \number
    template<class Cmp = less<>, class C = identity>
    struct is_sorted
    {
        template<class... xs>
        using f = JLN_MP_DCALL_XS(xs, C,
            typename detail::_is_sorted<detail::min(sizeof...(xs), 3)>
            ::template f<Cmp, xs...>
        );
    };

    /// \cond
    template<class Cmp>
    struct is_sorted<Cmp, identity>
    {
        template<class... xs>
        using f = typename detail::_is_sorted<detail::min(sizeof...(xs), 3)>
            ::template f<Cmp, xs...>;
    };
    /// \endcond

    namespace emp
    {
        template<class L, class Cmp = mp::less<>, class C = mp::identity>
        using is_sorted = unpack<L, mp::is_sorted<Cmp, C>>;
    }
}


/// \cond
namespace jln::mp::detail
{
    template<>
    struct _is_sorted<0>
    {
        template<class Cmp>
        using f = number<1>;
    };

    template<>
    struct _is_sorted<1>
    {
        template<class Cmp, class x>
        using f = number<1>;
    };

    template<>
    struct _is_sorted<2>
    {
        template<class Cmp, class x, class y>
        using f = number<!Cmp::template f<y, x>::value>;
    };

    template<>
    struct _is_sorted<3>
    {
        template<class Cmp, class x, class y, class... xs>
        using f = typename zip_with<Cmp, or_<not_<>>>::template f<
            list<y, xs...>,
            rotate_c<-1, pop_front<>>::f<x, y, xs...>
        >;
    };
}
/// \endcond
namespace jln::mp
{
    /// \cond
    namespace detail
    {
        template<uint_>
        struct _is_subset;
    }
    /// \endcond

    /// \ingroup algorithm

    /// Checks whether \value in `seqs[0]` are subset from the \value in `seqs[1:]`.
    /// Returns `mp::true_` when `sizeof...(seqs) < 2`
    /// \treturn \number
    /// \see is_subset
    template<class Equal = lift<std::is_same>, class C = identity>
    struct is_subset_with
    {
        template<class... seqs>
        using f = typename C::template f<number<
            detail::_is_subset<detail::min(3, sizeof...(seqs))>
            ::template f<detail::to_not_fn_t<Equal>, seqs...>
            ::value
            >>;
    };

    /// Checks whether \value in `seqs[0]` are subset from the \value in `seqs[1:]`.
    /// Returns `mp::true_` when `sizeof...(seqs) < 2`
    /// \treturn \number
    /// \see is_subset_with
    template<class C = identity>
    using is_subset = is_subset_with<lift<std::is_same>, C>;

    namespace emp
    {
#ifdef JLN_MP_DOXYGENATING
        template<class L1, class L2, class C = mp::identity>
        using is_subset = typename is_subset<C>::template f<L1, L2>;

        template<class L1, class L2, class Equal = lift<std::is_same>, class C = mp::identity>
        using is_subset_with = typename is_subset_with<Equal, C>::template f<L1, L2>;
#endif
    }
}


/// \cond
namespace jln::mp::detail
{
    template<class NotEqual, class... xs>
    struct _is_subset_of
    {
        template<class x>
        using f = number<!std::is_same<_drop_while_continue, typename _drop_while<
            n_8_or_more_16_32_64_128_256(sizeof...(xs)), true
        >::template f<0, push_back<x, NotEqual>, xs...>>::value>;
    };

    template<>
    struct _is_subset<0>
    {
        template<class NotEqual, class... xs>
        using f = true_;
    };

    template<>
    struct _is_subset<1> : _is_subset<0>
    {};

    template<class NotEqual, class seq0>
    using to_is_subset_impl = is_disjoint_impl<
        typename _unpack<seq0>::template f<
        if_<
        size<>,
        lift<_is_subset_of>,
        always<size<not_<>>>
        >,
        NotEqual
        >
    >;

    template<>
    struct _is_subset<2>
    {
        template<class NotEqual, class seq0, class seq1>
        using f = typename _unpack<seq1>
            ::template f<to_is_subset_impl<NotEqual, seq0>>;
    };

    template<>
    struct _is_subset<3>
    {
        template<class NotEqual, class seq0, class... seqs>
        using f = number<std::is_same<_drop_while_continue, typename _drop_while<
            n_8_or_more_16_32_64_128_256(sizeof...(seqs)), true
        >::template f<
            0,
            unpack<to_is_subset_impl<NotEqual, seq0>>,
            seqs...
            >>::value>;
    };
}

#ifndef JLN_MP_DOXYGENATING
namespace jln::mp::emp
{
    template<class L1, class L2, class C = mp::identity>
    using is_subset = typename C::template f<number<
        detail::_is_subset<2>::f<mp::lift<std::is_same, mp::not_<>>, L1, L2>::value
        >>;

    template<class L1, class L2, class Equal = lift<std::is_same>, class C = mp::identity>
    using is_subset_with = typename C::template f<number<
        detail::_is_subset<2>::f<detail::to_not_fn_t<Equal>, L1, L2>::value
        >>;
}
#endif
/// \endcond
namespace jln::mp
{
    /// \cond
    namespace detail
    {
        template<class Cmp, class C>
        struct mk_unique;
    }
    /// \endcond

    /// \ingroup algorithm

    /// Returns a list of the same form as L with the duplicate elements removed.
    /// \treturn \sequence
    template<class C = listify>
    using unique = typename detail::mk_unique<lift<std::is_same>, C>::type;

    template<class Cmp = lift<std::is_same>, class C = listify>
    using unique_if = typename detail::mk_unique<Cmp, C>::type;

    namespace emp
    {
        template<class L, class C = mp::listify>
        using unique = unpack<L, unique<C>>;

        template<class L, class Cmp = lift<std::is_same>, class C = mp::listify>
        using unique_if = unpack<L, unique_if<Cmp, C>>;
    }
}


/// \cond
namespace jln::mp::detail
{
    template<class x>
    struct _inherit_impl {};
    template<std::size_t i, class x>
    struct inherit_impl : _inherit_impl<x> {};

    template<class, class...>
    struct inherit;

    template<std::size_t... ints, class... xs>
    struct inherit<std::integer_sequence<std::size_t, ints...>, xs...>
        : inherit_impl<ints, xs>...
    {};

#ifdef _MSC_VER
    template<class... xs>
    struct _is_set
    {
        template <class Pack>
        static auto is_set(Pack pack) -> decltype((
            static_cast<_inherit_impl<xs>*>(pack), ...
            ), number<1>());

        static number<0> is_set(...);

        using type = decltype(is_set(static_cast<
            inherit<std::make_index_sequence<sizeof...(xs)>, xs...>*
        >(nullptr)));
    };
#endif

    template<class L, class x, class = void>
    struct _set_push_back
    {
        using type = L;
    };

    template<class... xs, class x>
    struct _set_push_back<list<xs...>, x,
        std::enable_if_t<
#ifdef _MSC_VER
        _is_set<xs..., x>::type::value
#else
        sizeof(inherit<std::make_index_sequence<sizeof...(xs) + 1>, xs..., x>) == 1
#endif
        >>
    {
        using type = list<xs..., x>;
    };

    struct _set_cmp_push_back_impl
    {
#ifdef __clang__
        template<class Cmp, class x, class... xs>
        using f = typename conditional_c<
            (!Cmp::template f<xs, x>::value && ...)
        >::template f<list<xs..., x>, list<xs...>>;
#else
        template<class> using to_false = false_;

        template<class Cmp, class x, class... xs>
        using f = typename conditional_c<std::is_same<
            list<number<Cmp::template f<xs, x>::value ? 1 : 0>...>,
            list<to_false<xs>...>
        >::value>::template f<list<xs..., x>, list<xs...>>;
#endif
    };

    template<class Cmp>
    struct _set_cmp_push_back
    {
        template<class L, class x>
        using f = typename unpack<_set_cmp_push_back_impl>::template f<L, Cmp, x>;
    };

    template<class Cmp, class C>
    struct mk_unique
    {
        using type = push_front<list<>, fold_left<
            _set_cmp_push_back<Cmp>,
            unpack<C>
            >>;
    };

    template<class C>
    struct mk_unique<lift<std::is_same>, C>
    {
        using type = push_front<list<>, fold_left<
            lift_t<_set_push_back>,
            optimize_useless_unpack_t<unpack<C>>
            >>;
    };

    template<class C>
    struct mk_unique<lift_t<std::is_same>, C>
        : mk_unique<lift<std::is_same>, C>
    {};

    template<class C>
    struct mk_unique<same<>, C>
        : mk_unique<lift<std::is_same>, C>
    {};
}
/// \endcond
namespace jln::mp
{
    /// \cond
    namespace detail
    {
        template<class Cmp, class C>
        struct mk_is_unique;
    }
    /// \endcond

    /// \ingroup algorithm

    /// Checks whether no \values are identical.
    /// \treturn \number
    template<class C = identity>
    using is_unique = typename detail::mk_is_unique<lift<std::is_same>, C>::type;

    template<class Cmp = lift<std::is_same>, class C = identity>
    using is_unique_with = typename detail::mk_is_unique<Cmp, C>::type;

    namespace emp
    {
        template<class L, class C = mp::identity>
        using is_unique = unpack<L, is_unique<C>>;

        template<class L, class Cmp = lift<std::is_same>, class C = mp::identity>
        using is_unique_with = unpack<L, is_unique_with<Cmp, C>>;
    }
}


/// \cond
namespace jln::mp::detail
{
    template<class C>
    struct _is_unique
    {
        template<class... xs>
#ifdef _MSC_VER
        using f = JLN_MP_DCALL_XS(xs, C, typename _is_set<xs...>::type);
#else
        using f = JLN_MP_DCALL_XS(xs, C,
            mp::number<sizeof(inherit<std::make_index_sequence<sizeof...(xs)>, xs...>) == 1>);
#endif
    };

    template<class Cmp, class C>
    struct mk_is_unique
    {
        using type = tee<unique_if<Cmp>, listify, lift_t<std::is_same, to_bool<C>>>;
    };

    template<class C>
    struct mk_is_unique<lift<std::is_same>, C>
    {
        using type = _is_unique<C>;
    };

    template<class C>
    struct mk_is_unique<lift_t<std::is_same>, C>
        : mk_is_unique<lift<std::is_same>, C>
    {};

    template<class C>
    struct mk_is_unique<same<>, C>
        : mk_is_unique<lift<std::is_same>, C>
    {};
}
/// \endcond
namespace jln::mp
{
    /// \cond
    namespace detail
    {
        template<class, class>
        struct _mismatch;
    }
    /// \endcond

    /// \ingroup algorithm

    /// Returns mismatching info of elements from two sequences.
    /// Uses \c C when a element mismatch and \c NC when one of
    /// the sequences equals the start of the other.
    /// \semantics
    ///   `NC::f<number<-1>, number<emp::size<seq1>>>` if \c seq1 == \c seq2.
    ///   `NC::f<number<i>, number<-1>>` if \c seq2 starts with \c seq1.
    ///   `NC::f<number<i>, number<1>>` if \c seq1 starts with \c seq2.
    ///   otherwise `C::f<number<i>, number<0>>`.
    /// \treturn pair or \number
    template<class Cmp = equal<>, class C = listify, class NC = C>
    struct mismatch
    {
        template<class seq1, class seq2>
        using f = typename detail::_mismatch<seq1, seq2>::template f<Cmp, C, NC>;
    };

    namespace emp
    {
        template<class seq1, class seq2, class Cmp = mp::equal<>, class C = mp::listify, class NC = C>
        using mismatch = typename mismatch<Cmp, C, NC>::template f<seq1, seq2>;
    }
}


/// \cond
namespace jln::mp::detail
{
    template<class Cmp, int n>
    struct _mismatch_tree;

    template<class Cmp>
    struct _mismatch_tree<Cmp, 1>
    {
        struct type
        {
            template<class x, class y>
            using f = number<call<Cmp, x, y>::value>;
        };
    };

    template<class Cmp>
    struct _mismatch_tree<Cmp, 2>
    {
        struct type
        {
            template<class x0, class x1, class y0, class y1>
            using f = number<
                !call<Cmp, x0, y0>::value ? 0
                : !call<Cmp, x1, y1>::value ? 1
                : 2
            >;
        };
    };

    template<class Cmp>
    struct _mismatch_tree<Cmp, 3>
    {
        struct type
        {
            template<class x0, class x1, class x2, class y0, class y1, class y2>
            using f = number<
                !call<Cmp, x0, y0>::value ? 0
                : !call<Cmp, x1, y1>::value ? 1
                : !call<Cmp, x2, y2>::value ? 2
                : 3
            >;
        };
    };

    template<class Cmp>
    struct _mismatch_tree<Cmp, 4>
    {
        struct type
        {
            template<class x0, class x1, class x2, class x3, class y0, class y1, class y2, class y3>
            using f = number<
                !call<Cmp, x0, y0>::value ? 0
                : !call<Cmp, x1, y1>::value ? 1
                : !call<Cmp, x2, y2>::value ? 2
                : !call<Cmp, x3, y3>::value ? 3
                : 4
            >;
        };
    };

    constexpr int_ _mismatch_size(int n)
    {
        // is pow 2
        if ((-n ^ n) < -n) {
            return n / 2;
        }

        if (n > 0xffff) {
            return 0x10000;
        }

        // std::bit_floor (c++20)
        int r = 1;

        if (n > 0xff) {
            r <<= 8;
            n >>= 8;
        }

        if (n > 0xf) {
            r <<= 4;
            n >>= 4;
        }

        if (n > 0b11) {
            r <<= 2;
            n >>= 2;
        }

        return (n > 0b1) ? (r << 1) : r;
    }

    template<class Cmp, int_ n, int_ size = _mismatch_size(n)>
    struct _mismatch_tree_next
    {
        template<class i, class = void>
        struct dispatch
        {
            template<class...>
            using f = i;
        };

        template<class v>
        struct dispatch<number<size>, v>
        {
            template<class... xs>
            using f = number<size + call<
                drop_front_c<size,
                rotate_c<n - size,
                drop_front_c<size,
                typename _mismatch_tree<Cmp, n - size>::type
                >
                >
                >,
                xs...
            >::value>;
        };

        template<class... xs>
        using f = typename dispatch<
            typename rotate_c<size,
            drop_front_c<n - size,
            rotate_c<size,
            drop_front_c<n - size,
            typename _mismatch_tree<Cmp, size>::type
            >
            >
            >
      >::template f<xs...>
        >::template f<xs...>;
    };

    template<class Cmp, int n>
    struct _mismatch_tree
    {
        using type = _mismatch_tree_next<Cmp, n>;
    };

    template<
        template<class...> class Txs, class... xs,
        template<class...> class Tys, class... ys>
    struct _mismatch<Txs<xs...>, Tys<ys...>>
    {
        template<class i, class n>
        struct apply_index
        {
            template<class C, class NC, class, class>
            using f = typename C::template f<i, number<0>>;
        };

        template<class n>
        struct apply_index<n, n>
        {
            template<class C, class NC, class i, class r>
            using f = typename NC::template f<i, r>;
        };

        template<bool, bool>
        struct impl
        {
            template<class Cmp, class C, class NC>
            using f = typename apply_index<
                typename _mismatch_tree<Cmp, sizeof...(xs)>::type
                ::template f<xs..., ys...>,
                number<sizeof...(xs)>
            >::template f<C, NC, number<-1>, number<sizeof...(xs)>>;
        };

        template<bool b>
        struct impl<true, b>
        {
            template<class Cmp, class C, class NC>
            using f = typename apply_index<
                typename take_front_c<
                sizeof...(xs) * 2,
                typename _mismatch_tree<Cmp, sizeof...(xs)>::type
                >::template f<xs..., ys...>,
                number<sizeof...(xs)>
            >::template f<C, NC, number<sizeof...(xs)>, number<-1>>;
        };

        template<bool b>
        struct impl<b, true>
        {
            template<class Cmp, class C, class NC>
            using f = typename apply_index<
                typename take_front_c<
                sizeof...(ys) * 2,
                typename _mismatch_tree<Cmp, sizeof...(ys)>::type
                >::template f<ys..., xs...>,
                number<sizeof...(ys)>
            >::template f<C, NC, number<sizeof...(ys)>, number<1>>;
        };

        template<class Cmp, class C, class NC>
        using f = typename impl<
            (sizeof...(xs) < sizeof...(ys)),
            (sizeof...(ys) < sizeof...(xs))
            >::template f<Cmp, C, NC>;
    };

    template<
        template<class...> class Txs,
        template<class...> class Tys, class... ys>
    struct _mismatch<Txs<>, Tys<ys...>>
    {
        template<class Cmp, class C, class NC>
        using f = typename NC::template f<number<0>, number<-1>>;
    };

    template<
        template<class...> class Txs, class... xs,
        template<class...> class Tys>
    struct _mismatch<Txs<xs...>, Tys<>>
    {
        template<class Cmp, class C, class NC>
        using f = typename NC::template f<number<0>, number<1>>;
    };

    template<template<class...> class Txs, template<class...> class Tys>
    struct _mismatch<Txs<>, Tys<>>
    {
        template<class Cmp, class C, class NC>
        using f = typename NC::template f<number<-1>, number<0>>;
    };
}
/// \endcond
namespace jln::mp
{
    /// \cond
    namespace detail
    {
        template<class, class>
        struct _lexicographical_compare;

        template<class Cmp>
        struct _lexicographical_compare_equal;
    }
    /// \endcond

    /// \ingroup algorithm

    /// Checks if \c seq1  is lexicographically less than \c seq2.
    /// \treturn \bool
    template<class Cmp = less<>, class C = identity>
    struct lexicographical_compare
    {
        template<class seq1, class seq2>
        using f = typename C::template f<
            typename mismatch<
            detail::_lexicographical_compare_equal<Cmp>,
            lift<detail::_lexicographical_compare>
            >
            ::template f<seq1, seq2>
            ::template f<Cmp, seq1, seq2>
        >;
    };

    template<class CmpLess = less<>, class CmpEq = equal<>, class C = identity>
    struct lexicographical_compare2
    {
        template<class seq1, class seq2>
        using f = typename C::template f<
            typename mismatch<CmpEq, lift<detail::_lexicographical_compare>>
            ::template f<seq1, seq2>
            ::template f<CmpLess, seq1, seq2>
        >;
    };

    namespace emp
    {
        template<class seq1, class seq2, class Cmp = mp::less<>, class C = mp::identity>
        using lexicographical_compare = typename lexicographical_compare<Cmp, C>
            ::template f<seq1, seq2>;

        template<
            class seq1, class seq2,
            class CmpLess = mp::less<>, class CmpEq = mp::equal<>, class C = mp::identity>
            using lexicographical_compare2 = typename lexicographical_compare2<CmpLess, CmpEq, C>
            ::template f<seq1, seq2>;
    }

    /// \cond
    template<class Cmp>
    struct lexicographical_compare<Cmp, identity>
    {
        template<class seq1, class seq2>
        using f = typename mismatch<
            detail::_lexicographical_compare_equal<Cmp>,
            lift<detail::_lexicographical_compare>
        >
            ::template f<seq1, seq2>
            ::template f<Cmp, seq1, seq2>;
    };

    template<class CmpLess, class CmpEq>
    struct lexicographical_compare2<CmpLess, CmpEq, identity>
    {
        template<class seq1, class seq2>
        using f = typename mismatch<CmpEq, lift<detail::_lexicographical_compare>>
            ::template f<seq1, seq2>
            ::template f<CmpLess, seq1, seq2>;
    };
    /// \endcond
}


namespace jln::mp
{
    /// \ingroup list

    /// Retrieves an element of a sequence at an arbitrary position.
    /// \pre `0 <= N < sizeof...(xs)`
    /// \treturn \value
    template<class N, class C = identity>
    using at = drop_front<N, front<C>>;

    template<int_ n, class C = identity>
    using at_c = drop_front<number<n>, front<C>>;

    template<class C = identity>
    using at0 = front<C>;

    template<class C = identity>
    using at1 = drop_front<number<1>, front<C>>;

    template<class C = identity>
    using at2 = drop_front<number<2>, front<C>>;

    template<class C = identity>
    using at3 = drop_front<number<3>, front<C>>;

    template<class C = identity>
    using at4 = drop_front<number<4>, front<C>>;

    template<class C = identity>
    using at5 = drop_front<number<5>, front<C>>;

    template<class C = identity>
    using at6 = drop_front<number<6>, front<C>>;

    template<class C = identity>
    using at7 = drop_front<number<7>, front<C>>;

    template<class C = identity>
    using at8 = drop_front<number<8>, front<C>>;

    template<class C = identity>
    using at9 = drop_front<number<9>, front<C>>;

    namespace emp
    {
        template<class L, class i, class C = mp::identity>
        using at = unpack<L, mp::drop_front<number<i::value>, mp::front<C>>>;

        template<class L, int_ i, class C = mp::identity>
        using at_c = unpack<L, mp::drop_front<number<i>, mp::front<C>>>;

        template<class L, class C = mp::identity>
        using at0 = unpack<L, mp::front<C>>;

        template<class L, class C = mp::identity>
        using at1 = unpack<L, mp::drop_front<number<1>, mp::front<C>>>;

        template<class L, class C = mp::identity>
        using at2 = unpack<L, mp::drop_front<number<2>, mp::front<C>>>;

        template<class L, class C = mp::identity>
        using at3 = unpack<L, mp::drop_front<number<3>, mp::front<C>>>;

        template<class L, class C = mp::identity>
        using at4 = unpack<L, mp::drop_front<number<4>, mp::front<C>>>;

        template<class L, class C = mp::identity>
        using at5 = unpack<L, mp::drop_front<number<5>, mp::front<C>>>;

        template<class L, class C = mp::identity>
        using at6 = unpack<L, mp::drop_front<number<6>, mp::front<C>>>;

        template<class L, class C = mp::identity>
        using at7 = unpack<L, mp::drop_front<number<7>, mp::front<C>>>;

        template<class L, class C = mp::identity>
        using at8 = unpack<L, mp::drop_front<number<8>, mp::front<C>>>;

        template<class L, class C = mp::identity>
        using at9 = unpack<L, mp::drop_front<number<9>, mp::front<C>>>;
    }
}
/// \cond
namespace jln::mp::detail
{
    template<class Cmp>
    struct _lexicographical_compare_equal
    {
        template<class x, class y>
        using f = number<!call<Cmp, x, y>::value && !call<Cmp, y, x>::value>;
    };

    template<class i, class n>
    struct _lexicographical_compare
    {
        template<class Cmp, class seq1, class seq2>
        using f = false_;
    };

    template<>
    struct _lexicographical_compare<number<-1>, number<0>>
    {
        template<class Cmp, class seq1, class seq2>
        using f = false_;
    };

    template<class i>
    struct _lexicographical_compare<i, number<0>>
    {
        template<class Cmp, class seq1, class seq2>
        using f = typename Cmp::template f<emp::at<seq1, i>, emp::at<seq2, i>>;
    };

    template<class i>
    struct _lexicographical_compare<i, number<-1>>
    {
        template<class Cmp, class seq1, class seq2>
        using f = true_;
    };
}
/// \endcond
namespace jln::mp
{
    /// \cond
    namespace detail
    {
        template<int>
        struct _lower_bound;

        constexpr int _lower_bound_select(unsigned n);

        template<class>
        struct optimize_cmp;
    }
    /// \endcond

    /// \ingroup search

    /// Finds first element that is not less than (i.e. greater or equal to) \c x.
    /// Calls \c FC with all the elements since the one found at the end.
    /// If no element is found, \c NFC is used.
    /// \pre \c is_sorted<Cmp>::f<xs...>
    /// \treturn \sequence
    template<class x, class Cmp = less<>, class C = listify, class NC = C>
    struct lower_bound
    {
        template<class... xs>
        using f = typename detail::_lower_bound<
            detail::_lower_bound_select(sizeof...(xs))
        >::template f<
            sizeof...(xs),
            push_back<x, typename detail::optimize_cmp<Cmp>::type>,
            C, NC, xs...>;
    };

    template<int_ x, class Cmp = less<>, class C = listify, class NC = C>
    using lower_bound_c = lower_bound<number<x>, Cmp, C, NC>;

    template<class x, class C = listify, class NC = C>
    using lower_bound_than = lower_bound<x, less<>, C, NC>;

    template<int_ x, class C = listify, class NC = C>
    using lower_bound_than_c = lower_bound<number<x>, less<>, C, NC>;

    namespace emp
    {
        template<class L, class x, class Cmp = mp::less<>, class C = mp::listify, class NC = C>
        using lower_bound = unpack<L, mp::lower_bound<x, Cmp, C, NC>>;

        template<class L, int_ x, class Cmp = mp::less<>, class C = mp::listify, class NC = C>
        using lower_bound_c = unpack<L, mp::lower_bound<mp::number<x>, Cmp, C, NC>>;

        template<class L, class x, class C = mp::listify, class NC = C>
        using lower_bound_than = unpack<L, mp::lower_bound<x, mp::less<>, C, NC>>;

        template<class L, int_ x, class C = mp::listify, class NC = C>
        using lower_bound_than_c = unpack<L, mp::lower_bound<mp::number<x>, mp::less<>, C, NC>>;
    }
}


/// \cond
namespace jln::mp::detail
{
    template<class Cmp>
    struct optimize_cmp
    {
        using type = Cmp;
    };

    template<class Cmp>
    struct optimize_cmp<flip<tee<less<Cmp>, not_<>>>>
    {
        using type = flip<less<not_<Cmp>>>;
    };

    template<class Cmp, class C>
    struct optimize_cmp<flip<tee<flip<Cmp>, C>>>
        : optimize_cmp<tee<Cmp, C>>
    {};

    template<class C>
    struct optimize_cmp<tee<less<>, C>>
    {
        using type = less<C>;
    };

    template<int>
    struct _smp_lower_bound;

    template<class Bool, class = void>
    struct _smp_conditional
    {
        template<class C, class NC>
        using f = violation;
    };

    template<class Bool>
    struct _smp_conditional<Bool, decltype(void(bool(Bool::value)))>
    {
        template<class C, class NC>
        using f = typename conditional_c<bool(Bool::value)>::template f<C, NC>;
    };

    struct _lower_bound_violation2
    {
        template<unsigned, class...>
        using f = na;
    };

    template<class Bool, class = void>
    struct _smp_conditional2
    {
        template<class C, class NC>
        using f = _lower_bound_violation2;
    };

    template<class Bool>
    struct _smp_conditional2<Bool, decltype(void(bool(Bool::value)))>
    {
        template<class C, class NC>
        using f = typename conditional_c<bool(Bool::value)>::template f<C, NC>;
    };

    constexpr int _lower_bound_select(unsigned n)
    {
        return
            n < 2 ? -int(n)
            : n < 4 ? 2
            : n < 8 ? 4
            : n < 16 ? 8
            : n < 32 ? 16
            : n < 64 ? 32
            : n < 128 ? 64
            : n < 256 ? 128
            : 256
            ;
    }

#define JLN_MP_LOWER_BOUND_IMPL(prefix, Cond)              \
  template<>                                               \
  struct prefix##lower_bound<0>                            \
  {                                                        \
    template<unsigned n, class Pred, class C, class NC,    \
      class... xs>                                         \
    using f = typename NC::template f<>;                   \
  };                                                       \
                                                           \
  /* original size == 1 */                                 \
  template<>                                               \
  struct prefix##lower_bound<-1>                           \
  {                                                        \
    template<unsigned n, class Pred, class C, class NC,    \
      class x>                                             \
    using f = typename Cond(x)                             \
      ::template f<_cartesian<NC, 0> /* for NC::f<> */, C> \
      ::template f<x>;                                     \
  };                                                       \
                                                           \
  template<>                                               \
  struct prefix##lower_bound<1>                            \
  {                                                        \
    template<unsigned n, class Pred, class C, class NC,    \
      class x, class... xs>                                \
    using f = typename Cond(x)                             \
      ::template f<pop_front<C>, C>                        \
      ::template f<x, xs...>;                              \
  };

#define JLN_MP_LOWER_BOUND_PRED_CALL(x) \
  conditional_c<bool(Pred::template f<x>::value)>

#define JLN_MP_LOWER_BOUND_SMP_PRED_CALL(x) \
  _smp_conditional<typename Pred::template f<x>>

    JLN_MP_LOWER_BOUND_IMPL(_, JLN_MP_LOWER_BOUND_PRED_CALL)
        JLN_MP_LOWER_BOUND_IMPL(_smp_, JLN_MP_LOWER_BOUND_SMP_PRED_CALL)

#undef JLN_MP_LOWER_BOUND_IMPL

        template<unsigned> struct _lower_bound_drop_front;
    template<unsigned> struct _smp_lower_bound_drop_front;

#define JLN_MP_LOWER_BOUND_IMPL2(prefix, Cond, n, mp_xs) \
  template<>                                             \
  struct prefix##lower_bound_drop_front<n>                     \
  {                                                      \
    template<int count, class Pred, class C, class NC,   \
      mp_xs(class, JLN_MP_NIL, JLN_MP_COMMA),            \
      class... xs>                                       \
    using f = typename detail::prefix##lower_bound<      \
      _lower_bound_select(count-n)                       \
    >::template f<count-n, Pred, C, NC, xs...>;          \
  };                                                     \
                                                         \
  template<>                                             \
  struct prefix##lower_bound<n>                          \
  {                                                      \
    template<int count, class Pred, class C, class NC,   \
      mp_xs(class, JLN_MP_NIL, JLN_MP_COMMA),            \
      class... xs>                                       \
    using f = typename Cond(_##n)::template f<           \
      prefix##lower_bound_drop_front<n>,                       \
      prefix##lower_bound<n/2>                           \
    >::template f<count, Pred, C, NC,                    \
      mp_xs(JLN_MP_NIL, JLN_MP_NIL, JLN_MP_COMMA),       \
      xs...                                              \
    >;                                                   \
  };

#undef JLN_MP_LOWER_BOUND_SMP_PRED_CALL
#define JLN_MP_LOWER_BOUND_SMP_PRED_CALL(x) \
  _smp_conditional2<typename Pred::template f<x>>

#define JLN_MP_LOWER_BOUND_IMPL(n, mp_xs, mp_rxs, mp_rep)             \
  JLN_MP_LOWER_BOUND_IMPL2(_, JLN_MP_LOWER_BOUND_PRED_CALL, n, mp_xs) \
  JLN_MP_LOWER_BOUND_IMPL2(_smp_, JLN_MP_LOWER_BOUND_SMP_PRED_CALL, n, mp_xs)

    JLN_MP_GEN_XS_2_4_8_16_32_64_128_256(JLN_MP_LOWER_BOUND_IMPL)

#undef JLN_MP_LOWER_BOUND_IMPL
#undef JLN_MP_LOWER_BOUND_PRED_CALL
#undef JLN_MP_LOWER_BOUND_SMP_PRED_CALL
}
/// \endcond
namespace jln::mp
{
    /// \cond
    namespace detail
    {
        struct _drop_back
        {
            template<class F, int_ min, class n>
            using impl = typename conditional_c<min != n::value>
                ::template f<unpack<take_front_c<min, F>>, identity>;

            template<class F, class C, class... ns>
            using f = _each < C, impl < F, std::min({ int_(~0u), ns::value... }), ns > ... > ;
        };
    }
    /// \endcond

    /// \ingroup algorithm

    /// Truncates a \sequence of \typelist on the smallest size.
    /// \pre `sizeof...(result) == sizeof...(xs)`
    /// \post `emp::size<result> == emp::min<emp::size<xs>> && ...`
    /// \semantics
    ///   \code
    ///   call<matrix_shortest<listify>,
    ///     list<_1, _2>,
    ///     list<_1, _2, _3>,
    ///     list<_1, _2>,
    ///     list<_1, _2, _3, _4>
    ///   > = list<
    ///     list<_1, _2>,
    ///     list<_1, _2>,
    ///     list<_1, _2>,
    ///     list<_1, _2>
    ///   >
    ///   \endcode
    /// \treturn \sequence
    /// \note A truncated values becomes a \list
    template<class F = listify, class C = listify>
    struct matrix_shortest_with
    {
        template<class... seqs>
        using f = typename detail::_drop_back::f<F, C, unpack<size<>>::f<seqs>...>
            ::template f<seqs...>;
    };

    template<class C = listify>
    using matrix_shortest = matrix_shortest_with<listify, C>;

    namespace emp
    {
        template<class L, class F = mp::listify, class C = mp::listify>
        using matrix_shortest_with = unpack<L, mp::matrix_shortest_with<F, C>>;

        template<class L, class C = mp::listify>
        using matrix_shortest = unpack<L, mp::matrix_shortest<C>>;
    }
}
namespace jln::mp
{
    /// \cond
    namespace detail
    {
        template<bool, bool>
        struct _dispatch_merge;
    }
    /// \endcond

    /// \ingroup algorithm

    /// Merges two \list into one sorted \sequence.
    /// \pre `emp::is_sorted<seq1, Cmp>`
    /// \pre `emp::is_sorted<seq2, Cmp>`
    /// \post \c emp::is_sorted<result...>
    /// \treturn \sequence
    template<class Cmp = less<>, class C = listify>
    struct merge
    {
        template<class seq1, class seq2>
        using f = typename unpack<C>::template f<
            typename detail::_dispatch_merge<
            std::is_same<seq1, list<>>::value,
            std::is_same<seq2, list<>>::value
            >
            ::template f<seq1, seq2>
            ::template f<Cmp>::type
        >;
    };

    namespace emp
    {
        template<class L, class Cmp = mp::less<>, class C = listify>
        using merge = unpack<L, mp::merge<Cmp, C>>;
    }
}


namespace jln::mp
{
    /// \ingroup list

    /// Inserts elements at the start of \c L list.
    /// \treturn \sequence
    template<class L, class C = listify>
    using prepend = push_front<L, unpack<C>>;

    namespace emp
    {
        template<class L, class... xs>
        using prepend = unpack<L, listify, xs...>;
    }
}
#if __cplusplus > 201703L
namespace jln::mp::detail
{
    using std::type_identity;
}
#else
namespace jln::mp::detail
{
    template<class T>
    struct type_identity
    {
        using type = T;
    };
}
#endif
namespace jln::mp
{
    /// \cond
    template<class Cmp>
    struct merge<Cmp, listify>
    {
        template<class seq1, class seq2>
        using f = typename detail::_dispatch_merge<
            std::is_same<seq1, list<>>::value,
            std::is_same<seq2, list<>>::value
    >::template f<seq1, seq2>
            ::template f<Cmp>::type;
    };
    /// \endcond
}

/// \cond
namespace jln::mp::detail
{
    template<class, class, class, class>
    struct _merge_impl
    {};

    template<class x, class y, class z = list<>, class = true_>
    struct _merge;

    template<bool, bool>
    struct _dispatch_merge
    {
        template<class x, class y>
        using f = _merge<x, y>;
    };

    template<>
    struct _dispatch_merge<true, true>
    {
        template<class x, class y>
        using f = always<type_identity<list<>>>;
    };

    constexpr int _fast_merge_impl_select(bool b, int xn, int yn)
    {
        return b ? (yn >= 4) : (xn >= 4) + 2;
    }

    template<int>
    struct _fast_merge_impl;

    template<>
    struct _fast_merge_impl<0>
    {
        template<class, class, class, class>
        struct f
        {};

        template<class xh, class... xt, class yh, class... yt, class... zs, class cmp>
        struct f<list<xh, xt...>, list<yh, yt...>, list<zs...>, cmp>
            : _merge_impl<list<xh, xt...>, list<yt...>, list<zs..., yh>, cmp>
        {};
    };

    template<>
    struct _fast_merge_impl<1>
    {
        template<class, class, class, class, class = true_>
        struct f
        {};

        template<
            class xh, class... xt,
            class yh0, class yh1, class yh2, class yh3, class yh4, class... yt,
            class... zs, class cmp>
            struct f<
            list<xh, xt...>,
            list<yh0, yh1, yh2, yh3, yh4, yt...>,
            list<zs...>, cmp,
            number<bool(cmp::template f<yh4, xh>::value)>>
            : _merge_impl<list<xh, xt...>, list<yt...>, list<zs..., yh0, yh1, yh2, yh3, yh4>, cmp>
        {};

        template<
            class xh, class... xt,
            class yh0, class yh1, class yh2, class yh3, class yh4, class... yt,
            class... zs, class cmp>
            struct f<
            list<xh, xt...>,
            list<yh0, yh1, yh2, yh3, yh4, yt...>,
            list<zs...>, cmp,
            number<!bool(cmp::template f<yh4, xh>::value)>>
            : _merge_impl<list<xh, xt...>, list<yh1, yh2, yh3, yh4, yt...>, list<zs..., yh0>, cmp>
        {};
    };

    template<>
    struct _fast_merge_impl<2>
    {
        template<class, class, class, class>
        struct f
        {};

        template<class xh, class... xt, class yh, class... yt, class... zs, class cmp>
        struct f<list<xh, xt...>, list<yh, yt...>, list<zs...>, cmp>
            : _merge_impl<list<xt...>, list<yh, yt...>, list<zs..., xh>, cmp>
        {};
    };

    template<>
    struct _fast_merge_impl<3>
    {
        template<class, class, class, class, class = true_>
        struct f
        {};

        template<
            class xh0, class xh1, class xh2, class xh3, class xh4, class... xt,
            class yh, class... yt,
            class... zs, class cmp>
            struct f<
            list<xh0, xh1, xh2, xh3, xh4, xt...>,
            list<yh, yt...>,
            list<zs...>, cmp,
            number<!bool(cmp::template f<yh, xh4>::value)>>
            : _merge_impl<list<xt...>, list<yh, yt...>, list<zs..., xh0, xh1, xh2, xh3, xh4>, cmp>
        {};

        template<
            class xh0, class xh1, class xh2, class xh3, class xh4, class... xt,
            class yh, class... yt,
            class... zs, class cmp>
            struct f<
            list<xh0, xh1, xh2, xh3, xh4, xt...>,
            list<yh, yt...>,
            list<zs...>, cmp,
            number<bool(cmp::template f<yh, xh4>::value)>>
            : _merge_impl<list<xh1, xh2, xh3, xh4, xt...>, list<yh, yt...>, list<zs..., xh0>, cmp>
        {};
    };

    template<class xh, class... xt, class yh, class... yt, class... zs, class cmp>
    struct _merge_impl<list<xh, xt...>, list<yh, yt...>, list<zs...>, cmp>
        : _fast_merge_impl<_fast_merge_impl_select(
            bool(cmp::template f<yh, xh>::value), sizeof...(xt), sizeof...(yt)
        )>::template f<
        list<xh, xt...>, list<yh, yt...>, list<zs...>, cmp>
    {};

    template<class... xs, class... zs, class cmp>
    struct _merge_impl<list<xs...>, list<>, list<zs...>, cmp>
    {
        template<class x, class y>
        using part = typename _merge<emp::prepend<x, xs...>, y, list<zs...>>::template f<cmp>;

        using type = list<zs..., xs...>;
    };

    template<class... ys, class... zs, class cmp>
    struct _merge_impl<list<>, list<ys...>, list<zs...>, cmp>
    {
        template<class x, class y>
        using part = typename _merge<x, emp::prepend<y, ys...>, list<zs...>>::template f<cmp>;

        using type = list<zs..., ys...>;
    };

    template<class... xs, class... ys, class z>
    struct _merge<list<xs...>, list<ys...>, z, number<
        // x.size + y.size >= 100, unless empty list
        (sizeof...(xs) && sizeof...(ys) && sizeof...(xs) > 0 && sizeof...(ys) >= 100u)>>
    {
        using xm = number<(sizeof...(xs) + 1) / 2>;
        using ym = number<(sizeof...(ys) + 1) / 2>;

        using xr = typename drop_front<xm>::template f<xs...>;
        using yr = typename drop_front<ym>::template f<ys...>;

        using l = _merge<
            typename take_front<xm>::template f<xs...>,
            typename take_front<ym>::template f<ys...>,
            z
        >;

        template<class cmp>
        using f = typename l::template f<cmp>::template part<xr, yr>;
    };

    template<class x, class y, class z, class>
    struct _merge
    {
        template<class cmp>
        using f = _merge_impl<x, y, z, cmp>;
    };

    // could be deleted
    template<class x, class z>
    struct _merge<x, list<>, z>
    {
        template<class cmp>
        using f = _merge_impl<x, list<>, z, cmp>;
    };

    // could be deleted
    template<class y, class z>
    struct _merge<list<>, y, z>
    {
        template<class cmp>
        using f = _merge_impl<list<>, y, z, cmp>;
    };
}
/// \endcond
namespace jln::mp
{
    /// \ingroup functional

    /// Invokes twice.
    /// \treturn \value
    template<class F>
    struct invoke_twice
    {
        template<class... xs>
        using f = typename call<F, xs...>::template f<xs...>;
    };
}
namespace jln::mp
{
    /// \ingroup algorithm

    /// Returns the first mismatching index of elements from two sequences, otherwise the size of the sequences.
    /// \treturn \number
    template<class Cmp = equal<>, class C = identity>
    using mismatch_index = mismatch<Cmp, at0<C>, if_<at0<is<number<-1>>>, at1<C>, at0<C>>>;

    namespace emp
    {
        template<class seq1, class seq2, class Cmp = mp::equal<>, class C = mp::identity>
        using mismatch_index = typename mismatch_index<Cmp, C>::template f<seq1, seq2>;
    }
}
namespace jln::mp
{
    /// \ingroup algorithm

    /// Checks whether a predicate does not hold for any element of a \sequence.
    /// \treturn \bool
    template<class Pred, class C = identity>
    using none_of = drop_while<not_fn<Pred>, is_empty<C>>;

    namespace emp
    {
        template<class L, class Pred, class C = mp::identity>
        using none_of = unpack<L, mp::none_of<Pred, C>>;
    }
} // namespace jln::mp
namespace jln::mp
{
    /// \ingroup group

    // TODO remove <-> copy
    // Pred::f<xs>::value... -> mk_list<b>::f<xs> | mk_list<!b>::f<xs>

    /// Splits a \list in two according to a predicate.
    /// The first value contains all elements for which the predicate returns true,
    /// the second value contains all elements for which predicate returns false
    /// \treturn \sequence of two \values
    template<class Pred, class F = listify, class C = listify>
    using partition_with = tee<copy_if<Pred, F>, remove_if<Pred, F>, C>;

    /// Splits a \list in two according to a predicate.
    /// The first value contains all elements for which the predicate returns true,
    /// the second value contains all elements for which predicate returns false
    /// \treturn \sequence of two \lists
    /// \see partition_with
    template<class Pred, class C = listify>
    using partition = tee<copy_if<Pred>, remove_if<Pred>, C>;

    namespace emp
    {
        template<class L, class Pred, class F = mp::listify, class C = mp::listify>
        using partition_with = unpack<L, mp::partition_with<Pred, F, C>>;

        template<class L, class Pred, class C = mp::listify>
        using partition = unpack<L, mp::partition<Pred, C>>;
    }
}
namespace jln::mp
{
    /// \cond
    namespace detail
    {
        template<int>
        struct _permutations;

        constexpr int _permutations_select(int n);
    }
    /// \endcond

    /// \ingroup algorithm

    /// Generates all permutations of \sequence.
    /// \post sizeof...(result) == sizeof...(xs)!
    /// \treturn \sequence of \list
    template<class C = listify>
    struct permutations
    {
        template<class... xs>
        using f = typename detail::_permutations<detail::_permutations_select(sizeof...(xs))>
            ::template f<C, xs...>;
    };

    namespace emp
    {
        template<class L, class C = listify>
        using permutations = unpack<L, mp::permutations<C>>;
    }
}


/// \cond
namespace jln::mp::detail
{
    constexpr int _permutations_select(int n)
    {
#ifdef JLN_MP_BIG_LIST_PERMUTATIONS
        // -1 with big list result (40320 elements)
        return n > 8 ? -1 : n;
#else
        // -1 with big list result (5040 elements)
        return n > 7 ? -1 : n;
#endif
    }

    template<>
    struct _permutations<-1>
    {};

    template<>
    struct _permutations<0>
    {
        template<class C, class...>
        using f = typename C::template f<>;
    };

    template<>
    struct _permutations<1>
    {
        template<class C, class x>
        using f = typename C::template f<list<x>>;
    };

    template<>
    struct _permutations<2>
    {
        template<class C, class x0, class x1>
        using f = typename C::template f<
            list<x0, x1>, list<x1, x0>
        >;
    };

    template<>
    struct _permutations<3>
    {
        template<class C, class x0, class x1, class x2>
        using f = typename C::template f<
            list<x0, x1, x2>, list<x0, x2, x1>,
            list<x1, x0, x2>, list<x1, x2, x0>,
            list<x2, x0, x1>, list<x2, x1, x0>
        >;
    };

    template<>
    struct _permutations<4>
    {
        template<class C, class x0, class x1, class x2, class x3>
        using f = typename C::template f<
            list<x0, x1, x2, x3>, list<x0, x1, x3, x2>,
            list<x0, x2, x1, x3>, list<x0, x2, x3, x1>,
            list<x0, x3, x1, x2>, list<x0, x3, x2, x1>,

            list<x1, x0, x2, x3>, list<x1, x0, x3, x2>,
            list<x1, x2, x0, x3>, list<x1, x2, x3, x0>,
            list<x1, x3, x0, x2>, list<x1, x3, x2, x0>,

            list<x2, x0, x1, x3>, list<x2, x0, x3, x1>,
            list<x2, x1, x0, x3>, list<x2, x1, x3, x0>,
            list<x2, x3, x0, x1>, list<x2, x3, x1, x0>,

            list<x3, x0, x1, x2>, list<x3, x0, x2, x1>,
            list<x3, x1, x0, x2>, list<x3, x1, x2, x0>,
            list<x3, x2, x0, x1>, list<x3, x2, x1, x0>
        >;
    };

    struct _insert_permutations4
    {
        template<class x, class x0, class x1, class x2, class x3>
        using f = list<
            list<x, x0, x1, x2, x3>, list<x, x0, x1, x3, x2>,
            list<x, x0, x2, x1, x3>, list<x, x0, x2, x3, x1>,
            list<x, x0, x3, x1, x2>, list<x, x0, x3, x2, x1>,

            list<x, x1, x0, x2, x3>, list<x, x1, x0, x3, x2>,
            list<x, x1, x2, x0, x3>, list<x, x1, x2, x3, x0>,
            list<x, x1, x3, x0, x2>, list<x, x1, x3, x2, x0>,

            list<x, x2, x0, x1, x3>, list<x, x2, x0, x3, x1>,
            list<x, x2, x1, x0, x3>, list<x, x2, x1, x3, x0>,
            list<x, x2, x3, x0, x1>, list<x, x2, x3, x1, x0>,

            list<x, x3, x0, x1, x2>, list<x, x3, x0, x2, x1>,
            list<x, x3, x1, x0, x2>, list<x, x3, x1, x2, x0>,
            list<x, x3, x2, x0, x1>, list<x, x3, x2, x1, x0>
        >;
    };

    template<>
    struct _permutations<5>
    {
        template<class C, class x0, class x1, class x2, class x3, class x4>
        using f = typename join<C>::template f<
            _insert_permutations4::f<x0, x1, x2, x3, x4>,
            _insert_permutations4::f<x1, x2, x3, x4, x0>,
            _insert_permutations4::f<x2, x3, x4, x0, x1>,
            _insert_permutations4::f<x3, x4, x0, x1, x2>,
            _insert_permutations4::f<x4, x0, x1, x2, x3>
        >;
    };

    struct _push_permutations4
    {
        template<class F, class x, class x0, class x1, class x2, class x3>
        using f = list<
            typename F::template f<x, x0, x1, x2, x3>, typename F::template f<x, x0, x1, x3, x2>,
            typename F::template f<x, x0, x2, x1, x3>, typename F::template f<x, x0, x2, x3, x1>,
            typename F::template f<x, x0, x3, x1, x2>, typename F::template f<x, x0, x3, x2, x1>,

            typename F::template f<x, x1, x0, x2, x3>, typename F::template f<x, x1, x0, x3, x2>,
            typename F::template f<x, x1, x2, x0, x3>, typename F::template f<x, x1, x2, x3, x0>,
            typename F::template f<x, x1, x3, x0, x2>, typename F::template f<x, x1, x3, x2, x0>,

            typename F::template f<x, x2, x0, x1, x3>, typename F::template f<x, x2, x0, x3, x1>,
            typename F::template f<x, x2, x1, x0, x3>, typename F::template f<x, x2, x1, x3, x0>,
            typename F::template f<x, x2, x3, x0, x1>, typename F::template f<x, x2, x3, x1, x0>,

            typename F::template f<x, x3, x0, x1, x2>, typename F::template f<x, x3, x0, x2, x1>,
            typename F::template f<x, x3, x1, x0, x2>, typename F::template f<x, x3, x1, x2, x0>,
            typename F::template f<x, x3, x2, x0, x1>, typename F::template f<x, x3, x2, x1, x0>
        >;
    };

    template<class C>
    struct _permutations5
    {
        template<class F, class x0, class x1, class x2, class x3, class x4>
        using f = typename join<C>::template f<
            _push_permutations4::f<F, x0, x1, x2, x3, x4>,
            _push_permutations4::f<F, x1, x2, x3, x4, x0>,
            _push_permutations4::f<F, x2, x3, x4, x0, x1>,
            _push_permutations4::f<F, x3, x4, x0, x1, x2>,
            _push_permutations4::f<F, x4, x0, x1, x2, x3>
        >;
    };

    template<class C>
    struct _permutations6
    {
        template<class F, class x0, class x1, class x2, class x3, class x4, class x5>
        using f = typename join<C>::template f<
            _permutations5<listify>::f<push_front<x0, F>, x1, x2, x3, x4, x5>,
            _permutations5<listify>::f<push_front<x1, F>, x2, x3, x4, x5, x0>,
            _permutations5<listify>::f<push_front<x2, F>, x3, x4, x5, x0, x1>,
            _permutations5<listify>::f<push_front<x3, F>, x4, x5, x0, x1, x2>,
            _permutations5<listify>::f<push_front<x4, F>, x5, x0, x1, x2, x3>,
            _permutations5<listify>::f<push_front<x5, F>, x0, x1, x2, x3, x4>
        >;
    };

    template<>
    struct _permutations<6>
    {
        template<class C, class x0, class x1, class x2, class x3, class x4, class x5>
        using f = typename _permutations6<C>::template f<listify, x0, x1, x2, x3, x4, x5>;
    };

    template<class C>
    struct _permutations7
    {
        template<class F, class x0, class x1, class x2, class x3, class x4, class x5, class x6>
        using f = typename join<C>::template f<
            _permutations6<listify>::f<push_front<x0, F>, x1, x2, x3, x4, x5, x6>,
            _permutations6<listify>::f<push_front<x1, F>, x2, x3, x4, x5, x6, x0>,
            _permutations6<listify>::f<push_front<x2, F>, x3, x4, x5, x6, x0, x1>,
            _permutations6<listify>::f<push_front<x3, F>, x4, x5, x6, x0, x1, x2>,
            _permutations6<listify>::f<push_front<x4, F>, x5, x6, x0, x1, x2, x3>,
            _permutations6<listify>::f<push_front<x5, F>, x6, x0, x1, x2, x3, x4>,
            _permutations6<listify>::f<push_front<x6, F>, x0, x1, x2, x3, x4, x5>
        >;
    };

    template<>
    struct _permutations<7>
    {
        template<class C, class x0, class x1, class x2, class x3, class x4, class x5, class x6>
        using f = typename _permutations7<C>::template f<listify, x0, x1, x2, x3, x4, x5, x6>;
    };

#ifdef JLN_MP_BIG_LIST_PERMUTATIONS
    template<class C>
    struct _permutations8
    {
        template<class F, class x0, class x1, class x2, class x3, class x4, class x5, class x6, class x7>
        using f = typename join<C>::template f<
            _permutations7<listify>::f<push_front<x0, F>, x1, x2, x3, x4, x5, x6, x7>,
            _permutations7<listify>::f<push_front<x1, F>, x2, x3, x4, x5, x6, x7, x0>,
            _permutations7<listify>::f<push_front<x2, F>, x3, x4, x5, x6, x7, x0, x1>,
            _permutations7<listify>::f<push_front<x3, F>, x4, x5, x6, x7, x0, x1, x2>,
            _permutations7<listify>::f<push_front<x4, F>, x5, x6, x7, x0, x1, x2, x3>,
            _permutations7<listify>::f<push_front<x5, F>, x6, x7, x0, x1, x2, x3, x4>,
            _permutations7<listify>::f<push_front<x6, F>, x7, x0, x1, x2, x3, x4, x5>,
            _permutations7<listify>::f<push_front<x7, F>, x0, x1, x2, x3, x4, x5, x6>
        >;
    };

    template<>
    struct _permutations<8>
    {
        template<class C, class x0, class x1, class x2, class x3, class x4, class x5, class x6, class x7>
        using f = typename _permutations8<C>::template f<listify, x0, x1, x2, x3, x4, x5, x6, x7>;
    };
#endif
}
/// \endcond
namespace jln::mp
{
    /// \cond
    namespace detail
    {
        template<class C, bool>
        struct _powerset;
    }
    /// \endcond

    /// \ingroup algorithm

    /// Computes the powerset of a \sequence.
    /// \semantics
    ///   \code
    ///   call<powerset<>, a, b, c> == list<
    ///     list<>,
    ///     list<a>, list<b>, list<a, b>, list<c>,
    ///     list<a, c>, list<b, c>,
    ///     list<a, b, c>
    ///   >
    ///   \endcode
    /// \treturn \sequence of \list
    template<class C = listify>
    struct powerset
    {
        template<class... xs>
        using f = typename detail::_powerset<C, bool(sizeof...(xs))>
            ::type::template f<list<list<>>, xs...>;
    };

    namespace emp
    {
        template<class L, class C = mp::listify>
        using powerset = unpack<L, mp::powerset<C>>;
    }
}


/// \cond
namespace jln::mp::detail
{
    template<class, class>
    struct _power;

    template<class... xs, class y>
    struct _power<list<xs...>, y>
    {
        using type = list<list<xs...>, list<y, xs...>>;
    };

    template<class, class>
    struct _powerset_impl;

    template<class... seqs, class y>
    struct _powerset_impl<y, list<seqs...>>
    {
        using type = join<>::f<typename _power<seqs, y>::type...>;
    };

    template<class C, bool>
    struct _powerset
    {
        using type = fold_right<lift_t<_powerset_impl>, unpack<C>>;
    };

    template<>
    struct _powerset<listify, true>
    {
        using type = fold_right<lift_t<_powerset_impl>>;
    };

    template<class C>
    struct _powerset<C, false>
    {
        using type = _cartesian<C, 0>; // for C::f<>
    };

    template<>
    struct _powerset<listify, false>
    {
        using type = always<list<>>;
    };
}
/// \endcond
namespace jln::mp
{
    /// \ingroup algorithm

    /// Inserts a \value before each element of a \sequence.
    /// \treturn \list
    /// \see intersperse, suffix
    template<class x, class C = listify>
    struct prefix
    {
        template<class... xs>
        using f = typename detail::_join_select<sizeof...(xs)>
            ::template f<C, list<x, xs>...>::type;
    };

    namespace emp
    {
        template<class L, class x, class C = mp::listify>
        using prefix = unpack<L, mp::prefix<x, C>>;
    }
}
namespace jln::mp
{
    /// \ingroup algorithm

    /// Replaces every occurrence that satisfy a predicate by some \value.
    /// \treturn \sequence
    template<class Pred, class T, class C = listify>
    using replace_if = transform<if_<Pred, always<T>, identity>, C>;

    /// Replaces every occurrence of a \value by another \value.
    /// \treturn \sequence
    template<class T, class U, class C = listify>
    using replace = replace_if<is<T>, U, C>;

    namespace emp
    {
        template<class L, class T, class U, class C = mp::listify>
        using replace = unpack<L, mp::replace<T, U, C>>;

        template<class L, class Pred, class T, class C = mp::listify>
        using replace_if = unpack<L, mp::replace_if<Pred, T, C>>;
    }
}
namespace jln::mp
{
    /// \cond
    namespace detail
    {
        template<unsigned>
        struct _reverse;

        constexpr unsigned _rotate_select(unsigned n)
        {
            return (detail::n_8_or_less_16_64_256(n) == n) * 20
                + detail::n_8_or_less_16_64_256(n);
        }
    }
    /// \endcond

    /// \ingroup algorithm

    /// Reverses the order of the elements of a \sequence.
    /// \treturn \sequence
    template<class C = listify>
    struct reverse
    {
        template<class... xs>
        using f = typename detail::_reverse<
            detail::_rotate_select(sizeof...(xs))
        >::template f<C, sizeof...(xs), xs...>;
    };

    namespace emp
    {
        template<class L, class C = mp::listify>
        using reverse = unpack<L, mp::reverse<C>>;
    }
}


/// \cond
namespace jln::mp::detail
{
#define JLN_MP_REVERSE_IMPL(n, mp_xs, mp_rxs, mp_rep)    \
  template<>                                             \
  struct _reverse<n>                                     \
  {                                                      \
    template<class C, std::size_t count                  \
      mp_xs(JLN_MP_COMMA class, JLN_MP_NIL, JLN_MP_NIL), \
      class... xs>                                       \
    using f = typename reverse<join<C>>                  \
      ::template f<xs..., list<                          \
        mp_rxs(JLN_MP_NIL, JLN_MP_NIL, JLN_MP_COMMA)>>;  \
  };                                                     \
  template<>                                             \
  struct _reverse<n+20>                                  \
  {                                                      \
    template<class C, std::size_t count                  \
      mp_xs(JLN_MP_COMMA class, JLN_MP_NIL, JLN_MP_NIL), \
      class... xs>                                       \
    using f = typename C::template f<                    \
      mp_rxs(JLN_MP_NIL, JLN_MP_NIL, JLN_MP_COMMA)>;     \
  };

    JLN_MP_GEN_XS_0_TO_8(JLN_MP_REVERSE_IMPL)

#undef JLN_MP_REVERSE_IMPL

#define JLN_MP_REVERSE_IMPL(n, mp_xs, mp_rxs, mp_rep) \
  template<>                                          \
  struct _reverse<n>                                  \
  {                                                   \
    template<class C, std::size_t count,              \
      mp_xs(class, JLN_MP_NIL, JLN_MP_COMMA),         \
      class... xs>                                    \
    using f = typename _reverse<                      \
      detail::n_8_or_less_16_64_256(count-n)          \
    >::template f<C, count-n, xs..., list<            \
      mp_rxs(JLN_MP_NIL, JLN_MP_NIL, JLN_MP_COMMA)>>; \
  };                                                  \
  template<>                                          \
  struct _reverse<n+20>                               \
  {                                                   \
    template<class C, std::size_t count,              \
      mp_xs(class, JLN_MP_NIL, JLN_MP_COMMA),         \
      class... xs>                                    \
    using f = typename C::template f<                 \
      mp_rxs(JLN_MP_NIL, JLN_MP_NIL, JLN_MP_COMMA)>;  \
  };

        JLN_MP_GEN_XS_8_16_64_256(JLN_MP_REVERSE_IMPL)

#undef JLN_MP_REVERSE_IMPL
}
/// \endcond
namespace jln::mp
{
    /// \cond
    namespace detail
    {
        template<int>
        struct _reverse_fold;
    }
    /// \endcond

    /// \ingroup algorithm

    /// Folds right over a list using a binary predicate.
    /// reverse_fold consideres the first element in the input pack as the state,
    /// use \c push_front<> to add state if needed.
    /// \semantics
    ///   Equivalent to
    ///   \code
    ///   F::f<... F::f<F::f<state, xs[n-1]>, x[n-2]>, ..., x[0]>
    ///   \endcode
    /// \treturn \value
    /// \see fold_left, fold_right, fold_tree, fold_balanced_tree
#ifdef JLN_MP_DOXYGENATING
    template<class F, class C = identity>
    using reverse_fold = reverse<fold_left<F, C>>;
#else
    template<class F, class C = identity>
    struct reverse_fold
    {
        template<class... xs>
        using f = typename C::template f<
            typename detail::_reverse_fold<
            detail::sub_1_n_4_or_less_8_16_64_256(sizeof...(xs))
      >::template f<F::template f, xs...>
        >;
    };
#endif

    namespace emp
    {
        template<class L, class state, class F, class C = mp::identity>
        using reverse_fold = unpack<L,
            mp::push_front<state, mp::reverse_fold<F, C>>>;
    }
}


/// \cond
namespace jln::mp::detail
{
#define JLN_MP_REVERSE_FOLD_SELECT(n, mp_xs, mp_rxs, mp_dup) \
  template<>                                                 \
  struct _reverse_fold<n>                                    \
  {                                                          \
    template<template<class...> class F, class state,        \
      mp_xs(class, JLN_MP_NIL, JLN_MP_COMMA),                \
      class... xs>                                           \
    using f = mp_dup(F<, JLN_MP_NIL)                         \
      typename _reverse_fold<                                \
        detail::n_4_or_less_8_16_64_256(sizeof...(xs))       \
      >::template f<F, state, xs...>,                        \
    mp_rxs(JLN_MP_NIL, >, JLN_MP_COMMA);                     \
  };

    JLN_MP_GEN_XS_4_8_16_64_256(JLN_MP_REVERSE_FOLD_SELECT)

#undef JLN_MP_REVERSE_FOLD_SELECT

#define JLN_MP_REVERSE_FOLD_SELECT(n, mp_xs, mp_rxs, mp_dup) \
  template<>                                                 \
  struct _reverse_fold<n>                                    \
  {                                                          \
    template<template<class...> class F, class state,        \
      mp_xs(class, JLN_MP_NIL, JLN_MP_COMMA)>                \
    using f = mp_dup(F<, JLN_MP_NIL)                         \
      state, mp_rxs(JLN_MP_NIL, >, JLN_MP_COMMA);            \
  };

        JLN_MP_GEN_XS_1_TO_4(JLN_MP_REVERSE_FOLD_SELECT)

#undef JLN_MP_REVERSE_FOLD_SELECT

        template<>
    struct _reverse_fold<0>
    {
        template<template<class...> class, class state>
        using f = state;
    };

    template<>
    struct _reverse_fold<-1>
    {};
}
/// \endcond
namespace jln::mp
{
    /// \cond
    namespace detail
    {
        template<unsigned>
        struct _scan_left;
    }
    /// \endcond

    /// \ingroup algorithm

    /// Fold a \sequence to the left and return a list containing the successive reduction states.
    /// \semantics
    ///   Equivalent to
    ///   \code
    ///   C::f<
    ///     xs[0],
    ///     F::f<xs[0], xs[1]>,
    ///     F::f<F::f<xs[0], xs[1]>, xs[2]>,
    ///     ...
    ///   >
    ///   \endcode
    /// \treturn \sequence
    template<class F, class C = listify>
    struct scan_left
    {
        template<class... xs>
        using f = typename detail::_scan_left<
            detail::n_8_or_more_16_32_64_128_256(sizeof...(xs))
        >::template f<sizeof...(xs), C, F::template f, xs...>;
    };

    namespace emp
    {
        template<class L, class F, class C = mp::listify>
        using scan_left = unpack<L, mp::scan_left<F, C>>;
    }
}


/// \cond
namespace jln::mp::detail
{
    template<>
    struct _scan_left<0>
    {
        template<unsigned remaining, class C, template<class...> class F,
            class... xs>
            using f = typename join<C>::template f<xs...>;
    };

    template<>
    struct _scan_left<1>
    {
        template<unsigned remaining, class C, template<class...> class F,
            class _0, class... xs>
            using f = typename join<C>::template f<xs..., list<_0>>;
    };

    template<>
    struct _scan_left<2>
    {
        template<unsigned remaining, class C, template<class...> class F,
            class _0, class _1, class... xs>
            using f = typename join<C>::template f<xs..., list<_0, F<_0, _1>>>;
    };

    // for n in range(3,9):
    //   args=', '.join(f'class _{i}' for i in range(1,n))
    //   ps=', '.join(f'_{i}' for i in range(n))
    //   rs=', '.join(f'class r{i} = F<r{i-1}, _{i}>' for i in range(1, n-1))
    //   l=', '.join(f'r{i}' for i in range(n-1))
    //   sep=', ' if n > 2 else ''
    //   print(f'''
    //   template<>
    //   struct _scan_left<{n}>
    //   {{
    //     template<template<class...> class F,
    //       class r0, {args}{sep}{rs}>
    //     using g = list<{l}, F<r{n-2}, _{n-1}>>;
    //
    //     template<unsigned remaining, class C, template<class...> class F,
    //       class _0, {args}, class... xs>
    //     using f = typename join<C>::template f<xs..., g<F, {ps}>>;
    //   }};''')

    template<>
    struct _scan_left<3>
    {
        template<template<class...> class F,
            class r0, class _1, class _2, class r1 = F<r0, _1>>
            using g = list<r0, r1, F<r1, _2>>;

        template<unsigned remaining, class C, template<class...> class F,
            class _0, class _1, class _2, class... xs>
            using f = typename join<C>::template f<xs..., g<F, _0, _1, _2>>;
    };

    template<>
    struct _scan_left<4>
    {
        template<template<class...> class F,
            class r0, class _1, class _2, class _3,
            class r1 = F<r0, _1>, class r2 = F<r1, _2>>
            using g = list<r0, r1, r2, F<r2, _3>>;

        template<unsigned remaining, class C, template<class...> class F,
            class _0, class _1, class _2, class _3, class... xs>
            using f = typename join<C>::template f<xs..., g<F, _0, _1, _2, _3>>;
    };

    template<>
    struct _scan_left<5>
    {
        template<template<class...> class F,
            class r0, class _1, class _2, class _3, class _4,
            class r1 = F<r0, _1>, class r2 = F<r1, _2>, class r3 = F<r2, _3>>
            using g = list<r0, r1, r2, r3, F<r3, _4>>;

        template<unsigned remaining, class C, template<class...> class F,
            class _0, class _1, class _2, class _3, class _4, class... xs>
            using f = typename join<C>::template f<xs..., g<F, _0, _1, _2, _3, _4>>;
    };

    template<>
    struct _scan_left<6>
    {
        template<template<class...> class F,
            class r0, class _1, class _2, class _3, class _4, class _5,
            class r1 = F<r0, _1>, class r2 = F<r1, _2>, class r3 = F<r2, _3>,
            class r4 = F<r3, _4>>
            using g = list<r0, r1, r2, r3, r4, F<r4, _5>>;

        template<unsigned remaining, class C, template<class...> class F,
            class _0, class _1, class _2, class _3, class _4, class _5,
            class... xs>
            using f = typename join<C>
            ::template f<xs..., g<F, _0, _1, _2, _3, _4, _5>>;
    };

    template<>
    struct _scan_left<7>
    {
        template<template<class...> class F,
            class r0, class _1, class _2, class _3, class _4, class _5, class _6,
            class r1 = F<r0, _1>, class r2 = F<r1, _2>, class r3 = F<r2, _3>,
            class r4 = F<r3, _4>, class r5 = F<r4, _5>>
            using g = list<r0, r1, r2, r3, r4, r5, F<r5, _6>>;

        template<unsigned remaining, class C, template<class...> class F,
            class _0, class _1, class _2, class _3, class _4, class _5, class _6,
            class... xs>
            using f = typename join<C>
            ::template f<xs..., g<F, _0, _1, _2, _3, _4, _5, _6>>;
    };

    template<>
    struct _scan_left<8>
    {
        template<template<class...> class F,
            class r0, class _1, class _2, class _3, class _4, class _5, class _6,
            class _7, class r1 = F<r0, _1>, class r2 = F<r1, _2>,
            class r3 = F<r2, _3>, class r4 = F<r3, _4>, class r5 = F<r4, _5>,
            class r6 = F<r5, _6>>
            using g = list<r0, r1, r2, r3, r4, r5, r6, F<r6, _7>>;

        template<unsigned remaining, class C, template<class...> class F,
            class _0, class _1, class _2, class _3, class _4, class _5, class _6,
            class _7, class... xs>
            using f = typename join<C>::template f<xs..., g<F, _0, _1, _2, _3, _4, _5, _6, _7>>;
    };

    template<class state, class l>
    struct scan_continuation
    {
        template<
            unsigned remaining, class Next, class C,
            template<class...> class F, class... xs>
        using f = typename Next::template f<remaining, C, F, state, xs..., l>;
    };

    // for n in [8,16,32,64,128]:
    //   args=', '.join(f'class _{i}' for i in range(1, n))
    //   ps=', '.join(f'_{i}' for i in range(n))
    //   rs=', '.join(f'class r{i} = F<r{i-1}, _{i}>' for i in range(1, n-1))
    //   l=', '.join(f'r{i}' for i in range(n-1))
    //   print(f'''
    //   template<>
    //   struct _scan_left<{n*2}>
    //   {{
    //     template<template<class...> class F,
    //       class r0, {args},
    //       {rs},
    //     using g = scan_continuation<F<r{n-2}, _{n-1}>, list<{l}>>;
    //
    //     template<unsigned remaining, class C, template<class...> class F,
    //       class _0, {args},
    //       class... xs>
    //     using f = typename g<F, {ps}>
    //       ::template f<remaining-{n-1}, _scan_left<n_8_or_more_16_32_64_128_256(remaining-{n-1})>, C, F, xs...>;
    //   }};''')

    template<>
    struct _scan_left<16>
    {
        template<
            template<class...> class F, class r0, class _1, class _2, class _3,
            class _4, class _5, class _6, class _7, class r1 = F<r0, _1>,
            class r2 = F<r1, _2>, class r3 = F<r2, _3>, class r4 = F<r3, _4>,
            class r5 = F<r4, _5>, class r6 = F<r5, _6>>
            using g = scan_continuation<F<r6, _7>, list<r0, r1, r2, r3, r4, r5, r6>>;

        template<
            unsigned remaining, class C, template<class...> class F, class _0,
            class _1, class _2, class _3, class _4, class _5, class _6, class _7,
            class... xs>
            using f = typename g<F, _0, _1, _2, _3, _4, _5, _6, _7>::template f<
            remaining - 7, _scan_left<n_8_or_more_16_32_64_128_256(remaining - 7)>,
            C, F, xs...>;
    };

    template<>
    struct _scan_left<32>
    {
        template<
            template<class...> class F, class r0, class _1, class _2, class _3,
            class _4, class _5, class _6, class _7, class _8, class _9, class _10,
            class _11, class _12, class _13, class _14, class _15, class r1 = F<r0, _1>,
            class r2 = F<r1, _2>, class r3 = F<r2, _3>, class r4 = F<r3, _4>,
            class r5 = F<r4, _5>, class r6 = F<r5, _6>, class r7 = F<r6, _7>,
            class r8 = F<r7, _8>, class r9 = F<r8, _9>, class r10 = F<r9, _10>,
            class r11 = F<r10, _11>, class r12 = F<r11, _12>, class r13 = F<r12, _13>,
            class r14 = F<r13, _14>>
            using g = scan_continuation<F<r14, _15>, list<
            r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11, r12,
            r13, r14>>;

        template<
            unsigned remaining, class C, template<class...> class F, class _0,
            class _1, class _2, class _3, class _4, class _5, class _6, class _7,
            class _8, class _9, class _10, class _11, class _12, class _13,
            class _14, class _15,
            class... xs>
            using f = typename g<
            F, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15
            >::template f<remaining - 15,
            _scan_left<n_8_or_more_16_32_64_128_256(remaining - 15)>,
            C, F, xs...>;
    };

    template<>
    struct _scan_left<64>
    {
        template<
            template<class...> class F, class r0, class _1, class _2, class _3,
            class _4, class _5, class _6, class _7, class _8, class _9, class _10,
            class _11, class _12, class _13, class _14, class _15, class _16,
            class _17, class _18, class _19, class _20, class _21, class _22,
            class _23, class _24, class _25, class _26, class _27, class _28,
            class _29, class _30, class _31, class r1 = F<r0, _1>,
            class r2 = F<r1, _2>, class r3 = F<r2, _3>, class r4 = F<r3, _4>,
            class r5 = F<r4, _5>, class r6 = F<r5, _6>, class r7 = F<r6, _7>,
            class r8 = F<r7, _8>, class r9 = F<r8, _9>, class r10 = F<r9, _10>,
            class r11 = F<r10, _11>, class r12 = F<r11, _12>, class r13 = F<r12, _13>,
            class r14 = F<r13, _14>, class r15 = F<r14, _15>, class r16 = F<r15, _16>,
            class r17 = F<r16, _17>, class r18 = F<r17, _18>, class r19 = F<r18, _19>,
            class r20 = F<r19, _20>, class r21 = F<r20, _21>, class r22 = F<r21, _22>,
            class r23 = F<r22, _23>, class r24 = F<r23, _24>, class r25 = F<r24, _25>,
            class r26 = F<r25, _26>, class r27 = F<r26, _27>, class r28 = F<r27, _28>,
            class r29 = F<r28, _29>, class r30 = F<r29, _30>>
            using g = scan_continuation<F<r30, _31>, list<
            r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11, r12,
            r13, r14, r15, r16, r17, r18, r19, r20, r21, r22, r23,
            r24, r25, r26, r27, r28, r29, r30>>;

        template<
            unsigned remaining, class C, template<class...> class F, class _0,
            class _1, class _2, class _3, class _4, class _5, class _6, class _7,
            class _8, class _9, class _10, class _11, class _12, class _13,
            class _14, class _15, class _16, class _17, class _18, class _19,
            class _20, class _21, class _22, class _23, class _24, class _25,
            class _26, class _27, class _28, class _29, class _30, class _31,
            class... xs>
            using f = typename g<
            F, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15,
            _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30,
            _31
            >::template f<remaining - 31,
            _scan_left<n_8_or_more_16_32_64_128_256(remaining - 31)>,
            C, F, xs...>;
    };

    template<>
    struct _scan_left<128>
    {
        template<
            template<class...> class F, class r0, class _1, class _2, class _3,
            class _4, class _5, class _6, class _7, class _8, class _9, class _10,
            class _11, class _12, class _13, class _14, class _15, class _16,
            class _17, class _18, class _19, class _20, class _21, class _22,
            class _23, class _24, class _25, class _26, class _27, class _28,
            class _29, class _30, class _31, class _32, class _33, class _34,
            class _35, class _36, class _37, class _38, class _39, class _40,
            class _41, class _42, class _43, class _44, class _45, class _46,
            class _47, class _48, class _49, class _50, class _51, class _52,
            class _53, class _54, class _55, class _56, class _57, class _58,
            class _59, class _60, class _61, class _62, class _63,
            class r1 = F<r0, _1>, class r2 = F<r1, _2>, class r3 = F<r2, _3>,
            class r4 = F<r3, _4>, class r5 = F<r4, _5>, class r6 = F<r5, _6>,
            class r7 = F<r6, _7>, class r8 = F<r7, _8>, class r9 = F<r8, _9>,
            class r10 = F<r9, _10>, class r11 = F<r10, _11>, class r12 = F<r11, _12>,
            class r13 = F<r12, _13>, class r14 = F<r13, _14>, class r15 = F<r14, _15>,
            class r16 = F<r15, _16>, class r17 = F<r16, _17>, class r18 = F<r17, _18>,
            class r19 = F<r18, _19>, class r20 = F<r19, _20>, class r21 = F<r20, _21>,
            class r22 = F<r21, _22>, class r23 = F<r22, _23>, class r24 = F<r23, _24>,
            class r25 = F<r24, _25>, class r26 = F<r25, _26>, class r27 = F<r26, _27>,
            class r28 = F<r27, _28>, class r29 = F<r28, _29>, class r30 = F<r29, _30>,
            class r31 = F<r30, _31>, class r32 = F<r31, _32>, class r33 = F<r32, _33>,
            class r34 = F<r33, _34>, class r35 = F<r34, _35>, class r36 = F<r35, _36>,
            class r37 = F<r36, _37>, class r38 = F<r37, _38>, class r39 = F<r38, _39>,
            class r40 = F<r39, _40>, class r41 = F<r40, _41>, class r42 = F<r41, _42>,
            class r43 = F<r42, _43>, class r44 = F<r43, _44>, class r45 = F<r44, _45>,
            class r46 = F<r45, _46>, class r47 = F<r46, _47>, class r48 = F<r47, _48>,
            class r49 = F<r48, _49>, class r50 = F<r49, _50>, class r51 = F<r50, _51>,
            class r52 = F<r51, _52>, class r53 = F<r52, _53>, class r54 = F<r53, _54>,
            class r55 = F<r54, _55>, class r56 = F<r55, _56>, class r57 = F<r56, _57>,
            class r58 = F<r57, _58>, class r59 = F<r58, _59>, class r60 = F<r59, _60>,
            class r61 = F<r60, _61>, class r62 = F<r61, _62 >>
            using g = scan_continuation<F<r62, _63>, list<
            r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11, r12, r13, r14, r15,
            r16, r17, r18, r19, r20, r21, r22, r23, r24, r25, r26, r27, r28, r29,
            r30, r31, r32, r33, r34, r35, r36, r37, r38, r39, r40, r41, r42, r43,
            r44, r45, r46, r47, r48, r49, r50, r51, r52, r53, r54, r55, r56, r57,
            r58, r59, r60, r61, r62>>;

        template<
            unsigned remaining, class C, template<class...> class F, class _0,
            class _1, class _2, class _3, class _4, class _5, class _6, class _7,
            class _8, class _9, class _10, class _11, class _12, class _13,
            class _14, class _15, class _16, class _17, class _18, class _19,
            class _20, class _21, class _22, class _23, class _24, class _25,
            class _26, class _27, class _28, class _29, class _30, class _31,
            class _32, class _33, class _34, class _35, class _36, class _37,
            class _38, class _39, class _40, class _41, class _42, class _43,
            class _44, class _45, class _46, class _47, class _48, class _49,
            class _50, class _51, class _52, class _53, class _54, class _55,
            class _56, class _57, class _58, class _59, class _60, class _61,
            class _62, class _63, class... xs>
            using f = typename g<
            F, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15,
            _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30,
            _31, _32, _33, _34, _35, _36, _37, _38, _39, _40, _41, _42, _43, _44, _45,
            _46, _47, _48, _49, _50, _51, _52, _53, _54, _55, _56, _57, _58, _59, _60,
            _61, _62,
            _63>::template f<remaining - 63,
            _scan_left<n_8_or_more_16_32_64_128_256(remaining - 63)>,
            C, F, xs...>;
    };

    template<>
    struct _scan_left<256>
    {
        template<
            template<class...> class F, class r0, class _1, class _2, class _3,
            class _4, class _5, class _6, class _7, class _8, class _9, class _10,
            class _11, class _12, class _13, class _14, class _15, class _16,
            class _17, class _18, class _19, class _20, class _21, class _22,
            class _23, class _24, class _25, class _26, class _27, class _28,
            class _29, class _30, class _31, class _32, class _33, class _34,
            class _35, class _36, class _37, class _38, class _39, class _40,
            class _41, class _42, class _43, class _44, class _45, class _46,
            class _47, class _48, class _49, class _50, class _51, class _52,
            class _53, class _54, class _55, class _56, class _57, class _58,
            class _59, class _60, class _61, class _62, class _63, class _64,
            class _65, class _66, class _67, class _68, class _69, class _70,
            class _71, class _72, class _73, class _74, class _75, class _76,
            class _77, class _78, class _79, class _80, class _81, class _82,
            class _83, class _84, class _85, class _86, class _87, class _88,
            class _89, class _90, class _91, class _92, class _93, class _94,
            class _95, class _96, class _97, class _98, class _99, class _100,
            class _101, class _102, class _103, class _104, class _105, class _106,
            class _107, class _108, class _109, class _110, class _111, class _112,
            class _113, class _114, class _115, class _116, class _117, class _118,
            class _119, class _120, class _121, class _122, class _123, class _124,
            class _125, class _126, class _127, class r1 = F<r0, _1>,
            class r2 = F<r1, _2>, class r3 = F<r2, _3>, class r4 = F<r3, _4>,
            class r5 = F<r4, _5>, class r6 = F<r5, _6>, class r7 = F<r6, _7>,
            class r8 = F<r7, _8>, class r9 = F<r8, _9>, class r10 = F<r9, _10>,
            class r11 = F<r10, _11>, class r12 = F<r11, _12>, class r13 = F<r12, _13>,
            class r14 = F<r13, _14>, class r15 = F<r14, _15>, class r16 = F<r15, _16>,
            class r17 = F<r16, _17>, class r18 = F<r17, _18>, class r19 = F<r18, _19>,
            class r20 = F<r19, _20>, class r21 = F<r20, _21>, class r22 = F<r21, _22>,
            class r23 = F<r22, _23>, class r24 = F<r23, _24>, class r25 = F<r24, _25>,
            class r26 = F<r25, _26>, class r27 = F<r26, _27>, class r28 = F<r27, _28>,
            class r29 = F<r28, _29>, class r30 = F<r29, _30>, class r31 = F<r30, _31>,
            class r32 = F<r31, _32>, class r33 = F<r32, _33>, class r34 = F<r33, _34>,
            class r35 = F<r34, _35>, class r36 = F<r35, _36>, class r37 = F<r36, _37>,
            class r38 = F<r37, _38>, class r39 = F<r38, _39>, class r40 = F<r39, _40>,
            class r41 = F<r40, _41>, class r42 = F<r41, _42>, class r43 = F<r42, _43>,
            class r44 = F<r43, _44>, class r45 = F<r44, _45>, class r46 = F<r45, _46>,
            class r47 = F<r46, _47>, class r48 = F<r47, _48>, class r49 = F<r48, _49>,
            class r50 = F<r49, _50>, class r51 = F<r50, _51>, class r52 = F<r51, _52>,
            class r53 = F<r52, _53>, class r54 = F<r53, _54>, class r55 = F<r54, _55>,
            class r56 = F<r55, _56>, class r57 = F<r56, _57>, class r58 = F<r57, _58>,
            class r59 = F<r58, _59>, class r60 = F<r59, _60>, class r61 = F<r60, _61>,
            class r62 = F<r61, _62>, class r63 = F<r62, _63>, class r64 = F<r63, _64>,
            class r65 = F<r64, _65>, class r66 = F<r65, _66>, class r67 = F<r66, _67>,
            class r68 = F<r67, _68>, class r69 = F<r68, _69>, class r70 = F<r69, _70>,
            class r71 = F<r70, _71>, class r72 = F<r71, _72>, class r73 = F<r72, _73>,
            class r74 = F<r73, _74>, class r75 = F<r74, _75>, class r76 = F<r75, _76>,
            class r77 = F<r76, _77>, class r78 = F<r77, _78>, class r79 = F<r78, _79>,
            class r80 = F<r79, _80>, class r81 = F<r80, _81>, class r82 = F<r81, _82>,
            class r83 = F<r82, _83>, class r84 = F<r83, _84>, class r85 = F<r84, _85>,
            class r86 = F<r85, _86>, class r87 = F<r86, _87>, class r88 = F<r87, _88>,
            class r89 = F<r88, _89>, class r90 = F<r89, _90>, class r91 = F<r90, _91>,
            class r92 = F<r91, _92>, class r93 = F<r92, _93>, class r94 = F<r93, _94>,
            class r95 = F<r94, _95>, class r96 = F<r95, _96>, class r97 = F<r96, _97>,
            class r98 = F<r97, _98>, class r99 = F<r98, _99>,
            class r100 = F<r99, _100>, class r101 = F<r100, _101>,
            class r102 = F<r101, _102>, class r103 = F<r102, _103>,
            class r104 = F<r103, _104>, class r105 = F<r104, _105>,
            class r106 = F<r105, _106>, class r107 = F<r106, _107>,
            class r108 = F<r107, _108>, class r109 = F<r108, _109>,
            class r110 = F<r109, _110>, class r111 = F<r110, _111>,
            class r112 = F<r111, _112>, class r113 = F<r112, _113>,
            class r114 = F<r113, _114>, class r115 = F<r114, _115>,
            class r116 = F<r115, _116>, class r117 = F<r116, _117>,
            class r118 = F<r117, _118>, class r119 = F<r118, _119>,
            class r120 = F<r119, _120>, class r121 = F<r120, _121>,
            class r122 = F<r121, _122>, class r123 = F<r122, _123>,
            class r124 = F<r123, _124>, class r125 = F<r124, _125>,
            class r126 = F<r125, _126 >>
            using g = scan_continuation<F<r126, _127>, list<
            r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11, r12, r13, r14, r15,
            r16, r17, r18, r19, r20, r21, r22, r23, r24, r25, r26, r27, r28, r29,
            r30, r31, r32, r33, r34, r35, r36, r37, r38, r39, r40, r41, r42, r43,
            r44, r45, r46, r47, r48, r49, r50, r51, r52, r53, r54, r55, r56, r57,
            r58, r59, r60, r61, r62, r63, r64, r65, r66, r67, r68, r69, r70, r71,
            r72, r73, r74, r75, r76, r77, r78, r79, r80, r81, r82, r83, r84, r85,
            r86, r87, r88, r89, r90, r91, r92, r93, r94, r95, r96, r97, r98, r99,
            r100, r101, r102, r103, r104, r105, r106, r107, r108, r109, r110,
            r111, r112, r113, r114, r115, r116, r117, r118, r119, r120, r121,
            r122, r123, r124, r125, r126>>;

        template<
            unsigned remaining, class C, template<class...> class F, class _0,
            class _1, class _2, class _3, class _4, class _5, class _6, class _7,
            class _8, class _9, class _10, class _11, class _12, class _13,
            class _14, class _15, class _16, class _17, class _18, class _19,
            class _20, class _21, class _22, class _23, class _24, class _25,
            class _26, class _27, class _28, class _29, class _30, class _31,
            class _32, class _33, class _34, class _35, class _36, class _37,
            class _38, class _39, class _40, class _41, class _42, class _43,
            class _44, class _45, class _46, class _47, class _48, class _49,
            class _50, class _51, class _52, class _53, class _54, class _55,
            class _56, class _57, class _58, class _59, class _60, class _61,
            class _62, class _63, class _64, class _65, class _66, class _67,
            class _68, class _69, class _70, class _71, class _72, class _73,
            class _74, class _75, class _76, class _77, class _78, class _79,
            class _80, class _81, class _82, class _83, class _84, class _85,
            class _86, class _87, class _88, class _89, class _90, class _91,
            class _92, class _93, class _94, class _95, class _96, class _97,
            class _98, class _99, class _100, class _101, class _102, class _103,
            class _104, class _105, class _106, class _107, class _108, class _109,
            class _110, class _111, class _112, class _113, class _114, class _115,
            class _116, class _117, class _118, class _119, class _120, class _121,
            class _122, class _123, class _124, class _125, class _126, class _127,
            class... xs >
            using f = typename g<
            F, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15,
            _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30,
            _31, _32, _33, _34, _35, _36, _37, _38, _39, _40, _41, _42, _43, _44, _45,
            _46, _47, _48, _49, _50, _51, _52, _53, _54, _55, _56, _57, _58, _59, _60,
            _61, _62, _63, _64, _65, _66, _67, _68, _69, _70, _71, _72, _73, _74, _75,
            _76, _77, _78, _79, _80, _81, _82, _83, _84, _85, _86, _87, _88, _89, _90,
            _91, _92, _93, _94, _95, _96, _97, _98, _99, _100, _101, _102, _103, _104,
            _105, _106, _107, _108, _109, _110, _111, _112, _113, _114, _115, _116,
            _117, _118, _119, _120, _121, _122, _123, _124, _125, _126, _127>::
            template f<remaining - 127,
            _scan_left<n_8_or_more_16_32_64_128_256(remaining - 127)>,
            C, F, xs...>;
    };
}
/// \endcond
namespace jln::mp
{
    /// \cond
    namespace detail
    {
        template<class F, class C>
        struct scan_right_impl;
    }
    /// \endcond

    /// \ingroup algorithm

    /// Fold a \sequence to the right and return a list containing the successive reduction states.
    /// \semantics
    ///   Equivalent to
    ///   \code
    ///   C::f<
    ///     ...
    ///     F::f<xs[n-3], F::f<xs[n-2], xs[n-1]>>,
    ///     F::f<xs[n-2], xs[n-1]>,
    ///     xs[n-1],
    ///   >
    ///   \endcode
    /// \treturn \sequence
    template<class F, class C = listify>
    using scan_right = reverse<detail::scan_right_impl<F, C>>;

    namespace emp
    {
        template<class L, class F, class C = mp::listify>
        using scan_right = unpack<L, mp::scan_right<F, C>>;
    }
}


// scan_continuation

/// \cond
namespace jln::mp::detail
{
    template<unsigned>
    struct _scan_right;

    template<class F, class C>
    struct scan_right_impl
    {
        template<class... xs>
        using f = typename _scan_right<
            detail::n_8_or_more_16_32_64_128_256(sizeof...(xs))
        >::template f<sizeof...(xs), C, F::template f, xs...>;
    };

    template<>
    struct _scan_right<0>
    {
        template<unsigned remaining, class C, template<class...> class F,
            class... xs>
            using f = typename join<C>::template f<xs...>;
    };

    template<>
    struct _scan_right<1>
    {
        template<unsigned remaining, class C, template<class...> class F,
            class _0, class... xs>
            using f = typename join<C>::template f<list<_0>, xs...>;
    };

    template<>
    struct _scan_right<2>
    {
        template<unsigned remaining, class C, template<class...> class F,
            class _0, class _1, class... xs>
            using f = typename join<C>::template f<list<F<_1, _0>, _0>, xs...>;
    };

    // for n in range(3,9):
    //   args=', '.join(f'class _{i}' for i in range(1,n))
    //   ps=', '.join(f'_{i}' for i in range(n))
    //   rs=', '.join(f'class r{i} = F<_{i}, r{i-1}>' for i in range(1, n-1))
    //   l=', '.join(f'r{i}' for i in range(n-2, -1, -1))
    //   sep=', ' if n > 2 else ''
    //   print(f'''
    //   template<>
    //   struct _scan_right<{n}>
    //   {{
    //     template<template<class...> class F,
    //       class r0, {args}{sep}{rs}>
    //     using g = list<F<_{n-1}, r{n-2}>, {l}>;
    //
    //     template<unsigned remaining, class C, template<class...> class F,
    //       class _0, {args}, class... xs>
    //     using f = typename join<C>::template f<g<F, {ps}>, xs...>;
    //   }};''')

    template<>
    struct _scan_right<3>
    {
        template<template<class...> class F, class r0, class _1, class _2,
            class r1 = F<_1, r0>>
            using g = list<F<_2, r1>, r1, r0>;

        template<unsigned remaining, class C, template<class...> class F, class _0,
            class _1, class _2, class... xs>
            using f = typename join<C>::template f<xs..., g<F, _0, _1, _2>>;
    };

    template<>
    struct _scan_right<4>
    {
        template<template<class...> class F, class r0, class _1, class _2, class _3,
            class r1 = F<_1, r0>, class r2 = F<_2, r1>>
            using g = list<F<_3, r2>, r2, r1, r0>;

        template<unsigned remaining, class C, template<class...> class F, class _0,
            class _1, class _2, class _3, class... xs>
            using f = typename join<C>::template f<g<F, _0, _1, _2, _3>, xs...>;
    };

    template<>
    struct _scan_right<5>
    {
        template<template<class...> class F, class r0, class _1, class _2, class _3,
            class _4, class r1 = F<_1, r0>, class r2 = F<_2, r1>,
            class r3 = F<_3, r2>>
            using g = list<F<_4, r3>, r3, r2, r1, r0>;

        template<unsigned remaining, class C, template<class...> class F, class _0,
            class _1, class _2, class _3, class _4, class... xs>
            using f = typename join<C>::template f<g<F, _0, _1, _2, _3, _4>, xs...>;
    };

    template<>
    struct _scan_right<6>
    {
        template<template<class...> class F, class r0, class _1, class _2, class _3,
            class _4, class _5, class r1 = F<_1, r0>, class r2 = F<_2, r1>,
            class r3 = F<_3, r2>, class r4 = F<_4, r3>>
            using g = list<F<_5, r4>, r4, r3, r2, r1, r0>;

        template<unsigned remaining, class C, template<class...> class F, class _0,
            class _1, class _2, class _3, class _4, class _5, class... xs>
            using f = typename join<C>::template f<g<F, _0, _1, _2, _3, _4, _5>, xs...>;
    };

    template<>
    struct _scan_right<7>
    {
        template<template<class...> class F, class r0, class _1, class _2, class _3,
            class _4, class _5, class _6, class r1 = F<_1, r0>,
            class r2 = F<_2, r1>, class r3 = F<_3, r2>, class r4 = F<_4, r3>,
            class r5 = F<_5, r4>>
            using g = list<F<_6, r5>, r5, r4, r3, r2, r1, r0>;

        template<unsigned remaining, class C, template<class...> class F, class _0,
            class _1, class _2, class _3, class _4, class _5, class _6,
            class... xs>
            using f = typename join<C>::template f<g<F, _0, _1, _2, _3, _4, _5, _6>, xs...>;
    };

    template<>
    struct _scan_right<8>
    {
        template<template<class...> class F, class r0, class _1, class _2, class _3,
            class _4, class _5, class _6, class _7, class r1 = F<_1, r0>,
            class r2 = F<_2, r1>, class r3 = F<_3, r2>, class r4 = F<_4, r3>,
            class r5 = F<_5, r4>, class r6 = F<_6, r5>>
            using g = list<F<_7, r6>, r6, r5, r4, r3, r2, r1, r0>;

        template<unsigned remaining, class C, template<class...> class F, class _0,
            class _1, class _2, class _3, class _4, class _5, class _6,
            class _7, class... xs>
            using f = typename join<C>::template f<g<F, _0, _1, _2, _3, _4, _5, _6, _7>, xs...>;
    };

    // for n in [8,16,32,64,128]:
    //   args=', '.join(f'class _{i}' for i in range(1, n))
    //   ps=', '.join(f'_{i}' for i in range(n))
    //   rs=', '.join(f'class r{i} = F<_{i}, r{i-1}>' for i in range(1, n-1))
    //   l=', '.join(f'r{i}' for i in range(n-2,-1,-1))
    //   print(f'''
    //   template<>
    //   struct _scan_right<{n*2}>
    //   {{
    //     template<template<class...> class F,
    //       class r0, {args},
    //       {rs}>
    //     using g = scan_continuation<F<_{n-1}, r{n-2}>, list<{l}>>;
    //
    //     template<unsigned remaining, class C, template<class...> class F,
    //       class _0, {args},
    //       class... xs>
    //     using f = typename g<F, {ps}>
    //       ::template f<remaining-{n-1}, _scan_right<n_8_or_more_16_32_64_128_256(remaining-{n-1})>, C, F, xs..
    // .>;
    //   }};''')

    template<> struct _scan_right<16>
    {
        template<template<class...> class F, class r0, class _1, class _2, class _3,
            class _4, class _5, class _6, class _7, class r1 = F<_1, r0>,
            class r2 = F<_2, r1>, class r3 = F<_3, r2>, class r4 = F<_4, r3>,
            class r5 = F<_5, r4>, class r6 = F<_6, r5>>
            using g = scan_continuation<F<_7, r6>, list<r6, r5, r4, r3, r2, r1, r0>>;

        template<unsigned remaining, class C, template<class...> class F, class _0,
            class _1, class _2, class _3, class _4, class _5, class _6,
            class _7, class... xs>
            using f = typename g<F, _0, _1, _2, _3, _4, _5, _6, _7>::template f<
            remaining - 7, _scan_right<n_8_or_more_16_32_64_128_256(remaining - 7)>,
            C, F, xs...>;
    };

    template<> struct _scan_right<32> {
        template<template<class...> class F, class r0, class _1, class _2, class _3,
            class _4, class _5, class _6, class _7, class _8, class _9,
            class _10, class _11, class _12, class _13, class _14, class _15,
            class r1 = F<_1, r0>, class r2 = F<_2, r1>, class r3 = F<_3, r2>,
            class r4 = F<_4, r3>, class r5 = F<_5, r4>, class r6 = F<_6, r5>,
            class r7 = F<_7, r6>, class r8 = F<_8, r7>, class r9 = F<_9, r8>,
            class r10 = F<_10, r9>, class r11 = F<_11, r10>,
            class r12 = F<_12, r11>, class r13 = F<_13, r12>,
            class r14 = F<_14, r13>>
            using g =
            scan_continuation<F<_15, r14>, list<r14, r13, r12, r11, r10, r9, r8, r7,
            r6, r5, r4, r3, r2, r1, r0>>;

        template<unsigned remaining, class C, template<class...> class F, class _0,
            class _1, class _2, class _3, class _4, class _5, class _6,
            class _7, class _8, class _9, class _10, class _11, class _12,
            class _13, class _14, class _15, class... xs>
            using f = typename g<F, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12,
            _13, _14, _15>::
            template f<remaining - 15,
            _scan_right<n_8_or_more_16_32_64_128_256(remaining - 15)>, C,
            F, xs...>;
    };

    template<> struct _scan_right<64> {
        template<
            template<class...> class F, class r0, class _1, class _2, class _3,
            class _4, class _5, class _6, class _7, class _8, class _9, class _10,
            class _11, class _12, class _13, class _14, class _15, class _16,
            class _17, class _18, class _19, class _20, class _21, class _22,
            class _23, class _24, class _25, class _26, class _27, class _28,
            class _29, class _30, class _31, class r1 = F<_1, r0>,
            class r2 = F<_2, r1>, class r3 = F<_3, r2>, class r4 = F<_4, r3>,
            class r5 = F<_5, r4>, class r6 = F<_6, r5>, class r7 = F<_7, r6>,
            class r8 = F<_8, r7>, class r9 = F<_9, r8>, class r10 = F<_10, r9>,
            class r11 = F<_11, r10>, class r12 = F<_12, r11>, class r13 = F<_13, r12>,
            class r14 = F<_14, r13>, class r15 = F<_15, r14>, class r16 = F<_16, r15>,
            class r17 = F<_17, r16>, class r18 = F<_18, r17>, class r19 = F<_19, r18>,
            class r20 = F<_20, r19>, class r21 = F<_21, r20>, class r22 = F<_22, r21>,
            class r23 = F<_23, r22>, class r24 = F<_24, r23>, class r25 = F<_25, r24>,
            class r26 = F<_26, r25>, class r27 = F<_27, r26>, class r28 = F<_28, r27>,
            class r29 = F<_29, r28>, class r30 = F<_30, r29>>
            using g =
            scan_continuation<F<_31, r30>,
            list<r30, r29, r28, r27, r26, r25, r24, r23, r22, r21,
            r20, r19, r18, r17, r16, r15, r14, r13, r12, r11,
            r10, r9, r8, r7, r6, r5, r4, r3, r2, r1, r0>>;

        template<unsigned remaining, class C, template<class...> class F, class _0,
            class _1, class _2, class _3, class _4, class _5, class _6,
            class _7, class _8, class _9, class _10, class _11, class _12,
            class _13, class _14, class _15, class _16, class _17, class _18,
            class _19, class _20, class _21, class _22, class _23, class _24,
            class _25, class _26, class _27, class _28, class _29, class _30,
            class _31, class... xs>
            using f = typename g<F, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12,
            _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23,
            _24, _25, _26, _27, _28, _29, _30, _31>::
            template f<remaining - 31,
            _scan_right<n_8_or_more_16_32_64_128_256(remaining - 31)>, C,
            F, xs...>;
    };

    template<> struct _scan_right<128> {
        template<
            template<class...> class F, class r0, class _1, class _2, class _3,
            class _4, class _5, class _6, class _7, class _8, class _9, class _10,
            class _11, class _12, class _13, class _14, class _15, class _16,
            class _17, class _18, class _19, class _20, class _21, class _22,
            class _23, class _24, class _25, class _26, class _27, class _28,
            class _29, class _30, class _31, class _32, class _33, class _34,
            class _35, class _36, class _37, class _38, class _39, class _40,
            class _41, class _42, class _43, class _44, class _45, class _46,
            class _47, class _48, class _49, class _50, class _51, class _52,
            class _53, class _54, class _55, class _56, class _57, class _58,
            class _59, class _60, class _61, class _62, class _63,
            class r1 = F<_1, r0>, class r2 = F<_2, r1>, class r3 = F<_3, r2>,
            class r4 = F<_4, r3>, class r5 = F<_5, r4>, class r6 = F<_6, r5>,
            class r7 = F<_7, r6>, class r8 = F<_8, r7>, class r9 = F<_9, r8>,
            class r10 = F<_10, r9>, class r11 = F<_11, r10>, class r12 = F<_12, r11>,
            class r13 = F<_13, r12>, class r14 = F<_14, r13>, class r15 = F<_15, r14>,
            class r16 = F<_16, r15>, class r17 = F<_17, r16>, class r18 = F<_18, r17>,
            class r19 = F<_19, r18>, class r20 = F<_20, r19>, class r21 = F<_21, r20>,
            class r22 = F<_22, r21>, class r23 = F<_23, r22>, class r24 = F<_24, r23>,
            class r25 = F<_25, r24>, class r26 = F<_26, r25>, class r27 = F<_27, r26>,
            class r28 = F<_28, r27>, class r29 = F<_29, r28>, class r30 = F<_30, r29>,
            class r31 = F<_31, r30>, class r32 = F<_32, r31>, class r33 = F<_33, r32>,
            class r34 = F<_34, r33>, class r35 = F<_35, r34>, class r36 = F<_36, r35>,
            class r37 = F<_37, r36>, class r38 = F<_38, r37>, class r39 = F<_39, r38>,
            class r40 = F<_40, r39>, class r41 = F<_41, r40>, class r42 = F<_42, r41>,
            class r43 = F<_43, r42>, class r44 = F<_44, r43>, class r45 = F<_45, r44>,
            class r46 = F<_46, r45>, class r47 = F<_47, r46>, class r48 = F<_48, r47>,
            class r49 = F<_49, r48>, class r50 = F<_50, r49>, class r51 = F<_51, r50>,
            class r52 = F<_52, r51>, class r53 = F<_53, r52>, class r54 = F<_54, r53>,
            class r55 = F<_55, r54>, class r56 = F<_56, r55>, class r57 = F<_57, r56>,
            class r58 = F<_58, r57>, class r59 = F<_59, r58>, class r60 = F<_60, r59>,
            class r61 = F<_61, r60>, class r62 = F<_62, r61 >>
            using g = scan_continuation<
            F<_63, r62>,
            list<r62, r61, r60, r59, r58, r57, r56, r55, r54, r53, r52, r51, r50, r49,
            r48, r47, r46, r45, r44, r43, r42, r41, r40, r39, r38, r37, r36, r35,
            r34, r33, r32, r31, r30, r29, r28, r27, r26, r25, r24, r23, r22, r21,
            r20, r19, r18, r17, r16, r15, r14, r13, r12, r11, r10, r9, r8, r7,
            r6, r5, r4, r3, r2, r1, r0>>;

        template<unsigned remaining, class C, template<class...> class F, class _0,
            class _1, class _2, class _3, class _4, class _5, class _6,
            class _7, class _8, class _9, class _10, class _11, class _12,
            class _13, class _14, class _15, class _16, class _17, class _18,
            class _19, class _20, class _21, class _22, class _23, class _24,
            class _25, class _26, class _27, class _28, class _29, class _30,
            class _31, class _32, class _33, class _34, class _35, class _36,
            class _37, class _38, class _39, class _40, class _41, class _42,
            class _43, class _44, class _45, class _46, class _47, class _48,
            class _49, class _50, class _51, class _52, class _53, class _54,
            class _55, class _56, class _57, class _58, class _59, class _60,
            class _61, class _62, class _63, class... xs>
            using f = typename g<F, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12,
            _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23,
            _24, _25, _26, _27, _28, _29, _30, _31, _32, _33, _34,
            _35, _36, _37, _38, _39, _40, _41, _42, _43, _44, _45,
            _46, _47, _48, _49, _50, _51, _52, _53, _54, _55, _56,
            _57, _58, _59, _60, _61, _62, _63>::
            template f<remaining - 63,
            _scan_right<n_8_or_more_16_32_64_128_256(remaining - 63)>, C,
            F, xs...>;
    };

    template<> struct _scan_right<256> {
        template<
            template<class...> class F, class r0, class _1, class _2, class _3,
            class _4, class _5, class _6, class _7, class _8, class _9, class _10,
            class _11, class _12, class _13, class _14, class _15, class _16,
            class _17, class _18, class _19, class _20, class _21, class _22,
            class _23, class _24, class _25, class _26, class _27, class _28,
            class _29, class _30, class _31, class _32, class _33, class _34,
            class _35, class _36, class _37, class _38, class _39, class _40,
            class _41, class _42, class _43, class _44, class _45, class _46,
            class _47, class _48, class _49, class _50, class _51, class _52,
            class _53, class _54, class _55, class _56, class _57, class _58,
            class _59, class _60, class _61, class _62, class _63, class _64,
            class _65, class _66, class _67, class _68, class _69, class _70,
            class _71, class _72, class _73, class _74, class _75, class _76,
            class _77, class _78, class _79, class _80, class _81, class _82,
            class _83, class _84, class _85, class _86, class _87, class _88,
            class _89, class _90, class _91, class _92, class _93, class _94,
            class _95, class _96, class _97, class _98, class _99, class _100,
            class _101, class _102, class _103, class _104, class _105, class _106,
            class _107, class _108, class _109, class _110, class _111, class _112,
            class _113, class _114, class _115, class _116, class _117, class _118,
            class _119, class _120, class _121, class _122, class _123, class _124,
            class _125, class _126, class _127, class r1 = F<_1, r0>,
            class r2 = F<_2, r1>, class r3 = F<_3, r2>, class r4 = F<_4, r3>,
            class r5 = F<_5, r4>, class r6 = F<_6, r5>, class r7 = F<_7, r6>,
            class r8 = F<_8, r7>, class r9 = F<_9, r8>, class r10 = F<_10, r9>,
            class r11 = F<_11, r10>, class r12 = F<_12, r11>, class r13 = F<_13, r12>,
            class r14 = F<_14, r13>, class r15 = F<_15, r14>, class r16 = F<_16, r15>,
            class r17 = F<_17, r16>, class r18 = F<_18, r17>, class r19 = F<_19, r18>,
            class r20 = F<_20, r19>, class r21 = F<_21, r20>, class r22 = F<_22, r21>,
            class r23 = F<_23, r22>, class r24 = F<_24, r23>, class r25 = F<_25, r24>,
            class r26 = F<_26, r25>, class r27 = F<_27, r26>, class r28 = F<_28, r27>,
            class r29 = F<_29, r28>, class r30 = F<_30, r29>, class r31 = F<_31, r30>,
            class r32 = F<_32, r31>, class r33 = F<_33, r32>, class r34 = F<_34, r33>,
            class r35 = F<_35, r34>, class r36 = F<_36, r35>, class r37 = F<_37, r36>,
            class r38 = F<_38, r37>, class r39 = F<_39, r38>, class r40 = F<_40, r39>,
            class r41 = F<_41, r40>, class r42 = F<_42, r41>, class r43 = F<_43, r42>,
            class r44 = F<_44, r43>, class r45 = F<_45, r44>, class r46 = F<_46, r45>,
            class r47 = F<_47, r46>, class r48 = F<_48, r47>, class r49 = F<_49, r48>,
            class r50 = F<_50, r49>, class r51 = F<_51, r50>, class r52 = F<_52, r51>,
            class r53 = F<_53, r52>, class r54 = F<_54, r53>, class r55 = F<_55, r54>,
            class r56 = F<_56, r55>, class r57 = F<_57, r56>, class r58 = F<_58, r57>,
            class r59 = F<_59, r58>, class r60 = F<_60, r59>, class r61 = F<_61, r60>,
            class r62 = F<_62, r61>, class r63 = F<_63, r62>, class r64 = F<_64, r63>,
            class r65 = F<_65, r64>, class r66 = F<_66, r65>, class r67 = F<_67, r66>,
            class r68 = F<_68, r67>, class r69 = F<_69, r68>, class r70 = F<_70, r69>,
            class r71 = F<_71, r70>, class r72 = F<_72, r71>, class r73 = F<_73, r72>,
            class r74 = F<_74, r73>, class r75 = F<_75, r74>, class r76 = F<_76, r75>,
            class r77 = F<_77, r76>, class r78 = F<_78, r77>, class r79 = F<_79, r78>,
            class r80 = F<_80, r79>, class r81 = F<_81, r80>, class r82 = F<_82, r81>,
            class r83 = F<_83, r82>, class r84 = F<_84, r83>, class r85 = F<_85, r84>,
            class r86 = F<_86, r85>, class r87 = F<_87, r86>, class r88 = F<_88, r87>,
            class r89 = F<_89, r88>, class r90 = F<_90, r89>, class r91 = F<_91, r90>,
            class r92 = F<_92, r91>, class r93 = F<_93, r92>, class r94 = F<_94, r93>,
            class r95 = F<_95, r94>, class r96 = F<_96, r95>, class r97 = F<_97, r96>,
            class r98 = F<_98, r97>, class r99 = F<_99, r98>,
            class r100 = F<_100, r99>, class r101 = F<_101, r100>,
            class r102 = F<_102, r101>, class r103 = F<_103, r102>,
            class r104 = F<_104, r103>, class r105 = F<_105, r104>,
            class r106 = F<_106, r105>, class r107 = F<_107, r106>,
            class r108 = F<_108, r107>, class r109 = F<_109, r108>,
            class r110 = F<_110, r109>, class r111 = F<_111, r110>,
            class r112 = F<_112, r111>, class r113 = F<_113, r112>,
            class r114 = F<_114, r113>, class r115 = F<_115, r114>,
            class r116 = F<_116, r115>, class r117 = F<_117, r116>,
            class r118 = F<_118, r117>, class r119 = F<_119, r118>,
            class r120 = F<_120, r119>, class r121 = F<_121, r120>,
            class r122 = F<_122, r121>, class r123 = F<_123, r122>,
            class r124 = F<_124, r123>, class r125 = F<_125, r124>,
            class r126 = F<_126, r125 >>
            using g = scan_continuation<
            F<_127, r126>,
            list<r126, r125, r124, r123, r122, r121, r120, r119, r118, r117, r116,
            r115, r114, r113, r112, r111, r110, r109, r108, r107, r106, r105,
            r104, r103, r102, r101, r100, r99, r98, r97, r96, r95, r94, r93, r92,
            r91, r90, r89, r88, r87, r86, r85, r84, r83, r82, r81, r80, r79, r78,
            r77, r76, r75, r74, r73, r72, r71, r70, r69, r68, r67, r66, r65, r64,
            r63, r62, r61, r60, r59, r58, r57, r56, r55, r54, r53, r52, r51, r50,
            r49, r48, r47, r46, r45, r44, r43, r42, r41, r40, r39, r38, r37, r36,
            r35, r34, r33, r32, r31, r30, r29, r28, r27, r26, r25, r24, r23, r22,
            r21, r20, r19, r18, r17, r16, r15, r14, r13, r12, r11, r10, r9, r8,
            r7, r6, r5, r4, r3, r2, r1, r0>>;

        template<
            unsigned remaining, class C, template<class...> class F, class _0,
            class _1, class _2, class _3, class _4, class _5, class _6, class _7,
            class _8, class _9, class _10, class _11, class _12, class _13, class _14,
            class _15, class _16, class _17, class _18, class _19, class _20,
            class _21, class _22, class _23, class _24, class _25, class _26,
            class _27, class _28, class _29, class _30, class _31, class _32,
            class _33, class _34, class _35, class _36, class _37, class _38,
            class _39, class _40, class _41, class _42, class _43, class _44,
            class _45, class _46, class _47, class _48, class _49, class _50,
            class _51, class _52, class _53, class _54, class _55, class _56,
            class _57, class _58, class _59, class _60, class _61, class _62,
            class _63, class _64, class _65, class _66, class _67, class _68,
            class _69, class _70, class _71, class _72, class _73, class _74,
            class _75, class _76, class _77, class _78, class _79, class _80,
            class _81, class _82, class _83, class _84, class _85, class _86,
            class _87, class _88, class _89, class _90, class _91, class _92,
            class _93, class _94, class _95, class _96, class _97, class _98,
            class _99, class _100, class _101, class _102, class _103, class _104,
            class _105, class _106, class _107, class _108, class _109, class _110,
            class _111, class _112, class _113, class _114, class _115, class _116,
            class _117, class _118, class _119, class _120, class _121, class _122,
            class _123, class _124, class _125, class _126, class _127, class... xs >
            using f = typename g<
            F, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15,
            _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30,
            _31, _32, _33, _34, _35, _36, _37, _38, _39, _40, _41, _42, _43, _44, _45,
            _46, _47, _48, _49, _50, _51, _52, _53, _54, _55, _56, _57, _58, _59, _60,
            _61, _62, _63, _64, _65, _66, _67, _68, _69, _70, _71, _72, _73, _74, _75,
            _76, _77, _78, _79, _80, _81, _82, _83, _84, _85, _86, _87, _88, _89, _90,
            _91, _92, _93, _94, _95, _96, _97, _98, _99, _100, _101, _102, _103, _104,
            _105, _106, _107, _108, _109, _110, _111, _112, _113, _114, _115, _116,
            _117, _118, _119, _120, _121, _122, _123, _124, _125, _126, _127>::
            template f<remaining - 127,
            _scan_right<n_8_or_more_16_32_64_128_256(remaining - 127)>, C,
            F, xs...>;
    };
}
/// \endcond
namespace jln::mp
{
    /// \cond
    namespace detail
    {
        template<int n, class Cmp>
        struct _sort;
    }
    /// \endcond

    /// \ingroup algorithm

    /// Sorts the elements of a \sequence according to an ordering relation.
    /// \post \c is_sorted<result...>
    /// \treturn \sequence
    template<class Cmp = less<>, class C = listify>
    struct sort
    {
        template<class... xs>
        using f = typename unpack<C>::template f<
            typename detail::_sort<detail::min(3, sizeof...(xs)), Cmp>
            ::template f<xs...>
        >;
    };

    template<class Cmp = less<>, class C = listify>
    using stable_sort = sort<Cmp, C>;

    namespace emp
    {
        template<class L, class Cmp = mp::less<>, class C = listify>
        using sort = unpack<L, mp::sort<Cmp, C>>;

        template<class L, class Cmp = mp::less<>, class C = listify>
        using stable_sort = unpack<L, mp::sort<Cmp, C>>;
    }
}


namespace jln::mp
{
    /// \cond
    template<class Cmp>
    struct sort<Cmp, listify>
    {
        template<class... xs>
        using f = typename detail::_sort<detail::min(3, sizeof...(xs)), Cmp>
            ::template f<xs...>;
    };
    /// \endcond
}

/// \cond
namespace jln::mp::detail
{
    template<class Cmp>
    struct _sort<3, Cmp>
    {
        template<class... xs>
        using f = typename memoize_call<_merge<
            typename take_front_c<sizeof...(xs) / 2, _sort<min(3, sizeof...(xs) / 2), Cmp>>
            ::template f<xs...>,
            typename drop_front_c<sizeof...(xs) / 2, _sort<min(3, (sizeof...(xs) + 1) / 2), Cmp>>
            ::template f<xs...>
        >, Cmp>::type;
    };

    template<bool>
    struct mk_list2
    {
        template<class... xs>
        using f = list<xs...>;
    };

    template<>
    struct mk_list2<true>
    {
        template<class x, class y>
        using f = list<y, x>;
    };

    template<class Cmp>
    struct _sort<2, Cmp>
    {
        template<class x, class y>
        using f = typename mk_list2<bool(call<Cmp, y, x>::value)>
            ::template f<x, y>;
    };

    template<class Cmp>
    struct _sort<1, Cmp>
    {
        template<class x>
        using f = list<x>;
    };

    template<class Cmp>
    struct _sort<0, Cmp>
    {
        template<class...>
        using f = list<>;
    };
}
/// \endcond
namespace jln::mp
{
    /// \ingroup group

    /// Splits a \sequence into multiple \lists at every point that satisfy a predicate.
    /// The split value is inserted at the end of the previous list.
    /// \semantics
    ///   \code
    ///   call<split_after_if<is<void>, _0, _1, _2, _0, _3> == list<
    ///     list<_0>,
    ///     list<_1, _2, _0>,
    ///     list<_3>
    ///   >
    ///   \endcode
    /// \treturn \sequence of \list
    /// \see split_if, split_before_if
    template<class Pred, class C = listify>
    struct split_after_if
    {
        template <class... xs>
        using f = typename detail::_split<sizeof...(xs) != 0>
            ::template f<detail::split_after, C, Pred, xs...>;
    };

    template<class x, class C = listify>
    using split_after = split_after_if<is<x>, C>;

    namespace emp
    {
        template<class L, class Pred, class C = mp::listify>
        using split_after_if = unpack<L, mp::split_after_if<Pred, C>>;

        template<class L, class x, class C = mp::listify>
        using split_after = unpack<L, mp::split_after<x, C>>;
    }
}
namespace jln::mp
{
    /// \ingroup group

    /// Splits a sequence at an arbitrary position.
    /// \pre i >= 0 && i <= sizeof...(xs)
    /// \treturn \sequence of two \values
    template<class i, class F = listify, class C = listify>
    using split_at_with = tee<take_front<i, F>, drop_front<i, F>, C>;

    template<int_ i, class F = listify, class C = listify>
    using split_at_with_c = split_at_with<number<i>, F, C>;

    /// Splits a sequence at an arbitrary position.
    /// \pre i >= 0 && i <= sizeof...(xs)
    /// \treturn \sequence of two \lists
    template<class i, class C = listify>
    using split_at = tee<take_front<i>, drop_front<i>, C>;

    template<int_ i, class C = listify>
    using split_at_c = split_at<number<i>, C>;

    namespace emp
    {
        template<class L, class i, class F = mp::listify, class C = mp::listify>
        using split_at_with = unpack<L, mp::split_at_with<i, F, C>>;

        template<class L, int_ i, class F = mp::listify, class C = mp::listify>
        using split_at_with_c = unpack<L, mp::split_at_with_c<i, F, C>>;

        template<class L, class i, class C = mp::listify>
        using split_at = unpack<L, mp::split_at<i, C>>;

        template<class L, int_ i, class C = mp::listify>
        using split_at_c = unpack<L, mp::split_at_c<i, C>>;
    }
}
namespace jln::mp
{
    /// \ingroup group

    /// Splits a \sequence into multiple \lists at every point that satisfy a predicate.
    /// The split value is inserted at the beginning of the following list.
    /// \semantics
    ///   \code
    ///   call<split_before_if<is<void>, _0, _1, _2, _0, _3> == list<
    ///     list<>,
    ///     list<_0, _1, _2>,
    ///     list<_0, _3>
    ///   >
    ///   \endcode
    /// \treturn \sequence of \list
    /// \see split_if, split_after_if
    template<class Pred, class C = listify>
    struct split_before_if
    {
        template <class... xs>
        using f = typename detail::_split<sizeof...(xs) != 0>
            ::template f<detail::split_before, C, Pred, xs...>;
    };

    template<class x, class C = listify>
    using split_before = split_before_if<is<x>, C>;

    namespace emp
    {
        template<class L, class Pred, class C = mp::listify>
        using split_before_if = unpack<L, mp::split_before_if<Pred, C>>;

        template<class L, class x, class C = mp::listify>
        using split_before = unpack<L, mp::split_before<x, C>>;
    }
}
namespace jln::mp
{
    /// \cond
    namespace detail
    {
        template<class position, class C1, class C2, class C, class... xs>
        using _split_by_i = typename C::template f<
            typename take_front<position, C1>::template f<xs...>,
            typename drop_front<position, C2>::template f<xs...>
        >;
    }
    /// \endcond

    /// \ingroup group

    /// Splits a sequence at an arbitrary position returns by \c GetIndex.
    /// \pre `GetIndex::f<xs...>::value >= 0 && GetIndex::f<xs...>::value <= sizeof...(xs)`
    /// \treturn \sequence of two \values
    template<class GetIndex, class SubC1 = listify, class SubC2 = SubC1, class C = listify>
    struct split_by
    {
        template<class... xs>
        using f = typename detail::_split_by_i<
            JLN_MP_DCALL_XS(xs, GetIndex, xs...), SubC1, SubC2, C, xs...>;
    };

    template<class GetIndex, class C = listify>
    using split_by2 = split_by<GetIndex, listify, listify, C>;

    namespace emp
    {
        template<class L, class GetIndex,
            class SubC1 = mp::listify, class SubC2 = SubC1, class C = mp::listify>
            using split_by = unpack<L, mp::split_by<GetIndex, SubC1, SubC2, C>>;

        template<class L, class GetIndex, class C = mp::listify>
        using split_by2 = unpack<L, mp::split_by<GetIndex, mp::listify, mp::listify, C>>;
    }
}
namespace jln::mp
{
    /// \ingroup algorithm

    /// Inserts a \value after each element of a \sequence.
    /// \treturn \list
    /// \see intersperse, prefix
    template<class x, class C = listify>
    struct suffix
    {
        template<class... xs>
        using f = typename detail::_join_select<sizeof...(xs)>
            ::template f<C, list<xs, x>...>::type;
    };

    namespace emp
    {
        template<class L, class x, class C = mp::listify>
        using suffix = unpack<L, mp::suffix<x, C>>;
    }
}
namespace jln::mp
{
    /// \cond
    namespace detail
    {
        template<class>
        struct take_while_impl;
    }
    /// \endcond

    /// \ingroup search

    /// Extracts the first elements of a \sequence that satisfy a \predicate.
    /// \treturn \sequence
    /// \see take_front, take_back, drop_while
    template<class Pred, class C = listify>
    struct take_while
    {
        template<class... xs>
        using f = typename detail::take_while_impl<
            typename detail::_drop_while<
            detail::n_8_or_more_16_32_64_128_256(sizeof...(xs)), true
      >::template f<0, Pred, xs...>
        >::template f<C, xs...>;
    };
}


/// \cond
namespace jln::mp::detail
{
    template<>
    struct take_while_impl<_drop_while_continue>
    {
        template<class C, class... xs>
        using f = typename C::template f<xs...>;
    };

    template<std::size_t n>
    struct take_while_impl<_drop_while_result<n>>
    {
        template<class C, class... xs>
        using f = typename take_front<number<sizeof...(xs) - n - 1>, C>::template f<xs...>;
    };
}
/// \encond
namespace jln::mp
{
    /// \ingroup search

    /// Finds first element that is greater that \c x.
    /// Invokes \c FC with all the elements since the one found at the end.
    /// If no element is found, \c NFC is used.
    /// \pre \c is_sorted<Cmp>::f<xs...>
    /// \treturn \sequence
    template<class x, class Cmp = less<>, class C = listify, class NC = C>
    using upper_bound = lower_bound<x, flip<tee<Cmp, not_<>>>, C, NC>;

    template<int_ x, class Cmp = less<>, class C = listify, class NC = C>
    using upper_bound_c = upper_bound<number<x>, Cmp, C, NC>;

    template<class x, class C = listify, class NC = C>
    using upper_bound_than = upper_bound<x, less<>, C, NC>;

    template<int_ x, class C = listify, class NC = C>
    using upper_bound_than_c = upper_bound<number<x>, less<>, C, NC>;

    namespace emp
    {
        template<class L, class x, class Cmp = mp::less<>, class C = mp::listify, class NC = C>
        using upper_bound = unpack<L, mp::upper_bound<x, Cmp, C, NC>>;

        template<class L, int_ x, class Cmp = mp::less<>, class C = mp::listify, class NC = C>
        using upper_bound_c = unpack<L, mp::upper_bound<mp::number<x>, Cmp, C, NC>>;

        template<class L, class x, class C = mp::listify, class NC = C>
        using upper_bound_than = unpack<L, mp::upper_bound<x, mp::less<>, C, NC>>;

        template<class L, int_ x, class C = mp::listify, class NC = C>
        using upper_bound_than_c = unpack<L, mp::upper_bound<mp::number<x>, mp::less<>, C, NC>>;
    }
}
namespace jln::mp
{
    /// \ingroup functional

    /// Partially apply a function to some arguments.
    /// \treturn \sequence
    template<class F, class... xs>
    struct bind
    {
        template<class... ys>
        using f = call<F, xs..., ys...>;
    };

    /// Partially apply a function to some arguments.
    /// \treturn \sequence
    template<class F, class... xs>
    struct reverse_bind
    {
        template<class... ys>
        using f = call<F, ys..., xs...>;
    };

    template<class F, class C>
    using bind1st = partial<F, C>;

    template<class F, class C>
    using bind2nd = partial<identity, F, C>;
} // namespace jln::mp
namespace jln::mp
{
    /// \ingroup value

#if __cplusplus >= 201703L
# if !JLN_MP_ENABLE_DEBUG || defined(__clang__)
    template<auto v>
    struct val
    {
        static constexpr decltype(v) value = v;
    };

    template<class T, T v>
    using typed_val = val<v>;
# else
    template<class T, T v>
    struct typed_val
    {
        static constexpr T const value = v;
    };

    template<auto v>
    using val = typed_val<std::remove_const_t<decltype(v)>, v>;
# endif
#else
    template<class T, T v>
    struct typed_val
    {
        static constexpr T const value = v;
    };
#endif
}
namespace jln::mp
{
    /// \ingroup functional

    template<class... xs>
    struct capture
    {
        template<class C, class... ys>
        using f = typename C::template f<xs..., ys...>;
    };

    template<class... xs>
    struct capture_v
    {
        template<class C, class... ys>
        using f = typename C::template f<xs::value..., ys::value...>;
    };

#if __cplusplus >= 201703L
    template<auto... xs>
    using capture_c = capture<val<xs>...>;

    template<auto... xs>
    struct capture_v_c
#else
    template<int_... xs>
    using capture_c = capture<number<xs>...>;

    template<int_... xs>
    struct capture_v_c
#endif
    {
        template<class C, class... ys>
        using f = typename C::template f<xs..., ys::value...>;
    };

    template<class... xs>
    struct reverse_capture
    {
        template<class C, class... ys>
        using f = typename C::template f<ys..., xs...>;
    };

    template<class... xs>
    struct reverse_capture_v
    {
        template<class C, class... ys>
        using f = typename C::template f<ys::value..., xs::value...>;
    };

#if __cplusplus >= 201703L
    template<auto... xs>
    using reverse_capture_c = reverse_capture<val<xs>...>;

    template<auto... xs>
    struct reverse_capture_v_c
#else
    template<int_... xs>
    using reverse_capture_c = reverse_capture<number<xs>...>;

    template<int_... xs>
    struct reverse_capture_v_c
#endif
    {
        template<class C, class... ys>
        using f = typename C::template f<ys::value..., xs...>;
    };

}
namespace jln::mp
{
    /// \cond
    namespace detail
    {
        template<class, class...>
        struct _cascade;
    }
    /// \endcond

    /// \ingroup functional

    /// Recursively invokes \functions to nested \typelist of \typelists.
    /// \semantics
    ///   \code
    ///     cascade<F0,F1,F2> = transform<unpack<transform<unpack<F2>, F1>>, F0>
    ///   \endcode
    template<class F, class... Fs>
    using cascade = typename detail::_cascade<F, Fs...>::type;
}


/// \cond
namespace jln::mp::detail
{
    template<class F, class... Fs>
    struct _cascade
    {
        using type = transform<unpack<typename _cascade<Fs...>::type>, F>;
    };

    template<class F0, class F1>
    struct _cascade<F0, F1>
    {
        using type = transform<unpack<F1>, F0>;
    };

    template<class F>
    struct _cascade<F>
    {
        using type = transform<F>;
    };
}
/// \endcond
namespace jln::mp
{
    /// \cond
    namespace detail
    {
        template<unsigned> struct _compose_f;
        template<class, class> struct _compose;
    }
    /// \endcond

    /// \ingroup functional

    /// Composition of two \metafunctions or more.
    /// \treturn \function
    template<template<class...> class F, template<class...> class... Fs>
    using compose_f = typename detail::_compose_f<sizeof...(Fs)>
        ::template f<F, Fs...>;

    /// Composition of two \functions or more.
    /// \treturn \function
    template<class F, class... Fs>
    using compose = typename conditional_c<sizeof...(Fs) == 0>
        ::template f<
        at1<F>,
        mp::fold_right<lift_t<detail::_compose>>
        >
        ::template f<identity, F, Fs...>;
}


/// \cond
namespace jln::mp::detail
{
#define JLN_COMPOSE_IMPL(n, mp_xs, mp_rxs, mp_dup) \
  template<template<class...> class F              \
    mp_xs(JLN_MP_COMMA template<class...> class,   \
      JLN_MP_NIL, JLN_MP_NIL)>                     \
  struct _compose_f_impl##n                        \
  {                                                \
    template<class... xs>                          \
    using f = mp_rxs(JLN_MP_NIL, <, JLN_MP_NIL)    \
      JLN_MP_DCALLF_XS(xs, F, xs...)               \
    mp_dup(>, JLN_MP_NIL);                         \
  };

    JLN_MP_GEN_XS_0_TO_8(JLN_COMPOSE_IMPL)

#undef JLN_COMPOSE_IMPL

#define JLN_COMPOSE_IMPL(n, mp_xs, mp_rxs, mp_dup)  \
  template<>                                        \
  struct _compose_f<n>                              \
  {                                                 \
    template<template<class...> class F             \
      mp_xs(JLN_MP_COMMA template<class...> class,  \
        JLN_MP_NIL, JLN_MP_NIL)>                    \
    using f = _compose_f_impl##n<F                  \
      mp_xs(JLN_MP_COMMA, JLN_MP_NIL, JLN_MP_NIL)>; \
  };

        JLN_MP_GEN_XS_0_TO_8(JLN_COMPOSE_IMPL)

#undef JLN_COMPOSE_IMPL

        template<unsigned n>
    struct _compose_f
    {
        template<
            template<class...> class F0,
            template<class...> class F1,
            template<class...> class F2,
            template<class...> class F3,
            template<class...> class F4,
            template<class...> class F5,
            template<class...> class F6,
            template<class...> class F7,
            template<class...> class... Fs>
        using f = typename _compose_f<n - 8>::template f<
            _compose_f_impl7<F0, F1, F2, F3, F4, F5, F6, F7>::template f,
            Fs...
        >;
    };

    template<class F, class C>
    struct _compose
    {
        using type = tee<F, C>;
    };

    template<template<class...> class F, class x>
    struct _compose<lift<F>, x>
    {
        using type = lift<F, x>;
    };

    template<template<class...> class F, class x>
    struct _compose<lift_t<F>, x>
    {
        using type = lift_t<F, x>;
    };
}
/// \endcond
namespace jln::mp
{
#ifdef __cpp_nontype_template_parameter_class
#if __cpp_nontype_template_parameter_class >= 201806L
    /// \ingroup functional

    /// Invokes a lambda function.
    /// \treturn \value
    template<auto F, class C = identity>
    struct eval
    {
        template<class... xs>
        using f = JLN_MP_DCALL_XS(xs,
            C, decltype(F.template operator() < xs... > ()));
    };

    namespace emp
    {
        template<auto F, class... xs>
        using eval = decltype(F.template operator() < xs... > ());
    }
#endif
#endif
}
namespace jln::mp
{
    /// \ingroup functional

    /// Invokes a function computing the fixed point of a function.
    /// \treturn \value
    /// \see recurse, recurse_fix
    template<class C>
    struct fix
    {
        template<class... xs>
        using f = call<C, fix<C>, xs...>;
    };

    namespace emp
    {
        template<class F, class... xs>
        using fix = typename fix<F>::template f<xs...>;
    }
}
namespace jln::mp
{
    /// \ingroup functional

    /// Checks whether \c F::f<xs...> is invocable.
    /// \pre \c F::f<xs...> must be a SFINAE compatible expression
    /// \treturn \bool
    template<class F, class C = identity>
    using is_invocable = try_<F, always<true_, C>, always<false_, C>>;

    namespace emp
    {
        template<class F, class... xs>
        using is_invocable = typename mp::is_invocable<F>::template f<xs...>;
    }
}
namespace jln::mp
{
    /// \ingroup functional

    /// Invokes \c FC whether `na`, otherwise \c C.
    /// \treturn \value
    template<class C, class FC = violation>
    using monadic = if_<is<na>, FC, C>;

    /// Invokes \c FC whether first value is `na`, otherwise \c C.
    /// \treturn \value
    template<class C, class FC = violation>
    using monadic0 = if_<front<is<na>>, FC, C>;

    /// Invokes \c FC whether any value is `na`, otherwise \c C.
    /// \treturn \value
    template<class C, class FC = violation>
    using monadic_xs = if_<transform<is<na>, or_<>>, FC, C>;

    /// Monadify only if \c x is \c na.
    /// \treturn \value
    template<class x, template<class...> class M, class C, class FC = violation>
    using monadic_if_na = typename conditional_c<std::is_same<na, x>::value>
        ::template f<M<C, FC>, C>;
}
namespace jln::mp
{
    /// \cond
    namespace detail
    {
        template <class C, class... Fs>
        struct _on;
    }
    /// \endcond

    /// \ingroup functional

    /// Invokes multiple functions each taking the parameter corresponding to its position
    /// (or without parameter whether it does not exist)
    /// then calls \c C with the results and the rest of the parameters.
    /// \semantics
    ///   \code
    ///   on<F,G,C>::f<a> == C::f<F::f<a>, G::f<>>
    ///   on<F,G,C>::f<a,b,c,d> == C::f<F::f<a>, G::f<b>, c, d>
    ///   \endcode
    /// \treturn \value
    /// \see each, tee, partial
#ifdef JLN_MP_DOXYGENATING
    template <class... Fs, class C>
    struct on
    {
        template<class... xs>
        using f = /* unspecified */;
    };
#else
    template <class... Fs>
    struct on
        : rotate<number<-1>, lift<detail::_on>>
        ::template f<Fs...>
    {};
#endif
}

/// \cond
namespace jln::mp::detail
{
    constexpr int on_select(std::size_t nf, std::size_t nx)
    {
        return (nf < nx) ? 1 : (nf > nx) ? 2 : 0;
    }

    template <int, class C, class... Fs>
    struct _on_select;

    // each
    template <class C, class... Fs>
    struct _on_select<0, C, Fs...>
    {
        template<class... xs>
        using f = call<C, call<Fs, xs>...>;
    };

    // partial
    template <class C, class... Fs>
    struct _on_select<1, C, Fs...>
    {
        template<class... xs>
        using f = typename _join_select<2>::f<
            C,
            typename take_front_c<sizeof...(Fs), _each<listify, Fs...>>::template f<xs...>,
            typename drop_front_c<sizeof...(Fs)>::template f<xs...>
        >::type;
    };

    template <class C, class... Fs>
    struct _on_select<2, C, Fs...>
    {
        template<class... xs>
        using f = typename _join_select<2>::f<
            C,
            typename take_front_c<sizeof...(xs) + 1, lift<_each>>
            ::template f<listify, Fs...>
            ::template f<xs...>,
            typename drop_front_c<sizeof...(xs), lift<tee>>
            ::template f<Fs..., listify>
            ::template f<>
        >::type;
    };

    template <class C, class... Fs>
    struct _on
    {
        template<class... xs>
        using f = typename _on_select<on_select(sizeof...(Fs), sizeof...(xs)), C, Fs...>
            ::template f<xs...>;
    };
} // namespace jln::mp
/// \endcond
namespace jln::mp
{
    /// \ingroup functional

    /// Apply a function while some \predicate is satisfied.
    /// \treturn \value
    /// \see while_xs
    template<class Pred, class F, class C = identity>
    using while_ = recursively<if_<Pred, F, stop_recursion>, C>;

    /// Apply a function while some \predicate is satisfied.
    /// \treturn \value
    /// \see while
    template<class Pred, class F, class C = listify>
    using while_xs = recursively<if_<Pred, F, stop_recursion>, C>;

    namespace emp
    {
        template<class L, class Pred, class F, class C = mp::identity>
        using while_ = unpack<L, mp::while_<Pred, F, C>>;

        template<class L, class Pred, class F, class C = mp::listify>
        using while_xs = unpack<L, mp::while_<Pred, F, C>>;
    }
}
namespace jln::mp
{
    /// \ingroup list

    /// Inserts elements at the end of \c L list.
    /// \treturn \sequence
    template<class L, class C = listify>
    using append = push_front<L, unpack_append<C>>;

    namespace emp
    {
        template<class L, class... xs>
        using append = unpack_append<L, listify, xs...>;
    }
}
namespace jln::mp
{
    /// \cond
    namespace detail
    {
        template<class seq>
        struct _as_list
        {};
    }
    /// \endcond

    /// \ingroup list

    /// Extracts type paramaters of a template class or union, then constructs a \list.
    /// \pre \c seq must be compatible with \typelist or \c detail::_as_list<seq>::type.
    /// \treturn \list
    template<class C = identity>
    struct as_list
    {
        template<class seq>
        using f = typename C::template f<typename detail::_as_list<seq>::type>;
    };

    namespace emp
    {
        template<class seq, class C = mp::identity>
        using as_list = typename as_list<C>::template f<seq>;
    }
}

/// \cond
namespace jln::mp
{
    template<>
    struct as_list<identity>
    {
        template<class seq>
        using f = typename detail::_as_list<seq>::type;
    };
}

namespace jln::mp::detail
{
    template<template<class...> class seq, class... xs>
    struct _as_list<seq<xs...>>
    {
        using type = list<xs...>;
    };

    template<template<class T, T...> class seq, class T, T... ns>
    struct _as_list<seq<T, ns...>>
    {
        using type = list<number<ns>...>;
    };

    template<template<auto...> class seq, auto... ns>
    struct _as_list<seq<ns...>>
    {
        using type = list<number<ns>...>;
    };
}
/// \endcond
namespace jln::mp
{
    /// \ingroup list

    /// Retrieves the last element of a sequence.
    /// \treturn \value
    template<class C = identity>
    using back = rotate<number<-1>, front<C>>;

    namespace emp
    {
        template<class L, class C = mp::identity>
        using back = unpack<L, mp::back<C>>;
    }
}
namespace jln::mp
{
    /// \ingroup list

    /// Removes \c N elements from the end of a \sequence.
    /// \pre `0 <= N <= sizeof...(xs)`
    /// \treturn \sequence
    /// \see drop_back_max, drop_front, take_front, take_back, drop_while
    template<class N, class C = listify>
    struct drop_back
    {
        template<class... xs>
        using f = typename rotate<number<-N::value>, drop_front<N, C>>::template f<xs...>;
    };

    /// Removes at most \c N elements from the end of a \sequence.
    /// \pre `0 <= N`
    /// \treturn \sequence
    /// \see drop_back, drop_front, drop_while
    template<class N, class C = listify>
    struct drop_back_max
    {
        template<class... xs>
        using f = typename rotate < number<-N::value>,
            drop_front < number < detail::min(std::size_t{ N::value }, sizeof...(xs)) > , C >
        > ::template f<xs...>;
    };

    template<int_ n, class C = listify>
    using drop_back_c = drop_back<number<n>, C>;

    template<int_ n, class C = listify>
    using drop_back_max_c = drop_back_max<number<n>, C>;

    namespace emp
    {
        template<class L, class N, class C = mp::listify>
        using drop_back = unpack<L, mp::drop_back<N, C>>;

        template<class L, int_ n, class C = mp::listify>
        using drop_back_c = unpack<L, mp::drop_back<number<n>, C>>;

        template<class L, class N, class C = mp::listify>
        using drop_back_max = unpack<L, mp::drop_back_max<N, C>>;

        template<class L, int_ n, class C = mp::listify>
        using drop_back_max_c = unpack<L, mp::drop_back_max<number<n>, C>>;
    }

    /// \cond
    template<class C>
    struct drop_back<number<0>, C>
    {
        template<class... xs>
        using f = JLN_MP_DCALL(sizeof...(xs) >= 0, C, xs...);
    };

    template<class C>
    struct drop_back_max<number<0>, C>
    {
        template<class... xs>
        using f = JLN_MP_DCALL(sizeof...(xs) >= 0, C, xs...);
    };
    /// \endcond
}
namespace jln::mp
{
    /// \ingroup list

    /// Removes all elements between two arbitrary indices of a sequence.
    /// \pre `0 <= start < sizeof...(xs)`
    /// \pre `0 <= start + size < sizeof...(xs)`
    /// \treturn \sequence
    template<class start, class size = number<1>, class C = listify>
    struct erase
    {
        template<class... xs>
        using f = typename join<C>::template f<
            typename take_front<start>::template f<xs...>,
            typename drop_front_c < detail::min(
                sizeof...(xs),
                unsigned{ detail::validate_index<start::value, sizeof...(xs)>::value
                + unsigned{size::value} }
            ) > ::template f<xs...>
        > ;
    };

    template<int_ start, int_ size = 1, class C = listify>
    using erase_c = erase<number<start>, number<size>, C>;

    namespace emp
    {
        template<class L, class start, class size = mp::number<1>, class C = mp::listify>
        using erase = unpack<L, mp::erase<start, size, C>>;

        template<class L, int_ start, int_ size = 1, class C = mp::listify>
        using erase_c = erase<L, number<start>, number<size>, C>;
    }
}
namespace jln::mp
{
    /// \ingroup list

    /// Inserts all elements of \c seq at an arbitrary position.
    /// \pre `0 <= i < sizeof...(xs)`
    /// \pre seq must be a \list
    /// \treturn \sequence
    template<class i, class seq, class C = listify>
    using insert_range = tee<take_front<i>, always<seq>, drop_front<i>, join<C>>;

    template<int_ i, class seq, class C = listify>
    using insert_range_c = insert_range<number<i>, seq, C>;

    namespace emp
    {
        template<class L, class i, class seq, class C = mp::listify>
        using insert_range = unpack<L, mp::insert_range<i, seq, C>>;

        template<class L, int_ i, class seq, class C = mp::listify>
        using insert_range_c = unpack<L, mp::insert_range_c<i, seq, C>>;
    }
}
namespace jln::mp
{
    /// \ingroup list

    /// Inserts an elements at an arbitrary position.
    /// \pre `0 <= i < sizeof...(xs)`
    /// \treturn \sequence
    template<class i, class x, class C = listify>
    using insert = insert_range<i, list<x>, C>;

    template<int_ i, class x, class C = listify>
    using insert_c = insert_range_c<i, list<x>, C>;

    namespace emp
    {
        template<class L, class i, class x, class C = mp::listify>
        using insert = unpack<L, mp::insert<i, x, C>>;

        template<class L, int_ i, class x, class C = mp::listify>
        using insert_c = insert<L, number<i>, x, C>;
    }
}
namespace jln::mp
{
    /// \cond
    namespace detail
    {
        template<class x>
        struct _is_list;
    }
    /// \endcond

    /// \ingroup list

    /// Checks whether \c x is a \list.
    /// \treturn \number
    template<class C = identity>
    struct is_list
    {
        template<class x>
        using f = typename C::template f<typename detail::_is_list<x>::type>;
    };

    namespace emp
    {
        template<class x, class C = mp::identity>
        using is_list = typename C::template f<typename detail::_is_list<x>::type>;
    }
} // namespace jln::mp


/// \cond
namespace jln::mp::detail
{
    template<class x>
    struct _is_list
    {
        using type = false_;
    };

    template<class... xs>
    struct _is_list<list<xs...>>
    {
        using type = true_;
    };
} // namespace jln::mp::detail
/// \endcond
namespace jln::mp
{
    /// \ingroup list

    template<class N, class C = identity>
    using is_size_of = size<is<N, C>>;

    template<int_ n, class C = identity>
    using is_size_of_c = size<is<number<n>, C>>;

    namespace emp
    {
        template<class L, class N, class C = mp::identity>
        using is_size_of = unpack<L, mp::is_size_of<N, C>>;

        template<class L, int_ n, class C = mp::identity>
        using is_size_of_c = unpack<L, mp::is_size_of_c<n, C>>;
    }
}
namespace jln::mp
{
    /// \ingroup search

    /// Difference between the number of parameter \c xs and \c I::value.
    /// \semantics
    ///   Equivalent to
    ///   \code
    ///   size<push_front<I, sub<C>>>
    ///   \endcode
    /// \treturn \number
    template <class I, class C = identity>
    struct offset
    {
        template <class... xs>
        using f = typename C::template f < number < (I::value - int_{ sizeof...(xs) }) >> ;
    };

    /// \cond
    template <class I>
    struct offset<I, identity>
    {
        template <class... xs>
        using f = number < (I::value - int_{ sizeof...(xs) }) > ;
    };
    /// \endcond

    template<int_ i, class C = identity>
    using offset_c = offset<number<i>, C>;

    namespace emp
    {
        template<class L, class I, class C = mp::identity>
        using offset = unpack<L, mp::offset<I, C>>;

        template<class L, int_ i, class C = mp::identity>
        using offset_c = unpack<L, mp::offset<number<i>, C>>;
    }
}
namespace jln::mp
{
    /// \ingroup list

    /// Removes the last element of sequence.
    /// \pre `sizeof...(xs) > 0`
    /// \treturn \sequence
    template<class C = listify>
    using pop_back = rotate<number<-1>, pop_front<C>>;

    namespace emp
    {
        template<class L, class C = mp::listify>
        using pop_back = unpack<L, mp::pop_back<C>>;
    }
}
namespace jln::mp
{
    /// \cond
    namespace detail
    {
        template<int_ beg, int_ end, class C, bool _
            = !(beg == -1
                || ((beg >= 0 ? end >= 0 : end < 0) && beg >= end)),
            bool = _ && (beg >= 0),
            bool = _ && (end >= 0)>
            struct _range;
    }
    /// \endcond

    /// \ingroup list

    /// Returns a contiguous subsequence of a \sequence.
    /// A negative value represents an index starting from the end.
    /// if finally, \c end <= \c beg, then an empty \list is returned.
    /// \treturn \sequence
    template<class beg, class end, class C = listify>
    struct range : detail::_range<beg::value, end::value, C>::type
    {
#ifdef JLN_MP_DOXYGENATING
        template<class... xs>
        using f;
#endif
    };

    template<int_ beg, int_ end, class C = listify>
#ifdef JLN_MP_DOXYGENATING
    using range_c = range<number<beg>, number<end>, C>;
#else
    using range_c = typename detail::_range<beg, end, C>::type;
#endif

    namespace emp
    {
        template<class L, class beg, class end, class C = mp::listify>
        using range = unpack<L, mp::range<beg, end, C>>;

        template<class L, int_ beg, int_ end, class C = mp::listify>
        using range_c = unpack<L, mp::range_c<beg, end, C>>;
    }
}

/// \cond
namespace jln::mp::detail
{
    template<int_ beg, int_ end, class C>
    struct _range<beg, end, C, false, false, false>
    {
        using type = _cartesian<C, 0>; // for C::f<>
    };

    template<int_ beg, int_ end>
    struct _range<beg, end, listify, false, false, false>
    {
        using type = always<list<>>;
    };

    template<class C>
    struct _range<0, -1, C, true, true, false>
    {
        using type = C;
    };

    template<int_ beg, int_ end, class C, bool b1, bool b2>
    struct _range<beg, end, C, true, b1, b2>
    {
        struct impl
        {
            template<class... xs>
            using f = typename range_c<
                (beg >= 0
                    ? beg
                    : (sizeof...(xs) <= -beg
                        ? 0
                        : int_(sizeof...(xs)) + beg)),
                (end >= 0
                    ? end
                    : (sizeof...(xs) <= -end
                        ? 0
                        : int_(sizeof...(xs)) + (end + 1))),
                C
            >::template f<xs...>;
        };

        using type = impl;
    };

    template<int_ beg, int_ end, int_ n, class C,
        bool = (beg < n), bool = (end < n)>
        struct _range_impl // true, true
    {
        using type = drop_front_c<beg, take_front_c<end - beg, C>>;
    };

    template<int_ end, int_ n, class C>
    struct _range_impl<0, end, n, C, true, true>
    {
        using type = take_front_c<end, C>;
    };

    template<int_ beg, int_ end, int_ n, class C>
    struct _range_impl<beg, end, n, C, true, false>
    {
        using type = drop_front_c<beg, C>;
    };

    template<int_ end, int_ n, class C>
    struct _range_impl<0, end, n, C, true, false>
    {
        using type = C;
    };

    template<int_ beg, int_ end, int_ n, class C>
    struct _range_impl<beg, end, n, C, false, false>
    {
        using type = _cartesian<C, 0>; // for C::f<>
    };

    template<int_ beg, int_ end, int_ n>
    struct _range_impl<beg, end, n, listify, false, false>
    {
        using type = always<list<>>;
    };

    template<int_ beg, int_ end, class C>
    struct _range<beg, end, C, true, true, true>
    {
        struct impl
        {
            template<class... xs>
            using f = typename _range_impl<beg, end, sizeof...(xs), C>
                ::type::template f<xs...>;
        };

        using type = impl;
    };

    template<int_ beg, class C>
    struct _range<beg, -1, C, true, true, false>
    {
        struct impl
        {
            template<class... xs>
            using f = typename _range_impl<beg, sizeof...(xs), sizeof...(xs), C>
                ::type::template f<xs...>;
        };

        using type = impl;
    };
}
/// \endcond
namespace jln::mp
{
    /// \cond
    namespace detail
    {
        template<int>
        struct _slice;

        constexpr int_ slide_select(int_ nx, int_ size, int_ stride);
    }
    /// \endcond

    /// \ingroup list

    /// Returns a subset of elements in a \c xs picked at regular intervals in range.
    /// \pre `0 <= start < sizeof...(xs)`
    /// \pre `stride > 0`
    /// \pre `0 <= size * (stride - 1) + 1 < sizeof...(xs) - start`
    /// \treturn \sequence
    template<class start, class size, class stride = number<1>, class C = listify>
    struct slice
    {
        template<class... xs>
        using f = typename detail::_slice<
            detail::slide_select(int_(sizeof...(xs)) - start::value, size::value, stride::value)
        >
            ::template f <
            start::value, size::value,
            // verify that stride is strictly greater than 0
#ifdef _MSC_VER
            emp::conditional_c<(stride::value > 0), stride, void>::value,
#else
            unsigned{ int_(stride::value) - 1 } + 1u,
#endif
            C, sizeof...(xs) >
            ::template f<xs...>;
    };

    template<int_ start, int_ size, int_ stride = 1, class C = listify>
    using slice_c = slice<number<start>, number<size>, number<stride>, C>;

    namespace emp
    {
        template<class L, class start, class size, class stride = number<1>, class C = mp::listify>
        using slice = unpack<L, slice<start, size, stride, C>>;

        template<class L, int_ start, int_ size, int_ stride = 1, class C = mp::listify>
        using slice_c = slice<L, number<start>, number<size>, number<stride>, C>;
    }
}


/// \cond
namespace jln::mp::detail
{
    constexpr int_ slide_select(int_ nx, int_ size, int_ stride)
    {
        return !size ? 0
            : size == 1 ? 3
            : stride <= 1 ? 2
            : nx < stride ? 2
            : 1;
    }

    template<>
    struct _slice<2>
    {
        template<int_ start, int_ size, unsigned /*stride*/, class C, std::size_t len>
        using f = drop_front_c < start, take_front_c <
            detail::validate_index < size - 1,
#ifdef _MSC_VER
            (start < int_(len) ? int_(len) - start : 0)
#else
            unsigned{ len - start }
#endif
            > ::value + 1,
            C >> ;
    };

    template<int_ size, int_ stride, class C>
    struct _slice_impl
    {
#ifdef _MSC_VER
        template<int_ i, class x>
        using g = typename wrap_in_list_c<(i <= size && i % stride == 0)>::template f<x>;
#endif

        template<int_... ints>
        struct impl
        {
#ifdef _MSC_VER
            template<class... xs>
            using f = call<join<C>, g<ints, xs>...>;
#else
            template<class... xs>
            using f = typename join<C>::template f<
                typename wrap_in_list_c<(ints <= size && ints % stride == 0)>
                ::template f<xs>
                ...>;
#endif
        };
    };

    template<>
    struct _slice<1>
    {
        template<int_ start, int_ size, unsigned stride, class C, std::size_t len>
        using f = drop_front_c<
            start,
            typename emp::make_int_sequence_v_c<
            detail::validate_index<int_(len) - start, len>::value,
            lift_c<_slice_impl<
            detail::validate_index<size* stride - stride + 1, len - start>::value,
            stride, C
            >::template impl>
            >
        >;
    };

    template<>
    struct _slice<0>
    {
        template<int_ start, int_ size, unsigned /*stride*/, class C, std::size_t len>
        using f = typename conditional_c<
            bool(detail::validate_index<start, len>::value)
        >::template f<C, C>;
    };

    template<>
    struct _slice<3>
    {
        template<int_ start, int_ size, unsigned /*stride*/, class C, std::size_t len>
        using f = drop_front_c<start, front<C>>;
    };
}
/// \endcond
namespace jln::mp
{
    /// \cond
    namespace detail
    {
        constexpr int_ sliding_stride(int_ size, int_ stride);

        template<int_ size, int_ stride, int_>
        struct mk_sliding;
    }
    /// \endcond

    /// \ingroup list

    /// Returns sliding windows of width \c size.
    /// \pre `stride != 0`
    /// \pre `size >= 0`
    /// \treturn \sequence of \list
    /// Given a sequence and a count n, place a window over the first n elements of the underlying range. Return the contents of that window as the first element of the adapted range, then slide the window forward one element at a time until hitting the end of the underlying range.
    /// \semantics
    ///     If `stride < 0`, then `stride = stride + size`
    ///     If `sizeof...(xs) < size`, then `f = C::f<xs...>`
    ///     If `stride > 1`, the last window may be smaller than \c size
    template<class size, class stride, class C = listify>
    using sliding_with_stride = typename detail::mk_sliding<
        size::value, stride::value,
        detail::sliding_stride(size::value, stride::value)
    >::template f<C>;

    template<class size, class C = listify>
    using sliding = sliding_with_stride<size, number<1>, C>;

    template<int_ size, int_ stride = 1, class C = listify>
    using sliding_with_stride_c = typename detail::mk_sliding<size, stride,
        detail::sliding_stride(size, stride)
    >::template f<C>;

    template<int_ size, class C = listify>
    using sliding_c = sliding_with_stride_c<size, 1, C>;

    namespace emp
    {
        template<class L, class size, class stride, class C = mp::listify>
        using sliding_with_stride = unpack<L, mp::sliding_with_stride<size, stride, C>>;

        template<class L, int_ size, int_ stride, class C = mp::listify>
        using sliding_with_stride_c = unpack<L, mp::sliding_with_stride_c<size, stride, C>>;

        template<class L, class size, class C = mp::listify>
        using sliding = unpack<L, mp::sliding<size, C>>;

        template<class L, int_ size, class C = mp::listify>
        using sliding_c = unpack<L, mp::sliding_c<size, C>>;
    }
}


/// \cond
namespace jln::mp::detail
{
    constexpr int_ sliding_stride(int_ size, int_ stride)
    {
        if (!stride || size < 1) {
            return size == 0 ? -2 : -1;
        }

        if (stride < 0) {
            stride += size;
            if (stride <= 0) {
                return -1;
            }
        }

        return stride;
    }

    template<int>
    struct _sliding;

    constexpr int sliding_select(int n, int_ size, int_ stride);

    template<int_ size, int_ stride, class C>
    struct sliding_
    {
        template<class... xs>
        using f = typename detail::_sliding<detail::sliding_select(
            sizeof...(xs), size, stride)
        >::template f<C, size, stride, xs...>;
    };

    template<int_ size, int_, int_ stride>
    struct mk_sliding
    {
        template<class C>
        using f = sliding_<size, stride, optimize_useless_transform_unpack_t<C>>;
    };

    // TODO generic class for error
    template<class C>
    struct sliding_nostride
    {};

    template<class C>
    struct sliding_0size
    {
        template<class... xs>
        using f = JLN_MP_DCALL_XS(xs, C);
    };

    template<int_ size, int_ stride>
    struct mk_sliding<size, stride, -1>
    {
        template<class C>
        using f = sliding_nostride<C>;
    };

    template<int_ size, int_ stride>
    struct mk_sliding<size, stride, -2>
    {
        template<class C>
        using f = sliding_0size<C>;
    };

    constexpr int_ slinding_pivot(int_ nx, int_ size, int_ stride)
    {
        return nx - (nx - size + stride - 1) / stride * stride - 1;
    }

    constexpr int sliding_select(int n, int_ size, int_ stride)
    {
        return n <= size ? (n ? 1 : 0)         // C<list<xs...>>
            : size == 1 ? (stride == 1 ? 2 : 3)  // C<list<xs>...> / slice<0, n/stride, stride>
            : stride > n ? 9                     // take_front
            : stride == 1 ? (size == 2 ? 4 : 5)  // common case
            : stride == size ? 6                 // group_n
            // slice<zip_longest> / slice<zip>
            : ((n - size) % stride) ? (slinding_pivot(n, size, stride) < 0 ? 7 : 8) : 7;
    }

    template<>
    struct _sliding<-1>
    {};

    template<>
    struct _sliding<0>
    {
        template<class C, int_, int_, class...>
        using f = typename C::template f<>;
    };

    template<>
    struct _sliding<1>
    {
        template<class C, int_, int_, class... xs>
        using f = typename C::template f<list<xs...>>;
    };

    template<>
    struct _sliding<2>
    {
        template<class C, int_, int_, class... xs>
        using f = typename C::template f<list<xs>...>;
    };

    template<>
    struct _sliding<3>
    {
        template<class C, int_, int_ stride, class... xs>
        using f = typename _slice<1>
            ::template f<0, (sizeof...(xs) + stride - 1) / stride, stride, transform<listify, C>, sizeof...(xs)>
            ::template f<xs...>;
    };

    template<>
    struct _sliding<4>
    {
        template<class C, int_, int_, class x, class... xs>
        using f = typename _zip_impl<
            C,
            mp::rotate_c<-1, drop_front_c<1>>::f<x, xs...>,
            list<xs...>
        >::type;
    };

    template<>
    struct _sliding<5>
    {
        template<class C, int_ size, class drop_front>
        struct impl
        {
            template<int_... i>
            using f = _tee<zip<C>, rotate_c<i - size, drop_front>...>;
        };

        template<class C, int_ size, int_, class... xs>
        using f = typename memoize_call<
            make_int_sequence_v<impl<C, size - 1, drop_front_c<size - 1>>>,
            number<size>
        >::template f<xs...>;
    };

    template<>
    struct _sliding<6>
    {
        template<class C, int_ size, int_, class... xs>
        using f = typename emp::make_int_sequence_v_c<
            sizeof...(xs), lift_c<_group_n_impl>
        >::template f<C, size, xs...>;
    };

    template<>
    struct _sliding<7>
    {
        template<class C, int_ size, int_ stride>
        struct impl
        {
            template<size_t... i>
            using f = _tee<zip<C>, slice_c<i, size, stride>...>;
        };

        template<class C, int_ size, int_ stride, class... xs>
        using f = typename memoize_call<
            make_int_sequence_v<impl<C, (sizeof...(xs) - size) / stride + 1, stride>>,
            number<size>
        >::template f<xs...>;
    };

    constexpr int_ slinding8_pivot(int_ nx, int_ size, int_ stride)
    {
        int_ r = slinding_pivot(nx, size, stride);
        return r < 0 ? size : r;
    }

    template<>
    struct _sliding<8>
    {
        template<class F, class C>
        struct adjust
        {
            template<class x, class... xs>
            using f = typename C::template f<xs..., typename F::template f<x>>;
        };

        template<class C, int_ size, int_ stride, int_ pivot>
        struct impl
        {
            template<int_... i>
            using f = _tee<zip<
                rotate_c<-1, adjust<unpack<rotate_c<-1, pop_front<>>>, C>>
            >, slice_c<i, size - (pivot < i), stride,
                typename conditional_c<(pivot < i)>::template f<push_back<void>, listify>
                >...>;
        };

        template<class C, int_ size, int_ stride, class... xs>
        using f = typename memoize_call<
            make_int_sequence_v<impl<
            C,
            (sizeof...(xs) - size) / stride + 2,
            stride,
            slinding8_pivot(sizeof...(xs), size, stride)
            >>,
            number<size>
            >::template f<xs...>;
    };

    template<>
    struct _sliding<9>
    {
        template<class C, int_ size, int_, class... xs>
        using f = typename C::template f<typename take_front_c<size>::template f<xs...>>;
    };
}
/// \endcond
namespace jln::mp
{
    /// \cond
    namespace detail
    {
        template<unsigned i, unsigned j, bool = (i < j), bool = (i == j)>
            struct _select_swap_index;
    }
    /// \endcond

    /// \ingroup list

    /// Swap elements at indexes \c I and \c J of a \sequence.
    /// \pre `0 < I < sizeof...(xs)`
    /// \pre `0 < J < sizeof...(xs)`
    /// \treturn \sequence
    /// \note `swap_index<I, J>` == `swap_index<J, I>`
    template<class I, class J, class C = listify>
    using swap_index = typename detail::_select_swap_index<
        unsigned{ I::value }, unsigned{ J::value } > ::template f<C>;

    template<unsigned i, unsigned j, class C = listify>
    using swap_index_c = typename detail::_select_swap_index<
        i, j>::template f<C>;

    namespace emp
    {
        template<class L, class I, class J, class C = mp::listify>
        using swap_index = unpack<L, swap_index<I, J, C>>;

        template<class L, unsigned i, unsigned j, class C = mp::listify>
        using swap_index_c = unpack<L, swap_index_c<i, j, C>>;
    }
}


/// \cond
namespace jln::mp::detail
{
    template<unsigned i, unsigned j, class C>
    struct swap_elem
    {
        template<class... xs>
        using f = typename join<C>::template f<
            typename take_front_c<i>::template f<xs...>,
            list<typename at_c<j>::template f<xs...>>,
            typename drop_front_c<i + 1, take_front_c<j - i - 1>>::template f<xs...>,
            list<typename at_c<i>::template f<xs...>>,
            typename drop_front_c<j + 1>::template f<xs...>
        >;
    };

    template<unsigned i, unsigned j, bool, bool>
    struct _select_swap_index
    {
        template<class C>
        using f = swap_elem<i, j, C>;
    };

    template<unsigned i, unsigned j>
    struct _select_swap_index<i, j, false, false>
    {
        template<class C>
        using f = swap_elem<j, i, C>;
    };

    template<unsigned i, class C>
    struct if_valid_index
    {
        template<class... xs>
        using f = JLN_MP_DCALL(unsigned{ int(sizeof...(xs)) - int{i} } < JLN_MP_MAX_CALL_ELEMENT, C, xs...);
    };

    template<unsigned i, unsigned j>
    struct _select_swap_index<i, j, false, true>
    {
        template<class C>
        using f = if_valid_index<i, C>;
    };
}
/// \endcond
namespace jln::mp
{
    /// \ingroup number

    /// Narrowing convertion from \value to \bool.
    /// \treturn \bool
    template<class C = identity>
    struct as_bool
    {
        template<class x>
        using f = call < C, number<bool{ x::value } >> ;
    };

    namespace emp
    {
        template<class x>
        using as_bool = number<bool{ x::value } > ;
    }
}
namespace jln::mp
{
    /// \ingroup number

    /// Narrowing convertion from \value to \number.
    /// \treturn \number
    template<class C = identity>
    struct as_number
    {
        template<class x>
        using f = call < C, number < int_{ x::value } >> ;
    };

    namespace emp
    {
        template<class x>
        using as_number = number < int_{ x::value } > ;
    }
}
namespace jln::mp
{
    /// \cond
    namespace detail
    {
        template<class x>
        struct _is_number;
    }
    /// \endcond

    /// \ingroup number

    /// Checks whether a \value is a \number.
    /// \treturn \bool
    template<class C = identity>
    struct is_number
    {
        template<class x>
        using f = call<C, typename detail::_is_number<x>::type>;
    };

    namespace emp
    {
        template<class x>
        using is_number = typename detail::_is_number<x>::type;
    }
}


/// \cond
namespace jln::mp::detail
{
    template<class x>
    struct _is_number
    {
        using type = false_;
    };

    template<int_ x>
    struct _is_number<number<x>>
    {
        using type = true_;
    };
}
/// \endcond
namespace jln::mp
{
    /// \cond
    namespace detail
    {
        struct _pow;
    }
    /// \endcond

    /// \ingroup number

    template<class Cmp = less<>, class C = identity>
    using min = fold_left<if_<flip<Cmp>, at1<>, at0<>>, C>;

    template<class Cmp = less<>, class C = identity>
    using min0 = if_<size<>, min<Cmp, C>, always<number<0>, C>>;


    template<class Cmp = less<>, class C = identity>
    using max = fold_left<if_<Cmp, at1<>, at0<>>, C>;

    template<class Cmp = less<>, class C = identity>
    using max0 = if_<size<>, max<Cmp, C>, always<number<0>, C>>;


    template<class Min, class Max, class Cmp = less<>, class C = identity>
    using clamp = if_<push_back<Min, Cmp>, always<Min>,
        if_<push_front<Max, Cmp>, always<Max>, identity>>;

    template<int_ min, int_ max, class Cmp = less<>, class C = identity>
    using clamp_c = clamp<number<min>, number<max>, Cmp, C>;


    template<class Cmp = less<>, class C = identity>
    using abs = tee<identity, neg<>, max<Cmp, C>>;


    template<class C = identity>
    using pow = fold_left<detail::_pow, C>;

    template<class C = identity>
    using pow0 = if_<size<>, pow<C>, always<number<0>, C>>;

    template<class C = identity>
    using pow1 = if_<size<>, pow<C>, always<number<1>, C>>;


    namespace emp
    {
        template<class L, class Cmp = mp::less<>, class C = mp::identity>
        using min = unpack<L, mp::min<Cmp, C>>;

        template<class L, class Cmp = mp::less<>, class C = mp::identity>
        using min0 = unpack<L, mp::min0<Cmp, C>>;

        template<class L, class Cmp = mp::less<>, class C = mp::identity>
        using max = unpack<L, mp::max<Cmp, C>>;

        template<class L, class Cmp = mp::less<>, class C = mp::identity>
        using max0 = unpack<L, mp::max0<Cmp, C>>;

        template<class L, class Min, class Max, class Cmp = mp::less<>, class C = mp::identity>
        using clamp = unpack<L, mp::clamp<Min, Max, Cmp, C>>;

        template<class L, int_ min, int_ max, class Cmp = mp::less<>, class C = mp::identity>
        using clamp_c = unpack<L, mp::clamp_c<min, max, Cmp, C>>;

        template<class L, class Cmp = mp::less<>, class C = mp::identity>
        using abs = unpack<L, mp::abs<Cmp, C>>;

        template<class L, class C = mp::identity>
        using pow = unpack<L, mp::pow<C>>;

        template<class L, class C = mp::identity>
        using pow0 = unpack<L, mp::pow0<C>>;

        template<class L, class C = mp::identity>
        using pow1 = unpack<L, mp::pow1<C>>;
    }
}

/// \cond
namespace jln::mp::detail
{
    template<int_ b, int_ e, int_ r = 1>
    struct _ipow
        : _ipow<(b* b), (e / 2), (e % 2 ? b * r : r)>
    {};

    struct _pow
    {
        template<class base, class exponent>
        using f = typename _ipow<base::value, exponent::value, 1>::type;
    };

    template<int_ b, int_ r>
    struct _ipow<b, 0, r>
    {
        using type = number<1>;
    };

    template<int_ b, int_ r>
    struct _ipow<b, 1, r>
    {
        using type = number<b* r>;
    };

    template<int_ b, int_ r>
    struct _ipow<b, -1, r>
    {
        using type = number<1 / (b * r)>;
    };

    template<int_ r>
    struct _ipow<0, -1, r>
    {};
}
/// \endcond
namespace jln::mp
{
    /// \ingroup trait

    /// Wrapper for alignof keyword
    /// \treturn \number
    template<class C = identity>
    struct alignof_
    {
        template<class x>
        using f = typename C::template f<number<alignof(x)>>;
    };

    namespace emp
    {
        template<class x>
        using alignof_ = number<alignof(x)>;
    }
}
namespace jln::mp
{
    /// \cond
    namespace detail
    {
        template<class x, class = void>
        struct _has_type;
    }
    /// \endcond

    /// \ingroup trait

    /// Checks whether a \value to a type member.
    /// \treturn \bool
    template<class C = identity>
    struct has_type
    {
        template<class x>
        using f = typename C::template f<
            typename detail::_has_type<x>::type>;
    };

    namespace emp
    {
        template<class x>
        using has_type = typename detail::_has_type<x>::type;

        template<class x>
        inline constexpr bool has_type_v = detail::_has_type<x>::type::value;
    }
}

/// \cond
namespace jln::mp::detail
{
    template<class x, class>
    struct _has_type
    {
        using type = false_;
    };

    template<class x>
    struct _has_type<x, std::void_t<typename x::type>>
    {
        using type = true_;
    };
}
/// \endcond
namespace jln::mp
{
    /// \cond
    namespace detail
    {
        template<class x, class = void>
        struct _has_value_type;
    }
    /// \endcond

    /// \ingroup trait

    /// Checks whether a \value to a type member.
    /// \treturn \bool
    template<class C = identity>
    struct has_value_type
    {
        template<class x>
        using f = typename C::template f<
            typename detail::_has_value_type<x>::type>;
    };

    namespace emp
    {
        template<class x>
        using has_value_type = typename detail::_has_value_type<x>::type;

        template<class x>
        inline constexpr bool has_value_type_v = detail::_has_value_type<x>::type::value;
    }
}

/// \cond
namespace jln::mp::detail
{
    template<class x, class>
    struct _has_value_type
    {
        using type = false_;
    };

    template<class x>
    struct _has_value_type<x, std::void_t<typename x::type>>
    {
        using type = true_;
    };
}
/// \endcond
namespace jln::mp
{
    /// \ingroup utility

    /// \treturn \bool
    template <class T, class C = identity>
    using is_not = is<T, not_<C>>;
} // namespace jln::mp
namespace jln::mp
{
    /// \cond
    namespace detail
    {
        template<template<class...> class Tpl, class T>
        struct _is_specialization_of;
    }
    /// \endcond

    /// \ingroup trait

    /// Checks whether \c x is \c Tpl<xs...>
    /// \treturn \bool
    template<template<class...> class Tpl, class C = identity>
    struct is_specialization_of
    {
        template<class x>
        using f = typename C::template f<
            typename detail::_is_specialization_of<Tpl, x>::type>;
    };

    namespace emp
    {
        template<template<class...> class Tpl, class x>
        using is_specialization_of = typename detail::_is_specialization_of<Tpl, x>::type;
    }
}

/// \cond
namespace jln::mp::detail
{
    template<template<class...> class Tpl, class T>
    struct _is_specialization_of
    {
        using type = false_;
    };

    template<template<class...> class Tpl, class... Ts>
    struct _is_specialization_of<Tpl, Tpl<Ts...>>
    {
        using type = true_;
    };
}
/// \endcond
namespace jln::mp
{
    /// \cond
    namespace detail
    {
        template<uint_>
        struct _iterate;
    }
    /// \endcond

    /// \ingroup utility

    /// Apply a function \c n times to its argument.
    /// \treturn \value
    template <class n, class F, class C = identity>
    struct iterate
    {
        template<class x>
        using f = typename detail::_iterate<detail::n_8_or_more_16_32_64_128_256(n::value)>
            ::template f<n::value, C, F, x>;
    };

    /// Apply a function \c n times to its argument.
    /// \treturn \value
    template <uint_ n, class F, class C = identity>
    using iterate_c = iterate<number<n>, F, C>;

    namespace emp
    {
        template<class L, class n, class F, class C = mp::identity>
        using iterate = unpack<L, mp::iterate<n, F, C>>;

        template<class L, uint_ n, class F, class C = mp::identity>
        using iterate_c = unpack<L, mp::iterate<number<n>, F, C>>;
    }
} // namespace jln::mp


/// \cond
namespace jln::mp::detail
{
    template<>
    struct _iterate<0>
    {
        template<uint_ i, class C, class F, class x>
        using f = typename C::template f<x>;
    };

#define JLN_MP_ITERATE(n, mp_xs, mp_rsx, mp_rep)  \
  template<>                                      \
  struct _iterate<n>                              \
  {                                               \
    template<uint_ i, class C, class F, class x>  \
    using f = typename C::template f<             \
      mp_rep(typename F::template f<, JLN_MP_NIL) \
      x                                           \
      mp_rep(>, JLN_MP_NIL)                       \
    >;                                            \
  };

    JLN_MP_GEN_XS_1_TO_8(JLN_MP_ITERATE)

#undef JLN_MP_ITERATE

#define JLN_MP_ITERATE(n, mp_xs, mp_rsx, mp_rep)                   \
  template<>                                                       \
  struct _iterate<n>                                               \
  {                                                                \
    template<uint_ i, class C, class F, class x>                   \
    using f = typename _iterate<n_8_or_more_16_32_64_128_256(i-n)> \
      ::template f<i-n, C, F,                                      \
        mp_rep(typename F::template f<, JLN_MP_NIL)                \
        x                                                          \
        mp_rep(>, JLN_MP_NIL)                                      \
      >;                                                           \
  };

        JLN_MP_GEN_XS_16_32_64_128_256(JLN_MP_ITERATE)

#undef JLN_MP_ITERATE
}
/// \endcond
namespace jln::mp
{
    /// \ingroup trait

    /// Wrapper for sizeof keyword
    /// \treturn \number
    template<class C = identity>
    struct sizeof_
    {
        template<class x>
        using f = typename C::template f<number<sizeof(x)>>;
    };

    namespace emp
    {
        template<class x>
        using sizeof_ = number<sizeof(x)>;
    }
}
namespace jln::mp::traits
{
    /// \ingroup trait

#define JLN_MP_MAKE_TRAIT(Name)                   \
  template<class C = identity>                    \
  struct Name                                     \
  {                                               \
    template<class... xs>                         \
    using f = JLN_MP_DCALL_XS(xs, C,              \
      typename std::Name<xs...>::type);           \
  };                                              \
                                                  \
  namespace emp                                   \
  {                                               \
    template<class... xs>                         \
    using Name = typename std::Name<xs...>::type; \
  }                                               \
                                                  \
  template<>                                      \
  struct Name<identity>                           \
  {                                               \
    template<class... xs>                         \
    using f = typename std::Name<xs...>::type;    \
  }


  // primary type categories:
#if __cplusplus >= 201703L
    JLN_MP_MAKE_TRAIT(is_void);
#endif
#if __cplusplus >= 201402L
    JLN_MP_MAKE_TRAIT(is_null_pointer);
#endif
    JLN_MP_MAKE_TRAIT(is_integral);
    JLN_MP_MAKE_TRAIT(is_floating_point);
    JLN_MP_MAKE_TRAIT(is_array);
    JLN_MP_MAKE_TRAIT(is_pointer);
    JLN_MP_MAKE_TRAIT(is_lvalue_reference);
    JLN_MP_MAKE_TRAIT(is_rvalue_reference);
    JLN_MP_MAKE_TRAIT(is_member_object_pointer);
    JLN_MP_MAKE_TRAIT(is_member_function_pointer);
    JLN_MP_MAKE_TRAIT(is_enum);
    JLN_MP_MAKE_TRAIT(is_union);
    JLN_MP_MAKE_TRAIT(is_class);
    JLN_MP_MAKE_TRAIT(is_function);

    // composite type categories:
    JLN_MP_MAKE_TRAIT(is_reference);
    JLN_MP_MAKE_TRAIT(is_arithmetic);
    JLN_MP_MAKE_TRAIT(is_fundamental);
    JLN_MP_MAKE_TRAIT(is_object);
    JLN_MP_MAKE_TRAIT(is_scalar);
    JLN_MP_MAKE_TRAIT(is_compound);
    JLN_MP_MAKE_TRAIT(is_member_pointer);

    // type properties:
    JLN_MP_MAKE_TRAIT(is_const);
    JLN_MP_MAKE_TRAIT(is_volatile);
    JLN_MP_MAKE_TRAIT(is_trivial);
    JLN_MP_MAKE_TRAIT(is_trivially_copyable);
    JLN_MP_MAKE_TRAIT(is_standard_layout);
#if __cplusplus <= 201703L
    JLN_MP_MAKE_TRAIT(is_pod);
#endif
    JLN_MP_MAKE_TRAIT(is_empty);
    JLN_MP_MAKE_TRAIT(is_polymorphic);
    JLN_MP_MAKE_TRAIT(is_abstract);
#if __cplusplus >= 201402L
    JLN_MP_MAKE_TRAIT(is_final);
#endif
#if __cplusplus >= 201703L
    JLN_MP_MAKE_TRAIT(is_aggregate);
#endif
    JLN_MP_MAKE_TRAIT(is_signed);
    JLN_MP_MAKE_TRAIT(is_unsigned);
#if defined(__cpp_lib_bounded_array_traits) && __cpp_lib_bounded_array_traits
    JLN_MP_MAKE_TRAIT(is_bounded_array);
    JLN_MP_MAKE_TRAIT(is_unbounded_array);
#endif
    JLN_MP_MAKE_TRAIT(is_constructible);
    JLN_MP_MAKE_TRAIT(is_default_constructible);
    JLN_MP_MAKE_TRAIT(is_copy_constructible);
    JLN_MP_MAKE_TRAIT(is_move_constructible);
    JLN_MP_MAKE_TRAIT(is_assignable);
    JLN_MP_MAKE_TRAIT(is_copy_assignable);
    JLN_MP_MAKE_TRAIT(is_move_assignable);
#if __cplusplus >= 201703L
    JLN_MP_MAKE_TRAIT(is_swappable_with);
    JLN_MP_MAKE_TRAIT(is_swappable);
    JLN_MP_MAKE_TRAIT(is_nothrow_swappable_with);
    JLN_MP_MAKE_TRAIT(is_nothrow_swappable);
#endif
    JLN_MP_MAKE_TRAIT(is_destructible);
    JLN_MP_MAKE_TRAIT(is_trivially_constructible);
    JLN_MP_MAKE_TRAIT(is_trivially_default_constructible);
    JLN_MP_MAKE_TRAIT(is_trivially_copy_constructible);
    JLN_MP_MAKE_TRAIT(is_trivially_move_constructible);
    JLN_MP_MAKE_TRAIT(is_trivially_assignable);
    JLN_MP_MAKE_TRAIT(is_trivially_copy_assignable);
    JLN_MP_MAKE_TRAIT(is_trivially_move_assignable);
    JLN_MP_MAKE_TRAIT(is_trivially_destructible);
    JLN_MP_MAKE_TRAIT(is_nothrow_constructible);
    JLN_MP_MAKE_TRAIT(is_nothrow_default_constructible);
    JLN_MP_MAKE_TRAIT(is_nothrow_copy_constructible);
    JLN_MP_MAKE_TRAIT(is_nothrow_move_constructible);
    JLN_MP_MAKE_TRAIT(is_nothrow_assignable);
    JLN_MP_MAKE_TRAIT(is_nothrow_copy_assignable);
    JLN_MP_MAKE_TRAIT(is_nothrow_move_assignable);
    JLN_MP_MAKE_TRAIT(is_nothrow_destructible);
    JLN_MP_MAKE_TRAIT(has_virtual_destructor);
#if __cplusplus >= 201703L
    JLN_MP_MAKE_TRAIT(has_unique_object_representations);
#endif

    // type property queries:
    JLN_MP_MAKE_TRAIT(alignment_of);
    JLN_MP_MAKE_TRAIT(rank);

    template<class C = identity>
    struct extent
    {
        template<class x, class... i>
        using f = call<C, typename std::extent<x, i::value...>::type>;
    };

    namespace emp
    {
        template<class x, class i = number<0>>
        using extent = typename std::extent<x, i::value>::type;
    }

    /// \cond
    template<>
    struct extent<identity>
    {
        template<class x, class i = number<0>>
        using f = typename std::extent<x, i::value>::type;
    };
    /// \endcond

    // type relations:
    JLN_MP_MAKE_TRAIT(is_same);
    JLN_MP_MAKE_TRAIT(is_base_of);
    JLN_MP_MAKE_TRAIT(is_convertible);
#if defined(__cpp_lib_is_nothrow_convertible) && __cpp_lib_is_nothrow_convertible
    JLN_MP_MAKE_TRAIT(is_nothrow_convertible);
#endif
#if defined(__cpp_lib_is_layout_compatible) && __cpp_lib_is_layout_compatible
    JLN_MP_MAKE_TRAIT(is_layout_compatible);
#endif
#if defined(__cpp_lib_is_pointer_interconvertible) && __cpp_lib_is_pointer_interconvertible
    JLN_MP_MAKE_TRAIT(is_pointer_interconvertible_base_of);
#endif
#if __cplusplus >= 201703L
    JLN_MP_MAKE_TRAIT(is_invocable);
    JLN_MP_MAKE_TRAIT(is_invocable_r);
    JLN_MP_MAKE_TRAIT(is_nothrow_invocable);
    JLN_MP_MAKE_TRAIT(is_nothrow_invocable_r);
#endif

    // const-volatile modifications:
    JLN_MP_MAKE_TRAIT(remove_const);
    JLN_MP_MAKE_TRAIT(remove_volatile);
    JLN_MP_MAKE_TRAIT(remove_cv);
    JLN_MP_MAKE_TRAIT(add_const);
    JLN_MP_MAKE_TRAIT(add_volatile);
    JLN_MP_MAKE_TRAIT(add_cv);

    // reference modifications:
    JLN_MP_MAKE_TRAIT(remove_reference);
    JLN_MP_MAKE_TRAIT(add_lvalue_reference);
    JLN_MP_MAKE_TRAIT(add_rvalue_reference);

    // sign modifications:
    JLN_MP_MAKE_TRAIT(make_signed);
    JLN_MP_MAKE_TRAIT(make_unsigned);

    // array modifications:
    JLN_MP_MAKE_TRAIT(remove_extent);
    JLN_MP_MAKE_TRAIT(remove_all_extents);

    // pointer modifications:
    JLN_MP_MAKE_TRAIT(remove_pointer);
    JLN_MP_MAKE_TRAIT(add_pointer);

    // other transformations:
    JLN_MP_MAKE_TRAIT(decay);
#if defined(__cpp_lib_unwrap_ref) && __cpp_lib_unwrap_ref
    JLN_MP_MAKE_TRAIT(unwrap_ref_decay);
    JLN_MP_MAKE_TRAIT(unwrap_reference);
#endif
#if defined(__cpp_lib_remove_cvref) && __cpp_lib_remove_cvref
    JLN_MP_MAKE_TRAIT(remove_cvref);
#endif
#if __cplusplus >= 202002L
    JLN_MP_MAKE_TRAIT(common_reference);
    JLN_MP_MAKE_TRAIT(basic_common_reference);
#else
    JLN_MP_MAKE_TRAIT(result_of);
#endif
    JLN_MP_MAKE_TRAIT(underlying_type);
    JLN_MP_MAKE_TRAIT(common_type);
#if __cplusplus >= 201703L
    JLN_MP_MAKE_TRAIT(invoke_result);
#endif

    template<class C = identity>
    struct aligned_storage
    {
        template<class Len, class... Alignment>
        using f = call<C, typename std::aligned_storage<Len::value, Alignment::value...>::type>;
    };

    namespace emp
    {
        template<class Len, class... Alignment>
        using aligned_storage = typename std::aligned_storage<
            Len::value, Alignment::value...>::type;
    }

    /// \cond
    template<>
    struct aligned_storage<identity>
    {
        template<class Len, class... Alignment>
        using f = typename std::aligned_storage<Len::value, Alignment::value...>::type;
    };
    /// \endcond

    template<class C = identity>
    struct aligned_union
    {
        template<class len, class... xs>
        using f = call<C, typename std::aligned_union<len::value, xs...>::type>;
    };

    namespace emp
    {
        template<class len, class... xs>
        using aligned_union = typename std::aligned_union<len::value, xs...>::type;
    }

    /// \cond
    template<>
    struct aligned_union<identity>
    {
        template<class len, class... xs>
        using f = typename std::aligned_union<len::value, xs...>::type;
    };
    /// \endcond
}
namespace jln::mp
{
    /// \ingroup trait

    /// Function for \c x::type.
    /// \treturn \value
    template<class C = identity>
    struct type_
    {
        template<class x>
        using f = typename C::template f<typename x::type>;
    };

    template<>
    struct type_<identity>
    {
        template<class x>
        using f = typename x::type;
    };

    namespace emp
    {
        template<class x>
        using type_ = typename x::type;
    }
}
namespace jln::mp
{
    /// \ingroup trait

    /// Function for \c x::value_type.
    /// \treturn \value
    template<class C = identity>
    struct value_type
    {
        template<class x>
        using f = typename C::template f<typename x::value_type>;
    };

    template<>
    struct value_type<identity>
    {
        template<class x>
        using f = typename x::value_type;
    };

    namespace emp
    {
        template<class x>
        using value_type = typename x::value_type;
    }
}
namespace jln::mp
{
    /// \ingroup value

    /// \cond
#ifdef _MSC_VER
    namespace detail
    {
        template<class x>
        struct _one
        {
            using type = x;
        };
    }
#endif
    /// \endcond

    /// Converts \c x to \val.
    /// \pre \c emp::has_value<x> == \c true
    /// \treturn \bool
    template<class C = identity>
    struct as_val
    {
#if __cplusplus >= 201703L
# ifndef _MSC_VER
        template<class x>
        using f = JLN_MP_DCALL(sizeof(C), C, val<x::value>);
# else
        template<class... xs>
        using f = typename detail::_memoizer_impl<C, list<
            typename detail::_one<val<xs::value>...>::type
            >>::type;
# endif
#else
# ifndef _MSC_VER
        template<class x>
        using f = JLN_MP_DCALL(sizeof(C), C, typed_value<decltype(x::value), x::value>);
# else
        template<class... xs>
        using f = typename detail::_memoizer_impl<C, list<
            typename detail::_one<typed_value<decltype(x::value), x::value>...>::type
            >>::type;
# endif
#endif
    };

    /// \cond
    template<>
    struct as_val<identity>
    {
#if __cplusplus >= 201703L
# ifndef _MSC_VER
        template<class x>
        using f = val<x::value>;
# else
        template<class... xs>
        using f = typename detail::_one<val<xs::value>...>::type;
# endif
#else
# ifndef _MSC_VER
        template<class x>
        using f = typed_value<decltype(x::value), x::value>;
# else
        template<class... xs>
        using f = typename detail::_one<typed_value<decltype(xs::value), xs::value>...>::type;
# endif
#endif
    };
    /// \endcond

    namespace emp
    {
        template<class x, class C = mp::identity>
        using as_val = typename mp::as_val<C>::template f<x>;
    }
}
namespace jln::mp
{
    /// \cond
    namespace detail
    {
        template<class x, class = void>
        struct _has_value;
    }
    /// \endcond

    /// \ingroup value

    /// Checks whether \c x has a value member.
    /// \treturn \bool
    template<class C = identity>
    struct has_value
    {
        template<class x>
        using f = typename C::template f<
            typename detail::_has_value<x>::type>;
    };

    namespace emp
    {
        template<class x>
        using has_value = typename detail::_has_value<x>::type;

        template<class x>
        inline constexpr bool has_value_v = detail::_has_value<x>::type::value;
    }
}

/// \cond
namespace jln::mp::detail
{
    template<class x, class>
    struct _has_value
    {
        using type = false_;
    };

    template<class x>
    struct _has_value<x, decltype(void(x::value))>
    {
        using type = true_;
    };
}
/// \endcond
namespace jln::mp
{
    /// \cond
    namespace detail
    {
        template<class x>
        struct _is_val;
    }
    /// \endcond

    /// \ingroup value

    /// Checks whether \c x is a \val.
    /// \treturn \bool
    template<class C = identity>
    struct is_val
    {
        template<class x>
        using f = call<C, typename detail::_is_val<x>::type>;
    };

    namespace emp
    {
        template<class x>
        using is_val = typename detail::_is_val<x>::type;
    }
}


/// \cond
namespace jln::mp::detail
{
    template<class x>
    struct _is_val
    {
        using type = false_;
    };

#if __cplusplus >= 201703L
    template<auto x>
    struct _is_val<val<x>>
#else
    template<class T, T x>
    struct _is_val<typed_value<T, x>>
#endif
    {
        using type = true_;
    };
}
/// \endcond
JLN_DIAGNOSTIC_PUSH
JLN_DIAGNOSTIC_GCC_ONLY_IGNORE("-Wlogical-op")

namespace jln::mp
{
    /// \ingroup value

    template<class C = identity>
    struct val_or
    {
        template<class... xs>
        using f = typename C::template f<val<(false || ... || xs::value)>>;
    };

    template<class C = identity>
    struct val_and
    {
        template<class... xs>
        using f = typename C::template f<val<(true && ... && xs::value)>>;
    };

    template<class C = identity>
    struct val_add
    {
        template<class... xs>
        using f = typename C::template f<val<(... + xs::value)>>;
    };

    template<class C = identity>
    using val_add0 = if_<size<>, val_add<C>, always<val<0>, C>>;

    template<class C = identity>
    struct val_sub
    {
        template<class... xs>
        using f = typename C::template f<val<(... - xs::value)>>;
    };

    template<class C = identity>
    using val_sub0 = if_<size<>, val_sub<C>, always<val<0>, C>>;

    template<class C = identity>
    struct val_lshift
    {
        template<class... xs>
        using f = typename C::template f<val<(... << xs::value)>>;
    };

    template<class C = identity>
    using val_lshift0 = if_<size<>, val_lshift<C>, always<val<0>, C>>;

    template<class C = identity>
    struct val_rshift
    {
        template<class... xs>
        using f = typename C::template f<val<(... >> xs::value)>>;
    };

    template<class C = identity>
    using val_rshift0 = if_<size<>, val_rshift<C>, always<val<0>, C>>;

    template<class C = identity>
    struct val_mul
    {
        template<class... xs>
        using f = typename C::template f<val<(... * xs::value)>>;
    };

    template<class C = identity>
    using val_mul0 = if_<size<>, val_mul<C>, always<val<0>, C>>;

    template<class C = identity>
    using val_mul1 = if_<size<>, val_mul<C>, always<val<1>, C>>;

    template<class C = identity>
    struct val_div
    {
        template<class... xs>
        using f = typename C::template f<val<(... / xs::value)>>;
    };

    template<class C = identity>
    using val_div0 = if_<size<>, val_div<C>, always<val<0>, C>>;

    template<class C = identity>
    using val_div1 = if_<size<>, val_div<C>, always<val<1>, C>>;

    template<class C = identity>
    struct val_mod
    {
        template<class... xs>
        using f = typename C::template f<val<(... % xs::value)>>;
    };

    template<class C = identity>
    using val_mod0 = if_<size<>, val_mod<C>, always<val<0>, C>>;

    template<class C = identity>
    using val_mod1 = if_<size<>, val_mod<C>, always<val<1>, C>>;

    template<class C = identity>
    struct val_xor
    {
        template<class... xs>
        using f = typename C::template f<val<(... ^ xs::value)>>;
    };

    template<class C = identity>
    using val_xor0 = if_<size<>, val_xor<C>, always<val<0>, C>>;

    template<class C = identity>
    struct val_bit_and
    {
        template<class... xs>
        using f = typename C::template f<val<(... & xs::value)>>;
    };

    template<class C = identity>
    using val_bit_and0 = if_<size<>, val_bit_and<C>, always<val<0>, C>>;

    template<class C = identity>
    struct val_bit_or
    {
        template<class... xs>
        using f = typename C::template f<val<(... | xs::value)>>;
    };

    template<class C = identity>
    using val_bit_or0 = if_<size<>, val_bit_or<C>, always<val<0>, C>>;

    template<class C = identity>
    struct val_neg
    {
        template<class x>
        using f = typename C::template f<val<(-x::value)>>;
    };

    template<class C = identity>
    struct val_unary_plus
    {
        template<class x>
        using f = typename C::template f<val<(+x::value)>>;
    };

    template<class C = identity>
    struct val_not
    {
        template<class x>
        using f = typename C::template f<val<(!x::value)>>;
    };

    template<class C = identity>
    struct val_bit_not
    {
        template<class x>
        using f = typename C::template f<val<(~x::value)>>;
    };

    template<class C = identity>
    struct val_inc
    {
        template<class x>
        using f = typename C::template f<val<(x::value + 1)>>;
    };

    template<class C = identity>
    struct val_dec
    {
        template<class x>
        using f = typename C::template f<val<(x::value - 1)>>;
    };

    template<class C = identity>
    struct val_equal
    {
        template<class x, class y>
        using f = typename C::template f<val<(x::value == y::value)>>;
    };

    template<class C = identity>
    struct val_not_equal
    {
        template<class x, class y>
        using f = typename C::template f<val<(x::value != y::value)>>;
    };

    template<class C = identity>
    struct val_less
    {
        template<class x, class y>
        using f = typename C::template f<val<(x::value < y::value)>>;
    };

    template<class C = identity>
    struct val_less_equal
    {
        template<class x, class y>
        using f = typename C::template f<val<(x::value <= y::value)>>;
    };

    template<class C = identity>
    struct val_greater
    {
        template<class x, class y>
        using f = typename C::template f<val<(x::value > y::value)>>;
    };

    template<class C = identity>
    struct val_greater_equal
    {
        template<class x, class y>
        using f = typename C::template f<val<(x::value >= y::value)>>;
    };


    template<class N, class C = identity>
    using val_equal_to = push_back<N, val_equal<C>>;

    template<class N, class C = identity>
    using val_not_equal_to = push_back<N, val_not_equal<C>>;

    template<class N, class C = identity>
    using val_less_than = push_back<N, val_less<C>>;

    template<class N, class C = identity>
    using val_less_equal_than = push_back<N, val_less_equal<C>>;

    template<class N, class C = identity>
    using val_greater_than = push_back<N, val_greater<C>>;

    template<class N, class C = identity>
    using val_greater_equal_than = push_back<N, val_greater_equal<C>>;


    template<auto x, class C = identity>
    using val_equal_to_c = val_equal_to<val<x>, C>;

    template<auto x, class C = identity>
    using val_not_equal_to_c = val_not_equal_to<val<x>, C>;

    template<auto x, class C = identity>
    using val_less_than_c = val_less_than<val<x>, C>;

    template<auto x, class C = identity>
    using val_less_equal_than_c = val_less_equal_than<val<x>, C>;

    template<auto x, class C = identity>
    using val_greater_than_c = val_greater_than<val<x>, C>;

    template<auto x, class C = identity>
    using val_greater_equal_than_c = val_greater_equal_than<val<x>, C>;
}

namespace jln::mp::emp
{
    template<class L, class C = mp::identity>
    using val_or_seq = unpack<L, mp::val_or<C>>;

    template<class L, class C = mp::identity>
    using val_and_seq = unpack<L, mp::val_and<C>>;

    template<class L, class C = mp::identity>
    using val_add_seq = unpack<L, mp::val_add<C>>;

    template<class L, class C = mp::identity>
    using val_add0_seq = unpack<L, mp::val_add0<C>>;

    template<class L, class C = mp::identity>
    using val_sub_seq = unpack<L, mp::val_sub<C>>;

    template<class L, class C = mp::identity>
    using val_sub0_seq = unpack<L, mp::val_sub0<C>>;

    template<class L, class C = mp::identity>
    using val_lshift_seq = unpack<L, mp::val_lshift<C>>;

    template<class L, class C = mp::identity>
    using val_lshift0_seq = unpack<L, mp::val_lshift0<C>>;

    template<class L, class C = mp::identity>
    using val_rshift_seq = unpack<L, mp::val_rshift<C>>;

    template<class L, class C = mp::identity>
    using val_rshift0_seq = unpack<L, mp::val_rshift0<C>>;

    template<class L, class C = mp::identity>
    using val_mul_seq = unpack<L, mp::val_mul<C>>;

    template<class L, class C = mp::identity>
    using val_mul0_seq = unpack<L, mp::val_mul0<C>>;

    template<class L, class C = mp::identity>
    using val_mul1_seq = unpack<L, mp::val_mul1<C>>;

    template<class L, class C = mp::identity>
    using val_div_seq = unpack<L, mp::val_div<C>>;

    template<class L, class C = mp::identity>
    using val_div0_seq = unpack<L, mp::val_div0<C>>;

    template<class L, class C = mp::identity>
    using val_div1_seq = unpack<L, mp::val_div1<C>>;

    template<class L, class C = mp::identity>
    using val_mod_seq = unpack<L, mp::val_mod<C>>;

    template<class L, class C = mp::identity>
    using val_mod0_seq = unpack<L, mp::val_mod0<C>>;

    template<class L, class C = mp::identity>
    using val_mod1_seq = unpack<L, mp::val_mod1<C>>;

    template<class L, class C = mp::identity>
    using val_xor_seq = unpack<L, mp::val_xor<C>>;

    template<class L, class C = mp::identity>
    using val_xor0_seq = unpack<L, mp::val_xor0<C>>;

    template<class L, class C = mp::identity>
    using val_bit_and_seq = unpack<L, mp::val_bit_and<C>>;

    template<class L, class C = mp::identity>
    using val_bit_and0_seq = unpack<L, mp::val_bit_and0<C>>;

    template<class L, class C = mp::identity>
    using val_bit_or_seq = unpack<L, mp::val_bit_or<C>>;

    template<class L, class C = mp::identity>
    using val_bit_or0_seq = unpack<L, mp::val_bit_or0<C>>;


    template<auto... xs>
    using val_or_c = val<(false || ... || xs)>;

    template<auto... xs>
    using val_and_c = val<(true && ... && xs)>;

    template<auto... xs>
    using val_add_c = val<(... + xs)>;

    template<auto... xs>
    using val_add0_c = val_add_c<xs..., 0>;

    template<auto... xs>
    using val_sub_c = val<(... - xs)>;

    template<auto... xs>
    using val_sub0_c = val_sub_c<xs..., 0>;

    template<auto... xs>
    using val_lshift_c = val<(... << xs)>;

    template<auto... xs>
    using val_lshift0_c = val_lshift_c<xs..., 0>;

    template<auto... xs>
    using val_rshift_c = val<(... >> xs)>;

    template<auto... xs>
    using val_rshift0_c = val_rshift_c<xs..., 0>;

    template<auto... xs>
    using val_mul_c = val<(... * xs)>;

    template<auto... xs>
    using val_mul0_c = val_mul_c<xs..., (sizeof...(xs) ? 1 : 0)>;

    template<auto... xs>
    using val_mul1_c = val_mul_c<xs..., 1>;

    template<auto... xs>
    using val_div_c = val<(... / xs)>;

    template<auto... xs>
    using val_div0_c = val_div_c<xs..., (sizeof...(xs) ? 1 : 0)>;

    template<auto... xs>
    using val_div1_c = val_div_c<xs..., 1>;

    template<auto... xs>
    using val_mod_c = val<(... % xs)>;
}

/// \cond
namespace jln::mp::detail
{
    template<bool, template<auto...> class f, int_ d>
    struct val__emp_op;

    template<template<auto...> class F, int_ d>
    struct val__emp_op<true, F, d>
    {
        template<auto... xs>
        using f = F<xs...>;
    };

    template<template<auto...> class F, int_ d>
    struct val__emp_op<false, F, d>
    {
        template<auto...>
        using f = val<d>;
    };
}
/// \endcond

namespace jln::mp::emp
{
    template<auto... xs>
    using val_mod0_c = typename detail::val__emp_op<sizeof...(xs) != 0, val_mod_c, 0>
        ::template f<xs...>;

    template<auto... xs>
    using val_mod1_c = typename detail::val__emp_op<sizeof...(xs) != 0, val_mod_c, 1>
        ::template f<xs...>;

    template<auto... xs>
    using val_xor_c = val<(... ^ xs)>;

    template<auto... xs>
    using val_xor0_c = val_xor_c<xs..., 0, 0>;

    template<auto... xs>
    using val_bit_and_c = val<(... & xs)>;

    template<auto... xs>
    using val_bit_and0_c = typename detail::val__emp_op<sizeof...(xs) != 0, val_bit_and_c, 0>
        ::template f<xs...>;

    template<auto... xs>
    using val_bit_or_c = val<(... | xs)>;

    template<auto... xs>
    using val_bit_or0_c = typename detail::val__emp_op<sizeof...(xs) != 0, val_bit_or_c, 0>
        ::template f<xs...>;


    template<class... xs>
    using val_or = val<(false || ... || xs::value)>;

    template<class... xs>
    using val_and = val<(true && ... && xs::value)>;

    template<class... xs>
    using val_add = val<(... + xs::value)>;

    template<class... xs>
    using val_add0 = call<mp::val_add0<>, xs...>;

    template<class... xs>
    using val_sub = val<(... - xs::value)>;

    template<class... xs>
    using val_sub0 = call<mp::val_sub0<>, xs...>;

    template<class... xs>
    using val_lshift = val<(... << xs::value)>;

    template<class... xs>
    using val_lshift0 = call<mp::val_lshift0<>, xs...>;

    template<class... xs>
    using val_rshift = val<(... >> xs::value)>;

    template<class... xs>
    using val_rshift0 = call<mp::val_rshift0<>, xs...>;

    template<class... xs>
    using val_mul = val<(... * xs::value)>;

    template<class... xs>
    using val_mul0 = call<mp::val_mul0<>, xs...>;

    template<class... xs>
    using val_mul1 = call<mp::val_mul1<>, xs...>;

    template<class... xs>
    using val_div = val<(... / xs::value)>;

    template<class... xs>
    using val_div0 = call<mp::val_div0<>, xs...>;

    template<class... xs>
    using val_div1 = call<mp::val_div1<>, xs...>;

    template<class... xs>
    using val_mod = val<(... % xs::value)>;

    template<class... xs>
    using val_mod0 = call<mp::val_mod0<>, xs...>;

    template<class... xs>
    using val_mod1 = call<mp::val_mod1<>, xs...>;

    template<class... xs>
    using val_xor = val<(... ^ xs::value)>;

    template<class... xs>
    using val_xor0 = call<mp::val_xor0<>, xs...>;

    template<class... xs>
    using val_bit_and = val<(... & xs::value)>;

    template<class... xs>
    using val_bit_and0 = call<mp::val_bit_and0<>, xs...>;

    template<class... xs>
    using val_bit_or = val<(... | xs::value)>;

    template<class... xs>
    using val_bit_or0 = call<mp::val_bit_or0<>, xs...>;


    template<class x, class C = mp::identity>
    using val_neg = call<mp::val_neg<C>, x>;

    template<class x, class C = mp::identity>
    using val_unary_plus = call<mp::val_unary_plus<C>, x>;

    template<class x, class C = mp::identity>
    using val_not = call<mp::val_not<C>, x>;

    template<class x, class C = mp::identity>
    using val_bit_not = call<mp::val_bit_not<C>, x>;

    template<class x, class C = mp::identity>
    using val_inc = call<mp::val_inc<C>, x>;

    template<class x, class C = mp::identity>
    using val_dec = call<mp::val_dec<C>, x>;

    template<class x, class y, class C = mp::identity>
    using val_equal = call<mp::val_equal<C>, x, y>;

    template<class x, class y, class C = mp::identity>
    using val_not_equal = call<mp::val_not_equal<C>, x, y>;

    template<class x, class y, class C = mp::identity>
    using val_less = call<mp::val_less<C>, x, y>;

    template<class x, class y, class C = mp::identity>
    using val_less_equal = call<mp::val_less_equal<C>, x, y>;

    template<class x, class y, class C = mp::identity>
    using val_greater = call<mp::val_greater<C>, x, y>;

    template<class x, class y, class C = mp::identity>
    using val_greater_equal = call<mp::val_greater_equal<C>, x, y>;
}

JLN_DIAGNOSTIC_POP
#if __cplusplus >= 201703L
namespace jln::mp
{
    /// \ingroup value

    template<class C = listify>
    struct values
    {
        template<class... xs>
        using f = call<C, val<xs::value>...>;
    };

    template<class C = listify>
    struct typed_values
    {
        template<class T, class... xs>
        using f = call<C, val<T(xs::value)>...>;
    };

    namespace emp
    {
        template<auto... xs>
        using values = list<val<xs>...>;

        template<class T, T... xs>
        using typed_values = list<val<T(xs)>...>;
    }
} // namespace jln::mp
#else
namespace jln::mp
{
    template<class C>
    struct values
    {
        template<class... xs>
        using f = call<C, typed_val<decltype(xs::value), xs::value>...>;
    };

    template<class C>
    struct typed_values
    {
        template<class T, class... xs>
        using f = call<C, typed_val<T, xs::value>...>;
    };

    namespace emp
    {
        template<class T, T... xs>
        using typed_values = list<typed_val<T, xs>...>;
    }
}
#endif


#endif
