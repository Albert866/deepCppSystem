
template <bool>
class std __basic_string_common
{
protected:
    _LIBCPP_NORETURN void __throw_length_error() const;
    _LIBCPP_NORETURN void __throw_out_of_range() const;
};

template <class _CharT, class _Traits, class _Allocator>
class std basic_string : private __basic_string_common<true>
{
public:
    typedef basic_string __self;
    typedef basic_string_view<_CharT, _Traits> __self_view;
    typedef _Traits traits_type;
    typedef _CharT value_type;
    typedef _Allocator allocator_type;
    typedef allocator_traits<allocator_type> __alloc_traits;
    typedef typename __alloc_traits::size_type size_type;
    typedef typename __alloc_traits::difference_type difference_type;
    typedef value_type & reference;
    typedef const value_type & const_reference;
    typedef typename __alloc_traits::pointer pointer;
    typedef typename __alloc_traits::const_pointer const_pointer;

    typedef __wrap_iter<pointer> iterator;
    typedef __wrap_iter<const_pointer> const_iterator;
    typedef _VSTD::reverse_iterator<iterator> reverse_iterator;
    typedef _VSTD::reverse_iterator<const_iterator> const_reverse_iterator;

private:
    struct __long
    {
        pointer __data_; //堆对象指针
        size_type __size_;
        size_type __cap_;
    };

    struct __short
    {
        value_type __data_[__min_cap]; //存储在栈上
        struct : __padding<value_type>
        {
            unsigned char __size_;
        };
    };

#ifdef _LIBCPP_BIG_ENDIAN
    static const size_type __short_mask = 0x01;
    static const size_type __long_mask  = 0x1ul;
#else  // _LIBCPP_BIG_ENDIAN
    static const size_type __short_mask = 0x80;
    static const size_type __long_mask  = ~(size_type(~0) >> 1);
#endif // _LIBCPP_BIG_ENDIAN

    enum
    {
        __min_cap = (sizeof(__long) - 1) / sizeof(value_type) > 2 ? (sizeof(__long) - 1) / sizeof(value_type) : 2
    };

#else

    struct __long
    {
        size_type __cap_;
        size_type __size_;
        pointer __data_;
    };

#ifdef _LIBCPP_BIG_ENDIAN
    static const size_type __short_mask = 0x80;
    static const size_type __long_mask  = ~(size_type(~0) >> 1);
#else  // _LIBCPP_BIG_ENDIAN
    static const size_type __short_mask = 0x01;
    static const size_type __long_mask  = 0x1ul;
#endif // _LIBCPP_BIG_ENDIAN

    enum
    {
        __min_cap = (sizeof(__long) - 1) / sizeof(value_type) > 2 ? (sizeof(__long) - 1) / sizeof(value_type) : 2
    };

    struct __short
    {
        union
        {
            unsigned char __size_;
            value_type __lx;
        };
        value_type __data_[__min_cap];
    };

#endif // _LIBCPP_ABI_ALTERNATE_STRING_LAYOUT

    union __ulx
    {
        __long __lx;
        __short __lxx;
    };

    enum
    {
        __n_words = sizeof(__ulx) / sizeof(size_type)
    };

    struct __raw
    {
        size_type __words[__n_words];
    };

    struct __rep
    {
        union
        {
            __long __l;
            __short __s;
            __raw __r;
        };
    };

    __compressed_pair<__rep, allocator_type> __r_;

public:
    _LIBCPP_FUNC_VIS
    static const size_type npos = -1;

    _LIBCPP_INLINE_VISIBILITY basic_string() _NOEXCEPT_(is_nothrow_default_constructible<allocator_type>::value);

    _LIBCPP_INLINE_VISIBILITY explicit basic_string(const allocator_type & __a)
#if _LIBCPP_STD_VER <= 14
        _NOEXCEPT_(is_nothrow_copy_constructible<allocator_type>::value);
#else
        _NOEXCEPT;
#endif

    basic_string(const basic_string & __str);
    basic_string(const basic_string & __str, const allocator_type & __a);

#ifndef _LIBCPP_CXX03_LANG
    _LIBCPP_INLINE_VISIBILITY
    basic_string(basic_string && __str)
#if _LIBCPP_STD_VER <= 14
        _NOEXCEPT_(is_nothrow_move_constructible<allocator_type>::value);
#else
        _NOEXCEPT;
#endif

    _LIBCPP_INLINE_VISIBILITY
    basic_string(basic_string && __str, const allocator_type & __a);
#endif // _LIBCPP_CXX03_LANG

    template <class = _EnableIf<__is_allocator<_Allocator>::value, nullptr_t>>
    _LIBCPP_INLINE_VISIBILITY basic_string(const _CharT * __s) : __r_(__default_init_tag(), __default_init_tag())
    {
        _LIBCPP_ASSERT(__s != nullptr, "basic_string(const char*) detected nullptr");
        __init(__s, traits_type::length(__s));
#if _LIBCPP_DEBUG_LEVEL >= 2
        __get_db()->__insert_c(this);
#endif
    }

    template <class = _EnableIf<__is_allocator<_Allocator>::value, nullptr_t>>
    _LIBCPP_INLINE_VISIBILITY basic_string(const _CharT * __s, const _Allocator & __a);

    _LIBCPP_INLINE_VISIBILITY
    basic_string(const _CharT * __s, size_type __n);
    _LIBCPP_INLINE_VISIBILITY
    basic_string(const _CharT * __s, size_type __n, const _Allocator & __a);
    _LIBCPP_INLINE_VISIBILITY
    basic_string(size_type __n, _CharT __c);

    template <class = _EnableIf<__is_allocator<_Allocator>::value, nullptr_t>>
    _LIBCPP_INLINE_VISIBILITY basic_string(size_type __n, _CharT __c, const _Allocator & __a);

    basic_string(const basic_string & __str, size_type __pos, size_type __n, const _Allocator & __a = _Allocator());
    _LIBCPP_INLINE_VISIBILITY
    basic_string(const basic_string & __str, size_type __pos, const _Allocator & __a = _Allocator());

    template <class _Tp,
              class = _EnableIf<__can_be_converted_to_string_view<_CharT, _Traits, _Tp>::value &&
                                !__is_same_uncvref<_Tp, basic_string>::value>>
    _LIBCPP_METHOD_TEMPLATE_IMPLICIT_INSTANTIATION_VIS basic_string(const _Tp & __t, size_type __pos, size_type __n,
                                                                    const allocator_type & __a = allocator_type());

    template <class _Tp,
              class = _EnableIf<__can_be_converted_to_string_view<_CharT, _Traits, _Tp>::value &&
                                !__is_same_uncvref<_Tp, basic_string>::value>>
    _LIBCPP_METHOD_TEMPLATE_IMPLICIT_INSTANTIATION_VIS explicit basic_string(const _Tp & __t);

    template <class _Tp,
              class = _EnableIf<__can_be_converted_to_string_view<_CharT, _Traits, _Tp>::value &&
                                !__is_same_uncvref<_Tp, basic_string>::value>>
    _LIBCPP_METHOD_TEMPLATE_IMPLICIT_INSTANTIATION_VIS explicit basic_string(const _Tp & __t, const allocator_type & __a);

    template <class _InputIterator, class = _EnableIf<__is_cpp17_input_iterator<_InputIterator>::value>>
    _LIBCPP_INLINE_VISIBILITY basic_string(_InputIterator __first, _InputIterator __last);
    template <class _InputIterator, class = _EnableIf<__is_cpp17_input_iterator<_InputIterator>::value>>
    _LIBCPP_INLINE_VISIBILITY basic_string(_InputIterator __first, _InputIterator __last, const allocator_type & __a);
#ifndef _LIBCPP_CXX03_LANG
    _LIBCPP_INLINE_VISIBILITY
    basic_string(initializer_list<_CharT> __il);
    _LIBCPP_INLINE_VISIBILITY
    basic_string(initializer_list<_CharT> __il, const _Allocator & __a);
#endif // _LIBCPP_CXX03_LANG

    inline ~basic_string();

    _LIBCPP_INLINE_VISIBILITY
    operator __self_view() const _NOEXCEPT
    {
        return __self_view(data(), size());
    }

    basic_string & operator=(const basic_string & __str);

    template <class _Tp,
              class = _EnableIf<__can_be_converted_to_string_view<_CharT, _Traits, _Tp>::value &&
                                !__is_same_uncvref<_Tp, basic_string>::value>>
    basic_string & operator=(const _Tp & __t)
    {
        __self_view __sv = __t;
        return assign(__sv);
    }

#ifndef _LIBCPP_CXX03_LANG
    _LIBCPP_INLINE_VISIBILITY
    basic_string & operator=(basic_string && __str)
        _NOEXCEPT_((__noexcept_move_assign_container<_Allocator, __alloc_traits>::value));
    _LIBCPP_INLINE_VISIBILITY
    basic_string & operator=(initializer_list<value_type> __il)
    {
        return assign(__il.begin(), __il.size());
    }
#endif
    _LIBCPP_INLINE_VISIBILITY basic_string & operator=(const value_type * __s)
    {
        return assign(__s);
    }
    basic_string & operator=(value_type __c);

#if _LIBCPP_DEBUG_LEVEL >= 2
    _LIBCPP_INLINE_VISIBILITY
    iterator begin() _NOEXCEPT
    {
        return iterator(this, __get_pointer());
    }
    _LIBCPP_INLINE_VISIBILITY
    const_iterator begin() const _NOEXCEPT
    {
        return const_iterator(this, __get_pointer());
    }
    _LIBCPP_INLINE_VISIBILITY
    iterator end() _NOEXCEPT
    {
        return iterator(this, __get_pointer() + size());
    }
    _LIBCPP_INLINE_VISIBILITY
    const_iterator end() const _NOEXCEPT
    {
        return const_iterator(this, __get_pointer() + size());
    }
#else
    _LIBCPP_INLINE_VISIBILITY
    iterator begin() _NOEXCEPT
    {
        return iterator(__get_pointer());
    }
    _LIBCPP_INLINE_VISIBILITY
    const_iterator begin() const _NOEXCEPT
    {
        return const_iterator(__get_pointer());
    }
    _LIBCPP_INLINE_VISIBILITY
    iterator end() _NOEXCEPT
    {
        return iterator(__get_pointer() + size());
    }
    _LIBCPP_INLINE_VISIBILITY
    const_iterator end() const _NOEXCEPT
    {
        return const_iterator(__get_pointer() + size());
    }
#endif // _LIBCPP_DEBUG_LEVEL >= 2
    _LIBCPP_INLINE_VISIBILITY
    reverse_iterator rbegin() _NOEXCEPT
    {
        return reverse_iterator(end());
    }
    _LIBCPP_INLINE_VISIBILITY
    const_reverse_iterator rbegin() const _NOEXCEPT
    {
        return const_reverse_iterator(end());
    }
    _LIBCPP_INLINE_VISIBILITY
    reverse_iterator rend() _NOEXCEPT
    {
        return reverse_iterator(begin());
    }
    _LIBCPP_INLINE_VISIBILITY
    const_reverse_iterator rend() const _NOEXCEPT
    {
        return const_reverse_iterator(begin());
    }

    _LIBCPP_INLINE_VISIBILITY
    const_iterator cbegin() const _NOEXCEPT
    {
        return begin();
    }
    _LIBCPP_INLINE_VISIBILITY
    const_iterator cend() const _NOEXCEPT
    {
        return end();
    }
    _LIBCPP_INLINE_VISIBILITY
    const_reverse_iterator crbegin() const _NOEXCEPT
    {
        return rbegin();
    }
    _LIBCPP_INLINE_VISIBILITY
    const_reverse_iterator crend() const _NOEXCEPT
    {
        return rend();
    }

    _LIBCPP_INLINE_VISIBILITY size_type size() const _NOEXCEPT
    {
        return __is_long() ? __get_long_size() : __get_short_size();
    }
    _LIBCPP_INLINE_VISIBILITY size_type length() const _NOEXCEPT
    {
        return size();
    }
    _LIBCPP_INLINE_VISIBILITY size_type max_size() const _NOEXCEPT;
    _LIBCPP_INLINE_VISIBILITY size_type capacity() const _NOEXCEPT
    {
        return (__is_long() ? __get_long_cap() : static_cast<size_type>(__min_cap)) - 1;
    }

    void resize(size_type __n, value_type __c);
    _LIBCPP_INLINE_VISIBILITY void resize(size_type __n)
    {
        resize(__n, value_type());
    }

    void reserve(size_type __res_arg);
    _LIBCPP_INLINE_VISIBILITY void __resize_default_init(size_type __n);

    _LIBCPP_INLINE_VISIBILITY
    void reserve() _NOEXCEPT
    {
        reserve(0);
    }
    _LIBCPP_INLINE_VISIBILITY
    void shrink_to_fit() _NOEXCEPT
    {
        reserve();
    }
    _LIBCPP_INLINE_VISIBILITY
    void clear() _NOEXCEPT;
    _LIBCPP_NODISCARD_AFTER_CXX17 _LIBCPP_INLINE_VISIBILITY bool empty() const _NOEXCEPT
    {
        return size() == 0;
    }

    _LIBCPP_INLINE_VISIBILITY const_reference operator[](size_type __pos) const _NOEXCEPT;
    _LIBCPP_INLINE_VISIBILITY reference operator[](size_type __pos) _NOEXCEPT;

    const_reference at(size_type __n) const;
    reference at(size_type __n);

    _LIBCPP_INLINE_VISIBILITY basic_string & operator+=(const basic_string & __str)
    {
        return append(__str);
    }

    template <class _Tp>
    _LIBCPP_METHOD_TEMPLATE_IMPLICIT_INSTANTIATION_VIS
        _EnableIf<__can_be_converted_to_string_view<_CharT, _Traits, _Tp>::value && !__is_same_uncvref<_Tp, basic_string>::value,
                  basic_string &>
        operator+=(const _Tp & __t)
    {
        __self_view __sv = __t;
        return append(__sv);
    }
    _LIBCPP_INLINE_VISIBILITY basic_string & operator+=(const value_type * __s)
    {
        return append(__s);
    }
    _LIBCPP_INLINE_VISIBILITY basic_string & operator+=(value_type __c)
    {
        push_back(__c);
        return *this;
    }
#ifndef _LIBCPP_CXX03_LANG
    _LIBCPP_INLINE_VISIBILITY basic_string & operator+=(initializer_list<value_type> __il)
    {
        return append(__il);
    }
#endif // _LIBCPP_CXX03_LANG

    _LIBCPP_INLINE_VISIBILITY
    basic_string & append(const basic_string & __str);

    template <class _Tp>
    _LIBCPP_METHOD_TEMPLATE_IMPLICIT_INSTANTIATION_VIS
        _EnableIf<__can_be_converted_to_string_view<_CharT, _Traits, _Tp>::value && !__is_same_uncvref<_Tp, basic_string>::value,
                  basic_string &>
        append(const _Tp & __t)
    {
        __self_view __sv = __t;
        return append(__sv.data(), __sv.size());
    }
    basic_string & append(const basic_string & __str, size_type __pos, size_type __n = npos);

    template <class _Tp>
    _LIBCPP_METHOD_TEMPLATE_IMPLICIT_INSTANTIATION_VIS
        _EnableIf<__can_be_converted_to_string_view<_CharT, _Traits, _Tp>::value && !__is_same_uncvref<_Tp, basic_string>::value,
                  basic_string &>
        append(const _Tp & __t, size_type __pos, size_type __n = npos);
    basic_string & append(const value_type * __s, size_type __n);
    basic_string & append(const value_type * __s);
    basic_string & append(size_type __n, value_type __c);

    _LIBCPP_INLINE_VISIBILITY
    void __append_default_init(size_type __n);

    template <class _ForwardIterator>
    _LIBCPP_METHOD_TEMPLATE_IMPLICIT_INSTANTIATION_VIS basic_string & __append_forward_unsafe(_ForwardIterator, _ForwardIterator);
    template <class _InputIterator>
    _LIBCPP_METHOD_TEMPLATE_IMPLICIT_INSTANTIATION_VIS _EnableIf<__is_exactly_cpp17_input_iterator<_InputIterator>::value ||
                                                                     !__libcpp_string_gets_noexcept_iterator<_InputIterator>::value,
                                                                 basic_string &>
        _LIBCPP_INLINE_VISIBILITY append(_InputIterator __first, _InputIterator __last)
    {
        const basic_string __temp(__first, __last, __alloc());
        append(__temp.data(), __temp.size());
        return *this;
    }
    template <class _ForwardIterator>
    _LIBCPP_METHOD_TEMPLATE_IMPLICIT_INSTANTIATION_VIS
        _EnableIf<__is_cpp17_forward_iterator<_ForwardIterator>::value &&
                      __libcpp_string_gets_noexcept_iterator<_ForwardIterator>::value,
                  basic_string &>
            _LIBCPP_INLINE_VISIBILITY append(_ForwardIterator __first, _ForwardIterator __last)
    {
        return __append_forward_unsafe(__first, __last);
    }

#ifndef _LIBCPP_CXX03_LANG
    _LIBCPP_INLINE_VISIBILITY
    basic_string & append(initializer_list<value_type> __il)
    {
        return append(__il.begin(), __il.size());
    }
#endif // _LIBCPP_CXX03_LANG

    void push_back(value_type __c);
    _LIBCPP_INLINE_VISIBILITY
    void pop_back();
    _LIBCPP_INLINE_VISIBILITY reference front() _NOEXCEPT;
    _LIBCPP_INLINE_VISIBILITY const_reference front() const _NOEXCEPT;
    _LIBCPP_INLINE_VISIBILITY reference back() _NOEXCEPT;
    _LIBCPP_INLINE_VISIBILITY const_reference back() const _NOEXCEPT;

    template <class _Tp>
    _LIBCPP_METHOD_TEMPLATE_IMPLICIT_INSTANTIATION_VIS
        _EnableIf<__can_be_converted_to_string_view<_CharT, _Traits, _Tp>::value, basic_string &>
        assign(const _Tp & __t)
    {
        __self_view __sv = __t;
        return assign(__sv.data(), __sv.size());
    }
    _LIBCPP_INLINE_VISIBILITY
    basic_string & assign(const basic_string & __str)
    {
        return *this = __str;
    }
#ifndef _LIBCPP_CXX03_LANG
    _LIBCPP_INLINE_VISIBILITY
    basic_string & assign(basic_string && __str) _NOEXCEPT_((__noexcept_move_assign_container<_Allocator, __alloc_traits>::value))
    {
        *this = _VSTD::move(__str);
        return *this;
    }
#endif
    basic_string & assign(const basic_string & __str, size_type __pos, size_type __n = npos);
    template <class _Tp>
    _LIBCPP_METHOD_TEMPLATE_IMPLICIT_INSTANTIATION_VIS
        _EnableIf<__can_be_converted_to_string_view<_CharT, _Traits, _Tp>::value && !__is_same_uncvref<_Tp, basic_string>::value,
                  basic_string &>
        assign(const _Tp & __t, size_type __pos, size_type __n = npos);
    basic_string & assign(const value_type * __s, size_type __n);
    basic_string & assign(const value_type * __s);
    basic_string & assign(size_type __n, value_type __c);
    template <class _InputIterator>
    _LIBCPP_METHOD_TEMPLATE_IMPLICIT_INSTANTIATION_VIS _EnableIf<__is_exactly_cpp17_input_iterator<_InputIterator>::value ||
                                                                     !__libcpp_string_gets_noexcept_iterator<_InputIterator>::value,
                                                                 basic_string &>
    assign(_InputIterator __first, _InputIterator __last);
    template <class _ForwardIterator>
    _LIBCPP_METHOD_TEMPLATE_IMPLICIT_INSTANTIATION_VIS
        _EnableIf<__is_cpp17_forward_iterator<_ForwardIterator>::value &&
                      __libcpp_string_gets_noexcept_iterator<_ForwardIterator>::value,
                  basic_string &>
        assign(_ForwardIterator __first, _ForwardIterator __last);
#ifndef _LIBCPP_CXX03_LANG
    _LIBCPP_INLINE_VISIBILITY
    basic_string & assign(initializer_list<value_type> __il)
    {
        return assign(__il.begin(), __il.size());
    }
#endif // _LIBCPP_CXX03_LANG

    _LIBCPP_INLINE_VISIBILITY
    basic_string & insert(size_type __pos1, const basic_string & __str);

    template <class _Tp>
    _LIBCPP_METHOD_TEMPLATE_IMPLICIT_INSTANTIATION_VIS
        _EnableIf<__can_be_converted_to_string_view<_CharT, _Traits, _Tp>::value, basic_string &>
        insert(size_type __pos1, const _Tp & __t)
    {
        __self_view __sv = __t;
        return insert(__pos1, __sv.data(), __sv.size());
    }

    template <class _Tp>
    _LIBCPP_METHOD_TEMPLATE_IMPLICIT_INSTANTIATION_VIS
        _EnableIf<__can_be_converted_to_string_view<_CharT, _Traits, _Tp>::value && !__is_same_uncvref<_Tp, basic_string>::value,
                  basic_string &>
        insert(size_type __pos1, const _Tp & __t, size_type __pos2, size_type __n = npos);
    basic_string & insert(size_type __pos1, const basic_string & __str, size_type __pos2, size_type __n = npos);
    basic_string & insert(size_type __pos, const value_type * __s, size_type __n);
    basic_string & insert(size_type __pos, const value_type * __s);
    basic_string & insert(size_type __pos, size_type __n, value_type __c);
    iterator insert(const_iterator __pos, value_type __c);
    _LIBCPP_INLINE_VISIBILITY
    iterator insert(const_iterator __pos, size_type __n, value_type __c);
    template <class _InputIterator>
    _LIBCPP_METHOD_TEMPLATE_IMPLICIT_INSTANTIATION_VIS _EnableIf<__is_exactly_cpp17_input_iterator<_InputIterator>::value ||
                                                                     !__libcpp_string_gets_noexcept_iterator<_InputIterator>::value,
                                                                 iterator>
    insert(const_iterator __pos, _InputIterator __first, _InputIterator __last);
    template <class _ForwardIterator>
    _LIBCPP_METHOD_TEMPLATE_IMPLICIT_INSTANTIATION_VIS
        _EnableIf<__is_cpp17_forward_iterator<_ForwardIterator>::value &&
                      __libcpp_string_gets_noexcept_iterator<_ForwardIterator>::value,
                  iterator>
        insert(const_iterator __pos, _ForwardIterator __first, _ForwardIterator __last);
#ifndef _LIBCPP_CXX03_LANG
    _LIBCPP_INLINE_VISIBILITY
    iterator insert(const_iterator __pos, initializer_list<value_type> __il)
    {
        return insert(__pos, __il.begin(), __il.end());
    }
#endif // _LIBCPP_CXX03_LANG

    basic_string & erase(size_type __pos = 0, size_type __n = npos);
    _LIBCPP_INLINE_VISIBILITY
    iterator erase(const_iterator __pos);
    _LIBCPP_INLINE_VISIBILITY
    iterator erase(const_iterator __first, const_iterator __last);

    _LIBCPP_INLINE_VISIBILITY
    basic_string & replace(size_type __pos1, size_type __n1, const basic_string & __str);

    template <class _Tp>
    _LIBCPP_METHOD_TEMPLATE_IMPLICIT_INSTANTIATION_VIS
        _EnableIf<__can_be_converted_to_string_view<_CharT, _Traits, _Tp>::value, basic_string &>
        replace(size_type __pos1, size_type __n1, const _Tp & __t)
    {
        __self_view __sv = __t;
        return replace(__pos1, __n1, __sv.data(), __sv.size());
    }
    basic_string & replace(size_type __pos1, size_type __n1, const basic_string & __str, size_type __pos2, size_type __n2 = npos);
    template <class _Tp>
    _LIBCPP_METHOD_TEMPLATE_IMPLICIT_INSTANTIATION_VIS
        _EnableIf<__can_be_converted_to_string_view<_CharT, _Traits, _Tp>::value && !__is_same_uncvref<_Tp, basic_string>::value,
                  basic_string &>
        replace(size_type __pos1, size_type __n1, const _Tp & __t, size_type __pos2, size_type __n2 = npos);
    basic_string & replace(size_type __pos, size_type __n1, const value_type * __s, size_type __n2);
    basic_string & replace(size_type __pos, size_type __n1, const value_type * __s);
    basic_string & replace(size_type __pos, size_type __n1, size_type __n2, value_type __c);
    _LIBCPP_INLINE_VISIBILITY
    basic_string & replace(const_iterator __i1, const_iterator __i2, const basic_string & __str);

    template <class _Tp>
    _LIBCPP_METHOD_TEMPLATE_IMPLICIT_INSTANTIATION_VIS
        _EnableIf<__can_be_converted_to_string_view<_CharT, _Traits, _Tp>::value, basic_string &>
        replace(const_iterator __i1, const_iterator __i2, const _Tp & __t)
    {
        __self_view __sv = __t;
        return replace(__i1 - begin(), __i2 - __i1, __sv);
    }

    _LIBCPP_INLINE_VISIBILITY
    basic_string & replace(const_iterator __i1, const_iterator __i2, const value_type * __s, size_type __n);
    _LIBCPP_INLINE_VISIBILITY
    basic_string & replace(const_iterator __i1, const_iterator __i2, const value_type * __s);
    _LIBCPP_INLINE_VISIBILITY
    basic_string & replace(const_iterator __i1, const_iterator __i2, size_type __n, value_type __c);
    template <class _InputIterator>
    _LIBCPP_METHOD_TEMPLATE_IMPLICIT_INSTANTIATION_VIS _EnableIf<__is_cpp17_input_iterator<_InputIterator>::value, basic_string &>
    replace(const_iterator __i1, const_iterator __i2, _InputIterator __j1, _InputIterator __j2);
#ifndef _LIBCPP_CXX03_LANG
    _LIBCPP_INLINE_VISIBILITY
    basic_string & replace(const_iterator __i1, const_iterator __i2, initializer_list<value_type> __il)
    {
        return replace(__i1, __i2, __il.begin(), __il.end());
    }
#endif // _LIBCPP_CXX03_LANG

    size_type copy(value_type * __s, size_type __n, size_type __pos = 0) const;
    _LIBCPP_INLINE_VISIBILITY
    basic_string substr(size_type __pos = 0, size_type __n = npos) const;

    _LIBCPP_INLINE_VISIBILITY
    void swap(basic_string & __str)
#if _LIBCPP_STD_VER >= 14
        _NOEXCEPT;
#else
        _NOEXCEPT_(!__alloc_traits::propagate_on_container_swap::value || __is_nothrow_swappable<allocator_type>::value);
#endif

    _LIBCPP_INLINE_VISIBILITY
    const value_type * c_str() const _NOEXCEPT
    {
        return data();
    }
    _LIBCPP_INLINE_VISIBILITY
    const value_type * data() const _NOEXCEPT
    {
        return _VSTD::__to_address(__get_pointer());
    }
#if _LIBCPP_STD_VER > 14 || defined(_LIBCPP_BUILDING_LIBRARY)
    _LIBCPP_INLINE_VISIBILITY
    value_type * data() _NOEXCEPT
    {
        return _VSTD::__to_address(__get_pointer());
    }
#endif

    _LIBCPP_INLINE_VISIBILITY
    allocator_type get_allocator() const _NOEXCEPT
    {
        return __alloc();
    }

    _LIBCPP_INLINE_VISIBILITY
    size_type find(const basic_string & __str, size_type __pos = 0) const _NOEXCEPT;

    template <class _Tp>
    _LIBCPP_METHOD_TEMPLATE_IMPLICIT_INSTANTIATION_VIS
        _EnableIf<__can_be_converted_to_string_view<_CharT, _Traits, _Tp>::value, size_type>
        find(const _Tp & __t, size_type __pos = 0) const;
    size_type find(const value_type * __s, size_type __pos, size_type __n) const _NOEXCEPT;
    _LIBCPP_INLINE_VISIBILITY
    size_type find(const value_type * __s, size_type __pos = 0) const _NOEXCEPT;
    size_type find(value_type __c, size_type __pos = 0) const _NOEXCEPT;

    _LIBCPP_INLINE_VISIBILITY
    size_type rfind(const basic_string & __str, size_type __pos = npos) const _NOEXCEPT;

    template <class _Tp>
    _LIBCPP_METHOD_TEMPLATE_IMPLICIT_INSTANTIATION_VIS
        _EnableIf<__can_be_converted_to_string_view<_CharT, _Traits, _Tp>::value, size_type>
        rfind(const _Tp & __t, size_type __pos = npos) const;
    size_type rfind(const value_type * __s, size_type __pos, size_type __n) const _NOEXCEPT;
    _LIBCPP_INLINE_VISIBILITY
    size_type rfind(const value_type * __s, size_type __pos = npos) const _NOEXCEPT;
    size_type rfind(value_type __c, size_type __pos = npos) const _NOEXCEPT;

    _LIBCPP_INLINE_VISIBILITY
    size_type find_first_of(const basic_string & __str, size_type __pos = 0) const _NOEXCEPT;

    template <class _Tp>
    _LIBCPP_METHOD_TEMPLATE_IMPLICIT_INSTANTIATION_VIS
        _EnableIf<__can_be_converted_to_string_view<_CharT, _Traits, _Tp>::value, size_type>
        find_first_of(const _Tp & __t, size_type __pos = 0) const;
    size_type find_first_of(const value_type * __s, size_type __pos, size_type __n) const _NOEXCEPT;
    _LIBCPP_INLINE_VISIBILITY
    size_type find_first_of(const value_type * __s, size_type __pos = 0) const _NOEXCEPT;
    _LIBCPP_INLINE_VISIBILITY
    size_type find_first_of(value_type __c, size_type __pos = 0) const _NOEXCEPT;

    _LIBCPP_INLINE_VISIBILITY
    size_type find_last_of(const basic_string & __str, size_type __pos = npos) const _NOEXCEPT;

    template <class _Tp>
    _LIBCPP_METHOD_TEMPLATE_IMPLICIT_INSTANTIATION_VIS
        _EnableIf<__can_be_converted_to_string_view<_CharT, _Traits, _Tp>::value, size_type>
        find_last_of(const _Tp & __t, size_type __pos = npos) const;
    size_type find_last_of(const value_type * __s, size_type __pos, size_type __n) const _NOEXCEPT;
    _LIBCPP_INLINE_VISIBILITY
    size_type find_last_of(const value_type * __s, size_type __pos = npos) const _NOEXCEPT;
    _LIBCPP_INLINE_VISIBILITY
    size_type find_last_of(value_type __c, size_type __pos = npos) const _NOEXCEPT;

    _LIBCPP_INLINE_VISIBILITY
    size_type find_first_not_of(const basic_string & __str, size_type __pos = 0) const _NOEXCEPT;

    template <class _Tp>
    _LIBCPP_METHOD_TEMPLATE_IMPLICIT_INSTANTIATION_VIS
        _EnableIf<__can_be_converted_to_string_view<_CharT, _Traits, _Tp>::value, size_type>
        find_first_not_of(const _Tp & __t, size_type __pos = 0) const;
    size_type find_first_not_of(const value_type * __s, size_type __pos, size_type __n) const _NOEXCEPT;
    _LIBCPP_INLINE_VISIBILITY
    size_type find_first_not_of(const value_type * __s, size_type __pos = 0) const _NOEXCEPT;
    _LIBCPP_INLINE_VISIBILITY
    size_type find_first_not_of(value_type __c, size_type __pos = 0) const _NOEXCEPT;

    _LIBCPP_INLINE_VISIBILITY
    size_type find_last_not_of(const basic_string & __str, size_type __pos = npos) const _NOEXCEPT;

    template <class _Tp>
    _LIBCPP_METHOD_TEMPLATE_IMPLICIT_INSTANTIATION_VIS
        _EnableIf<__can_be_converted_to_string_view<_CharT, _Traits, _Tp>::value, size_type>
        find_last_not_of(const _Tp & __t, size_type __pos = npos) const;
    size_type find_last_not_of(const value_type * __s, size_type __pos, size_type __n) const _NOEXCEPT;
    _LIBCPP_INLINE_VISIBILITY
    size_type find_last_not_of(const value_type * __s, size_type __pos = npos) const _NOEXCEPT;
    _LIBCPP_INLINE_VISIBILITY
    size_type find_last_not_of(value_type __c, size_type __pos = npos) const _NOEXCEPT;

    _LIBCPP_INLINE_VISIBILITY
    int compare(const basic_string & __str) const _NOEXCEPT;

    template <class _Tp>
    _LIBCPP_METHOD_TEMPLATE_IMPLICIT_INSTANTIATION_VIS
        _EnableIf<__can_be_converted_to_string_view<_CharT, _Traits, _Tp>::value, int>
        compare(const _Tp & __t) const;

    template <class _Tp>
    _LIBCPP_METHOD_TEMPLATE_IMPLICIT_INSTANTIATION_VIS
        _EnableIf<__can_be_converted_to_string_view<_CharT, _Traits, _Tp>::value, int>
        compare(size_type __pos1, size_type __n1, const _Tp & __t) const;

    _LIBCPP_INLINE_VISIBILITY
    int compare(size_type __pos1, size_type __n1, const basic_string & __str) const;
    int compare(size_type __pos1, size_type __n1, const basic_string & __str, size_type __pos2, size_type __n2 = npos) const;

    template <class _Tp>
    inline _LIBCPP_INLINE_VISIBILITY
        _EnableIf<__can_be_converted_to_string_view<_CharT, _Traits, _Tp>::value && !__is_same_uncvref<_Tp, basic_string>::value,
                  int>
        compare(size_type __pos1, size_type __n1, const _Tp & __t, size_type __pos2, size_type __n2 = npos) const;
    int compare(const value_type * __s) const _NOEXCEPT;
    int compare(size_type __pos1, size_type __n1, const value_type * __s) const;
    int compare(size_type __pos1, size_type __n1, const value_type * __s, size_type __n2) const;

#if _LIBCPP_STD_VER > 17
    _LIBCPP_CONSTEXPR_AFTER_CXX11 _LIBCPP_INLINE_VISIBILITY bool starts_with(__self_view __sv) const _NOEXCEPT
    {
        return __self_view(data(), size()).starts_with(__sv);
    }

    _LIBCPP_CONSTEXPR_AFTER_CXX11 _LIBCPP_INLINE_VISIBILITY bool starts_with(value_type __c) const _NOEXCEPT
    {
        return !empty() && _Traits::eq(front(), __c);
    }

    _LIBCPP_CONSTEXPR_AFTER_CXX11 _LIBCPP_INLINE_VISIBILITY bool starts_with(const value_type * __s) const _NOEXCEPT
    {
        return starts_with(__self_view(__s));
    }

    _LIBCPP_CONSTEXPR_AFTER_CXX11 _LIBCPP_INLINE_VISIBILITY bool ends_with(__self_view __sv) const _NOEXCEPT
    {
        return __self_view(data(), size()).ends_with(__sv);
    }

    _LIBCPP_CONSTEXPR_AFTER_CXX11 _LIBCPP_INLINE_VISIBILITY bool ends_with(value_type __c) const _NOEXCEPT
    {
        return !empty() && _Traits::eq(back(), __c);
    }

    _LIBCPP_CONSTEXPR_AFTER_CXX11 _LIBCPP_INLINE_VISIBILITY bool ends_with(const value_type * __s) const _NOEXCEPT
    {
        return ends_with(__self_view(__s));
    }
#endif

    _LIBCPP_INLINE_VISIBILITY bool __invariants() const;

    _LIBCPP_INLINE_VISIBILITY void __clear_and_shrink() _NOEXCEPT;

    _LIBCPP_INLINE_VISIBILITY
    bool __is_long() const _NOEXCEPT
    {
        return bool(__r_.first().__s.__size_ & __short_mask);
    }

#if _LIBCPP_DEBUG_LEVEL >= 2

    bool __dereferenceable(const const_iterator * __i) const;
    bool __decrementable(const const_iterator * __i) const;
    bool __addable(const const_iterator * __i, ptrdiff_t __n) const;
    bool __subscriptable(const const_iterator * __i, ptrdiff_t __n) const;

#endif // _LIBCPP_DEBUG_LEVEL >= 2

private:
    _LIBCPP_INLINE_VISIBILITY
    allocator_type & __alloc() _NOEXCEPT
    {
        return __r_.second();
    }
    _LIBCPP_INLINE_VISIBILITY
    const allocator_type & __alloc() const _NOEXCEPT
    {
        return __r_.second();
    }

#ifdef _LIBCPP_ABI_ALTERNATE_STRING_LAYOUT

    _LIBCPP_INLINE_VISIBILITY
    void __set_short_size(size_type __s) _NOEXCEPT
#ifdef _LIBCPP_BIG_ENDIAN
    {
        __r_.first().__s.__size_ = (unsigned char) (__s << 1);
    }
#else
    {
        __r_.first().__s.__size_ = (unsigned char) (__s);
    }
#endif

    _LIBCPP_INLINE_VISIBILITY
    size_type __get_short_size() const _NOEXCEPT
#ifdef _LIBCPP_BIG_ENDIAN
    {
        return __r_.first().__s.__size_ >> 1;
    }
#else
    {
        return __r_.first().__s.__size_;
    }
#endif

#else // _LIBCPP_ABI_ALTERNATE_STRING_LAYOUT

    _LIBCPP_INLINE_VISIBILITY
    void __set_short_size(size_type __s) _NOEXCEPT
#ifdef _LIBCPP_BIG_ENDIAN
    {
        __r_.first().__s.__size_ = (unsigned char) (__s);
    }
#else
    {
        __r_.first().__s.__size_ = (unsigned char) (__s << 1);
    }
#endif

    _LIBCPP_INLINE_VISIBILITY
    size_type __get_short_size() const _NOEXCEPT
#ifdef _LIBCPP_BIG_ENDIAN
    {
        return __r_.first().__s.__size_;
    }
#else
    {
        return __r_.first().__s.__size_ >> 1;
    }
#endif

#endif // _LIBCPP_ABI_ALTERNATE_STRING_LAYOUT

    _LIBCPP_INLINE_VISIBILITY
    void __set_long_size(size_type __s) _NOEXCEPT
    {
        __r_.first().__l.__size_ = __s;
    }
    _LIBCPP_INLINE_VISIBILITY
    size_type __get_long_size() const _NOEXCEPT
    {
        return __r_.first().__l.__size_;
    }
    _LIBCPP_INLINE_VISIBILITY
    void __set_size(size_type __s) _NOEXCEPT
    {
        if (__is_long())
            __set_long_size(__s);
        else
            __set_short_size(__s);
    }

    _LIBCPP_INLINE_VISIBILITY
    void __set_long_cap(size_type __s) _NOEXCEPT
    {
        __r_.first().__l.__cap_ = __long_mask | __s;
    }
    _LIBCPP_INLINE_VISIBILITY
    size_type __get_long_cap() const _NOEXCEPT
    {
        return __r_.first().__l.__cap_ & size_type(~__long_mask);
    }

    _LIBCPP_INLINE_VISIBILITY
    void __set_long_pointer(pointer __p) _NOEXCEPT
    {
        __r_.first().__l.__data_ = __p;
    }
    _LIBCPP_INLINE_VISIBILITY
    pointer __get_long_pointer() _NOEXCEPT
    {
        return __r_.first().__l.__data_;
    }
    _LIBCPP_INLINE_VISIBILITY
    const_pointer __get_long_pointer() const _NOEXCEPT
    {
        return __r_.first().__l.__data_;
    }
    _LIBCPP_INLINE_VISIBILITY
    pointer __get_short_pointer() _NOEXCEPT
    {
        return pointer_traits<pointer>::pointer_to(__r_.first().__s.__data_[0]);
    }
    _LIBCPP_INLINE_VISIBILITY
    const_pointer __get_short_pointer() const _NOEXCEPT
    {
        return pointer_traits<const_pointer>::pointer_to(__r_.first().__s.__data_[0]);
    }
    _LIBCPP_INLINE_VISIBILITY
    pointer __get_pointer() _NOEXCEPT
    {
        return __is_long() ? __get_long_pointer() : __get_short_pointer();
    }
    _LIBCPP_INLINE_VISIBILITY
    const_pointer __get_pointer() const _NOEXCEPT
    {
        return __is_long() ? __get_long_pointer() : __get_short_pointer();
    }

    _LIBCPP_INLINE_VISIBILITY
    void __zero() _NOEXCEPT
    {
        size_type(&__a)[__n_words] = __r_.first().__r.__words;
        for (unsigned __i = 0; __i < __n_words; ++__i)
            __a[__i] = 0;
    }

    template <size_type __a>
    static _LIBCPP_INLINE_VISIBILITY size_type __align_it(size_type __s) _NOEXCEPT
    {
        return (__s + (__a - 1)) & ~(__a - 1);
    }
    enum
    {
        __alignment = 16
    };
    static _LIBCPP_INLINE_VISIBILITY size_type __recommend(size_type __s) _NOEXCEPT
    {
        if (__s < __min_cap)
            return static_cast<size_type>(__min_cap) - 1;
        size_type __guess = __align_it < sizeof(value_type) < __alignment ? __alignment / sizeof(value_type) : 1 > (__s + 1) - 1;
        if (__guess == __min_cap)
            ++__guess;
        return __guess;
    }

    inline void __init(const value_type * __s, size_type __sz, size_type __reserve);
    inline void __init(const value_type * __s, size_type __sz);
    inline void __init(size_type __n, value_type __c);

    // Slow path for the (inlined) copy constructor for 'long' strings.
    // Always externally instantiated and not inlined.
    // Requires that __s is zero terminated.
    // The main reason for this function to exist is because for unstable, we
    // want to allow inlining of the copy constructor. However, we don't want
    // to call the __init() functions as those are marked as inline which may
    // result in over-aggressive inlining by the compiler, where our aim is
    // to only inline the fast path code directly in the ctor.
    void __init_copy_ctor_external(const value_type * __s, size_type __sz);

    template <class _InputIterator>
    inline _EnableIf<__is_exactly_cpp17_input_iterator<_InputIterator>::value> __init(_InputIterator __first,
                                                                                      _InputIterator __last);

    template <class _ForwardIterator>
    inline _EnableIf<__is_cpp17_forward_iterator<_ForwardIterator>::value> __init(_ForwardIterator __first,
                                                                                  _ForwardIterator __last);

    void __grow_by(size_type __old_cap, size_type __delta_cap, size_type __old_sz, size_type __n_copy, size_type __n_del,
                   size_type __n_add = 0);
    void __grow_by_and_replace(size_type __old_cap, size_type __delta_cap, size_type __old_sz, size_type __n_copy,
                               size_type __n_del, size_type __n_add, const value_type * __p_new_stuff);

    // __assign_no_alias is invoked for assignment operations where we
    // have proof that the input does not alias the current instance.
    // For example, operator=(basic_string) performs a 'self' check.
    template <bool __is_short>
    basic_string & __assign_no_alias(const value_type * __s, size_type __n);

    _LIBCPP_INLINE_VISIBILITY
    void __erase_to_end(size_type __pos);

    // __erase_external_with_move is invoked for erase() invocations where
    // `n ~= npos`, likely requiring memory moves on the string data.
    void __erase_external_with_move(size_type __pos, size_type __n);

    _LIBCPP_INLINE_VISIBILITY
    void __copy_assign_alloc(const basic_string & __str)
    {
        __copy_assign_alloc(__str, integral_constant<bool, __alloc_traits::propagate_on_container_copy_assignment::value>());
    }

    _LIBCPP_INLINE_VISIBILITY
    void __copy_assign_alloc(const basic_string & __str, true_type)
    {
        if (__alloc() == __str.__alloc())
            __alloc() = __str.__alloc();
        else
        {
            if (!__str.__is_long())
            {
                __clear_and_shrink();
                __alloc() = __str.__alloc();
            }
            else
            {
                allocator_type __a = __str.__alloc();
                pointer __p        = __alloc_traits::allocate(__a, __str.__get_long_cap());
                __clear_and_shrink();
                __alloc() = _VSTD::move(__a);
                __set_long_pointer(__p);
                __set_long_cap(__str.__get_long_cap());
                __set_long_size(__str.size());
            }
        }
    }

    _LIBCPP_INLINE_VISIBILITY
    void __copy_assign_alloc(const basic_string &, false_type) _NOEXCEPT {}

#ifndef _LIBCPP_CXX03_LANG
    _LIBCPP_INLINE_VISIBILITY
    void __move_assign(basic_string & __str, false_type) _NOEXCEPT_(__alloc_traits::is_always_equal::value);
    _LIBCPP_INLINE_VISIBILITY
    void __move_assign(basic_string & __str, true_type)
#if _LIBCPP_STD_VER > 14
        _NOEXCEPT;
#else
        _NOEXCEPT_(is_nothrow_move_assignable<allocator_type>::value);
#endif
#endif

    _LIBCPP_INLINE_VISIBILITY
    void __move_assign_alloc(basic_string & __str) _NOEXCEPT_(!__alloc_traits::propagate_on_container_move_assignment::value ||
                                                              is_nothrow_move_assignable<allocator_type>::value)
    {
        __move_assign_alloc(__str, integral_constant<bool, __alloc_traits::propagate_on_container_move_assignment::value>());
    }

    _LIBCPP_INLINE_VISIBILITY
    void __move_assign_alloc(basic_string & __c, true_type) _NOEXCEPT_(is_nothrow_move_assignable<allocator_type>::value)
    {
        __alloc() = _VSTD::move(__c.__alloc());
    }

    _LIBCPP_INLINE_VISIBILITY
    void __move_assign_alloc(basic_string &, false_type) _NOEXCEPT {}

    basic_string & __assign_external(const value_type * __s);
    basic_string & __assign_external(const value_type * __s, size_type __n);

    // Assigns the value in __s, guaranteed to be __n < __min_cap in length.
    inline basic_string & __assign_short(const value_type * __s, size_type __n)
    {
        pointer __p = __is_long() ? (__set_long_size(__n), __get_long_pointer()) : (__set_short_size(__n), __get_short_pointer());
        traits_type::move(_VSTD::__to_address(__p), __s, __n);
        traits_type::assign(__p[__n], value_type());
        return *this;
    }

    _LIBCPP_INLINE_VISIBILITY void __invalidate_all_iterators();
    _LIBCPP_INLINE_VISIBILITY void __invalidate_iterators_past(size_type);

    friend basic_string operator+<>(const basic_string &, const basic_string &);
    friend basic_string operator+<>(const value_type *, const basic_string &);
    friend basic_string operator+<>(value_type, const basic_string &);
    friend basic_string operator+<>(const basic_string &, const value_type *);
    friend basic_string operator+<>(const basic_string &, value_type);
};

// These declarations must appear before any functions are implicitly used
// so that they have the correct visibility specifier.
#ifdef _LIBCPP_ABI_STRING_OPTIMIZED_EXTERNAL_INSTANTIATION
_LIBCPP_STRING_UNSTABLE_EXTERN_TEMPLATE_LIST(_LIBCPP_EXTERN_TEMPLATE, char)
_LIBCPP_STRING_UNSTABLE_EXTERN_TEMPLATE_LIST(_LIBCPP_EXTERN_TEMPLATE, wchar_t)
#else
_LIBCPP_STRING_V1_EXTERN_TEMPLATE_LIST(_LIBCPP_EXTERN_TEMPLATE, char)
_LIBCPP_STRING_V1_EXTERN_TEMPLATE_LIST(_LIBCPP_EXTERN_TEMPLATE, wchar_t)
#endif

#ifndef _LIBCPP_HAS_NO_DEDUCTION_GUIDES
template <class _InputIterator, class _CharT = typename iterator_traits<_InputIterator>::value_type,
          class _Allocator = allocator<_CharT>, class = _EnableIf<__is_cpp17_input_iterator<_InputIterator>::value>,
          class = _EnableIf<__is_allocator<_Allocator>::value>>
basic_string(_InputIterator, _InputIterator, _Allocator = _Allocator()) -> basic_string<_CharT, char_traits<_CharT>, _Allocator>;

template <class _CharT, class _Traits, class _Allocator = allocator<_CharT>, class = _EnableIf<__is_allocator<_Allocator>::value>>
explicit basic_string(basic_string_view<_CharT, _Traits>, const _Allocator & = _Allocator())
    -> basic_string<_CharT, _Traits, _Allocator>;

template <class _CharT, class _Traits, class _Allocator = allocator<_CharT>, class = _EnableIf<__is_allocator<_Allocator>::value>,
          class _Sz = typename allocator_traits<_Allocator>::size_type>
basic_string(basic_string_view<_CharT, _Traits>, _Sz, _Sz, const _Allocator & = _Allocator())
    -> basic_string<_CharT, _Traits, _Allocator>;
#endif

template <class _CharT, class _Traits, class _Allocator>
inline void basic_string<_CharT, _Traits, _Allocator>::__invalidate_all_iterators()
{
#if _LIBCPP_DEBUG_LEVEL >= 2
    __get_db()->__invalidate_all(this);
#endif // _LIBCPP_DEBUG_LEVEL >= 2
}

template <class _CharT, class _Traits, class _Allocator>
inline void basic_string<_CharT, _Traits, _Allocator>::__invalidate_iterators_past(size_type
#if _LIBCPP_DEBUG_LEVEL >= 2
                                                                                       __pos
#endif
)
{
#if _LIBCPP_DEBUG_LEVEL >= 2
    __c_node * __c = __get_db()->__find_c_and_lock(this);
    if (__c)
    {
        const_pointer __new_last = __get_pointer() + __pos;
        for (__i_node ** __p = __c->end_; __p != __c->beg_;)
        {
            --__p;
            const_iterator * __i = static_cast<const_iterator *>((*__p)->__i_);
            if (__i->base() > __new_last)
            {
                (*__p)->__c_ = nullptr;
                if (--__c->end_ != __p)
                    memmove(__p, __p + 1, (__c->end_ - __p) * sizeof(__i_node *));
            }
        }
        __get_db()->unlock();
    }
#endif // _LIBCPP_DEBUG_LEVEL >= 2
}

template <class _CharT, class _Traits, class _Allocator>
inline basic_string<_CharT, _Traits, _Allocator>::basic_string()
    _NOEXCEPT_(is_nothrow_default_constructible<allocator_type>::value) :
    __r_(__default_init_tag(), __default_init_tag())
{
#if _LIBCPP_DEBUG_LEVEL >= 2
    __get_db()->__insert_c(this);
#endif
    __zero();
}

template <class _CharT, class _Traits, class _Allocator>
inline basic_string<_CharT, _Traits, _Allocator>::basic_string(const allocator_type & __a)
#if _LIBCPP_STD_VER <= 14
    _NOEXCEPT_(is_nothrow_copy_constructible<allocator_type>::value)
#else
        _NOEXCEPT
#endif
    :
    __r_(__default_init_tag(), __a)
{
#if _LIBCPP_DEBUG_LEVEL >= 2
    __get_db()->__insert_c(this);
#endif
    __zero();
}

template <class _CharT, class _Traits, class _Allocator>
void basic_string<_CharT, _Traits, _Allocator>::__init(const value_type * __s, size_type __sz, size_type __reserve)
{
    if (__reserve > max_size())
        this->__throw_length_error();
    pointer __p;
    if (__reserve < __min_cap)
    {
        __set_short_size(__sz);
        __p = __get_short_pointer();
    }
    else
    {
        size_type __cap = __recommend(__reserve);
        __p             = __alloc_traits::allocate(__alloc(), __cap + 1);
        __set_long_pointer(__p);
        __set_long_cap(__cap + 1);
        __set_long_size(__sz);
    }
    traits_type::copy(_VSTD::__to_address(__p), __s, __sz);
    traits_type::assign(__p[__sz], value_type());
}

template <class _CharT, class _Traits, class _Allocator>
void basic_string<_CharT, _Traits, _Allocator>::__init(const value_type * __s, size_type __sz)
{
    if (__sz > max_size())
        this->__throw_length_error();
    pointer __p;
    if (__sz < __min_cap)
    {
        __set_short_size(__sz);
        __p = __get_short_pointer();
    }
    else
    {
        size_type __cap = __recommend(__sz);
        __p             = __alloc_traits::allocate(__alloc(), __cap + 1);
        __set_long_pointer(__p);
        __set_long_cap(__cap + 1);
        __set_long_size(__sz);
    }
    traits_type::copy(_VSTD::__to_address(__p), __s, __sz);
    traits_type::assign(__p[__sz], value_type());
}

template <class _CharT, class _Traits, class _Allocator>
template <class>
basic_string<_CharT, _Traits, _Allocator>::basic_string(const _CharT * __s, const _Allocator & __a) :
    __r_(__default_init_tag(), __a)
{
    _LIBCPP_ASSERT(__s != nullptr, "basic_string(const char*, allocator) detected nullptr");
    __init(__s, traits_type::length(__s));
#if _LIBCPP_DEBUG_LEVEL >= 2
    __get_db()->__insert_c(this);
#endif
}

template <class _CharT, class _Traits, class _Allocator>
inline basic_string<_CharT, _Traits, _Allocator>::basic_string(const _CharT * __s, size_type __n) :
    __r_(__default_init_tag(), __default_init_tag())
{
    _LIBCPP_ASSERT(__n == 0 || __s != nullptr, "basic_string(const char*, n) detected nullptr");
    __init(__s, __n);
#if _LIBCPP_DEBUG_LEVEL >= 2
    __get_db()->__insert_c(this);
#endif
}

template <class _CharT, class _Traits, class _Allocator>
inline basic_string<_CharT, _Traits, _Allocator>::basic_string(const _CharT * __s, size_type __n, const _Allocator & __a) :
    __r_(__default_init_tag(), __a)
{
    _LIBCPP_ASSERT(__n == 0 || __s != nullptr, "basic_string(const char*, n, allocator) detected nullptr");
    __init(__s, __n);
#if _LIBCPP_DEBUG_LEVEL >= 2
    __get_db()->__insert_c(this);
#endif
}

template <class _CharT, class _Traits, class _Allocator>
basic_string<_CharT, _Traits, _Allocator>::basic_string(const basic_string & __str) :
    __r_(__default_init_tag(), __alloc_traits::select_on_container_copy_construction(__str.__alloc()))
{
    if (!__str.__is_long())
        __r_.first().__r = __str.__r_.first().__r;
    else
        __init_copy_ctor_external(_VSTD::__to_address(__str.__get_long_pointer()), __str.__get_long_size());

#if _LIBCPP_DEBUG_LEVEL >= 2
    __get_db()->__insert_c(this);
#endif
}

template <class _CharT, class _Traits, class _Allocator>
basic_string<_CharT, _Traits, _Allocator>::basic_string(const basic_string & __str, const allocator_type & __a) :
    __r_(__default_init_tag(), __a)
{
    if (!__str.__is_long())
        __r_.first().__r = __str.__r_.first().__r;
    else
        __init_copy_ctor_external(_VSTD::__to_address(__str.__get_long_pointer()), __str.__get_long_size());
#if _LIBCPP_DEBUG_LEVEL >= 2
    __get_db()->__insert_c(this);
#endif
}

template <class _CharT, class _Traits, class _Allocator>
void basic_string<_CharT, _Traits, _Allocator>::__init_copy_ctor_external(const value_type * __s, size_type __sz)
{
    pointer __p;
    if (__sz < __min_cap)
    {
        __p = __get_short_pointer();
        __set_short_size(__sz);
    }
    else
    {
        if (__sz > max_size())
            this->__throw_length_error();
        size_t __cap = __recommend(__sz);
        __p          = __alloc_traits::allocate(__alloc(), __cap + 1);
        __set_long_pointer(__p);
        __set_long_cap(__cap + 1);
        __set_long_size(__sz);
    }
    traits_type::copy(_VSTD::__to_address(__p), __s, __sz + 1);
}

#ifndef _LIBCPP_CXX03_LANG

template <class _CharT, class _Traits, class _Allocator>
inline basic_string<_CharT, _Traits, _Allocator>::basic_string(basic_string && __str)
#if _LIBCPP_STD_VER <= 14
    _NOEXCEPT_(is_nothrow_move_constructible<allocator_type>::value)
#else
        _NOEXCEPT
#endif
    :
    __r_(_VSTD::move(__str.__r_))
{
    __str.__zero();
#if _LIBCPP_DEBUG_LEVEL >= 2
    __get_db()->__insert_c(this);
    if (__is_long())
        __get_db()->swap(this, &__str);
#endif
}

template <class _CharT, class _Traits, class _Allocator>
inline basic_string<_CharT, _Traits, _Allocator>::basic_string(basic_string && __str, const allocator_type & __a) :
    __r_(__default_init_tag(), __a)
{
    if (__str.__is_long() && __a != __str.__alloc()) // copy, not move
        __init(_VSTD::__to_address(__str.__get_long_pointer()), __str.__get_long_size());
    else
    {
        __r_.first().__r = __str.__r_.first().__r;
        __str.__zero();
    }
#if _LIBCPP_DEBUG_LEVEL >= 2
    __get_db()->__insert_c(this);
    if (__is_long())
        __get_db()->swap(this, &__str);
#endif
}

#endif // _LIBCPP_CXX03_LANG

template <class _CharT, class _Traits, class _Allocator>
void basic_string<_CharT, _Traits, _Allocator>::__init(size_type __n, value_type __c)
{
    if (__n > max_size())
        this->__throw_length_error();
    pointer __p;
    if (__n < __min_cap)
    {
        __set_short_size(__n);
        __p = __get_short_pointer();
    }
    else
    {
        size_type __cap = __recommend(__n);
        __p             = __alloc_traits::allocate(__alloc(), __cap + 1);
        __set_long_pointer(__p);
        __set_long_cap(__cap + 1);
        __set_long_size(__n);
    }
    traits_type::assign(_VSTD::__to_address(__p), __n, __c);
    traits_type::assign(__p[__n], value_type());
}

template <class _CharT, class _Traits, class _Allocator>
inline basic_string<_CharT, _Traits, _Allocator>::basic_string(size_type __n, _CharT __c) :
    __r_(__default_init_tag(), __default_init_tag())
{
    __init(__n, __c);
#if _LIBCPP_DEBUG_LEVEL >= 2
    __get_db()->__insert_c(this);
#endif
}

template <class _CharT, class _Traits, class _Allocator>
template <class>
basic_string<_CharT, _Traits, _Allocator>::basic_string(size_type __n, _CharT __c, const _Allocator & __a) :
    __r_(__default_init_tag(), __a)
{
    __init(__n, __c);
#if _LIBCPP_DEBUG_LEVEL >= 2
    __get_db()->__insert_c(this);
#endif
}

template <class _CharT, class _Traits, class _Allocator>
basic_string<_CharT, _Traits, _Allocator>::basic_string(const basic_string & __str, size_type __pos, size_type __n,
                                                        const _Allocator & __a) :
    __r_(__default_init_tag(), __a)
{
    size_type __str_sz = __str.size();
    if (__pos > __str_sz)
        this->__throw_out_of_range();
    __init(__str.data() + __pos, _VSTD::min(__n, __str_sz - __pos));
#if _LIBCPP_DEBUG_LEVEL >= 2
    __get_db()->__insert_c(this);
#endif
}

template <class _CharT, class _Traits, class _Allocator>
inline basic_string<_CharT, _Traits, _Allocator>::basic_string(const basic_string & __str, size_type __pos,
                                                               const _Allocator & __a) :
    __r_(__default_init_tag(), __a)
{
    size_type __str_sz = __str.size();
    if (__pos > __str_sz)
        this->__throw_out_of_range();
    __init(__str.data() + __pos, __str_sz - __pos);
#if _LIBCPP_DEBUG_LEVEL >= 2
    __get_db()->__insert_c(this);
#endif
}

template <class _CharT, class _Traits, class _Allocator>
template <class _Tp, class>
basic_string<_CharT, _Traits, _Allocator>::basic_string(const _Tp & __t, size_type __pos, size_type __n,
                                                        const allocator_type & __a) :
    __r_(__default_init_tag(), __a)
{
    __self_view __sv0 = __t;
    __self_view __sv  = __sv0.substr(__pos, __n);
    __init(__sv.data(), __sv.size());
#if _LIBCPP_DEBUG_LEVEL >= 2
    __get_db()->__insert_c(this);
#endif
}

template <class _CharT, class _Traits, class _Allocator>
template <class _Tp, class>
basic_string<_CharT, _Traits, _Allocator>::basic_string(const _Tp & __t) : __r_(__default_init_tag(), __default_init_tag())
{
    __self_view __sv = __t;
    __init(__sv.data(), __sv.size());
#if _LIBCPP_DEBUG_LEVEL >= 2
    __get_db()->__insert_c(this);
#endif
}

template <class _CharT, class _Traits, class _Allocator>
template <class _Tp, class>
basic_string<_CharT, _Traits, _Allocator>::basic_string(const _Tp & __t, const _Allocator & __a) : __r_(__default_init_tag(), __a)
{
    __self_view __sv = __t;
    __init(__sv.data(), __sv.size());
#if _LIBCPP_DEBUG_LEVEL >= 2
    __get_db()->__insert_c(this);
#endif
}

template <class _CharT, class _Traits, class _Allocator>
template <class _InputIterator>
_EnableIf<__is_exactly_cpp17_input_iterator<_InputIterator>::value>
basic_string<_CharT, _Traits, _Allocator>::__init(_InputIterator __first, _InputIterator __last)
{
    __zero();
#ifndef _LIBCPP_NO_EXCEPTIONS
    try
    {
#endif // _LIBCPP_NO_EXCEPTIONS
        for (; __first != __last; ++__first)
            push_back(*__first);
#ifndef _LIBCPP_NO_EXCEPTIONS
    } catch (...)
    {
        if (__is_long())
            __alloc_traits::deallocate(__alloc(), __get_long_pointer(), __get_long_cap());
        throw;
    }
#endif // _LIBCPP_NO_EXCEPTIONS
}

template <class _CharT, class _Traits, class _Allocator>
template <class _ForwardIterator>
_EnableIf<__is_cpp17_forward_iterator<_ForwardIterator>::value>
basic_string<_CharT, _Traits, _Allocator>::__init(_ForwardIterator __first, _ForwardIterator __last)
{
    size_type __sz = static_cast<size_type>(_VSTD::distance(__first, __last));
    if (__sz > max_size())
        this->__throw_length_error();
    pointer __p;
    if (__sz < __min_cap)
    {
        __set_short_size(__sz);
        __p = __get_short_pointer();
    }
    else
    {
        size_type __cap = __recommend(__sz);
        __p             = __alloc_traits::allocate(__alloc(), __cap + 1);
        __set_long_pointer(__p);
        __set_long_cap(__cap + 1);
        __set_long_size(__sz);
    }
    for (; __first != __last; ++__first, (void) ++__p)
        traits_type::assign(*__p, *__first);
    traits_type::assign(*__p, value_type());
}

template <class _CharT, class _Traits, class _Allocator>
template <class _InputIterator, class>
inline basic_string<_CharT, _Traits, _Allocator>::basic_string(_InputIterator __first, _InputIterator __last) :
    __r_(__default_init_tag(), __default_init_tag())
{
    __init(__first, __last);
#if _LIBCPP_DEBUG_LEVEL >= 2
    __get_db()->__insert_c(this);
#endif
}

template <class _CharT, class _Traits, class _Allocator>
template <class _InputIterator, class>
inline basic_string<_CharT, _Traits, _Allocator>::basic_string(_InputIterator __first, _InputIterator __last,
                                                               const allocator_type & __a) :
    __r_(__default_init_tag(), __a)
{
    __init(__first, __last);
#if _LIBCPP_DEBUG_LEVEL >= 2
    __get_db()->__insert_c(this);
#endif
}

#ifndef _LIBCPP_CXX03_LANG

template <class _CharT, class _Traits, class _Allocator>
inline basic_string<_CharT, _Traits, _Allocator>::basic_string(initializer_list<_CharT> __il) :
    __r_(__default_init_tag(), __default_init_tag())
{
    __init(__il.begin(), __il.end());
#if _LIBCPP_DEBUG_LEVEL >= 2
    __get_db()->__insert_c(this);
#endif
}

template <class _CharT, class _Traits, class _Allocator>
inline

    basic_string<_CharT, _Traits, _Allocator>::basic_string(initializer_list<_CharT> __il, const _Allocator & __a) :
    __r_(__default_init_tag(), __a)
{
    __init(__il.begin(), __il.end());
#if _LIBCPP_DEBUG_LEVEL >= 2
    __get_db()->__insert_c(this);
#endif
}

#endif // _LIBCPP_CXX03_LANG

template <class _CharT, class _Traits, class _Allocator>
basic_string<_CharT, _Traits, _Allocator>::~basic_string()
{
#if _LIBCPP_DEBUG_LEVEL >= 2
    __get_db()->__erase_c(this);
#endif
    if (__is_long())
        __alloc_traits::deallocate(__alloc(), __get_long_pointer(), __get_long_cap());
}

template <class _CharT, class _Traits, class _Allocator>
void basic_string<_CharT, _Traits, _Allocator>::__grow_by_and_replace(size_type __old_cap, size_type __delta_cap,
                                                                      size_type __old_sz, size_type __n_copy, size_type __n_del,
                                                                      size_type __n_add, const value_type * __p_new_stuff)
{
    size_type __ms = max_size();
    if (__delta_cap > __ms - __old_cap - 1)
        this->__throw_length_error();
    pointer __old_p = __get_pointer();
    size_type __cap =
        __old_cap < __ms / 2 - __alignment ? __recommend(_VSTD::max(__old_cap + __delta_cap, 2 * __old_cap)) : __ms - 1;
    pointer __p = __alloc_traits::allocate(__alloc(), __cap + 1);
    __invalidate_all_iterators();
    if (__n_copy != 0)
        traits_type::copy(_VSTD::__to_address(__p), _VSTD::__to_address(__old_p), __n_copy);
    if (__n_add != 0)
        traits_type::copy(_VSTD::__to_address(__p) + __n_copy, __p_new_stuff, __n_add);
    size_type __sec_cp_sz = __old_sz - __n_del - __n_copy;
    if (__sec_cp_sz != 0)
        traits_type::copy(_VSTD::__to_address(__p) + __n_copy + __n_add, _VSTD::__to_address(__old_p) + __n_copy + __n_del,
                          __sec_cp_sz);
    if (__old_cap + 1 != __min_cap)
        __alloc_traits::deallocate(__alloc(), __old_p, __old_cap + 1);
    __set_long_pointer(__p);
    __set_long_cap(__cap + 1);
    __old_sz = __n_copy + __n_add + __sec_cp_sz;
    __set_long_size(__old_sz);
    traits_type::assign(__p[__old_sz], value_type());
}

template <class _CharT, class _Traits, class _Allocator>
void basic_string<_CharT, _Traits, _Allocator>::__grow_by(size_type __old_cap, size_type __delta_cap, size_type __old_sz,
                                                          size_type __n_copy, size_type __n_del, size_type __n_add)
{
    size_type __ms = max_size();
    if (__delta_cap > __ms - __old_cap)
        this->__throw_length_error();
    pointer __old_p = __get_pointer();
    size_type __cap =
        __old_cap < __ms / 2 - __alignment ? __recommend(_VSTD::max(__old_cap + __delta_cap, 2 * __old_cap)) : __ms - 1;
    pointer __p = __alloc_traits::allocate(__alloc(), __cap + 1);
    __invalidate_all_iterators();
    if (__n_copy != 0)
        traits_type::copy(_VSTD::__to_address(__p), _VSTD::__to_address(__old_p), __n_copy);
    size_type __sec_cp_sz = __old_sz - __n_del - __n_copy;
    if (__sec_cp_sz != 0)
        traits_type::copy(_VSTD::__to_address(__p) + __n_copy + __n_add, _VSTD::__to_address(__old_p) + __n_copy + __n_del,
                          __sec_cp_sz);
    if (__old_cap + 1 != __min_cap)
        __alloc_traits::deallocate(__alloc(), __old_p, __old_cap + 1);
    __set_long_pointer(__p);
    __set_long_cap(__cap + 1);
}

// assign

template <class _CharT, class _Traits, class _Allocator>
template <bool __is_short>
basic_string<_CharT, _Traits, _Allocator> & basic_string<_CharT, _Traits, _Allocator>::__assign_no_alias(const value_type * __s,
                                                                                                         size_type __n)
{
    size_type __cap = __is_short ? __min_cap : __get_long_cap();
    if (__n < __cap)
    {
        pointer __p = __is_short ? __get_short_pointer() : __get_long_pointer();
        __is_short ? __set_short_size(__n) : __set_long_size(__n);
        traits_type::copy(_VSTD::__to_address(__p), __s, __n);
        traits_type::assign(__p[__n], value_type());
        __invalidate_iterators_past(__n);
    }
    else
    {
        size_type __sz = __is_short ? __get_short_size() : __get_long_size();
        __grow_by_and_replace(__cap - 1, __n - __cap + 1, __sz, 0, __sz, __n, __s);
    }
    return *this;
}

template <class _CharT, class _Traits, class _Allocator>
basic_string<_CharT, _Traits, _Allocator> & basic_string<_CharT, _Traits, _Allocator>::__assign_external(const value_type * __s,
                                                                                                         size_type __n)
{
    size_type __cap = capacity();
    if (__cap >= __n)
    {
        value_type * __p = _VSTD::__to_address(__get_pointer());
        traits_type::move(__p, __s, __n);
        traits_type::assign(__p[__n], value_type());
        __set_size(__n);
        __invalidate_iterators_past(__n);
    }
    else
    {
        size_type __sz = size();
        __grow_by_and_replace(__cap, __n - __cap, __sz, 0, __sz, __n, __s);
    }
    return *this;
}

template <class _CharT, class _Traits, class _Allocator>
basic_string<_CharT, _Traits, _Allocator> & basic_string<_CharT, _Traits, _Allocator>::assign(const value_type * __s, size_type __n)
{
    _LIBCPP_ASSERT(__n == 0 || __s != nullptr, "string::assign received nullptr");
    return (_LIBCPP_BUILTIN_CONSTANT_P(__n) && __n < __min_cap) ? __assign_short(__s, __n) : __assign_external(__s, __n);
}

template <class _CharT, class _Traits, class _Allocator>
basic_string<_CharT, _Traits, _Allocator> & basic_string<_CharT, _Traits, _Allocator>::assign(size_type __n, value_type __c)
{
    size_type __cap = capacity();
    if (__cap < __n)
    {
        size_type __sz = size();
        __grow_by(__cap, __n - __cap, __sz, 0, __sz);
    }
    else
        __invalidate_iterators_past(__n);
    value_type * __p = _VSTD::__to_address(__get_pointer());
    traits_type::assign(__p, __n, __c);
    traits_type::assign(__p[__n], value_type());
    __set_size(__n);
    return *this;
}

template <class _CharT, class _Traits, class _Allocator>
basic_string<_CharT, _Traits, _Allocator> & basic_string<_CharT, _Traits, _Allocator>::operator=(value_type __c)
{
    pointer __p;
    if (__is_long())
    {
        __p = __get_long_pointer();
        __set_long_size(1);
    }
    else
    {
        __p = __get_short_pointer();
        __set_short_size(1);
    }
    traits_type::assign(*__p, __c);
    traits_type::assign(*++__p, value_type());
    __invalidate_iterators_past(1);
    return *this;
}

template <class _CharT, class _Traits, class _Allocator>
basic_string<_CharT, _Traits, _Allocator> & basic_string<_CharT, _Traits, _Allocator>::operator=(const basic_string & __str)
{
    if (this != &__str)
    {
        __copy_assign_alloc(__str);
        if (!__is_long())
        {
            if (!__str.__is_long())
            {
                __r_.first().__r = __str.__r_.first().__r;
            }
            else
            {
                return __assign_no_alias<true>(__str.data(), __str.size());
            }
        }
        else
        {
            return __assign_no_alias<false>(__str.data(), __str.size());
        }
    }
    return *this;
}

#ifndef _LIBCPP_CXX03_LANG

template <class _CharT, class _Traits, class _Allocator>
inline void basic_string<_CharT, _Traits, _Allocator>::__move_assign(basic_string & __str, false_type)
    _NOEXCEPT_(__alloc_traits::is_always_equal::value)
{
    if (__alloc() != __str.__alloc())
        assign(__str);
    else
        __move_assign(__str, true_type());
}

template <class _CharT, class _Traits, class _Allocator>
inline void basic_string<_CharT, _Traits, _Allocator>::__move_assign(basic_string & __str, true_type)
#if _LIBCPP_STD_VER > 14
    _NOEXCEPT
#else
    _NOEXCEPT_(is_nothrow_move_assignable<allocator_type>::value)
#endif
{
    if (__is_long())
    {
        __alloc_traits::deallocate(__alloc(), __get_long_pointer(), __get_long_cap());
#if _LIBCPP_STD_VER <= 14
        if (!is_nothrow_move_assignable<allocator_type>::value)
        {
            __set_short_size(0);
            traits_type::assign(__get_short_pointer()[0], value_type());
        }
#endif
    }
    __move_assign_alloc(__str);
    __r_.first() = __str.__r_.first();
    __str.__set_short_size(0);
    traits_type::assign(__str.__get_short_pointer()[0], value_type());
}

template <class _CharT, class _Traits, class _Allocator>
inline basic_string<_CharT, _Traits, _Allocator> & basic_string<_CharT, _Traits, _Allocator>::operator=(basic_string && __str)
    _NOEXCEPT_((__noexcept_move_assign_container<_Allocator, __alloc_traits>::value))
{
    __move_assign(__str, integral_constant<bool, __alloc_traits::propagate_on_container_move_assignment::value>());
    return *this;
}

#endif

template <class _CharT, class _Traits, class _Allocator>
template <class _InputIterator>
_EnableIf<__is_exactly_cpp17_input_iterator<_InputIterator>::value ||
              !__libcpp_string_gets_noexcept_iterator<_InputIterator>::value,
          basic_string<_CharT, _Traits, _Allocator> &>
basic_string<_CharT, _Traits, _Allocator>::assign(_InputIterator __first, _InputIterator __last)
{
    const basic_string __temp(__first, __last, __alloc());
    assign(__temp.data(), __temp.size());
    return *this;
}

template <class _CharT, class _Traits, class _Allocator>
template <class _ForwardIterator>
_EnableIf<__is_cpp17_forward_iterator<_ForwardIterator>::value && __libcpp_string_gets_noexcept_iterator<_ForwardIterator>::value,
          basic_string<_CharT, _Traits, _Allocator> &>
basic_string<_CharT, _Traits, _Allocator>::assign(_ForwardIterator __first, _ForwardIterator __last)
{
    size_type __n   = static_cast<size_type>(_VSTD::distance(__first, __last));
    size_type __cap = capacity();
    if (__cap < __n)
    {
        size_type __sz = size();
        __grow_by(__cap, __n - __cap, __sz, 0, __sz);
    }
    else
        __invalidate_iterators_past(__n);
    pointer __p = __get_pointer();
    for (; __first != __last; ++__first, ++__p)
        traits_type::assign(*__p, *__first);
    traits_type::assign(*__p, value_type());
    __set_size(__n);
    return *this;
}

template <class _CharT, class _Traits, class _Allocator>
basic_string<_CharT, _Traits, _Allocator> & basic_string<_CharT, _Traits, _Allocator>::assign(const basic_string & __str,
                                                                                              size_type __pos, size_type __n)
{
    size_type __sz = __str.size();
    if (__pos > __sz)
        this->__throw_out_of_range();
    return assign(__str.data() + __pos, _VSTD::min(__n, __sz - __pos));
}

template <class _CharT, class _Traits, class _Allocator>
template <class _Tp>
_EnableIf<__can_be_converted_to_string_view<_CharT, _Traits, _Tp>::value &&
              !__is_same_uncvref<_Tp, basic_string<_CharT, _Traits, _Allocator>>::value,
          basic_string<_CharT, _Traits, _Allocator> &>
basic_string<_CharT, _Traits, _Allocator>::assign(const _Tp & __t, size_type __pos, size_type __n)
{
    __self_view __sv = __t;
    size_type __sz   = __sv.size();
    if (__pos > __sz)
        this->__throw_out_of_range();
    return assign(__sv.data() + __pos, _VSTD::min(__n, __sz - __pos));
}

template <class _CharT, class _Traits, class _Allocator>
basic_string<_CharT, _Traits, _Allocator> & basic_string<_CharT, _Traits, _Allocator>::__assign_external(const value_type * __s)
{
    return __assign_external(__s, traits_type::length(__s));
}

template <class _CharT, class _Traits, class _Allocator>
basic_string<_CharT, _Traits, _Allocator> & basic_string<_CharT, _Traits, _Allocator>::assign(const value_type * __s)
{
    _LIBCPP_ASSERT(__s != nullptr, "string::assign received nullptr");
    return _LIBCPP_BUILTIN_CONSTANT_P(*__s)
        ? (traits_type::length(__s) < __min_cap ? __assign_short(__s, traits_type::length(__s))
                                                : __assign_external(__s, traits_type::length(__s)))
        : __assign_external(__s);
}
// append

template <class _CharT, class _Traits, class _Allocator>
basic_string<_CharT, _Traits, _Allocator> & basic_string<_CharT, _Traits, _Allocator>::append(const value_type * __s, size_type __n)
{
    _LIBCPP_ASSERT(__n == 0 || __s != nullptr, "string::append received nullptr");
    size_type __cap = capacity();
    size_type __sz  = size();
    if (__cap - __sz >= __n)
    {
        if (__n)
        {
            value_type * __p = _VSTD::__to_address(__get_pointer());
            traits_type::copy(__p + __sz, __s, __n);
            __sz += __n;
            __set_size(__sz);
            traits_type::assign(__p[__sz], value_type());
        }
    }
    else
        __grow_by_and_replace(__cap, __sz + __n - __cap, __sz, __sz, 0, __n, __s);
    return *this;
}

template <class _CharT, class _Traits, class _Allocator>
basic_string<_CharT, _Traits, _Allocator> & basic_string<_CharT, _Traits, _Allocator>::append(size_type __n, value_type __c)
{
    if (__n)
    {
        size_type __cap = capacity();
        size_type __sz  = size();
        if (__cap - __sz < __n)
            __grow_by(__cap, __sz + __n - __cap, __sz, __sz, 0);
        pointer __p = __get_pointer();
        traits_type::assign(_VSTD::__to_address(__p) + __sz, __n, __c);
        __sz += __n;
        __set_size(__sz);
        traits_type::assign(__p[__sz], value_type());
    }
    return *this;
}

template <class _CharT, class _Traits, class _Allocator>
inline void basic_string<_CharT, _Traits, _Allocator>::__append_default_init(size_type __n)
{
    if (__n)
    {
        size_type __cap = capacity();
        size_type __sz  = size();
        if (__cap - __sz < __n)
            __grow_by(__cap, __sz + __n - __cap, __sz, __sz, 0);
        pointer __p = __get_pointer();
        __sz += __n;
        __set_size(__sz);
        traits_type::assign(__p[__sz], value_type());
    }
}

template <class _CharT, class _Traits, class _Allocator>
void basic_string<_CharT, _Traits, _Allocator>::push_back(value_type __c)
{
    bool __is_short = !__is_long();
    size_type __cap;
    size_type __sz;
    if (__is_short)
    {
        __cap = __min_cap - 1;
        __sz  = __get_short_size();
    }
    else
    {
        __cap = __get_long_cap() - 1;
        __sz  = __get_long_size();
    }
    if (__sz == __cap)
    {
        __grow_by(__cap, 1, __sz, __sz, 0);
        __is_short = !__is_long();
    }
    pointer __p;
    if (__is_short)
    {
        __p = __get_short_pointer() + __sz;
        __set_short_size(__sz + 1);
    }
    else
    {
        __p = __get_long_pointer() + __sz;
        __set_long_size(__sz + 1);
    }
    traits_type::assign(*__p, __c);
    traits_type::assign(*++__p, value_type());
}

template <class _Tp>
bool __ptr_in_range(const _Tp * __p, const _Tp * __first, const _Tp * __last)
{
    return __first <= __p && __p < __last;
}

template <class _Tp1, class _Tp2>
bool __ptr_in_range(const _Tp1 *, const _Tp2 *, const _Tp2 *)
{
    return false;
}

template <class _CharT, class _Traits, class _Allocator>
template <class _ForwardIterator>
basic_string<_CharT, _Traits, _Allocator> &
basic_string<_CharT, _Traits, _Allocator>::__append_forward_unsafe(_ForwardIterator __first, _ForwardIterator __last)
{
    static_assert(__is_cpp17_forward_iterator<_ForwardIterator>::value, "function requires a ForwardIterator");
    size_type __sz  = size();
    size_type __cap = capacity();
    size_type __n   = static_cast<size_type>(_VSTD::distance(__first, __last));
    if (__n)
    {
        typedef typename iterator_traits<_ForwardIterator>::reference _CharRef;
        _CharRef __tmp_ref = *__first;
        if (__ptr_in_range(_VSTD::addressof(__tmp_ref), data(), data() + size()))
        {
            const basic_string __temp(__first, __last, __alloc());
            append(__temp.data(), __temp.size());
        }
        else
        {
            if (__cap - __sz < __n)
                __grow_by(__cap, __sz + __n - __cap, __sz, __sz, 0);
            pointer __p = __get_pointer() + __sz;
            for (; __first != __last; ++__p, ++__first)
                traits_type::assign(*__p, *__first);
            traits_type::assign(*__p, value_type());
            __set_size(__sz + __n);
        }
    }
    return *this;
}

template <class _CharT, class _Traits, class _Allocator>
inline basic_string<_CharT, _Traits, _Allocator> & basic_string<_CharT, _Traits, _Allocator>::append(const basic_string & __str)
{
    return append(__str.data(), __str.size());
}

template <class _CharT, class _Traits, class _Allocator>
basic_string<_CharT, _Traits, _Allocator> & basic_string<_CharT, _Traits, _Allocator>::append(const basic_string & __str,
                                                                                              size_type __pos, size_type __n)
{
    size_type __sz = __str.size();
    if (__pos > __sz)
        this->__throw_out_of_range();
    return append(__str.data() + __pos, _VSTD::min(__n, __sz - __pos));
}

template <class _CharT, class _Traits, class _Allocator>
template <class _Tp>
_EnableIf<__can_be_converted_to_string_view<_CharT, _Traits, _Tp>::value &&
              !__is_same_uncvref<_Tp, basic_string<_CharT, _Traits, _Allocator>>::value,
          basic_string<_CharT, _Traits, _Allocator> &>
basic_string<_CharT, _Traits, _Allocator>::append(const _Tp & __t, size_type __pos, size_type __n)
{
    __self_view __sv = __t;
    size_type __sz   = __sv.size();
    if (__pos > __sz)
        this->__throw_out_of_range();
    return append(__sv.data() + __pos, _VSTD::min(__n, __sz - __pos));
}

template <class _CharT, class _Traits, class _Allocator>
basic_string<_CharT, _Traits, _Allocator> & basic_string<_CharT, _Traits, _Allocator>::append(const value_type * __s)
{
    _LIBCPP_ASSERT(__s != nullptr, "string::append received nullptr");
    return append(__s, traits_type::length(__s));
}

// insert

template <class _CharT, class _Traits, class _Allocator>
basic_string<_CharT, _Traits, _Allocator> & basic_string<_CharT, _Traits, _Allocator>::insert(size_type __pos,
                                                                                              const value_type * __s, size_type __n)
{
    _LIBCPP_ASSERT(__n == 0 || __s != nullptr, "string::insert received nullptr");
    size_type __sz = size();
    if (__pos > __sz)
        this->__throw_out_of_range();
    size_type __cap = capacity();
    if (__cap - __sz >= __n)
    {
        if (__n)
        {
            value_type * __p   = _VSTD::__to_address(__get_pointer());
            size_type __n_move = __sz - __pos;
            if (__n_move != 0)
            {
                if (__p + __pos <= __s && __s < __p + __sz)
                    __s += __n;
                traits_type::move(__p + __pos + __n, __p + __pos, __n_move);
            }
            traits_type::move(__p + __pos, __s, __n);
            __sz += __n;
            __set_size(__sz);
            traits_type::assign(__p[__sz], value_type());
        }
    }
    else
        __grow_by_and_replace(__cap, __sz + __n - __cap, __sz, __pos, 0, __n, __s);
    return *this;
}

template <class _CharT, class _Traits, class _Allocator>
basic_string<_CharT, _Traits, _Allocator> & basic_string<_CharT, _Traits, _Allocator>::insert(size_type __pos, size_type __n,
                                                                                              value_type __c)
{
    size_type __sz = size();
    if (__pos > __sz)
        this->__throw_out_of_range();
    if (__n)
    {
        size_type __cap = capacity();
        value_type * __p;
        if (__cap - __sz >= __n)
        {
            __p                = _VSTD::__to_address(__get_pointer());
            size_type __n_move = __sz - __pos;
            if (__n_move != 0)
                traits_type::move(__p + __pos + __n, __p + __pos, __n_move);
        }
        else
        {
            __grow_by(__cap, __sz + __n - __cap, __sz, __pos, 0, __n);
            __p = _VSTD::__to_address(__get_long_pointer());
        }
        traits_type::assign(__p + __pos, __n, __c);
        __sz += __n;
        __set_size(__sz);
        traits_type::assign(__p[__sz], value_type());
    }
    return *this;
}

template <class _CharT, class _Traits, class _Allocator>
template <class _InputIterator>
_EnableIf<__is_exactly_cpp17_input_iterator<_InputIterator>::value ||
              !__libcpp_string_gets_noexcept_iterator<_InputIterator>::value,
          typename basic_string<_CharT, _Traits, _Allocator>::iterator>
basic_string<_CharT, _Traits, _Allocator>::insert(const_iterator __pos, _InputIterator __first, _InputIterator __last)
{
#if _LIBCPP_DEBUG_LEVEL >= 2
    _LIBCPP_ASSERT(__get_const_db()->__find_c_from_i(&__pos) == this,
                   "string::insert(iterator, range) called with an iterator not"
                   " referring to this string");
#endif
    const basic_string __temp(__first, __last, __alloc());
    return insert(__pos, __temp.data(), __temp.data() + __temp.size());
}

template <class _CharT, class _Traits, class _Allocator>
template <class _ForwardIterator>
_EnableIf<__is_cpp17_forward_iterator<_ForwardIterator>::value && __libcpp_string_gets_noexcept_iterator<_ForwardIterator>::value,
          typename basic_string<_CharT, _Traits, _Allocator>::iterator>
basic_string<_CharT, _Traits, _Allocator>::insert(const_iterator __pos, _ForwardIterator __first, _ForwardIterator __last)
{
#if _LIBCPP_DEBUG_LEVEL >= 2
    _LIBCPP_ASSERT(__get_const_db()->__find_c_from_i(&__pos) == this,
                   "string::insert(iterator, range) called with an iterator not"
                   " referring to this string");
#endif
    size_type __ip = static_cast<size_type>(__pos - begin());
    size_type __n  = static_cast<size_type>(_VSTD::distance(__first, __last));
    if (__n)
    {
        typedef typename iterator_traits<_ForwardIterator>::reference _CharRef;
        _CharRef __tmp_char = *__first;
        if (__ptr_in_range(_VSTD::addressof(__tmp_char), data(), data() + size()))
        {
            const basic_string __temp(__first, __last, __alloc());
            return insert(__pos, __temp.data(), __temp.data() + __temp.size());
        }

        size_type __sz  = size();
        size_type __cap = capacity();
        value_type * __p;
        if (__cap - __sz >= __n)
        {
            __p                = _VSTD::__to_address(__get_pointer());
            size_type __n_move = __sz - __ip;
            if (__n_move != 0)
                traits_type::move(__p + __ip + __n, __p + __ip, __n_move);
        }
        else
        {
            __grow_by(__cap, __sz + __n - __cap, __sz, __ip, 0, __n);
            __p = _VSTD::__to_address(__get_long_pointer());
        }
        __sz += __n;
        __set_size(__sz);
        traits_type::assign(__p[__sz], value_type());
        for (__p += __ip; __first != __last; ++__p, ++__first)
            traits_type::assign(*__p, *__first);
    }
    return begin() + __ip;
}

template <class _CharT, class _Traits, class _Allocator>
inline basic_string<_CharT, _Traits, _Allocator> & basic_string<_CharT, _Traits, _Allocator>::insert(size_type __pos1,
                                                                                                     const basic_string & __str)
{
    return insert(__pos1, __str.data(), __str.size());
}

template <class _CharT, class _Traits, class _Allocator>
basic_string<_CharT, _Traits, _Allocator> &
basic_string<_CharT, _Traits, _Allocator>::insert(size_type __pos1, const basic_string & __str, size_type __pos2, size_type __n)
{
    size_type __str_sz = __str.size();
    if (__pos2 > __str_sz)
        this->__throw_out_of_range();
    return insert(__pos1, __str.data() + __pos2, _VSTD::min(__n, __str_sz - __pos2));
}

template <class _CharT, class _Traits, class _Allocator>
template <class _Tp>
_EnableIf<__can_be_converted_to_string_view<_CharT, _Traits, _Tp>::value &&
              !__is_same_uncvref<_Tp, basic_string<_CharT, _Traits, _Allocator>>::value,
          basic_string<_CharT, _Traits, _Allocator> &>
basic_string<_CharT, _Traits, _Allocator>::insert(size_type __pos1, const _Tp & __t, size_type __pos2, size_type __n)
{
    __self_view __sv   = __t;
    size_type __str_sz = __sv.size();
    if (__pos2 > __str_sz)
        this->__throw_out_of_range();
    return insert(__pos1, __sv.data() + __pos2, _VSTD::min(__n, __str_sz - __pos2));
}

template <class _CharT, class _Traits, class _Allocator>
basic_string<_CharT, _Traits, _Allocator> & basic_string<_CharT, _Traits, _Allocator>::insert(size_type __pos,
                                                                                              const value_type * __s)
{
    _LIBCPP_ASSERT(__s != nullptr, "string::insert received nullptr");
    return insert(__pos, __s, traits_type::length(__s));
}

template <class _CharT, class _Traits, class _Allocator>
typename basic_string<_CharT, _Traits, _Allocator>::iterator basic_string<_CharT, _Traits, _Allocator>::insert(const_iterator __pos,
                                                                                                               value_type __c)
{
    size_type __ip  = static_cast<size_type>(__pos - begin());
    size_type __sz  = size();
    size_type __cap = capacity();
    value_type * __p;
    if (__cap == __sz)
    {
        __grow_by(__cap, 1, __sz, __ip, 0, 1);
        __p = _VSTD::__to_address(__get_long_pointer());
    }
    else
    {
        __p                = _VSTD::__to_address(__get_pointer());
        size_type __n_move = __sz - __ip;
        if (__n_move != 0)
            traits_type::move(__p + __ip + 1, __p + __ip, __n_move);
    }
    traits_type::assign(__p[__ip], __c);
    traits_type::assign(__p[++__sz], value_type());
    __set_size(__sz);
    return begin() + static_cast<difference_type>(__ip);
}

template <class _CharT, class _Traits, class _Allocator>
inline typename basic_string<_CharT, _Traits, _Allocator>::iterator
basic_string<_CharT, _Traits, _Allocator>::insert(const_iterator __pos, size_type __n, value_type __c)
{
#if _LIBCPP_DEBUG_LEVEL >= 2
    _LIBCPP_ASSERT(__get_const_db()->__find_c_from_i(&__pos) == this,
                   "string::insert(iterator, n, value) called with an iterator not"
                   " referring to this string");
#endif
    difference_type __p = __pos - begin();
    insert(static_cast<size_type>(__p), __n, __c);
    return begin() + __p;
}

// replace

template <class _CharT, class _Traits, class _Allocator>
basic_string<_CharT, _Traits, _Allocator> &
basic_string<_CharT, _Traits, _Allocator>::replace(size_type __pos, size_type __n1, const value_type * __s,
                                                   size_type __n2) _LIBCPP_DISABLE_UBSAN_UNSIGNED_INTEGER_CHECK
{
    _LIBCPP_ASSERT(__n2 == 0 || __s != nullptr, "string::replace received nullptr");
    size_type __sz = size();
    if (__pos > __sz)
        this->__throw_out_of_range();
    __n1            = _VSTD::min(__n1, __sz - __pos);
    size_type __cap = capacity();
    if (__cap - __sz + __n1 >= __n2)
    {
        value_type * __p = _VSTD::__to_address(__get_pointer());
        if (__n1 != __n2)
        {
            size_type __n_move = __sz - __pos - __n1;
            if (__n_move != 0)
            {
                if (__n1 > __n2)
                {
                    traits_type::move(__p + __pos, __s, __n2);
                    traits_type::move(__p + __pos + __n2, __p + __pos + __n1, __n_move);
                    goto __finish;
                }
                if (__p + __pos < __s && __s < __p + __sz)
                {
                    if (__p + __pos + __n1 <= __s)
                        __s += __n2 - __n1;
                    else // __p + __pos < __s < __p + __pos + __n1
                    {
                        traits_type::move(__p + __pos, __s, __n1);
                        __pos += __n1;
                        __s += __n2;
                        __n2 -= __n1;
                        __n1 = 0;
                    }
                }
                traits_type::move(__p + __pos + __n2, __p + __pos + __n1, __n_move);
            }
        }
        traits_type::move(__p + __pos, __s, __n2);
    __finish:
        // __sz += __n2 - __n1; in this and the below function below can cause unsigned
        // integer overflow, but this is a safe operation, so we disable the check.
        __sz += __n2 - __n1;
        __set_size(__sz);
        __invalidate_iterators_past(__sz);
        traits_type::assign(__p[__sz], value_type());
    }
    else
        __grow_by_and_replace(__cap, __sz - __n1 + __n2 - __cap, __sz, __pos, __n1, __n2, __s);
    return *this;
}

template <class _CharT, class _Traits, class _Allocator>
basic_string<_CharT, _Traits, _Allocator> &
basic_string<_CharT, _Traits, _Allocator>::replace(size_type __pos, size_type __n1, size_type __n2,
                                                   value_type __c) _LIBCPP_DISABLE_UBSAN_UNSIGNED_INTEGER_CHECK
{
    size_type __sz = size();
    if (__pos > __sz)
        this->__throw_out_of_range();
    __n1            = _VSTD::min(__n1, __sz - __pos);
    size_type __cap = capacity();
    value_type * __p;
    if (__cap - __sz + __n1 >= __n2)
    {
        __p = _VSTD::__to_address(__get_pointer());
        if (__n1 != __n2)
        {
            size_type __n_move = __sz - __pos - __n1;
            if (__n_move != 0)
                traits_type::move(__p + __pos + __n2, __p + __pos + __n1, __n_move);
        }
    }
    else
    {
        __grow_by(__cap, __sz - __n1 + __n2 - __cap, __sz, __pos, __n1, __n2);
        __p = _VSTD::__to_address(__get_long_pointer());
    }
    traits_type::assign(__p + __pos, __n2, __c);
    __sz += __n2 - __n1;
    __set_size(__sz);
    __invalidate_iterators_past(__sz);
    traits_type::assign(__p[__sz], value_type());
    return *this;
}

template <class _CharT, class _Traits, class _Allocator>
template <class _InputIterator>
_EnableIf<__is_cpp17_input_iterator<_InputIterator>::value, basic_string<_CharT, _Traits, _Allocator> &>
basic_string<_CharT, _Traits, _Allocator>::replace(const_iterator __i1, const_iterator __i2, _InputIterator __j1,
                                                   _InputIterator __j2)
{
    const basic_string __temp(__j1, __j2, __alloc());
    return this->replace(__i1, __i2, __temp);
}

template <class _CharT, class _Traits, class _Allocator>
inline basic_string<_CharT, _Traits, _Allocator> &
basic_string<_CharT, _Traits, _Allocator>::replace(size_type __pos1, size_type __n1, const basic_string & __str)
{
    return replace(__pos1, __n1, __str.data(), __str.size());
}

template <class _CharT, class _Traits, class _Allocator>
basic_string<_CharT, _Traits, _Allocator> & basic_string<_CharT, _Traits, _Allocator>::replace(size_type __pos1, size_type __n1,
                                                                                               const basic_string & __str,
                                                                                               size_type __pos2, size_type __n2)
{
    size_type __str_sz = __str.size();
    if (__pos2 > __str_sz)
        this->__throw_out_of_range();
    return replace(__pos1, __n1, __str.data() + __pos2, _VSTD::min(__n2, __str_sz - __pos2));
}

template <class _CharT, class _Traits, class _Allocator>
template <class _Tp>
_EnableIf<__can_be_converted_to_string_view<_CharT, _Traits, _Tp>::value &&
              !__is_same_uncvref<_Tp, basic_string<_CharT, _Traits, _Allocator>>::value,
          basic_string<_CharT, _Traits, _Allocator> &>
basic_string<_CharT, _Traits, _Allocator>::replace(size_type __pos1, size_type __n1, const _Tp & __t, size_type __pos2,
                                                   size_type __n2)
{
    __self_view __sv   = __t;
    size_type __str_sz = __sv.size();
    if (__pos2 > __str_sz)
        this->__throw_out_of_range();
    return replace(__pos1, __n1, __sv.data() + __pos2, _VSTD::min(__n2, __str_sz - __pos2));
}

template <class _CharT, class _Traits, class _Allocator>
basic_string<_CharT, _Traits, _Allocator> & basic_string<_CharT, _Traits, _Allocator>::replace(size_type __pos, size_type __n1,
                                                                                               const value_type * __s)
{
    _LIBCPP_ASSERT(__s != nullptr, "string::replace received nullptr");
    return replace(__pos, __n1, __s, traits_type::length(__s));
}

template <class _CharT, class _Traits, class _Allocator>
inline basic_string<_CharT, _Traits, _Allocator> &
basic_string<_CharT, _Traits, _Allocator>::replace(const_iterator __i1, const_iterator __i2, const basic_string & __str)
{
    return replace(static_cast<size_type>(__i1 - begin()), static_cast<size_type>(__i2 - __i1), __str.data(), __str.size());
}

template <class _CharT, class _Traits, class _Allocator>
inline basic_string<_CharT, _Traits, _Allocator> &
basic_string<_CharT, _Traits, _Allocator>::replace(const_iterator __i1, const_iterator __i2, const value_type * __s, size_type __n)
{
    return replace(static_cast<size_type>(__i1 - begin()), static_cast<size_type>(__i2 - __i1), __s, __n);
}

template <class _CharT, class _Traits, class _Allocator>
inline basic_string<_CharT, _Traits, _Allocator> &
basic_string<_CharT, _Traits, _Allocator>::replace(const_iterator __i1, const_iterator __i2, const value_type * __s)
{
    return replace(static_cast<size_type>(__i1 - begin()), static_cast<size_type>(__i2 - __i1), __s);
}

template <class _CharT, class _Traits, class _Allocator>
inline basic_string<_CharT, _Traits, _Allocator> &
basic_string<_CharT, _Traits, _Allocator>::replace(const_iterator __i1, const_iterator __i2, size_type __n, value_type __c)
{
    return replace(static_cast<size_type>(__i1 - begin()), static_cast<size_type>(__i2 - __i1), __n, __c);
}

// erase

// 'externally instantiated' erase() implementation, called when __n != npos.
// Does not check __pos against size()
template <class _CharT, class _Traits, class _Allocator>
void basic_string<_CharT, _Traits, _Allocator>::__erase_external_with_move(size_type __pos, size_type __n)
{
    if (__n)
    {
        size_type __sz     = size();
        value_type * __p   = _VSTD::__to_address(__get_pointer());
        __n                = _VSTD::min(__n, __sz - __pos);
        size_type __n_move = __sz - __pos - __n;
        if (__n_move != 0)
            traits_type::move(__p + __pos, __p + __pos + __n, __n_move);
        __sz -= __n;
        __set_size(__sz);
        __invalidate_iterators_past(__sz);
        traits_type::assign(__p[__sz], value_type());
    }
}

template <class _CharT, class _Traits, class _Allocator>
basic_string<_CharT, _Traits, _Allocator> & basic_string<_CharT, _Traits, _Allocator>::erase(size_type __pos, size_type __n)
{
    if (__pos > size())
        this->__throw_out_of_range();
    if (__n == npos)
    {
        __erase_to_end(__pos);
    }
    else
    {
        __erase_external_with_move(__pos, __n);
    }
    return *this;
}

template <class _CharT, class _Traits, class _Allocator>
inline typename basic_string<_CharT, _Traits, _Allocator>::iterator
basic_string<_CharT, _Traits, _Allocator>::erase(const_iterator __pos)
{
#if _LIBCPP_DEBUG_LEVEL >= 2
    _LIBCPP_ASSERT(__get_const_db()->__find_c_from_i(&__pos) == this,
                   "string::erase(iterator) called with an iterator not"
                   " referring to this string");
#endif
    _LIBCPP_ASSERT(__pos != end(), "string::erase(iterator) called with a non-dereferenceable iterator");
    iterator __b  = begin();
    size_type __r = static_cast<size_type>(__pos - __b);
    erase(__r, 1);
    return __b + static_cast<difference_type>(__r);
}

template <class _CharT, class _Traits, class _Allocator>
inline typename basic_string<_CharT, _Traits, _Allocator>::iterator
basic_string<_CharT, _Traits, _Allocator>::erase(const_iterator __first, const_iterator __last)
{
#if _LIBCPP_DEBUG_LEVEL >= 2
    _LIBCPP_ASSERT(__get_const_db()->__find_c_from_i(&__first) == this,
                   "string::erase(iterator,  iterator) called with an iterator not"
                   " referring to this string");
#endif
    _LIBCPP_ASSERT(__first <= __last, "string::erase(first, last) called with invalid range");
    iterator __b  = begin();
    size_type __r = static_cast<size_type>(__first - __b);
    erase(__r, static_cast<size_type>(__last - __first));
    return __b + static_cast<difference_type>(__r);
}

template <class _CharT, class _Traits, class _Allocator>
inline void basic_string<_CharT, _Traits, _Allocator>::pop_back()
{
    _LIBCPP_ASSERT(!empty(), "string::pop_back(): string is already empty");
    size_type __sz;
    if (__is_long())
    {
        __sz = __get_long_size() - 1;
        __set_long_size(__sz);
        traits_type::assign(*(__get_long_pointer() + __sz), value_type());
    }
    else
    {
        __sz = __get_short_size() - 1;
        __set_short_size(__sz);
        traits_type::assign(*(__get_short_pointer() + __sz), value_type());
    }
    __invalidate_iterators_past(__sz);
}

template <class _CharT, class _Traits, class _Allocator>
inline void basic_string<_CharT, _Traits, _Allocator>::clear() _NOEXCEPT
{
    __invalidate_all_iterators();
    if (__is_long())
    {
        traits_type::assign(*__get_long_pointer(), value_type());
        __set_long_size(0);
    }
    else
    {
        traits_type::assign(*__get_short_pointer(), value_type());
        __set_short_size(0);
    }
}

template <class _CharT, class _Traits, class _Allocator>
inline void basic_string<_CharT, _Traits, _Allocator>::__erase_to_end(size_type __pos)
{
    if (__is_long())
    {
        traits_type::assign(*(__get_long_pointer() + __pos), value_type());
        __set_long_size(__pos);
    }
    else
    {
        traits_type::assign(*(__get_short_pointer() + __pos), value_type());
        __set_short_size(__pos);
    }
    __invalidate_iterators_past(__pos);
}

template <class _CharT, class _Traits, class _Allocator>
void basic_string<_CharT, _Traits, _Allocator>::resize(size_type __n, value_type __c)
{
    size_type __sz = size();
    if (__n > __sz)
        append(__n - __sz, __c);
    else
        __erase_to_end(__n);
}

template <class _CharT, class _Traits, class _Allocator>
inline void basic_string<_CharT, _Traits, _Allocator>::__resize_default_init(size_type __n)
{
    size_type __sz = size();
    if (__n > __sz)
    {
        __append_default_init(__n - __sz);
    }
    else
        __erase_to_end(__n);
}

template <class _CharT, class _Traits, class _Allocator>
inline typename basic_string<_CharT, _Traits, _Allocator>::size_type
basic_string<_CharT, _Traits, _Allocator>::max_size() const _NOEXCEPT
{
    size_type __m = __alloc_traits::max_size(__alloc());
#ifdef _LIBCPP_BIG_ENDIAN
    return (__m <= ~__long_mask ? __m : __m / 2) - __alignment;
#else
    return __m - __alignment;
#endif
}

template <class _CharT, class _Traits, class _Allocator>
void basic_string<_CharT, _Traits, _Allocator>::reserve(size_type __res_arg)
{
    if (__res_arg > max_size())
        this->__throw_length_error();
    size_type __cap = capacity();
    size_type __sz  = size();
    __res_arg       = _VSTD::max(__res_arg, __sz);
    __res_arg       = __recommend(__res_arg);
    if (__res_arg != __cap)
    {
        pointer __new_data, __p;
        bool __was_long, __now_long;
        if (__res_arg == __min_cap - 1)
        {
            __was_long = true;
            __now_long = false;
            __new_data = __get_short_pointer();
            __p        = __get_long_pointer();
        }
        else
        {
            if (__res_arg > __cap)
                __new_data = __alloc_traits::allocate(__alloc(), __res_arg + 1);
            else
            {
#ifndef _LIBCPP_NO_EXCEPTIONS
                try
                {
#endif // _LIBCPP_NO_EXCEPTIONS
                    __new_data = __alloc_traits::allocate(__alloc(), __res_arg + 1);
#ifndef _LIBCPP_NO_EXCEPTIONS
                } catch (...)
                {
                    return;
                }
#else  // _LIBCPP_NO_EXCEPTIONS
                if (__new_data == nullptr)
                    return;
#endif // _LIBCPP_NO_EXCEPTIONS
            }
            __now_long = true;
            __was_long = __is_long();
            __p        = __get_pointer();
        }
        traits_type::copy(_VSTD::__to_address(__new_data), _VSTD::__to_address(__p), size() + 1);
        if (__was_long)
            __alloc_traits::deallocate(__alloc(), __p, __cap + 1);
        if (__now_long)
        {
            __set_long_cap(__res_arg + 1);
            __set_long_size(__sz);
            __set_long_pointer(__new_data);
        }
        else
            __set_short_size(__sz);
        __invalidate_all_iterators();
    }
}

template <class _CharT, class _Traits, class _Allocator>
inline typename basic_string<_CharT, _Traits, _Allocator>::const_reference
basic_string<_CharT, _Traits, _Allocator>::operator[](size_type __pos) const _NOEXCEPT
{
    _LIBCPP_ASSERT(__pos <= size(), "string index out of bounds");
    return *(data() + __pos);
}

template <class _CharT, class _Traits, class _Allocator>
inline typename basic_string<_CharT, _Traits, _Allocator>::reference
basic_string<_CharT, _Traits, _Allocator>::operator[](size_type __pos) _NOEXCEPT
{
    _LIBCPP_ASSERT(__pos <= size(), "string index out of bounds");
    return *(__get_pointer() + __pos);
}

template <class _CharT, class _Traits, class _Allocator>
typename basic_string<_CharT, _Traits, _Allocator>::const_reference
basic_string<_CharT, _Traits, _Allocator>::at(size_type __n) const
{
    if (__n >= size())
        this->__throw_out_of_range();
    return (*this)[__n];
}

template <class _CharT, class _Traits, class _Allocator>
typename basic_string<_CharT, _Traits, _Allocator>::reference basic_string<_CharT, _Traits, _Allocator>::at(size_type __n)
{
    if (__n >= size())
        this->__throw_out_of_range();
    return (*this)[__n];
}

template <class _CharT, class _Traits, class _Allocator>
inline typename basic_string<_CharT, _Traits, _Allocator>::reference basic_string<_CharT, _Traits, _Allocator>::front() _NOEXCEPT
{
    _LIBCPP_ASSERT(!empty(), "string::front(): string is empty");
    return *__get_pointer();
}

template <class _CharT, class _Traits, class _Allocator>
inline typename basic_string<_CharT, _Traits, _Allocator>::const_reference
basic_string<_CharT, _Traits, _Allocator>::front() const _NOEXCEPT
{
    _LIBCPP_ASSERT(!empty(), "string::front(): string is empty");
    return *data();
}

template <class _CharT, class _Traits, class _Allocator>
inline typename basic_string<_CharT, _Traits, _Allocator>::reference basic_string<_CharT, _Traits, _Allocator>::back() _NOEXCEPT
{
    _LIBCPP_ASSERT(!empty(), "string::back(): string is empty");
    return *(__get_pointer() + size() - 1);
}

template <class _CharT, class _Traits, class _Allocator>
inline typename basic_string<_CharT, _Traits, _Allocator>::const_reference
basic_string<_CharT, _Traits, _Allocator>::back() const _NOEXCEPT
{
    _LIBCPP_ASSERT(!empty(), "string::back(): string is empty");
    return *(data() + size() - 1);
}

template <class _CharT, class _Traits, class _Allocator>
typename basic_string<_CharT, _Traits, _Allocator>::size_type
basic_string<_CharT, _Traits, _Allocator>::copy(value_type * __s, size_type __n, size_type __pos) const
{
    size_type __sz = size();
    if (__pos > __sz)
        this->__throw_out_of_range();
    size_type __rlen = _VSTD::min(__n, __sz - __pos);
    traits_type::copy(__s, data() + __pos, __rlen);
    return __rlen;
}

template <class _CharT, class _Traits, class _Allocator>
inline basic_string<_CharT, _Traits, _Allocator> basic_string<_CharT, _Traits, _Allocator>::substr(size_type __pos,
                                                                                                   size_type __n) const
{
    return basic_string(*this, __pos, __n, __alloc());
}

template <class _CharT, class _Traits, class _Allocator>
inline void basic_string<_CharT, _Traits, _Allocator>::swap(basic_string & __str)
#if _LIBCPP_STD_VER >= 14
    _NOEXCEPT
#else
    _NOEXCEPT_(!__alloc_traits::propagate_on_container_swap::value || __is_nothrow_swappable<allocator_type>::value)
#endif
{
#if _LIBCPP_DEBUG_LEVEL >= 2
    if (!__is_long())
        __get_db()->__invalidate_all(this);
    if (!__str.__is_long())
        __get_db()->__invalidate_all(&__str);
    __get_db()->swap(this, &__str);
#endif
    _LIBCPP_ASSERT(__alloc_traits::propagate_on_container_swap::value || __alloc_traits::is_always_equal::value ||
                       __alloc() == __str.__alloc(),
                   "swapping non-equal allocators");
    _VSTD::swap(__r_.first(), __str.__r_.first());
    __swap_allocator(__alloc(), __str.__alloc());
}

// find

template <class _Traits>
struct _LIBCPP_HIDDEN __traits_eq
{
    typedef typename _Traits::char_type char_type;
    _LIBCPP_INLINE_VISIBILITY
    bool operator()(const char_type & __x, const char_type & __y) _NOEXCEPT { return _Traits::eq(__x, __y); }
};

template <class _CharT, class _Traits, class _Allocator>
typename basic_string<_CharT, _Traits, _Allocator>::size_type
basic_string<_CharT, _Traits, _Allocator>::find(const value_type * __s, size_type __pos, size_type __n) const _NOEXCEPT
{
    _LIBCPP_ASSERT(__n == 0 || __s != nullptr, "string::find(): received nullptr");
    return __str_find<value_type, size_type, traits_type, npos>(data(), size(), __s, __pos, __n);
}

template <class _CharT, class _Traits, class _Allocator>
inline typename basic_string<_CharT, _Traits, _Allocator>::size_type
basic_string<_CharT, _Traits, _Allocator>::find(const basic_string & __str, size_type __pos) const _NOEXCEPT
{
    return __str_find<value_type, size_type, traits_type, npos>(data(), size(), __str.data(), __pos, __str.size());
}

template <class _CharT, class _Traits, class _Allocator>
template <class _Tp>
_EnableIf<__can_be_converted_to_string_view<_CharT, _Traits, _Tp>::value,
          typename basic_string<_CharT, _Traits, _Allocator>::size_type>
basic_string<_CharT, _Traits, _Allocator>::find(const _Tp & __t, size_type __pos) const
{
    __self_view __sv = __t;
    return __str_find<value_type, size_type, traits_type, npos>(data(), size(), __sv.data(), __pos, __sv.size());
}

template <class _CharT, class _Traits, class _Allocator>
inline typename basic_string<_CharT, _Traits, _Allocator>::size_type
basic_string<_CharT, _Traits, _Allocator>::find(const value_type * __s, size_type __pos) const _NOEXCEPT
{
    _LIBCPP_ASSERT(__s != nullptr, "string::find(): received nullptr");
    return __str_find<value_type, size_type, traits_type, npos>(data(), size(), __s, __pos, traits_type::length(__s));
}

template <class _CharT, class _Traits, class _Allocator>
typename basic_string<_CharT, _Traits, _Allocator>::size_type
basic_string<_CharT, _Traits, _Allocator>::find(value_type __c, size_type __pos) const _NOEXCEPT
{
    return __str_find<value_type, size_type, traits_type, npos>(data(), size(), __c, __pos);
}

// rfind

template <class _CharT, class _Traits, class _Allocator>
typename basic_string<_CharT, _Traits, _Allocator>::size_type
basic_string<_CharT, _Traits, _Allocator>::rfind(const value_type * __s, size_type __pos, size_type __n) const _NOEXCEPT
{
    _LIBCPP_ASSERT(__n == 0 || __s != nullptr, "string::rfind(): received nullptr");
    return __str_rfind<value_type, size_type, traits_type, npos>(data(), size(), __s, __pos, __n);
}

template <class _CharT, class _Traits, class _Allocator>
inline typename basic_string<_CharT, _Traits, _Allocator>::size_type
basic_string<_CharT, _Traits, _Allocator>::rfind(const basic_string & __str, size_type __pos) const _NOEXCEPT
{
    return __str_rfind<value_type, size_type, traits_type, npos>(data(), size(), __str.data(), __pos, __str.size());
}

template <class _CharT, class _Traits, class _Allocator>
template <class _Tp>
_EnableIf<__can_be_converted_to_string_view<_CharT, _Traits, _Tp>::value,
          typename basic_string<_CharT, _Traits, _Allocator>::size_type>
basic_string<_CharT, _Traits, _Allocator>::rfind(const _Tp & __t, size_type __pos) const
{
    __self_view __sv = __t;
    return __str_rfind<value_type, size_type, traits_type, npos>(data(), size(), __sv.data(), __pos, __sv.size());
}

template <class _CharT, class _Traits, class _Allocator>
inline typename basic_string<_CharT, _Traits, _Allocator>::size_type
basic_string<_CharT, _Traits, _Allocator>::rfind(const value_type * __s, size_type __pos) const _NOEXCEPT
{
    _LIBCPP_ASSERT(__s != nullptr, "string::rfind(): received nullptr");
    return __str_rfind<value_type, size_type, traits_type, npos>(data(), size(), __s, __pos, traits_type::length(__s));
}

template <class _CharT, class _Traits, class _Allocator>
typename basic_string<_CharT, _Traits, _Allocator>::size_type
basic_string<_CharT, _Traits, _Allocator>::rfind(value_type __c, size_type __pos) const _NOEXCEPT
{
    return __str_rfind<value_type, size_type, traits_type, npos>(data(), size(), __c, __pos);
}

// find_first_of

template <class _CharT, class _Traits, class _Allocator>
typename basic_string<_CharT, _Traits, _Allocator>::size_type
basic_string<_CharT, _Traits, _Allocator>::find_first_of(const value_type * __s, size_type __pos, size_type __n) const _NOEXCEPT
{
    _LIBCPP_ASSERT(__n == 0 || __s != nullptr, "string::find_first_of(): received nullptr");
    return __str_find_first_of<value_type, size_type, traits_type, npos>(data(), size(), __s, __pos, __n);
}

template <class _CharT, class _Traits, class _Allocator>
inline typename basic_string<_CharT, _Traits, _Allocator>::size_type
basic_string<_CharT, _Traits, _Allocator>::find_first_of(const basic_string & __str, size_type __pos) const _NOEXCEPT
{
    return __str_find_first_of<value_type, size_type, traits_type, npos>(data(), size(), __str.data(), __pos, __str.size());
}

template <class _CharT, class _Traits, class _Allocator>
template <class _Tp>
_EnableIf<__can_be_converted_to_string_view<_CharT, _Traits, _Tp>::value,
          typename basic_string<_CharT, _Traits, _Allocator>::size_type>
basic_string<_CharT, _Traits, _Allocator>::find_first_of(const _Tp & __t, size_type __pos) const
{
    __self_view __sv = __t;
    return __str_find_first_of<value_type, size_type, traits_type, npos>(data(), size(), __sv.data(), __pos, __sv.size());
}

template <class _CharT, class _Traits, class _Allocator>
inline typename basic_string<_CharT, _Traits, _Allocator>::size_type
basic_string<_CharT, _Traits, _Allocator>::find_first_of(const value_type * __s, size_type __pos) const _NOEXCEPT
{
    _LIBCPP_ASSERT(__s != nullptr, "string::find_first_of(): received nullptr");
    return __str_find_first_of<value_type, size_type, traits_type, npos>(data(), size(), __s, __pos, traits_type::length(__s));
}

template <class _CharT, class _Traits, class _Allocator>
inline typename basic_string<_CharT, _Traits, _Allocator>::size_type
basic_string<_CharT, _Traits, _Allocator>::find_first_of(value_type __c, size_type __pos) const _NOEXCEPT
{
    return find(__c, __pos);
}

// find_last_of

template <class _CharT, class _Traits, class _Allocator>
typename basic_string<_CharT, _Traits, _Allocator>::size_type
basic_string<_CharT, _Traits, _Allocator>::find_last_of(const value_type * __s, size_type __pos, size_type __n) const _NOEXCEPT
{
    _LIBCPP_ASSERT(__n == 0 || __s != nullptr, "string::find_last_of(): received nullptr");
    return __str_find_last_of<value_type, size_type, traits_type, npos>(data(), size(), __s, __pos, __n);
}

template <class _CharT, class _Traits, class _Allocator>
inline typename basic_string<_CharT, _Traits, _Allocator>::size_type
basic_string<_CharT, _Traits, _Allocator>::find_last_of(const basic_string & __str, size_type __pos) const _NOEXCEPT
{
    return __str_find_last_of<value_type, size_type, traits_type, npos>(data(), size(), __str.data(), __pos, __str.size());
}

template <class _CharT, class _Traits, class _Allocator>
template <class _Tp>
_EnableIf<__can_be_converted_to_string_view<_CharT, _Traits, _Tp>::value,
          typename basic_string<_CharT, _Traits, _Allocator>::size_type>
basic_string<_CharT, _Traits, _Allocator>::find_last_of(const _Tp & __t, size_type __pos) const
{
    __self_view __sv = __t;
    return __str_find_last_of<value_type, size_type, traits_type, npos>(data(), size(), __sv.data(), __pos, __sv.size());
}

template <class _CharT, class _Traits, class _Allocator>
inline typename basic_string<_CharT, _Traits, _Allocator>::size_type
basic_string<_CharT, _Traits, _Allocator>::find_last_of(const value_type * __s, size_type __pos) const _NOEXCEPT
{
    _LIBCPP_ASSERT(__s != nullptr, "string::find_last_of(): received nullptr");
    return __str_find_last_of<value_type, size_type, traits_type, npos>(data(), size(), __s, __pos, traits_type::length(__s));
}

template <class _CharT, class _Traits, class _Allocator>
inline typename basic_string<_CharT, _Traits, _Allocator>::size_type
basic_string<_CharT, _Traits, _Allocator>::find_last_of(value_type __c, size_type __pos) const _NOEXCEPT
{
    return rfind(__c, __pos);
}

// find_first_not_of

template <class _CharT, class _Traits, class _Allocator>
typename basic_string<_CharT, _Traits, _Allocator>::size_type
basic_string<_CharT, _Traits, _Allocator>::find_first_not_of(const value_type * __s, size_type __pos, size_type __n) const _NOEXCEPT
{
    _LIBCPP_ASSERT(__n == 0 || __s != nullptr, "string::find_first_not_of(): received nullptr");
    return __str_find_first_not_of<value_type, size_type, traits_type, npos>(data(), size(), __s, __pos, __n);
}

template <class _CharT, class _Traits, class _Allocator>
inline typename basic_string<_CharT, _Traits, _Allocator>::size_type
basic_string<_CharT, _Traits, _Allocator>::find_first_not_of(const basic_string & __str, size_type __pos) const _NOEXCEPT
{
    return __str_find_first_not_of<value_type, size_type, traits_type, npos>(data(), size(), __str.data(), __pos, __str.size());
}

template <class _CharT, class _Traits, class _Allocator>
template <class _Tp>
_EnableIf<__can_be_converted_to_string_view<_CharT, _Traits, _Tp>::value,
          typename basic_string<_CharT, _Traits, _Allocator>::size_type>
basic_string<_CharT, _Traits, _Allocator>::find_first_not_of(const _Tp & __t, size_type __pos) const
{
    __self_view __sv = __t;
    return __str_find_first_not_of<value_type, size_type, traits_type, npos>(data(), size(), __sv.data(), __pos, __sv.size());
}

template <class _CharT, class _Traits, class _Allocator>
inline typename basic_string<_CharT, _Traits, _Allocator>::size_type
basic_string<_CharT, _Traits, _Allocator>::find_first_not_of(const value_type * __s, size_type __pos) const _NOEXCEPT
{
    _LIBCPP_ASSERT(__s != nullptr, "string::find_first_not_of(): received nullptr");
    return __str_find_first_not_of<value_type, size_type, traits_type, npos>(data(), size(), __s, __pos, traits_type::length(__s));
}

template <class _CharT, class _Traits, class _Allocator>
inline typename basic_string<_CharT, _Traits, _Allocator>::size_type
basic_string<_CharT, _Traits, _Allocator>::find_first_not_of(value_type __c, size_type __pos) const _NOEXCEPT
{
    return __str_find_first_not_of<value_type, size_type, traits_type, npos>(data(), size(), __c, __pos);
}

// find_last_not_of

template <class _CharT, class _Traits, class _Allocator>
typename basic_string<_CharT, _Traits, _Allocator>::size_type
basic_string<_CharT, _Traits, _Allocator>::find_last_not_of(const value_type * __s, size_type __pos, size_type __n) const _NOEXCEPT
{
    _LIBCPP_ASSERT(__n == 0 || __s != nullptr, "string::find_last_not_of(): received nullptr");
    return __str_find_last_not_of<value_type, size_type, traits_type, npos>(data(), size(), __s, __pos, __n);
}

template <class _CharT, class _Traits, class _Allocator>
inline typename basic_string<_CharT, _Traits, _Allocator>::size_type
basic_string<_CharT, _Traits, _Allocator>::find_last_not_of(const basic_string & __str, size_type __pos) const _NOEXCEPT
{
    return __str_find_last_not_of<value_type, size_type, traits_type, npos>(data(), size(), __str.data(), __pos, __str.size());
}

template <class _CharT, class _Traits, class _Allocator>
template <class _Tp>
_EnableIf<__can_be_converted_to_string_view<_CharT, _Traits, _Tp>::value,
          typename basic_string<_CharT, _Traits, _Allocator>::size_type>
basic_string<_CharT, _Traits, _Allocator>::find_last_not_of(const _Tp & __t, size_type __pos) const
{
    __self_view __sv = __t;
    return __str_find_last_not_of<value_type, size_type, traits_type, npos>(data(), size(), __sv.data(), __pos, __sv.size());
}

template <class _CharT, class _Traits, class _Allocator>
inline typename basic_string<_CharT, _Traits, _Allocator>::size_type
basic_string<_CharT, _Traits, _Allocator>::find_last_not_of(const value_type * __s, size_type __pos) const _NOEXCEPT
{
    _LIBCPP_ASSERT(__s != nullptr, "string::find_last_not_of(): received nullptr");
    return __str_find_last_not_of<value_type, size_type, traits_type, npos>(data(), size(), __s, __pos, traits_type::length(__s));
}

template <class _CharT, class _Traits, class _Allocator>
inline typename basic_string<_CharT, _Traits, _Allocator>::size_type
basic_string<_CharT, _Traits, _Allocator>::find_last_not_of(value_type __c, size_type __pos) const _NOEXCEPT
{
    return __str_find_last_not_of<value_type, size_type, traits_type, npos>(data(), size(), __c, __pos);
}

// compare

template <class _CharT, class _Traits, class _Allocator>
template <class _Tp>
_EnableIf<__can_be_converted_to_string_view<_CharT, _Traits, _Tp>::value, int>
basic_string<_CharT, _Traits, _Allocator>::compare(const _Tp & __t) const
{
    __self_view __sv = __t;
    size_t __lhs_sz  = size();
    size_t __rhs_sz  = __sv.size();
    int __result     = traits_type::compare(data(), __sv.data(), _VSTD::min(__lhs_sz, __rhs_sz));
    if (__result != 0)
        return __result;
    if (__lhs_sz < __rhs_sz)
        return -1;
    if (__lhs_sz > __rhs_sz)
        return 1;
    return 0;
}

template <class _CharT, class _Traits, class _Allocator>
inline int basic_string<_CharT, _Traits, _Allocator>::compare(const basic_string & __str) const _NOEXCEPT
{
    return compare(__self_view(__str));
}

template <class _CharT, class _Traits, class _Allocator>
int basic_string<_CharT, _Traits, _Allocator>::compare(size_type __pos1, size_type __n1, const value_type * __s,
                                                       size_type __n2) const
{
    _LIBCPP_ASSERT(__n2 == 0 || __s != nullptr, "string::compare(): received nullptr");
    size_type __sz = size();
    if (__pos1 > __sz || __n2 == npos)
        this->__throw_out_of_range();
    size_type __rlen = _VSTD::min(__n1, __sz - __pos1);
    int __r          = traits_type::compare(data() + __pos1, __s, _VSTD::min(__rlen, __n2));
    if (__r == 0)
    {
        if (__rlen < __n2)
            __r = -1;
        else if (__rlen > __n2)
            __r = 1;
    }
    return __r;
}

template <class _CharT, class _Traits, class _Allocator>
template <class _Tp>
_EnableIf<__can_be_converted_to_string_view<_CharT, _Traits, _Tp>::value, int>
basic_string<_CharT, _Traits, _Allocator>::compare(size_type __pos1, size_type __n1, const _Tp & __t) const
{
    __self_view __sv = __t;
    return compare(__pos1, __n1, __sv.data(), __sv.size());
}

template <class _CharT, class _Traits, class _Allocator>
inline int basic_string<_CharT, _Traits, _Allocator>::compare(size_type __pos1, size_type __n1, const basic_string & __str) const
{
    return compare(__pos1, __n1, __str.data(), __str.size());
}

template <class _CharT, class _Traits, class _Allocator>
template <class _Tp>
_EnableIf<__can_be_converted_to_string_view<_CharT, _Traits, _Tp>::value &&
              !__is_same_uncvref<_Tp, basic_string<_CharT, _Traits, _Allocator>>::value,
          int>
basic_string<_CharT, _Traits, _Allocator>::compare(size_type __pos1, size_type __n1, const _Tp & __t, size_type __pos2,
                                                   size_type __n2) const
{
    __self_view __sv = __t;
    return __self_view(*this).substr(__pos1, __n1).compare(__sv.substr(__pos2, __n2));
}

template <class _CharT, class _Traits, class _Allocator>
int basic_string<_CharT, _Traits, _Allocator>::compare(size_type __pos1, size_type __n1, const basic_string & __str,
                                                       size_type __pos2, size_type __n2) const
{
    return compare(__pos1, __n1, __self_view(__str), __pos2, __n2);
}

template <class _CharT, class _Traits, class _Allocator>
int basic_string<_CharT, _Traits, _Allocator>::compare(const value_type * __s) const _NOEXCEPT
{
    _LIBCPP_ASSERT(__s != nullptr, "string::compare(): received nullptr");
    return compare(0, npos, __s, traits_type::length(__s));
}

template <class _CharT, class _Traits, class _Allocator>
int basic_string<_CharT, _Traits, _Allocator>::compare(size_type __pos1, size_type __n1, const value_type * __s) const
{
    _LIBCPP_ASSERT(__s != nullptr, "string::compare(): received nullptr");
    return compare(__pos1, __n1, __s, traits_type::length(__s));
}

// __invariants

template <class _CharT, class _Traits, class _Allocator>
inline bool basic_string<_CharT, _Traits, _Allocator>::__invariants() const
{
    if (size() > capacity())
        return false;
    if (capacity() < __min_cap - 1)
        return false;
    if (data() == 0)
        return false;
    if (data()[size()] != value_type(0))
        return false;
    return true;
}

// __clear_and_shrink

template <class _CharT, class _Traits, class _Allocator>
inline void basic_string<_CharT, _Traits, _Allocator>::__clear_and_shrink() _NOEXCEPT
{
    clear();
    if (__is_long())
    {
        __alloc_traits::deallocate(__alloc(), __get_long_pointer(), capacity() + 1);
        __set_long_cap(0);
        __set_short_size(0);
    }
}

// operator==

template <class _CharT, class _Traits, class _Allocator>
inline _LIBCPP_INLINE_VISIBILITY bool operator==(const basic_string<_CharT, _Traits, _Allocator> & __lhs,
                                                 const basic_string<_CharT, _Traits, _Allocator> & __rhs) _NOEXCEPT
{
    size_t __lhs_sz = __lhs.size();
    return __lhs_sz == __rhs.size() && _Traits::compare(__lhs.data(), __rhs.data(), __lhs_sz) == 0;
}

template <class _Allocator>
inline _LIBCPP_INLINE_VISIBILITY bool operator==(const basic_string<char, char_traits<char>, _Allocator> & __lhs,
                                                 const basic_string<char, char_traits<char>, _Allocator> & __rhs) _NOEXCEPT
{
    size_t __lhs_sz = __lhs.size();
    if (__lhs_sz != __rhs.size())
        return false;
    const char * __lp = __lhs.data();
    const char * __rp = __rhs.data();
    if (__lhs.__is_long())
        return char_traits<char>::compare(__lp, __rp, __lhs_sz) == 0;
    for (; __lhs_sz != 0; --__lhs_sz, ++__lp, ++__rp)
        if (*__lp != *__rp)
            return false;
    return true;
}

template <class _CharT, class _Traits, class _Allocator>
inline _LIBCPP_INLINE_VISIBILITY bool operator==(const _CharT * __lhs,
                                                 const basic_string<_CharT, _Traits, _Allocator> & __rhs) _NOEXCEPT
{
    typedef basic_string<_CharT, _Traits, _Allocator> _String;
    _LIBCPP_ASSERT(__lhs != nullptr, "operator==(char*, basic_string): received nullptr");
    size_t __lhs_len = _Traits::length(__lhs);
    if (__lhs_len != __rhs.size())
        return false;
    return __rhs.compare(0, _String::npos, __lhs, __lhs_len) == 0;
}

template <class _CharT, class _Traits, class _Allocator>
inline _LIBCPP_INLINE_VISIBILITY bool operator==(const basic_string<_CharT, _Traits, _Allocator> & __lhs,
                                                 const _CharT * __rhs) _NOEXCEPT
{
    typedef basic_string<_CharT, _Traits, _Allocator> _String;
    _LIBCPP_ASSERT(__rhs != nullptr, "operator==(basic_string, char*): received nullptr");
    size_t __rhs_len = _Traits::length(__rhs);
    if (__rhs_len != __lhs.size())
        return false;
    return __lhs.compare(0, _String::npos, __rhs, __rhs_len) == 0;
}

template <class _CharT, class _Traits, class _Allocator>
inline _LIBCPP_INLINE_VISIBILITY bool operator!=(const basic_string<_CharT, _Traits, _Allocator> & __lhs,
                                                 const basic_string<_CharT, _Traits, _Allocator> & __rhs) _NOEXCEPT
{
    return !(__lhs == __rhs);
}

template <class _CharT, class _Traits, class _Allocator>
inline _LIBCPP_INLINE_VISIBILITY bool operator!=(const _CharT * __lhs,
                                                 const basic_string<_CharT, _Traits, _Allocator> & __rhs) _NOEXCEPT
{
    return !(__lhs == __rhs);
}

template <class _CharT, class _Traits, class _Allocator>
inline _LIBCPP_INLINE_VISIBILITY bool operator!=(const basic_string<_CharT, _Traits, _Allocator> & __lhs,
                                                 const _CharT * __rhs) _NOEXCEPT
{
    return !(__lhs == __rhs);
}

// operator<

template <class _CharT, class _Traits, class _Allocator>
inline _LIBCPP_INLINE_VISIBILITY bool operator<(const basic_string<_CharT, _Traits, _Allocator> & __lhs,
                                                const basic_string<_CharT, _Traits, _Allocator> & __rhs) _NOEXCEPT
{
    return __lhs.compare(__rhs) < 0;
}

template <class _CharT, class _Traits, class _Allocator>
inline _LIBCPP_INLINE_VISIBILITY bool operator<(const basic_string<_CharT, _Traits, _Allocator> & __lhs,
                                                const _CharT * __rhs) _NOEXCEPT
{
    return __lhs.compare(__rhs) < 0;
}

template <class _CharT, class _Traits, class _Allocator>
inline _LIBCPP_INLINE_VISIBILITY bool operator<(const _CharT * __lhs,
                                                const basic_string<_CharT, _Traits, _Allocator> & __rhs) _NOEXCEPT
{
    return __rhs.compare(__lhs) > 0;
}

// operator>

template <class _CharT, class _Traits, class _Allocator>
inline _LIBCPP_INLINE_VISIBILITY bool operator>(const basic_string<_CharT, _Traits, _Allocator> & __lhs,
                                                const basic_string<_CharT, _Traits, _Allocator> & __rhs) _NOEXCEPT
{
    return __rhs < __lhs;
}

template <class _CharT, class _Traits, class _Allocator>
inline _LIBCPP_INLINE_VISIBILITY bool operator>(const basic_string<_CharT, _Traits, _Allocator> & __lhs,
                                                const _CharT * __rhs) _NOEXCEPT
{
    return __rhs < __lhs;
}

template <class _CharT, class _Traits, class _Allocator>
inline _LIBCPP_INLINE_VISIBILITY bool operator>(const _CharT * __lhs,
                                                const basic_string<_CharT, _Traits, _Allocator> & __rhs) _NOEXCEPT
{
    return __rhs < __lhs;
}

// operator<=

template <class _CharT, class _Traits, class _Allocator>
inline _LIBCPP_INLINE_VISIBILITY bool operator<=(const basic_string<_CharT, _Traits, _Allocator> & __lhs,
                                                 const basic_string<_CharT, _Traits, _Allocator> & __rhs) _NOEXCEPT
{
    return !(__rhs < __lhs);
}

template <class _CharT, class _Traits, class _Allocator>
inline _LIBCPP_INLINE_VISIBILITY bool operator<=(const basic_string<_CharT, _Traits, _Allocator> & __lhs,
                                                 const _CharT * __rhs) _NOEXCEPT
{
    return !(__rhs < __lhs);
}

template <class _CharT, class _Traits, class _Allocator>
inline _LIBCPP_INLINE_VISIBILITY bool operator<=(const _CharT * __lhs,
                                                 const basic_string<_CharT, _Traits, _Allocator> & __rhs) _NOEXCEPT
{
    return !(__rhs < __lhs);
}

// operator>=

template <class _CharT, class _Traits, class _Allocator>
inline _LIBCPP_INLINE_VISIBILITY bool operator>=(const basic_string<_CharT, _Traits, _Allocator> & __lhs,
                                                 const basic_string<_CharT, _Traits, _Allocator> & __rhs) _NOEXCEPT
{
    return !(__lhs < __rhs);
}

template <class _CharT, class _Traits, class _Allocator>
inline _LIBCPP_INLINE_VISIBILITY bool operator>=(const basic_string<_CharT, _Traits, _Allocator> & __lhs,
                                                 const _CharT * __rhs) _NOEXCEPT
{
    return !(__lhs < __rhs);
}

template <class _CharT, class _Traits, class _Allocator>
inline _LIBCPP_INLINE_VISIBILITY bool operator>=(const _CharT * __lhs,
                                                 const basic_string<_CharT, _Traits, _Allocator> & __rhs) _NOEXCEPT
{
    return !(__lhs < __rhs);
}

// operator +

template <class _CharT, class _Traits, class _Allocator>
basic_string<_CharT, _Traits, _Allocator> operator+(const basic_string<_CharT, _Traits, _Allocator> & __lhs,
                                                    const basic_string<_CharT, _Traits, _Allocator> & __rhs)
{
    basic_string<_CharT, _Traits, _Allocator> __r(__lhs.get_allocator());
    typename basic_string<_CharT, _Traits, _Allocator>::size_type __lhs_sz = __lhs.size();
    typename basic_string<_CharT, _Traits, _Allocator>::size_type __rhs_sz = __rhs.size();
    __r.__init(__lhs.data(), __lhs_sz, __lhs_sz + __rhs_sz);
    __r.append(__rhs.data(), __rhs_sz);
    return __r;
}

template <class _CharT, class _Traits, class _Allocator>
basic_string<_CharT, _Traits, _Allocator> operator+(const _CharT * __lhs, const basic_string<_CharT, _Traits, _Allocator> & __rhs)
{
    basic_string<_CharT, _Traits, _Allocator> __r(__rhs.get_allocator());
    typename basic_string<_CharT, _Traits, _Allocator>::size_type __lhs_sz = _Traits::length(__lhs);
    typename basic_string<_CharT, _Traits, _Allocator>::size_type __rhs_sz = __rhs.size();
    __r.__init(__lhs, __lhs_sz, __lhs_sz + __rhs_sz);
    __r.append(__rhs.data(), __rhs_sz);
    return __r;
}

template <class _CharT, class _Traits, class _Allocator>
basic_string<_CharT, _Traits, _Allocator> operator+(_CharT __lhs, const basic_string<_CharT, _Traits, _Allocator> & __rhs)
{
    basic_string<_CharT, _Traits, _Allocator> __r(__rhs.get_allocator());
    typename basic_string<_CharT, _Traits, _Allocator>::size_type __rhs_sz = __rhs.size();
    __r.__init(&__lhs, 1, 1 + __rhs_sz);
    __r.append(__rhs.data(), __rhs_sz);
    return __r;
}

template <class _CharT, class _Traits, class _Allocator>
inline basic_string<_CharT, _Traits, _Allocator> operator+(const basic_string<_CharT, _Traits, _Allocator> & __lhs,
                                                           const _CharT * __rhs)
{
    basic_string<_CharT, _Traits, _Allocator> __r(__lhs.get_allocator());
    typename basic_string<_CharT, _Traits, _Allocator>::size_type __lhs_sz = __lhs.size();
    typename basic_string<_CharT, _Traits, _Allocator>::size_type __rhs_sz = _Traits::length(__rhs);
    __r.__init(__lhs.data(), __lhs_sz, __lhs_sz + __rhs_sz);
    __r.append(__rhs, __rhs_sz);
    return __r;
}

template <class _CharT, class _Traits, class _Allocator>
basic_string<_CharT, _Traits, _Allocator> operator+(const basic_string<_CharT, _Traits, _Allocator> & __lhs, _CharT __rhs)
{
    basic_string<_CharT, _Traits, _Allocator> __r(__lhs.get_allocator());
    typename basic_string<_CharT, _Traits, _Allocator>::size_type __lhs_sz = __lhs.size();
    __r.__init(__lhs.data(), __lhs_sz, __lhs_sz + 1);
    __r.push_back(__rhs);
    return __r;
}

#ifndef _LIBCPP_CXX03_LANG

template <class _CharT, class _Traits, class _Allocator>
inline _LIBCPP_INLINE_VISIBILITY basic_string<_CharT, _Traits, _Allocator>
operator+(basic_string<_CharT, _Traits, _Allocator> && __lhs, const basic_string<_CharT, _Traits, _Allocator> & __rhs)
{
    return _VSTD::move(__lhs.append(__rhs));
}

template <class _CharT, class _Traits, class _Allocator>
inline _LIBCPP_INLINE_VISIBILITY basic_string<_CharT, _Traits, _Allocator>
operator+(const basic_string<_CharT, _Traits, _Allocator> & __lhs, basic_string<_CharT, _Traits, _Allocator> && __rhs)
{
    return _VSTD::move(__rhs.insert(0, __lhs));
}

template <class _CharT, class _Traits, class _Allocator>
inline _LIBCPP_INLINE_VISIBILITY basic_string<_CharT, _Traits, _Allocator>
operator+(basic_string<_CharT, _Traits, _Allocator> && __lhs, basic_string<_CharT, _Traits, _Allocator> && __rhs)
{
    return _VSTD::move(__lhs.append(__rhs));
}

template <class _CharT, class _Traits, class _Allocator>
inline _LIBCPP_INLINE_VISIBILITY basic_string<_CharT, _Traits, _Allocator>
operator+(const _CharT * __lhs, basic_string<_CharT, _Traits, _Allocator> && __rhs)
{
    return _VSTD::move(__rhs.insert(0, __lhs));
}

template <class _CharT, class _Traits, class _Allocator>
inline _LIBCPP_INLINE_VISIBILITY basic_string<_CharT, _Traits, _Allocator>
operator+(_CharT __lhs, basic_string<_CharT, _Traits, _Allocator> && __rhs)
{
    __rhs.insert(__rhs.begin(), __lhs);
    return _VSTD::move(__rhs);
}

template <class _CharT, class _Traits, class _Allocator>
inline _LIBCPP_INLINE_VISIBILITY basic_string<_CharT, _Traits, _Allocator>
operator+(basic_string<_CharT, _Traits, _Allocator> && __lhs, const _CharT * __rhs)
{
    return _VSTD::move(__lhs.append(__rhs));
}

template <class _CharT, class _Traits, class _Allocator>
inline _LIBCPP_INLINE_VISIBILITY basic_string<_CharT, _Traits, _Allocator>
operator+(basic_string<_CharT, _Traits, _Allocator> && __lhs, _CharT __rhs)
{
    __lhs.push_back(__rhs);
    return _VSTD::move(__lhs);
}

#endif // _LIBCPP_CXX03_LANG

// swap

template <class _CharT, class _Traits, class _Allocator>
inline _LIBCPP_INLINE_VISIBILITY void swap(basic_string<_CharT, _Traits, _Allocator> & __lhs,
                                           basic_string<_CharT, _Traits, _Allocator> & __rhs)
    _NOEXCEPT_(_NOEXCEPT_(__lhs.swap(__rhs)))
{
    __lhs.swap(__rhs);
}

#ifndef _LIBCPP_NO_HAS_CHAR8_T
typedef basic_string<char8_t> u8string;
#endif

#ifndef _LIBCPP_HAS_NO_UNICODE_CHARS
typedef basic_string<char16_t> u16string;
typedef basic_string<char32_t> u32string;
#endif // _LIBCPP_HAS_NO_UNICODE_CHARS

_LIBCPP_FUNC_VIS int stoi(const string & __str, size_t * __idx = 0, int __base = 10);
_LIBCPP_FUNC_VIS long stol(const string & __str, size_t * __idx = 0, int __base = 10);
_LIBCPP_FUNC_VIS unsigned long stoul(const string & __str, size_t * __idx = 0, int __base = 10);
_LIBCPP_FUNC_VIS long long stoll(const string & __str, size_t * __idx = 0, int __base = 10);
_LIBCPP_FUNC_VIS unsigned long long stoull(const string & __str, size_t * __idx = 0, int __base = 10);

_LIBCPP_FUNC_VIS float stof(const string & __str, size_t * __idx = 0);
_LIBCPP_FUNC_VIS double stod(const string & __str, size_t * __idx = 0);
_LIBCPP_FUNC_VIS long double stold(const string & __str, size_t * __idx = 0);

_LIBCPP_FUNC_VIS string to_string(int __val);
_LIBCPP_FUNC_VIS string to_string(unsigned __val);
_LIBCPP_FUNC_VIS string to_string(long __val);
_LIBCPP_FUNC_VIS string to_string(unsigned long __val);
_LIBCPP_FUNC_VIS string to_string(long long __val);
_LIBCPP_FUNC_VIS string to_string(unsigned long long __val);
_LIBCPP_FUNC_VIS string to_string(float __val);
_LIBCPP_FUNC_VIS string to_string(double __val);
_LIBCPP_FUNC_VIS string to_string(long double __val);

_LIBCPP_FUNC_VIS int stoi(const wstring & __str, size_t * __idx = 0, int __base = 10);
_LIBCPP_FUNC_VIS long stol(const wstring & __str, size_t * __idx = 0, int __base = 10);
_LIBCPP_FUNC_VIS unsigned long stoul(const wstring & __str, size_t * __idx = 0, int __base = 10);
_LIBCPP_FUNC_VIS long long stoll(const wstring & __str, size_t * __idx = 0, int __base = 10);
_LIBCPP_FUNC_VIS unsigned long long stoull(const wstring & __str, size_t * __idx = 0, int __base = 10);

_LIBCPP_FUNC_VIS float stof(const wstring & __str, size_t * __idx = 0);
_LIBCPP_FUNC_VIS double stod(const wstring & __str, size_t * __idx = 0);
_LIBCPP_FUNC_VIS long double stold(const wstring & __str, size_t * __idx = 0);

_LIBCPP_FUNC_VIS wstring to_wstring(int __val);
_LIBCPP_FUNC_VIS wstring to_wstring(unsigned __val);
_LIBCPP_FUNC_VIS wstring to_wstring(long __val);
_LIBCPP_FUNC_VIS wstring to_wstring(unsigned long __val);
_LIBCPP_FUNC_VIS wstring to_wstring(long long __val);
_LIBCPP_FUNC_VIS wstring to_wstring(unsigned long long __val);
_LIBCPP_FUNC_VIS wstring to_wstring(float __val);
_LIBCPP_FUNC_VIS wstring to_wstring(double __val);
_LIBCPP_FUNC_VIS wstring to_wstring(long double __val);

template <class _CharT, class _Traits, class _Allocator>
_LIBCPP_FUNC_VIS const typename basic_string<_CharT, _Traits, _Allocator>::size_type
    basic_string<_CharT, _Traits, _Allocator>::npos;

template <class _CharT, class _Allocator>
struct std hash<basic_string<_CharT, char_traits<_CharT>, _Allocator>>
    : public unary_function<basic_string<_CharT, char_traits<_CharT>, _Allocator>, size_t>
{
    size_t operator()(const basic_string<_CharT, char_traits<_CharT>, _Allocator> & __val) const _NOEXCEPT
    {
        return __do_string_hash(__val.data(), __val.data() + __val.size());
    }
};

template <class _CharT, class _Traits, class _Allocator>
basic_ostream<_CharT, _Traits> & operator<<(basic_ostream<_CharT, _Traits> & __os,
                                            const basic_string<_CharT, _Traits, _Allocator> & __str);

template <class _CharT, class _Traits, class _Allocator>
basic_istream<_CharT, _Traits> & operator>>(basic_istream<_CharT, _Traits> & __is,
                                            basic_string<_CharT, _Traits, _Allocator> & __str);

template <class _CharT, class _Traits, class _Allocator>
basic_istream<_CharT, _Traits> & getline(basic_istream<_CharT, _Traits> & __is, basic_string<_CharT, _Traits, _Allocator> & __str,
                                         _CharT __dlm);

template <class _CharT, class _Traits, class _Allocator>
inline _LIBCPP_INLINE_VISIBILITY basic_istream<_CharT, _Traits> & getline(basic_istream<_CharT, _Traits> & __is,
                                                                          basic_string<_CharT, _Traits, _Allocator> & __str);

#ifndef _LIBCPP_CXX03_LANG

template <class _CharT, class _Traits, class _Allocator>
inline _LIBCPP_INLINE_VISIBILITY basic_istream<_CharT, _Traits> &
getline(basic_istream<_CharT, _Traits> && __is, basic_string<_CharT, _Traits, _Allocator> & __str, _CharT __dlm);

template <class _CharT, class _Traits, class _Allocator>
inline _LIBCPP_INLINE_VISIBILITY basic_istream<_CharT, _Traits> & getline(basic_istream<_CharT, _Traits> && __is,
                                                                          basic_string<_CharT, _Traits, _Allocator> & __str);

#endif // _LIBCPP_CXX03_LANG

#if _LIBCPP_STD_VER > 17
template <class _CharT, class _Traits, class _Allocator, class _Up>
inline _LIBCPP_INLINE_VISIBILITY typename basic_string<_CharT, _Traits, _Allocator>::size_type
erase(basic_string<_CharT, _Traits, _Allocator> & __str, const _Up & __v)
{
    auto __old_size = __str.size();
    __str.erase(_VSTD::remove(__str.begin(), __str.end(), __v), __str.end());
    return __old_size - __str.size();
}

template <class _CharT, class _Traits, class _Allocator, class _Predicate>
inline _LIBCPP_INLINE_VISIBILITY typename basic_string<_CharT, _Traits, _Allocator>::size_type
erase_if(basic_string<_CharT, _Traits, _Allocator> & __str, _Predicate __pred)
{
    auto __old_size = __str.size();
    __str.erase(_VSTD::remove_if(__str.begin(), __str.end(), __pred), __str.end());
    return __old_size - __str.size();
}
#endif

#if _LIBCPP_DEBUG_LEVEL >= 2

template <class _CharT, class _Traits, class _Allocator>
bool basic_string<_CharT, _Traits, _Allocator>::__dereferenceable(const const_iterator * __i) const
{
    return this->data() <= _VSTD::__to_address(__i->base()) && _VSTD::__to_address(__i->base()) < this->data() + this->size();
}

template <class _CharT, class _Traits, class _Allocator>
bool basic_string<_CharT, _Traits, _Allocator>::__decrementable(const const_iterator * __i) const
{
    return this->data() < _VSTD::__to_address(__i->base()) && _VSTD::__to_address(__i->base()) <= this->data() + this->size();
}

template <class _CharT, class _Traits, class _Allocator>
bool basic_string<_CharT, _Traits, _Allocator>::__addable(const const_iterator * __i, ptrdiff_t __n) const
{
    const value_type * __p = _VSTD::__to_address(__i->base()) + __n;
    return this->data() <= __p && __p <= this->data() + this->size();
}

template <class _CharT, class _Traits, class _Allocator>
bool basic_string<_CharT, _Traits, _Allocator>::__subscriptable(const const_iterator * __i, ptrdiff_t __n) const
{
    const value_type * __p = _VSTD::__to_address(__i->base()) + __n;
    return this->data() <= __p && __p < this->data() + this->size();
}

#endif // _LIBCPP_DEBUG_LEVEL >= 2

#if _LIBCPP_STD_VER > 11
// Literal suffixes for basic_string [basic.string.literals]
inline namespace literals {
inline namespace string_literals {
inline _LIBCPP_INLINE_VISIBILITY basic_string<char> operator"" s(const char * __str, size_t __len)
{
    return basic_string<char>(__str, __len);
}

inline _LIBCPP_INLINE_VISIBILITY basic_string<wchar_t> operator"" s(const wchar_t * __str, size_t __len)
{
    return basic_string<wchar_t>(__str, __len);
}

#ifndef _LIBCPP_NO_HAS_CHAR8_T
inline _LIBCPP_INLINE_VISIBILITY basic_string<char8_t> operator"" s(const char8_t * __str, size_t __len) _NOEXCEPT
{
    return basic_string<char8_t>(__str, __len);
}
#endif

inline _LIBCPP_INLINE_VISIBILITY basic_string<char16_t> operator"" s(const char16_t * __str, size_t __len)
{
    return basic_string<char16_t>(__str, __len);
}

inline _LIBCPP_INLINE_VISIBILITY basic_string<char32_t> operator"" s(const char32_t * __str, size_t __len)
{
    return basic_string<char32_t>(__str, __len);
}
} // namespace string_literals
} // namespace literals
#endif

_LIBCPP_END_NAMESPACE_STD

_LIBCPP_POP_MACROS

#endif // _LIBCPP_STRING