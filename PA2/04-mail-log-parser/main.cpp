#ifndef __PROGTEST__
#include <cassert>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <list>
#include <algorithm>
#include <memory>
#include <compare>
#include <functional>
#include <optional>

class CTimeStamp {
public:
    CTimeStamp(int year,
               int month,
               int day,
               int hour,
               int minute,
               double sec): year(year), month(month), day(day), hour(hour), minute(minute), sec(sec) {}

    int compare(const CTimeStamp &x) const {
        if (year != x.year) return year > x.year ? 1 : -1;
        if (month != x.month) return month > x.month ? 1 : -1;
        if (day != x.day) return day > x.day ? 1 : -1;
        if (hour != x.hour) return hour > x.hour ? 1 : -1;
        if (minute != x.minute) return minute > x.minute ? 1 : -1;
        if (sec != x.sec) return sec > x.sec ? 1 : -1;
        return 0;
    }

    friend std::ostream &operator <<(std::ostream &os, const CTimeStamp &x) {
        os << std::setw(4) << std::setfill('0') << std::right << x.year << "-"
           << std::setw(2) << std::setfill('0') << x.month << "-"
           << std::setw(2) << std::setfill('0') << x.day << " "
           << std::setw(2) << std::setfill('0') << x.hour << ":"
           << std::setw(2) << std::setfill('0') << x.minute << ":"
           << std::setw(6) << std::setfill('0') << std::fixed << std::setprecision(3) << x.sec;
        return os;
    }

private:
    int year, month, day, hour, minute;
    double sec;
};

class CMail {
public:
    CMail(const CTimeStamp &timeStamp,
          const std::string &from,
          const std::string &to,
          const std::optional<std::string> &subject): m_timeStamp(timeStamp), m_from(from), m_to(to), m_subject(subject) {}

    int compareByTime(const CTimeStamp &x) const {
        return m_timeStamp.compare(x);
    }

    int compareByTime(const CMail &x) const {
        return m_timeStamp.compare(x.m_timeStamp);
    }

    const std::string &from() const { return m_from; }

    const std::string &to() const { return m_to; }

    const std::optional<std::string> &subject() const { return m_subject; }

    const CTimeStamp &timeStamp() const { return m_timeStamp; }

    friend std::ostream &operator <<(std::ostream &os, const CMail &x) {
        os << x.m_timeStamp << " " << x.m_from << " -> " << x.m_to;
        if (x.m_subject)
            os << ", subject: " << *x.m_subject;
        return os;
    }

private:
    CTimeStamp m_timeStamp;
    std::string m_from, m_to;
    std::optional<std::string> m_subject;
};

// your code will be compiled in a separate namespace
namespace MysteriousNamespace {
#endif /* __PROGTEST__ */
//----------------------------------------------------------------------------------------

class CompareTime {
public:
    bool operator()(const CTimeStamp &time1, const CTimeStamp &time2) const {
        return time1.compare(time2) < 0;
    }
};

struct SMail {
    std::string from;
    std::multimap<CTimeStamp, std::string, CompareTime> to;
    std::optional<std::string> subject = std::nullopt;
};

class CMailLog {
public:
    CMailLog() = default;

    ~CMailLog() = default;

    int parseLog(std::istream &in) {
        int parsedCount = 0;
        std::map<std::string, SMail> tmpMails;
        std::string str;
        std::string month_s, time, shit, id, full_content, type, content;
        int month, day, year, hour, minute; double sec; char tmp;
        while (std::getline(in, str)) {
            std::istringstream iss(str);
            iss >> month_s >> day >> year >> hour >> tmp >> minute >> tmp >> sec >> shit >> id;
            month = month_map.at(month_s);
            id = id.substr(0, id.size() - 1);
            full_content = str.substr(str.find(": ") + 2);

            if (full_content.find("from=") == 0) {
                type = "from";
                content = full_content.substr(5);
            }
            else if (full_content.find("subject=") == 0) {
                type = "subject";
                content = full_content.substr(8);
            }
            else if (full_content.find("to=") == 0) {
                type = "to";
                content = full_content.substr(3);
            }
            else {
                type = "err";
                content = "err";
            }

            if (type == "to") ++parsedCount;

            if (type != "err") {
                if (tmpMails.count(id) != 0) {
                    if (type == "subject") tmpMails.at(id).subject = content;
                    if (type == "to") tmpMails.at(id).to.insert({ CTimeStamp(year, month, day, hour, minute, sec), content });
                }
                else {
                    SMail tempMail;
                    tempMail.from = content;
                    tmpMails.insert({ id, tempMail });
                }
            }
        }
        for (const auto &elem : tmpMails)
            if (!elem.second.to.empty())
                for (const auto &toElem : elem.second.to) {
                    m_mails.insert({toElem.first, CMail(toElem.first, elem.second.from, toElem.second, elem.second.subject)});
                }
        return parsedCount;
    }

    std::list<CMail> listMail(const CTimeStamp &from, const CTimeStamp &to) const {
        std::list<CMail> tmpList;
        auto it = m_mails.lower_bound(from);
        if (it != m_mails.end()) {
            while (it->first.compare(from) >= 0 && it->first.compare(to) <= 0) {
                tmpList.push_back(it->second);
                ++it;
            }
        }
        return tmpList;
    }

    std::set<std::string> activeUsers(const CTimeStamp &from, const CTimeStamp &to) const {
        std::set<std::string> tmpSet;
        auto it = m_mails.lower_bound(from);
        if (it != m_mails.end()) {
            while (it->first.compare(from) >= 0 && it->first.compare(to) <= 0) {
                tmpSet.insert(it->second.from());
                tmpSet.insert(it->second.to());
                ++it;
            }
        }
        return tmpSet;
    }

    void print() {
        for (const auto &elem : m_mails) {
            std::cout << elem.second << std::endl;
        }
    }

private:
    std::multimap<CTimeStamp, CMail, CompareTime> m_mails;
    const std::map<std::string, int> month_map = {{"Jan", 1}, {"Feb", 2}, {"Mar", 3}, {"Apr", 4}, {"May", 5}, {"Jun", 6}, {"Jul", 7}, {"Aug", 8}, {"Sep", 9}, {"Oct", 10}, {"Nov", 11}, {"Dec", 12}};
};
//----------------------------------------------------------------------------------------
#ifndef __PROGTEST__
} // namespace
std::string printMail(const std::list<CMail> &all) {
    std::ostringstream oss;
    for (const auto &mail: all)
        oss << mail << "\n";
    return oss.str();
}

int main() {
    MysteriousNamespace::CMailLog m;
    std::list<CMail> mailList;
    std::set<std::string> users;
    std::istringstream iss;

    iss.clear();
    iss.str(
        "Mar 29 2025 12:35:32.233 relay.fit.cvut.cz ADFger72343D: from=user1@fit.cvut.cz\n"
        "Mar 29 2025 12:37:16.234 relay.fit.cvut.cz JlMSRW4232Df: from=person3@fit.cvut.cz\n"
        "Mar 29 2025 12:55:13.023 relay.fit.cvut.cz JlMSRW4232Df: subject=New progtest homework!\n"
        "Mar 29 2025 13:38:45.043 relay.fit.cvut.cz Kbced342sdgA: from=office13@fit.cvut.cz\n"
        "Mar 29 2025 13:36:13.023 relay.fit.cvut.cz JlMSRW4232Df: to=user76@fit.cvut.cz\n"
        "Mar 29 2025 13:55:31.456 relay.fit.cvut.cz KhdfEjkl247D: from=PR-department@fit.cvut.cz\n"
        "Mar 29 2025 14:18:12.654 relay.fit.cvut.cz Kbced342sdgA: to=boss13@fit.cvut.cz\n"
        "Mar 29 2025 14:48:32.563 relay.fit.cvut.cz KhdfEjkl247D: subject=Business partner\n"
        "Mar 29 2025 14:58:32.000 relay.fit.cvut.cz KhdfEjkl247D: to=HR-department@fit.cvut.cz\n"
        "Mar 29 2025 14:25:23.233 relay.fit.cvut.cz ADFger72343D: mail undeliverable\n"
        "Mar 29 2025 15:02:34.231 relay.fit.cvut.cz KhdfEjkl247D: to=CIO@fit.cvut.cz\n"
        "Mar 29 2025 15:02:34.230 relay.fit.cvut.cz KhdfEjkl247D: to=CEO@fit.cvut.cz\n"
        "Mar 29 2025 15:02:34.230 relay.fit.cvut.cz KhdfEjkl247D: to=dean@fit.cvut.cz\n"
        "Mar 29 2025 15:02:34.230 relay.fit.cvut.cz KhdfEjkl247D: to=vice-dean@fit.cvut.cz\n"
        "Mar 29 2025 15:02:34.230 relay.fit.cvut.cz KhdfEjkl247D: to=archive@fit.cvut.cz\n");
    assert(m . parseLog ( iss ) == 8);
    mailList = m.listMail(CTimeStamp(2025, 3, 28, 0, 0, 0),
                          CTimeStamp(2025, 3, 29, 23, 59, 59));
    assert(
        printMail ( mailList ) ==
        R"###(2025-03-29 13:36:13.023 person3@fit.cvut.cz -> user76@fit.cvut.cz, subject: New progtest homework!
2025-03-29 14:18:12.654 office13@fit.cvut.cz -> boss13@fit.cvut.cz
2025-03-29 14:58:32.000 PR-department@fit.cvut.cz -> HR-department@fit.cvut.cz, subject: Business partner
2025-03-29 15:02:34.230 PR-department@fit.cvut.cz -> CEO@fit.cvut.cz, subject: Business partner
2025-03-29 15:02:34.230 PR-department@fit.cvut.cz -> dean@fit.cvut.cz, subject: Business partner
2025-03-29 15:02:34.230 PR-department@fit.cvut.cz -> vice-dean@fit.cvut.cz, subject: Business partner
2025-03-29 15:02:34.230 PR-department@fit.cvut.cz -> archive@fit.cvut.cz, subject: Business partner
2025-03-29 15:02:34.231 PR-department@fit.cvut.cz -> CIO@fit.cvut.cz, subject: Business partner
)###");
    mailList = m.listMail(CTimeStamp(2025, 3, 28, 0, 0, 0),
                          CTimeStamp(2025, 3, 29, 14, 58, 32));
    assert(
        printMail ( mailList ) ==
        R"###(2025-03-29 13:36:13.023 person3@fit.cvut.cz -> user76@fit.cvut.cz, subject: New progtest homework!
2025-03-29 14:18:12.654 office13@fit.cvut.cz -> boss13@fit.cvut.cz
2025-03-29 14:58:32.000 PR-department@fit.cvut.cz -> HR-department@fit.cvut.cz, subject: Business partner
)###");
    mailList = m.listMail(CTimeStamp(2025, 3, 30, 0, 0, 0),
                          CTimeStamp(2025, 3, 30, 23, 59, 59));
    assert(printMail ( mailList ) == "");
    users = m.activeUsers(CTimeStamp(2025, 3, 28, 0, 0, 0),
                          CTimeStamp(2025, 3, 29, 23, 59, 59));
    assert(
        users == std::set<std::string>( { "CEO@fit.cvut.cz", "CIO@fit.cvut.cz", "HR-department@fit.cvut.cz",
            "PR-department@fit.cvut.cz", "archive@fit.cvut.cz", "boss13@fit.cvut.cz", "dean@fit.cvut.cz",
            "office13@fit.cvut.cz", "person3@fit.cvut.cz", "user76@fit.cvut.cz", "vice-dean@fit.cvut.cz" } ));
    users = m.activeUsers(CTimeStamp(2025, 3, 28, 0, 0, 0),
                          CTimeStamp(2025, 3, 29, 13, 59, 59));
    assert(users == std::set<std::string>( { "person3@fit.cvut.cz", "user76@fit.cvut.cz" } ));
    return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
