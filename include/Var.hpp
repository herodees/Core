#pragma once

namespace box
{
    class var_members;
    class var_elements;

    enum var_error
    {
        ok,
        expecting_string,
        expecting_value,
        invalid_literal_name,
        invalid_number,
        invalid_string_char,
        invalid_string_escape,
        invalid_surrogate_pair,
        missing_colon,
        missing_comma_or_bracket,
        unexpected_character,
    };

    template <typename T>
    inline void MsgFree(T* p)
    {
        ::free(p);
    }
    template <typename T>
    inline T* MsgAlloc(size_t s)
    {
        T* p = (T*)::malloc(s * sizeof(T));
        return p;
    }
    template <typename T>
    inline T* MsgCreate()
    {
        T* p = new (MsgAlloc<T>(1)) T();
        return p;
    }
    template <typename T>
    inline void MsgDelete(T* p)
    {
        if (!p)
            return;
        p->~T();
        MsgFree(p);
    }

    template <typename T>
    struct Arr
    {
        uint32_t _ref      = 1;
        uint32_t _size     = 0;
        uint32_t _capacity = 0;
        T*       _data     = nullptr;
        ~Arr()
        {
            MsgFree(_data);
        }
        void reserve(uint32_t n)
        {
            if (n <= _capacity)
                return;
            T* tmp = MsgAlloc<T>(n);
            if (_data)
            {
                ::memcpy(tmp, _data, _size * sizeof(T));
                MsgFree(_data);
            }
            _data = tmp;
            ::memset(_data + _capacity, 0, sizeof(T) * (n - _capacity));
            _capacity = n;
        }
        void push_back(const T& x)
        {
            reserve((_size + 4) & ~3);
            new (&_data[_size++]) T(x);
        }
        void resize(uint32_t n)
        {
            reserve(n);
            _size = n;
        }
        T& back()
        {
            return _data[_size - 1];
        }
    };

    class var
    {
        struct Parser;
        enum class tag
        {
            Undefined,
            Null,
            Int32,
            Int64,
            Flt32,
            Flt64,
            Bool,
            Id,
            String,
            Array,
            Object
        };
        using ArrData = Arr<var>;

    public:
        using Member = std::pair<var, var>;

        ~var()
        {
            clear();
        }
        var() : _tag(tag::Undefined), _u64(0)
        {
        }
        var(var_error err) : _tag(tag::Undefined), _u64(err)
        {
        }
        var(var&& v) : _tag(v._tag), _u64(v._u64)
        {
            v._tag = tag::Undefined;
            v._u64 = 0;
        }
        var(const var& v) : _tag(v._tag), _u64(v._u64)
        {
            if (_tag > tag::Id)
                ++_arr->_ref;
        }
        var(int32_t v) : _tag(tag::Int32), _u32(v)
        {
        }
        var(int64_t v) : _tag(tag::Int64), _u64(v)
        {
        }
        var(uint32_t v) : _tag(tag::Int32), _u32(v)
        {
        }
        var(uint64_t v) : _tag(tag::Int64), _u64(v)
        {
        }
        var(nullptr_t) : _tag(tag::Null), _u64(0)
        {
        }
        var(float v) : _tag(tag::Flt32), _flt32(v)
        {
        }
        var(double v) : _tag(tag::Flt64), _flt64(v)
        {
        }
        var(bool v) : _tag(tag::Bool), _bool(v)
        {
        }
        var(const char* v) : var(std::string_view(v))
        {
        }
        var(std::string_view v) : var()
        {
            (v.size() < sizeof(_u64)) ? setid(v) : setstr(v);
        }

        bool is_undefined() const;
        bool is_error() const;
        bool is_null() const;
        bool is_object() const;
        bool is_array() const;
        bool is_int() const;
        bool is_int64() const;
        bool is_float() const;
        bool is_double() const;
        bool is_string() const;
        bool is_bool() const;

        bool             get(bool def) const;
        int32_t          get(int32_t def) const;
        uint32_t         get(uint32_t def) const;
        int64_t          get(int64_t def) const;
        uint64_t         get(uint64_t def) const;
        float            get(float def) const;
        double           get(double def) const;
        const char*      get(const char* def) const;
        std::string_view get(std::string_view def) const;

        std::string_view str() const;
        const char*      c_str() const;

        void push_back(const var& v);
        void push_back(std::string_view key, const var& v);

        var  get_item(uint32_t n);
        void set_item(uint32_t n, const var& v);

        var  get_item(std::string_view key);
        var  get_key(uint32_t n);
        void set_item(std::string_view key, const var& v);

        void make_array(uint32_t s);
        void make_object(uint32_t s);

        uint32_t size() const;
        void     clear();

        var  operator[](uint32_t n);
        var  operator[](std::string_view k);
        var& operator=(const var& c);

        var_members  members() const;
        var_elements elements() const;

        var_error from_string(const char* str);
        bool     to_string(std::string& str);

        var_error error() const;

    private:
        void setstr(std::string_view v);
        void setid(std::string_view v);
        void set(const var& v);

        tag _tag;
        union
        {
            int64_t  _u64;
            int32_t  _u32;
            int16_t  _u16;
            int8_t   _u8;
            double   _flt64;
            float    _flt32;
            bool     _bool;
            ArrData* _arr;
            char     _str[sizeof(double)];
        };
    };


    class var_members
    {
    public:
        constexpr var_members(var::Member* b, var::Member* e) : _begin(b), _end(e){};
        constexpr var::Member* begin() const
        {
            return _begin;
        }
        constexpr var::Member* end() const
        {
            return _end;
        }

    private:
        var::Member* _begin;
        var::Member* _end;
    };


    class var_elements
    {
    public:
        constexpr var_elements(var* b, var* e) : _begin(b), _end(e){};
        constexpr var* begin() const
        {
            return _begin;
        }
        constexpr var* end() const
        {
            return _end;
        }

    private:
        var* _begin;
        var* _end;
    };


    struct Stream
    {
        Stream(const char* s) : _s(s)
        {
        }
        const char* _s;
        const char* c_str() const
        {
            return _s;
        }
        int peek() const
        {
            return static_cast<unsigned char>(*_s);
        }
        int getch()
        {
            return static_cast<unsigned char>(*_s++);
        }
        int skipws()
        {
            while (peek() == '\x20' || peek() == '\x9' || peek() == '\xD' || peek() == '\xA')
                getch();
            return peek();
        }
    };


    struct var::Parser
    {
        Arr<char> _string;
        Arr<var>  _backlog;

        constexpr bool is_digit(int c) const
        {
            return c >= '0' && c <= '9';
        }
        int parse_hex(Stream& s);
        var parse_number(Stream& s);
        var parse_string(Stream& s, Arr<char>& v);
        var parse_value(Stream& s);
        var parse(const char* str);

        var make(bool arr, var* p, uint32_t s);
    };


    inline int var::Parser::parse_hex(Stream& s)
    {
        int cp = 0;
        for (int i = 0; i < 4; ++i)
        {
            if (is_digit(s.peek()))
                cp = (cp * 16) + (s.getch() - '0');
            else if ((s.peek() | 0x20) >= 'a' && (s.peek() | 0x20) <= 'f')
                cp = (cp * 16) + ((s.getch() | 0x20) - 'a' + 10);
            else
                return -1;
        }
        return cp;
    }

    inline var var::Parser::parse_number(Stream& s)
    {
        bool               intOnly     = true;
        unsigned long long integer     = 0;
        double             significand = 0;
        int                fraction    = 0;
        int                exponent    = 0;

        integer = s.getch() - '0';
        if (integer)
            while (is_digit(s.peek()) && integer < 0x1999999999999999ull)
                integer = (integer * 10) + (s.getch() - '0');

        if (integer >= 0x1999999999999999ull)
        {
            significand = static_cast<double>(integer);
            while (is_digit(s.peek()))
                significand = (significand * 10) + (s.getch() - '0');
        }

        if (s.peek() == '.')
        {
            intOnly = false;
            s.getch();
            while (is_digit(s.peek()) && integer < 0x1FFFFFFFFFFFFFull)
            {
                integer = (integer * 10) + (s.getch() - '0');
                --fraction;
            }

            while (is_digit(s.peek()))
                s.getch();
        }

        if (integer < 0x1999999999999999ull)
            significand = static_cast<double>(integer);

        if ((s.peek() | 0x20) == 'e')
        {
            intOnly = false;
            s.getch();
            if (s.peek() == '-')
            {
                s.getch();
                if (!is_digit(s.peek()))
                    return var();
                while (is_digit(s.peek()))
                    exponent = (exponent * 10) - (s.getch() - '0');
            }
            else
            {
                if (s.peek() == '+')
                    s.getch();
                if (!is_digit(s.peek()))
                    return var();
                while (is_digit(s.peek()))
                    exponent = (exponent * 10) + (s.getch() - '0');
            }
        }

        static constexpr double exp10[] =
            {1e0,   1e1,   1e2,   1e3,   1e4,   1e5,   1e6,   1e7,   1e8,   1e9,   1e10,  1e11,  1e12,  1e13,  1e14,
             1e15,  1e16,  1e17,  1e18,  1e19,  1e20,  1e21,  1e22,  1e23,  1e24,  1e25,  1e26,  1e27,  1e28,  1e29,
             1e30,  1e31,  1e32,  1e33,  1e34,  1e35,  1e36,  1e37,  1e38,  1e39,  1e40,  1e41,  1e42,  1e43,  1e44,
             1e45,  1e46,  1e47,  1e48,  1e49,  1e50,  1e51,  1e52,  1e53,  1e54,  1e55,  1e56,  1e57,  1e58,  1e59,
             1e60,  1e61,  1e62,  1e63,  1e64,  1e65,  1e66,  1e67,  1e68,  1e69,  1e70,  1e71,  1e72,  1e73,  1e74,
             1e75,  1e76,  1e77,  1e78,  1e79,  1e80,  1e81,  1e82,  1e83,  1e84,  1e85,  1e86,  1e87,  1e88,  1e89,
             1e90,  1e91,  1e92,  1e93,  1e94,  1e95,  1e96,  1e97,  1e98,  1e99,  1e100, 1e101, 1e102, 1e103, 1e104,
             1e105, 1e106, 1e107, 1e108, 1e109, 1e110, 1e111, 1e112, 1e113, 1e114, 1e115, 1e116, 1e117, 1e118, 1e119,
             1e120, 1e121, 1e122, 1e123, 1e124, 1e125, 1e126, 1e127, 1e128, 1e129, 1e130, 1e131, 1e132, 1e133, 1e134,
             1e135, 1e136, 1e137, 1e138, 1e139, 1e140, 1e141, 1e142, 1e143, 1e144, 1e145, 1e146, 1e147, 1e148, 1e149,
             1e150, 1e151, 1e152, 1e153, 1e154, 1e155, 1e156, 1e157, 1e158, 1e159, 1e160, 1e161, 1e162, 1e163, 1e164,
             1e165, 1e166, 1e167, 1e168, 1e169, 1e170, 1e171, 1e172, 1e173, 1e174, 1e175, 1e176, 1e177, 1e178, 1e179,
             1e180, 1e181, 1e182, 1e183, 1e184, 1e185, 1e186, 1e187, 1e188, 1e189, 1e190, 1e191, 1e192, 1e193, 1e194,
             1e195, 1e196, 1e197, 1e198, 1e199, 1e200, 1e201, 1e202, 1e203, 1e204, 1e205, 1e206, 1e207, 1e208, 1e209,
             1e210, 1e211, 1e212, 1e213, 1e214, 1e215, 1e216, 1e217, 1e218, 1e219, 1e220, 1e221, 1e222, 1e223, 1e224,
             1e225, 1e226, 1e227, 1e228, 1e229, 1e230, 1e231, 1e232, 1e233, 1e234, 1e235, 1e236, 1e237, 1e238, 1e239,
             1e240, 1e241, 1e242, 1e243, 1e244, 1e245, 1e246, 1e247, 1e248, 1e249, 1e250, 1e251, 1e252, 1e253, 1e254,
             1e255, 1e256, 1e257, 1e258, 1e259, 1e260, 1e261, 1e262, 1e263, 1e264, 1e265, 1e266, 1e267, 1e268, 1e269,
             1e270, 1e271, 1e272, 1e273, 1e274, 1e275, 1e276, 1e277, 1e278, 1e279, 1e280, 1e281, 1e282, 1e283, 1e284,
             1e285, 1e286, 1e287, 1e288, 1e289, 1e290, 1e291, 1e292, 1e293, 1e294, 1e295, 1e296, 1e297, 1e298, 1e299,
             1e300, 1e301, 1e302, 1e303, 1e304, 1e305, 1e306, 1e307, 1e308};
        exponent += fraction;
        if (exponent < 0)
        {
            if (exponent < -308)
            {
                significand /= exp10[308];
                exponent += 308;
            }
            significand = exponent < -308 ? 0.0 : significand / exp10[-exponent];
        }
        else
        {
            significand *= exp10[exponent < 308 ? exponent : 308];
        }

        if (intOnly)
            return integer;
        return significand;
    }

    inline var var::Parser::parse_string(Stream& s, Arr<char>& v)
    {
        v.resize(0);

        for (uint32_t length = 0, offset = v._size;;)
        {
            v.resize(v._size + 32);

            char* first = v._data + offset + length;
            char* last  = v._data + v._size;

            while (first < last)
            {
                int ch = s.getch();

                if (ch < ' ')
                    return var(var_error::invalid_string_char);

                if (ch == '"')
                {
                    length = uint32_t(first - (v._data + offset));
                    return std::string_view(v._data, length);
                }

                if (ch == '\\')
                {
                    switch (s.getch())
                    {
                            // clang-format off
						case '\x22': ch = '"'; break;
						case '\x2F': ch = '/'; break;
						case '\x5C': ch = '\\'; break;
						case '\x62': ch = '\b'; break;
						case '\x66': ch = '\f'; break;
						case '\x6E': ch = '\n'; break;
						case '\x72': ch = '\r'; break;
						case '\x74': ch = '\t'; break;
                            // clang-format on
                        case '\x75':
                            if ((ch = parse_hex(s)) < 0)
                                return var(var_error::invalid_string_escape);
                            if (ch >= 0xD800 && ch <= 0xDBFF)
                            {
                                if (s.getch() != '\\' || s.getch() != '\x75')
                                    return var(var_error::invalid_surrogate_pair);
                                int low = parse_hex(s);
                                if (low < 0xDC00 || low > 0xDFFF)
                                    return var(var_error::invalid_surrogate_pair);
                                ch = 0x10000 + ((ch & 0x3FF) << 10) + (low & 0x3FF);
                            }

                            if (ch < 0x80)
                            {
                                *first++ = (char)ch;
                            }
                            else if (ch < 0x800)
                            {
                                *first++ = 0xC0 | ((char)(ch >> 6));
                                *first++ = 0x80 | (ch & 0x3F);
                            }
                            else if (ch < 0x10000)
                            {
                                *first++ = 0xE0 | ((char)(ch >> 12));
                                *first++ = 0x80 | ((ch >> 6) & 0x3F);
                                *first++ = 0x80 | (ch & 0x3F);
                            }
                            else
                            {
                                *first++ = 0xF0 | ((char)(ch >> 18));
                                *first++ = 0x80 | ((ch >> 12) & 0x3F);
                                *first++ = 0x80 | ((ch >> 6) & 0x3F);
                                *first++ = 0x80 | (ch & 0x3F);
                            }
                            continue;
                        default:
                            return var(var_error::invalid_string_escape);
                    }
                }

                *first++ = (char)ch;
            }
            length = uint32_t(first - (v._data + offset));
        }
    }

    inline var var::Parser::parse_value(Stream& s)
    {
        switch (s.skipws())
        {
            case '"':
                s.getch();
                return parse_string(s, _string);
            case 'f':
                s.getch();
                if (s.getch() == 'a' && s.getch() == 'l' && s.getch() == 's' && s.getch() == 'e')
                    return false;
                return var_error::invalid_literal_name;
            case 't':
                s.getch();
                if (s.getch() == 'r' && s.getch() == 'u' && s.getch() == 'e')
                    return true;
                return var_error::invalid_literal_name;
            case 'n':
                s.getch();
                if (s.getch() == 'u' && s.getch() == 'l' && s.getch() == 'l')
                    return nullptr;
                return var_error::invalid_literal_name;
            case '[':
            {
                s.getch();
                uint32_t frame = _backlog._size;
                if (s.skipws() != ']')
                {
                element:
                    _backlog.push_back(parse_value(s));
                    if (_backlog.back().is_error())
                        return _backlog.back();

                    if (s.skipws() == ',')
                    {
                        s.getch();
                        goto element;
                    }
                }

                if (s.getch() != ']')
                    return var_error::missing_comma_or_bracket;

                uint32_t size = uint32_t(_backlog._size - frame);
                if (!size)
                    return make(true, nullptr, 0);

                var a = make(true, _backlog._data + frame, size);
                _backlog.resize(frame);
                return a;
            }
            case '{':
            {
                s.getch();
                uint32_t frame = _backlog._size;
                if (s.skipws() != '}')
                {
                member:
                    if (s.peek() != '"')
                        return var_error::expecting_string;
                    s.getch();
                    _backlog.push_back(parse_string(s, _string));
                    if (_backlog.back().is_error())
                        return _backlog.back();

                    if (s.skipws() != ':')
                        return var_error::missing_colon;
                    s.getch();
                    _backlog.push_back(parse_value(s));
                    if (_backlog.back().is_error())
                        return _backlog.back();

                    if (s.skipws() == ',')
                    {
                        s.getch();
                        s.skipws();
                        goto member;
                    }
                }

                if (s.getch() != '}')
                    return var_error::missing_comma_or_bracket;

                uint32_t size = _backlog._size - frame;
                if (!size)
                    return make(false, nullptr, 0);

                var a = make(false, _backlog._data + frame, size);
                _backlog.resize(frame);

                return a;
            }
            case '-':
                s.getch();
                if (is_digit(s.peek()))
                {
                    var n = parse_number(s);
                    if (n.is_double())
                        return -n.get(0.0);
                    if (n.is_int64())
                        return -n.get(0ll);
                    return n;
                }
                break;
            default:
                if (is_digit(s.peek()))
                    return parse_number(s);
                break;
        }
        return var(var_error::expecting_value);
    }

    inline var var::Parser::parse(const char* str)
    {
        Stream s(str);
        var    v = parse_value(s);
        if (v.is_error() && s.skipws())
            v = var_error::unexpected_character;
        for (uint32_t n = 0; n < _backlog._size; ++n)
            _backlog._data[n].clear();
        return v;
    }

    inline var var::Parser::make(bool arr, var* p, uint32_t s)
    {
        var v;
        if (arr)
            v.make_array(s);
        else
            v.make_object(s / 2);
        v._arr->_size = s;
        ::memcpy(v._arr->_data, p, s * sizeof(var));
        return v;
    }

    inline void var::clear()
    {
        if (_tag > tag::String)
        {
            if (--_arr->_ref == 0)
            {
                auto* eit = _arr->_data + _arr->_size;
                for (auto* it = _arr->_data; it != eit; ++it)
                    it->clear();
                MsgDelete(_arr);
            }
        }
        else if (_tag == tag::String)
        {
            if (--_arr->_ref == 0)
                MsgDelete(_arr);
        }
        _tag = tag::Undefined;
        _u64 = 0;
    }

    inline var_error var::error() const
    {
        if (_tag == tag::Undefined)
            return var_error(_u64);
        return var_error::ok;
    }

    inline std::string_view var::str() const
    {
        if (_tag == tag::String)
            return std::string_view((char*)_arr->_data, _arr->_size);
        if (_tag == tag::Id)
            return std::string_view(_str);
        return std::string_view();
    }

    inline const char* var::c_str() const
    {
        if (_tag == tag::String)
            return (char*)_arr->_data;
        if (_tag == tag::Id)
            return _str;
        return "";
    }

    inline void var::setstr(std::string_view v)
    {
        _tag             = tag::String;
        _arr             = MsgCreate<ArrData>();
        _arr->_size      = (uint32_t)v.size();
        const uint32_t s = _arr->_size + 1;
        _arr->reserve(s / sizeof(var) + ((s % sizeof(var)) != 0));
        memcpy(_arr->_data, v.data(), v.size());
    }

    inline void var::setid(std::string_view v)
    {
        _tag = tag::Id;
        memcpy(_str, v.data(), v.size());
    }

    inline void var::push_back(const var& v)
    {
        if (_tag != tag::Array)
        {
            clear();
            _tag = tag::Array;
            _arr = MsgCreate<ArrData>();
        }
        _arr->reserve((_arr->_size + 4) & ~3);
        new (&_arr->_data[_arr->_size]) var(v);
        ++_arr->_size;
    }

    inline void var::set_item(std::string_view key, const var& v)
    {
        if (_tag != tag::Object)
        {
            clear();
            _tag = tag::Object;
            _arr = MsgCreate<ArrData>();
        }
        for (Member& it : members())
        {
            if (key == it.first.c_str())
            {
                it.second = v;
                return;
            }
        }
        _arr->reserve((_arr->_size + 5) & ~3);
        new (&_arr->_data[_arr->_size]) var(key);
        ++_arr->_size;
        new (&_arr->_data[_arr->_size]) var(v);
        ++_arr->_size;
    }

    inline var var::get_item(std::string_view key)
    {
        if (_tag != tag::Object)
            return var();
        auto* eit = _arr->_data + _arr->_size;
        for (auto* it = _arr->_data; it != eit; it += 2)
        {
            if (key == it->c_str())
                return it[1];
        }
        return var();
    }

    inline void var::push_back(std::string_view key, const var& v)
    {
        if (_tag != tag::Object)
        {
            clear();
            _tag = tag::Object;
            _arr = MsgCreate<ArrData>();
        }
        _arr->reserve((_arr->_size + 5) & ~3);
        new (&_arr->_data[_arr->_size]) var(key);
        ++_arr->_size;
        new (&_arr->_data[_arr->_size]) var(v);
        ++_arr->_size;
    }

    inline var var::get_key(uint32_t n)
    {
        if (_tag == tag::Object)
            return _arr->_data[n * 2];
        return var();
    }

    inline void var::make_array(uint32_t s)
    {
        if (_tag != tag::Array)
        {
            clear();
            _tag = tag::Array;
            _arr = MsgCreate<ArrData>();
        }
        _arr->reserve(s);
    }

    inline void var::make_object(uint32_t s)
    {
        if (_tag != tag::Object)
        {
            clear();
            _tag = tag::Object;
            _arr = MsgCreate<ArrData>();
        }
        _arr->reserve(s * 2);
    }

    inline var var::operator[](uint32_t n)
    {
        return get_item(n);
    }

    inline var var::operator[](std::string_view k)
    {
        return get_item(k);
    }

    inline void var::set_item(uint32_t n, const var& v)
    {
        if (_tag != tag::Array)
        {
            clear();
            _tag = tag::Array;
            _arr = MsgCreate<ArrData>();
        }
        if (_arr->_size <= n)
        {
            auto s = (n + 4) & ~3;
            _arr->reserve(s);
            _arr->_size = n + 1;
        }
        else
            _arr->_data[n].clear();

        new (&_arr->_data[n]) var(v);
    }

    inline var var::get_item(uint32_t n)
    {
        if (_tag == tag::Array)
            return _arr->_data[n];
        if (_tag == tag::Object)
            return _arr->_data[n * 2 + 1];
        return var();
    }

    inline var& var::operator=(const var& c)
    {
        if (&c == this)
            return *this;
        clear();
        _tag = c._tag;
        _u64 = c._u64;
        if (c._tag > tag::Id)
            ++_arr->_ref;
        return *this;
    }

    inline var_members var::members() const
    {
        if (_tag == tag::Object)
            return var_members((Member*)_arr->_data, (Member*)(_arr->_data + _arr->_size));
        return var_members(nullptr, nullptr);
    }

    inline var_elements var::elements() const
    {
        if (_tag == tag::Array)
            return var_elements(_arr->_data, _arr->_data + _arr->_size);
        return var_elements(nullptr, nullptr);
    }

    inline var_error var::from_string(const char* str)
    {
        Parser p;
        *this = p.parse(str);
        return error();
    }

    inline bool var::to_string(std::string& s)
    {
        bool                 ret = true;
        char                 buf[32];
        std::to_chars_result res;

        switch (_tag)
        {
            case tag::Undefined:
            case tag::Null:
                s.append("null", 4);
                break;
            case tag::Int32:
                res = std::to_chars(buf, buf + std::size(buf), _u32);
                s.append(buf, res.ptr - buf);
                break;
            case tag::Int64:
                res = std::to_chars(buf, buf + std::size(buf), _u64);
                s.append(buf, res.ptr - buf);
                break;
            case tag::Flt32:
                res = std::to_chars(buf, buf + std::size(buf), _flt32);
                s.append(buf, res.ptr - buf);
                break;
            case tag::Flt64:
                res = std::to_chars(buf, buf + std::size(buf), _flt64);
                s.append(buf, res.ptr - buf);
                break;
            case tag::Bool:
                _bool ? s.append("true", 4) : s.append("false", 5);
                break;
            case tag::Id:
            case tag::String:
            {
                s.push_back('\"');
                std::string_view val(str());
                const char*      e = val.data() + val.size();
                for (const char* p = val.data(); p < e;)
                {
                    char c = *p++;
                    switch (c)
                    {
                        case '\b':
                            s.append("\\b", 2);
                            break;
                        case '\f':
                            s.append("\\f", 2);
                            break;
                        case '\n':
                            s.append("\\n", 2);
                            break;
                        case '\r':
                            s.append("\\r", 2);
                            break;
                        case '\t':
                            s.append("\\t", 2);
                            break;
                        case '\\':
                            s.append("\\\\", 2);
                            break;
                        case '"':
                            s.append("\\\"", 2);
                            break;
                        default:
                            s.push_back(c);
                    }
                }
                s.push_back('\"');
            }
            break;

            case tag::Array:
            {
                if (_arr->_size)
                {
                    char comma = '[';
                    for (auto& it : elements())
                    {
                        s.push_back(comma);
                        ret &= it.to_string(s);
                        comma = ',';
                    }
                    s.push_back(']');
                }
                else
                    s.append("[]", 2);
            }
            break;

            case tag::Object:
            {
                if (_arr->_size)
                {
                    char comma = '{';
                    for (auto& it : members())
                    {
                        s.push_back(comma);
                        ret &= it.first.to_string(s);
                        s.push_back(':');
                        ret &= it.second.to_string(s);
                        comma = ',';
                    }
                    s.push_back('}');
                }
                else
                    s.append("{}", 2);
            }
            break;

            default:
                return false;
        }

        return ret;
    }

    inline uint32_t var::size() const
    {
        if (_tag == tag::Array)
            return _arr->_size;
        if (_tag == tag::Object)
            return _arr->_size / 2;
        return 0;
    }

    inline bool var::is_undefined() const
    {
        return _tag == tag::Undefined;
    }

    inline bool var::is_error() const
    {
        return _tag == tag::Undefined && _u64;
    }

    inline bool var::is_null() const
    {
        return _tag == tag::Null;
    }

    inline bool var::is_object() const
    {
        return _tag == tag::Object;
    }

    inline bool var::is_array() const
    {
        return _tag == tag::Array;
    }

    inline bool var::is_int() const
    {
        return _tag == tag::Int32;
    }

    inline bool var::is_int64() const
    {
        return _tag == tag::Int64;
    }

    inline bool var::is_float() const
    {
        return _tag == tag::Flt32;
    }

    inline bool var::is_double() const
    {
        return _tag == tag::Flt64;
    }

    inline bool var::is_string() const
    {
        return _tag == tag::Id || _tag == tag::String;
    }

    inline bool var::is_bool() const
    {
        return _tag == tag::Bool;
    }

    inline bool var::get(bool def) const
    {
        if (_tag == tag::Bool)
            return _bool;
        return def;
    }

    inline int32_t var::get(int32_t def) const
    {
        if (_tag == tag::Int32)
            return _u32;
        if (_tag == tag::Int64)
            return int32_t(_u64);
        return def;
    }

    inline uint32_t var::get(uint32_t def) const
    {
        if (_tag == tag::Int32)
            return _u32;
        if (_tag == tag::Int64)
            return uint32_t(_u64);
        return def;
    }

    inline int64_t var::get(int64_t def) const
    {
        if (_tag == tag::Int64)
            return _u64;
        if (_tag == tag::Int32)
            return _u32;
        return def;
    }

    inline uint64_t var::get(uint64_t def) const
    {
        if (_tag == tag::Int64)
            return _u64;
        if (_tag == tag::Int32)
            return _u32;
        return def;
    }

    inline float var::get(float def) const
    {
        if (_tag == tag::Flt32)
            return _flt32;
        if (_tag == tag::Flt64)
            return float(_flt64);
        return def;
    }

    inline double var::get(double def) const
    {
        if (_tag == tag::Flt64)
            return _flt64;
        if (_tag == tag::Flt32)
            return _flt32;
        return def;
    }

    inline const char* var::get(const char* def) const
    {
        if (_tag == tag::String)
            return (char*)_arr->_data;
        if (_tag == tag::Id)
            return _str;
        return def;
    }

    inline std::string_view var::get(std::string_view def) const
    {
        if (_tag == tag::String)
            return std::string_view((char*)_arr->_data, _arr->_size);
        if (_tag == tag::Id)
            return _str;
        return def;
    }

} // namespace box