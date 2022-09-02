project('NickvisionApplication', ['cpp', 'c'], version: '2022.9.0', meson_version: '>= 0.59.0', default_options: ['warning_level=3', 'werror=false', 'cpp_std=c++20'])

adwaita = dependency('libadwaita-1', version: '>= 1.1.0')

subdir('src')

executable('NickvisionApplication', sources, dependencies: [adwaita], install: true)