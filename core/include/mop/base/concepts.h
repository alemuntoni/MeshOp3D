// MeshOp3D
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef MOP_CORE_BASE_CONCEPTS_H
#define MOP_CORE_BASE_CONCEPTS_H

#include <vclib/base.h>

#include <concepts>

namespace mop {

namespace detail {

// true if T is a shared_ptr, and the type pointed by T is a base of D
template<typename T, typename D>
concept IsSharedPtrOfBaseOf =
    vcl::IsSharedPointer<T> &&
    std::derived_from<D, std::remove_pointer_t<typename T::element_type>>;

} // namespace detail

/**
 * @brief Concept that is evaluated true if T is a cloneable object.
 *
 * A cloneable object is an object that can be cloned by calling the method
 * `clone()`, which returns a shared pointer to the cloned object. The type of
 * the shared pointer returned by `clone()` must be a shared pointer to a base
 * class of T.
 *
 * Cloneable objects are useful when polymorphism is needed, and the object
 * needs to be copied without knowing the exact type of the object:
 *
 * @code
 * std::shared_ptr<Shape> circle = std::make_shared<Circle>();
 *
 * std::shared_ptr<Shape> clonedCircle = circle->clone();
 * @endcode
 *
 * @tparam T The type to be checked if it is cloneable.
 *
 * @ingroup util_concepts
 */
template<typename T>
concept Cloneable = requires (T&& obj) {
    { obj.clone() } -> detail::IsSharedPtrOfBaseOf<std::remove_cvref_t<T>>;
};

} // namespace mop

#endif // MOP_CORE_BASE_CONCEPTS_H
