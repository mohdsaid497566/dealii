// ------------------------------------------------------------------------
//
// SPDX-License-Identifier: LGPL-2.1-or-later
// Copyright (C) 2017 - 2020 by the deal.II authors
//
// This file is part of the deal.II library.
//
// Part of the source code is dual licensed under Apache-2.0 WITH
// LLVM-exception OR LGPL-2.1-or-later. Detailed license information
// governing the source code and code contributions can be found in
// LICENSE.md and CONTRIBUTING.md at the top level directory of deal.II.
//
// ------------------------------------------------------------------------


// test for Point::distance_square() for VectorizedArray numbers

#include <deal.II/base/point.h>
#include <deal.II/base/vectorization.h>

#include "../tests.h"


template <int dim, typename number>
void
check()
{
  VectorizedArray<number>             distance_vec;
  Point<dim, VectorizedArray<number>> p1_vec, p2_vec;

  for (unsigned int v = 0; v < VectorizedArray<number>::size(); ++v)
    {
      Point<dim, number> p1, p2;
      for (unsigned int i = 0; i < dim; ++i)
        {
          p1[i] = 10.0 + 0.12345 * i + 0.987 * v;
          p1[i] = 0.5 + 0.6789 * i - 0.543 * v;

          p1_vec[i][v] = p1[i];
          p2_vec[i][v] = p2[i];
        }

      distance_vec[v] = p1.distance_square(p2);
    }

  const VectorizedArray<number> distance_vec2 = p1_vec.distance_square(p2_vec);
  distance_vec -= distance_vec2;
  number diff = 0.;

  for (unsigned int v = 0; v < VectorizedArray<number>::size(); ++v)
    diff += std::abs(distance_vec[v]);

  AssertThrow(diff < 2 * std::numeric_limits<number>::epsilon() *
                       VectorizedArray<number>::size() *
                       VectorizedArray<number>::size(),
              ExcMessage("diff is " + std::to_string(diff)));

  deallog << "Ok" << std::endl;
}

int
main()
{
  initlog();
  deallog << std::setprecision(3);

  check<1, float>();
  check<2, float>();
  check<3, float>();

  check<1, double>();
  check<2, double>();
  check<3, double>();
}
