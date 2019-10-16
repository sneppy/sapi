from distutils.core import setup, Extension

# Setup script
setup(
	name='sapi',
	version='0.1',
	description='API utils by Sneppy',
	author='sneppy',
	author_email='mecchia.andrea@gmail.com',
	ext_modules=[
		Extension('sapi', sources=['src/sapi.c', 'src/edge.c'], include_dirs=['include'])
	]
)