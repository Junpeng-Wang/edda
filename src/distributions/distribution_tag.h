// Copyright 2015 The Edda Authors. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef DISTRIBUTION_TAG_H_
#define DISTRIBUTION_TAG_H_

/// Distributions:
/// Define generic functions, tags
///

#include <iostream>
#include <typeinfo>
#include <iostream>

#include "common.h"
#include "edda_export.h"
#include "core/vector_matrix.h"
#include "core/tuple.h"
#include "core/thrust_common.h"

namespace edda {
namespace dist {

///
/// \brief The Distribution class is a root class for all distribution-type classes.
///
/// This is useful for applications to identify whether a class is a distribution-type class, by using ENABLE_IF_BASE_OF()
///
class EDDA_EXPORT DistributionTag {
    void NotImpError() const {std::cerr << "Functions in this class should be overloaded."; throw NotImplementedException(); }
};

class EDDA_EXPORT ContinuousDistributionTag : public DistributionTag{
};

class EDDA_EXPORT DiscreteDistributionTag : public DistributionTag{
};

// Here are generic functions that can be reused by new distributions if not
// implemented.

///
/// \brief random variable -=
///
template<class T, ENABLE_IF_BASE_OF(T, DistributionTag) >
inline T operator-=(const T& lhs, const T& rhs) {
    T h(lhs);
      return h += (-rhs);
}

///
/// \brief random variable +
///
template<class T, ENABLE_IF_BASE_OF(T, DistributionTag) >
inline T operator+(const T& lhs, const T& rhs) {
    T h(lhs);
    return h += rhs;
}

///
/// \brief random variable -
///
template<class T, ENABLE_IF_BASE_OF(T, DistributionTag) >
inline T operator-(const T& lhs, const T& rhs) {
    T h(lhs);
    return h -= rhs;
}

///
/// \brief random variable *
///
template<class T, ENABLE_IF_BASE_OF(T, DistributionTag) >
inline T operator*(const T& lhs, const double x) {
    T h(lhs);
    return h *= x;
}

///
/// \brief Return a vector sample from a vector of distributions
///
template <typename T, int N, ENABLE_IF_BASE_OF(T, DistributionTag)>
__host__ __device__
inline Vector<Real, N> getSample(const Tuple<T, N> &distvec, thrust::default_random_engine &rng)
{
  Vector<Real, N> out;
  for (int i=0; i<N; i++)
    out[i] = getSample(distvec[i], rng);
  return out;
}

///
/// \brief Get vector mean
///
template <class T, int N, ENABLE_IF_BASE_OF(T, DistributionTag) >
inline Vector<double, N> getMean(const Tuple<T, N> &dist)
{
    Vector<double,N> result;
    for (int i=0; i<N; i++)
        result[i] = getMean(dist[i]);
    return result;
}

/////////////////////////////////////////////////////////
/// Degenerated functions for Real type
inline double getPdf(const Real& value, double x) { return (x==value)?1.:0; }
inline double getCdf(const Real& value, double x) { return (x<=value)?1.:0; }
inline double getMean(const Real& value) { return value; }
inline double getVar(const Real& value) { return 0; }
inline double getSample(const Real& value) { return value; }
inline const char *getName(const Real &x) { return "Real"; }
}  // namespace dist
}  // namespace edda

#endif // DISTRIBUTION_TAG_H_
