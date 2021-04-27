#pragma once
#include <string>
#include <map>
using namespace std;

namespace ariel
{
    class staticMap
    {
    public:
        map<string, map<string, double> *> smap;
        staticMap()
        {
            smap = {};
        }
        ~staticMap()
        {
            for (map<string, map<string, double> *>::iterator it = smap.begin(); it != smap.end(); it++)
            {
                delete it->second;
            }
        }
    };
    class NumberWithUnits
    {
        static int num_units;
        static map<string, int> unit_key_map; //maybe change to long
        static staticMap units_map;
        friend NumberWithUnits operator+(const NumberWithUnits &curr);
        friend NumberWithUnits operator-(const NumberWithUnits &curr);
        friend ostream &operator<<(ostream &os, const NumberWithUnits &curr);
        friend NumberWithUnits operator*(const double d, const NumberWithUnits &curr);
        friend istream &operator>>(istream &is, NumberWithUnits &curr);
        friend NumberWithUnits &operator++(NumberWithUnits &curr);
        friend NumberWithUnits &operator--(NumberWithUnits &curr);

    public:
        string unit;
        double dim;
        NumberWithUnits(double dim, const string &unit)
        {

            if (NumberWithUnits::units_map.smap.find(unit) == NumberWithUnits::units_map.smap.end())
            {
                throw invalid_argument("Invalid Unit");
            }
            else
            {
                this->unit = unit;
                this->dim = dim;
            }
        };
        NumberWithUnits(const NumberWithUnits &other)
        {
            this->dim = other.dim;
            this->unit = other.unit;
        };
        ~NumberWithUnits(){};
        static void read_units(ifstream &units_file);
        NumberWithUnits operator+(const NumberWithUnits &other) const;
        NumberWithUnits &operator+=(const NumberWithUnits &other);
        NumberWithUnits operator-(const NumberWithUnits &other) const;
        NumberWithUnits &operator-=(const NumberWithUnits &other);
        NumberWithUnits operator*(const double d) const;
        bool operator<(const NumberWithUnits &other) const;
        bool operator<=(const NumberWithUnits &other) const;
        bool operator==(const NumberWithUnits &other) const;
        bool operator>=(const NumberWithUnits &other) const;
        bool operator>(const NumberWithUnits &other) const;
        bool operator!=(const NumberWithUnits &other) const;
        NumberWithUnits operator++(int);
        NumberWithUnits operator--(int);
    };


}