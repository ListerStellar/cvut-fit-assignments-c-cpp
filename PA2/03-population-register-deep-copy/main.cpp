#ifndef __PROGTEST__
#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <cstring>
#include <iostream>
#include <sstream>
#endif /* __PROGTEST__ */
//#include <bits/fs_path.h>

class CString {
public:
    CString(const char* str = "") {
        m_string = new char[strlen(str)+1];
        strcpy(m_string, str);
    }
    CString(const char* str1, const char* str2) {
        m_string = new char[strlen(str1)+strlen(str2)+2];
        strcpy(m_string, str1);
        strcat(m_string, " ");
        strcat(m_string, str2);
    }
    CString(const CString& str) {
        m_string = new char[strlen(str.m_string)+1];
        strcpy(m_string, str.m_string);
    }
    CString& operator=(const CString& str) {
        if (this != &str) {
            delete [] m_string;
            m_string = new char[strlen(str.m_string)+1];
            strcpy(m_string, str.m_string);
        }
        return *this;
    }
    ~CString() {
        delete[] m_string;
        m_string = nullptr;
    }
    const char* str() const {
        return m_string;
    }
    const size_t size() const {
        return strlen(m_string);
    }
private:
    char *m_string;
};

class CPerson {
public:
    CPerson(): m_id(), m_name(), m_dates(new CString[1]), m_addresses(new CString[0]), m_settles_size(0), m_settles_capacity(1) {}
    CPerson(const char *id,
            const char *name,
            const char *surname,
            const char *date,
            const char *street,
            const char *city):  m_id(id),
                                m_name(name, surname),
                                m_dates(new CString[4]),
                                m_addresses(new CString[4]),
                                m_settles_size(1),
                                m_settles_capacity(4) {
        m_dates[0] = CString(date);
        m_addresses[0] = CString(street, city);
    }
    CPerson(const CPerson& person) {
        m_id = person.m_id;
        m_name = person.m_name;
        m_dates = new CString[person.m_dates->size()];
        m_addresses = new CString[person.m_addresses->size()];
        m_settles_size = person.m_settles_size;
        m_settles_capacity = person.m_settles_capacity;
        for (size_t i = 0; i < m_settles_size; i++) {
            m_dates[i] = person.m_dates[i];
            m_addresses[i] = person.m_addresses[i];
        }
    }
    CPerson& operator=(const CPerson& person) {
        if (this != &person) {
            delete [] m_dates;
            delete [] m_addresses;
            m_id = person.m_id;
            m_name = person.m_name;
            m_dates = new CString[person.m_dates->size()];
            m_addresses = new CString[person.m_addresses->size()];
            m_settles_size = person.m_settles_size;
            m_settles_capacity = person.m_settles_capacity;
            for (size_t i = 0; i < m_settles_size; i++) {
                m_dates[i] = person.m_dates[i];
                m_addresses[i] = person.m_addresses[i];
            }
        }
        return *this;
    }
    ~CPerson() {
        delete [] m_dates;
        delete [] m_addresses;
        m_dates = nullptr; m_addresses = nullptr;
        m_settles_size = 0; m_settles_capacity = 0;
    }
    bool add_settle(const char *date, const char *street, const char *city) {
        for (size_t i = 0; i < m_settles_size; i++)
            if (strcmp(m_dates[i].str(), date) == 0) return false;
        size_t index = 0;
        while (index < m_settles_size && strcmp(m_dates[index].str(), date) <= 0) index++;
        if (m_settles_size >= m_settles_capacity) {
            m_settles_capacity *= 2;
            CString* new_dates = new CString[m_settles_capacity];
            CString* new_addresses = new CString[m_settles_capacity];
            for (size_t i = 0; i < index; i++) {
                new_dates[i] = m_dates[i];
                new_addresses[i] = m_addresses[i];
            }
            delete[] m_dates;
            delete[] m_addresses;
            m_dates = new_dates;
            m_addresses = new_addresses;
        }
        if (index != m_settles_size) {
            for (size_t i = m_settles_size; i > index; i--) {
                m_dates[i] = m_dates[i-1];
                m_addresses[i] = m_addresses[i-1];
            }
        }
        m_dates[index] = CString(date);
        m_addresses[index] = CString(street, city);
        m_settles_size++;
        return true;
    }
    const char* id() const {
        return m_id.str();
    }
    void print(std::ostream &os) const {
        os << m_id.str() << " " << m_name.str() << std::endl;
        for (size_t i = 0; i < m_settles_size; i++)
            os << m_dates[i].str() << " " << m_addresses[i].str() << std::endl;
    }
private:
    CString m_id, m_name;
    CString *m_dates, *m_addresses;
    size_t m_settles_size, m_settles_capacity;
};

class CRegister {
public:
    // default constructor
    CRegister(): m_persons(new CPerson[4]), m_persons_size(0), m_persons_capacity(4) {}
    // copy constructor
    CRegister(const CRegister& reg) {
        m_persons_size = reg.m_persons_size;
        m_persons_capacity = reg.m_persons_capacity;
        m_persons = new CPerson[m_persons_capacity];
        for (size_t i = 0; i < m_persons_size; i++) {
            m_persons[i] = reg.m_persons[i];
        }
    }
    // destructor
    ~CRegister() {
        delete [] m_persons;
        m_persons = nullptr;
        m_persons_size = 0; m_persons_capacity = 0;
    }
    // operator =
    CRegister& operator=(const CRegister& reg) {
        if (this != &reg) {
            delete [] m_persons;
            m_persons_size = reg.m_persons_size;
            m_persons_capacity = reg.m_persons_capacity;
            m_persons = new CPerson[m_persons_capacity];
            for (size_t i = 0; i < m_persons_size; i++) {
                m_persons[i] = reg.m_persons[i];
            }
        }
        return *this;
    }
    bool add(const char id[],
             const char name[],
             const char surname[],
             const char date[],
             const char street[],
             const char city[]) {
        for (size_t i = 0; i < m_persons_size; i++)
            if (strcmp(m_persons[i].id(), id) == 0) return false;
        if (m_persons_size >= m_persons_capacity) {
            m_persons_capacity *= 2;
            CPerson *new_persons = new CPerson[m_persons_capacity];
            for (size_t i = 0; i < m_persons_size; i++) {
                new_persons[i] = m_persons[i];
            }
            delete[] m_persons;
            m_persons = new_persons;
        }
        m_persons[m_persons_size++] = CPerson(id, name, surname, date, street, city);
        return true;
    }

    bool resettle(const char id[],
                  const char date[],
                  const char street[],
                  const char city[]) {
        for (size_t i = 0; i < m_persons_size; i++)
            if (strcmp(m_persons[i].id(), id) == 0)
                return m_persons[i].add_settle(date, street, city);
        return false;
    }

    bool print(std::ostream &os,
               const char id[]) const {
        for (size_t i = 0; i < m_persons_size; i++)
            if (strcmp(m_persons[i].id(), id) == 0) {
                m_persons[i].print(os);
                return true;
            }
        return false;
    }

    void print_all(std::ostream &os) const {
        for (size_t i = 0; i < m_persons_size; i++)
            m_persons[i].print(os);
    }

private:
    CPerson *m_persons;
    size_t m_persons_size, m_persons_capacity;
};

#ifndef __PROGTEST__
int main() {
    char lID[12], lDate[12], lName[50], lSurname[50], lStreet[50], lCity[50];
    std::ostringstream oss;
    CRegister a;
    assert(a . add ( "123456/7890", "John", "Smith", "2000-01-01", "Main street", "Seattle" ) == true);
    assert(a . add ( "987654/3210", "Freddy", "Kruger", "2001-02-03", "Elm street", "Sacramento" ) == true);
    assert(a . resettle ( "123456/7890", "2003-05-12", "Elm street", "Atlanta" ) == true);
    assert(a . resettle ( "123456/7890", "2002-12-05", "Sunset boulevard", "Los Angeles" ) == true);
    oss.str("");
    assert(a . print ( oss, "123456/7890" ) == true);
    assert(! strcmp ( oss . str () . c_str (), R"###(123456/7890 John Smith
2000-01-01 Main street Seattle
2002-12-05 Sunset boulevard Los Angeles
2003-05-12 Elm street Atlanta
)###" ));
    oss.str("");
    assert(a . print ( oss, "987654/3210" ) == true);
    assert(! strcmp ( oss . str () . c_str (), R"###(987654/3210 Freddy Kruger
2001-02-03 Elm street Sacramento
)###" ));
    CRegister b(a);
    assert(b . resettle ( "987654/3210", "2008-04-12", "Elm street", "Cinccinati" ) == true);
    assert(a . resettle ( "987654/3210", "2007-02-11", "Elm street", "Indianapolis" ) == true);
    oss.str("");
    assert(a . print ( oss, "987654/3210" ) == true);
    assert(! strcmp ( oss . str () . c_str (), R"###(987654/3210 Freddy Kruger
2001-02-03 Elm street Sacramento
2007-02-11 Elm street Indianapolis
)###" ));
    oss.str("");
    assert(b . print ( oss, "987654/3210" ) == true);
    assert(! strcmp ( oss . str () . c_str (), R"###(987654/3210 Freddy Kruger
2001-02-03 Elm street Sacramento
2008-04-12 Elm street Cinccinati
)###" ));
    a = b;
    assert(a . resettle ( "987654/3210", "2011-05-05", "Elm street", "Salt Lake City" ) == true);
    oss.str("");
    assert(a . print ( oss, "987654/3210" ) == true);
    assert(! strcmp ( oss . str () . c_str (), R"###(987654/3210 Freddy Kruger
2001-02-03 Elm street Sacramento
2008-04-12 Elm street Cinccinati
2011-05-05 Elm street Salt Lake City
)###" ));
    oss.str("");
    assert(b . print ( oss, "987654/3210" ) == true);
    assert(! strcmp ( oss . str () . c_str (), R"###(987654/3210 Freddy Kruger
2001-02-03 Elm street Sacramento
2008-04-12 Elm street Cinccinati
)###" ));
    assert(b . add ( "987654/3210", "Joe", "Lee", "2010-03-17", "Abbey road", "London" ) == false);
    assert(a . resettle ( "987654/3210", "2001-02-03", "Second street", "Milwaukee" ) == false);
    oss.str("");
    assert(a . print ( oss, "666666/6666" ) == false);

    CRegister c;
    strncpy(lID, "123456/7890", sizeof (lID));
    strncpy(lName, "John", sizeof (lName));
    strncpy(lSurname, "Smith", sizeof (lSurname));
    strncpy(lDate, "2000-01-01", sizeof (lDate));
    strncpy(lStreet, "Main street", sizeof (lStreet));
    strncpy(lCity, "Seattle", sizeof (lCity));
    assert(c . add ( lID, lName, lSurname, lDate, lStreet, lCity ) == true);
    strncpy(lID, "987654/3210", sizeof (lID));
    strncpy(lName, "Freddy", sizeof (lName));
    strncpy(lSurname, "Kruger", sizeof (lSurname));
    strncpy(lDate, "2001-02-03", sizeof (lDate));
    strncpy(lStreet, "Elm street", sizeof (lStreet));
    strncpy(lCity, "Sacramento", sizeof (lCity));
    assert(c . add ( lID, lName, lSurname, lDate, lStreet, lCity ) == true);
    strncpy(lID, "123456/7890", sizeof (lID));
    strncpy(lDate, "2003-05-12", sizeof (lDate));
    strncpy(lStreet, "Elm street", sizeof (lStreet));
    strncpy(lCity, "Atlanta", sizeof (lCity));
    assert(c . resettle ( lID, lDate, lStreet, lCity ) == true);
    strncpy(lID, "123456/7890", sizeof (lID));
    strncpy(lDate, "2002-12-05", sizeof (lDate));
    strncpy(lStreet, "Sunset boulevard", sizeof (lStreet));
    strncpy(lCity, "Los Angeles", sizeof (lCity));
    assert(c . resettle ( lID, lDate, lStreet, lCity ) == true);
    oss.str("");
    assert(c . print ( oss, "123456/7890" ) == true);
    assert(! strcmp ( oss . str () . c_str (), R"###(123456/7890 John Smith
2000-01-01 Main street Seattle
2002-12-05 Sunset boulevard Los Angeles
2003-05-12 Elm street Atlanta
)###" ));

    return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
