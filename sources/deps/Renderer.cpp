#include "Renderer.h"

#include "raylib.h"
#include "rlgl.h"           // Required for: Vertex definition
#include "raymath.h"

namespace box
{
    Renderer::Renderer()
    {
    }

    void Renderer::setup()
    {

    }

    bool Renderer::begin2D(const Camera& cam, bool depthsort)
    {
        _command.index_size  = 0;
        _command.vertex_size = 0;
        _command.index       = 0;
        _command.vertex      = 0;
        _command.blendmode   = RL_BLEND_ALPHA;
        _command.texture     = nullptr;
        _depth               = 0;
        _depthsort           = depthsort;
        _camera              = cam;

        _verts.resize(0xffff);
        _inds.resize(0xffff);
        _depths.clear();
        _commands.clear();

        Camera2D raycam;
        raycam.offset.x = _camera.offset.x;
        raycam.offset.y = _camera.offset.y;
        raycam.target.x = _camera.target.x;
        raycam.target.y = _camera.target.y;
        raycam.zoom     = _camera.zoom;
        raycam.rotation = _camera.rotation;
        BeginMode2D(raycam);

        return true;
    }

    void Renderer::end2D()
    {
        newCommand();

        if (_depthsort)
        {
            std::sort(std::execution::seq,
                      _depths.begin(),
                      _depths.end());
        }

        drawBuffers(_depths, _commands, _verts.data(), _inds.data());

        EndMode2D();

        _verts.resize(0xffff);
        _inds.resize(0xffff);
        _depths.clear();
        _commands.clear();
        _command.index_size  = 0;
        _command.vertex_size = 0;
        _command.index       = 0;
        _command.vertex      = 0;
        _depth               = 0;
    }

    void Renderer::newCommand()
    {
        if (_command.index_size + _command.vertex_size)
        {
            _commands.emplace_back(_command);
            _command.index += _command.index_size;
            _command.index_size = 0;
            _command.vertex += _command.vertex_size;
            _command.vertex_size = 0;
            _depths.emplace_back(_depth);
            _depth   = 0;
        }
    }

    void Renderer::drawBuffers(const std::vector<u32>&     depths,
                               const std::vector<Command>& cmds,
                               const Vertex*               vtx,
                               const i32*                  idx)
    {
        if (depths.size())
        {
            for (auto& ind : depths)
            {
                rlBegin(RL_TRIANGLES);
                auto& command = cmds[ind];
                {
                    if (rlCheckRenderBatchLimit(command.index_size))
                    {
                        rlBegin(RL_TRIANGLES);
                    }

                    rlSetTexture(command.texture ? command.texture->id : 0);
                    rlSetBlendMode(command.blendmode);

                    for (auto i = 0; i < command.index_size; ++i)
                    {
                        auto& vertex = vtx[idx[command.index + i]];

                        rlColor4ub(vertex.color.r, vertex.color.g, vertex.color.b, vertex.color.a);
                        rlTexCoord2f(vertex.tex_coord.x, vertex.tex_coord.y);
                        rlVertex2f(vertex.position.x, vertex.position.y);
                    }
                }
                rlEnd();
            }
        }
        else
        {
            rlBegin(RL_TRIANGLES);
            for (auto& command : cmds)
            {
                if (rlCheckRenderBatchLimit(command.index_size))
                {
                    rlBegin(RL_TRIANGLES);
                }

                rlSetTexture(command.texture ? command.texture->id : 0);
                rlSetBlendMode(command.blendmode);

                for (auto i = 0; i < command.index_size; ++i)
                {
                    auto& vertex = vtx[idx[command.index + i]];

                    rlColor4ub(vertex.color.r, vertex.color.g, vertex.color.b, vertex.color.a);
                    rlTexCoord2f(vertex.tex_coord.x, vertex.tex_coord.y);
                    rlVertex2f(vertex.position.x, vertex.position.y);
                }
            }
            rlEnd();
        }
    }


    void Renderer::clear(Color clr)
    {
        ClearBackground({clr.r, clr.g, clr.b, clr.a});
    }

    void Renderer::setDepth(u32 depth)
    {
        if (!_depthsort || _depth == depth)
            return;
        newCommand();
        _depth = depth;
    }

    bool Renderer::beginFrame()
    {
        BeginDrawing();
        return true;
    }

    void Renderer::endFrame()
    {
        EndDrawing();
    }

    u32 Renderer::getDepth() const
    {
        return _depth;
    }

    void Renderer::setTexture(Renderer::Texture* tx)
    {
        if (_command.texture == tx)
            return;
        newCommand();
        _command.texture = tx;
    }

    Renderer::Texture* Renderer::getTexture() const
    {
        return _command.texture;
    }

    void Renderer::setBlendMode(u32 bm)
    {
        if (_command.blendmode == bm)
            return;
        newCommand();
        _command.blendmode = bm;
    }

    u32  Renderer::getBlendMode() const
    {
        return _command.blendmode;
    }

    Renderer::Mesh Renderer::beginMesh(uint32_t vtx, uint32_t idx)
    {
        Mesh msh;
        msh.vertex = &_verts[_command.vertex + _command.vertex_size];
        msh.index  = &_inds[_command.index + _command.index_size];
        return msh;
    }

    void Renderer::endMesh(const Renderer::Mesh& m)
    {
        for (uint32_t n = 0; n < m.index_size; ++n)
            m.index[n] += _command.vertex_size;
        _command.vertex_size += m.vertex_size;
        _command.index_size += m.index_size;
    }

} // namespace box