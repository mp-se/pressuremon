Import("env")
import shutil

def get_build_flag_value(flag_name):
    build_flags = env.ParseFlags(env['BUILD_FLAGS'])
    flags_with_value_list = [build_flag for build_flag in build_flags.get('CPPDEFINES') if type(build_flag) == list]
    defines = {k: v for (k, v) in flags_with_value_list}
    return defines.get(flag_name)

def after_build(source, target, env):
    print( "Executing custom step " )
    dir    = env.GetLaunchDir()
    name   = env.get( "PIOENV" )

    # Pressuremon

    if name == "pressure32s3-release" :
        target = dir + "/bin/firmware32s3.bin"
        source = dir + "/.pio/build/" + name + "/firmware.bin"
        print( "Copy file : " + source + " -> " + target )
        shutil.copyfile( source, target )

        target = dir + "/bin/partitions32s3.bin"
        source = dir + "/.pio/build/" + name + "/partitions.bin"
        print( "Copy file : " + source + " -> " + target )
        shutil.copyfile( source, target )

    elif name == "pressure32s2-release" :
        target = dir + "/bin/firmware32s2.bin"
        source = dir + "/.pio/build/" + name + "/firmware.bin"
        print( "Copy file : " + source + " -> " + target )
        shutil.copyfile( source, target )

        target = dir + "/bin/partitions32s2.bin"
        source = dir + "/.pio/build/" + name + "/partitions.bin"
        print( "Copy file : " + source + " -> " + target )
        shutil.copyfile( source, target )

    elif name == "pressure32c3-release" :
        target = dir + "/bin/firmware32c3.bin"
        source = dir + "/.pio/build/" + name + "/firmware.bin"
        print( "Copy file : " + source + " -> " + target )
        shutil.copyfile( source, target )

        target = dir + "/bin/partitions32c3.bin"
        source = dir + "/.pio/build/" + name + "/partitions.bin"
        print( "Copy file : " + source + " -> " + target )
        shutil.copyfile( source, target )

print( "Adding custom build step (copy firmware): ")
env.AddPostAction("buildprog", after_build)
