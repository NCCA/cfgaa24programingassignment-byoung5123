#ifndef CUBE_H_
#define CUBE_H_

#include <ngl/ShaderLib.h>
#include <ngl/Transformation.h>
#include <ngl/Vec3.h>
#include <ngl/Vec4.h>
#include <string>

/// @class Cube
/// @brief Manages the properties and rendering of a cube object in a 3D space.
class Cube
{
public:
    /// Default constructor that initializes the position and color to default values.
    Cube();

    /// Constructor with parameters to set the cube's initial position and color.
    /// @param _position Initial position of the cube as Vec3.
    /// @param _colour Color of the cube as Vec4.
    Cube(ngl::Vec3 _position, ngl::Vec4 _colour);

    /// Gets the position of the cube.
    /// @return The current position as Vec3.
    ngl::Vec3 getPos() const { return m_pos; }

    /// Gets the color of the cube.
    /// @return The current color as Vec4.
    ngl::Vec4 getColour() const { return m_colour; }

    /// Draws the cube using the specified shader and matrices.
    /// @param _shaderName The name of the shader program to use.
    /// @param _globalMat Global transformation matrix.
    /// @param _view View matrix from the camera.
    /// @param _project Projection matrix.
    void draw(const std::string &_shaderName, const ngl::Mat4 &_globalMat,
              const ngl::Mat4 &_view, const ngl::Mat4 &_project);

    /// Updates the position of the cube (to be defined based on how positions are handled, e.g., physics or animation).
    void updatePos();

    /// Loads transformation matrices into the currently active shader.
    /// @param _tx Transformation object containing local transformations.
    /// @param _globalMat Global transformation matrix.
    /// @param _view View matrix from the camera.
    /// @param _project Projection matrix.
    void loadMatricesToShader(ngl::Transformation &_tx, const ngl::Mat4 &_globalMat,
                              const ngl::Mat4 &_view, const ngl::Mat4 &_project);

private:
    ngl::Vec3 m_pos; ///< Position of the cube in 3D space.
    ngl::Vec4 m_colour; ///< Color of the cube.
    ngl::Transformation m_transform; ///< Transformation object for handling local transformations.

    // PBR specific material properties
    ngl::Vec4 m_albedo;  // Base color of the cube.
    float m_metallic;  // Metallic property of the material.
    float m_roughness;  // Roughness property of the material.
    float m_ao;  // Ambient occlusion factor.
};

#endif // CUBE_H_
