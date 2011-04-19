/* 
 * PROJECT: NyARToolkitCPP
 * --------------------------------------------------------------------------------
 *
 * The NyARToolkitCS is C++ version NyARToolkit class library.
 * 
 * Copyright (C)2008 R.Iizuka
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this framework; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 * 
 * For further information please contact.
 *	http://nyatla.jp/nyatoolkit/
 *	<airmail(at)ebony.plala.or.jp>
 * 
 */
#include "NyARTransOffset.h"

namespace NyARToolkitCPP
{
	void NyARTransOffset::setSquare(double i_width,const TNyARDoublePoint2d& i_center)
	{
		const double w_2 = i_width / 2.0;

		TNyARDoublePoint3d *vertex3d_ptr= this->vertex;
		vertex3d_ptr->x = -w_2;
		vertex3d_ptr->y =  w_2;
		vertex3d_ptr->z = 0.0;
		vertex3d_ptr++;
		vertex3d_ptr->x = w_2;
		vertex3d_ptr->y = w_2;
		vertex3d_ptr->z = 0.0;
		vertex3d_ptr++;
		vertex3d_ptr->x =  w_2;
		vertex3d_ptr->y = -w_2;
		vertex3d_ptr->z = 0.0;
		vertex3d_ptr++;
		vertex3d_ptr->x = -w_2;
		vertex3d_ptr->y = -w_2;
		vertex3d_ptr->z = 0.0;

		this->point.x=-i_center.x;
		this->point.y=-i_center.y;
		this->point.z=0;
		return;
	}
}
