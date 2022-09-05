#pragma once

#include <tuple>
#include <functional>
#include <utility>

namespace nglpmt::native {

namespace detailed {

template<std::size_t N, std::size_t... Seq>
constexpr std::index_sequence<(N + Seq)...> add(std::index_sequence<Seq...>){
    return {};
}

template<std::size_t Min, std::size_t Max>
using make_index_range = decltype(add<Min>(std::make_index_sequence<Max-Min>()));

template <int I>
struct ignore {
    template <typename T>
    ignore(T&&){}
};

template <int I, class... Ts>
inline decltype(auto) getArg(Ts&&... ts) {
  return std::get<I>(std::forward_as_tuple(ts...));
}

template<class F, size_t ... Use, class ... ArgsW>
static decltype(auto) call(std::index_sequence<Use...>, F&& func, ArgsW&& ... args){
    return func(getArg<Use>(std::forward<ArgsW>(args)...)...);
};

template<auto F, class ... ArgsW, size_t ... Use>
static auto call_compiletime(std::index_sequence<Use...>, ArgsW&& ... args){
    return F(getArg<Use>(std::forward<ArgsW>(args)...)...);
};

template <class T, class Tuple>
struct tuple_type_index;

template <class T, class... Types>
struct tuple_type_index<T, std::tuple<T, Types...>> {
    static constexpr std::size_t value = 0;
};

template <class T, class U, class... Types>
struct tuple_type_index<T, std::tuple<U, Types...>> {
    static constexpr std::size_t value = tuple_type_index<T, std::tuple<Types...>>::value + 1;
};

template <class T, class... Types>
using tuple_type_index_v = typename tuple_type_index<T, Types...>::value;

template <class T1, class T2, size_t ... Seq>
constexpr bool is_same_tuples(std::index_sequence<Seq...>){
    return (std::is_same_v<std::tuple_element_t<Seq, T1>, std::tuple_element_t<Seq, T1>> && ...);
};

template <typename T>
struct Signature;

template <typename R, typename... Args>
struct Signature<std::function<R(Args...)>> {
    using type = R(Args...);
};

// =============
//   constexpr
// =============

template<auto F, auto Seq, class ... ArgsOut>
struct expanded_function_compiletime {
    static auto call(ArgsOut&& ... args){
        return detailed::call_compiletime<F, ArgsOut...>(Seq, std::forward<ArgsOut>(args)...);
    }
};

template<auto F, class ... ArgsOut, class R, class ... ArgsIn>
static constexpr auto func_expand_back_compiletime(const Signature<std::function<R(ArgsIn...)>>& signature){
    constexpr auto OutS = sizeof...(ArgsOut);
    constexpr auto InS = sizeof...(ArgsIn);
    static_assert(OutS >= InS);

    if constexpr (OutS == InS){
        return F;
    } else {
        using Seq = std::make_index_sequence<InS>;
        using Holder = expanded_function_compiletime<F, Seq{}, ArgsOut...>;
        return &Holder::call;
    }
}

template<auto F, class ... ArgsOut, class R, class ... ArgsIn>
static constexpr auto func_expand_front_compiletime(const Signature<std::function<R(ArgsIn...)>>& signature){
    constexpr auto OutS = sizeof...(ArgsOut);
    constexpr auto InS = sizeof...(ArgsIn);
    static_assert(OutS >= InS);

    if constexpr (OutS == InS){
        return F;
    } else {
        using Seq = make_index_range<OutS - InS, OutS>;
        using Holder = expanded_function_compiletime<F, Seq{}, ArgsOut...>;
        return &Holder::call;
    }
}

template<auto F, auto Seq, class ... ArgsOut, class R, class ... ArgsIn>
static constexpr auto func_expand_compiletime(const Signature<std::function<R(ArgsIn...)>>& signature){
    constexpr auto OutS = sizeof...(ArgsOut);
    constexpr auto InS = sizeof...(ArgsIn);
    
    if constexpr (OutS == InS){
        return F;
    } else {
        using Holder = expanded_function_compiletime<F, Seq, ArgsOut...>;
        return &Holder::call;
    }
}

template<auto F, class ... ArgsOut, class R, class ... ArgsIn>
static constexpr auto func_wrap_compiletime(const Signature<std::function<R(ArgsIn...)>>& signature){
    using TupleArgsIn = std::tuple<std::decay_t<ArgsIn>...>;
    using TupleArgsOut = std::tuple<std::decay_t<ArgsOut>...>;

    constexpr size_t SizeIn = sizeof...(ArgsIn);
    constexpr size_t SizeOut = sizeof...(ArgsOut);

    static_assert(SizeOut >= SizeIn);

    if constexpr (SizeIn == SizeOut){
        if constexpr (SizeOut == 0){
            return F;
        } else {
            using Seq = std::make_index_sequence<SizeIn>;
            if constexpr (detailed::is_same_tuples<TupleArgsIn, TupleArgsOut>(Seq{})){
                return F;
            } else {
                []<bool B = false>(){static_assert(B, "Function can not be wrapped.");}();
            }
        }
    } if constexpr (SizeIn == 0) {
        return func_expand_back_compiletime<F, ArgsOut...>(signature);
    } else {
        using FirstIn = std::tuple_element_t<0, TupleArgsIn>;
        using FirstOut = std::tuple_element_t<0, TupleArgsOut>;

        if constexpr (std::is_same_v<FirstIn, FirstOut>){
            return func_expand_back_compiletime<F, ArgsOut...>(signature);
        } else {
            using LastIn = std::tuple_element_t<SizeIn - 1, TupleArgsIn>;
            using LastOut = std::tuple_element_t<SizeOut - 1, TupleArgsOut>;
            
            if constexpr (std::is_same_v<LastIn, LastOut>){
                return func_expand_front_compiletime<F, ArgsOut...>(signature);
            } else {
                constexpr size_t start = detailed::tuple_type_index<FirstIn, TupleArgsOut>::value;
                using Seq = detailed::make_index_range<start, start + SizeIn>;
                return func_expand_compiletime<F, Seq{}, ArgsOut...>(signature);
            }
        }
    }
}

// =============
//   runtime
// =============

template<class ... ArgsOut, size_t ... Use, class F>
auto get_expanded(const F& func, const std::index_sequence<Use...>& seq){
    using Seq = std::decay_t<decltype(seq)>;
    return [func](ArgsOut&& ... args){
        return call(Seq{}, func, std::forward<ArgsOut>(args)...);
    };
}

template<class ... ArgsOut, class F, class R, class ... ArgsIn>
auto func_expand_back(const F& func,
                      const Signature<std::function<R(ArgsIn...)>>& signature){
    constexpr auto OutS = sizeof...(ArgsOut);
    constexpr auto InS = sizeof...(ArgsIn);
    static_assert(OutS >= InS);

    if constexpr (OutS == InS){
        return func;
    } else {
        using Seq = std::make_index_sequence<InS>;
        return get_expanded<ArgsOut...>(func, Seq{});
    }
}

template<class ... ArgsOut, class F, class R, class ... ArgsIn>
auto func_expand_front(const F& func,
                       const Signature<std::function<R(ArgsIn...)>>& signature){
    constexpr auto OutS = sizeof...(ArgsOut);
    constexpr auto InS = sizeof...(ArgsIn);
    static_assert(OutS >= InS);

    if constexpr (OutS == InS){
        return func;
    } else {
        using Seq = detailed::make_index_range<OutS - InS, OutS>;
        return get_expanded<ArgsOut...>(func, Seq{});
    }
}

template<class ... ArgsOut, size_t ... Use, class F, class R, class ... ArgsIn>
auto func_expand(const F& func,
                 const Signature<std::function<R(ArgsIn...)>>& signature,
                 const std::index_sequence<Use...> &seq){
    constexpr auto OutS = sizeof...(ArgsOut);
    constexpr auto InS = sizeof...(ArgsIn);
    
    if constexpr (OutS == InS){
        return func;
    } else {
        return get_expanded<ArgsOut...>(func, seq);
    }
}

template<class ... ArgsOut, class F, class R, class ... ArgsIn>
static auto func_wrap(const F& func,
                      const Signature<std::function<R(ArgsIn...)>>& signature){
    using TupleArgsIn = std::tuple<std::decay_t<ArgsIn>...>;
    using TupleArgsOut = std::tuple<std::decay_t<ArgsOut>...>;

    constexpr size_t SizeIn = sizeof...(ArgsIn);
    constexpr size_t SizeOut = sizeof...(ArgsOut);

    static_assert(SizeOut >= SizeIn);

    if constexpr (SizeIn == SizeOut){
        if constexpr (SizeOut == 0){
            return func;
        } else {
            using Seq = std::make_index_sequence<std::min(SizeIn, SizeOut)>;
            if constexpr (detailed::is_same_tuples<TupleArgsIn, TupleArgsOut>(Seq{})){
                return func;
            } else {
                []<bool B = false>(){static_assert(B, "Function can not be wrapped.");}();
            }
        }
    } if constexpr (SizeIn == 0) {
        return func_expand_back<ArgsOut...>(func, signature);
    } else {
        using FirstIn = std::tuple_element_t<0, TupleArgsIn>;
        using FirstOut = std::tuple_element_t<0, TupleArgsOut>;

        if constexpr (std::is_same_v<FirstIn, FirstOut>){
            return func_expand_back<ArgsOut...>(func, signature);
        } else {
            using LastIn = std::tuple_element_t<SizeIn - 1, TupleArgsIn>;
            using LastOut = std::tuple_element_t<SizeOut - 1, TupleArgsOut>;
            
            if constexpr (std::is_same_v<LastIn, LastOut>){
                return func_expand_front<ArgsOut...>(func, signature);
            } else {
                constexpr size_t start = detailed::tuple_type_index<FirstIn, TupleArgsOut>::value;
                using Seq = detailed::make_index_range<start, start + SizeIn>;
                return func_expand<ArgsOut...>(func, signature, Seq{});
            }
        }
    }
}

}

template<class ... ArgsOut, class F>
static auto expand_func(const F& func){
    using Signature = detailed::Signature<decltype(std::function(func))>;
    return detailed::func_wrap<ArgsOut...>(func, Signature{});
}

template<auto F, class ... ArgsOut>
static constexpr auto expand_func(){
    using Signature = detailed::Signature<decltype(std::function(F))>;
    return detailed::func_wrap_compiletime<F, ArgsOut...>(Signature{});
}

template<class ... ArgsOut, class F>
static auto expand_func_back(const F& func){
    using Signature = detailed::Signature<decltype(std::function(func))>;
    return detailed::func_expand_back<ArgsOut...>(func, Signature{});
}

template<auto F, class ... ArgsOut>
static constexpr auto expand_func_back(){
    using Signature = detailed::Signature<decltype(std::function(F))>;
    return detailed::func_expand_back_compiletime<F, ArgsOut...>(Signature{});
}

template<class ... ArgsOut, class F>
static auto expand_func_front(const F& func){
    using Signature = detailed::Signature<decltype(std::function(func))>;
    return detailed::func_expand_front<ArgsOut...>(func, Signature{});
}

template<auto F, class ... ArgsOut>
static constexpr auto expand_func_front(){
    using Signature = detailed::Signature<decltype(std::function(F))>;
    return detailed::func_expand_front_compiletime<F, ArgsOut...>(Signature{});
}


}