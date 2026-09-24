#ifndef CLI_HPP
#define CLI_HPP

#include "scene_data.hpp"

class CLI {
    public:
        static void RunCLI(SceneData& scenes);

        static void EditScene(Scene& scene);

    private:
        static void Help();

        static void EditHelp();
};

#endif