asb_plugins_cargs = ['-DG_LOG_DOMAIN="Asb"']

asb_plugin_dbus = shared_library(
  'libasb_plugin_dbus', 'asb-plugin-dbus.c',
  include_directories : [
    top_build_incdir,
    asbuilder_incdir,
    asglib_incdir,
  ],
  dependencies : [glib, gdkpixbuf, soup],
  c_args : asb_plugins_cargs
)

# TODO: add other plugins
