//  supercollider-style synthdef
//  Copyright (C) 2008, 2009 Tim Blechmann
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; see the file COPYING.  If not, write to
//  the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
//  Boston, MA 02111-1307, USA.

#ifndef SC_SYNTHDEF_HPP
#define SC_SYNTHDEF_HPP

#include <string>
#include <vector>
#include <fstream>
#include <map>

#include <boost/cstdint.hpp>
#include <boost/filesystem/path.hpp>

namespace nova
{

class sc_synthdef
{
    typedef std::string string;
    typedef std::vector<float> fvector;
    typedef std::vector<string> svector;
    typedef std::map<string, int> parameter_map_t;

    typedef boost::int16_t int16;
    typedef boost::int32_t int32;

public:
    struct input_spec
    {
        input_spec(int16_t source, int16_t index):
            source(source), index(index)
        {}

        int16_t source;   /* index of ugen or -1 for constant */
        int16_t index;    /* number of output or constant index */
    };

    struct unit_spec_t
    {
        explicit unit_spec_t(std::istream & istream);

        unit_spec_t(string const & name, int16_t rate, int16_t special_index,
                    std::vector<input_spec> const & in_specs,
                    std::vector<char> const & out_specs):
            name(name), rate(rate), special_index(special_index),
            input_specs(in_specs), output_specs(out_specs)
        {}

        string name;
        int16_t rate;           /* 0: scalar rate, 1: buffer rate, 2: full rate, 3: demand rate */
        int16_t special_index;

        std::vector<input_spec> input_specs;
        std::vector<char> output_specs;
    };

    friend class sc_synth_prototype;
    friend class sc_synth;
    friend class sc_ugen_factory;
    friend class sc_ugen_def;

    typedef std::vector<unit_spec_t> graph_t;

    explicit sc_synthdef(std::istream & istream);
    explicit sc_synthdef(boost::filesystem::path const & path);

    string dump(void) const;

    string const & name(void) const
    {
        return name_;
    }

    size_t parameter_count(void) const
    {
        assert(parameters.size() == parameter_map.size());
        return parameters.size();
    }

private:
    void read_synthdef(std::istream & istream);

    string name_;
    fvector constants;
    fvector parameters;
    parameter_map_t parameter_map;

    graph_t graph;
};

std::vector<sc_synthdef> read_synthdef_file(boost::filesystem::path const & filename);

} /* namespace nova */

#endif /* SC_SYNTHDEF_HPP */
