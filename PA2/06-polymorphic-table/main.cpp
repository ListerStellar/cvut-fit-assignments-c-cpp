#ifndef __PROGTEST__
#include <cstring>
#include <cassert>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cctype>
#include <cmath>
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
#include <functional>
#include <stdexcept>
#endif /* __PROGTEST__ */

// Abstract base class representing any content that can be placed into a table cell.
class CContent {
public:

    // Virtual destructor for safe polymorphic deletion.
    virtual ~CContent() = default;

    // Pure virtual equality operator - checks if two contents are the same.
    virtual bool operator==(const CContent& second) const = 0;

    // Inequality operator implemented via equality.
    bool operator!=(const CContent& second) const {
        return !(*this == second);
    }

    // Pure virtual function to generate a printable representation of the content given a (height, width).
    virtual std::vector<std::string> print(std::pair<int,int> size) const = 0;

    // Creates and returns a shared copy of the content.
    virtual std::shared_ptr<CContent> share() const = 0;

    // Returns the dimensions of the content (height, width).
    virtual std::pair<int,int> getSize() const = 0;

};

// Concrete class representing text content, with optional left/right alignment.
class CText : public CContent {
public:

    // Enumeration for alignment type (left or right).
    enum EAlign {
        ALIGN_LEFT, ALIGN_RIGHT
    };

    // Constructor that splits input text by \n.
    CText(const std::string &str, EAlign al): align(al) {
        std::istringstream iss(str);
        std::string s;
        while (std::getline(iss, s)) {
            data.emplace_back(s);
        }
    }

    // Creates a deep copy of the text content.
    std::shared_ptr<CContent> share() const override {
        return std::make_shared<CText>(*this);
    }

    // Sets the text content.
    void setText(const std::string &str) {
        data.clear();
        std::istringstream iss(str);
        std::string s;
        while (std::getline(iss, s)) {
            data.emplace_back(s);
        }
    }

    // Checks for equality between two CText objects.
    bool operator==(const CContent& second) const override {
        if (typeid(*this) != typeid(second)) return false;
        return data == dynamic_cast<const CText&>(second).data && align == dynamic_cast<const CText&>(second).align;
    }

    // Formats the text to fit within a given cell size and applies alignment.
    std::vector<std::string> print(std::pair<int,int> size) const override {
        std::vector<std::string> res;
        for (int i = 0; i < size.first; i++) {
            std::string s = (i < static_cast<int>(data.size())) ? data.at(i) : "";
            if (align == ALIGN_LEFT) {
                res.emplace_back(s + std::string(size.second - s.length(), ' '));
            } else {
                res.emplace_back(std::string(size.second - s.length(), ' ') + s);
            }
        }
        return res;
    }

    // Calculates the number of lines and maximum line width.
    std::pair<int,int> getSize() const override {
        int maxWidth = 0;
        for (const auto &s : data) {
            maxWidth = std::max(maxWidth, static_cast<int>(s.size()));
        }
        return {static_cast<int>(data.size()), maxWidth};
    }

private:
    std::vector<std::string> data; // Holds the text lines.
    EAlign align; // Alignment setting.
};

// Class representing an empty content cell. Fills space with blanks.
class CEmpty : public CContent {
public:

    CEmpty() = default;

    // Returns a shared copy of itself.
    std::shared_ptr<CContent> share() const override {
        return std::make_shared<CEmpty>(*this);
    }

    // Two CEmpty objects are always equal.
    bool operator==(const CContent& second) const override {
        return typeid(*this) == typeid(second);
    }

    // Prints an empty (blank) space of given size.
    std::vector<std::string> print(std::pair<int,int> size) const override {
        std::vector<std::string> res;
        for (int i = 0; i < size.first; i++) {
            res.emplace_back(size.second, ' ');
        }
        return res;
    }

    // Empty content reports (0, 0) size.
    std::pair<int,int> getSize() const override {
        return {0, 0};
    }

};

// Class to handle image content represented as lines of strings.
class CImage : public CContent {
public:

    CImage(): size({0, 0}) {}

    std::shared_ptr<CContent> share() const override {
        return std::make_shared<CImage>(*this);
    }

    // Adds a new line to the image.
    CImage &addRow(const std::string &str) {
        if (data.empty()) {
            size.second = static_cast<int>(str.size());
        }
        data.emplace_back(str);
        ++size.first;
        return *this;
    }

    // Compares two images.
    bool operator==(const CContent& second) const override {
        if (typeid(*this) != typeid(second)) return false;
        return data == dynamic_cast<const CImage&>(second).data;
    }

    // Centers the image inside a given area size.
    std::vector<std::string> print(std::pair<int,int> size) const override {
        std::vector<std::string> res;
        for (int i = 0; i < size.first; i++) {
            res.emplace_back(size.second, ' ');
        }
        std::pair<int, int> imageSize = {static_cast<int>(data.size()), data.empty() ? 0 : static_cast<int>(data[0].size())};
        std::pair<int, int> imagePos = {(size.first - imageSize.first) / 2, (size.second - imageSize.second) / 2};
        for (int i = 0; i < imageSize.first; ++i) {
            for (int j = 0; j < imageSize.second; ++j) {
                if ((imagePos.first + i) < size.first && (imagePos.second + j) < size.second) {
                    res.at(imagePos.first + i).at(imagePos.second + j) = data.at(i).at(j);
                }
            }
        }
        return res;
    }

    // Returns the dimensions of the image (height, width).
    std::pair<int,int> getSize() const override {
        return size;
    }

private:
    std::vector<std::string> data; // Image lines.
    std::pair<int,int> size; // Image dimensions.
};

// Main class representing a table consisting of cells with shared content (CContent).
class CTable {
public:

    // Constructor initializes the table with a specified number of rows and columns, filled with empty cells.
    CTable(int rows, int cols): size({rows, cols}) {
        for (int i = 0; i < rows; i++) {
            data.emplace_back();
            for (int j = 0; j < cols; j++) {
                data.at(i).emplace_back(CEmpty().share());
            }
        }
    }

    // Deep copy constructor.
    CTable(const CTable &other): size(other.size) {
        for (int i = 0; i < size.first; i++) {
            data.emplace_back();
            for (int j = 0; j < size.second; j++) {
                data.at(i).emplace_back(other.data.at(i).at(j)->share());
            }
        }
    }

    // Deep assignment operator.
    CTable &operator=(const CTable &other) {
        if (this == &other) return *this;
        size = other.size;
        data.clear();
        for (int i = 0; i < size.first; i++) {
            data.emplace_back();
            for (int j = 0; j < size.second; j++) {
                data.at(i).emplace_back(other.data.at(i).at(j)->share());
            }
        }
        return *this;
    }

    // Access cell content by (row, column).
    CContent &getCell(int row, int col) const {
        return *data.at(row).at(col);
    }

    // Set a cell content to a given content.
    void setCell(int row, int col, const CContent &content) {
        data.at(row).at(col) = content.share();
    }

    // Special case: set a cell to contain a nested table.
    void setCell(int row, int col, const CTable &table) {
        std::ostringstream oss;
        oss << table;
        data.at(row).at(col) = CText(oss.str(), CText::ALIGN_LEFT).share();
    }

    // Equality and inequality operators for tables.
    bool operator==(const CTable& second) const {
        if (size != second.size) return false;
        for (int i = 0; i < size.first; i++) {
            for (int j = 0; j < size.second; j++) {
                if (data.at(i).at(j) != second.data.at(i).at(j)) return false;
            }
        }
        return true;
    }
    bool operator!=(const CTable& second) const {
        return !(*this == second);
    }

    // Stream output of the table: Calculates per-row height and per-column width, renders table with border characters (+, -, |) and the content inside.
    friend std::ostream &operator<<(std::ostream &os, const CTable &table) {
        std::vector<int> rowHeight(table.size.first, 0);
        std::vector<int> columnWidth(table.size.second, 0);
        for (int i = 0; i < table.size.first; i++) {
            for (int j = 0; j < table.size.second; j++) {
                rowHeight[i] = std::max(rowHeight[i], table.getCell(i,j).getSize().first);
                columnWidth[j] = std::max(columnWidth[j], table.getCell(i,j).getSize().second);
            }
        }
        for (int i = 0; i < table.size.first; i++) {
            os << "+";
            for (int j = 0; j < table.size.second; j++) {
                os << std::string(columnWidth[j], '-') << '+';
            }
            os << "\n";
            std::vector<std::vector<std::string>> elements;
            for (int j = 0; j < table.size.second; j++) {
                elements.emplace_back(table.getCell(i, j).print({rowHeight[i], columnWidth[j]}));
            }
            for (int h = 0; h < rowHeight[i]; h++) {
                os << "|";
                for (int w = 0; w < table.size.second; w++) {
                    os << elements.at(w).at(h) << "|";
                }
                os << "\n";
            }
        }
        os << "+";
        for (int j = 0; j < table.size.second; j++) {
            os << std::string(columnWidth[j], '-') << '+';
        }
        os << "\n";
        return os;
    }

private:
    std::vector<std::vector<std::shared_ptr<CContent>>> data; // 2D grid of cell contents.
    std::pair<int, int> size; // Table dimensions (rows x columns).
};

#ifndef __PROGTEST__
int main() { // Testing various table constructions and asserts.
    std::ostringstream oss;
    CTable t0 ( 3, 2 );
    t0 . setCell ( 0, 0, CText ( "Hello,\n"
          "Hello Kitty", CText::ALIGN_LEFT ) );
    t0 . setCell ( 1, 0, CText ( "Lorem ipsum dolor sit amet", CText::ALIGN_LEFT ) );
    t0 . setCell ( 2, 0, CText ( "Bye,\n"
          "Hello Kitty", CText::ALIGN_RIGHT ) );
    t0 . setCell ( 1, 1, CImage ()
            . addRow ( "###                   " )
            . addRow ( "#  #                  " )
            . addRow ( "#  # # ##   ###    ###" )
            . addRow ( "###  ##    #   #  #  #" )
            . addRow ( "#    #     #   #  #  #" )
            . addRow ( "#    #     #   #  #  #" )
            . addRow ( "#    #      ###    ###" )
            . addRow ( "                     #" )
            . addRow ( "                   ## " )
            . addRow ( "                      " )
            . addRow ( " #    ###   ###   #   " )
            . addRow ( "###  #   # #     ###  " )
            . addRow ( " #   #####  ###   #   " )
            . addRow ( " #   #         #  #   " )
            . addRow ( "  ##  ###   ###    ## " ) );
    t0 . setCell ( 2, 1, CEmpty () );
    oss . str ("");
    oss . clear ();
    oss << t0;
    assert ( oss . str () ==
          "+--------------------------+----------------------+\n"
          "|Hello,                    |                      |\n"
          "|Hello Kitty               |                      |\n"
          "+--------------------------+----------------------+\n"
          "|Lorem ipsum dolor sit amet|###                   |\n"
          "|                          |#  #                  |\n"
          "|                          |#  # # ##   ###    ###|\n"
          "|                          |###  ##    #   #  #  #|\n"
          "|                          |#    #     #   #  #  #|\n"
          "|                          |#    #     #   #  #  #|\n"
          "|                          |#    #      ###    ###|\n"
          "|                          |                     #|\n"
          "|                          |                   ## |\n"
          "|                          |                      |\n"
          "|                          | #    ###   ###   #   |\n"
          "|                          |###  #   # #     ###  |\n"
          "|                          | #   #####  ###   #   |\n"
          "|                          | #   #         #  #   |\n"
          "|                          |  ##  ###   ###    ## |\n"
          "+--------------------------+----------------------+\n"
          "|                      Bye,|                      |\n"
          "|               Hello Kitty|                      |\n"
          "+--------------------------+----------------------+\n" );
    t0 . setCell ( 0, 1, t0 . getCell ( 1, 1 ) );
    t0 . setCell ( 2, 1, CImage ()
            . addRow ( "*****   *      *  *      ******* ******  *" )
            . addRow ( "*    *  *      *  *      *            *  *" )
            . addRow ( "*    *  *      *  *      *           *   *" )
            . addRow ( "*    *  *      *  *      *****      *    *" )
            . addRow ( "****    *      *  *      *         *     *" )
            . addRow ( "*  *    *      *  *      *        *       " )
            . addRow ( "*   *   *      *  *      *       *       *" )
            . addRow ( "*    *    *****   ****** ******* ******  *" ) );
    dynamic_cast<CText &> ( t0 . getCell ( 1, 0 ) ) . setText ( "Lorem ipsum dolor sit amet,\n"
          "consectetur adipiscing\n"
          "elit. Curabitur scelerisque\n"
          "lorem vitae lectus cursus,\n"
          "vitae porta ante placerat. Class aptent taciti\n"
          "sociosqu ad litora\n"
          "torquent per\n"
          "conubia nostra,\n"
          "per inceptos himenaeos.\n"
          "\n"
          "Donec tincidunt augue\n"
          "sit amet metus\n"
          "pretium volutpat.\n"
          "Donec faucibus,\n"
          "ante sit amet\n"
          "luctus posuere,\n"
          "mauris tellus" );
    oss . str ("");
    oss . clear ();
    oss << t0;
    assert ( oss . str () ==
          "+----------------------------------------------+------------------------------------------+\n"
          "|Hello,                                        |          ###                             |\n"
          "|Hello Kitty                                   |          #  #                            |\n"
          "|                                              |          #  # # ##   ###    ###          |\n"
          "|                                              |          ###  ##    #   #  #  #          |\n"
          "|                                              |          #    #     #   #  #  #          |\n"
          "|                                              |          #    #     #   #  #  #          |\n"
          "|                                              |          #    #      ###    ###          |\n"
          "|                                              |                               #          |\n"
          "|                                              |                             ##           |\n"
          "|                                              |                                          |\n"
          "|                                              |           #    ###   ###   #             |\n"
          "|                                              |          ###  #   # #     ###            |\n"
          "|                                              |           #   #####  ###   #             |\n"
          "|                                              |           #   #         #  #             |\n"
          "|                                              |            ##  ###   ###    ##           |\n"
          "+----------------------------------------------+------------------------------------------+\n"
          "|Lorem ipsum dolor sit amet,                   |                                          |\n"
          "|consectetur adipiscing                        |          ###                             |\n"
          "|elit. Curabitur scelerisque                   |          #  #                            |\n"
          "|lorem vitae lectus cursus,                    |          #  # # ##   ###    ###          |\n"
          "|vitae porta ante placerat. Class aptent taciti|          ###  ##    #   #  #  #          |\n"
          "|sociosqu ad litora                            |          #    #     #   #  #  #          |\n"
          "|torquent per                                  |          #    #     #   #  #  #          |\n"
          "|conubia nostra,                               |          #    #      ###    ###          |\n"
          "|per inceptos himenaeos.                       |                               #          |\n"
          "|                                              |                             ##           |\n"
          "|Donec tincidunt augue                         |                                          |\n"
          "|sit amet metus                                |           #    ###   ###   #             |\n"
          "|pretium volutpat.                             |          ###  #   # #     ###            |\n"
          "|Donec faucibus,                               |           #   #####  ###   #             |\n"
          "|ante sit amet                                 |           #   #         #  #             |\n"
          "|luctus posuere,                               |            ##  ###   ###    ##           |\n"
          "|mauris tellus                                 |                                          |\n"
          "+----------------------------------------------+------------------------------------------+\n"
          "|                                          Bye,|*****   *      *  *      ******* ******  *|\n"
          "|                                   Hello Kitty|*    *  *      *  *      *            *  *|\n"
          "|                                              |*    *  *      *  *      *           *   *|\n"
          "|                                              |*    *  *      *  *      *****      *    *|\n"
          "|                                              |****    *      *  *      *         *     *|\n"
          "|                                              |*  *    *      *  *      *        *       |\n"
          "|                                              |*   *   *      *  *      *       *       *|\n"
          "|                                              |*    *    *****   ****** ******* ******  *|\n"
          "+----------------------------------------------+------------------------------------------+\n" );
    CTable t1 ( t0 );
    t1 . setCell ( 1, 0, CEmpty () );
    t1 . setCell ( 1, 1, CEmpty () );
    oss . str ("");
    oss . clear ();
    oss << t0;
    assert ( oss . str () ==
          "+----------------------------------------------+------------------------------------------+\n"
          "|Hello,                                        |          ###                             |\n"
          "|Hello Kitty                                   |          #  #                            |\n"
          "|                                              |          #  # # ##   ###    ###          |\n"
          "|                                              |          ###  ##    #   #  #  #          |\n"
          "|                                              |          #    #     #   #  #  #          |\n"
          "|                                              |          #    #     #   #  #  #          |\n"
          "|                                              |          #    #      ###    ###          |\n"
          "|                                              |                               #          |\n"
          "|                                              |                             ##           |\n"
          "|                                              |                                          |\n"
          "|                                              |           #    ###   ###   #             |\n"
          "|                                              |          ###  #   # #     ###            |\n"
          "|                                              |           #   #####  ###   #             |\n"
          "|                                              |           #   #         #  #             |\n"
          "|                                              |            ##  ###   ###    ##           |\n"
          "+----------------------------------------------+------------------------------------------+\n"
          "|Lorem ipsum dolor sit amet,                   |                                          |\n"
          "|consectetur adipiscing                        |          ###                             |\n"
          "|elit. Curabitur scelerisque                   |          #  #                            |\n"
          "|lorem vitae lectus cursus,                    |          #  # # ##   ###    ###          |\n"
          "|vitae porta ante placerat. Class aptent taciti|          ###  ##    #   #  #  #          |\n"
          "|sociosqu ad litora                            |          #    #     #   #  #  #          |\n"
          "|torquent per                                  |          #    #     #   #  #  #          |\n"
          "|conubia nostra,                               |          #    #      ###    ###          |\n"
          "|per inceptos himenaeos.                       |                               #          |\n"
          "|                                              |                             ##           |\n"
          "|Donec tincidunt augue                         |                                          |\n"
          "|sit amet metus                                |           #    ###   ###   #             |\n"
          "|pretium volutpat.                             |          ###  #   # #     ###            |\n"
          "|Donec faucibus,                               |           #   #####  ###   #             |\n"
          "|ante sit amet                                 |           #   #         #  #             |\n"
          "|luctus posuere,                               |            ##  ###   ###    ##           |\n"
          "|mauris tellus                                 |                                          |\n"
          "+----------------------------------------------+------------------------------------------+\n"
          "|                                          Bye,|*****   *      *  *      ******* ******  *|\n"
          "|                                   Hello Kitty|*    *  *      *  *      *            *  *|\n"
          "|                                              |*    *  *      *  *      *           *   *|\n"
          "|                                              |*    *  *      *  *      *****      *    *|\n"
          "|                                              |****    *      *  *      *         *     *|\n"
          "|                                              |*  *    *      *  *      *        *       |\n"
          "|                                              |*   *   *      *  *      *       *       *|\n"
          "|                                              |*    *    *****   ****** ******* ******  *|\n"
          "+----------------------------------------------+------------------------------------------+\n" );
    oss . str ("");
    oss . clear ();
    oss << t1;
    assert ( oss . str () ==
          "+-----------+------------------------------------------+\n"
          "|Hello,     |          ###                             |\n"
          "|Hello Kitty|          #  #                            |\n"
          "|           |          #  # # ##   ###    ###          |\n"
          "|           |          ###  ##    #   #  #  #          |\n"
          "|           |          #    #     #   #  #  #          |\n"
          "|           |          #    #     #   #  #  #          |\n"
          "|           |          #    #      ###    ###          |\n"
          "|           |                               #          |\n"
          "|           |                             ##           |\n"
          "|           |                                          |\n"
          "|           |           #    ###   ###   #             |\n"
          "|           |          ###  #   # #     ###            |\n"
          "|           |           #   #####  ###   #             |\n"
          "|           |           #   #         #  #             |\n"
          "|           |            ##  ###   ###    ##           |\n"
          "+-----------+------------------------------------------+\n"
          "+-----------+------------------------------------------+\n"
          "|       Bye,|*****   *      *  *      ******* ******  *|\n"
          "|Hello Kitty|*    *  *      *  *      *            *  *|\n"
          "|           |*    *  *      *  *      *           *   *|\n"
          "|           |*    *  *      *  *      *****      *    *|\n"
          "|           |****    *      *  *      *         *     *|\n"
          "|           |*  *    *      *  *      *        *       |\n"
          "|           |*   *   *      *  *      *       *       *|\n"
          "|           |*    *    *****   ****** ******* ******  *|\n"
          "+-----------+------------------------------------------+\n" );
    t1 = t0;
    t1 . setCell ( 0, 0, CEmpty () );
    t1 . setCell ( 1, 1, CImage ()
            . addRow ( "  ********                    " )
            . addRow ( " **********                   " )
            . addRow ( "**        **                  " )
            . addRow ( "**             **        **   " )
            . addRow ( "**             **        **   " )
            . addRow ( "***         ********  ********" )
            . addRow ( "****        ********  ********" )
            . addRow ( "****           **        **   " )
            . addRow ( "****           **        **   " )
            . addRow ( "****      **                  " )
            . addRow ( " **********                   " )
            . addRow ( "  ********                    " ) );
    oss . str ("");
    oss . clear ();
    oss << t0;
    assert ( oss . str () ==
          "+----------------------------------------------+------------------------------------------+\n"
          "|Hello,                                        |          ###                             |\n"
          "|Hello Kitty                                   |          #  #                            |\n"
          "|                                              |          #  # # ##   ###    ###          |\n"
          "|                                              |          ###  ##    #   #  #  #          |\n"
          "|                                              |          #    #     #   #  #  #          |\n"
          "|                                              |          #    #     #   #  #  #          |\n"
          "|                                              |          #    #      ###    ###          |\n"
          "|                                              |                               #          |\n"
          "|                                              |                             ##           |\n"
          "|                                              |                                          |\n"
          "|                                              |           #    ###   ###   #             |\n"
          "|                                              |          ###  #   # #     ###            |\n"
          "|                                              |           #   #####  ###   #             |\n"
          "|                                              |           #   #         #  #             |\n"
          "|                                              |            ##  ###   ###    ##           |\n"
          "+----------------------------------------------+------------------------------------------+\n"
          "|Lorem ipsum dolor sit amet,                   |                                          |\n"
          "|consectetur adipiscing                        |          ###                             |\n"
          "|elit. Curabitur scelerisque                   |          #  #                            |\n"
          "|lorem vitae lectus cursus,                    |          #  # # ##   ###    ###          |\n"
          "|vitae porta ante placerat. Class aptent taciti|          ###  ##    #   #  #  #          |\n"
          "|sociosqu ad litora                            |          #    #     #   #  #  #          |\n"
          "|torquent per                                  |          #    #     #   #  #  #          |\n"
          "|conubia nostra,                               |          #    #      ###    ###          |\n"
          "|per inceptos himenaeos.                       |                               #          |\n"
          "|                                              |                             ##           |\n"
          "|Donec tincidunt augue                         |                                          |\n"
          "|sit amet metus                                |           #    ###   ###   #             |\n"
          "|pretium volutpat.                             |          ###  #   # #     ###            |\n"
          "|Donec faucibus,                               |           #   #####  ###   #             |\n"
          "|ante sit amet                                 |           #   #         #  #             |\n"
          "|luctus posuere,                               |            ##  ###   ###    ##           |\n"
          "|mauris tellus                                 |                                          |\n"
          "+----------------------------------------------+------------------------------------------+\n"
          "|                                          Bye,|*****   *      *  *      ******* ******  *|\n"
          "|                                   Hello Kitty|*    *  *      *  *      *            *  *|\n"
          "|                                              |*    *  *      *  *      *           *   *|\n"
          "|                                              |*    *  *      *  *      *****      *    *|\n"
          "|                                              |****    *      *  *      *         *     *|\n"
          "|                                              |*  *    *      *  *      *        *       |\n"
          "|                                              |*   *   *      *  *      *       *       *|\n"
          "|                                              |*    *    *****   ****** ******* ******  *|\n"
          "+----------------------------------------------+------------------------------------------+\n" );
    oss . str ("");
    oss . clear ();
    oss << t1;
    assert ( oss . str () ==
          "+----------------------------------------------+------------------------------------------+\n"
          "|                                              |          ###                             |\n"
          "|                                              |          #  #                            |\n"
          "|                                              |          #  # # ##   ###    ###          |\n"
          "|                                              |          ###  ##    #   #  #  #          |\n"
          "|                                              |          #    #     #   #  #  #          |\n"
          "|                                              |          #    #     #   #  #  #          |\n"
          "|                                              |          #    #      ###    ###          |\n"
          "|                                              |                               #          |\n"
          "|                                              |                             ##           |\n"
          "|                                              |                                          |\n"
          "|                                              |           #    ###   ###   #             |\n"
          "|                                              |          ###  #   # #     ###            |\n"
          "|                                              |           #   #####  ###   #             |\n"
          "|                                              |           #   #         #  #             |\n"
          "|                                              |            ##  ###   ###    ##           |\n"
          "+----------------------------------------------+------------------------------------------+\n"
          "|Lorem ipsum dolor sit amet,                   |                                          |\n"
          "|consectetur adipiscing                        |                                          |\n"
          "|elit. Curabitur scelerisque                   |        ********                          |\n"
          "|lorem vitae lectus cursus,                    |       **********                         |\n"
          "|vitae porta ante placerat. Class aptent taciti|      **        **                        |\n"
          "|sociosqu ad litora                            |      **             **        **         |\n"
          "|torquent per                                  |      **             **        **         |\n"
          "|conubia nostra,                               |      ***         ********  ********      |\n"
          "|per inceptos himenaeos.                       |      ****        ********  ********      |\n"
          "|                                              |      ****           **        **         |\n"
          "|Donec tincidunt augue                         |      ****           **        **         |\n"
          "|sit amet metus                                |      ****      **                        |\n"
          "|pretium volutpat.                             |       **********                         |\n"
          "|Donec faucibus,                               |        ********                          |\n"
          "|ante sit amet                                 |                                          |\n"
          "|luctus posuere,                               |                                          |\n"
          "|mauris tellus                                 |                                          |\n"
          "+----------------------------------------------+------------------------------------------+\n"
          "|                                          Bye,|*****   *      *  *      ******* ******  *|\n"
          "|                                   Hello Kitty|*    *  *      *  *      *            *  *|\n"
          "|                                              |*    *  *      *  *      *           *   *|\n"
          "|                                              |*    *  *      *  *      *****      *    *|\n"
          "|                                              |****    *      *  *      *         *     *|\n"
          "|                                              |*  *    *      *  *      *        *       |\n"
          "|                                              |*   *   *      *  *      *       *       *|\n"
          "|                                              |*    *    *****   ****** ******* ******  *|\n"
          "+----------------------------------------------+------------------------------------------+\n" );
    CTable t2 ( 2, 2 );
    t2 . setCell ( 0, 0, CText ( "OOP", CText::ALIGN_LEFT ) );
    t2 . setCell ( 0, 1, CText ( "Encapsulation", CText::ALIGN_LEFT ) );
    t2 . setCell ( 1, 0, CText ( "Polymorphism", CText::ALIGN_LEFT ) );
    t2 . setCell ( 1, 1, CText ( "Inheritance", CText::ALIGN_LEFT ) );
    oss . str ("");
    oss . clear ();
    oss << t2;
    assert ( oss . str () ==
          "+------------+-------------+\n"
          "|OOP         |Encapsulation|\n"
          "+------------+-------------+\n"
          "|Polymorphism|Inheritance  |\n"
          "+------------+-------------+\n" );
    t1 . setCell ( 0, 0, t2 );
    dynamic_cast<CText &> ( t2 . getCell ( 0, 0 ) ) . setText ( "Object Oriented Programming" );
    oss . str ("");
    oss . clear ();
    oss << t2;
    assert ( oss . str () ==
          "+---------------------------+-------------+\n"
          "|Object Oriented Programming|Encapsulation|\n"
          "+---------------------------+-------------+\n"
          "|Polymorphism               |Inheritance  |\n"
          "+---------------------------+-------------+\n" );
    oss . str ("");
    oss . clear ();
    oss << t1;
    assert ( oss . str () ==
          "+----------------------------------------------+------------------------------------------+\n"
          "|+------------+-------------+                  |          ###                             |\n"
          "||OOP         |Encapsulation|                  |          #  #                            |\n"
          "|+------------+-------------+                  |          #  # # ##   ###    ###          |\n"
          "||Polymorphism|Inheritance  |                  |          ###  ##    #   #  #  #          |\n"
          "|+------------+-------------+                  |          #    #     #   #  #  #          |\n"
          "|                                              |          #    #     #   #  #  #          |\n"
          "|                                              |          #    #      ###    ###          |\n"
          "|                                              |                               #          |\n"
          "|                                              |                             ##           |\n"
          "|                                              |                                          |\n"
          "|                                              |           #    ###   ###   #             |\n"
          "|                                              |          ###  #   # #     ###            |\n"
          "|                                              |           #   #####  ###   #             |\n"
          "|                                              |           #   #         #  #             |\n"
          "|                                              |            ##  ###   ###    ##           |\n"
          "+----------------------------------------------+------------------------------------------+\n"
          "|Lorem ipsum dolor sit amet,                   |                                          |\n"
          "|consectetur adipiscing                        |                                          |\n"
          "|elit. Curabitur scelerisque                   |        ********                          |\n"
          "|lorem vitae lectus cursus,                    |       **********                         |\n"
          "|vitae porta ante placerat. Class aptent taciti|      **        **                        |\n"
          "|sociosqu ad litora                            |      **             **        **         |\n"
          "|torquent per                                  |      **             **        **         |\n"
          "|conubia nostra,                               |      ***         ********  ********      |\n"
          "|per inceptos himenaeos.                       |      ****        ********  ********      |\n"
          "|                                              |      ****           **        **         |\n"
          "|Donec tincidunt augue                         |      ****           **        **         |\n"
          "|sit amet metus                                |      ****      **                        |\n"
          "|pretium volutpat.                             |       **********                         |\n"
          "|Donec faucibus,                               |        ********                          |\n"
          "|ante sit amet                                 |                                          |\n"
          "|luctus posuere,                               |                                          |\n"
          "|mauris tellus                                 |                                          |\n"
          "+----------------------------------------------+------------------------------------------+\n"
          "|                                          Bye,|*****   *      *  *      ******* ******  *|\n"
          "|                                   Hello Kitty|*    *  *      *  *      *            *  *|\n"
          "|                                              |*    *  *      *  *      *           *   *|\n"
          "|                                              |*    *  *      *  *      *****      *    *|\n"
          "|                                              |****    *      *  *      *         *     *|\n"
          "|                                              |*  *    *      *  *      *        *       |\n"
          "|                                              |*   *   *      *  *      *       *       *|\n"
          "|                                              |*    *    *****   ****** ******* ******  *|\n"
          "+----------------------------------------------+------------------------------------------+\n" );
    assert ( t0 != t1 );
    assert ( !( t0 == t1 ) );
    assert ( t0 . getCell ( 1, 1 ) == t0 . getCell ( 0, 1 ) );
    assert ( ! ( t0 . getCell ( 1, 1 ) != t0 . getCell ( 0, 1 ) ) );
    assert ( t0 . getCell ( 0, 0 ) != t0 . getCell ( 0, 1 ) );
    assert ( ! ( t0 . getCell ( 0, 0 ) == t0 . getCell ( 0, 1 ) ) );
    t1 . setCell ( 0, 0, t1 );
    oss . str ("");
    oss . clear ();
    oss << t1;
    assert ( oss . str () ==
          "+-------------------------------------------------------------------------------------------+------------------------------------------+\n"
          "|+----------------------------------------------+------------------------------------------+|                                          |\n"
          "||+------------+-------------+                  |          ###                             ||                                          |\n"
          "|||OOP         |Encapsulation|                  |          #  #                            ||                                          |\n"
          "||+------------+-------------+                  |          #  # # ##   ###    ###          ||                                          |\n"
          "|||Polymorphism|Inheritance  |                  |          ###  ##    #   #  #  #          ||                                          |\n"
          "||+------------+-------------+                  |          #    #     #   #  #  #          ||                                          |\n"
          "||                                              |          #    #     #   #  #  #          ||                                          |\n"
          "||                                              |          #    #      ###    ###          ||                                          |\n"
          "||                                              |                               #          ||                                          |\n"
          "||                                              |                             ##           ||                                          |\n"
          "||                                              |                                          ||                                          |\n"
          "||                                              |           #    ###   ###   #             ||                                          |\n"
          "||                                              |          ###  #   # #     ###            ||                                          |\n"
          "||                                              |           #   #####  ###   #             ||                                          |\n"
          "||                                              |           #   #         #  #             ||          ###                             |\n"
          "||                                              |            ##  ###   ###    ##           ||          #  #                            |\n"
          "|+----------------------------------------------+------------------------------------------+|          #  # # ##   ###    ###          |\n"
          "||Lorem ipsum dolor sit amet,                   |                                          ||          ###  ##    #   #  #  #          |\n"
          "||consectetur adipiscing                        |                                          ||          #    #     #   #  #  #          |\n"
          "||elit. Curabitur scelerisque                   |        ********                          ||          #    #     #   #  #  #          |\n"
          "||lorem vitae lectus cursus,                    |       **********                         ||          #    #      ###    ###          |\n"
          "||vitae porta ante placerat. Class aptent taciti|      **        **                        ||                               #          |\n"
          "||sociosqu ad litora                            |      **             **        **         ||                             ##           |\n"
          "||torquent per                                  |      **             **        **         ||                                          |\n"
          "||conubia nostra,                               |      ***         ********  ********      ||           #    ###   ###   #             |\n"
          "||per inceptos himenaeos.                       |      ****        ********  ********      ||          ###  #   # #     ###            |\n"
          "||                                              |      ****           **        **         ||           #   #####  ###   #             |\n"
          "||Donec tincidunt augue                         |      ****           **        **         ||           #   #         #  #             |\n"
          "||sit amet metus                                |      ****      **                        ||            ##  ###   ###    ##           |\n"
          "||pretium volutpat.                             |       **********                         ||                                          |\n"
          "||Donec faucibus,                               |        ********                          ||                                          |\n"
          "||ante sit amet                                 |                                          ||                                          |\n"
          "||luctus posuere,                               |                                          ||                                          |\n"
          "||mauris tellus                                 |                                          ||                                          |\n"
          "|+----------------------------------------------+------------------------------------------+|                                          |\n"
          "||                                          Bye,|*****   *      *  *      ******* ******  *||                                          |\n"
          "||                                   Hello Kitty|*    *  *      *  *      *            *  *||                                          |\n"
          "||                                              |*    *  *      *  *      *           *   *||                                          |\n"
          "||                                              |*    *  *      *  *      *****      *    *||                                          |\n"
          "||                                              |****    *      *  *      *         *     *||                                          |\n"
          "||                                              |*  *    *      *  *      *        *       ||                                          |\n"
          "||                                              |*   *   *      *  *      *       *       *||                                          |\n"
          "||                                              |*    *    *****   ****** ******* ******  *||                                          |\n"
          "|+----------------------------------------------+------------------------------------------+|                                          |\n"
          "+-------------------------------------------------------------------------------------------+------------------------------------------+\n"
          "|Lorem ipsum dolor sit amet,                                                                |                                          |\n"
          "|consectetur adipiscing                                                                     |                                          |\n"
          "|elit. Curabitur scelerisque                                                                |        ********                          |\n"
          "|lorem vitae lectus cursus,                                                                 |       **********                         |\n"
          "|vitae porta ante placerat. Class aptent taciti                                             |      **        **                        |\n"
          "|sociosqu ad litora                                                                         |      **             **        **         |\n"
          "|torquent per                                                                               |      **             **        **         |\n"
          "|conubia nostra,                                                                            |      ***         ********  ********      |\n"
          "|per inceptos himenaeos.                                                                    |      ****        ********  ********      |\n"
          "|                                                                                           |      ****           **        **         |\n"
          "|Donec tincidunt augue                                                                      |      ****           **        **         |\n"
          "|sit amet metus                                                                             |      ****      **                        |\n"
          "|pretium volutpat.                                                                          |       **********                         |\n"
          "|Donec faucibus,                                                                            |        ********                          |\n"
          "|ante sit amet                                                                              |                                          |\n"
          "|luctus posuere,                                                                            |                                          |\n"
          "|mauris tellus                                                                              |                                          |\n"
          "+-------------------------------------------------------------------------------------------+------------------------------------------+\n"
          "|                                                                                       Bye,|*****   *      *  *      ******* ******  *|\n"
          "|                                                                                Hello Kitty|*    *  *      *  *      *            *  *|\n"
          "|                                                                                           |*    *  *      *  *      *           *   *|\n"
          "|                                                                                           |*    *  *      *  *      *****      *    *|\n"
          "|                                                                                           |****    *      *  *      *         *     *|\n"
          "|                                                                                           |*  *    *      *  *      *        *       |\n"
          "|                                                                                           |*   *   *      *  *      *       *       *|\n"
          "|                                                                                           |*    *    *****   ****** ******* ******  *|\n"
          "+-------------------------------------------------------------------------------------------+------------------------------------------+\n" );
    t1 . setCell ( 0, 0, t1 );
    oss . str ("");
    oss . clear ();
    oss << t1;
    assert ( oss . str () ==
          "+----------------------------------------------------------------------------------------------------------------------------------------+------------------------------------------+\n"
          "|+-------------------------------------------------------------------------------------------+------------------------------------------+|                                          |\n"
          "||+----------------------------------------------+------------------------------------------+|                                          ||                                          |\n"
          "|||+------------+-------------+                  |          ###                             ||                                          ||                                          |\n"
          "||||OOP         |Encapsulation|                  |          #  #                            ||                                          ||                                          |\n"
          "|||+------------+-------------+                  |          #  # # ##   ###    ###          ||                                          ||                                          |\n"
          "||||Polymorphism|Inheritance  |                  |          ###  ##    #   #  #  #          ||                                          ||                                          |\n"
          "|||+------------+-------------+                  |          #    #     #   #  #  #          ||                                          ||                                          |\n"
          "|||                                              |          #    #     #   #  #  #          ||                                          ||                                          |\n"
          "|||                                              |          #    #      ###    ###          ||                                          ||                                          |\n"
          "|||                                              |                               #          ||                                          ||                                          |\n"
          "|||                                              |                             ##           ||                                          ||                                          |\n"
          "|||                                              |                                          ||                                          ||                                          |\n"
          "|||                                              |           #    ###   ###   #             ||                                          ||                                          |\n"
          "|||                                              |          ###  #   # #     ###            ||                                          ||                                          |\n"
          "|||                                              |           #   #####  ###   #             ||                                          ||                                          |\n"
          "|||                                              |           #   #         #  #             ||          ###                             ||                                          |\n"
          "|||                                              |            ##  ###   ###    ##           ||          #  #                            ||                                          |\n"
          "||+----------------------------------------------+------------------------------------------+|          #  # # ##   ###    ###          ||                                          |\n"
          "|||Lorem ipsum dolor sit amet,                   |                                          ||          ###  ##    #   #  #  #          ||                                          |\n"
          "|||consectetur adipiscing                        |                                          ||          #    #     #   #  #  #          ||                                          |\n"
          "|||elit. Curabitur scelerisque                   |        ********                          ||          #    #     #   #  #  #          ||                                          |\n"
          "|||lorem vitae lectus cursus,                    |       **********                         ||          #    #      ###    ###          ||                                          |\n"
          "|||vitae porta ante placerat. Class aptent taciti|      **        **                        ||                               #          ||                                          |\n"
          "|||sociosqu ad litora                            |      **             **        **         ||                             ##           ||                                          |\n"
          "|||torquent per                                  |      **             **        **         ||                                          ||                                          |\n"
          "|||conubia nostra,                               |      ***         ********  ********      ||           #    ###   ###   #             ||                                          |\n"
          "|||per inceptos himenaeos.                       |      ****        ********  ********      ||          ###  #   # #     ###            ||                                          |\n"
          "|||                                              |      ****           **        **         ||           #   #####  ###   #             ||                                          |\n"
          "|||Donec tincidunt augue                         |      ****           **        **         ||           #   #         #  #             ||                                          |\n"
          "|||sit amet metus                                |      ****      **                        ||            ##  ###   ###    ##           ||          ###                             |\n"
          "|||pretium volutpat.                             |       **********                         ||                                          ||          #  #                            |\n"
          "|||Donec faucibus,                               |        ********                          ||                                          ||          #  # # ##   ###    ###          |\n"
          "|||ante sit amet                                 |                                          ||                                          ||          ###  ##    #   #  #  #          |\n"
          "|||luctus posuere,                               |                                          ||                                          ||          #    #     #   #  #  #          |\n"
          "|||mauris tellus                                 |                                          ||                                          ||          #    #     #   #  #  #          |\n"
          "||+----------------------------------------------+------------------------------------------+|                                          ||          #    #      ###    ###          |\n"
          "|||                                          Bye,|*****   *      *  *      ******* ******  *||                                          ||                               #          |\n"
          "|||                                   Hello Kitty|*    *  *      *  *      *            *  *||                                          ||                             ##           |\n"
          "|||                                              |*    *  *      *  *      *           *   *||                                          ||                                          |\n"
          "|||                                              |*    *  *      *  *      *****      *    *||                                          ||           #    ###   ###   #             |\n"
          "|||                                              |****    *      *  *      *         *     *||                                          ||          ###  #   # #     ###            |\n"
          "|||                                              |*  *    *      *  *      *        *       ||                                          ||           #   #####  ###   #             |\n"
          "|||                                              |*   *   *      *  *      *       *       *||                                          ||           #   #         #  #             |\n"
          "|||                                              |*    *    *****   ****** ******* ******  *||                                          ||            ##  ###   ###    ##           |\n"
          "||+----------------------------------------------+------------------------------------------+|                                          ||                                          |\n"
          "|+-------------------------------------------------------------------------------------------+------------------------------------------+|                                          |\n"
          "||Lorem ipsum dolor sit amet,                                                                |                                          ||                                          |\n"
          "||consectetur adipiscing                                                                     |                                          ||                                          |\n"
          "||elit. Curabitur scelerisque                                                                |        ********                          ||                                          |\n"
          "||lorem vitae lectus cursus,                                                                 |       **********                         ||                                          |\n"
          "||vitae porta ante placerat. Class aptent taciti                                             |      **        **                        ||                                          |\n"
          "||sociosqu ad litora                                                                         |      **             **        **         ||                                          |\n"
          "||torquent per                                                                               |      **             **        **         ||                                          |\n"
          "||conubia nostra,                                                                            |      ***         ********  ********      ||                                          |\n"
          "||per inceptos himenaeos.                                                                    |      ****        ********  ********      ||                                          |\n"
          "||                                                                                           |      ****           **        **         ||                                          |\n"
          "||Donec tincidunt augue                                                                      |      ****           **        **         ||                                          |\n"
          "||sit amet metus                                                                             |      ****      **                        ||                                          |\n"
          "||pretium volutpat.                                                                          |       **********                         ||                                          |\n"
          "||Donec faucibus,                                                                            |        ********                          ||                                          |\n"
          "||ante sit amet                                                                              |                                          ||                                          |\n"
          "||luctus posuere,                                                                            |                                          ||                                          |\n"
          "||mauris tellus                                                                              |                                          ||                                          |\n"
          "|+-------------------------------------------------------------------------------------------+------------------------------------------+|                                          |\n"
          "||                                                                                       Bye,|*****   *      *  *      ******* ******  *||                                          |\n"
          "||                                                                                Hello Kitty|*    *  *      *  *      *            *  *||                                          |\n"
          "||                                                                                           |*    *  *      *  *      *           *   *||                                          |\n"
          "||                                                                                           |*    *  *      *  *      *****      *    *||                                          |\n"
          "||                                                                                           |****    *      *  *      *         *     *||                                          |\n"
          "||                                                                                           |*  *    *      *  *      *        *       ||                                          |\n"
          "||                                                                                           |*   *   *      *  *      *       *       *||                                          |\n"
          "||                                                                                           |*    *    *****   ****** ******* ******  *||                                          |\n"
          "|+-------------------------------------------------------------------------------------------+------------------------------------------+|                                          |\n"
          "+----------------------------------------------------------------------------------------------------------------------------------------+------------------------------------------+\n"
          "|Lorem ipsum dolor sit amet,                                                                                                             |                                          |\n"
          "|consectetur adipiscing                                                                                                                  |                                          |\n"
          "|elit. Curabitur scelerisque                                                                                                             |        ********                          |\n"
          "|lorem vitae lectus cursus,                                                                                                              |       **********                         |\n"
          "|vitae porta ante placerat. Class aptent taciti                                                                                          |      **        **                        |\n"
          "|sociosqu ad litora                                                                                                                      |      **             **        **         |\n"
          "|torquent per                                                                                                                            |      **             **        **         |\n"
          "|conubia nostra,                                                                                                                         |      ***         ********  ********      |\n"
          "|per inceptos himenaeos.                                                                                                                 |      ****        ********  ********      |\n"
          "|                                                                                                                                        |      ****           **        **         |\n"
          "|Donec tincidunt augue                                                                                                                   |      ****           **        **         |\n"
          "|sit amet metus                                                                                                                          |      ****      **                        |\n"
          "|pretium volutpat.                                                                                                                       |       **********                         |\n"
          "|Donec faucibus,                                                                                                                         |        ********                          |\n"
          "|ante sit amet                                                                                                                           |                                          |\n"
          "|luctus posuere,                                                                                                                         |                                          |\n"
          "|mauris tellus                                                                                                                           |                                          |\n"
          "+----------------------------------------------------------------------------------------------------------------------------------------+------------------------------------------+\n"
          "|                                                                                                                                    Bye,|*****   *      *  *      ******* ******  *|\n"
          "|                                                                                                                             Hello Kitty|*    *  *      *  *      *            *  *|\n"
          "|                                                                                                                                        |*    *  *      *  *      *           *   *|\n"
          "|                                                                                                                                        |*    *  *      *  *      *****      *    *|\n"
          "|                                                                                                                                        |****    *      *  *      *         *     *|\n"
          "|                                                                                                                                        |*  *    *      *  *      *        *       |\n"
          "|                                                                                                                                        |*   *   *      *  *      *       *       *|\n"
          "|                                                                                                                                        |*    *    *****   ****** ******* ******  *|\n"
          "+----------------------------------------------------------------------------------------------------------------------------------------+------------------------------------------+\n" );
    return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
