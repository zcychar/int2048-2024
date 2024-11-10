#pragma once
#ifndef SJTU_BIGINTEGER
#define SJTU_BIGINTEGER

// Integer 1:
// 实现一个有符号的大整数类，只需支持简单的加减

// Integer 2:
// 实现一个有符号的大整数类，支持加减乘除，并重载相关运算符

// 请不要使用除了以下头文件之外的其它头文件
#include <complex>
#include <cstdio>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <vector>

// 请不要使用 using namespace std;

namespace sjtu {
  class int2048 {
    std::vector<long long> digit;
    bool symbol = false; //true=negative
    int length = 0;

    static constexpr long long p = 1e8;

  public:
    // 构造函数
    int2048();

    int2048(long long);

    int2048(const std::string &);

    int2048(const int2048 &);


    // 以下给定函数的形式参数类型仅供参考，可自行选择使用常量引用或者不使用引用
    // 如果需要，可以自行增加其他所需的函数
    // ===================================
    // Integer1
    // ===================================

    // 读入一个大整数
    void read(const std::string &);

    // 输出储存的大整数，无需换行
    void print();

    // 加上一个大整数
    int2048 &add(const int2048 &);

    // 返回两个大整数之和
    friend int2048 add(int2048, const int2048 &);

    // 减去一个大整数
    int2048 &minus(const int2048 &);

    // 返回两个大整数之差
    friend int2048 minus(int2048, const int2048 &);

    friend bool isAbsLess(const int2048 &, const int2048 &);

    friend bool isAbsLeq(const int2048 &, const int2048 &);
    // ===================================
    // Integer2
    // ===================================

    int2048 operator+() const;

    int2048 operator-() const;

    int2048 &operator=(const int2048 &);

    int2048 &operator+=(const int2048 &);

    friend int2048 operator+(int2048, const int2048 &);

    int2048 &operator-=(const int2048 &);

    friend int2048 operator-(const int2048&, const int2048 &);

    int2048 &operator*=(const int2048 &);

    friend int2048 operator*(int2048, const int2048 &);

    int2048 &operator/=(const int2048 &);

    friend int2048 operator/(int2048, const int2048 &);

    int2048 &operator%=(const int2048 &);

    friend int2048 operator%(int2048, const int2048 &);

    friend std::istream &operator>>(std::istream &, int2048 &);

    friend std::ostream &operator<<(std::ostream &, const int2048 &);

    friend bool operator==(const int2048 &, const int2048 &);

    friend bool operator!=(const int2048 &, const int2048 &);

    friend bool operator<(const int2048 &, const int2048 &);

    friend bool operator>(const int2048 &, const int2048 &);

    friend bool operator<=(const int2048 &, const int2048 &);

    friend bool operator>=(const int2048 &, const int2048 &);
  };

  inline int2048::int2048() {
    length = 1;
    digit.push_back(0);
  }

  inline int2048::int2048(long long num) {
    if (num < 0) {
      symbol = true;
      num = -num;
    }
    while (num) {
      digit.push_back(num % p);
      num /= p;
      length++;
    }
    if (length == 0) {
      length++;
      digit.push_back(0);
    }
  }

  inline int2048::int2048(const std::string &str) {
    read(str);
  }

  inline int2048::int2048(const int2048 &str) = default;

  inline void int2048::read(const std::string &str) {
    digit.clear();
    symbol = false;
    if (str[0] == '-') {
      symbol = true;
      length = (str.length() + 6) / 8;
      if (str[1] == '0') {
        digit.push_back(0);
        symbol = false;
        length = 1;
        return;
      }
    } else {
      length = (str.length() + 7) / 8;
    }
    digit.resize(length);
    int pointer = str.length() - 1, cnt = 0;
    for (int i = 0; i < length - 1; ++i) {
      for (int j = 7; j >= 0; --j) {
        digit[i] = digit[i] * 10 + (str[pointer - j] - '0');
      }
      pointer -= 8;
    }
    for (int i = (symbol ? 1 : 0); i <= pointer; ++i) {
      digit[length - 1] = digit[length - 1] * 10 + (str[i] - '0');
    }
  }

  inline void int2048::print() {
    if (symbol) {
      std::cout << '-';
    }
    printf("%lld", digit[length - 1]);
    for (int i = length - 2; i >= 0; --i) {
      printf("%.8lld", digit[i]);
    }
  }


  inline int2048 &int2048::add(const int2048 &other) {
    if (symbol == other.symbol) {
      int res = 0;
      length = std::max(length, other.length);
      digit.resize(length);
      for (int i = 0; i < other.length; ++i) {
        digit[i] = digit[i] + other.digit[i] + res;
        res = digit[i] / p;
        digit[i] %= p;
      }
      for (int i = other.length; i < length; ++i) {
        digit[i] = digit[i] + res;
        res = digit[i] / p;
        digit[i] %= p;
      }
      if (res) {
        digit.push_back(res);
        length++;
      }
    } else {
      bool status = isAbsLess(*this, other); //true:other > this

      if (status) {
        //symbol=true:negative+positive positive result
        //symbol=false:positive+negative negative result
        int res = 0;
        digit.resize(other.length);
        length = other.length;
        symbol ^= 1;
        for (int i = 0; i < length; ++i) {
          digit[i] = (other.digit[i] - digit[i] - res);
          res = 0;
          if (digit[i] < 0) {
            res = 1;
            digit[i] += p;
          }
        }
        while (digit[length - 1] == 0 && length >= 1) {
          length--;
        }
      } else {
        //symbol==true:negative+positive negative result
        //symbol==false:positive+negative positive result
        int res = 0;
        for (int i = 0; i < other.length; ++i) {
          digit[i] = (digit[i] - other.digit[i] - res);
          res = 0;
          if (digit[i] < 0) {
            res = 1;
            digit[i] += p;
          }
        }
        for (int i = other.length; i < length; ++i) {
          digit[i] = digit[i] - res;
          res = 0;
          if (digit[i] < 0) {
            res = 1;
            digit[i] += p;
          }
        }
        while (digit[length - 1] == 0 && length >= 1) {
          length--;
        }
        ///@note:this function MIGHT produce zero as result.
        if (length == 0) {
          length = 1;
          symbol = false;
        }
      }
    }
    return *this;
  }

  inline int2048 add(int2048 first, const int2048 &second) {
    return first.add(second);
  }

  inline int2048 &int2048::minus(const int2048 &other) {
    if (symbol != other.symbol) {
      //negative-positive=negative+negative: negative
      //positive-negative=positive+positive: positive
      long long res = 0;
      length = std::max(length, other.length);
      digit.resize(length);
      for (int i = 0; i < other.length; ++i) {
        digit[i] = digit[i] + other.digit[i] + res;
        res = digit[i] / p;
        digit[i] %= p;
      }
      for (int i = other.length; i < length; ++i) {
        digit[i] = digit[i] + res;
        res = digit[i] / p;
        digit[i] %= p;
      }
      if (res) {
        digit.push_back(res);
        length++;
      }
    } else {
      bool status = isAbsLess(*this, other); //true:this < other
      if (status) {
        //symbol=true:negative-negative positive result
        //symbol=false:positive-positive negative result
        long long res = 0;
        digit.resize(other.length);
        length = other.length;
        symbol = symbol ^ 1;
        for (int i = 0; i < length; ++i) {
          digit[i] = (other.digit[i] - digit[i] - res);
          res = 0;
          if (digit[i] < 0) {
            res = 1;
            digit[i] += p;
          }
        }
        while (digit[length - 1] == 0 && length >= 1) {
          length--;
        }
      } else {
        //symbol=true:negative-negative negative result
        //symbol=false:positive-positive positive result
        int res = 0;
        for (int i = 0; i < other.length; ++i) {
          digit[i] = (digit[i] - other.digit[i] - res);
          res = 0;
          if (digit[i] < 0) {
            res = 1;
            digit[i] += p;
          }
        }
        for (int i = other.length; i < length; ++i) {
          digit[i] = digit[i] - res;
          res = 0;
          if (digit[i] < 0) {
            res = 1;
            digit[i] += p;
          }
        }
        while (digit[length - 1] == 0 && length >= 1) {
          length--;
        }
        ///@note:this function MIGHT produce zero as result.
        if (length == 0) {
          length = 1;
          symbol = false;
        }
      }
    }
    return *this;
  }

  inline int2048 minus(int2048 first, const int2048 &second) {
    return first.minus(second);
  }

  inline bool isAbsLess(const int2048 &first, const int2048 &second) {
    if (first.length < second.length) {
      return true;
    }
    if (first.length > second.length) {
      return false;
    }
    for (int i = first.length - 1; i >= 0; --i) {
      if (first.digit[i] < second.digit[i]) {
        return true;
      }
      if (first.digit[i] > second.digit[i]) {
        return false;
      }
    }
    return false;
  }

  inline bool isAbsLeq(const int2048 &first, const int2048 &second) {
    if (first.length < second.length) {
      return true;
    }
    if (first.length > second.length) {
      return false;
    }
    for (int i = first.length - 1; i >= 0; --i) {
      if (first.digit[i] < second.digit[i]) {
        return true;
      }
      if (first.digit[i] > second.digit[i]) {
        return false;
      }
    }
    return true;
  }

  inline int2048 int2048::operator+() const {
    return *this;
  }

  inline int2048 int2048::operator-() const {
    int2048 result=*this;
    result.symbol^=1;
    return result;
  }

  inline std::istream &operator>>(std::istream &in, int2048 &number) {
    std::string s;
    in>>s;
    number.read(s);
    return in;
  }

  inline std::ostream &operator<<(std::ostream &out, const int2048 &number) {
    if (number.symbol) {
      out<< '-';
    }
    printf("%lld",number. digit[number.length - 1]);
    for (int i = number.length - 2; i >= 0; --i) {
      out<<std::setw(8)<<std::setfill('0')<<number.digit[i];
    }
    return out;
  }


  inline int2048 &int2048::operator=(const int2048 &str) = default;

  inline int2048 operator+(int2048 lhs, const int2048 &rhs) {
    return add(lhs,rhs);
  }

  inline int2048 operator-(const int2048& lhs,const int2048 &rhs) {
    return minus(lhs, rhs);
  }

  inline int2048 &int2048::operator+=(const int2048 &rhs) {
    return add(rhs);
  }

  inline int2048 &int2048::operator-=(const int2048 &rhs) {
    return minus(rhs);
  }

  inline bool operator==(const int2048 &lhs, const int2048 &rhs) {
    if(lhs.symbol!=rhs.symbol) {
      return false;
    }
    if(lhs.length!=rhs.length) {
      return false;
    }
    if(lhs.digit!=rhs.digit) {
      return false;
    }
    return true;
  }

  inline bool operator!=(const int2048 &lhs, const int2048 &rhs) {
    return operator==(lhs,rhs)^1;
  }

  inline bool operator<(const int2048 &lhs, const int2048 &rhs) {
    if(lhs.symbol!=rhs.symbol) {//symbol=true: negative
      return lhs.symbol;
    }
    if(lhs.symbol) {
      return isAbsLeq(lhs,rhs)^1;
    }
    return isAbsLess(lhs,rhs);
  }

  inline bool operator>(const int2048 &lhs, const int2048 &rhs) {
    return operator<(rhs,lhs);
  }

  inline bool operator<=(const int2048 &lhs, const int2048 &rhs) {
    return operator>(lhs,rhs)^1;
  }

  inline bool operator>=(const int2048 &lhs, const int2048 &rhs) {
    return operator<(lhs,rhs)^1;
  }





} // namespace sjtu

#endif
