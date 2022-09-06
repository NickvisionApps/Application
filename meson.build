project('org.nickvision.application', ['cpp', 'c'], version: '2022.9.0', meson_version: '>= 0.59.0', default_options: ['warning_level=3', 'werror=false', 'cpp_std=c++20'])

adwaita = dependency('libadwaita-1', version: '>= 1.1.4')
jsoncpp = dependency('jsoncpp')
curlpp = dependency('curlpp')

subdir('src')

executable('org.nickvision.application', sources, dependencies: [adwaita, jsoncpp, curlpp], install: true)
install_data(resources, install_dir: 'share/icons/hicolor/scalable/apps')
install_data('org.nickvision.application.desktop', install_dir: 'share/applications')
install_data('org.nickvision.application.metainfo.xml', install_dir: 'share/metainfo')