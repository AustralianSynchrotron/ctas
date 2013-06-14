/******************************************************************************
 *   Copyright (C) 2007 by Anton Maksimenko                                   *
 *   antonmx@post.kek.jp                                                      *
 *                                                                            *
 *   This program is free software; you can redistribute it and/or modify     *
 *   it under the terms of the GNU General Public License as published by     *
 *   the Free Software Foundation; either version 2 of the License, or        *
 *   (at your option) any later version.                                      *
 *                                                                            *
 *   This program is distributed in the hope that it will be useful,          *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of           *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *
 *   GNU General Public License for more details.                             *
 *                                                                            *
 *   You should have received a copy of the GNU General Public License        *
 *   along with this program; if not, write to the                            *
 *   Free Software Foundation, Inc.,                                          *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.                *
 ******************************************************************************/

///
/// @file
/// @author antonmx <antonmx@amx-mac.synchrotron.org.au>
/// @date   Tue Mar 16 15:57:18 2010
///
/// @brief Needed for the all-in-one processing of the absorption experiment.
///


#ifndef _H_ABS_H_
#define _H_ABS_H_

/// \cond
#ifdef _H_EXPERIMENT_H_
/// \endcond

/// \addtogroup experiment
/// @{

/// \brief Class which projections from the input data of the absorption-based experiment.
class ABSexp : public Experiment {
private:

  static const std::string modname;  ///< Name of the module.
  AqSeries list;               ///< Data acquired in the experiment.

public:

  /// Constructor
  ///
  /// @param _list Data acquired in the experiment.
  ///
  inline ABSexp(const AqSeries & _list) :
    Experiment (_list.shape(), _list.thetas()),
    list(_list)
  {
    cntr = Contrast::ABS;
  }

  void projection(int itheta, Map & proj,
                  const std::vector<int> & sliceV,
                  float angle=0, const Crop &crop = Crop()) const {
    list.projection(itheta, proj, sliceV, angle, crop);
  }

};

const std::string ABSexp::modname="ABS experiment";

/// @}

/// \cond
#endif // _H_EXPERIMENT_H_
/// \endcond


#endif
