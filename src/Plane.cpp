//=============================================================================
//
//   Exercise code for the lecture
//   "Introduction to Computer Graphics"
//   by Prof. Dr. Mario Botsch, Bielefeld University
//
//   Copyright (C) Computer Graphics Group, Bielefeld University.
//
//=============================================================================


//== INCLUDES =================================================================

#include "Plane.h"
#include <float.h>


//== CLASS DEFINITION =========================================================



Plane::Plane(const vec3& _center, const vec3& _normal)
: center(_center), normal(_normal)
{
}


//-----------------------------------------------------------------------------


bool
Plane::
intersect(const Ray& _ray,
          vec3&      _intersection_point,
          vec3&      _intersection_normal,
          vec3&      _intersection_diffuse,
          double&    _intersection_t ) const
{

    _intersection_diffuse = material.diffuse;
/** \todo
 * - compute the intersection of the plane with `_ray`
 * - if ray and plane are parallel there is no intersection
 * - otherwise compute intersection data and store it in `_intersection_point`, `_intersection_normal`, and `_intersection_t`.
 * - return whether there is an intersection for t>1e-5 (avoids "shadow acne").
*/
    
    _intersection_t = 0.0;
    // get denominator
    double denom = dot(this->normal, _ray.direction);
    
    // avoid division by 0 and parallel ray to plane 
    // if normal and ray are perpendicular (dot product 0), then ray is parallel to plane
    if(abs(denom) > 0.000001) {
        // compute intersection
        vec3 diff = this->center - _ray.origin;
        _intersection_t = dot(diff, this->normal) / denom;

        //compute intersection point by putting intersection_t it into the ray equation
        _intersection_point = _ray.origin + _ray.direction * _intersection_t;
        //normal of plane stays the same
        _intersection_normal = this->normal;

        // avoid shadow acne
        if (_intersection_t > 1e-5) {
            return true;
        }

    }

    return false;
}


//=============================================================================
