#ifndef RESP_HPP
#define RESP_HPP
#include "type.h"
#include <unordered_map>
using namespace std;
namespace TEngine{
    struct resp_body{
        id_t id;
        unordered_map<id_t , ll> dist;
    };

    struct context{
        unordered_map<id_t, vector<vertex>> *vertex_map_in;
        unordered_map<id_t, vector<vertex>> *vertex_map_out;
    };

}
#endif