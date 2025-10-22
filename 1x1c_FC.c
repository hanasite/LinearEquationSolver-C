#include <stdio.h>
#include <string.h>

//自定义函数
/*  先对=两边进行分割
    然后对每一边进行遍历
对于遍历数组可能遇见的情况有4种：
    1.数字0-9
    2.x
    3.+
    4.-

实现路径：输入字符串，然后对=两边分割
            再遍历,调用对应的四种函数，
            返回值为一个结构体，包含计算结果和下一个位置
        根据返回值更新含x项和常数项的和，并继续遍历直到字符串结束
    最后解方程
*/

typedef struct {
    double value;
    int type; //0表示常数项，1表示含x项
    int ord ;//表示当前数字在字符串中的位置
} NumType;

//1.数字0-9
//参数一和二表示数字所在数组和编号，参数三为常数项或含x项的和，用来计算返回计算的结果
//返回值第一个是计算的结果，第二个是声明该数字的属于常数项（0）还是含x项（1）
NumType isNum(char *input,int ord ,double sum_num)
{   int i = ord;

    //初始化结构体
    NumType num;
    num.value = 0;


     // 核心：先把当前位置及后续的数字全部计算（比如"123x"中的123）
    while (input[i] >= '0' && input[i] <= '9') {
        num.value = num.value * 10 + (input[i] - '0'); // 累加当前数字,'0'的ASCII码是48
        i++; // 移动到下一个字符
    }

    // 计算完数字后，判断下一个字符是什么（决定type）
    if (input[i] == 'x') {
        num.type = 1;       // 含x项
        num.ord = i + 1;    // 下一个要处理的位置（x的下一位）
    } 
    
    else {
        num.type = 0;       // 常数项
        num.ord = i;        // 下一个要处理的位置（当前非数字位）
    }

    return num;   
}


//2.x
//同上
NumType isX(char *input,int ord ,double sum_x)  
//为了和isNum函数参数统一，sumx其实没什么用 
{

    NumType num;
    num.value = 1;
    num.type = 1;
    num.ord = ord + 1; //返回下一个位置

    return num;
}

//3.+
//同上
NumType isPlus(char *input,int ord ,double sum_x)  
{
    NumType num;
    num.value = 0;
   
    if (input[ord+1] == 'x'){
        num.value = 1;
        num.type = 1;
        num.ord = ord + 2; //返回下一个位置
    }
    else{ 
        num = isNum(input, ord + 1, sum_x); 
    }
    return num;
}

//4.-
//同上
NumType isMinus(char *input,int ord ,double sum_x)  
{
    NumType num;
    num.value = 0;
    
    // ord是'-'的位置，下一位是内容
    if (input[ord+1] == 'x'){
        num.value = -1;
        num.type = 1;
        num.ord = ord + 2; //返回下一个位置（x的下一位）
    }
    else{ 
        num = isNum(input,ord+1,sum_x);
        num.value = -num.value; //取反
    }
    return num;
}


int main() {
    char  left[1024] = {0};   // 初始化
    char right[1024] = {0};
    char input[1024] = {0};

    printf("请输入方程：");
    scanf("%s", input); 

// 1查找等号位置
    int equal_pos = -1;
    for (int i = 0; input[i] != '\0'; i++) {
        if (input[i] == '=') {
            equal_pos = i;
            break;  // 找到第一个等号后退出
        }
    }
// 检查是否找到等号（避免无等号的情况）
    if (equal_pos == -1) {
        printf("输入的方程没有等号！\n");
        return 1;
    }

// 2分割等号左边到 left字符串
    int L_length = 0;
    for (int i = 0; i < equal_pos; i++) {  // 复制到等号前
        left[i] = input[i];
        L_length++;
    }
    left[L_length] = '\0';  // 添加结束符

    // 3. 分割等号右边到 right字符串
    int R_length = 0;
    // 从等号的下一个位置（equal_pos + 1）开始复制
    for (int i = equal_pos + 1; input[i] != '\0'; i++) {
        right[R_length] = input[i];
        R_length++;
    }
    right[R_length] = '\0';  // 添加结束符

    // 测试输出
    printf("等号左边：%s\n", left);
    printf("等号右边：%s\n", right);

//初始化含x项和常数项
double x_left = 0; //含x项的和
double num_left = 0; //常数项的和
double x_right = 0; //含x项的和
double num_right = 0; //常数项的和

//对等号左边进行遍历
int current_pos = 0; // 处理的字符串位置
int left_len = strlen(left);
    while (current_pos < left_len) {
        NumType res; // 接收返回的结果

        if (left[current_pos] == '+') {
            res = isPlus(left, current_pos, x_left);
        }
         else if (left[current_pos] == '-') {
            res = isMinus(left, current_pos, x_left);
        } 
        else if (left[current_pos] >= '0' && left[current_pos] <= '9') {
            res = isNum(left, current_pos, num_left);
        } 
        else {
            // 异常字符退出
            printf("左边输入非法字符！\n");
            return 1;
        }

        // 根据返回的type，更新总和
        if (res.type == 1) { // 含x项
            x_left += res.value;
        } 
        else { // 常数项
            num_left += res.value;
        }

        // 更新当前位置
        current_pos = res.ord;
    }


//对等号右边进行遍历
current_pos = 0; // 同上
int right_len = strlen(right);
 while (current_pos < right_len) {
        NumType res;
        if (right[current_pos] == '+') {
            res = isPlus(right, current_pos, x_right);
        } else if (right[current_pos] == '-') {
            res = isMinus(right, current_pos, x_right);
        } else if (right[current_pos] >= '0' && right[current_pos] <= '9') {
            res = isNum(right, current_pos, num_right);
        } else {
            printf("右边输入非法字符！\n");
            return 1;
        }

        // 右边的项最终要移到左边，符号反转
        if (res.type == 1) {
            x_right += res.value;
        } else {
            num_right += res.value;
        }

        current_pos = res.ord;
    }

//最后是 ax=b  -->就是 xleft - xright = num_right - num_left   解方程即可
    double x_total = x_left - x_right;
    double num_total = num_right - num_left;
//debug
    printf("左边x项和:%f，左边常数项和:%f\n", x_left, num_left);
    printf("右边x项和:%f，右边常数项和:%f\n", x_right, num_right);
    printf("含x项总差:%f\n", x_total);
    printf("常数项总差:%f\n", num_total);

    // 避免除以0
    if (x_total == 0) {
        if (num_total == 0) {
            printf("方程有无数解！\n");
        } else {
            printf("方程无解！\n");
        }
    } else {
        printf("方程的解为:x = %.4f\n", num_total / x_total);
    }
}