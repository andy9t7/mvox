/*
* Copyright (C) 2020 Benjamin F. Zwick
*
* This file is part of MVox.
*
* MVox is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* MVox is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with MVox.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "mvox/mfemutil.hpp"

#include <cstring>

#include "mvox/fileutil.hpp"    // file_ext

void save_mesh(mfem::Mesh &mesh, const char *filename)
{
   // Create ouput file stream
   std::ostream *ofs;
   if (strcmp(file_ext(filename), "gz") == 0) // compressed MFEM mesh
   {
#ifdef MFEM_USE_ZLIB
      // See https://github.com/mfem/mfem/pull/638/files
      ofs = new mfem::ofgzstream(filename, "zwb9");
#else
      MFEM_ABORT( "Cannot compress file because MFEM was built without ZLIB" );
#endif
   }
   else
   {
      ofs = new std::ofstream (filename, std::ofstream::out);
   }
   ofs->precision(14);

   // Write the mesh to output file stream
   if (strcmp(file_ext(filename), "vtk") == 0)
   {
      mesh.PrintVTK(*ofs);
   }
   else if (strcmp(file_ext(filename), "vtu") == 0)
   {
      // NOTE: Use PrintVTK not PrintVTU because duplicate nodes -> large file size
      // TODO: strip the file extension otherwise filename.vtu.vtu
      mesh.PrintVTU(filename, mfem::VTKFormat::BINARY, false, 0, false);
   }
   else if (strcmp(file_ext(filename), "mesh") == 0) // MFEM mesh
   {
      mesh.Print(*ofs);
   }
   else if (strcmp(file_ext(filename), "gz") == 0) // compressed MFEM mesh
   {
      mesh.Print(*ofs);
   }
   else
   {
      MFEM_ABORT( "Invalid file extension or unkown output file type: " << filename );
   }

   delete ofs;
}
