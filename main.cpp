#include <iostream>
#include "core/TEngine.hpp"

int main() {
    TEngine::Temporal_Engine engine;
    engine.load("/mnt/i/projects/Temporal_path/data/first.txt");
    engine.transform();
    return 0;
}
