// MathFoundation.hpp
// MathBase
// 信息安全的数学基础课程依赖库
// 实现一系列信息安全的数学基础运算
//
//  Created by 杨磊 on 2019/4.
//  Copyright © 2019 杨磊. All rights reserved.
//

#ifndef MathFoundation_hpp
#define MathFoundation_hpp

#include <cmath>
#include <vector>
#include <list>
#include <algorithm>

//使用欧几里得除法计算最大公因数，两个参数为给定的两个整数，返回值为最大公因数
int MaxCommonFactor(const int numa, const int numb){
    int a,b,r;
    if(numa == 0 && numb == 0){
        return -1;//不存在最大公因数
    }
    if(numa == 0 || numb == 0){
        return (numa == 0 ? numb:numa);//0和任意数A的最大公因数为A
    }
    //设置初始值
    a = abs(numa) > abs(numb)? abs(numa):abs(numb);
    b = abs(numa) < abs(numb)? abs(numa):abs(numb);
    r = abs(numa);
    //迭代相除
    while(r != 0){
        r = a%b;
        a = b;
        b = r;
    }
    return a;
}

//使用最大公因数判断两个整数是否互质/互素
bool isCoprime(const int numa,const int numb){
    return (MaxCommonFactor(numa, numb) == 1);
}

//计算Bezout等式，前两个参数为给定的整数，后两个参数为计算得到的系数的引用
//printResult为真时，输出 s*a+t*b = (a,b)
void BezoutEquation(const int a,const int b,int & s,int & t,bool printResult = true){
    std::vector<int> va,vb,vq,vr;//存放计算得到的序列
    std::vector<int>::iterator itr;
    std::vector<int> ans;//存放反向迭代得到的参数
    
    //特殊情况的处理
    if(a == 0 || b == 0){
        if(a == 0 && b == 0){
            s = t = 0;
            std::cout << "a and b should not be both 0 \n";
            return ;
        }
        s = (a == 0? 0:1);
        t = (b == 0? 0:1);
        std::cout << s << " * " << a << " + "
        << t << " * " << b << " = "
        << "(" << a << "," << b << ") = "
        << MaxCommonFactor(a,b) << "\n" ;
        return ;
    }
    //赋初值
    va.push_back(abs(a) > abs(b)? abs(a):abs(b));
    vb.push_back(abs(a) < abs(b)? abs(a):abs(b));
    //计算a=q*b+r
    while(true){
        vq.push_back(va.back()/vb.back());
        vr.push_back(va.back()%vb.back());
        if(vr.back() == 0)break;
        va.push_back(vb.back());
        vb.push_back(vr.back());
    }
    //为便于计算，移除最后r=0的系数并反转
    vq.pop_back();
    std::reverse(vq.begin(),vq.end());
    //赋初值
    ans.push_back(-vq[0]);
    ans.push_back(1);
    //反向代回
    for(int i = 1; i < vq.size();i++){
        ans[i] -= ans[i-1] * vq[i];
        ans.push_back(ans[i-1]) ;
    }
    //为便于导出结果，移除最后一次的push并反转
    ans.pop_back();
    std::reverse(ans.begin(), ans.end());
    //得到与输入相符合的结果
    s = (a<b?ans[0]:ans[1]);
    t = (a<b?ans[1]:ans[0]);
    //输出结果
    if(printResult){
        std::cout << s << " * " << a << " + "
        << t << " * " << b << " = "
        << "(" << a << "," << b << ") = "
        << MaxCommonFactor(a,b) << "\n" ;
    }
    return ;
}

//使用递归方法计算欧几里得除法，前两个参数为被除数和除数，后两个参数为商和余数的引用
//形式为：dividend = qoutient * divisor + remainder
void EuclidDivide(const int dividend, const int divisor, int &quotient, int &remainder){
    if(dividend < divisor){//递归终止条件
        remainder = dividend;
        return ;
    }
    else{
        quotient += 1;
        EuclidDivide(dividend - divisor, divisor, quotient, remainder);
    }
}

//使用欧几里得除法判断素数
//printInfo 为真时输出：number is multiple of i
bool isPrime(const int number,bool printInfo = false){
    int quo = 0,re = 0;
    for(int i = 2;i <= sqrt(number);i++){//这里i的取值还可以进一步优化
        EuclidDivide(number, i, quo, re);
        if(re == 0){
            if(printInfo)std::cout << number << " is multiple of " << i << "\n";
            return false;
        }
    }
    return true;
}

//判断素数的交互式工具
void isPrimeNum(){
    int number;
    std::cout << "please enter a number:";
    std::cin >> number;
    if(isPrime(number)){
        std::cout << number << " is prime\n" ;
    }
    else{
        std::cout << number << " is not prime\n";
    }
}

//使用模重复平方算法计算
//base^exp (mod mod)
int opeMod(int base,int exp,int mod){
    //设置初值
    long answer = 1;
    long val = base % mod;
    
    while(exp != 0){
        if(exp % 2 == 1){
            answer = (answer *val) % mod;
        }
        exp /= 2;
        val = (val * val) % mod ;
    }
    
    while(answer <= 0)answer += mod;
    return int(answer % mod);
}

//模平方计算法的交互工具
void opeM(){
    int base,exp,mod;
    std::cout << "enter base:";std::cin >> base;
    std::cout << "enter exp:";std::cin >> exp;
    std::cout << "enter mod:";std::cin >> mod;
    std::cout << "answer is :" << opeMod(base, exp, mod) << std::endl;
}

//计算模m逆元的交互式工具
void opeE(){
    int origin,answer,modx,noUse;
    std::cout << "enter origin:";std::cin >> origin;
    std::cout << "enter mod:";std::cin >> modx;
    BezoutEquation(origin, modx, answer, noUse);
    while(answer <= 0){
        answer += modx;
    }
    std::cout << "the answer:" << answer << std::endl;
}

//分解素因数
int countFactor(int number,int factor);
void findFactor(const int number){
    int factor = 1;
    std::list<int> factors;
    std::list<int> exps;
    
    while(factor <= number){
        factor ++;
        if(!isPrime(factor))continue;
        if(number % factor != 0)continue;
        factors.push_back(factor);
        exps.push_back(countFactor(number, factor));
    }
    std::cout << std::endl << number << " = ";
    while(!factors.empty()){
        std::cout << factors.front() << " ^ " << exps.front() << " ";
        if(factors.size() != 1){
            std::cout << " * ";
        }
        factors.pop_front();
        exps.pop_front();
    }
}

//计算因数次数的辅助函数
int countFactor(int number, int factor){
    int exp = 0;
    
    while(number % factor == 0){
        exp ++;
        number /= factor;
    }
    return exp;
}

//分解质因数的交互式工具
void opeFF(void){
    int number = 0;
    std::cout << "please enter the number:";
    std::cin >> number;
    std::cout << "the answer is:";
    findFactor(number);
    
}

//使用贝祖等式计算不定方程
//方程形式：factor1 * x + factor2 = rightValue
bool DiophantineEquation(int factor1,int factor2,int rightValue){
    int x,y;
    int CF = MaxCommonFactor(factor1, factor2);
    int ratio = rightValue / CF;
    
    if(rightValue % CF != 0){
        std::cout << "the equation " << factor1 << " * x + " << factor2 << " * y = " << rightValue
        << " have no solution" << std::endl;
        return false;
    }
    BezoutEquation(factor1, factor2, x, y);
    x *= ratio;
    y *= ratio;
    std::cout << "Special solution: x = " << x << " , y = " << y << std::endl;
    std::cout << "Common solution: x = " << x << " - " << factor2/CF << " * t , y = "
    << y << " + " << factor1/CF << " * t . (t = 1,2,3...)" << std::endl;
    return true;
}

#endif /*MathFoundation_hpp*/