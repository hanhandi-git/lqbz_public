#include <iostream>
#include <string>
using namespace std;

int GetBirthdayFromCitizenId(const string& citizenId, int& birthYear, int& birthMonth, int& birthDay)
{
    if (citizenId.length() != 18 && citizenId.length() != 15)
    {
        std::cout << "length is not right" << std::endl;
        return -1; // 使用-1代替FICUS_INVALID_ARGUMENT_ERROR
    }

    try
    {
        // 18位身份证号，第7-10位表出生年份，11-12表示出生月份，13-14表示出生日期
        if (citizenId.length() == 18)
        {
            birthYear = std::stoi(citizenId.substr(6, 4));
            birthMonth = std::stoi(citizenId.substr(10, 2));
            birthDay = std::stoi(citizenId.substr(12, 2));
        }
        // 旧式15位身份证号，第7-8位表示19xx年出生的年份后两位，9-10表示出生月份，11-12表示出生日期
        else if (citizenId.length() == 15)
        {
            birthYear = 1900 + std::stoi(citizenId.substr(6, 2));
            birthMonth = std::stoi(citizenId.substr(8, 2));
            birthDay = std::stoi(citizenId.substr(10, 2));
        }
    } catch (...)
    {
        std::cout << "Transformation of birthday failed!" << std::endl;
        return -1; // 使用-1代替FICUS_INVALID_ARGUMENT_ERROR
    }

    return 0; // 使用0代替FICUS_SUCC
}

int CheckBirthdayIsRight(const string& citizenId)
{    
    int rtn;
    
    int birthDay, birthMonth, birthYear;

    rtn = GetBirthdayFromCitizenId(citizenId, birthYear, birthMonth, birthDay);
    if (rtn != 0) {
        std::cout << "Failed to get birthday information from citizenId!" << std::endl;
        return -1; // 使用-1代替FICUS_INVALID_ARGUMENT_ERROR
    }

    int dayCount;

    if (birthMonth < 1 || birthMonth > 12)
    {
        std::cout << "the month of birth is wrong!" << std::endl;
        return -1; // 使用-1代替FICUS_INVALID_ARGUMENT_ERROR
    }
    else 
    {
        if (birthMonth == 4 || birthMonth == 6 || birthMonth == 9 || birthMonth == 11)
        {
            dayCount = 30;
        }
        else if(birthMonth == 2)
        {
            if ((birthYear % 4 == 0) && ((birthYear % 100 != 0) || (birthYear % 400 == 0))) //闰年
            {
                dayCount = 29;
            }
            else
            {
                dayCount = 28;
            }
        }
        else
        {
            dayCount = 31;
        }
    }
    if (birthDay < 1 || birthDay > dayCount)
    {
        std::cout << "the date of birth is wrong!" << std::endl;
        return -1; // 使用-1代替FICUS_INVALID_ARGUMENT_ERROR
    }

    return 0; // 使用0代替FICUS_SUCC
}

int CheckCitizenIdIsRight(const string& citizenId)
{    
    int rtn;

    if (citizenId.length() != 15 && citizenId.length() != 18 )  //长度错误
    {
        std::cout << "The length of citizen ID is not legal！" << std::endl;
        return -1; // 使用-1代替FICUS_INVALID_ARGUMENT_ERROR
    }
    
    int index = citizenId.find_first_not_of("1234567890xX");

    if (index != citizenId.npos)    // 有非法字符
    {
        std::cout << "The input citizen ID has illegal character!" << std::endl;
        return -1; // 使用-1代替FICUS_INVALID_ARGUMENT_ERROR
    }

    rtn = CheckBirthdayIsRight(citizenId);
    if (rtn != 0) {
        std::cout << "判断出生年月是否合理失败" << std::endl; // 使用-1代替FICUS_INVALID_ARGUMENT_ERROR
        return -1;
    }

    if (citizenId.length() == 18)     // 18位身份证号的最后一位为校验位，基于此进行校验 
    {
        int no[] = {7, 9, 10, 5, 8, 4, 2, 1, 6, 3, 7, 9, 10, 5, 8, 4, 2};
        char checkBit[] = {'1', '0', 'X', '9', '8', '7', '6', '5', '4', '3', '2'};
        //1 0 X 9 8 7 6 5 4 3 2   校验码
        const char *start = citizenId.c_str();
        int idx = 0, weightI = 0, sum = 0;
        for (idx = 0; idx < 17; idx++)
        {
            weightI = (*(start+idx) - '0') * no[idx];
            sum += weightI;
        }
        if (*(start+idx) < '0' || *(start+idx) > '9')
        {
            if (*(start+idx) != 'X' && *(start+idx) != 'x')
            {
                std::cout << "身份证最后一位输入错误!" << std::endl;
                return -1; // 使用-1代替FICUS_INVALID_ARGUMENT_ERROR
            }
        }
        weightI = sum % 11;
        if (*(start+17) == 'x' || *(start+17) == 'X')         //最后一位为'x'或'X';             
        {
            if (checkBit[weightI] != 'x' && checkBit[weightI] != 'X')
            {
                std::cout << "身份证信息不合法!" << std::endl;
                return -1; // 使用-1代替FICUS_INVALID_ARGUMENT_ERROR
            }
        }
        else if (checkBit[weightI] != *(start+17))     //判断计算出的身份证校验码与输入是否相符  
        {
            std::cout << "身份证信息不合法!" << std::endl;
            return -1; // 使用-1代替FICUS_INVALID_ARGUMENT_ERROR
        }
    }

    return 0; // 使用0代替FICUS_SUCC
} 


void TestCheckCitizenIdIsRight() {
    // CitizenIdUtility utility; // 这里需要定义或包含CitizenIdUtility类

    // 测试用例
    struct TestCase {
        std::string citizenId;
    };

    TestCase testCases[] = {
        {"651723199212212212"},
        {"650300195406090318"}
    };

    for (const auto& testCase : testCases) {
        int result = CheckCitizenIdIsRight(testCase.citizenId); // 直接调用CheckCitizenIdIsRight
        if (result == 0) {
            std::cout << "Test passed for citizenId: " << testCase.citizenId << std::endl;
        } else {
            std::cout << "Test failed for citizenId: " << testCase.citizenId << ", res = "<< result << std::endl;
        }
    }
}



int main() {
    TestCheckCitizenIdIsRight();
    return 0;
} 
// 130431199803022112