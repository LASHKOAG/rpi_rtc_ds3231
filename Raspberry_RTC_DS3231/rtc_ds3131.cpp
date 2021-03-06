#include "rtc_ds3131.h"

RTC_DS3131::RTC_DS3131()
{

}

time_t RTC_DS3131::get_rawtime_from_RTC()
{
    time_t seconds;
    seconds = time (NULL); // получить текущую дату, выраженную в секундах
    return seconds;
}

char* RTC_DS3131::get_localtime_from_RTC()
{
    time_t rawtime = get_rawtime_from_RTC();
    return ctime (&rawtime);
}

int8_t RTC_DS3131::get_command_hardware_time()
{
    std::string cmd ="sudo hwclock -r";
    if(system_send_command(cmd) < 0){
        std::cout << "Problem: NO get hardware time\n";
        return ERR_NO_HARDWARE_TIME;
    }
    return  0;
}

int8_t RTC_DS3131::system_send_command(std::string m_cmd){
    char *cstr = new char[m_cmd.length() + 1];
    strcpy(cstr, m_cmd.c_str());
    if (system(cstr)!=0){ return  -1; }
    delete[] cstr;
    return  0;
}


int8_t RTC_DS3131::get_command_sync_sys_hw()
{
    std::string cmd ="sudo hwclock --hctosys";

    if(system_send_command(cmd) < 0){
        std::cout << "Problem: NO sync system time with hardware time\n";
        return ERR_NO_SYNC_TIME;
    }

    return 0;
}

int8_t RTC_DS3131::set_time_in_RTC(time_t t)
{
    char buff[20];
    strftime(buff, 20, "%Y-%m-%d %H:%M:%S", localtime(&t));
    //    если необходимо время utc
    //    strftime(buff, 20, "%Y-%m-%d %H:%M:%S", gmtime(&t));
    std::string s(buff);
    //    для отладки
    //    std::cout << "formatted time from time_t = " << s <<std::endl;

    std::string str2 ="\"" + s + "\"";
    std::string cmd = "sudo date --set " + str2;
    //    для отладки
    //        std::cout << "cmdcmd = " << cmd << std::endl;

    //установили системное время
    //sudo date --set "2004-02-29 16:21:42"
    if(system_send_command(cmd) < 0){
        std::cout << "Problem: NO set time in system\n";
        return ERR_NO_SET_TIME_SYS;
    }

    //записали время в RTC
    int8_t res_hwclock = get_command_hwclock();
    if(res_hwclock !=0){ return res_hwclock; }

    //    time_t now = time(0);
    //    tm *ltm = localtime(&now);
    //    std::stringstream date;
    //    date << ltm->tm_mday
    //         << "/"
    //         << 1 + ltm->tm_mon
    //         << "/"
    //         << 1900 + ltm->tm_year
    //         << " "
    //         << 1 + ltm->tm_hour
    //         << ":"
    //         << 1 + ltm->tm_min
    //         << ":"
    //         << 1 + ltm->tm_sec;
    //    std::cout << date.str() << "\n";

    return 0;
}

int8_t RTC_DS3131::set_time_in_RTC_string(std::string str)
{
    //пример ввода даты: "2004-02-29 16:21:42"
    //sudo date --set "2004-02-29 16:21:42"

    std::string str2 ="\"" + str + "\"";
    std::string cmd = "sudo date --set " + str2;
        std::cout << "\ncmd = " << cmd << std::endl;

    //    char *cstr = new char[str.length() + 1];
    //    strcpy(cstr, str.c_str());
    //    // do stuff
    //    delete [] cstr;

     //установили системное время
     //sudo date --set "2004-02-29 16:21:42"
     if(system_send_command(cmd) < 0){
        std::cout << "Problem: NO set time in system\n";
        return ERR_NO_SET_TIME_SYS;
     }

    //записали время в RTC (аппаратное время берется с RTC)
    int8_t res_hwclock = get_command_hwclock();
    if(res_hwclock !=0){ return res_hwclock; }

    return 0;
}

int8_t RTC_DS3131::get_command_hwclock()
{
    std::string cmd = "sudo hwclock -w";

    if(system_send_command(cmd) < 0){
        std::cout << "Problem: NO set time in hardware\n";
        return ERR_NO_SET_TIME_HARD;
    }

    return 0;
}

RTC_DS3131::~RTC_DS3131()
{

}


//#include <iostream>
//#include <string.h>
//#include <cstdio>
//using namespace std;

//time_t string_to_time_t(string s)
//{
//int yy, mm, dd, hour, min, sec;
//struct tm when;
//long tme;

//memset(&when, 0, sizeof(struct tm));
//sscanf(s.c_str(), "%d/%d/%d:%d:%d:%d", &dd, &mm, &yy, &hour, &min, &sec);

//time(&tme);
//when = *localtime(&tme);
//when.tm_year = yy;
//when.tm_mon = mm-1;
//when.tm_mday = dd;
//when.tm_hour = hour;
//when.tm_min = min;
//when.tm_sec = sec;

//return mktime(&when);
//}

//string time_t_to_string(time_t t)
//{
//char buff[20];
//strftime(buff, 20, "%d/%m/%Y:%H:%M:%S", localtime(&t));
//string s(buff);
//return s;
//}

//int main()
//{
//string s = "30/11/2012:13:49:55";

//time_t t = string_to_time_t(s);
//string ss = time_t_to_string(t);

//cout << ss << "\n";return 0;
//}
