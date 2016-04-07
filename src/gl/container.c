#include <at/gl.h>
/*===========================================================================
 * PRIVATE API
 *===========================================================================*/
typedef struct _AtGLContainerPrivate{

}AtGLContainerPrivate;

G_DEFINE_TYPE_WITH_PRIVATE(AtGLContainer, at_gl_container, G_TYPE_OBJECT)
static void
at_gl_container_class_init(AtGLContainerClass *klass){

}

static void
at_gl_container_init(AtGLContainer *self){

}
/*===========================================================================
 * PUBLIC API
 *===========================================================================*/
uint8_t
at_gl_container_get_num_children(AtGLContainer* container){
  return 0;
}
