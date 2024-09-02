#pragma once

#include "../Types/traits.h"
#include "../STL/array.h"

namespace traits_test
{   
    /// @b Primary_type_categories | @c stl::itegral_constant

    using two_t = stl::integral_constant<int, 2>;
    using four_t = stl::integral_constant<int, 4>;

    static_assert(two_t::value * 2 == four_t::value);
    static_assert(two_t() << 1 == four_t() >> 0);

    enum class E { e1, e2 };

    using c1 = stl::integral_constant<E, E::e1>;
    using c2 = stl::integral_constant<E, E::e2>;

    static_assert(c1::value != E::e2);
    static_assert(c1() == E::e1);

    /// @c is_void

    static_assert(
        stl::is_void<void>::value == true &&
        stl::is_void<const void>::value == true &&
        stl::is_void<volatile void>::value == true &&
        stl::is_void<void *>::value == false &&
        stl::is_void<int>::value == false
    );

    static_assert(
        stl::is_void_v<void> == true &&
        stl::is_void_v<const void> == true &&
        stl::is_void_v<volatile void> == true &&
        stl::is_void_v<void *> == false &&
        stl::is_void_v<int> == false &&
        stl::is_void_v<stl::is_void<void>> == false
    );

    /// @c is_null_pointer

    static_assert(stl::is_nullptr_v<decltype(nullptr)> == true);
    static_assert(stl::is_nullptr_v<stl::nullptr_t> == true);
    static_assert(stl::is_nullptr_v<const stl::nullptr_t> == true);
    static_assert(stl::is_nullptr_v<volatile stl::nullptr_t> == true);
    static_assert(stl::is_nullptr_v<const volatile stl::nullptr_t> == true);
    static_assert(stl::is_nullptr_v<int *> == false);

    static_assert(stl::is_null_pointer<decltype(nullptr)>::value == true);
    static_assert(stl::is_null_pointer<stl::nullptr_t>::value == true);
    static_assert(stl::is_null_pointer<const stl::nullptr_t>::value == true);
    static_assert(stl::is_null_pointer<volatile stl::nullptr_t>::value == true);
    static_assert(stl::is_null_pointer<const volatile stl::nullptr_t>::value == true);
    static_assert(stl::is_null_pointer<int *>::value == false);

    /// @c is_integral

    static_assert(
        stl::is_integral_v<float> == false &&
        stl::is_integral_v<int *> == false &&
        stl::is_integral_v<int> == true &&
        stl::is_integral_v<const int> == true &&
        stl::is_integral_v<bool> == true &&
        stl::is_integral_v<char> == true);

    class A { };
    static_assert(stl::is_integral_v<A> == false);

    struct B { int x = 99; };
    static_assert(stl::is_integral_v<B> == false);
    static_assert(stl::is_integral_v<decltype(B::x)> == true);

    enum E1 : int { };
    static_assert(stl::is_integral_v<E1> == false);

    template <typename T>
    constexpr T same(T i)
    {
        static_assert(stl::is_integral_v<T> == true, "Integral required.");
        return i;
    }

    static_assert(same('"') == 34);


    /// @c is_floating_point

    static_assert(stl::is_floating_point_v<A> == false);
    static_assert(stl::is_floating_point_v<float> == true);
    static_assert(stl::is_floating_point_v<double> == true);
    static_assert(stl::is_floating_point_v<float&> == false);
    static_assert(stl::is_floating_point_v<double&> == false);
    static_assert(stl::is_floating_point_v<int> == false);

    static_assert(stl::is_floating_point<A>::value == false);
    static_assert(stl::is_floating_point<float>::value == true);
    static_assert(stl::is_floating_point<double>::value == true);
    static_assert(stl::is_floating_point<float&>::value == false);
    static_assert(stl::is_floating_point<double&>::value == false);
    static_assert(stl::is_floating_point<int>::value == false);
    

    /// @c is_array

    static_assert(stl::is_array_v<A> == false);
    static_assert(stl::is_array_v<A[]> == true);
    static_assert(stl::is_array_v<A[3]> == true);
    static_assert(stl::is_array_v<float> == false);
    static_assert(stl::is_array_v<int> == false);
    static_assert(stl::is_array_v<int[]> == true);
    static_assert(stl::is_array_v<stl::array<int, 3>> == false);

    static_assert(stl::is_array<A>::value == false);
    static_assert(stl::is_array<A[]>::value == true);
    static_assert(stl::is_array<A[3]>::value == true);
    static_assert(stl::is_array<float>::value == false);
    static_assert(stl::is_array<int>::value == false);
    static_assert(stl::is_array<int[]>::value == true);
    static_assert(stl::is_array<stl::array<int, 3>>::value == false);


    /// @c is_function

    int f(); void g();
    static_assert(stl::is_function_v<decltype(f)> == true);    
    static_assert(stl::is_function_v<int(int)> == true);
    static_assert(stl::is_function_v<int> == false);
    static_assert(stl::is_function_v<decltype(g)> == true);

    static_assert(stl::is_function<decltype(f)>::value == true);    
    static_assert(stl::is_function<int(int)>::value == true);
    static_assert(stl::is_function<int>::value == false);
    static_assert(stl::is_function<decltype(g)>::value == true);
    
    struct O { void operator()() { } };
    static_assert(stl::is_function_v<O()> == true);

    struct C
    {
        static int foo();
        int fun() const&;
    };

    static_assert(stl::is_function_v<C> == false);
    static_assert(stl::is_function_v<decltype(C::foo)> == true);
    static_assert(stl::is_function_v<decltype(&C::fun)> == false);

    
    /// @c is_enum

    struct A1 { enum E { }; };
    static_assert(stl::is_enum_v<A1> == false);
    static_assert(stl::is_enum_v<A1::E> == true);
    static_assert(stl::is_enum<A1>::value == false);
    static_assert(stl::is_enum<A1::E>::value == true);

    enum E2 { };
    static_assert(stl::is_enum_v<E2> == true);
    static_assert(stl::is_enum_v<int> == false);
    static_assert(stl::is_enum<E2>::value == true);
    static_assert(stl::is_enum<int>::value == false);


    /// @c is_union

    struct uA { };

    typedef union
    {
        int a;
        float b;
    } uB;

    struct uC { uB d; };

    static_assert(stl::is_union_v<uA> == false);
    static_assert(stl::is_union_v<uB> == true);
    static_assert(stl::is_union_v<uC> == false);
    static_assert(stl::is_union_v<int> == false);

    static_assert(stl::is_union<uA>::value == false);
    static_assert(stl::is_union<uB>::value == true);
    static_assert(stl::is_union<uC>::value == false);
    static_assert(stl::is_union<int>::value == false);


    /// @c is_class

    struct cA { }; class cB { }; enum class cE { }; union cU { class uC { }; };

    static_assert(stl::is_class_v<cU> == false);
    static_assert(stl::is_class_v<cU::uC> == true);
    static_assert(stl::is_class_v<cA> == true);
    static_assert(stl::is_class_v<cB> == true);
    static_assert(stl::is_class_v<cB*> == false);
    static_assert(stl::is_class_v<cB&> == false);
    static_assert(stl::is_class_v<const cB> == true);
    static_assert(stl::is_class_v<cE> == false);
    static_assert(stl::is_class_v<int> == false);
    static_assert(stl::is_class_v<struct cS> == true);
    static_assert(stl::is_class_v<class cC> == true);

    static_assert(stl::is_class<cU>::value == false);
    static_assert(stl::is_class<cU::uC>::value == true);
    static_assert(stl::is_class<cA>::value == true);
    static_assert(stl::is_class<cB>::value == true);
    static_assert(stl::is_class<cB*>::value == false);
    static_assert(stl::is_class<cB&>::value == false);
    static_assert(stl::is_class<const cB>::value == true);
    static_assert(stl::is_class<cE>::value == false);
    static_assert(stl::is_class<int>::value == false);
    static_assert(stl::is_class<struct cS>::value == true);
    static_assert(stl::is_class<class cC>::value == true);


    /// @c is_pointer

    struct pA
    {
        int m;
        void f() { }
    };

    int pA::*mem_data_pointer = &pA::m;     // a pointer to member data
    void (pA::*mem_fun_pointer)() = &pA::f; // a pointer to member function

    static_assert(
        stl::is_pointer_v<pA> == false &&
        stl::is_pointer_v<pA*> == true &&
        stl::is_pointer_v<pA const* volatile> == true &&
        stl::is_pointer_v<pA&> == false &&
        stl::is_pointer_v<decltype(mem_data_pointer)> == false &&
        stl::is_pointer_v<decltype(mem_fun_pointer)> == false &&
        stl::is_pointer_v<void *> == true &&
        stl::is_pointer_v<int> == false &&
        stl::is_pointer_v<int*> == true &&
        stl::is_pointer_v<int**> == true && 
        stl::is_pointer_v<int[10]> == false &&
        stl::is_pointer_v<stl::nullptr_t> == false &&
        stl::is_pointer_v<void (*)()> == true
    );

    static_assert(
        stl::is_pointer<pA>::value == false &&
        stl::is_pointer<pA*>::value == true &&
        stl::is_pointer<pA const* volatile>::value == true &&
        stl::is_pointer<pA&>::value == false &&
        stl::is_pointer<decltype(mem_data_pointer)>::value == false &&
        stl::is_pointer<decltype(mem_fun_pointer)>::value == false &&
        stl::is_pointer<void *>::value == true &&
        stl::is_pointer<int>::value == false &&
        stl::is_pointer<int*>::value == true &&
        stl::is_pointer<int**>::value == true && 
        stl::is_pointer<int[10]>::value == false &&
        stl::is_pointer<stl::nullptr_t>::value == false &&
        stl::is_pointer<void (*)()>::value == true
    );


    /// @c is_lvalue_reference

    class lvrA { };

    static_assert(
        stl::is_lvalue_reference_v<lvrA> == false &&
        stl::is_lvalue_reference_v<lvrA&> == true &&
        stl::is_lvalue_reference_v<lvrA&&> == false &&
        stl::is_lvalue_reference_v<int> == false &&
        stl::is_lvalue_reference_v<int&> == true &&
        stl::is_lvalue_reference_v<int&&> == false
    );


    /// @c is_rvalue_reference

    class rvrA { }; 

    static_assert(
        stl::is_rvalue_reference_v<rvrA> == false &&
        stl::is_rvalue_reference_v<rvrA&> == false &&
        stl::is_rvalue_reference_v<rvrA&&> == true &&
        stl::is_rvalue_reference_v<char> == false &&
        stl::is_rvalue_reference_v<char&> == false &&
        stl::is_rvalue_reference_v<char&&> == true
    );

    namespace rvalue_ref
    {   
        template <typename T>
        void test(T&& v)
        {
            std::cout << std::boolalpha;
            std::cout << "T: " << stl::is_rvalue_reference_v<T> << std::endl;
            std::cout << "T&&: " << stl::is_rvalue_reference_v<T&&> << std::endl;
            std::cout << "decltype(x): " << stl::is_rvalue_reference_v<decltype(v)> << std::endl;
            std::cout << std::noboolalpha;
        }
    }


    /// @c is_member_object_pointer

    class C1 { };
    static_assert(
        stl::is_member_object_pointer_v<int(C1::*)> == true &&
        stl::is_member_object_pointer_v<int(C1::*)()> == false
    );


    /// @c is_member_function_pointer

    class A2
    {
    public:
        void member() { }
    };

    static_assert(stl::is_member_function_pointer_v<decltype(&A2::member)> == true);
    static_assert(stl::is_member_function_pointer<decltype(&A2::member)>::value == true);



    // main testing function
    void __TEST__() 
    { 
        /// @b is_rvalue_reference_test

        int x = 42;

        rvalue_ref::test(42);
        rvalue_ref::test(x);

        /// 
    }
}