Import("env", "projenv")
import shutil

def get_build_flag_value(flag_name):
    build_flags = env.ParseFlags(env['BUILD_FLAGS'])
    flags_with_value_list = [build_flag for build_flag in build_flags.get('CPPDEFINES') if type(build_flag) == list]
    defines = {k: v for (k, v) in flags_with_value_list}
    return defines.get(flag_name)

def after_build(source, target, env):
    print( "Executing custom step " )
    dir    = env.GetLaunchDir()
    #name   = env.get( "PIOENV" )

    target = dir + "/bin/version.json"
    ver = get_build_flag_value("CFG_APPVER")

    print( "Creating version.json" )
    f = open( target, "w" )
    f.write( "{ \"project\":\"pressmon\", \"version\":" + ver + ", " ) 
    #f.write( " \"html\": [ \"index.min.htm\", \"config.min.htm\", \"calibration.min.htm\", \"test.min.htm\", \"format.min.htm\", \"about.min.htm\" ] }" )
    f.write( " \"html\": [ ] }" )
    f.close()


print( "Adding custom build step (create json):")
env.AddPostAction("buildprog", after_build)
