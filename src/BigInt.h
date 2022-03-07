//
// Created by edward on 2021/6/23.
//

#ifndef OJ_BIGINT_H
#define OJ_BIGINT_H

#include <string>
#include <vector>
#include <iostream>

class BigInt {
    using ll = long long;
    static constexpr int BASE = 1e8;    //基数是1e8，即满1e8才进一位
    static constexpr int WIDTH = 8;     //每一位的十进制长度是8位，主要用于字符串和转换
    std::vector<int> bit_;               //保存BigInt的每一位，小端模式：低位低字节，高位高字节
    bool negative_;                      //记录数字是否是负数
public:
    static std::pair<int, int> trim(const std::string &s);                           //trim函数：删除头部和尾部的空格
    static void num2big(std::vector<int> &bit, ll num);         //实际处理函数：将正数num放入bit数组中
    static void str2big(std::vector<int> &bit,const std::string &s, std::pair<int, int> pr); //实际处理函数：将正数字符串放入bit数组中
    static bool less(const BigInt &lhs, const BigInt &rhs);     //比较两个数字的绝对值的大小
    BigInt(ll num = 0);
    BigInt(const std::string &s);
    BigInt & operator =(ll num);         //必须返回BigInt&，与内置类型一致
    BigInt & operator =(const std::string &s);
    BigInt operator -() const;
    friend std::ostream &operator << (std::ostream &os, const BigInt &bigInt);
    friend std::istream &operator >> (std::istream &is, BigInt &bigInt);
    friend BigInt operator + (const BigInt &lhs, const BigInt &rhs);
    friend BigInt operator - (const BigInt &lhs, const BigInt &rhs);
    friend BigInt operator * (const BigInt &lhs, const BigInt &rhs);
    friend BigInt operator / (const BigInt &lhs, const BigInt &rhs);
    friend bool operator < (const BigInt &lhs, const BigInt &rhs);
    friend bool operator > (const BigInt &lhs, const BigInt &rhs);
    friend bool operator == (const BigInt &lhs, const BigInt &rhs);
    friend bool operator != (const BigInt &lhs, const BigInt &rhs);
    friend bool operator <= (const BigInt &lhs, const BigInt &rhs);
    friend bool operator >= (const BigInt &lhs, const BigInt &rhs);
    BigInt & operator += (const BigInt &rhs);
    BigInt & operator -= (const BigInt &rhs);
    BigInt & operator *= (const BigInt &rhs);
    BigInt & operator /= (const BigInt &rhs);
    std::string toString() const;
    BigInt & setOppo();                 //取相反数
    bool isNegative() const;            //判断是否是一个负数
};

std::ostream & operator << (std::ostream &os, const BigInt &bigInt);
std::istream & operator >> (std::istream &is, BigInt &bigInt);
BigInt operator + (const BigInt &lhs, const BigInt &rhs);
BigInt operator - (const BigInt &lhs, const BigInt &rhs);
BigInt operator * (const BigInt &lhs, const BigInt &rhs);
BigInt operator / (const BigInt &lhs, const BigInt &rhs);
bool operator < (const BigInt &lhs, const BigInt &rhs);
bool operator > (const BigInt &lhs, const BigInt &rhs);
bool operator == (const BigInt &lhs, const BigInt &rhs);
bool operator != (const BigInt &lhs, const BigInt &rhs);
bool operator <= (const BigInt &lhs, const BigInt &rhs);
bool operator >= (const BigInt &lhs, const BigInt &rhs);


#endif //OJ_BIGINT_H
