#ifndef __PROGTEST__
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cassert>
#include <cstring>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <memory>
#include <compare>
#include <functional>
#include <stdexcept>
#include <algorithm>
#endif /* __PROGTEST__ */

class CPerson {
public:
    CPerson(): m_Account(""), m_Name(""), m_Address(""), m_SumIncome(0), m_SumExpense(0) {}
    CPerson(const std::string& name, const std::string& addr, const std::string& account): m_Account(account), m_Name(name), m_Address(addr), m_SumIncome(0), m_SumExpense(0) {}
    static bool less(const CPerson& a, const CPerson& b) {
        if (a.m_Name == b.m_Name) return a.m_Address < b.m_Address;
        return a.m_Name < b.m_Name;
    }
    bool isEqual(const CPerson& other) const {
        return ((m_Name == other.m_Name && m_Address == other.m_Address) || m_Account == other.m_Account);
    }
    bool isNameAddressEqual(const CPerson& other) const {
        return (m_Name == other.m_Name && m_Address == other.m_Address);
    }
    bool isAccountEqual(const CPerson& other) const {
        return (m_Account == other.m_Account);
    }

    std::string m_Account, m_Name, m_Address;
    int m_SumIncome, m_SumExpense;
private:
};

class CIterator {
public:
    CIterator(const std::vector<CPerson> &list): pos(0), m_PersonsList(list) {}
    bool atEnd() const {
        return pos >= m_PersonsList.size();
    }
    void next() {
        pos++;
    }
    const std::string& name() const { return m_PersonsList[pos].m_Name; }
    const std::string& addr() const { return m_PersonsList[pos].m_Address; }
    const std::string& account() const { return m_PersonsList[pos].m_Account; }
private:
    size_t pos;
    const std::vector<CPerson> &m_PersonsList;
};

class CTaxRegister {
public:
    CTaxRegister() {}
    ~CTaxRegister() = default;
    bool birth( const std::string& name, const std::string& addr, const std::string& account ) {
        CPerson person(name, addr, account);
        auto personInfo = std::upper_bound(m_PersonsList.begin(), m_PersonsList.end(), person, CPerson::less);
        if (!m_PersonsList.empty() && personInfo != m_PersonsList.begin() && (personInfo-1)->isEqual(person)) return false;
        m_PersonsList.insert(personInfo, person);
        return true;
    }
    bool death( const std::string& name, const std::string& addr ) {
        CPerson person(name, addr, "");
        auto personInfo = std::upper_bound(m_PersonsList.begin(), m_PersonsList.end(), person, CPerson::less);
        if (m_PersonsList.empty() || personInfo == m_PersonsList.begin() || !((personInfo-1)->isNameAddressEqual(person))) return false;
        m_PersonsList.erase(personInfo-1);
        return true;
    }
    bool income( const std::string& account, int amount ) {
        for (size_t i = 0; i < m_PersonsList.size(); i++) {
            if (m_PersonsList[i].m_Account == account) {
                m_PersonsList[i].m_SumIncome += amount;
                return true;
            }
        }
        return false;
    }
    bool income( const std::string& name, const std::string& addr, int amount ) {
        CPerson person(name, addr, "");
        auto personInfo = std::upper_bound(m_PersonsList.begin(), m_PersonsList.end(), person, CPerson::less);
        if (m_PersonsList.empty() || personInfo == m_PersonsList.begin() || !((personInfo-1)->isNameAddressEqual(person))) return false;
        (personInfo-1)->m_SumIncome += amount;
        return true;
    }
    bool expense( const std::string& account, int amount ) {
        for (size_t i = 0; i < m_PersonsList.size(); i++) {
            if (m_PersonsList[i].m_Account == account) {
                m_PersonsList[i].m_SumExpense += amount;
                return true;
            }
        }
        return false;
    }
    bool expense( const std::string& name, const std::string& addr, int amount ) {
        CPerson person(name, addr, "");
        auto personInfo = std::upper_bound(m_PersonsList.begin(), m_PersonsList.end(), person, CPerson::less);
        if (m_PersonsList.empty() || personInfo == m_PersonsList.begin() || !((personInfo-1)->isNameAddressEqual(person))) return false;
        (personInfo-1)->m_SumExpense += amount;
        return true;
    }
    bool audit( const std::string    & name,
                           const std::string    & addr,
                           std::string          & account,
                           int                  & sumIncome,
                           int                  & sumExpense ) const {
        CPerson person(name, addr, "");
        auto personInfo = std::upper_bound(m_PersonsList.begin(), m_PersonsList.end(), person, CPerson::less);
        if (m_PersonsList.empty() || personInfo == m_PersonsList.begin() || !((personInfo-1)->isNameAddressEqual(person))) return false;
        account = (personInfo-1)->m_Account;
        sumIncome = (personInfo-1)->m_SumIncome;
        sumExpense = (personInfo-1)->m_SumExpense;
        return true;
    }
    CIterator listByName() const {
        return CIterator(m_PersonsList);
    }
    void print() const {
        for (size_t i = 0; i < m_PersonsList.size(); i++) {
            std::cout << m_PersonsList[i].m_Name << " " << m_PersonsList[i].m_Address << " " << m_PersonsList[i].m_Account << std::endl;
        }
    }
private:
    std::vector<CPerson> m_PersonsList;
};

#ifndef __PROGTEST__
int main () {
    std::string acct;
    int    sumIncome, sumExpense;
    CTaxRegister b0;
    assert ( b0 . birth ( "John Smith", "Oak Road 23", "123/456/789" ) );
    assert ( b0 . birth ( "Jane Hacker", "Main Street 17", "Xuj5#94" ) );
    assert ( b0 . birth ( "Peter Hacker", "Main Street 17", "634oddT" ) );
    assert ( b0 . birth ( "John Smith", "Main Street 17", "Z343rwZ" ) );
    assert ( b0 . income ( "Xuj5#94", 1000 ) );
    assert ( b0 . income ( "634oddT", 2000 ) );
    assert ( b0 . income ( "123/456/789", 3000 ) );
    assert ( b0 . income ( "634oddT", 4000 ) );
    assert ( b0 . income ( "Peter Hacker", "Main Street 17", 2000 ) );
    assert ( b0 . expense ( "Jane Hacker", "Main Street 17", 2000 ) );
    assert ( b0 . expense ( "John Smith", "Main Street 17", 500 ) );
    assert ( b0 . expense ( "Jane Hacker", "Main Street 17", 1000 ) );
    assert ( b0 . expense ( "Xuj5#94", 1300 ) );
    assert ( b0 . audit ( "John Smith", "Oak Road 23", acct, sumIncome, sumExpense ) );
    assert ( acct == "123/456/789" );
    assert ( sumIncome == 3000 );
    assert ( sumExpense == 0 );
    assert ( b0 . audit ( "Jane Hacker", "Main Street 17", acct, sumIncome, sumExpense ) );
    assert ( acct == "Xuj5#94" );
    assert ( sumIncome == 1000 );
    assert ( sumExpense == 4300 );
    assert ( b0 . audit ( "Peter Hacker", "Main Street 17", acct, sumIncome, sumExpense ) );
    assert ( acct == "634oddT" );
    assert ( sumIncome == 8000 );
    assert ( sumExpense == 0 );
    assert ( b0 . audit ( "John Smith", "Main Street 17", acct, sumIncome, sumExpense ) );
    assert ( acct == "Z343rwZ" );
    assert ( sumIncome == 0 );
    assert ( sumExpense == 500 );
    CIterator it = b0 . listByName ();
    assert ( ! it . atEnd ()
             && it . name () == "Jane Hacker"
             && it . addr () == "Main Street 17"
             && it . account () == "Xuj5#94" );
    it . next ();
    assert ( ! it . atEnd ()
             && it . name () == "John Smith"
             && it . addr () == "Main Street 17"
             && it . account () == "Z343rwZ" );
    it . next ();
    assert ( ! it . atEnd ()
             && it . name () == "John Smith"
             && it . addr () == "Oak Road 23"
             && it . account () == "123/456/789" );
    it . next ();
    assert ( ! it . atEnd ()
             && it . name () == "Peter Hacker"
             && it . addr () == "Main Street 17"
             && it . account () == "634oddT" );
    it . next ();
    assert ( it . atEnd () );

    assert ( b0 . death ( "John Smith", "Main Street 17" ) );

    CTaxRegister b1;
    assert ( b1 . birth ( "John Smith", "Oak Road 23", "123/456/789" ) );
    assert ( b1 . birth ( "Jane Hacker", "Main Street 17", "Xuj5#94" ) );
    assert ( !b1 . income ( "634oddT", 4000 ) );
    assert ( !b1 . expense ( "John Smith", "Main Street 18", 500 ) );
    assert ( !b1 . audit ( "John Nowak", "Second Street 23", acct, sumIncome, sumExpense ) );
    assert ( !b1 . death ( "Peter Nowak", "5-th Avenue" ) );
    assert ( !b1 . birth ( "Jane Hacker", "Main Street 17", "4et689A" ) );
    assert ( !b1 . birth ( "Joe Hacker", "Elm Street 23", "Xuj5#94" ) );
    assert ( b1 . death ( "Jane Hacker", "Main Street 17" ) );
    assert ( b1 . birth ( "Joe Hacker", "Elm Street 23", "Xuj5#94" ) );
    assert ( b1 . audit ( "Joe Hacker", "Elm Street 23", acct, sumIncome, sumExpense ) );
    assert ( acct == "Xuj5#94" );
    assert ( sumIncome == 0 );
    assert ( sumExpense == 0 );
    assert ( !b1 . birth ( "Joe Hacker", "Elm Street 23", "AAj5#94" ) );

    return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
