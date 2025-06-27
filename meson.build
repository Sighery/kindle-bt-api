project(
  'kindle-bt-api',
  'cpp',
  version: 'v1.0.0',
  default_options: ['cpp_std=c++17'],
  meson_version: '>=1.1'
)

sources = files(
    './src/main.cpp',
)

include_dirs = include_directories(
  './src/include/',
)

cc = meson.get_compiler('cpp')

kindle_root = get_option('kindle_root_dir')
acebt_dep = declare_dependency(
  dependencies: cc.find_library('ace_bt', dirs: join_paths(kindle_root, 'usr/lib'), required: true)
)
pthread_dep = declare_dependency(
  dependencies: cc.find_library('pthread', dirs: join_paths(kindle_root, 'usr/lib'), required: true)
)


kindle_bt_api = executable(
  'kindle_bt_api',
  sources,
  include_directories: include_dirs,
  dependencies: [
    acebt_dep,
    pthread_dep,
  ],
  cpp_args: '-static-libstdc++',
  link_args: ['-static-libstdc++'],
)

custom_target(
  'clean up libace_bt hard link',
  depends: kindle_bt_api,
  input: [
    join_paths(kindle_root, 'usr/lib', 'libace_bt.so'),
    kindle_bt_api,
  ],
  output: 'fake',
  command: ['patchelf', '--replace-needed', '@INPUT0@', 'libace_bt.so', '@INPUT1@'],
  build_by_default: true,
)
