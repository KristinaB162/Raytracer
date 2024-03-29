//=============================================================================
//                                                                            
//   Exercise code for the lecture
//   "Introduction to Computer Graphics"
//   by Prof. Dr. Mario Botsch, Bielefeld University
//
//   Copyright (C) Computer Graphics Group, Bielefeld University.
//                                                                            
//=============================================================================

#ifndef OBJECT_H
#define OBJECT_H


//== INCLUDES =================================================================

#include "Ray.h"
#include "vec3.h"
#include "Material.h"


//== CLASS DEFINITION =========================================================


/// \class Object Object.h
/// This class implements an abstract class for an object.
/// Every derived object type will inherit the material property, and it
/// will have to override the virtual function Object::intersect().
struct Object
{
public:

    /// Default (empty) constructor
    Object() {}

    /// Destructor (if a class has virtual functions, then its destructor
    /// has to be virtual as well).
    virtual ~Object() {}

    /// Intersect the object with \c _ray, return whether there is an intersection.
    /// If \c _ray intersects the object, provide the following results:
    /// \param[in] _ray the ray to intersect the object with
    /// \param[out] _intersection_point the point of intersection
    /// \param[out] _intersection_normal the surface normal at intersection point
    /// \param[out] _intersection_t ray parameter at intersection point
    virtual bool intersect(const Ray&  _ray,
                           vec3&       _intersection_point,
                           vec3&       _intersection_normal,
                           vec3&       _intersection_diffuse,
                           double&     _intersection_t) const = 0;

    /// The material of this object
    Material material;
};


/// This typedef is for convencience only
typedef Object* Object_ptr;


//=============================================================================
#endif // OBJECT_H defined
//=============================================================================
