#pragma once

#include "Defines.h"
#include "math/Color.hpp"
#include "math/Math.hpp"
#include "math/Point.hpp"
#include "math/Rect.hpp"

namespace box
{
    class Renderer
    {
    public:
        struct Texture
        {
            u32 id;
            i16 width;
            i16 height;
            i16 mipmaps;
            i16 format;
        };

        struct Command
        {
            u32      vertex;
            u32      vertex_size;
            u32      index;
            u32      index_size;
            u32      blendmode;
            Texture* texture;
        };

        struct Vertex
        {
            FPoint position;
            Color  color;
            FPoint tex_coord;
        };

        struct Mesh
        {
            Vertex* vertex      = nullptr;
            u32     vertex_size = 0u;
            i32*    index       = nullptr;
            u32     index_size  = 0u;
        };

        struct Camera
        {
            FPoint offset;   // Camera offset (displacement from target)
            FPoint target;   // Camera target (rotation and zoom origin)
            f32    rotation; // Camera rotation in degrees
            f32    zoom;     // Camera zoom (scaling), should be 1.0f by default
        };

        Renderer();
        ~Renderer()               = default;
        Renderer(const Renderer&) = delete;

        bool beginFrame();
        void endFrame();

        bool begin2D(const Camera& cam, bool depthsort);
        void end2D();

        void clear(Color clr);

        void setDepth(u32 depth);
        u32  getDepth() const;

        void setBlendMode(u32 bm);
        u32  getBlendMode() const;

        void     setTexture(Texture* tx);
        Texture* getTexture() const;

        Mesh beginMesh(u32 vtx, u32 idx);
        void endMesh(const Mesh& m);

    private:
        void setup();
        void newCommand();
        void drawBuffers(const std::vector<u32>& depths, const std::vector<Command>& cmds, const Vertex* vtx, const i32* idx);

    private:
        Command              _command{};
        std::vector<Vertex>  _verts;
        std::vector<i32>     _inds;
        std::vector<Command> _commands;
        std::vector<u32>     _depths;
        bool                 _depthsort{};
        Camera               _camera{};
        uint32_t             _depth{};
    };
} // namespace box
