/**
 ** This file is part of the atkv project.
 ** Copyright 2016 Anderson Tavares <nocturne.pe@gmail.com>.
 **
 ** This program is free software: you can redistribute it and/or modify
 ** it under the terms of the GNU General Public License as published by
 ** the Free Software Foundation, either version 3 of the License, or
 ** (at your option) any later version.
 **
 ** This program is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 ** GNU General Public License for more details.
 **
 ** You should have received a copy of the GNU General Public License
 ** along with this program.  If not, see <http://www.gnu.org/licenses/>.
 **/


#include <locale.h>
#include <glib-object.h>
#include <at/gl.h>

/*===========================================================================
 * TEST CASES
 *===========================================================================*/

/*===========================================================================
 * MAIN FUNCTION
 *===========================================================================*/
int
main(int argc, char** argv){
  setlocale(LC_ALL, "");
  g_test_init(&argc, &argv, NULL);
  g_test_bug_base("http://github.com/atkv/atkv/issues/");

//  g_test_add_func("/at_gl/test_at_gl_loader_obj",
//                  test_at_gl_loader_obj);

  return g_test_run();
}
