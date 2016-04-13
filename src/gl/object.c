#include <at/gl.h>
G_DEFINE_INTERFACE(AtGLObject, at_gl_object, G_TYPE_OBJECT)
static void
at_gl_object_default_init(AtGLObjectInterface *klass){

}

/**
 * @brief Set name of the object
 * @param object
 * @param name
 */
void
at_gl_object_set_name(AtGLObject* object, char* name){
  AtGLObjectInterface *iface;
  g_return_if_fail(AT_IS_GL_OBJECT(object));
  iface = AT_GL_OBJECT_GET_IFACE(object);
  g_return_if_fail(iface->set_name != NULL);
  iface->set_name(object, name);
}

/**
 * @brief Get the object name
 * @param object
 * @return
 */
char*
at_gl_object_get_name(AtGLObject* object){
  AtGLObjectInterface *iface;
  g_return_val_if_fail(AT_IS_GL_OBJECT(object),NULL);
  iface = AT_GL_OBJECT_GET_IFACE(object);
  g_return_val_if_fail(iface->get_name != NULL, NULL);
  iface->get_name(object);
}
