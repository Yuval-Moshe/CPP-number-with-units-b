#include <string>
#include <map>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cctype>
#include <cmath>
#include <stdlib.h>
#include <algorithm>
using namespace std;

#include "NumberWithUnits.hpp"
using namespace ariel;

namespace ariel
{
    const float EPS = 0.001;
    int NumberWithUnits::num_units = 0;
    map<string, int> NumberWithUnits::unit_key_map = {};
    staticMap NumberWithUnits::units_map = {};
    void NumberWithUnits::read_units(ifstream &units_file)
    {
        string unit_str;
        units_file.clear();
        units_file.seekg(0, std::ios::beg);
        while (getline(units_file, unit_str))
        {
            string unit = unit_str.substr(1, unit_str.find_first_of('=') - 1);
            unit.erase(std::remove_if(unit.begin(), unit.end(), ::isspace), unit.end());
            if (NumberWithUnits::units_map.smap.find(unit) == NumberWithUnits::units_map.smap.end())
            {
                NumberWithUnits::units_map.smap.insert({unit, new map<string, double>});
                NumberWithUnits::units_map.smap.find(unit)->second->insert({unit, 1});
                NumberWithUnits::num_units++;
            }
            map<string, double> *unit_a_map = NumberWithUnits::units_map.smap.find(unit)->second;
            string unit_a = unit_str.substr(unit_str.find_first_of('=') + 1, unit_str.length() - 1);
            unit_a.erase(std::remove_if(unit_a.begin(), unit_a.end(), ::isspace), unit_a.end());
            unsigned long i = 0;
            double dim = 0;
            const int ten = 10;
            const int forty_eight = 48;
            while (isdigit(unit_a[i]) != 0)
            {
                dim *= ten;
                dim += (int)unit_a[i] - forty_eight;
                i++;
            }
            if (unit_a[i] == '.')
            {
                i++;
                int b = ten;
                while (isdigit(unit_a[i]) != 0)
                {
                    dim *= b;
                    dim += (int)unit_a[i] - forty_eight;
                    dim /= b;
                    i++;
                    b *= ten;
                }
            }
            string unit_b = unit_a.substr(i, unit_a.length() - 1);
            if (NumberWithUnits::units_map.smap.find(unit_b) == NumberWithUnits::units_map.smap.end())
            {
                NumberWithUnits::units_map.smap.insert({unit_b, new map<string, double>});
                NumberWithUnits::units_map.smap.find(unit_b)->second->insert({unit_b, 1});
                NumberWithUnits::num_units++;
            }
            map<string, double> *unit_b_map = NumberWithUnits::units_map.smap.find(unit_b)->second;
            if (unit_a_map->find(unit_b) == unit_a_map->end())
            {
                unit_a_map->insert({unit_b, dim});
                unit_b_map->insert({unit, 1 / dim});
            }
            for (map<string, double>::iterator it = unit_a_map->begin(); it != unit_a_map->end(); it++)
            {
                string curr_unit = it->first;
                if (curr_unit != unit_b && curr_unit != unit)
                {
                    map<string, double> *curr_unit_map = NumberWithUnits::units_map.smap.find(curr_unit)->second;
                    double curr_dim = curr_unit_map->find(unit)->second;
                    curr_unit_map->insert({unit_b, curr_dim * dim});
                    double curr_dim_b = unit_a_map->find(curr_unit)->second;
                    unit_b_map->insert({curr_unit, curr_dim_b / dim});

                }
            }
            for (map<string, double>::iterator it = unit_b_map->begin(); it != unit_b_map->end(); it++)
            {
                string curr_unit = it->first;
                if (curr_unit != unit_b && curr_unit != unit)
                {
                    map<string, double> *curr_unit_map = NumberWithUnits::units_map.smap.find(curr_unit)->second;
                    double curr_dim = curr_unit_map->find(unit_b)->second;
                    curr_unit_map->insert({unit, curr_dim / dim});
                    double curr_dim_b = unit_b_map->find(curr_unit)->second;
                    unit_a_map->insert({curr_unit, curr_dim_b * dim});
                }
            }
        }
    }

    NumberWithUnits NumberWithUnits::operator+(const NumberWithUnits &other) const
    {
        map<string, double> *this_unit_map = NumberWithUnits::units_map.smap.find(this->unit)->second;
        if (this_unit_map->find(other.unit) == this_unit_map->end())
        {
            throw string("Operation between 2 non-converting arguments");
        }
        map<string, double> *other_unit_map = NumberWithUnits::units_map.smap.find(other.unit)->second;
        double other_new_dim = other.dim * other_unit_map->find(this->unit)->second;
        double new_dim = this->dim + other_new_dim;
        return NumberWithUnits(new_dim, this->unit);
    }
    NumberWithUnits &NumberWithUnits::operator+=(const NumberWithUnits &other)
    {
        map<string, double> *this_unit_map = NumberWithUnits::units_map.smap.find(this->unit)->second;
        if (this_unit_map->find(other.unit) == this_unit_map->end())
        {
            throw string("Operation between 2 non-converting arguments");
        }
            map<string, double> *other_unit_map = NumberWithUnits::units_map.smap.find(other.unit)->second;
            double other_new_dim = other.dim * other_unit_map->find(this->unit)->second;
            this->dim += other_new_dim;
            return *this;
        
    }
    NumberWithUnits operator+(const NumberWithUnits &curr)
    {
        return NumberWithUnits(curr.dim, curr.unit);
    }
    NumberWithUnits operator-(const NumberWithUnits &curr)
    {
        return NumberWithUnits(-1 * curr.dim, curr.unit);
    }
    NumberWithUnits NumberWithUnits::operator-(const NumberWithUnits &other) const
    {
        map<string, double> *this_unit_map = NumberWithUnits::units_map.smap.find(this->unit)->second;
        if (this_unit_map->find(other.unit) == this_unit_map->end())
        {
            throw string("Operation between 2 non-converting arguments");
        }

            map<string, double> *other_unit_map = NumberWithUnits::units_map.smap.find(other.unit)->second;
            double other_new_dim = other.dim * other_unit_map->find(this->unit)->second;
            double new_dim = this->dim - other_new_dim;
            return NumberWithUnits(new_dim, this->unit);
        
    }
    NumberWithUnits &NumberWithUnits::operator-=(const NumberWithUnits &other)
    {
        map<string, double> *this_unit_map = NumberWithUnits::units_map.smap.find(this->unit)->second;
        if (this_unit_map->find(other.unit) == this_unit_map->end())
        {
            throw string("Operation between 2 non-converting arguments");
        }

            map<string, double> *other_unit_map = NumberWithUnits::units_map.smap.find(other.unit)->second;
            double other_new_dim = other.dim * other_unit_map->find(this->unit)->second;
            this->dim -= other_new_dim;
            return *this;
        
    }
    ostream &operator<<(ostream &os, const NumberWithUnits &curr)
    {
        return os << curr.dim << "[" << curr.unit << "]";
    }
    bool NumberWithUnits::operator<(const NumberWithUnits &other) const
    {
        map<string, double> *this_unit_map = NumberWithUnits::units_map.smap.find(this->unit)->second;
        if (this_unit_map->find(other.unit) == this_unit_map->end())
        {
            throw string("Operation between 2 non-converting arguments");
        }

            map<string, double> *other_unit_map = NumberWithUnits::units_map.smap.find(other.unit)->second;
            double other_new_dim = other.dim * other_unit_map->find(this->unit)->second;
            return this->dim < other_new_dim;
        
    }
    bool NumberWithUnits::operator<=(const NumberWithUnits &other) const
    {
        map<string, double> *this_unit_map = NumberWithUnits::units_map.smap.find(this->unit)->second;
        if (this_unit_map->find(other.unit) == this_unit_map->end())
        {
            throw string("Operation between 2 non-converting arguments");
        }

            map<string, double> *other_unit_map = NumberWithUnits::units_map.smap.find(other.unit)->second;
            double other_new_dim = other.dim * other_unit_map->find(this->unit)->second;
            return (this->dim - other_new_dim) <= EPS;
        
    }
    bool NumberWithUnits::operator==(const NumberWithUnits &other) const
    {
        map<string, double> *this_unit_map = NumberWithUnits::units_map.smap.find(this->unit)->second;
        if (this_unit_map->find(other.unit) == this_unit_map->end())
        {
            throw string("Operation between 2 non-converting arguments");
        }

            map<string, double> *other_unit_map = NumberWithUnits::units_map.smap.find(other.unit)->second;
            double other_new_dim = other.dim * other_unit_map->find(this->unit)->second;
            return abs(this->dim - other_new_dim) <= EPS;
        
    }
    bool NumberWithUnits::operator>=(const NumberWithUnits &other) const
    {
        map<string, double> *this_unit_map = NumberWithUnits::units_map.smap.find(this->unit)->second;
        if (this_unit_map->find(other.unit) == this_unit_map->end())
        {
            throw string("Operation between 2 non-converting arguments");
        }

            map<string, double> *other_unit_map = NumberWithUnits::units_map.smap.find(other.unit)->second;
            double other_new_dim = other.dim * other_unit_map->find(this->unit)->second;
            return this->dim >= other_new_dim;
        
    }
    bool NumberWithUnits::operator>(const NumberWithUnits &other) const
    {
        map<string, double> *this_unit_map = NumberWithUnits::units_map.smap.find(this->unit)->second;
        if (this_unit_map->find(other.unit) == this_unit_map->end())
        {
            throw string("Operation between 2 non-converting arguments");
        }

            map<string, double> *other_unit_map = NumberWithUnits::units_map.smap.find(other.unit)->second;
            double other_new_dim = other.dim * other_unit_map->find(this->unit)->second;
            return this->dim > other_new_dim;
        
    }
    bool NumberWithUnits::operator!=(const NumberWithUnits &other) const
    {
        map<string, double> *this_unit_map = NumberWithUnits::units_map.smap.find(this->unit)->second;
        if (this_unit_map->find(other.unit) == this_unit_map->end())
        {
            throw string("Operation between 2 non-converting arguments");
        }

            map<string, double> *other_unit_map = NumberWithUnits::units_map.smap.find(other.unit)->second;
            double other_new_dim = other.dim * other_unit_map->find(this->unit)->second;
            return abs(this->dim - other_new_dim) > EPS;
        
    }
    NumberWithUnits NumberWithUnits::operator++(int)
    {
        NumberWithUnits curr(this->dim, this->unit);
        this->dim++;
        return curr;
    }
    NumberWithUnits NumberWithUnits::operator--(int)
    {
        NumberWithUnits curr(this->dim, this->unit);
        this->dim--;
        return curr;
    }
    NumberWithUnits operator*(const double d, const NumberWithUnits &curr)
    {
        return NumberWithUnits(d * curr.dim, curr.unit);
    }
    istream &operator>>(istream &is, NumberWithUnits &curr)
    {
        double new_dim = 0;
        string dim_str, unit;
        char c = 0;
        is.get(c);
        unsigned int i = 0;
        while (c != '[')
        {
            dim_str += c;
            is.get(c);
            i++;
        }
        new_dim = stod(dim_str);
        is.get(c);
        i++;
        while (c != ']')
        {
            if (c != ' ')
            {
                unit += c;
            }
            is.get(c);
            i++;
        }
        if (NumberWithUnits::units_map.smap.find(unit) == NumberWithUnits::units_map.smap.end())
        {
            throw invalid_argument("Invalid Unit");
        }

            curr.dim = new_dim;
            curr.unit = unit;
        return is;
    }
    NumberWithUnits NumberWithUnits::operator*(const double d) const
    {
        return NumberWithUnits(d * this->dim, this->unit);
    }
    NumberWithUnits &operator++(NumberWithUnits &curr)
    {
        curr.dim++;
        return curr;
    }
    NumberWithUnits &operator--(NumberWithUnits &curr)
    {
        curr.dim--;
        return curr;
    }

}