#include <iostream>
#include "core/TEngine.hpp"

int main() {
    TEngine::Temporal_Engine engine;
    engine.load("/mnt/i/projects/Temporal_path/data/second.txt");
    engine.transform();
    auto res = engine.get_earliest_arrival_time(2,1,10);
    for(auto it:res.dist){
        cout<<"vertex: "<<it.first<<" arrival time: "<<it.second<<endl;
    }
    return 0;
}
