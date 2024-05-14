#include "Cube.h"
#include <ngl/VAOPrimitives.h>
#include <ngl/NGLStream.h>
#include <ngl/ShaderLib.h>

// Constructor that initializes the cube's position and color.
Cube::Cube(ngl::Vec3 _pos, ngl::Vec4 _colour)
        : m_pos(_pos), m_colour(_colour) {// Setup default PBR properties, adjust these as necessary
    m_albedo = m_colour;
    m_metallic = 0.5f;
    m_roughness = 0.5f;
    m_ao = 1.0f;}

// Draw method: Renders the cube using specified shader and transformation matrices.
void Cube::draw(const std::string &_shaderName, const ngl::Mat4 &_globalMat,
                const ngl::Mat4 &_view, const ngl::Mat4 &_project)
{
    // Set PBR material properties
    ngl::ShaderLib::setUniform("albedo", m_albedo);
    ngl::ShaderLib::setUniform("metallic", m_metallic);
    ngl::ShaderLib::setUniform("roughness", m_roughness);
    ngl::ShaderLib::setUniform("ao", m_ao);

    // Adjust cube's position to center blocks around origin before transforming.
    m_transform.reset();
    m_transform.setPosition(m_pos - ngl::Vec3(4.5, 0, 0));  // Adjust position
    loadMatricesToShader(m_transform, _globalMat, _view, _project);
    ngl::VAOPrimitives::draw("cube");
}

// Loads the necessary transformation matrices to the currently active shader.
void Cube::loadMatricesToShader(ngl::Transformation &_transform, const ngl::Mat4 &_globalMat,
                                const ngl::Mat4 &_view, const ngl::Mat4 &_project)
{
    ngl::Mat4 MV = _view * _globalMat * _transform.getMatrix();
    ngl::Mat4 MVP = _project * MV;
    ngl::Mat3 normalMatrix(MV);
    normalMatrix.inverse().transpose();

    ngl::ShaderLib::use("PBR");
    ngl::ShaderLib::setUniform("MVP", MVP);
    ngl::ShaderLib::setUniform("normalMatrix", normalMatrix);
    ngl::ShaderLib::setUniform("albedo", m_colour.m_r, m_colour.m_g, m_colour.m_b);
}

// Update position method: Resets and sets the transformation to the current position.
void Cube::updatePos()
{
    m_transform.reset();
    m_transform.setPosition(m_pos);
}
