#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <iostream>
#include <string>

namespace sogdanov {

    void cmd_graphs(std::istream& in, std::ostream& out);
    void cmd_vertexes(std::istream& in, std::ostream& out);
    void cmd_outbound(std::istream& in, std::ostream& out);
    void cmd_inbound(std::istream& in, std::ostream& out);
    void cmd_bind(std::istream& in, std::ostream& out);
    void cmd_cut(std::istream& in, std::ostream& out);
    void cmd_create(std::istream& in, std::ostream& out);
    void cmd_merge(std::istream& in, std::ostream& out);
    void cmd_extract(std::istream& in, std::ostream& out);

    void load_file(const std::string& filename);

    template<class Iter>
    void sort(Iter begin, Iter end);
} 

#endif
