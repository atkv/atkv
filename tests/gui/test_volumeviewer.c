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
#include <at/gui.h>
#include <at/imgproc.h>

void
mouse_cb(AtMouseEventType event, int x, int y, AtMouseEventFlags flags, void* user_data){

}

void
key_cb(AtKeyEventType event, int key_val, void* user_data){

}

int main(int argc, char** argv){
  char* name = "MRHead.nii.gz";
  if(argc > 1)
    name = argv[2];
  gtk_init(&argc, &argv);
  AtVolumeViewer* volumeviewer = at_volumeviewer_new_with_name("name");

  g_autoptr(AtNiftiImage) nifti_image = at_nifti_image_read(name,TRUE);

  at_volumeviewer_set_mouse_callback(volumeviewer, mouse_cb, NULL);
  at_volumeviewer_set_key_callback  (volumeviewer, key_cb, NULL);
  at_volumeviewer_show_nifti        (volumeviewer, nifti_image);
  at_volumeviewer_waitkey();

  return 0;
}
