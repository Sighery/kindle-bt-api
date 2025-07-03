project(
  'kindle-bt-api',
  'c',
  version: 'v1.0.0',
  default_options: [],
  meson_version: '>=1.1'
)

sources = files(
    './src/main.c',
)

include_dirs = include_directories(
  './src/include/',
)

acebt_dep = dependency('ace_bt')
ace_osal = dependency('ace_osal')
pthread_dep = dependency('pthread')

kindle_bt_api = executable(
  'kindle_bt_api',
  sources,
  include_directories: include_dirs,
  dependencies: [
    acebt_dep,
    ace_osal,
    pthread_dep,
  ],
  c_args: '-static-libgcc',
  link_args: ['-static-libgcc'],
)

kindle_root = get_option('kindle_root_dir')

# ACE libraries don't have soname so meson will create hardlinks
cleanup_ace_bt = custom_target(
  'clean up libace_bt hard link',
  depends: kindle_bt_api,
  input: [
    join_paths(kindle_root, 'usr/lib', 'libace_bt.so'),
    kindle_bt_api,
  ],
  output: 'fake1',
  command: ['patchelf', '--replace-needed', '@INPUT0@', 'libace_bt.so', '@INPUT1@'],
  build_by_default: true,
)

cleanup_ace_osal = custom_target(
  'clean up libace_osal hard link',
  depends: cleanup_ace_bt,
  input: [
    join_paths(kindle_root, 'usr/lib', 'libace_osal.so'),
    kindle_bt_api,
  ],
  output: 'fake2',
  command: ['patchelf', '--replace-needed', '@INPUT0@', 'libace_osal.so', '@INPUT1@'],
  build_by_default: true,
)
