# Sphinx configuration for PressureMon documentation.

project = 'PressureMon'
copyright = '2022-2026, Magnus Persson'
author = 'Magnus Persson'
release = '0.8.0'

extensions = [
    'sphinx_copybutton',
    'sphinx_design',
]

templates_path = ['_templates']
exclude_patterns = ['_build', 'Thumbs.db', '.DS_Store']

html_theme = 'shibuya'
html_static_path = ['_static']
html_css_files = ['custom.css']
html_logo = 'images/pressuremon_logo.png'
html_favicon = 'images/pressuremon_logo.png'
