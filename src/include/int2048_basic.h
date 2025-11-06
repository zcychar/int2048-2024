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
#include <iostream>
#include <vector>

// 请不要使用 using namespace std;

namespace sjtu {
  class int2048 {
    std::vector<int> digit  ;
    bool symbol = false; //true=negative
    int length = 0;

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

    // ===================================
    // Integer2
    // ===================================

    int2048 operator+() const;

    int2048 operator-() const;

    int2048 &operator=(const int2048 &);

    int2048 &operator+=(const int2048 &);

    friend int2048 operator+(int2048, const int2048 &);

    int2048 &operator-=(const int2048 &);

    friend int2048 operator-(int2048, const int2048 &);

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
    length=1;
    digit.push_back(0);
  }

  inline int2048::int2048(long long num) {
    if (num < 0) {
      symbol = true;
      num = -num;
    }
    while (num) {
      digit.push_back(num % 10);
      num /= 10;
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
    symbol = false;
    length = str.length();
    if (str[0] == '-') {
      symbol = true;
      length--;
      if (str[1] == '0') {
        digit.push_back(0);
        symbol = false;
        length = 1;
        return;
      }
    }
    digit.resize(length);
    for (int i = 0; i < length; ++i) {
      digit[i] = (str[str.size() - i - 1] - '0');
    }
  }

  inline void int2048::print() {
    if (symbol) {
      std::cout << '-';
    }
    for (int i = length - 1; i >= 0; --i) {
      std::cout << digit[i];
    }
  }

  inline int2048 &int2048::add(const int2048 &other) {
    if (symbol == other.symbol) {
      int res = 0;
      length = std::max(length, other.length);
      digit.resize(length);
      for (int i = 0; i < other.length; ++i) {
        digit[i] = digit[i] + other.digit[i] + res;
        res = digit[i] / 10;
        digit[i] %= 10;
      }
      for (int i = other.length; i < length; ++i) {
        digit[i] = digit[i] + res;
        res = digit[i] / 10;
        digit[i] %= 10;
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
        symbol^=1;
        for (int i = 0; i < length; ++i) {
          digit[i] = (other.digit[i] - digit[i] - res);
          res = 0;
          if (digit[i] < 0) {
            res = 1;
            digit[i] += 10;
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
            digit[i] += 10;
          }
        }
        for (int i = other.length; i < length; ++i) {
          digit[i] = digit[i] - res;
          res = 0;
          if (digit[i] < 0) {
            res = 1;
            digit[i] += 10;
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
      int res = 0;
      length = std::max(length, other.length);
      digit.resize(length);
      for (int i = 0; i < other.length; ++i) {
        digit[i] = digit[i] + other.digit[i] + res;
        res = digit[i] / 10;
        digit[i] %= 10;
      }
      for (int i = other.length; i < length; ++i) {
        digit[i] = digit[i] + res;
        res = digit[i] / 10;
        digit[i] %= 10;
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
        int res = 0;
        digit.resize(other.length);
        length = other.length;
        symbol = symbol^1;
        for (int i = 0; i < length; ++i) {
          digit[i] = (other.digit[i] - digit[i] - res);
          res = 0;
          if (digit[i] < 0) {
            res = 1;
            digit[i] += 10;
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
            digit[i] += 10;
          }
        }
        for (int i = other.length; i < length; ++i) {
          digit[i] = digit[i] - res;
          res = 0;
          if (digit[i] < 0) {
            res = 1;
            digit[i] += 10;
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
} // namespace sjtu

#endif
