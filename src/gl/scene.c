#include <at/gl.h>
/*===========================================================================
 * PRIVATE API
 *===========================================================================*/
typedef struct _AtGLScenePrivate{

}AtGLScenePrivate;

G_DEFINE_TYPE_WITH_PRIVATE(AtGLScene, at_gl_scene, G_TYPE_OBJECT)
static void
at_gl_scene_class_init(AtGLSceneClass *klass){

}
static void
at_gl_scene_init(AtGLScene *self){

}

/*===========================================================================
 * PUBLIC API
 *===========================================================================*/
AtGLScene*
at_gl_scene_new(){
  return g_object_new(AT_TYPE_GL_SCENE, NULL);
}

void
at_gl_scene_add_mesh(AtGLScene* scene, AtGLMesh* mesh){

}
