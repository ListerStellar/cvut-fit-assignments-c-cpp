#ifndef __PROGTEST__
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

constexpr unsigned DOW_MON = 0b0000'0001;
constexpr unsigned DOW_TUE = 0b0000'0010;
constexpr unsigned DOW_WED = 0b0000'0100;
constexpr unsigned DOW_THU = 0b0000'1000;
constexpr unsigned DOW_FRI = 0b0001'0000;
constexpr unsigned DOW_SAT = 0b0010'0000;
constexpr unsigned DOW_SUN = 0b0100'0000;
constexpr unsigned DOW_WORKDAYS = DOW_MON | DOW_TUE | DOW_WED | DOW_THU | DOW_FRI;
constexpr unsigned DOW_WEEKEND = DOW_SAT | DOW_SUN;
constexpr unsigned DOW_ALL = DOW_WORKDAYS | DOW_WEEKEND;

typedef struct TDate {
    unsigned m_Year;
    unsigned m_Month;
    unsigned m_Day;
} TDATE;

TDATE makeDate(unsigned y, unsigned m, unsigned d) {
    TDATE res = { y, m, d };
    return res;
}
#endif /* __PROGTEST__ */




unsigned getWeekDay(TDate date) {
    if (date.m_Month < 3) { date.m_Month += 12; date.m_Year -= 1; }
    switch ((date.m_Day + 2 * date.m_Month + (3 * (date.m_Month + 1) / 5) + date.m_Year + (date.m_Year / 4) - (date.m_Year / 100) + (date.m_Year / 400) - (date.m_Year / 4000)) % 7) {
    case 0:
        return DOW_MON;
        break;
    case 1:
        return DOW_TUE;
        break;
    case 2:
        return DOW_WED;
        break;
    case 3:
        return DOW_THU;
        break;
    case 4:
        return DOW_FRI;
        break;
    case 5:
        return DOW_SAT;
        break;
    case 6:
        return DOW_SUN;
        break;
    default:
        return 0;
        break;
    }
}
unsigned getWeekDayNum(int date) {
    switch (date) {
    case DOW_MON:
        return 0;
        break;
    case DOW_TUE:
        return 1;
        break;
    case DOW_WED:
        return 2;
        break;
    case DOW_THU:
        return 3;
        break;
    case DOW_FRI:
        return 4;
        break;
    case DOW_SAT:
        return 5;
        break;
    case DOW_SUN:
        return 6;
    default:
        return 0;
        break;
    }
}
bool isLeapYear(int year) {
    bool is_leap = false;
    is_leap = (!is_leap && (year % 4 == 0)) ? true : is_leap;
    is_leap = (is_leap && (year % 100 == 0)) ? false : is_leap;
    is_leap = (!is_leap && (year % 400 == 0)) ? true : is_leap;
    is_leap = (is_leap && (year % 4000 == 0)) ? false : is_leap;
    return is_leap;
}
unsigned int daysNum(int year, int month) {
    int days_num[12] = { 31, isLeapYear(year) ? 29 : 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    return days_num[month - 1];
}
int dateCompare(TDATE a, TDATE b) { // return 1 if a > b, 0 if a == b, -1 if a < b
    if (a.m_Year > b.m_Year) return 1;
    if (a.m_Year < b.m_Year) return -1;

    if (a.m_Month > b.m_Month) return 1;
    if (a.m_Month < b.m_Month) return -1;

    if (a.m_Day > b.m_Day) return 1;
    if (a.m_Day < b.m_Day) return -1;

    return 0;
}
TDATE addDaysToDate(TDATE date, int days) {
    for (int i = 1; i <= days; i++) {
        date.m_Day += 1;
        if (date.m_Day > daysNum(date.m_Year, date.m_Month)) {
            date.m_Day = 1; date.m_Month += 1;
            if (date.m_Month > 12) {
                date.m_Month = 1; date.m_Year += 1;
            }
        }
    }
    return date;
}
TDATE remDaysOffDate(TDATE date, int days) {
    for (int i = 1; i <= days; i++) {
        date.m_Day -= 1;
        if (date.m_Day <= 0) {
            date.m_Month -= 1;
            if (date.m_Month <= 0) {
                date.m_Month = 12; date.m_Year -= 1;
            }
            date.m_Day = daysNum(date.m_Year, date.m_Month);
        }
    }
    return date;
}
int julianDate(TDATE date) {
    if (date.m_Month <= 2) {
        date.m_Year -= 1;
        date.m_Month += 12;
    }
    return 365 * date.m_Year + date.m_Year / 4 - date.m_Year / 100 + date.m_Year / 400 + (153 * date.m_Month - 457) / 5 + date.m_Day - 306;
}
unsigned int getDateDiff(TDATE date1, TDATE date2) {
    return abs(julianDate(date2) - julianDate(date1)) + 1;
}

long long bad_countConnections(TDATE from, TDATE to, unsigned perWorkDay, unsigned dowMask) {
    if (from.m_Year < 2000 || from.m_Year > 1000000000 || to.m_Year < 2000 || to.m_Year > 1000000000 || dateCompare(from, to) == 1 || from.m_Month <= 0 || to.m_Month <= 0 || from.m_Month > 12 || to.m_Month > 12 || from.m_Day <= 0 || to.m_Day <= 0 || from.m_Day > daysNum(from.m_Year, from.m_Month) || to.m_Day > daysNum(to.m_Year, to.m_Month)) {
        return -1;
    }

    int addition, concount = 0; TDATE now = from; unsigned nowaday;
    while (dateCompare(now, to) <= 0) {
        nowaday = getWeekDay(now);
        if (nowaday & dowMask) {
            addition = perWorkDay;
            if (nowaday & DOW_SAT) addition = ceil(addition / 2.0);
            if (nowaday & DOW_SUN) addition = ceil(addition / 3.0);
            concount += addition;
        }
        now = addDaysToDate(now, 1);
    }
    return concount;
}

long long countConnections(TDATE from, TDATE to, unsigned perWorkDay, unsigned dowMask) {
    if (from.m_Year < 2000 || from.m_Year > 1000000000 || to.m_Year < 2000 || to.m_Year > 1000000000 || dateCompare(from, to) == 1 || from.m_Month <= 0 || to.m_Month <= 0 || from.m_Month > 12 || to.m_Month > 12 || from.m_Day <= 0 || to.m_Day <= 0 || from.m_Day > daysNum(from.m_Year, from.m_Month) || to.m_Day > daysNum(to.m_Year, to.m_Month)) {
        return -1;
    }

    if (getDateDiff(from, to) < 20000) {
        return bad_countConnections(from, to, perWorkDay, dowMask);
    }
    else {
        int concount = 0;
        unsigned from_nowaday = getWeekDay(from);
        unsigned to_nowaday = getWeekDay(to);
        TDATE new_from = from;
        TDATE new_to = to;
        if (from_nowaday != DOW_MON) {
            new_from = addDaysToDate(from, 6 - getWeekDayNum(from_nowaday));
            concount += bad_countConnections(from, new_from, perWorkDay, dowMask);
            new_from = addDaysToDate(new_from, 1);
        }
        if (to_nowaday != DOW_SUN) {
            new_to = remDaysOffDate(to, getWeekDayNum(to_nowaday));
            concount += bad_countConnections(new_to, to, perWorkDay, dowMask);
            new_to = remDaysOffDate(new_to, 1);
        }
        concount += bad_countConnections(new_from, addDaysToDate(new_from, 6), perWorkDay, dowMask) * (int)(getDateDiff(new_from, to) / 7);
        return concount;
    }

    return -1;
}

TDATE bad_endDate(TDATE from, long long connections, unsigned perWorkDay, unsigned dowMask) {
    if (from.m_Year < 2000 || from.m_Year > 1000000000 || from.m_Month <= 0 || from.m_Month > 12 || from.m_Day <= 0 || from.m_Day > daysNum(from.m_Year, from.m_Month) || connections <= 0 || perWorkDay == 0 || dowMask == 0) {
        return makeDate(0000, 0, 0);
    }


    //     UNOPTIMALIZED
    TDATE now = from; long long concount = connections; unsigned nowaday = getWeekDay(now); int minus_val; bool cnt = true;

    //if (nowaday & dowMask) {
    //    if (nowaday & DOW_SAT) minus_val = ceil(perWorkDay / 2.0);
    //    else if (nowaday & DOW_SUN) minus_val = ceil(perWorkDay / 3.0);
    //    else minus_val = perWorkDay;
    //    if (concount < minus_val) return makeDate(0000, 0, 0);
    //}

    while (cnt) {
        nowaday = getWeekDay(now);
        if (nowaday & dowMask) {
            if (nowaday & DOW_SAT) minus_val = ceil(perWorkDay / 2.0);
            else if (nowaday & DOW_SUN) minus_val = ceil(perWorkDay / 3.0);
            else minus_val = perWorkDay;
            if (concount >= minus_val) {
                if (nowaday & DOW_SAT) concount -= ceil(perWorkDay / 2.0);
                else if (nowaday & DOW_SUN) concount -= ceil(perWorkDay / 3.0);
                else concount -= perWorkDay;
            }
            else {
                cnt = false;
            }
        }
        now = addDaysToDate(now, 1);
    }
    now = remDaysOffDate(now, 2);
    return now;
}

TDATE endDate(TDATE from, long long connections, unsigned perWorkDay, unsigned dowMask) {
    if (from.m_Year < 2000 || from.m_Year > 1000000000 || from.m_Month <= 0 || from.m_Month > 12 || from.m_Day <= 0 || from.m_Day > daysNum(from.m_Year, from.m_Month) || connections <= 0 || connections >= 1000000000 || perWorkDay == 0 || dowMask == 0) {
        return makeDate(0000, 0, 0);
    }

    int minus_val, concount = connections;
    TDATE now = from;
    unsigned nowaday = getWeekDay(now);
    if (nowaday & dowMask) {
        if (nowaday & DOW_SAT) minus_val = ceil(perWorkDay / 2.0);
        else if (nowaday & DOW_SUN) minus_val = ceil(perWorkDay / 3.0);
        else minus_val = perWorkDay;
        if (concount < minus_val) return makeDate(0000, 0, 0);
    }

    if (connections > perWorkDay * 2100) {
        //printf("%d\n", nowaday);
        while (nowaday != DOW_SUN) {
            nowaday = getWeekDay(now);
            //printf("%d\n", getWeekDayNum(getWeekDay(now)));
            if (nowaday & dowMask) {
                if (nowaday & DOW_SAT) minus_val = ceil(perWorkDay / 2.0);
                else if (nowaday & DOW_SUN) minus_val = ceil(perWorkDay / 3.0);
                else minus_val = perWorkDay;
                concount -= minus_val;
            }
            now = addDaysToDate(now, 1);
        }
        nowaday = getWeekDay(now);
        if (nowaday == DOW_SUN) {
            if (nowaday & dowMask) {
                if (nowaday & DOW_SAT) minus_val = ceil(perWorkDay / 2.0);
                else if (nowaday & DOW_SUN) minus_val = ceil(perWorkDay / 3.0);
                else minus_val = perWorkDay;
                concount -= minus_val;
            }
            now = addDaysToDate(now, 1);
        }
        int weekly = 0;
        nowaday = getWeekDay(now);
        while (nowaday != DOW_SUN) {
            nowaday = getWeekDay(now);
            if (nowaday & dowMask) {
                if (nowaday & DOW_SAT) minus_val = ceil(perWorkDay / 2.0);
                else if (nowaday & DOW_SUN) minus_val = ceil(perWorkDay / 3.0);
                else minus_val = perWorkDay;
                concount -= minus_val;
                weekly += minus_val;
            }
            now = addDaysToDate(now, 1);
        }

        int weeks = concount / weekly;
        concount -= weeks * weekly;
        now = addDaysToDate(now, weeks * 7);
        //printf("%d\n", concount);
        
        //printf("%d %d %d\n", now.m_Year, now.m_Month, now.m_Day);

        if (concount == 0) {
            nowaday = getWeekDay(now);
            while (!(nowaday & dowMask)) {
                now = addDaysToDate(now, 1);
                nowaday = getWeekDay(now);
            }
            now = remDaysOffDate(now, 1);
            return now;
        }
        //return bad_endDate(from, connections, perWorkDay, dowMask);
        return bad_endDate(now, concount, perWorkDay, dowMask);
    }
    else {
        return bad_endDate(from, connections, perWorkDay, dowMask);
    }
    return makeDate(0000, 0, 0);
}







#ifndef __PROGTEST__
int main() {
    TDATE d;
    assert(countConnections(makeDate(2024, 10, 1), makeDate(2024, 10, 31), 1, DOW_ALL) == 31);
    assert(countConnections(makeDate(2024, 10, 1), makeDate(2024, 10, 31), 10, DOW_ALL) == 266);
    assert(countConnections(makeDate(2024, 10, 1), makeDate(2024, 10, 31), 1, DOW_WED) == 5);
    assert(countConnections(makeDate(2024, 10, 2), makeDate(2024, 10, 30), 1, DOW_WED) == 5);
    assert(countConnections(makeDate(2024, 10, 1), makeDate(2024, 10, 1), 10, DOW_TUE) == 10);
    assert(countConnections(makeDate(2024, 10, 1), makeDate(2024, 10, 1), 10, DOW_WED) == 0);
    assert(countConnections(makeDate(2024, 1, 1), makeDate(2034, 12, 31), 5, DOW_MON | DOW_FRI | DOW_SAT) == 7462);
    assert(countConnections(makeDate(2024, 1, 1), makeDate(2034, 12, 31), 0, DOW_MON | DOW_FRI | DOW_SAT) == 0);
    assert(countConnections(makeDate(2024, 1, 1), makeDate(2034, 12, 31), 100, 0) == 0);
    assert(countConnections(makeDate(2024, 10, 10), makeDate(2024, 10, 9), 1, DOW_MON) == -1);
    assert(countConnections(makeDate(2024, 2, 29), makeDate(2024, 2, 29), 1, DOW_ALL) == 1);
    assert(countConnections(makeDate(2023, 2, 29), makeDate(2023, 2, 29), 1, DOW_ALL) == -1);
    assert(countConnections(makeDate(2100, 2, 29), makeDate(2100, 2, 29), 1, DOW_ALL) == -1);
    assert(countConnections(makeDate(2400, 2, 29), makeDate(2400, 2, 29), 1, DOW_ALL) == 1);
    assert(countConnections(makeDate(4000, 2, 29), makeDate(4000, 2, 29), 1, DOW_ALL) == -1);
    d = endDate(makeDate(2024, 10, 1), 100, 1, DOW_ALL);
    assert(d.m_Year == 2025 && d.m_Month == 1 && d.m_Day == 8);
    d = endDate(makeDate(2024, 10, 1), 100, 6, DOW_ALL);
    assert(d.m_Year == 2024 && d.m_Month == 10 && d.m_Day == 20);
    d = endDate(makeDate(2024, 10, 1), 100, 1, DOW_WORKDAYS);
    assert(d.m_Year == 2025 && d.m_Month == 2 && d.m_Day == 17);
    d = endDate(makeDate(2024, 10, 1), 100, 4, DOW_WORKDAYS);
    assert(d.m_Year == 2024 && d.m_Month == 11 && d.m_Day == 4);
    d = endDate(makeDate(2024, 10, 1), 100, 1, DOW_THU);
    assert(d.m_Year == 2026 && d.m_Month == 9 && d.m_Day == 2);
    d = endDate(makeDate(2024, 10, 1), 100, 2, DOW_THU);
    assert(d.m_Year == 2025 && d.m_Month == 9 && d.m_Day == 17);
    d = endDate(makeDate(2024, 10, 1), 100, 0, DOW_THU);
    assert(d.m_Year == 0 && d.m_Month == 0 && d.m_Day == 0);
    d = endDate(makeDate(2024, 10, 1), 100, 1, 0);
    assert(d.m_Year == 0 && d.m_Month == 0 && d.m_Day == 0);


    //d = makeDate(2024, 10, 1);
    //d = remDaysOffDate(d, getWeekDayNum(getWeekDay(d)));
    //c = addDaysToDate(d, 6);
    // c = addDaysToDate(d, 6-getWeekDayNum(getWeekDay(d)));
    //printf("%d\n", getDateDiff(makeDate(2024,1,1), makeDate(2025,12,31)));
    //d = endDate(makeDate(602452728, 10, 1), 100000230, 2, DOW_THU);
    
    //d = endDate(makeDate(2005, 2, 4), 1521345, 26, DOW_MON | DOW_TUE | DOW_SAT);
    //c = bad_endDate(makeDate(2005, 2, 4), 1521345, 26, DOW_MON | DOW_TUE | DOW_SAT);
    //d = endDate(makeDate(2453, 9, 1), 7, 26, DOW_MON | DOW_TUE | DOW_SAT);
                       //1000000000         1000000000
    //printf("%d %d %d\n", d.m_Year, d.m_Month, d.m_Day);
    //printf("%d %d %d\n%d %d %d\n", d.m_Year,d.m_Month,d.m_Day,c.m_Year,c.m_Month,c.m_Day);

    return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
