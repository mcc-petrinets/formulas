// -*- coding: utf-8 -*-
// Copyright (C) 2012, 2013, 2014, 2015 Laboratoire de Recherche et
// Developpement de l'Epita (LRDE).
//
// This file is part of Spot, a model checking library.
//
// Spot is free software; you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
//
// Spot is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
// or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
// License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#pragma once

#include <spot/tl/formula.hh>
#include <unordered_map>

namespace spot
{
  typedef std::unordered_map<formula, formula> snf_cache;

  /// Helper to rewrite a sere in Star Normal Form.
  ///
  /// This should only be called on children of a Star operator.  It
  /// corresponds to the E° operation defined in the following
  /// paper.
  ///
  /** \verbatim
      @Article{          bruggeman.96.tcs,
      author        = {Anne Br{\"u}ggemann-Klein},
      title                = {Regular Expressions into Finite Automata},
      journal        = {Theoretical Computer Science},
      year                = {1996},
      volume        = {120},
      pages                = {87--98}
      }
      \endverbatim */
  ///
  /// \param sere the SERE to rewrite
  /// \param cache an optional cache
  SPOT_API formula
  star_normal_form(formula sere, snf_cache* cache = nullptr);

  /// A variant of star_normal_form() for r[*0..j] where j < ω.
  SPOT_API formula
  star_normal_form_bounded(formula sere, snf_cache* cache = nullptr);
}
