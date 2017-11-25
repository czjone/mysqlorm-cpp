/* Copyright Hengfeng Lang. Inspiredd by libuv link: http://www.libuv.org
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), toi
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:

 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#ifndef __column_h__
#define __column_h__

#include <sstream>
#include <string>
using namespace std;

namespace MySqlOrm {
template<typename T, int LENGTH = 0>
class Column {
public:
    Column() {}
    Column(const T & value) : val(value) {}
    Column(const Column<T, LENGTH>& other) : val(other.val), name(other.name) {}
    ~Column() {}
    
    Column & operator =(const T & value) {
        val = value;
        return *this;
    }
    string operator +(const T & value) {
        return UptKey("+", value);
    }
    string operator -(const T & value) {
        return UptKey("-", value);
    }
    string Value(const string & ext = "") {
        return name + "=values(" + name +")" + ext;
    };
    template<typename Y>
    auto operator ==(Y && value) {
        return Operator(" = ", "?", value);
    }
    template<typename Y>
    auto operator !=(Y && value) {
        return Operator(" <> ", "?", value);
    }
    template<typename Y>
    auto operator >(Y && value) {
        return Operator(" > ", "?", value);
    }
    template<typename Y>
    auto operator >=(Y && value) {
        return Operator(" >= ", "?", value);
    }
    template<typename Y>
    auto operator <(Y && value) {
        return Operator(" < ", "?", value);
    }
    template<typename Y>
    auto operator <=(Y && value) {
        return Operator(" <= ", "?", value);
    }
    
    T           val;
    string      name;

private:    
    template<typename Y>
    inline typename enable_if<is_pointer<Y>::value, tuple<string, const char*>>::type
    Operator(const string & op, const string & r, Y value) const {
        return make_tuple(name + op + r, value);
    }    
    template<typename Y>
    inline typename enable_if<is_arithmetic<Y>::value, tuple<string, const Y*>>::type
    Operator(const string & op, const string & r, Y & value) const {
        return make_tuple(name + op + r, &value);
    }
    inline tuple<string, const char*> Operator(const string & op, const string & r, string & value) const {
        return make_tuple(name + op + r, value.c_str());
    }
    template<typename Y>
    inline typename enable_if<is_arithmetic<Y>::value, string>::type UptKey(const string & op, const Y & value) {
        ostringstream   os;
        os << name << "=" << name << op << value;
        return os.str();
    }
};
};

#endif
