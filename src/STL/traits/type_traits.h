#ifndef __TRAITS_H__
#define __TRAITS_H__

namespace stl
{
    #if defined(_WIN32) || defined(_WIN64)
        #if defined(_WIN64)
            #define ENV 64
        #else
            #define ENV 32
        #endif
    #elif defined(__GNUC__)
        #if defined(__x86_64__) || defined(__ppc64__)
            #define ENV 64
        #else
            #define ENV 32
        #endif
    #else
        #error "Unknown platform!"
    #endif
    
    /// @brief the unsigned integer type of the result of the sizeof operator
    typedef decltype(sizeof(0)) size_t;
 
    typedef unsigned long long	uint64_t;

    #if defined(__x86_64__) || defined(_M_X64)  // 64-bit platform
        typedef unsigned long long uintptr_t;
    #elif defined(__i386__) || defined(_M_IX86) // 32-bit platform
        typedef unsigned int uintptr_t;
    #endif

    typedef decltype(static_cast<int*>(nullptr) - static_cast<int*>(nullptr)) ptrdiff_t;

    typedef decltype(nullptr) nullptr_t;


    /// @b Type_traits | Helper Classes


    /// @c stl::integral_constant wraps a static constant of specified type
    template <typename T, T v>
    struct integral_constant
    {
        static constexpr T value =        v;

        typedef T                       value_type;
        typedef integral_constant<T, v> type;

        // Conversion function. Returns the wrapped value.
        constexpr operator value_type() const noexcept { return value; } 

        // Returns the wrapped value. This function enables stl::integral_constant to be passed as a type parameter and used in constexpr expressions
        constexpr value_type operator()() const noexcept { return value; }
    };

    template <typename T, T v>
    constexpr T integral_constant<T, v>::value;

    typedef integral_constant<bool, true> true_type;

    typedef integral_constant<bool, false> false_type;

    template <bool B>
    using bool_constant = integral_constant<bool, B>;


    /** ##################################
    #  @b Meta_programming_helper_types  #
    ####################################*/

    template <bool B, typename T, typename F>
    struct conditional { typedef T type; };

    template <typename T, typename F>
    struct conditional<false, T, F> { typedef F type; };

    template <bool B, typename T, typename F>
    using conditional_t = typename conditional<B, T, F>::type;

    /// These templates are used to compute logical @c OR (__or_) and logical @c AND (__and_)

    template <typename...> struct __or_;
    template <typename...> struct __and_;

    template <> struct __or_<> : public false_type { };

    template <typename booleanT>
    struct __or_<booleanT> : public booleanT { };

    template <typename booleanT1, typename booleanT2>
    struct __or_<booleanT1, booleanT2> : public conditional<booleanT1::value, booleanT1, booleanT2>::type { };

    template <typename booleanT1, typename booleanT2, typename booleanT3, typename... booleanTn>
    struct __or_<booleanT1, booleanT2, booleanT3, booleanTn...> : public conditional<booleanT1::value, booleanT1, __or_<booleanT2, booleanT3, booleanTn...>>::type { };

    template <> struct __and_ <> : public true_type { };

    template <typename booleanT>
    struct __and_<booleanT> : public booleanT { };

    template <typename booleanT1, typename booleanT2>
    struct __and_<booleanT1, booleanT2> : public conditional<booleanT1::value, booleanT2, booleanT1>::type { };

    template <typename booleanT1, typename booleanT2, typename booleanT3, typename... booleanTn>
    struct __and_<booleanT1, booleanT2, booleanT3, booleanTn...> : public conditional<booleanT1::value, booleanT1, __and_<booleanT2, booleanT3, booleanTn...>>::type { };

    template <typename booleanT>
    struct __not_ : public integral_constant<bool, !booleanT::value> { };


    /** ################################
    #  @b Const-volatility_specifiers  #
    ##################################*/
    
    /// @c remove_cv
    
    template <typename T>
    struct remove_const { typedef T type; };

    template <typename T>
    struct remove_const<const T> { typedef T type; };

    template <typename T>
    struct remove_volatile { typedef T type; };

    template <typename T>
    struct remove_volatile<volatile T> { typedef T type; };

    template <typename T> using remove_const_t    = typename remove_const<T>::type;
    template <typename T> using remove_volatile_t = typename remove_volatile<T>::type;

    template <typename> 
    struct remove_cv;

    template <typename T>
    struct remove_cv { typedef typename remove_const<typename remove_volatile<T>::type>::type type; };

    template <typename T> using remove_cv_t = typename remove_cv<T>::type;

    /// @c add_cv

    template <typename T>
    struct add_const { typedef T const type; };

    template <typename T>
    struct add_volatile { typedef T volatile type; };

    template <typename T>
    struct add_cv { typedef typename add_const<typename add_volatile<T>::type>::type type; };

    template <typename T> using add_const_t = typename add_const<T>::type;
    template <typename T> using add_volatile_T = typename add_volatile<T>::type;
    template <typename T> using add_cv_t = typename add_cv<T>::type;

    /// Primary type categories

    /// @c is_void

    template <typename> struct is_void_helper : public false_type { };
    template <> struct is_void_helper<void>   : public true_type { };

    /**
     * @brief @c is_void checks if @c T is a void type 
     * @tparam T a type to check
    */ 
    template <typename T>
    struct is_void : public is_void_helper<typename remove_cv<T>::type>::type { };

    // Helper variable template | Since C++17
    template <typename T>
    constexpr bool is_void_v = is_void<T>::value; 


    /// @c is_null_pointer
    
    template <typename> struct is_null_pointer_helper    : public false_type { };
    template <> struct is_null_pointer_helper<nullptr_t> : public true_type { };

    template <typename T>
    struct is_null_pointer : public is_null_pointer_helper<typename remove_cv<T>::type>::type { };

    /**
     * @brief Checks whether @c T is the type @c stl::nullptr_t.
     * @tparam T a type to check
     */
    template <typename T>
    struct is_nullptr_t : public is_null_pointer<T> { };

    // Helper variable template | Since C++17
    template <typename T>
    constexpr bool is_nullptr_v = is_null_pointer<T>::value;


    /// @c is_integral

    template <typename> struct is_itegral_helper             : public false_type { };

    template <> struct is_itegral_helper<bool>               : public true_type { };
    template <> struct is_itegral_helper<char>               : public true_type { };
    template <> struct is_itegral_helper<signed char>        : public true_type { };
    template <> struct is_itegral_helper<unsigned char>      : public true_type { };
    template <> struct is_itegral_helper<wchar_t>            : public true_type { };
    template <> struct is_itegral_helper<char16_t>           : public true_type { };
    template <> struct is_itegral_helper<char32_t>           : public true_type { };
    template <> struct is_itegral_helper<short>              : public true_type { };
    template <> struct is_itegral_helper<unsigned short>     : public true_type { };
    template <> struct is_itegral_helper<int>                : public true_type { };
    template <> struct is_itegral_helper<unsigned int>       : public true_type { };
    template <> struct is_itegral_helper<long>               : public true_type { };
    template <> struct is_itegral_helper<unsigned long>      : public true_type { };
    template <> struct is_itegral_helper<long long>          : public true_type { };
    template <> struct is_itegral_helper<unsigned long long> : public true_type { };

    /**
     * @brief Checks whether @c T is an integral type 
     *        ( @b bool, 
     *          @b char, 
     *          @b char8_t (since C++20), 
     *          @b char16_t, 
     *          @b char32_t, 
     *          @b wchar_t, 
     *          @b short, 
     *          @b int, 
     *          @b long, 
     *          @b longlong 
     *         or any implementation-defined extended integer types, including any signed, unsigned, and cv-qualified variants)
     * @tparam T a type to check
     */
    template <typename T>
    struct is_integral : public is_itegral_helper<typename remove_cv<T>::type>::type { };

    // Helper variable template | Since C++17
    template <typename T>
    constexpr bool is_integral_v = is_integral<T>::value;


    /// @c is_floating_point

    template <typename>
    struct is_floating_point_helper : public false_type { };

    template <> struct is_floating_point_helper<float>       : public true_type { };
    template <> struct is_floating_point_helper<double>      : public true_type { };
    template <> struct is_floating_point_helper<long double> : public true_type { };
    template <> struct is_floating_point_helper<__float128>  : public true_type { };
    
    /**
     * @brief Checks whether @c T is a @c floating-point type
     * @tparam T a type to check
     */
    template <typename T>
    struct is_floating_point : public is_floating_point_helper<typename remove_cv<T>::type>::type { };

    // Helper variable template | Since C++17
    template <typename T>
    constexpr bool is_floating_point_v = is_floating_point<T>::value;


    /// @c is_array
    
    template <typename>
    struct is_array : public false_type { };

    template <typename T, size_t Size>
    struct is_array<T[Size]> : public true_type { };

    template <typename T>
    struct is_array<T[]> : public true_type { };

    /**
     * @brief  Checks whether @c T is an @c array type.
     * @tparam T a type to check
     * @note   Helper variable template | Since C++17
     */
    template <typename T>
    constexpr bool is_array_v = is_array<T>::value;


    /// @c is_enum

    /**
     * @brief  Checks whether @c T is an enumeration type
     * @tparam T a type to check
     */
    template <typename T>
    struct is_enum : public integral_constant<bool, __is_enum(T)> { };

    // Helper variable template | Since C++17
    template <typename T>
    constexpr bool is_enum_v = is_enum<T>::value;


    /// @c is_union

    /**
     * @brief  Checks whether @c T is a union type
     * @tparam T a type to check
     */
    template <typename T>
    struct is_union : public integral_constant<bool, __is_union(T)> { };

    // Helper variable template | Since C++17
    template <typename T>
    constexpr bool is_union_v = is_union<T>::value;

    
    /// @c is_class

    /**
     * @brief  Checks whether @c T is a @c non-union class type
     * @tparam T a type to check
     */
    template <typename T>
    struct is_class : public integral_constant<bool, __is_class(T)> { };

    // Helper variable template | Since C++17
    template <typename T>
    constexpr bool is_class_v = is_class<T>::value;


    /// @c is_function

    template <typename>
    struct is_function;

    template <typename>
    struct is_function : public false_type { };

    template <typename Res, typename... ArgTypes> struct is_function<Res(ArgTypes...)>                      : public true_type { }; 
    template <typename Res, typename... ArgTypes> struct is_function<Res(ArgTypes...) &>                    : public true_type { }; 
    template <typename Res, typename... ArgTypes> struct is_function<Res(ArgTypes...) &&>                   : public true_type { }; 
    template <typename Res, typename... ArgTypes> struct is_function<Res(ArgTypes......)>                   : public true_type { }; 
    template <typename Res, typename... ArgTypes> struct is_function<Res(ArgTypes......) &>                 : public true_type { };
    template <typename Res, typename... ArgTypes> struct is_function<Res(ArgTypes......) &&>                : public true_type { };
    template <typename Res, typename... ArgTypes> struct is_function<Res(ArgTypes...) const>                : public true_type { }; 
    template <typename Res, typename... ArgTypes> struct is_function<Res(ArgTypes...) const &>              : public true_type { }; 
    template <typename Res, typename... ArgTypes> struct is_function<Res(ArgTypes...) const &&>             : public true_type { };
    template <typename Res, typename... ArgTypes> struct is_function<Res(ArgTypes......) const>             : public true_type { }; 
    template <typename Res, typename... ArgTypes> struct is_function<Res(ArgTypes......) const &>           : public true_type { };
    template <typename Res, typename... ArgTypes> struct is_function<Res(ArgTypes......) const &&>          : public true_type { };
    template <typename Res, typename... ArgTypes> struct is_function<Res(ArgTypes...) volatile>             : public true_type { }; 
    template <typename Res, typename... ArgTypes> struct is_function<Res(ArgTypes...) volatile &>           : public true_type { }; 
    template <typename Res, typename... ArgTypes> struct is_function<Res(ArgTypes...) volatile &&>          : public true_type { };
    template <typename Res, typename... ArgTypes> struct is_function<Res(ArgTypes......) volatile>          : public true_type { }; 
    template <typename Res, typename... ArgTypes> struct is_function<Res(ArgTypes......) volatile &>        : public true_type { };
    template <typename Res, typename... ArgTypes> struct is_function<Res(ArgTypes......) volatile &&>       : public true_type { };
    template <typename Res, typename... ArgTypes> struct is_function<Res(ArgTypes...) const volatile>       : public true_type { }; 
    template <typename Res, typename... ArgTypes> struct is_function<Res(ArgTypes...) const volatile &>     : public true_type { }; 
    template <typename Res, typename... ArgTypes> struct is_function<Res(ArgTypes...) const volatile &&>    : public true_type { };
    template <typename Res, typename... ArgTypes> struct is_function<Res(ArgTypes......) const volatile>    : public true_type { }; 
    template <typename Res, typename... ArgTypes> struct is_function<Res(ArgTypes......) const volatile &>  : public true_type { };
    template <typename Res, typename... ArgTypes> struct is_function<Res(ArgTypes......) const volatile &&> : public true_type { };  

    /**
     * @brief Checks whether @c T is a function type
     * @tparam T a type to check
     */
    template <typename T>
    constexpr bool is_function_v = is_function<T>::value;


    /// @c is_pointer

    template <typename>
    struct is_pointer_helper : public false_type { };

    template <typename T>
    struct is_pointer_helper<T*> : public true_type { };

    /**
     * @brief  Checks whether @c T is a pointer to @c object or @c function (including pointer to @b void, but excluding pointer to member)
     * @tparam T a type to check 
     */
    template <typename T>
    struct is_pointer : public is_pointer_helper<typename remove_cv<T>::type>::type { }; 

    // Helper variable template | Since C++17
    template <typename T>
    constexpr bool is_pointer_v = is_pointer<T>::value;


    /// @c is_lvalue_reference

    template <typename>
    struct is_lvalue_reference : public false_type { };
    
    template <typename T>
    struct is_lvalue_reference<T&> : public true_type { };

    /**
     * @brief  Checks whether @c T is an @c lvalue reference type
     * @tparam T a type to check 
     * @note   Helper variable template | Since C++17
     */
    template <typename T>
    constexpr bool is_lvalue_reference_v = is_lvalue_reference<T>::value;


    /// @c is_rvalue_reference

    template <typename>
    struct is_rvalue_reference : public false_type { };

    template <typename T>
    struct is_rvalue_reference<T&&> : public true_type { };

    /**
     * @brief  Checks whether @c T is an @c rvalue reference type
     * @tparam T a type to check 
     * @note   Helper variable template | Since C++17
     */
    template <typename T>
    constexpr bool is_rvalue_reference_v = is_rvalue_reference<T>::value;


    /// @c is_member_object_pointer

    template <typename>
    struct is_member_object_pointer_helper : public false_type { };

    template <typename T, typename C>
    struct is_member_object_pointer_helper<T C::*> : public integral_constant<bool, !is_function<T>::value> { };

    /**
     * @brief  Checks whether @c T is a non-static member object pointer
     * @tparam T a type to check 
     */
    template <typename T>
    struct is_member_object_pointer : public is_member_object_pointer_helper<typename remove_cv<T>::type>::type { };
    
    // Helper variable template | Since C++17
    template <typename T>
    constexpr bool is_member_object_pointer_v = is_member_object_pointer<T>::value;


    /// @c is_member_function_pointer

    template <typename>
    struct is_member_function_pointer_helper : public false_type { };

    template <typename T, typename C>
    struct is_member_function_pointer_helper<T C::*> : public integral_constant<bool, is_function<T>::value> { };

    /**
     * @brief  Checks whether @c T is a non-static member function pointer
     * @tparam T a type to check 
     */
    template <typename T>
    struct is_member_function_pointer : public is_member_function_pointer_helper<typename remove_cv<T>::type>::type { };

    // Helper variable template | Since C++17
    template <typename T>
    constexpr bool is_member_function_pointer_v = is_member_function_pointer<T>::value;


    /** ##############################
    #  @b Composite_type_categories  #
    ################################*/


    /// @c is_reference

    /**
     * @brief  Checks if @c T is a reference type (lvalue reference or rvalue reference)
     * @tparam T a type to check 
     */
    template <typename T>
    struct is_reference : public __or_<is_lvalue_reference<T>, is_rvalue_reference<T>>::type { };

    // Helper variable template | Since C++17
    template <typename T>
    constexpr bool is_reference_v = is_reference<T>::value;


    /// @c is_arithmetic

    /**
     * @brief  If T is an arithmetic type (that is, an `integral` type or a `floating-point` type)
     * @tparam T a type to check 
     */
    template <typename T>
    struct is_arithmetic : public __or_<is_integral<T>, is_floating_point<T>>::type { };

    // Helper variable template | Since C++17
    template <typename T>
    constexpr bool is_arithmetic_v = is_arithmetic<T>::value;


    /// @c is_fundamental

    /**
     * @brief  If @c T is a fundamental type (that is, `arithmetic` type, `void`, or `nullptr_t`)
     * @tparam T a type to check 
     */
    template <typename T>
    struct is_fundamental : public __or_<is_arithmetic<T>, is_void<T>, is_null_pointer<T>>::type { };

    // Helper variable template | Since C++17
    template <typename T>
    constexpr bool is_fundamental_v = is_fundamental<T>::value;


    /// @c is_object

    /** 
     * @brief  If @c T is an object type (that is any possibly cv-qualified type other than `function`, `reference`, or `void` types)
     * @tparam T a type to check 
     */
    template <typename T>
    struct is_object : public __not_<__or_<is_function<T>, is_reference<T>, is_void<T>>>::type { };
    
    // Helper variable template | Since C++17
    template <typename T>
    constexpr bool is_object_v = is_object<T>::value;


    /// @c is_scalar

    /**
     * @brief  If @c T is a scalar type, provides the member constant value equal true
     * @tparam T a type to check 
     */
    template <typename T>
    struct is_scalar : public __or_<is_arithmetic<T>, is_enum<T>, is_pointer<T>, 
                                    is_member_object_pointer<T>, is_null_pointer<T>>::type { };

    // Helper variable template | Since C++17
    template <typename T>
    constexpr bool is_scalar_v = is_scalar<T>::value;


    /// @c is_compound
    
    /**
     * @brief  If @c T is a compound type (that is, 
     * 
     *           `array`, `function`, `object pointer`, `function pointer`, `member object pointer`, 
     *           `member function pointer`, `reference`, `class`, `union`, or `enumeration`, 
     * 
     *         including any cv-qualified variants)
     * @tparam T a type to check 
     */
    template <typename T>
    struct is_compound : public integral_constant<bool, !is_fundamental<T>::value> { };

    // Helper variable template | Since C++17
    template <typename T>
    constexpr bool is_compound_v = is_compound<T>::value;


    /// @c is_member_pointer

    template <typename T>
    struct is_member_pointer_helper : public false_type { };

    template <typename T, typename C>
    struct is_member_pointer_helper<T C::*> : public true_type { };

    /**
     * @brief  Checks if @c T is pointer to non-static member object or a pointer to non-static member function
     * @tparam T a type to check 
     */
    template <typename T>
    struct is_member_pointer : public is_member_pointer_helper<typename remove_cv<T>::type>::type { };

    // Helper variable template | Since C++17
    template <typename T>
    constexpr bool is_member_pointer_v = is_member_pointer<T>::value;














    /**
     * ***********************************************
     *          @b DONE + @b TESTED <<<<=            *
     *          @todo =>>>>                          *
     * ***********************************************
     */


    ///

    template <typename>
    struct is_const : public false_type { };

    template <typename T>
    struct is_const<const T> : public true_type { };
    
    template <typename>
    struct is_volatile : public false_type { };
    
    template <typename T>
    struct is_volatile<volatile T> : public true_type { };

    template <typename T>
    struct is_trivial : public integral_constant<bool, __is_trivial(T)> { };

    template <typename T>
    struct is_trivially_copyable : public integral_constant<bool, __is_trivially_copyable(T)> { };


    ////
    
    struct nonesuch
    {
        nonesuch() = delete;
        ~nonesuch() = delete;

        nonesuch(const nonesuch&) = delete;       // copy constructor
        void operator=(const nonesuch&) = delete; // Copy assignment
    };

    //

    template <typename T>
    struct success_type { typedef T type; };
    
    struct failure_type { };

    ////

    template <typename...>
    struct _void_t { typedef void type; };
    
    /** */
    template <bool, typename T = void>
    struct enable_if { };

    template <typename T>
    struct enable_if<true, T> { typedef T type; };

    template <typename... Require>
    using require = typename enable_if<__and_<Require...>::value>::type;

    //

    /// remove ref

    template <typename T>
    struct remove_reference { typedef T type; };

    template <typename T>
    struct remove_reference<T&> { typedef T type; };

    template <typename T>
    struct remove_reference<T&&> { typedef T type; };






    template <typename T>
    struct is_referenceable : public __or_<is_object<T>, is_reference<T>>::type { };

    template <typename Res, typename... Args>
    struct is_referenceable<Res(Args...)> : public true_type { };

    template <typename Res, typename... Args>
    struct is_referenceable<Res(Args......)> : public true_type { };






    template <typename T, bool = is_referenceable<T>::value>
    struct add_rvalue_reference_helper { typedef T type; };

    template <typename T>
    struct add_rvalue_reference_helper<T, true> { typedef T &&type; };

    template <typename T> 
    struct add_rvalue_reference : public add_rvalue_reference_helper<T> { };

    template <typename T> typename add_rvalue_reference<T>::type declval() noexcept;

    template <typename T>
    struct declval_protector
    {
        static constexpr bool stop = false;
        static typename add_rvalue_reference<T>::type delegate();
    };  

    template <typename T>
    inline typename add_rvalue_reference<T>::type declval() noexcept
    {
        static_assert(declval_protector<T>::stop, "declval() must not be used!");
        return declval_protector<T>::delegate();
    }

    //

    template <typename From, typename To, bool = __or_<is_void<From>, is_function<To>, is_array<To>>::value>
    struct is_convertible_helper { typedef typename is_void<To>::type type; };

    template <typename From, typename To>
    class is_convertible_helper<From, To, false> 
    {
    private:
        template <typename T> static void test_aux(T);

        template <typename From1, typename To1, typename = decltype(test_aux<To1>(declval<From1>()))>
        static true_type test(int);

        template <typename, typename> static false_type test(...);

    public:
        typedef decltype(test<From, To>(0)) type;
    };

    template <typename From, typename To>
    struct is_convertible : public is_convertible_helper<From, To>::type { };
}

#endif // TRAITS_H